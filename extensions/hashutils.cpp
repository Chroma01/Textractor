#include "hashutils.h"

#include <wincrypt.h>

std::string Md5(const std::string& text)
{
	HCRYPTPROV provider = 0;
	HCRYPTHASH hash = 0;
	BYTE digest[16] = {};
	DWORD digestSize = sizeof(digest);
	std::string result;

	auto cleanup = [&]
	{
		if (hash) CryptDestroyHash(hash);
		if (provider) CryptReleaseContext(provider, 0);
	};

	if (!CryptAcquireContextW(&provider, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) return result;
	if (!CryptCreateHash(provider, CALG_MD5, 0, 0, &hash))
	{
		cleanup();
		return result;
	}
	if (!CryptHashData(hash, reinterpret_cast<const BYTE*>(text.data()), static_cast<DWORD>(text.size()), 0))
	{
		cleanup();
		return result;
	}
	if (!CryptGetHashParam(hash, HP_HASHVAL, digest, &digestSize, 0))
	{
		cleanup();
		return result;
	}
	cleanup();

	constexpr char hex[] = "0123456789abcdef";
	result.reserve(digestSize * 2);
	for (DWORD i = 0; i < digestSize; ++i)
	{
		result.push_back(hex[digest[i] >> 4]);
		result.push_back(hex[digest[i] & 0x0F]);
	}
	return result;
}

std::string Md5Raw(const std::string& text)
{
	HCRYPTPROV provider = 0;
	HCRYPTHASH hash = 0;
	BYTE digest[16] = {};
	DWORD digestSize = sizeof(digest);
	std::string result;

	auto cleanup = [&]
	{
		if (hash) CryptDestroyHash(hash);
		if (provider) CryptReleaseContext(provider, 0);
	};

	if (!CryptAcquireContextW(&provider, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) return result;
	if (!CryptCreateHash(provider, CALG_MD5, 0, 0, &hash))
	{
		cleanup();
		return result;
	}
	if (!CryptHashData(hash, reinterpret_cast<const BYTE*>(text.data()), static_cast<DWORD>(text.size()), 0))
	{
		cleanup();
		return result;
	}
	if (!CryptGetHashParam(hash, HP_HASHVAL, digest, &digestSize, 0))
	{
		cleanup();
		return result;
	}
	cleanup();

	// Return raw bytes as string
	result.assign(reinterpret_cast<char*>(digest), digestSize);
	return result;
}

std::string Base64Encode(const BYTE* data, DWORD size)
{
	DWORD encodedSize = 0;
	if (!CryptBinaryToStringA(data, size, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, nullptr, &encodedSize))
		return "";

	std::string result(encodedSize, '\0');
	if (!CryptBinaryToStringA(data, size, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, result.data(), &encodedSize))
		return "";

	if (!result.empty() && result.back() == '\0')
		result.pop_back();
	return result;
}

std::string Base64Encode(const std::string& data)
{
	return Base64Encode(reinterpret_cast<const BYTE*>(data.data()), static_cast<DWORD>(data.size()));
}

std::string HmacSha1(const std::string& key, const std::string& data)
{
	HCRYPTPROV provider = 0;
	HCRYPTHASH hash = 0;
	HCRYPTKEY cryptKey = 0;
	BYTE digest[20] = {}; // SHA1 produces 20 bytes
	DWORD digestSize = sizeof(digest);
	std::string result;

	auto cleanup = [&]
	{
		if (hash) CryptDestroyHash(hash);
		if (cryptKey) CryptDestroyKey(cryptKey);
		if (provider) CryptReleaseContext(provider, 0);
	};

	if (!CryptAcquireContextW(&provider, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		return result;
	}

	// Create HMAC key structure
	struct {
		BLOBHEADER header;
		DWORD keySize;
		BYTE keyData[1]; // Variable length
	} *keyBlob = nullptr;

	DWORD blobSize = sizeof(BLOBHEADER) + sizeof(DWORD) + key.size();
	keyBlob = (decltype(keyBlob))malloc(blobSize);
	if (!keyBlob)
	{
		cleanup();
		return result;
	}

	keyBlob->header.bType = PLAINTEXTKEYBLOB;
	keyBlob->header.bVersion = CUR_BLOB_VERSION;
	keyBlob->header.reserved = 0;
	keyBlob->header.aiKeyAlg = CALG_RC2;
	keyBlob->keySize = static_cast<DWORD>(key.size());
	memcpy(keyBlob->keyData, key.data(), key.size());

	if (!CryptImportKey(provider, (BYTE*)keyBlob, blobSize, 0, CRYPT_IPSEC_HMAC_KEY, &cryptKey))
	{
		free(keyBlob);
		cleanup();
		return result;
	}
	free(keyBlob);

	if (!CryptCreateHash(provider, CALG_HMAC, cryptKey, 0, &hash))
	{
		cleanup();
		return result;
	}

	// Set HMAC algorithm to SHA1
	HMAC_INFO hmacInfo = {};
	hmacInfo.HashAlgid = CALG_SHA1;
	if (!CryptSetHashParam(hash, HP_HMAC_INFO, (BYTE*)&hmacInfo, 0))
	{
		cleanup();
		return result;
	}

	if (!CryptHashData(hash, reinterpret_cast<const BYTE*>(data.data()), static_cast<DWORD>(data.size()), 0))
	{
		cleanup();
		return result;
	}

	if (!CryptGetHashParam(hash, HP_HASHVAL, digest, &digestSize, 0))
	{
		cleanup();
		return result;
	}

	cleanup();

	// Return raw bytes as string
	result.assign(reinterpret_cast<char*>(digest), digestSize);
	return result;
}

