/************************************************************
*  File name:   CompressFace.cpp   	
*  Author:      guolisen
*  Version:   	v1.0
*  Create Date: 2011-02-22  
*  History:     Guolisen@gmail.com   
*     <author>     <time>     <version >     <desc>
*     guolisen   2011-02-22              一个很二的日子Create
***********************************************************/


#include "stdio.h"

#include "Threads.h"
#include "vxWorks/cs/VxWorksApi.h"
#include "MyWinPlatform.h"



MY_HANDLE_OBJECT g_HandleObject[MAX_HANDLE_OBJECT_COUNT] = {0};


STATUS waitForHandle(MYHANDLE eventId, int timeout)
{
    int count = 0;
	int index = (int)eventId - 1;

    while(!g_HandleObject[index].event_obj.bInitialState)
    {
        taskDelay(1);
        if(timeout != -1)
        {
            ++count;
            if(count == timeout)
			{
                return -1;
			}
        }
    }

    return 0;
}



CEvent MyCreateEvent(char* lpEventAttributes, BOOL bManualReset, BOOL bInitialState, const char* lpName)
{
    int i = 0;
 
    for(i = 0; i < MAX_HANDLE_OBJECT_COUNT; i++)
        if(MY_ENUM_NOTYPE_HANDLE == g_HandleObject[i].handle_type)
            break;

    if(i == MAX_HANDLE_OBJECT_COUNT)
    {
        printf("Error Event is Full!\n");
        return (CEvent)NULL;
    }

    g_HandleObject[i].handle_type   = MY_ENUM_EVENT_HANDLE;
    g_HandleObject[i].event_obj.bInitialState = bInitialState;
    g_HandleObject[i].event_obj.bManualReset  = bManualReset;

	return (CEvent)(i+1);
}

BOOL MySetEvent(CEvent event)
{
    int index = (int)event - 1;
    if(MY_ENUM_NOTYPE_HANDLE == g_HandleObject[index].handle_type)
    {
        printf("MySetEvent Can NOT Find EventObject(%x)\n", event);
        return FALSE;
    }

    g_HandleObject[index].event_obj.bInitialState = TRUE;
    
	return 0;
}

BOOL MyResetEvent(CEvent event)
{
    int index = (int)event - 1;
    if(MY_ENUM_NOTYPE_HANDLE == g_HandleObject[index].handle_type)
    {
        printf("MyResetEvent Can NOT Find EventObject(%x)\n", event);
        return FALSE;
    }

    g_HandleObject[index].event_obj.bInitialState = FALSE;
    
	return 0;
}

long MyWaitForSingleObject(MYHANDLE event, int time_out)
{
    int ret = -1;
    int index = (int)event - 1;

    if(g_HandleObject[index].handle_type == MY_ENUM_EVENT_HANDLE)
    {
        if(MY_ENUM_NOTYPE_HANDLE == g_HandleObject[index].handle_type)
        {
            printf("MyWaitForSingleObject Can NOT Find EventObject(%x)\n", index);
            return -1;
        }

        //无信号 阻塞
        ret = waitForHandle(event, time_out);
        if(ret < 0)   //time out return
            return -1;

        if(!g_HandleObject[index].event_obj.bManualReset)
            g_HandleObject[index].event_obj.bInitialState = FALSE;
    }
    else if(g_HandleObject[index].handle_type == MY_ENUM_THREAD_HANDLE)
    {
        semTake(g_HandleObject[index].thread_wait_tab.semId, WAIT_FOREVER);    
    }
    else
    {
        printf("MyWaitForSingleObject Error index(%d)!\n", index);
        return 0;
    }

    return 0;
}

BOOL MyCloseHandle(MYHANDLE event)
{
    int ret = -1;
    int index = (int)event - 1;

    if(g_HandleObject[index].handle_type == MY_ENUM_EVENT_HANDLE)
    {	
        g_HandleObject[index].handle_type = MY_ENUM_NOTYPE_HANDLE;
        g_HandleObject[index].event_obj.bInitialState = FALSE;
        g_HandleObject[index].event_obj.bManualReset  = FALSE;
    }
    else if(g_HandleObject[index].handle_type == MY_ENUM_THREAD_HANDLE)
    {	
        if(g_HandleObject[index].thread_wait_tab.semId != NULL)
        {
            semDelete(g_HandleObject[index].thread_wait_tab.semId);
        }
        
        if (taskDelete(g_HandleObject[index].thread_wait_tab.threadId) != OK)
            return FALSE;
        
        g_HandleObject[index].handle_type = MY_ENUM_NOTYPE_HANDLE;
        g_HandleObject[index].thread_wait_tab.threadId = NULL;
        g_HandleObject[index].thread_wait_tab.semId    = NULL;
    }
    else
    {
        printf("MyCloseHandle Error index(%d)!\n", index);
        return FALSE;
    }

    return TRUE;
}


long MyWaitForMultipleObjects(long nCount, MYHANDLE* lpHandles, BOOL fWaitAll, int dwMilliseconds)
{

    unsigned int i = 0;
    int waitTag = 0;
	int index = -1;
	unsigned long waitCount = 0; 

    if(!lpHandles)
        return -1;

    while(1)
    {
        waitTag   = 0;
		waitCount = 0;
        for(i = 0; i < nCount; ++i)
        {
			index = (int)lpHandles[i] - 1;
            if(g_HandleObject[index].event_obj.bInitialState)
            {
                ++waitTag;
            }
        }

        if(waitTag == nCount)
            break;

		if(!fWaitAll && waitTag>0)
			break;

        taskDelay(1);
		
        if(dwMilliseconds != -1)
        {
            ++waitCount;
            if(waitCount == dwMilliseconds)
                return -1;
        }			
    }    
    
    return 0;
}














