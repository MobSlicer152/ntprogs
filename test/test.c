#include "support.h"

void mainCRTStartup(void)
{
    NTSTATUS Status;
    PTEB Teb;
    PPEB Peb;

    __security_init_cookie();

    Status = STATUS_SUCCESS;
    Teb = NtCurrentTeb();
    Peb = Teb->ProcessEnvironmentBlock;
    DbgPrint("TEB is at 0x%llX\nPEB is at 0x%llX\n", Teb, Peb);

Cleanup:
    DbgPrint("Cleaning up\n");
    NtTerminateProcess(NtCurrentProcess(), Status);
}

