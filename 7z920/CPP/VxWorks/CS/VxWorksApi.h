#ifndef _VXWORKS_API_H
#define _VXWORKS_API_H

#ifdef __VXWORKS__
    #define KEEP_PROCESS_ALIVE
    #include <vxWorks.h>
    #include <errno.h>
    #include <taskLib.h>
    #include <inetLib.h>
    #include <msgQLib.h>
    #include <semLib.h>
    #include <hostLib.h>
    #include <sockLib.h>
    #include <ioLib.h>
    #include <pipeDrv.h>
    #include <sysLib.h>
    #include <tickLib.h>
    #include <selectLib.h>
    #include <stdio.h> 
    #include <string.h>
    #include <dosFsLib.h>
	#include <time.h>
#endif

#endif
