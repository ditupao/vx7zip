// Windows/NtCheck.h

#ifndef __WINDOWS_NT_CHECK_H
#define __WINDOWS_NT_CHECK_H

#ifdef _MY_WIN32

#if !defined(_WIN64) && !defined(UNDER_CE)
static inline bool IsItWindowsNT()
{
  OSVERSIONINFO vi;
  vi.dwOSVersionInfoSize = sizeof(vi);
  return (::GetVersionEx(&vi) && vi.dwPlatformId == VER_PLATFORM_WIN32_NT);
}
#endif

#ifndef _UNICODE
  #if defined(_WIN64) || defined(UNDER_CE)
    bool g_IsNT = true;
    #define SET_IS_NT
  #else
    bool g_IsNT = false;
    #define SET_IS_NT g_IsNT = IsItWindowsNT();
  #endif
  #define NT_CHECK_ACTION
  // #define NT_CHECK_ACTION { NT_CHECK_FAIL_ACTION }
#else
  #if !defined(_WIN64) && !defined(UNDER_CE)
    #define NT_CHECK_ACTION if (!IsItWindowsNT()) { NT_CHECK_FAIL_ACTION }
  #else
    #define NT_CHECK_ACTION
  #endif
  #define SET_IS_NT
#endif

#define NT_CHECK  NT_CHECK_ACTION SET_IS_NT

#else
bool g_IsNT = false;  //guolisen
#define NT_CHECK

#endif

#endif
