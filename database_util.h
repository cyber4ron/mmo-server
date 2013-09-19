/********************************************************************
	Author:		Wang Yu
	Created:	2013/06/23
	Revised:	2013/07/10   14:19
	File Base:	database_util
	
	Purpose:	���ڴ��������ݿ���ز�����ʵ���ࡣ
            ��Ҫ����ά�������ݿ�����ӣ����ṩ��һЩ�����ӿڹ�ʹ����ֱ�ӻ���������
*********************************************************************/
#ifndef DATABASE_UTIL_H_
#define DATABASE_UTIL_H_

#include <icrsint.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
using namespace std;

#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")
//������EOF�Է����Ƴ�ͻ

class question;

class DatabaseUtil {
 public:
  DatabaseUtil();
  explicit DatabaseUtil(_bstr_t& conn_str);
  ~DatabaseUtil();
  inline _bstr_t connection_string() const { return connection_string_; }
  inline void set_connection_string(const _bstr_t& conn_str) { 
    connection_string_ = conn_str; }

  // ��ȡ���ݿ������е���Ŀ�������questions��question_map��
  // question_map������ĿidΪkey������map����
  // ������Ŀ��������������쳣���򷵻�-1
  int GetAllQuestions(vector<question*>& questions, 
                      map<std::string, question*>& question_map); 
  void Init();

 private:
   // ���������ַ����������ݿ����ӣ��ɹ�����true��ʧ�ܷ���false
   bool OpenConnection();
   //�Ӽ�¼���ж�ȡ�ֶε�ֵ
   _variant_t GetFieldValueByName(const _RecordsetPtr& recordset, 
                                  _variant_t& field_name);
   _CommandPtr CreateCommandPtr(const char* command_text);
   //��ȡ��Ŀ��������ѡ���
   int GetQuestionsDetail(vector<question*>& questions, 
                          map<std::string/*id*/, question*>& question_map);
  //��ȡ��ĿͼƬ�����浽�����ļ����У�������Ŀ��Ӧ��ͼƬ����д����Ŀ���ֶ���
   int GetQuestionImgs(question& ques);
   //������XMl��ǩ����Ŀ����reference��ʽ���ص���Ӧ�Ľṹ��
   void ParseXmlQuesToString(const std::string& ques_xml,
                             std::string& stem, 
                             vector<std::string>& options, 
                             vector<int>& answers); 
   //�Ӹ������ļ�·������ȡ���ļ�����,δʵ��
   std::string ExtractFileName(const std::string& path);
   //��������_variant_t���͵ı���ת��Ϊstd::string����
   std::string ConvertVariantToString(_variant_t field);
   double ConvertVariantToDouble(_variant_t field);
   int ConvertVariantToInt32(_variant_t field);

   //�����ַ���
   _bstr_t connection_string_;
   _ConnectionPtr connection_;
   //�಻�����ƺ͸�ֵ���μ�stdafx.h
   DISALLOW_COPY_AND_ASSIGN(DatabaseUtil);
};
#endif  //DATABASE_UTIL_H_