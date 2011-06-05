#ifndef COMPRESS_FACE_H
#define COMPRESS_FACE_H
/****************************************************************************
*  File name:      	CompressFace.h
*  Author:       	guolisen
*  Version:   		v1.0
*  Create Date:    	2011-02-22
*  Description:    	Guolisen@gmail.com   
*  Change History:    
*  <author>    <time>     <version >      <desc>
*  Guolisen  2011-06-22                   
*****************************************************************************/
/*
注意：
    1. 由于代码可能调用层次较深，因此在Shell下直接输入命令可能导致任务栈崩溃。
    2. 解压、压缩时请注意RAMDisk 中剩余空间是否够用，否则会压缩失败
    3. 7z类型在压缩大文件时可能占用较大内存，导致内存分配失败
    4. 7z格式压缩率较高
    5. 解压支持RAR文件，可以在PC上使用WINRAR压缩后，在板子上解压
    6. 解压支持分卷，压缩也可支持分卷，但没有实现，如有需求将来再添加
    7. 默认路径，及路径相关部分由于可能应用较少，没有测试，建议压缩包中无路径信息
    8. 测试例程7z920\CPP\7zip\Bundles\VxZip\vx7ZipMain.txt
*/

#include <list>
#include <string>


using std::string;


typedef std::list<std::string>  FILE_NAME_LIST;

class CCompressFacadeImpl;
class CCompressFacade
{
private:
    CCompressFacade(const CCompressFacade& other);

public: 
	CCompressFacade();
	virtual ~CCompressFacade();
    
public:
    static CCompressFacade* GetInstance();

public: 
    long Init();
    long ShowCodeInfo();
    //long BenchmarkSystem();

    //支持压缩类型：7z、zip、gz、bz2、tar.gz、tar.bz2
    //gz、bz2不支持多文件打包
    //支持文件加密，password不为空时加密
    //压缩根据扩展名区别压缩类型，解压可以不需要考虑扩展名，代码可以自动识别解压
	long EncodeFile(FILE_NAME_LIST& file_name_list, 
                         const string& output_file_name, 
                         const string& password = "");  

    //支持解压类型：7z、zip、gz、bz2、tar.gz、tar.bz2、rar（其他解压类型可以按需要添加，例如iso、cab等）
    //由于eNodeb对路径要求不高，默认路径没有测，默认为空即可。
    //output_file_list参数为解压压缩包中特定文件，如果此列表为空则解压全部文件
	long DecodeFile(const string&		compress_file_name, 
	                    FILE_NAME_LIST&	output_file_list,
                        const string&	default_path = "", 
                        const string&	password = ""); 
                        
    //列出压缩文件中文件名
	long ListEncodeFile(const string& compress_file_name, 
                              const string& password = ""); 

    //返回的文件列表每项都带有路径信息(如果有)
    long GetEncodeFileList(const string& compress_file_name, FILE_NAME_LIST& file_name_list);

    //删除压缩文件中的若干文件
    long DeleteFileFromEncode(const string& compress_file_name,
                                      FILE_NAME_LIST& delete_file_list, 
                                      const string& password = "");

private:
    static CCompressFacade* mInstance;
    CCompressFacadeImpl* _impl;

};



#endif


