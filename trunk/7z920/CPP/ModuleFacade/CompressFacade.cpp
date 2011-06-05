/************************************************************
*  Copyright (C), 2006－2011, Potevio Institute of Technology.
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

#include "CompressFacade.h"
#include "7zip/UI/CompressFacade/CompressFacadeImpl.h"

CCompressFacade* CCompressFacade::mInstance = NULL;

CCompressFacade::CCompressFacade():_impl(new CCompressFacadeImpl())
{
}        

    
CCompressFacade::~CCompressFacade()
{
	if(_impl)
	{
		delete _impl;
		_impl = NULL;
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
long CCompressFacade::Init()
{
    return _impl->Init();
}

/****************************************************************************
*  Function:       GetInstance()
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
CCompressFacade* CCompressFacade::GetInstance()
{
    if(NULL == mInstance)
    {
        if (NULL == (mInstance = new CCompressFacade))
        {
			printf("Creat New Instance Error!\n"); 
            return NULL;
        }                                         
    }  
    
    return mInstance;    
}

/****************************************************************************
*  Function:       EncodeFile
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
long CCompressFacade::EncodeFile(FILE_NAME_LIST& file_name_list, const string& output_file_name, const string& password)
{
    return _impl->EncodeFile(file_name_list, output_file_name, password);
}

/****************************************************************************
*  Function:       DecodeFile
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
long CCompressFacade::DecodeFile(const string& compress_file_name, FILE_NAME_LIST& output_file_list, const string& default_path, const string& password)
{
    return _impl->DecodeFile(compress_file_name, output_file_list, default_path, password);
}

/****************************************************************************
*  Function:       BenchmarkSystem
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/

/*
long CCompressFacade::BenchmarkSystem()
{
    return _impl->BenchmarkSystem();
}
*/

/****************************************************************************
*  Function:       ShowCodeInfo
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
long CCompressFacade::ShowCodeInfo()
{
    return _impl->ShowCodeInfo();
}

/****************************************************************************
*  Function:       ListEncodeFile
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
long CCompressFacade::ListEncodeFile(const string& compress_file_name, const string& password)
{
    return _impl->ListEncodeFile(compress_file_name, password);
}

/****************************************************************************
*  Function:       GetEncodeFileList
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
long CCompressFacade::GetEncodeFileList(const string& compress_file_name, FILE_NAME_LIST& file_name_list)
{
    return _impl->GetEncodeFileList(compress_file_name, file_name_list);
}

/****************************************************************************
*  Function:       DeleteFileFromEncode
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/

long CCompressFacade::DeleteFileFromEncode(const string& compress_file_name, FILE_NAME_LIST& delete_file_list, const string& password)
{
    return _impl->DeleteFileFromEncode(compress_file_name, delete_file_list, password);
}


void ShowCompressInfo()
{
//		return 0;
 //   CCompressFacade::GetInstance()->ShowCodeInfo();
}

int GDecodeFile(const string& compress_file_name, const string& default_path, const string& password)
{
	return 0;
//    return CCompressFacade::GetInstance()->DecodeFile(compress_file_name, default_path, password);
}

int GEncodeFile(const string& file_name, const string& output_file_name, const string& password)
{
    FILE_NAME_LIST file_name_list;
    file_name_list.push_back(file_name);
		return 0;
//    return CCompressFacade::GetInstance()->EncodeFile(file_name_list, output_file_name, password);
}
int GListEncodeFile(const string& compress_file_name, const string& password)
{
		return 0;
//    return CCompressFacade::GetInstance()->ListEncodeFile(compress_file_name, password);
}





