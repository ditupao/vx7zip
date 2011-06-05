/* my_wcscpy function */
#include "wchar.h"
/*_STD_BEGIN*/

#ifdef _UNICODE
wchar_t*  my_wcscpy(wchar_t * s1, const wchar_t * s2)
	{	/* copy wchar_t s2[] to s1[] */
	wchar_t *s;

	for (s = s1; (*s++ = *s2++) != L'\0'; )
		;
	return (s1);
	}
#endif

/*_STD_END*/

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.00:1278 */
