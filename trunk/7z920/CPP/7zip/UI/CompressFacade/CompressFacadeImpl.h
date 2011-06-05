#ifndef COMPRESS_FACE_IMPL_H
#define COMPRESS_FACE_IMPL_H
/****************************************************************************
*  File name:      	CompressFace.h
*  Author:       	guolisen
*  Version:   		v1.0
*  Create Date:    	2011-02-22
*  Description:    	Guolisen@gmail.com   
*  Change History:    
*  <author>  <time>   <version >   <desc>
* 
*****************************************************************************/


#include <list>
#include <string>


using std::string;


enum VxEncodeType
{
    eVxWorkFileAdd,
    eVxWorkFileDelete,
    eVxWorkFileUpdate
};

class CCompressFacadeImpl
{
public: 
    CCompressFacadeImpl(void);
	virtual ~CCompressFacadeImpl(void);
    
public: 
    long Init();
    long ShowCodeInfo();
    //long BenchmarkSystem();

    //支持压缩类型：7z、zip、gz、bz2、tar.gz、tar.bz2
    //gz、bz2不支持多文件打包
    //支持文件加密，password不为空时加密
    //压缩根据扩展名区别压缩类型，解压可以不需要考虑扩展名，代码可以自动识别解压
	long EncodeFile(FILE_NAME_LIST& file_name_list, 
                         const string&  compress_file_name, 
                         const string&  password = "");  

    //支持解压类型：7z、zip、gz、bz2、tar.gz、tar.bz2、rar（其他解压类型可以按需要添加，例如iso、cab等）
    //由于eNodeb对路径要求不高，默认路径没有测，默认为空即可。
	long DecodeFile(const string&			compress_file_name, 
	                    FILE_NAME_LIST&   	output_file_list,
                        const string&		default_path = "", 
                        const string&		password = ""); 
    
	long ListEncodeFile(const string& compress_file_name, 
                              const string& password = ""); 

    //返回的文件列表每项都带有路径信息(如果有)
    long GetEncodeFileList(const string& compress_file_name, FILE_NAME_LIST& file_name_list);

    long DeleteFileFromEncode(const string& compress_file_name, 
                                      FILE_NAME_LIST& delete_file_list, 
                                      const string& password = "");

protected:
	long inEncodeFile(FILE_NAME_LIST& file_name_list, 
                          const string&   compress_file_name, 
                          const string&   password = "",
                          VxEncodeType    enc_type = eVxWorkFileAdd); 


protected:
    CCodecs*      mCodecs;
    CStdOutStream mStdStream;

};



#endif


