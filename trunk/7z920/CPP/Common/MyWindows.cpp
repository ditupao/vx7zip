// MyWindows.cpp

#include "StdAfx.h"

#ifndef _WIN32
#include <stdlib.h>
#include "Types.h"

#define INITGUID
#include "MyGuidDef.h"


#include "MyWindows.h"

#include "Common/StdOutStream.h"

int global_use_utf16_conversion = 1;

#include <stdio.h>
static inline void *AllocateForBSTR(size_t cb) {return ::malloc(cb);}
static inline void FreeForBSTR(void *pv) { ::free(pv);}


static void PrintString(CStdOutStream &stdStream, const BSTR s, int size)
{
  UInt len = *((UInt *)s -1);
  wchar_t *str = (wchar_t *)(s);
  stdStream << str;
  for (int i = len; i < size; i++)
    stdStream << ' ';
	
	stdStream << endl;
}

static UInt MyStringLen(const wchar_t *s)
{
  UInt i;
  for (i = 0; s[i] != '\0'; i++);
  return i;
}

BSTR SysAllocStringByteLen(LPCSTR psz, UInt len)
{
  // FIXED int realLen = len + sizeof(UInt) + 3;
  const int LEN_ADDON = sizeof(wchar_t) - 1;
  int realLen = len + sizeof(UInt) + sizeof(wchar_t) + LEN_ADDON;
  void *p = AllocateForBSTR(realLen);
  if (p == 0)
    return 0;
  *(UInt *)p = len;
  // "void *" instead of "BSTR" to avoid unaligned copy of "wchar_t" because of optimizer on Solaris
  void * bstr = (void *)((UInt *)p + 1);
  if (psz) memmove(bstr, psz, len); // psz does not always have "wchar_t" alignment.
  void *pb = (void *)(((Byte *)bstr) + len);
  memset(pb,0,sizeof(wchar_t) + LEN_ADDON);
  return (BSTR)bstr;
}

BSTR SysAllocStringByteLenW(LPCTSTR psz, UInt len)
{	
//	printf("IN: ");
//    if(psz)
//		PrintString(g_StdOut, (BSTR)psz, 4);
	
	int wcharlen = len * 2;
	// FIXED int realLen = len + sizeof(UInt) + 3;
	const int LEN_ADDON = sizeof(wchar_t) - 1;
	int realLen = wcharlen + sizeof(UInt) + sizeof(wchar_t) + LEN_ADDON;
	void *p = AllocateForBSTR(realLen);
	if (p == 0)
		return 0;
	*(UInt *)p = len;
	// "void *" instead of "BSTR" to avoid unaligned copy of "wchar_t" because of optimizer on Solaris
	void * bstr = (void *)((UInt *)p + 1);
	if (psz) memmove(bstr, psz, wcharlen); // psz does not always have "wchar_t" alignment.
	void *pb = (void *)(((Byte *)bstr) + wcharlen);
	memset(pb,0,sizeof(wchar_t) + LEN_ADDON);
	
//	printf("OUT: ");
//	PrintString(g_StdOut, (BSTR)bstr, 4);
	return (BSTR)bstr;
}

BSTR SysAllocString(const OLECHAR *sz)
{
  if (sz == 0)
    return 0;
  UInt strLen = MyStringLen(sz);
  UInt len = (strLen + 1) * sizeof(OLECHAR);
  void *p = AllocateForBSTR(len + sizeof(UInt));
  if (p == 0)
    return 0;
  *(UInt *)p = strLen * sizeof(OLECHAR); // FIXED
  void * bstr = (void *)((UInt *)p + 1);
  memmove(bstr, sz, len); // sz does not always have "wchar_t" alignment.
  return (BSTR)bstr;
}

void SysFreeString(BSTR bstr)
{
  if (bstr != 0)
    FreeForBSTR((UInt *)bstr - 1);
}

UInt SysStringByteLen(BSTR bstr)
{
  if (bstr == 0)
    return 0;
  return *((UInt *)bstr - 1);
}

UInt SysStringLen(BSTR bstr)
{
  return SysStringByteLen(bstr) / sizeof(OLECHAR);
}

HRESULT VariantClear(VARIANTARG *prop)
{
  if (prop->vt == VT_BSTR)
    SysFreeString(prop->bstrVal);
  prop->vt = VT_EMPTY;
  return S_OK;
}

HRESULT VariantCopy(VARIANTARG *dest, VARIANTARG *src)
{
  HRESULT res = ::VariantClear(dest);
  if (res != S_OK)
    return res;
  if (src->vt == VT_BSTR)
  {
    dest->bstrVal = SysAllocStringByteLenW((LPCTSTR)src->bstrVal,
        SysStringByteLen(src->bstrVal)); 
    if (dest->bstrVal == 0)
      return E_OUTOFMEMORY;
    dest->vt = VT_BSTR;
  } 
  else
    *dest = *src;
  return S_OK;
}

LONG CompareFileTime(const FILETIME* ft1, const FILETIME* ft2)
{
  if (ft1->dwHighDateTime < ft2->dwHighDateTime) return -1;
  if (ft1->dwHighDateTime > ft2->dwHighDateTime) return 1;
  if (ft1->dwLowDateTime < ft2->dwLowDateTime) return -1;
  if (ft1->dwLowDateTime > ft2->dwLowDateTime) return 1;
  return 0;
}

DWORD GetLastError()
{
  return 0;
}

#endif
