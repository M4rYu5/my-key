#if LINUX
#define PLATFORM_VALUE(linux_val, windows_val) linux_val
#elif WINDOWS
#define PLATFORM_VALUE(linux_val, windows_val) windows_val
#endif