// RE'd with ghidra combined with sources included in VS2022 (probably works, untested. Also seems to be present in ntdllp.lib)

#define PHNT_VERSION PHNT_WIN11
#include <phnt_windows.h>
#include <phnt.h>

extern void __security_check_cookie(UINT_PTR Cookie);

void __GSHandlerCheckCommon(void *EstablisherFrame, void *DispatcherContext, unsigned int *GSHandlerData)

{
  unsigned long long uVar1;
  void *pvVar2;

  DbgPrint("__GSHandlerCheckCommon(EstablisherFrame = 0x%llX, DispatcherContext = 0x%llX, GSHandlerData = 0x%llX)\n",
		  EstablisherFrame, DispatcherContext, GSHandlerData);
  
                    /* WARNING: Load size is inaccurate */
                    /* WARNING: Load size is inaccurate */
  pvVar2 = EstablisherFrame;
  if ((*GSHandlerData & 4) != 0) {
    pvVar2 = (void *)((long long)*(int *)((long long)GSHandlerData + 4) + (long long)EstablisherFrame &
                     (long long)-*(int *)((long long)GSHandlerData + 8));
  }
  uVar1 = (unsigned long long)*(unsigned int *)(*(long long *)((long long)DispatcherContext + 0x10) + 8);
  if ((*(unsigned char *)(uVar1 + 3 + *(long long *)((long long)DispatcherContext + 8)) & 0xf) != 0) {
    EstablisherFrame =
         (void *)((long long)EstablisherFrame +
                 (unsigned long long)
                 (*(unsigned char *)(uVar1 + 3 + *(long long *)((long long)DispatcherContext + 8)) & 0xfffffff0
                 ));
  }
  __security_check_cookie
            ((unsigned long long)EstablisherFrame ^
             *(unsigned long long *)((long long)(int)(*GSHandlerData & 0xfffffff8) + (long long)pvVar2));
  return;
}


unsigned char __GSHandlerCheck(unsigned char ExceptionRecord,void *EstablisherFrame,unsigned char ContextRecord,void *DispatcherContext)

{

  DbgPrint("__GSHandlerCheck(ExceptionRecord = 0x%llX, EstablisherFrame = 0x%llX, ContextRecord = 0x%llX, DispatcherContext = 0x%llX)\n",
		  ExceptionRecord, EstablisherFrame, ContextRecord, DispatcherContext);

  __GSHandlerCheckCommon(EstablisherFrame,DispatcherContext,*(void **)((long long)DispatcherContext + 0x38));
  return '\x01';
}



