// Common/C_FileIO.h

#include "C_FileIO.h"

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>




namespace NC {
namespace NFile {
namespace NIO {

bool CFileBase::OpenBinary(const char *name, int flags)
{
  #ifdef O_BINARY
  flags |= O_BINARY;
  #endif
  Close();
  _handle = ::open(name, flags, 0666);
  return _handle != -1;
}

bool CFileBase::Close()
{
  if (_handle == -1)
    return true;
  if (close(_handle) != 0)
    return false;
  _handle = -1;
  return true;
}

bool CFileBase::GetLength(UInt64 &length) const
{
  off_t curPos = Seek(0, SEEK_CUR);
  off_t lengthTemp = Seek(0, SEEK_END);
  Seek(curPos, SEEK_SET);
  length = (UInt64)lengthTemp;
  return true;
}

off_t CFileBase::Seek(off_t distanceToMove, int moveMethod) const
{
  return ::lseek(_handle, distanceToMove, moveMethod);
}

/////////////////////////
// CInFile

bool CInFile::Open(const char *name)
{
  return CFileBase::OpenBinary(name, O_RDONLY);
}

bool CInFile::OpenShared(const char *name, bool)
{
  return Open(name);
}

ssize_t CInFile::Read(void *data, size_t size)
{
  return read(_handle, (char *)data, size);
}

/////////////////////////
// COutFile

bool COutFile::Create(const char *name, bool createAlways)
{
  if (createAlways)
  {
    Close();
    _handle = ::creat(name, 0666);
    return _handle != -1;
  }
  return OpenBinary(name, O_CREAT | O_EXCL | O_WRONLY);
}

bool COutFile::Open(const char *name, DWORD creationDisposition)
{
  return Create(name, false);
}

ssize_t COutFile::Write(const void *data, size_t size)
{
  return write(_handle, (char *)data, size);
}

bool COutFile::SetTime(const FILETIME *cTime, const FILETIME *aTime, const FILETIME *mTime)
{
#if 0
  LARGE_INTEGER  ltime;
  DWORD dw;

  if (_fd == -1) {
     SetLastError( ERROR_INVALID_HANDLE );
     return false;
  }

  /* On some OS (cygwin, MacOSX ...), you must close the file before updating times */
  if (aTime) {
     ltime.QuadPart = aTime->dwHighDateTime;
     ltime.QuadPart = (ltime.QuadPart << 32) | aTime->dwLowDateTime;
     RtlTimeToSecondsSince1970( &ltime, &dw );
     _lastAccessTime = dw;
  }
  if (mTime) {
     ltime.QuadPart = mTime->dwHighDateTime;
     ltime.QuadPart = (ltime.QuadPart << 32) | mTime->dwLowDateTime;
     RtlTimeToSecondsSince1970( &ltime, &dw );
     _lastWriteTime = dw;
  }
#endif
  return true;
}

bool COutFile::SetMTime(const FILETIME *mTime)
{
  return SetTime(NULL, NULL, mTime);
}


}}}
