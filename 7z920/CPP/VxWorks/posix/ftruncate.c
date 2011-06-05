/* ftruncate.c - POSIX file truncation */

/* 
 * Copyright (c) 2005-2006 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */ 

/*
modification history
--------------------
01i,02jun06,pcs  Update documentation of ftruncate to reflect behaviour with
                 respect to Shared Memory Objects and Memory Mapped Files.
01h,28apr06,dgp  doc: fix ERRNO formatting
01g,28apr05,act  if underlying file system returns a POSIX error, leave it
                 alone
01f,09dec04,job  SPR#105131: Remove NOMANUAL 
01e,14oct04,pad  Fixed doc formatting (SPR #81017).
01d,30sep04,yvp  Added 64-bit support in user-mode.
01c,28jan94,dvs  doc changes
01b,05jan94,kdl  general cleanup.
01a,02nov93,dvs  written
*/

/*
DESCRIPTION
This module contains the POSIX compliant ftruncate() routine for truncating
a file.

INCLUDE FILES: unistd.h

SEE ALSO:
*/

/* INCLUDES */
#include <vxWorks.h>
#include <unistd.h>
#include <errno.h>
#include <ioLib.h>
#include <dosFsLib.h>

/******************************************************************************
*
* ftruncate - truncate a file (POSIX)
*
* This routine truncates a file to a specified size. 
* 
*\IFSET_START USER
*   If fildes refers to a Shared Memory Object, ftruncate() shall set the size 
*   of the shared memory object to length.
*   If the effect of ftruncate() is to decrease the size of a Shared Memory 
*   Object or Memory Mapped File and whole pages beyond the new end were
*   previously mapped, then the whole pages beyond the new end shall be 
*   discarded. References to discarded pages would be possible but, msync on 
*   the discarded  pages will not succeed.
*\IFSET_END USER
*
* RETURNS: 0 (OK) or -1 (ERROR) if unable to truncate file.
*
* ERRNO
* \is
* \i EROFS 
* File resides on a read-only file system.
* \i EBADF
* File is open for reading only.
* \i EINVAL
* File descriptor refers to a file on which this operation is impossible.
* \IFSET_START USER
* Length cannot be completely represented with an 'off_t' type.
* \IFSET_END USER
* \ie
*
*/

int ftruncate 
    (
    int 	fildes,			/* fd of file to truncate */
    off_t	length			/* length to truncate file */
    )
    {
    int 	status;			/* status value from ioctl */
    int		len;

#ifndef _WRS_KERNEL
    if (HIGH32(length) != 0)
	{
	errno = EINVAL;
	return (ERROR);
	}
    len = LOW32(length);
#else
    len = length;
#endif /* _WRS_KERNEL */

    if ((status = ioctl (fildes, FIOTRUNC, len)) == ERROR)
	{
	/* map errno to that specified by POSIX */
	switch (errno)
            {
	    case EINTR:
	    case EINVAL:
	    case EFBIG:
	    case EIO:
	    case EBADF:
	    case EROFS:
		break;

	    case S_ioLib_WRITE_PROTECTED:
		errno = EROFS;
		break;

	    case S_dosFsLib_READ_ONLY:
		errno = EBADF;
		break;

	    case S_ioLib_UNKNOWN_REQUEST:
	    default:
		errno = EINVAL;
		break;
	    }
	return (ERROR);
	}
    else 
	return (OK);
    }
