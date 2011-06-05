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
ע�⣺
    1. ���ڴ�����ܵ��ò�ν�������Shell��ֱ������������ܵ�������ջ������
    2. ��ѹ��ѹ��ʱ��ע��RAMDisk ��ʣ��ռ��Ƿ��ã������ѹ��ʧ��
    3. 7z������ѹ�����ļ�ʱ����ռ�ýϴ��ڴ棬�����ڴ����ʧ��
    4. 7z��ʽѹ���ʽϸ�
    5. ��ѹ֧��RAR�ļ���������PC��ʹ��WINRARѹ�����ڰ����Ͻ�ѹ
    6. ��ѹ֧�ַ־�ѹ��Ҳ��֧�ַ־���û��ʵ�֣����������������
    7. Ĭ��·������·����ز������ڿ���Ӧ�ý��٣�û�в��ԣ�����ѹ��������·����Ϣ
    8. ��������7z920\CPP\7zip\Bundles\VxZip\vx7ZipMain.txt
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

    //֧��ѹ�����ͣ�7z��zip��gz��bz2��tar.gz��tar.bz2
    //gz��bz2��֧�ֶ��ļ����
    //֧���ļ����ܣ�password��Ϊ��ʱ����
    //ѹ��������չ������ѹ�����ͣ���ѹ���Բ���Ҫ������չ������������Զ�ʶ���ѹ
	long EncodeFile(FILE_NAME_LIST& file_name_list, 
                         const string& output_file_name, 
                         const string& password = "");  

    //֧�ֽ�ѹ���ͣ�7z��zip��gz��bz2��tar.gz��tar.bz2��rar��������ѹ���Ϳ��԰���Ҫ��ӣ�����iso��cab�ȣ�
    //����eNodeb��·��Ҫ�󲻸ߣ�Ĭ��·��û�в⣬Ĭ��Ϊ�ռ��ɡ�
    //output_file_list����Ϊ��ѹѹ�������ض��ļ���������б�Ϊ�����ѹȫ���ļ�
	long DecodeFile(const string&		compress_file_name, 
	                    FILE_NAME_LIST&	output_file_list,
                        const string&	default_path = "", 
                        const string&	password = ""); 
                        
    //�г�ѹ���ļ����ļ���
	long ListEncodeFile(const string& compress_file_name, 
                              const string& password = ""); 

    //���ص��ļ��б�ÿ�����·����Ϣ(�����)
    long GetEncodeFileList(const string& compress_file_name, FILE_NAME_LIST& file_name_list);

    //ɾ��ѹ���ļ��е������ļ�
    long DeleteFileFromEncode(const string& compress_file_name,
                                      FILE_NAME_LIST& delete_file_list, 
                                      const string& password = "");

private:
    static CCompressFacade* mInstance;
    CCompressFacadeImpl* _impl;

};



#endif


