// Chinese (Simplified) translations

NATIVE_LANGUAGE = "Chinese (Simplified)";
ATTACH = u8"附加到游戏";
LAUNCH = u8"启动游戏";
CONFIG = u8"配置游戏";
DETACH = u8"从游戏分离";
FORGET = u8"移除游戏";
ADD_HOOK = u8"添加钩子";
REMOVE_HOOKS = u8"移除钩子";
SAVE_HOOKS = u8"保存钩子";
SEARCH_FOR_HOOKS = u8"搜索钩子";
SETTINGS = u8"设置";
EXTENSIONS = u8"扩展";
SELECT_PROCESS = u8"选择进程";
ATTACH_INFO = u8R"(如果没看见想要附加的进程，请尝试使用管理员权限运行Textractor
也可以手动输入进程ID)";
SELECT_PROCESS_INFO = u8"如果手动输入游戏名，请使用绝对路径";
FROM_COMPUTER = u8"从资源管理器中选择";
PROCESSES = u8"可执行文件 (*.exe)";
SAVE_SETTINGS = u8"保存设置";
EXTEN_WINDOW_INSTRUCTIONS = u8R"(在列表中单击右键以添加或移除扩展
在列表中拖拽扩展可以调整插件的执行顺序, 扩展会从上往下依次执行
排在下方的扩展只能接收到由排在上方的扩展处理过后的文本)";
ADD_EXTENSION = u8"添加扩展";
REMOVE_EXTENSION = u8"移除扩展";
INVALID_EXTENSION = u8"%1 是一个无效扩展";
CONFIRM_EXTENSION_OVERWRITE = u8"此扩展的另一个版本已启用，是否删除并覆盖它?";
EXTENSION_WRITE_ERROR = u8"保存扩展失败";
USE_JP_LOCALE = u8"模拟日本区域设置?";
FAILED_TO_CREATE_CONFIG_FILE = u8"无法创建配置文件 \"%1\"";
HOOK_SEARCH_UNSTABLE_WARNING = u8"搜索钩子的功能是不稳定的! 可能会导致你的游戏崩溃! ";
HOOK_SEARCH_STARTING_VIEW_CONSOLE = u8"正在初始化钩子搜索 - 请查看控制台以获取更多提示";
SEARCH_CJK = u8"搜索中文/日文/韩文";
SEARCH_PATTERN = u8"搜索匹配特征 (hex byte array)";
SEARCH_DURATION = u8"搜索持续时间 (ms)";
SEARCH_MODULE = u8"搜索指定模块";
PATTERN_OFFSET = u8"相对于特征地址的偏移值";
MAX_HOOK_SEARCH_RECORDS = u8"搜索结果达到上限";
MIN_ADDRESS = u8"起始地址 (hex)";
MAX_ADDRESS = u8"结束地址 (hex)";
STRING_OFFSET = u8"字符串偏移值 (hex)";
HOOK_SEARCH_FILTER = u8"结果必须匹配的正则表达式";
TEXT = u8"文本";
CODEPAGE = u8"代码页";
SEARCH_FOR_TEXT = u8"搜索指定文本";
START_HOOK_SEARCH = u8"开始搜索钩子";
SAVE_SEARCH_RESULTS = u8"保存搜索结果";
TEXT_FILES = u8"文本文档 (*.txt)";
DOUBLE_CLICK_TO_REMOVE_HOOK = u8"双击移除不需要的钩子";
FILTER_REPETITION = u8"过滤重复文本";
AUTO_ATTACH = u8"自动附加";
ATTACH_SAVED_ONLY = u8"自动附加 (仅限保存过配置的游戏)";
SHOW_SYSTEM_PROCESSES = u8"显示系统进程";
DEFAULT_CODEPAGE = u8"默认代码页";
FLUSH_DELAY = u8"刷新延迟";
MAX_BUFFER_SIZE = u8"最大缓冲区长度";
MAX_HISTORY_SIZE = u8"最大缓存文本长度";
CONFIG_JP_LOCALE = u8"模拟日本区域启动游戏";
CONSOLE = L"控制台";
CLIPBOARD = L"剪贴板";
ABOUT = L"Textractor " ARCH L" v" VERSION LR"( 作者: Artikash (email: akashmozumdar@gmail.com)
项目主页: https://github.com/Artikash/Textractor
教程视频: https://github.com/Artikash/Textractor/blob/master/docs/TUTORIAL.md
如果有任何关于 Textractor 的困难，功能请求或问题，请联系我
可以通过项目主页 (Issues 页面) 或通过邮件来联系
项目主页提供基于 GPLv3 协议的源代码)";
UPDATE_AVAILABLE = L"有可用的更新: 请从 https://github.com/Artikash/Textractor/releases 下载";
ALREADY_INJECTED = L"Textractor: 已经注入";
NEED_32_BIT = L"Textractor: 架构不匹配: 请尝试使用32位版本的Textractor";
NEED_64_BIT = L"Textractor: 架构不匹配: 请尝试使用64位版本的Textractor";
INJECT_FAILED = L"Textractor: 无法注入";
LAUNCH_FAILED = L"Textractor: 无法启动";
INVALID_CODE = L"Textractor: 无效代码";
INVALID_CODEPAGE = L"Textractor: 无法转换文本 (无效的代码页?)";
PIPE_CONNECTED = u8"Textractor: 管道已连接";
INSERTING_HOOK = u8"Textractor: 注入钩子: %s";
REMOVING_HOOK = u8"Textractor: 移除钩子: %s";
HOOK_FAILED = u8"Textractor: 钩子注入失败";
TOO_MANY_HOOKS = u8"Textractor: 钩子数量已达上限: 无法注入";
HOOK_SEARCH_STARTING = u8"Textractor: 开始搜索钩子";
HOOK_SEARCH_INITIALIZING = u8"Textractor: 初始化钩子搜索 (%f%%)";
NOT_ENOUGH_TEXT = u8"Textractor: 文本长度不足, 无法精确搜索";
HOOK_SEARCH_INITIALIZED = u8"Textractor: 搜索初始化完成, 创建了 %zd 个钩子";
MAKE_GAME_PROCESS_TEXT = u8"Textractor: 请点击游戏区域, 在接下来的 %d 秒内使游戏强制处理文本";
HOOK_SEARCH_FINISHED = u8"Textractor: 钩子搜索完毕, 找到了 %d 条结果";
OUT_OF_RECORDS_RETRY = u8"Textractor: 搜索结果已达上限, 如果结果不理想, 请重试(默认最大记录数增加)";
FUNC_MISSING = u8"Textractor: 函数不存在";
MODULE_MISSING = u8"Textractor: 模块不存在";
GARBAGE_MEMORY = u8"Textractor: 内存一直在变，读了也没用";
SEND_ERROR = u8"Textractor: Sender 错误 (H码可能不正确)";
READ_ERROR = u8"Textractor: Reader 错误 (R码可能不正确)";
HIJACK_ERROR = u8"Textractor: Hijack 错误";
COULD_NOT_FIND = u8"Textractor: 无法找到文本";
TRANSLATE_TO = u8"翻译为";
TRANSLATE_FROM = u8"原文语言";
FILTER_GARBAGE = u8"过滤无意义字符";
TRANSLATE_SELECTED_THREAD_ONLY = u8"仅翻译当前选择的文本线程";
RATE_LIMIT_ALL_THREADS = u8"限制请求频率";
RATE_LIMIT_SELECTED_THREAD = u8"限制选中线程的请求频率";
USE_TRANS_CACHE = u8"使用缓存的翻译结果";
MAX_TRANSLATIONS_IN_TIMESPAN = u8"单位时间内最大请求次数";
TIMESPAN = u8"单位时间长度 (ms)";
TOO_MANY_TRANS_REQUESTS = L"超出频率限制: 拒绝发出翻译请求";
TRANSLATION_ERROR = L"翻译时出错";
USE_PREV_SENTENCE_CONTEXT = u8"使用之前的句子作为上下文";
API_KEY = u8"API key";
CHROME_LOCATION = u8"Google Chrome 安装位置";
START_DEVTOOLS = u8"启动 DevTools";
STOP_DEVTOOLS = u8"停止 DevTools";
HIDE_CHROME = u8"隐藏 Chrome 窗口";
DEVTOOLS_STATUS = u8"DevTools 状态";
AUTO_START = u8"自动启动";
ERROR_START_CHROME = L"无法启动或连接到 Chrome";
EXTRA_WINDOW_INFO = u8R"(在此点击右键以修改设置
在窗口边缘点击并拖拽来移动, 在窗口右下角点击并拖拽来调整大小)";
MAX_SENTENCE_SIZE = u8"最大文本长度";
TOPMOST = u8"窗口总是置顶";
DICTIONARY = u8"字典";
SHOW_ORIGINAL = u8"显示原文";
SHOW_ORIGINAL_INFO = u8R"(原文将被隐藏
仅当此扩展位于翻译扩展之后使用时才有效)";
ORIGINAL_AFTER_TRANSLATION = u8"把原文显示在翻译文本的后面";
SIZE_LOCK = u8"锁定窗口大小";
POSITION_LOCK = u8"锁定窗口位置";
CENTERED_TEXT = u8"居中显示文本";
AUTO_RESIZE_WINDOW_HEIGHT = u8"自动改变窗口高度";
CLICK_THROUGH = u8"点击穿透\tAlt+X";
HIDE_MOUSEOVER = u8"鼠标经过时隐藏窗口";
OPACITY = u8"透明度";
BG_COLOR = u8"背景颜色";
TEXT_COLOR = u8"文本颜色";
TEXT_OUTLINE = u8"文字描边";
OUTLINE_COLOR = u8"文字描边颜色";
OUTLINE_SIZE = u8"文字描边大小";
OUTLINE_SIZE_INFO = u8"描边的单位为px (建议保持在字体大小的20%以下)";
FONT = u8"字体";
LUA_INTRO = u8R"(--[[
ProcessSentence 是Textractor每接收到一句文本时都会调用的函数.
参数 sentence:  Textractor接收到的文本 (UTF-8), 数据类型为 string.
参数 sentenceInfo: 用于保存文本相关信息的表, 数据类型为 tabel.

如果你返回一个字符串，文本将被转换为该字符串.
如果返回 nil, 该文本将不会被修改.

此扩展使用了几个Lua解释器的副本用于保证线程安全.
在 ProcessSentence 函数中对全局变量的修改可能不会生效.

sentenceInfo 表有以下关键字索引:
current select: 除非文本属于用户当前选择的文本线程, 否则为0. 数据类型为 number.
process id: 这句文本所属的进程ID. 0 表示控制台与剪贴板线程. 数据类型为 number.
text number: 当前选择的文本线程的ID. 这是在创建文本线程时自动递增的计数器. 0 为控制台, 1 为剪贴板. 数据类型为 number.
--]]

function ProcessSentence(sentence, sentenceInfo)
  --在此处添加你的代码...
end)";
LOAD_SCRIPT = u8"加载脚本";
LUA_ERROR = L"Lua 错误";
REGEX_FILTER = u8"正则表达式过滤器";
INVALID_REGEX = u8"无效的正则表达式";
CURRENT_FILTER = u8"当前过滤中: %1";
REPLACER_INSTRUCTIONS = LR"(使用"Replace"扩展时会使用此文件
替换指令必须遵循以下格式:
|ORIG|原文|BECOMES|替代文本|END|
此文件中替换指令之外的所有文本都会被忽略.
符号 ^ 可以代替任意一个字符.
原文中的空白字符将被忽略，但是替代文本可以包含空白字符、换行符等.
此文件必须使用 Unicode 编码 (UTF-16 little endian).)";
REGEX_REPLACER_INSTRUCTIONS = LR"(使用"Regex Replace"扩展时会使用此文件
替换指令必须遵循以下格式:
|REGEX|正则表达式|BECOMES|替代文本|MODIFIER|修饰符|END|
替代文本可以通过 $[编号] 来引用捕获分组 (例如: $1 引用第1个捕获分组).
修饰符可以包含以下选项:
g 全局替换.
i 忽略大小写差异.
如果替代文本为空, 那么只会替换第1个匹配的到结果, 而且强制启用大小写敏感.
此文件中替换指令之外的所有文本都会被忽略.
此文件必须使用 Unicode 编码 (UTF-16 little endian).
你可以通过下列链接来学习编写和在线测试正则表达式:
  https://www.runoob.com/regexp/regexp-intro.html
  https://regexr.com/)";
THREAD_LINKER = u8"线程链接器";
LINK = u8"链接";
UNLINK = u8"断开";
THREAD_LINK_FROM = u8"需要链接的线程ID";
THREAD_LINK_TO = u8"链接到的线程ID";
HEXADECIMAL = u8"十六进制";
