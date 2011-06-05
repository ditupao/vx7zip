#ifndef MY_WINDOWS_PLATFORM_H
#define MY_WINDOWS_PLATFORM_H
/****************************************************************************
*  File name:      	CompressFace.h
*  Author:       	guolisen
*  Version:   		v1.0
*  Create Date:    	2011-02-22
*  Description:    	Guolisen@gmail.com   
*  Change History:    
*  <author>    <time>     <version >      <desc>
*  Guolisen  2011-06-22                   
*****************************************************************************/

#include "Types.h"
#include "vxWorks/cs/VxWorksApi.h"
#include "Threads.h"


#ifdef __cplusplus
extern "C" {
#endif

#define MAX_HANDLE_OBJECT_COUNT 100

enum HANDLE_TYPE
{
    MY_ENUM_NOTYPE_HANDLE = 0,
    MY_ENUM_THREAD_HANDLE,
    MY_ENUM_EVENT_HANDLE
};

typedef struct thread_wait_table_tag{
    MYHANDLE threadId;
    SEM_ID semId;
}THREAD_WAIT_TABLE;


typedef struct EventObject
{
    BOOL bManualReset;
    BOOL bInitialState;
}MY_EVENT_OBJECT;


typedef struct Handle_Object_tag
{
    int handle_type;
    union{
        THREAD_WAIT_TABLE thread_wait_tab;
        MY_EVENT_OBJECT   event_obj;
    };
}MY_HANDLE_OBJECT;


CEvent MyCreateEvent(char* lpEventAttributes, BOOL bManualReset, BOOL bInitialState, const char* lpName);
BOOL MySetEvent(CEvent event);
BOOL MyResetEvent(CEvent event);
BOOL MyCloseHandle(MYHANDLE event);
long MyWaitForSingleObject(MYHANDLE event, int time_out);
long MyWaitForMultipleObjects(long nCount, MYHANDLE* lpHandles, BOOL fWaitAll, int dwMilliseconds);


#ifdef __cplusplus
}
#endif


#endif
