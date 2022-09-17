#include "support.h"

void mainCRTStartup(void)
{
    NTSTATUS Status;
    PTEB Teb;
    PPEB Peb;
    PS_CREATE_INFO ProcessInfo;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    UNICODE_STRING ProcessImageName;
    PPS_ATTRIBUTE_LIST ProcessAttributes;
    HANDLE ProcessHandle;
    HANDLE ThreadHandle;

    __security_init_cookie();

    Status = STATUS_SUCCESS;
    Teb = NtCurrentTeb();
    Peb = Teb->ProcessEnvironmentBlock;
    DbgPrint("TEB is at 0x%llX\nPEB is at 0x%llX\n", Teb, Peb);

    DbgPrint("Filling process creation information structures\n");

    RtlSecureZeroMemory(&ProcessInfo, sizeof(PS_CREATE_INFO));

    RtlInitUnicodeString(&ProcessImageName, (PWSTR)L"\\??\\C:\\Windows\\System32\\calc.exe");
    ASSERT(NT_SUCCESS(RtlCreateProcessParametersEx(&ProcessParameters, &ProcessImageName,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, RTL_USER_PROC_PARAMS_NORMALIZED)));

    ProcessAttributes = RtlAllocateHeap(RtlProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PS_ATTRIBUTE_LIST));
    ProcessAttributes->Attributes[0].Attribute = PS_ATTRIBUTE_IMAGE_NAME;
    ProcessAttributes->Attributes[0].Size = ProcessImageName.Length;
    ProcessAttributes->Attributes[0].Value = (ULONG_PTR)ProcessImageName.Buffer;
    ProcessAttributes->TotalLength = sizeof(PS_ATTRIBUTE_LIST);

    ProcessInfo.State = PsCreateInitialState;
    ProcessInfo.Size = sizeof(PS_CREATE_INFO);

    DbgPrint("RTL_USER_PROCESS_PARAMETERS:\n"
        "\tFlags: 0x%08lX\n"
        "\tImagePathName: %ls\n"
        "\tCommandLine: %ls\n"
        "\tLength: %lu\n"
        "\tMaximumLength: %lu\n"
        "PS_ATTRIBUTE_LIST:\n"
        "\tAttributes[0]:\n"
        "\t\tAttribute: 0x%08lX\n"
        "\t\tSize: %lu\n"
        "\t\tValue: %ls\n"
        "\tTotalLength: %lu\n"
        "PS_CREATE_INFO:\n"
        "\tState: 0x%08lX\n"
        "\tSize: %lu\n",
        ProcessParameters->Flags, ProcessParameters->ImagePathName.Buffer, ProcessParameters->CommandLine.Buffer,
        ProcessParameters->Length, ProcessParameters->MaximumLength, ProcessAttributes->Attributes[0].Attribute,
        ProcessAttributes->Attributes[0].Size, ProcessAttributes->Attributes[0].Value, ProcessAttributes->TotalLength,
        ProcessInfo.State, ProcessInfo.Size);

    DbgPrint("Calling NtCreateUserProcess(ProcessHandle = 0x%llX, ThreadHandle = 0x%llX, ProcessDesiredAccess = 0x%lX,\n"
        "\t\t\t\t\t\t\tThreadDesiredAccess = 0x%lX, ProcessObjectAttributes = 0x%llX, ThreadObjectAttributes = 0x%llX,\n"
        "\t\t\t\t\t\t\tProcessFlags = 0x%lX, ThreadFlags = 0x%lX, ProcessParameters = 0x%llX, CreateInfo = 0x%llX,\n"
        "\t\t\t\t\t\t\tAttributeList = 0x%llX)\n", &ProcessHandle, &ThreadHandle, PROCESS_ALL_ACCESS, THREAD_ALL_ACCESS,
        NULL, NULL, 0, 0, ProcessParameters, &ProcessInfo, ProcessAttributes);
    Status = NtCreateUserProcess(&ProcessHandle, &ThreadHandle, PROCESS_ALL_ACCESS, THREAD_ALL_ACCESS, NULL, NULL, 0,
        0, ProcessParameters, &ProcessInfo, ProcessAttributes);
    if (!NT_SUCCESS(Status))
    {
        DbgPrint("NtCreateUserProcess failed with status 0x%lX (PS_CREATE_INFO.State = 0x%08lX)\n", Status, ProcessInfo.State);
        goto Cleanup;
    }

    DbgPrint("NtCreateUserProcess succeeded, process 0x%llX, thread 0x%llX\n", ProcessHandle, ThreadHandle);

Cleanup:
    DbgPrint("Cleaning up\n");
    RtlDestroyProcessParameters(ProcessParameters);
    RtlFreeHeap(RtlProcessHeap(), 0, ProcessAttributes);
    NtClose(ThreadHandle);
    NtClose(ProcessHandle);
    NtTerminateProcess(NtCurrentProcess(), Status);
}

