
#include <windows.h>

void WINAPI RtlSecondsSince1970ToFileTime( DWORD Seconds, FILETIME * ft );
 
extern "C" int global_use_utf16_conversion;
#ifdef HAVE_LSTAT
extern "C" int global_use_lstat;
#endif

const char *my_getlocale(void);

#ifdef NEED_NAME_WINDOWS_TO_UNIX
static inline const char * nameWindowToUnix(const char * lpFileName) {
  if ((lpFileName[0] == 'c') && (lpFileName[1] == ':')) return lpFileName+2;
  return lpFileName;
}
#endif

