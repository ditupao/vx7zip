/* wchar.h standard header */
#ifndef _WCHAR_
#define _WCHAR_


#ifdef _UNICODE

#ifndef EXTERN_C_BEGIN
#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif
#endif

#ifndef _MY_WCHAR_
#define _MY_WCHAR_
#define wchar_t unsigned short
#endif

EXTERN_C_BEGIN



wchar_t* my_wcscat(wchar_t *, const wchar_t *);
wchar_t* my_wcscpy(wchar_t *, const wchar_t *);
long     my_wcslen(const wchar_t *);

EXTERN_C_END

#endif
 #endif 

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.00:1278 */
