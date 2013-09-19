/********************************************************************
	Author:		Wang Yu
	Created:	2013/07/10
	Revised:	2013/07/10   17:23
	File Base:	datafile_util
	
	Purpose:	datafile_util.h的实现类。
*********************************************************************/

#include "stdafx.h"
#include "datafile_util.h"
#include "question.h"
#include <vector>
#include <map>
#include <tinyxml.h>
#include <ctime>
#include <windows.h>


// 读取数据库中所有的题目，存放在questions和question_map中
// question_map是以题目id为key建立的map索引
// 返回题目总数，如果发生异常，则返回-1
int DatafileUtil::GetAllQuestions(QuestionListPtr question_list_ptr, 
                                  QuestionMapPtr question_map_ptr) {
   TiXmlDocument doc(file_path_.c_str());
  if(doc.LoadFile(TIXML_ENCODING_UTF8)== NULL) 
    return 0;
  TiXmlElement* root = doc.RootElement();
  TiXmlElement* ques_element = root->FirstChildElement("question");
  int ques_count = 0;
  
  while(ques_element != NULL) {
    question* ques = CreateNewQuestion(ques_element);
    question_list_ptr->push_back(ques);
    question_map_ptr->insert(make_pair(ques->qid(),ques));
    ques_element = ques_element->NextSiblingElement();
    ++ques_count;
  }
  // 不需要手动释放，TiXmlDocument析构时会自动delete
  // 如果是用指针doc = new TiXmlDocument(...)，这时需要手动delete
  //delete root;
  //delete ques_element;
  return ques_count;
}

question* DatafileUtil::CreateNewQuestion(TiXmlElement* ques_element) {
  question* ques = new question();
  ques->set_qid(ques_element->FirstChildElement("id")->GetText());

  char* category = UTF8ToGBK(
      ques_element->FirstChildElement("category")->GetText());
  ques->set_category(category);

  char* role = UTF8ToGBK(
    ques_element->FirstChildElement("role")->GetText()); 
  ques->set_role(role);

  char* difficulty = UTF8ToGBK(
    ques_element->FirstChildElement("difficulty")->GetText());
  ques->set_difficulty(difficulty);

  int full_score;
  sscanf_s(ques_element->FirstChildElement("score")->GetText(), "%d", &full_score);
  ques->set_full_score(full_score);

  char* stem = UTF8ToGBK(
    ques_element->FirstChildElement("body")->GetText());
  ques->set_stem(stem);

  vector<string>* options = new vector<string>;
  vector<int>* answers = new vector<int>;
  ParseOptionsAndAnswers(*ques_element, options, answers);
  ques->set_options(*options);
  ques->set_answers(*answers);
  ques->set_entry_time(ParseTimeT(
    ques_element->FirstChildElement("entryTime")->GetText()));

  vector<string>* img_names = new vector<string>;
  ParseImageNames(ques_element->FirstChildElement("picPath")->GetText(),
    img_names);
  ques->set_img_names(*img_names);
  delete options;
  delete answers;
  delete img_names;
  return ques;
}

time_t DatafileUtil::ParseTimeT(const char* time_in_string) {
  time_t entry_time;
  sscanf_s(time_in_string, "%lld", &entry_time);
  return entry_time;
}

void DatafileUtil::ParseOptionsAndAnswers(const TiXmlElement& ques_element, 
                                          vector<string>* options,
                                          vector<int>* answers) {
  const TiXmlElement* option = ques_element.FirstChildElement("option");
  while(option != NULL) {
    int index = 0;
    options->push_back(option->GetText());
    if(option->Attribute("answer") != NULL) {
      answers->push_back(index);
    }
    option = option->NextSiblingElement("option");
    ++index;
  }
}

void DatafileUtil::ParseImageNames(const char* images, 
                                   vector<string>* img_names) {
    while(images != NULL && *images != '\0') {
      const char* start = images;
      while(*images != ';') { images++; }
      size_t length = (images - start + 1) / sizeof(char);
      char* file_name = new char[length];
      copy(start, images, file_name);
      file_name[length - 1] = '\0';
      img_names->push_back(file_name);
      //copy(start, images, img_names->rbegin());
      ++images;
  }
}

char* DatafileUtil::UTF8ToGBK(const char* in)
{
  //只是为了获得长度，没有转化
  int len = MultiByteToWideChar(CP_UTF8, 0, in, -1, NULL, 0);
  unsigned short* wszGBK = new unsigned short[len + 1];
  memset(wszGBK, 0, len * 2 + 2);
  MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)in, -1, (LPWSTR)wszGBK, len);

  len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
  char* szGBK = new char[len + 1];
  memset(szGBK, '\0', len + 1);
  WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);

  //将szGBK的内容赋给szOut ，这样即使szGBK被释放也能得到正确的值
  //memset(out,'\0',strlen(szGBK)+1); 
  //memcpy(out,szGBK,strlen(szGBK));

  //delete []szGBK;
  delete []wszGBK;
  return szGBK;
}


//TextFileUtil

bool TextFileUtil::parseFile( std::string file, std::vector<std::string> &segs )
{
  FILE *fp = freopen(file.c_str(), "r", stdin); 

  if(!fp) return false;

  string temp;
  while(cin>>temp)
  {
    segs.push_back(temp);
  }

  fclose(fp);

  return true;
}

void TextFileUtil::split( const std::string str, std::vector<std::string> &vec, char token )
{
  string temp;
  for(int i=0;i<str.size();i++)
  {
    if(str[i] == token || str[i] == '\0')
    {
      temp += '\0';
      vec.push_back(temp);
      temp.clear();
    }
    else
    {
      temp += str[i];
    }
  }
}
