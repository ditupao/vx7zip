// Archive/Zip/Header.h

#include "StdAfx.h"

#include "ZipHeader.h"
#include <stdio.h>
namespace NArchive {
namespace NZip {

namespace NSignature
{ //guolisen
  UInt32 kLocalFileHeader   = 0x04034B50;
  UInt32 kDataDescriptor    = 0x08074B50;
  UInt32 kCentralFileHeader = 0x02014B50;
  UInt32 kEndOfCentralDir   = 0x06054B50;
  UInt32 kZip64EndOfCentralDir   = 0x06064B50;
  UInt32 kZip64EndOfCentralDirLocator   = 0x07064B50;
  
  class CMarkersInitializer
  {
  public:
    CMarkersInitializer()
    {
#if 0	//guolisen
      kLocalFileHeader--;
      kDataDescriptor--;
      kCentralFileHeader--;
      kEndOfCentralDir--;
      kZip64EndOfCentralDir--;
      kZip64EndOfCentralDirLocator--;
#endif	  
    }
  };
  static CMarkersInitializer g_MarkerInitializer;
}

}}

