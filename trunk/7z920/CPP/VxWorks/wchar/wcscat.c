/* my_wcscat function */
#include "wchar.h"
/*_STD_BEGIN*/

#ifdef _UNICODE
wchar_t* my_wcscat(wchar_t * s1, const wchar_t * s2)
{	/* copy wchar_t s2[] to end of s1[] */
	wchar_t *s;
	
	for (s = s1; *s != L'\0'; ++s)
		;			/* find end of s1[] */
	for (; (*s = *s2) != L'\0'; ++s, ++s2)
		;			/* copy s2[] to end */
	return (s1);
}
#endif

/*_STD_END*/


/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.00:1278 */
