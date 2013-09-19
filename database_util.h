/********************************************************************
	Author:		Wang Yu
	Created:	2013/06/23
	Revised:	2013/07/10   14:19
	File Base:	database_util
	
	Purpose:	用于处理与数据库相关操作的实体类。
            主要用来维护与数据库的连接，并提供了一些公共接口供使用者直接获得相关数据
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
//重命名EOF以防名称冲突

class question;

class DatabaseUtil {
 public:
  DatabaseUtil();
  explicit DatabaseUtil(_bstr_t& conn_str);
  ~DatabaseUtil();
  inline _bstr_t connection_string() const { return connection_string_; }
  inline void set_connection_string(const _bstr_t& conn_str) { 
    connection_string_ = conn_str; }

  // 读取数据库中所有的题目，存放在questions和question_map中
  // question_map是以题目id为key建立的map索引
  // 返回题目总数，如果发生异常，则返回-1
  int GetAllQuestions(vector<question*>& questions, 
                      map<std::string, question*>& question_map); 
  void Init();

 private:
   // 根据连接字符串，打开数据库连接，成功返回true，失败返回false
   bool OpenConnection();
   //从记录集中读取字段的值
   _variant_t GetFieldValueByName(const _RecordsetPtr& recordset, 
                                  _variant_t& field_name);
   _CommandPtr CreateCommandPtr(const char* command_text);
   //获取题目的描述、选项及答案
   int GetQuestionsDetail(vector<question*>& questions, 
                          map<std::string/*id*/, question*>& question_map);
  //获取题目图片，保存到本地文件夹中，并将题目对应的图片名称写入题目的字段中
   int GetQuestionImgs(question& ques);
   //解析带XMl标签的题目，以reference形式返回到相应的结构中
   void ParseXmlQuesToString(const std::string& ques_xml,
                             std::string& stem, 
                             vector<std::string>& options, 
                             vector<int>& answers); 
   //从给定的文件路径中提取出文件名称,未实现
   std::string ExtractFileName(const std::string& path);
   //将给定的_variant_t类型的变量转换为std::string类型
   std::string ConvertVariantToString(_variant_t field);
   double ConvertVariantToDouble(_variant_t field);
   int ConvertVariantToInt32(_variant_t field);

   //连接字符串
   _bstr_t connection_string_;
   _ConnectionPtr connection_;
   //类不允许复制和赋值，参见stdafx.h
   DISALLOW_COPY_AND_ASSIGN(DatabaseUtil);
};
#endif  //DATABASE_UTIL_H_