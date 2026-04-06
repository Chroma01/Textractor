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

