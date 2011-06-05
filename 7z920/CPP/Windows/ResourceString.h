// Windows/ResourceString.h

#ifndef __WINDOWS_RESOURCESTRING_H
#define __WINDOWS_RESOURCESTRING_H

#include "Common/MyString.h"

namespace NWindows {

CSysString MyLoadString(HINSTANCE hInstance, UInt resourceID);
CSysString MyLoadString(UInt resourceID);
#ifdef _UNICODE
inline UString MyLoadStringW(HINSTANCE hInstance, UInt resourceID) { return MyLoadString(hInstance, resourceID); }
inline UString MyLoadStringW(UInt resourceID) { return MyLoadString(resourceID); }
#else
UString MyLoadStringW(HINSTANCE hInstance, UInt resourceID);
UString MyLoadStringW(UInt resourceID);
#endif

}

#endif
