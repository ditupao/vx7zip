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

    //֧��ѹ�����ͣ�7z��zip��gz��bz2��tar.gz��tar.bz2
    //gz��bz2��֧�ֶ��ļ����
    //֧���ļ����ܣ�password��Ϊ��ʱ����
    //ѹ��������չ������ѹ�����ͣ���ѹ���Բ���Ҫ������չ������������Զ�ʶ���ѹ
	long EncodeFile(FILE_NAME_LIST& file_name_list, 
                         const string&  compress_file_name, 
                         const string&  password = "");  

    //֧�ֽ�ѹ���ͣ�7z��zip��gz��bz2��tar.gz��tar.bz2��rar��������ѹ���Ϳ��԰���Ҫ��ӣ�����iso��cab�ȣ�
    //����eNodeb��·��Ҫ�󲻸ߣ�Ĭ��·��û�в⣬Ĭ��Ϊ�ռ��ɡ�
	long DecodeFile(const string&			compress_file_name, 
	                    FILE_NAME_LIST&   	output_file_list,
                        const string&		default_path = "", 
                        const string&		password = ""); 
    
	long ListEncodeFile(const string& compress_file_name, 
                              const string& password = ""); 

    //���ص��ļ��б�ÿ�����·����Ϣ(�����)
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


