#include "support.h"


#ifdef _WIN64
#define SECURITY_COOKIE 0x6942069420694206
#else
#define SECURITY_COOKIE 0x69420694
#endif

UINT_PTR __security_cookie;
UINT_PTR __security_cookie_complement;

void __security_init_cookie(void)
{
    __security_cookie = SECURITY_COOKIE;
    DbgPrint("__security_cookie initialized to 0x%llX\n", __security_cookie);
}

void __security_check_cookie(UINT_PTR Cookie)
{
    ASSERT(Cookie == __security_cookie == SECURITY_COOKIE);
}
