/********************************************************************
	Author:		Wang Yu
	Created:	2013/07/10
	Revised:	10:7:2013   16:50
	File Base:	datafile_util
	
	Purpose:	文件操作有关的实体类。
            提供数据文件的读取、修改、保存，并提供公共接口可以获取题目内容。
*********************************************************************/
#ifndef DATAFILE_UTIL_H_
#define DATAFILE_UTIL_H_

#include <string>
#include <vector>
#include <map>
#include "question.h"
using namespace std;

struct tm;
class TiXmlElement;

// Sample usage:
// DatafileUtil instance;
// instance.set_file_path("myfile_path");
// instance.GetAllQuestions(questions, question_map);
class DatafileUtil {
 public:
  DatafileUtil() {}
  ~DatafileUtil() {}
  std::string file_path() const { return file_path_; }
  void set_file_path(const std::string& path) { file_path_ = path; }
  int GetAllQuestions(QuestionListPtr question_list_ptr, 
                      QuestionMapPtr question_map_ptr);
  question* CreateNewQuestion(TiXmlElement* ques_element);
 private:
  //将文件中字符串形式的日期转化为time_t
  time_t ParseTimeT(const char* time_in_string);
  void ParseOptionsAndAnswers(const TiXmlElement& ques_element, //题目根元素节点
                              vector<string>* options, 
                              vector<int>* answers);
  void ParseImageNames(const char* images, vector<string>* img_names);
  char* UTF8ToGBK(const char* in);
  std::string file_path_;
};


class TextFileUtil
{
public:
  bool parseFile(std::string file, std::vector<std::string> &segs);
  static void split(const std::string str, std::vector<std::string> &vec, char token);
};

#endif