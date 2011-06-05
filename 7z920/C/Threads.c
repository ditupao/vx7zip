/* Threads.c -- multithreading library
2009-09-20 : Igor Pavlov : Public domain */

#ifndef _WIN32_WCE
//#include <process.h>
#endif



#include "Threads.h"
//#include "common/mywindows.h"
#include "vxWorks/cs/VxWorksApi.h"
#include "MyWinPlatform.h"



extern MY_HANDLE_OBJECT g_HandleObject[MAX_HANDLE_OBJECT_COUNT];


static WRes GetError()
{
  int res = GetLastError();
  return (res) ? (WRes)(res) : 1;
}

WRes HandleToWRes(MYHANDLE h) { return (h != 0) ? 0 : GetError(); }
WRes BOOLToWRes(BOOL v) { return v ? 0 : GetError(); }

WRes HandlePtr_Close(MYHANDLE *p)
{
  if (*p != NULL)
    if (!MyCloseHandle(*p))
      return GetError();
  *p = NULL;
  
  return 0;
}

WRes Handle_WaitObject(MYHANDLE h) 
{ 
#if 0
    int index = (int)h - 1;

    if(g_HandleObject[index].thread_wait_tab.semId == NULL)
    {
        printf("Can NOT Find The Object For Waiting!\n");
        return 0;
    }

    semTake(g_HandleObject[index].thread_wait_tab.semId, WAIT_FOREVER);    
#endif
    return 0;
}

int thread_proc(THREAD_FUNC_TYPE func, LPVOID param, int tab_handle)
{
#if 0
	int i = 0;
    int index = tab_handle - 1;

    if(g_HandleObject[index].thread_wait_tab.semId == NULL)
    {
        printf("7zip thread_proc() Get sem Error\n");
        return 0;
    }

    //======================
    //  Thread Function
    //======================
	func(param);
    semFlush(g_HandleObject[index].thread_wait_tab.semId);

    taskDelay(30);
#else

    //======================
    //  Thread Function
    //======================
    func(param);
#endif
    return 0;
}
 
WRes Thread_Create(CThread *p, THREAD_FUNC_TYPE func, LPVOID param)
{
	int i = 0;
    for(i = 0; i < MAX_HANDLE_OBJECT_COUNT; i++)
        if(MY_ENUM_NOTYPE_HANDLE == g_HandleObject[i].handle_type)
            break;

    if(i == MAX_HANDLE_OBJECT_COUNT)
    {
        printf("Error Event is Full!\n");
        return -1;
    }

    *p = i + 1; /*return threadId*/
    g_HandleObject[i].handle_type = MY_ENUM_THREAD_HANDLE;
    g_HandleObject[i].thread_wait_tab.semId = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
    g_HandleObject[i].thread_wait_tab.threadId = (CThread)taskSpawn("tmpThread", 200, 0, 0x10000, (FUNCPTR)thread_proc, (int)func, 
                                                                    (int)param, *p, 0, 0, 0, 0, 0, 0, 0);

  return 0;
}




WRes EHandlePtr_Close(MYHANDLE *p)
{
  if (*p != NULL)
    if (!MyCloseHandle(*p))
      return GetError();
  *p = NULL;

  return 0;
}

WRes EHandle_WaitObject(MYHANDLE h) 
{ 
    return (WRes)MyWaitForSingleObject(h, WAIT_FOREVER); 

    return 0;
}


WRes Event_Create(CEvent *p, BOOL manualReset, int signaled)
{
  *p = MyCreateEvent(NULL, manualReset, (signaled ? TRUE : FALSE), NULL);

  return 0;
}

WRes Event_Set(CEvent *p) { return BOOLToWRes(MySetEvent(*p));}
WRes Event_Reset(CEvent *p) { return BOOLToWRes(MyResetEvent(*p));}

WRes ManualResetEvent_Create(CManualResetEvent *p, int signaled) { return Event_Create(p, TRUE, signaled); }
WRes AutoResetEvent_Create(CAutoResetEvent *p, int signaled) { return Event_Create(p, FALSE, signaled); }
WRes ManualResetEvent_CreateNotSignaled(CManualResetEvent *p) { return ManualResetEvent_Create(p, 0); }
WRes AutoResetEvent_CreateNotSignaled(CAutoResetEvent *p) { return AutoResetEvent_Create(p, 0); }

#if 0
WRes Semaphore_Create(CSemaphore *p, UInt32 initCount, UInt32 maxCount)
{
  *p = CreateSemaphore(NULL, (LONG)initCount, (LONG)maxCount, NULL);
  return HandleToWRes(*p);
}

static WRes Semaphore_Release(CSemaphore *p, LONG releaseCount, LONG *previousCount)
  { return BOOLToWRes(ReleaseSemaphore(*p, releaseCount, previousCount)); }
WRes Semaphore_ReleaseN(CSemaphore *p, UInt32 num)
  { return Semaphore_Release(p, (LONG)num, NULL); }
WRes Semaphore_Release1(CSemaphore *p) { return Semaphore_ReleaseN(p, 1); }
#endif 

WRes CriticalSection_Init(CCriticalSection *p)
{
#if 0
    
  /* InitializeCriticalSection can raise only STATUS_NO_MEMORY exception */
  #ifdef _MSC_VER
  __try
  #endif
  {
    InitializeCriticalSection(p);
    /* InitializeCriticalSectionAndSpinCount(p, 0); */
  }
  #ifdef _MSC_VER
  __except (EXCEPTION_EXECUTE_HANDLER) { return 1; }
  #endif
#endif  
  return 0;
}


