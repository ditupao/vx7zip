/* my_wcslen function */
#include "wchar.h"
/*_STD_BEGIN*/

#ifdef _UNICODE
long my_wcslen(const wchar_t *s)
	{	/* find length of wchar_t s[] */
	const wchar_t *sc;

	for (sc = s; *sc != L'\0'; ++sc)
		;
	return (sc - s);
	}
#endif

/*_STD_END*/


/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V4.00:1278 */
