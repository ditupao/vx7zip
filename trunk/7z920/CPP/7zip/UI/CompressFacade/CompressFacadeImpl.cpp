/************************************************************
*  File name:   CompressFace.cpp   	
*  Author:      guolisen
*  Version:   	v1.0
*  Create Date: 2011-02-22  
*  History:        Guolisen@gmail.com   
*     <author>     <time>     <version >     <desc>
*     guolisen   2011-02-22              一个很二的日子Create
***********************************************************/

#include <stdio.h>



//#include "Common/MyInitGuid.h"
#include "Common/MyWindows.h"
#include "Common/MyException.h"
#include "Common/NewHandler.h"
#include "Common/StdOutStream.h"
#include "Common/IntToString.h"
#include "Common/StringConvert.h"
#include "Common/StringToInt.h"

#include "Windows/Error.h"
#include "Windows/FileDir.h"
#include "Windows/FileFind.h"
#include "Windows/FileName.h"
#include "Windows/PropVariant.h"
#include "Windows/PropVariantConversions.h"


#include "7zip/Common/FileStreams.h"
#include "7zip/UI/Common/ExitCode.h"
#include "7zip/UI/Common/LoadCodecs.h"
#include "7zip/UI/Common/Extract.h"
#include "7zip/UI/Common/Update.h"
#include "7zip/UI/Common/UpdateAction.h"
#include "7zip/UI/Common/SortUtils.h"
#include "7zip/UI/Common/EnumDirItems.h"
#include "7zip/UI/Common/DefaultName.h"


#include "7zip/UI/Console/ExtractCallbackConsole.h"
#include "7zip/UI/Console/OpenCallbackConsole.h"
#include "7zip/UI/Console/UpdateCallbackConsole.h"
#include "7zip/UI/Console/List.h"
//#include "7zip/UI/Console/BenchCon.h"


#include "7zip/Archive/IArchive.h"
#include "7zip/Archive/LibExports.h"

#include "7zip/IPassword.h"
#include "7zip/MyVersion.h"


#include "ModuleFacade/CompressFacade.h"
#include "CompressFacadeImpl.h"

using namespace NWindows;

static const char *kEverythingIsOk = "Everything is Ok";
static const char *kExceptionErrorMessage = "\n\nError:\n";
static const char *kUserBreak  = "\nBreak signaled\n";
static const char *kMemoryExceptionMessage = "\n\nERROR: Can't allocate required memory!\n";
static const char *kUnknownExceptionMessage = "\n\nUnknown Error\n";
static const char *kInternalExceptionMessage = "\n\nInternal Error #";

int g_CodePage = 0;

static void PrintString(const AString &s)
{
  printf("%s", (LPCSTR)s);
}

static void PrintNewLine()
{
  PrintString("\n");
}



static void PrintString(CStdOutStream &stdStream, const UString &s, int size)
{
  int len = s.Length();
  stdStream << s;
  for (int i = len; i < size; i++)
    stdStream << ' ';
}

static inline char GetHex(Byte value)
{
  return (char)((value < 10) ? ('0' + value) : ('A' + (value - 10)));
}

static void enumerateDirItemsAndSort(NWildcard::CCensor &wildcardCensor,
                                               UStringVector &sortedPaths,
                                               UStringVector &sortedFullPaths)
{
    UStringVector paths;
    {
        CDirItems dirItems;
        {
            UStringVector errorPaths;
            CRecordVector<DWORD> errorCodes;
            HRESULT res = EnumerateItems(wildcardCensor, dirItems, NULL, errorPaths, errorCodes);
            if (res != S_OK || errorPaths.Size() > 0)
                throw "cannot find archive";
        }
        for (int i = 0; i < dirItems.Items.Size(); i++)
        {
            const CDirItem &dirItem = dirItems.Items[i];
            if (!dirItem.IsDir())
                paths.Add(dirItems.GetPhyPath(i));
        }
    }
  
    if(paths.Size() == 0)
        throw "there is no such archive";
  
    UStringVector fullPaths;

    int i;
    for (i = 0; i < paths.Size(); i++)
    {
        UString fullPath;
        NFile::NDirectory::MyGetFullPathName(paths[i], fullPath);
        fullPaths.Add(fullPath);
    }
    CIntVector indices;
    SortFileNames(fullPaths, indices);
    sortedPaths.Reserve(indices.Size());
    sortedFullPaths.Reserve(indices.Size());
    for (i = 0; i < indices.Size(); i++)
    {
        int index = indices[i];
        sortedPaths.Add(paths[index]);
        sortedFullPaths.Add(fullPaths[index]);
    }
}


CCompressFacadeImpl::CCompressFacadeImpl(void)
{

}

CCompressFacadeImpl::~CCompressFacadeImpl(void)
{
	if(mCodecs)
	{
		delete mCodecs;
		mCodecs = NULL;
	}
}

/****************************************************************************
*  Function:       Init
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
long CCompressFacadeImpl::Init()
{
    mStdStream = g_StdOut;
    mCodecs    = new CCodecs;

    HRESULT result = mCodecs->Load();
    if (result != S_OK)
		return NExitCode::kFatalError;
    
    return NExitCode::kSuccess;
}

/****************************************************************************
*  Function:       EncodeFile
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
long CCompressFacadeImpl::EncodeFile(FILE_NAME_LIST& file_name_list, const string& compress_file_name, const string& password)
{
    return inEncodeFile(file_name_list, compress_file_name, password, eVxWorkFileUpdate);
}

/****************************************************************************
*  Function:       inEncodeFile
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
long CCompressFacadeImpl::DeleteFileFromEncode(const string& compress_file_name, FILE_NAME_LIST& delete_file_list, const string& password)
{
    return inEncodeFile(delete_file_list, compress_file_name, password, eVxWorkFileDelete);
}
/****************************************************************************
*  Function:       inEncodeFile
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
long CCompressFacadeImpl::inEncodeFile(FILE_NAME_LIST& file_name_list, const string& compress_file_name, const string& password, VxEncodeType enc_type)
{
    if(file_name_list.empty())
    {
        printf("CCompressFacadeImpl::EncodeFile Warning: File List is Empty\n");
        return NExitCode::kSuccess;
    }
    if(compress_file_name.empty())
    {
        printf("CCompressFacadeImpl::EncodeFile Para is Empty\n");
        return NExitCode::kFatalError;
    }

    try
    {
        UString archiveName    = GetUnicodeString(compress_file_name.c_str());
        UString swPassword     = GetUnicodeString(password.c_str());

        CUpdateOptions uo;
        CIntVector formatIndices;

        formatIndices.Clear();
        int compIndex = -1;
        if ((compIndex = mCodecs->FindFormatForArchiveName(archiveName)) < 0)
        {
            printf("Can NOT Find the File Compress Type\n");
            return NExitCode::kFatalError;
        }
        formatIndices.Add(compIndex);
        
        NUpdateArchive::CActionSet defaultActionSet;

        switch(enc_type)
        {
            case eVxWorkFileAdd:
                defaultActionSet = NUpdateArchive::kAddActionSet;
                break;
            case eVxWorkFileUpdate:
                defaultActionSet = NUpdateArchive::kUpdateActionSet;
                break;
            case eVxWorkFileDelete:
                defaultActionSet = NUpdateArchive::kDeleteActionSet;
                break;
            default:
                printf("Unknow Encode Type\n");
                return NExitCode::kFatalError; 
        };
        
        uo.UpdateArchiveItself = true;
        
        CUpdateArchiveCommand updateMainCommand;
        updateMainCommand.ActionSet = defaultActionSet;
        uo.Commands.Add(updateMainCommand);

        if (!uo.Init(mCodecs, formatIndices, archiveName))
        {
            printf("uo.Init error\n");
            return NExitCode::kFatalError;
        }
        
        COpenCallbackConsole openCallback;
        openCallback.OutStream = &mStdStream;

        bool passwordIsDefined = false;
        
        if(!password.empty())
            passwordIsDefined = true;
        
        #ifndef _NO_CRYPTO
        openCallback.PasswordIsDefined = passwordIsDefined;
        openCallback.Password = swPassword;
        #endif

        CUpdateCallbackConsole callback;
        callback.EnablePercents = true;

        #ifndef _NO_CRYPTO
        callback.PasswordIsDefined = passwordIsDefined;
        callback.AskPassword = false;
        callback.Password = swPassword;
        #endif
        callback.StdOutMode = false;
        callback.Init(&mStdStream);
        
        CUpdateErrorInfo errorInfo;
        NWildcard::CCensor WildcardCensor;
      
        FILE_NAME_LIST::iterator iter;
        UString swName;
        for(iter = file_name_list.begin(); iter != file_name_list.end(); ++iter)
        {
            swName = GetUnicodeString(iter->c_str());
            WildcardCensor.AddItem(true, swName, false);
        }

        HRESULT result = UpdateArchive(mCodecs,
                                       WildcardCensor, uo,
                                       errorInfo, &openCallback, &callback);

        int exitCode = NExitCode::kSuccess;
        if (callback.CantFindFiles.Size() > 0)
        {
            mStdStream << endl;
            mStdStream << "WARNINGS for files:" << endl << endl;
            int numErrors = callback.CantFindFiles.Size();
            for (int i = 0; i < numErrors; i++)
            {
                mStdStream << callback.CantFindFiles[i] << " : ";
                mStdStream << NError::MyFormatMessageW(callback.CantFindCodes[i]) << endl;
            }
            mStdStream << "----------------" << endl;
            mStdStream << "WARNING: Cannot find " << numErrors << " file";
            if (numErrors > 1)
                mStdStream << "s";
            mStdStream << endl;
            exitCode = NExitCode::kWarning;
        }

        if (result != S_OK)
        {
            UString message;
            if (!errorInfo.Message.IsEmpty())
            {
                message += errorInfo.Message;
                message += L"\n";
            }
            if (!errorInfo.FileName.IsEmpty())
            {
                message += errorInfo.FileName;
                message += L"\n";
            }
            if (!errorInfo.FileName2.IsEmpty())
            {
                message += errorInfo.FileName2;
                message += L"\n";
            }
            if (errorInfo.SystemError != 0)
            {
                message += NError::MyFormatMessageW(errorInfo.SystemError);
                message += L"\n";
            }
            if (!message.IsEmpty())
                mStdStream << L"\nError:\n" << message;
            throw CSystemException(result);
        }
        int numErrors = callback.FailedFiles.Size();
        if (numErrors == 0)
        {
            if (callback.CantFindFiles.Size() == 0)
                mStdStream << kEverythingIsOk << endl;
        }
        else
        {
            mStdStream << endl;
            mStdStream << "WARNINGS for files:" << endl << endl;
            for (int i = 0; i < numErrors; i++)
            {
                mStdStream << callback.FailedFiles[i] << " : ";
                mStdStream << NError::MyFormatMessageW(callback.FailedCodes[i]) << endl;
            }
            mStdStream << "----------------" << endl;
            mStdStream << "WARNING: Cannot open " << numErrors << " file";
            if (numErrors > 1)
                mStdStream << "s";
            mStdStream << endl;
            exitCode = NExitCode::kWarning;
        }
        
        return exitCode;
    }
    catch(const CNewException &)
    {
        (mStdStream) << kMemoryExceptionMessage;
        return (NExitCode::kMemoryError);
    }
    catch(const CSystemException &systemError)
    {
        if (systemError.ErrorCode == E_OUTOFMEMORY)
        {
            (mStdStream) << kMemoryExceptionMessage;
            return (NExitCode::kMemoryError);
        }
        if (systemError.ErrorCode == E_ABORT)
        {
            (mStdStream) << endl << kUserBreak;
            return (NExitCode::kUserBreak);
        }
        UString message;
        NError::MyFormatMessage(systemError.ErrorCode, message);
        (mStdStream) << endl << endl << "System error:" << endl << message << endl;
        return (NExitCode::kFatalError);
    }
    catch(NExitCode::EEnum &exitCode)
    {
        (mStdStream) << kInternalExceptionMessage << exitCode << endl;
        return (exitCode);
    }
    /*
    catch(const NExitCode::CMultipleErrors &multipleErrors)
    {
        (mStdStream) << endl << multipleErrors.NumErrors << " errors" << endl;
        return (NExitCode::kFatalError);
    }
    */
    catch(const UString &s)
    {
        (mStdStream) << kExceptionErrorMessage << s << endl;
        return (NExitCode::kFatalError);
    }
    catch(const AString &s)
    {
        (mStdStream) << kExceptionErrorMessage << s << endl;
        return (NExitCode::kFatalError);
    }
    catch(const char *s)
    {
        (mStdStream) << kExceptionErrorMessage << s << endl;
        return (NExitCode::kFatalError);
    }
    catch(int t)
    {
        (mStdStream) << kInternalExceptionMessage << t << endl;
        return (NExitCode::kFatalError);
    }
    catch(...)
    {
        (mStdStream) << kUnknownExceptionMessage;
        return (NExitCode::kFatalError);
    }
   
	return NExitCode::kSuccess;
}

/****************************************************************************
*  Function:       DecodeFile
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
long CCompressFacadeImpl::DecodeFile(const string& compress_file_name, FILE_NAME_LIST& output_file_list, const string& default_path, const string& password)
{
    if(compress_file_name.empty())
    {
        printf("CCompressFacadeImpl::DecodeFile Warning: Compress File Name is Empty\n");
        return NExitCode::kSuccess;
    }

    try
    {
        CExtractCallbackConsole *ecs = new CExtractCallbackConsole;
        CMyComPtr<IFolderArchiveExtractCallback> extractCallback = ecs;

        ecs->OutStream = &mStdStream;

        bool passwordIsDefined = false;
        if(!password.empty())
            passwordIsDefined = true;

#ifndef _NO_CRYPTO
        ecs->PasswordIsDefined = passwordIsDefined;
        ecs->Password = GetUnicodeString(password.c_str());
#endif

        ecs->Init();

        COpenCallbackConsole openCallback;
        openCallback.OutStream = &mStdStream;

#ifndef _NO_CRYPTO
        UString swPassword = GetUnicodeString(password.c_str());
        openCallback.PasswordIsDefined = passwordIsDefined;
        openCallback.Password = swPassword;
#endif

        NWildcard::CCensor tmpWildcardCensor;
      
        UString swName;
        swName = GetUnicodeString(compress_file_name.c_str());
        tmpWildcardCensor.AddItem(true, swName, false);
        tmpWildcardCensor.ExtendExclude();
        
        UStringVector ArchivePathsSorted;
        UStringVector ArchivePathsFullSorted;
        enumerateDirItemsAndSort(tmpWildcardCensor, ArchivePathsSorted, ArchivePathsFullSorted);
        
        NWildcard::CCensor WildcardCensor;

        if(output_file_list.empty())
        {
            swName = L"*";
            WildcardCensor.AddItem(true, swName, false);
        }
        else
        { 
            FILE_NAME_LIST::iterator it;
            for(it = output_file_list.begin(); it != output_file_list.end(); ++it)
            {
                swName = GetUnicodeString(it->c_str());
                WildcardCensor.AddItem(true, swName, false);
            }
        }

        CExtractOptions eo;
        eo.StdInMode     = false;
        eo.StdOutMode    = false;
        eo.PathMode      = NExtract::NPathMode::kFullPathnames;
        eo.TestMode      = false;
        eo.OverwriteMode = NExtract::NOverwriteMode::kAskBefore;
        eo.OutputDir     = GetUnicodeString(default_path.c_str());
        eo.YesToAll      = true;
        eo.CalcCrc       = true;

        UString errorMessage;
        CDecompressStat stat;
        CIntVector formatIndices;

        //自动搜索解压类型
        formatIndices.Clear();
        HRESULT result = DecompressArchives(mCodecs,
                                            formatIndices,
                                            ArchivePathsSorted,
                                            ArchivePathsFullSorted,
                                            WildcardCensor.Pairs.Front().Head,
                                            eo, &openCallback, ecs, errorMessage, stat);

        if (!errorMessage.IsEmpty())
        {
            mStdStream << endl << "Error: " << errorMessage;
            if (result == S_OK)
                result = E_FAIL;
        }

        mStdStream << endl;
        if (ecs->NumArchives > 1)
            mStdStream << "Archives: " << ecs->NumArchives << endl;
        if (ecs->NumArchiveErrors != 0 || ecs->NumFileErrors != 0)
        {
            if (ecs->NumArchives > 1)
            {
                mStdStream << endl;
                if (ecs->NumArchiveErrors != 0)
                    mStdStream << "Archive Errors: " << ecs->NumArchiveErrors << endl;
                if (ecs->NumFileErrors != 0)
                    mStdStream << "Sub items Errors: " << ecs->NumFileErrors << endl;
            }
            if (result != S_OK)
                throw CSystemException(result);
            return NExitCode::kFatalError;
        }
        if (result != S_OK)
            throw CSystemException(result);
        if (stat.NumFolders != 0)
            mStdStream << "Folders: " << stat.NumFolders << endl;
        if (stat.NumFiles != 1 || stat.NumFolders != 0)
            mStdStream << "Files: "   << stat.NumFiles   << endl;
        
        mStdStream << "Size:       " << stat.UnpackSize << endl
                   << "Compressed: " << stat.PackSize   << endl;
        
        if (eo.CalcCrc)
        {
            char s[16];
            ConvertUInt32ToHexWithZeros(stat.CrcSum, s);
            mStdStream << "CRC: " << s << endl;
        }

    }
    catch(const CNewException &)
    {
        (mStdStream) << kMemoryExceptionMessage;
        return (NExitCode::kMemoryError);
    }
    catch(const CSystemException &systemError)
    {
        if (systemError.ErrorCode == E_OUTOFMEMORY)
        {
            (mStdStream) << kMemoryExceptionMessage;
            return (NExitCode::kMemoryError);
        }
        if (systemError.ErrorCode == E_ABORT)
        {
            (mStdStream) << endl << kUserBreak;
            return (NExitCode::kUserBreak);
        }
        UString message;
        NError::MyFormatMessage(systemError.ErrorCode, message);
        (mStdStream) << endl << endl << "System error:" << endl << message << endl;
        return (NExitCode::kFatalError);
    }
    catch(NExitCode::EEnum &exitCode)
    {
        (mStdStream) << kInternalExceptionMessage << exitCode << endl;
        return (exitCode);
    }
    catch(const UString &s)
    {
        (mStdStream) << kExceptionErrorMessage << s << endl;
        return (NExitCode::kFatalError);
    }
    catch(const AString &s)
    {
        (mStdStream) << kExceptionErrorMessage << s << endl;
        return (NExitCode::kFatalError);
    }
    catch(const char *s)
    {
        (mStdStream) << kExceptionErrorMessage << s << endl;
        return (NExitCode::kFatalError);
    }
    catch(int t)
    {
        (mStdStream) << kInternalExceptionMessage << t << endl;
        return (NExitCode::kFatalError);
    }
    catch(...)
    {
        (mStdStream) << kUnknownExceptionMessage;
        return (NExitCode::kFatalError);
    }

	return NExitCode::kSuccess;
}
/*
long CCompressFacadeImpl::BenchmarkSystem()
{

    HRESULT res = LzmaBenchCon(
                          #ifdef EXTERNAL_CODECS
                          //compressCodecsInfo, &externalCodecs,
                          #endif
                         (FILE *)mStdStream, 1, -1, -1);
    if (res != S_OK)
    {
        if (res == S_FALSE)
        {
            mStdStream << "\nDecoding Error\n";
            return NExitCode::kFatalError;
        }
        throw CSystemException(res);
    }

    return res;
}
*/
long CCompressFacadeImpl::ShowCodeInfo()
{
    mStdStream << endl << "Formats:" << endl;
    int i;
    for (i = 0; i < mCodecs->Formats.Size(); i++)
    {
        printf("----------------------------------------------------------------\n");
        const CArcInfoEx &arc = mCodecs->Formats[i];
        mStdStream << "  ";
        mStdStream << ' ';
        mStdStream << (char)(arc.UpdateEnabled ? 'C' : ' ');
        mStdStream << (char)(arc.KeepName ? 'K' : ' ');
        mStdStream << "  ";
        PrintString(mStdStream, arc.Name, 6);
        mStdStream << "  ";
        UString s;
        for (int t = 0; t < arc.Exts.Size(); t++)
        {
            const CArcExtInfo &ext = arc.Exts[t];
            s += ext.Ext;
            if (!ext.AddExt.IsEmpty())
            {
                s += L" (";
                s += ext.AddExt;
                s += L')';
            }
            s += L' ';
        }
        PrintString(mStdStream, s, 14);
        mStdStream << "  ";
        const CByteBuffer &sig = arc.StartSignature;
        for (size_t j = 0; j < sig.GetCapacity(); j++)
        {
            Byte b = sig[j];
            if (b > 0x20 && b < 0x80)
            {
                mStdStream << (char)b;
            }
            else
            {
                mStdStream << GetHex((Byte)((b >> 4) & 0xF));
                mStdStream << GetHex((Byte)(b & 0xF));
            }
            mStdStream << ' ';
        }
        mStdStream << endl;
    }

    return NExitCode::kSuccess;
}

long CCompressFacadeImpl::ListEncodeFile(const string& compress_file_name, const string& password)
{
    if(compress_file_name.empty())
    {
        printf("CCompressFacadeImpl::ListEncodeFile Warning: Compress File Name is Empty\n");
        return NExitCode::kSuccess;
    }

    try
    {
        bool passwordIsDefined = false;
        if(!password.empty())
            passwordIsDefined = true;

        NWildcard::CCensor tmpWildcardCensor;
      
        UString swName;
        swName = GetUnicodeString(compress_file_name.c_str());
        tmpWildcardCensor.AddItem(true, swName, false);
        tmpWildcardCensor.ExtendExclude();
        
        UStringVector ArchivePathsSorted;
        UStringVector ArchivePathsFullSorted;
        enumerateDirItemsAndSort(tmpWildcardCensor, ArchivePathsSorted, ArchivePathsFullSorted);
        
        NWildcard::CCensor WildcardCensor;
        UString allName = L"*";
        WildcardCensor.AddItem(true, allName, false);

        CIntVector formatIndices;
        formatIndices.Clear();
        int compIndex = -1;
        if ((compIndex = mCodecs->FindFormatForArchiveName2(swName)) < 0)
        {
            printf("Can NOT Find the File Compress Type\n");
            return NExitCode::kFatalError;
        }
        formatIndices.Add(compIndex);

        UString uPassword = GetUnicodeString(password.c_str());
        UInt64 numErrors = 0;
        HRESULT result = ListArchives(mCodecs,
                                      formatIndices,
                                      false,
                                      ArchivePathsSorted,
                                      ArchivePathsFullSorted,
                                      WildcardCensor.Pairs.Front().Head,
                                      true,
                                      false,
                                  #ifndef _NO_CRYPTO
                                      passwordIsDefined,
                                      uPassword,
                                  #endif
                                      numErrors);

        if (numErrors > 0)
        {
            g_StdOut << endl << "Errors: " << numErrors;
            return NExitCode::kFatalError;
        }
    }
    catch(const CNewException &)
    {
        (mStdStream) << kMemoryExceptionMessage;
        return (NExitCode::kMemoryError);
    }
    catch(const CSystemException &systemError)
    {
        if (systemError.ErrorCode == E_OUTOFMEMORY)
        {
            (mStdStream) << kMemoryExceptionMessage;
            return (NExitCode::kMemoryError);
        }
        if (systemError.ErrorCode == E_ABORT)
        {
            (mStdStream) << endl << kUserBreak;
            return (NExitCode::kUserBreak);
        }
        UString message;
        NError::MyFormatMessage(systemError.ErrorCode, message);
        (mStdStream) << endl << endl << "System error:" << endl << message << endl;
        return (NExitCode::kFatalError);
    }
    catch(NExitCode::EEnum &exitCode)
    {
        (mStdStream) << kInternalExceptionMessage << exitCode << endl;
        return (exitCode);
    }
    catch(const UString &s)
    {
        (mStdStream) << kExceptionErrorMessage << s << endl;
        return (NExitCode::kFatalError);
    }
    catch(const AString &s)
    {
        (mStdStream) << kExceptionErrorMessage << s << endl;
        return (NExitCode::kFatalError);
    }
    catch(const char *s)
    {
        (mStdStream) << kExceptionErrorMessage << s << endl;
        return (NExitCode::kFatalError);
    }
    catch(int t)
    {
        (mStdStream) << kInternalExceptionMessage << t << endl;
        return (NExitCode::kFatalError);
    }
    catch(...)
    {
        (mStdStream) << kUnknownExceptionMessage;
        return (NExitCode::kFatalError);
    }

    return NExitCode::kSuccess;
}

long CCompressFacadeImpl::GetEncodeFileList(const string& compress_file_name, FILE_NAME_LIST& file_name_list)
{
	if(compress_file_name.empty())
	{
		printf("CCompressFacadeImpl::GetEncodeFileList Para Error, File Name is Empty\n");
		return NExitCode::kFatalError;
	}

	const UString &archiveName = GetUnicodeString(compress_file_name.c_str());
    CIntVector formatIndices;
    formatIndices.Clear();
    int compIndex = -1;
    if ((compIndex = mCodecs->FindFormatForArchiveName2(archiveName)) < 0)
    {
        printf("Can NOT Find the File(%s) Compress Type\n", compress_file_name.c_str());
        return NExitCode::kFatalError;
    }
    formatIndices.Add(compIndex);

    UInt64 arcPackSize = 0;
    NFile::NFind::CFileInfoW fi;
    if (!fi.Find(archiveName) || fi.IsDir())
    {
        mStdStream << endl << "Error: " << archiveName << " is not file" << endl;
        return NExitCode::kFatalError;
    }

    CArchiveLink archiveLink;
    COpenCallbackConsole openCallback;
    openCallback.OutStream = &mStdStream;

    #ifndef _NO_CRYPTO
    openCallback.PasswordIsDefined = false;
    openCallback.Password = L"";
    #endif

    HRESULT result = archiveLink.Open2(mCodecs, formatIndices, false, NULL, archiveName, &openCallback);
    if (result != S_OK)
    {
        if (result == E_ABORT)
            return result;
        mStdStream << endl << "Error: " << archiveName << ": ";
        if (result == S_FALSE)
        {
#ifndef _NO_CRYPTO
        if (openCallback.Open_WasPasswordAsked())
            mStdStream << "Can not open encrypted archive. Wrong password?";
        else
#endif
            mStdStream << "Can not open file as archive";
        }
        else if (result == E_OUTOFMEMORY)
            mStdStream << "Can't allocate required memory";
        else
            mStdStream << NError::MyFormatMessage(result);
        mStdStream << endl;

        return result;
    }

    NWildcard::CCensor wildcardCensor;
    UString allName = L"*";
    wildcardCensor.AddItem(true, allName, false);

    const CArc &arc = archiveLink.Arcs.Back();
    IInArchive *archive = arc.Archive;

    UInt32 numItems;
    file_name_list.clear();
    RINOK(archive->GetNumberOfItems(&numItems));
    for (UInt32 i = 0; i < numItems; i++)
    {
        UString filePath;
        HRESULT res = arc.GetItemPath(i, filePath);
        RINOK(res);

        bool isFolder;
        RINOK(IsArchiveItemFolder(archive, i, isFolder));
        if (isFolder)
            continue;
        
        string strFilePath = string(UnicodeStringToMultiByte(filePath));
        file_name_list.push_back(strFilePath);
    }
 
    return NExitCode::kSuccess;
}

