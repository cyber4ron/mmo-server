#include "stdafx.h"
#include "database_util.h"
#include "question.h"
#include <tinyxml.h>
#include <sstream>

DatabaseUtil::DatabaseUtil() {
  Init();
}

DatabaseUtil::DatabaseUtil(_bstr_t& conn_str) : connection_string_(conn_str) {
  Init();
}

DatabaseUtil::~DatabaseUtil() {
  CoUninitialize();
}

void DatabaseUtil::Init() {
  CoInitialize(NULL);
}

bool DatabaseUtil::OpenConnection() {
  //未设置连接字符串
  if(connection_string_.length() == 0) return false;                         
  //连接指针实例化失败
  if(FAILED(connection_.CreateInstance(__uuidof(Connection)))) return false;  
  try {
    connection_->Open(connection_string_, "", "", adModeUnknown);
  } catch (_com_error& e) {
    cout << e.Description() << endl;
    return false;
  }
  return true;
}

int DatabaseUtil::GetAllQuestions(
  std::vector<question*>& questions,
  std::map<std::string,question*>& question_map) {
    if(!OpenConnection()) return -1; //打开连接失败 
    int ques_count = GetQuestionsDetail(questions, question_map);
    if (ques_count > 0) {
      for (size_t i = 0; i < questions.size(); i++) {
        GetQuestionImgs(*questions[i]);
      }
    }
    connection_->Close();
    return ques_count > 0 ? ques_count : -1;
}

_CommandPtr DatabaseUtil::CreateCommandPtr(const char* command_text) {
  _CommandPtr command;
  if (FAILED(command.CreateInstance(__uuidof(Command)))) return -1;        //命令指针实例化失败
  command->Prepared = true;
  command->ActiveConnection = connection_;
  command->CommandText = command_text;
  return command;
}

// 获得除题目图片之外的所有相关信息，存放在相应的结构中
int DatabaseUtil::GetQuestionsDetail(std::vector<question*>& questions,
  std::map<std::string,question*>& question_map) {
    _CommandPtr command = CreateCommandPtr(
      "SELECT * FROM Questions WHERE isdelete = 0");
    _RecordsetPtr recordset; //记录集对象指针，用来执行SQL语句并记录查询结果
    int record_count = 0;
    try {
      recordset = command->Execute(NULL, NULL, adCmdText);
      recordset->MoveFirst();
      while (!recordset->adoEOF) {
        //查询Questions中相关的列
        string id = ConvertVariantToString(GetFieldValueByName(
          recordset, _variant_t("id")));
        string ques_xml = ConvertVariantToString(GetFieldValueByName(
          recordset, _variant_t("detail")));
        //string pic_path = ConvertVariantToString(GetFieldValueByName(recordset, _variant_t("picpath")));
        int full_score = ConvertVariantToInt32(GetFieldValueByName(
          recordset, _variant_t("score")));
        question* ques = new question();
        ques->set_qid(id);
        ques->set_full_score(full_score);
        string stem;
        std::vector<std::string> options;
        std::vector<int> answers;
        ParseXmlQuesToString(ques_xml, stem, options, answers);
        ques->set_stem(stem);
        ques->set_options(options);
        ques->set_answers(answers);
        questions.push_back(ques);
        question_map.insert(make_pair(id, ques));

        recordset->MoveNext();
        record_count++;
      } 
    } catch (_com_error& e) { 
      cout << e.Description() << endl;
      recordset->Close();
      return -1;
    }
    recordset->Close();
    return record_count;
}

int DatabaseUtil::GetQuestionImgs(question& ques) {
  _CommandPtr command = CreateCommandPtr(
    "SELECT picture FROM Pictures WHERE qid = ?");

  _ParameterPtr param = command->CreateParameter(
    _bstr_t("qid"), 
    adVarChar, 
    adParamInput, 
    20, 
    _variant_t(ques.qid().c_str()));
  param->PutValue(_variant_t(ques.qid().c_str()));
  command->Parameters->Append(param);

  _StreamPtr stream;
  stream.CreateInstance(__uuidof(Stream));
  _variant_t varOptional(DISP_E_PARAMNOTFOUND, VT_ERROR);
  stream->PutType(adTypeBinary);  //类型为二进制

  _RecordsetPtr recordset;
  int img_count = 0;

  std::vector<string> img_names;

  try {
    //使用Execute方法记录集指针无法回撤，只能向前移动
    recordset = command->Execute(NULL, NULL, adCmdUnknown);
    if(!recordset->BOF) {
      //recordset->MoveFirst();
      while(!recordset->adoEOF) {
        //long img_size = recordset->GetFields()->GetItem("picture")->ActualSize;
        stream->Open(varOptional, adModeUnknown, 
          adOpenStreamUnspecified, 
          _bstr_t(), _bstr_t());
        // 将图像二进制流写入stream中
        stream->Write(_variant_t(recordset->GetFields()->GetItem("picture")->Value));
        stringstream str_stream;
        string img_path ,img_name;
        str_stream << ques.qid() << "_" << img_count << ".jpg";
        str_stream >> img_name;
        //img_name = str_stream.str();
        img_path = "H:\\images\\" + img_name;
        img_names.push_back(img_name);
        stream->SaveToFile(_bstr_t(img_path.c_str()), adSaveCreateOverWrite);
        stream->Close();
        recordset->MoveNext();
        img_count++;
      }
      ques.set_img_names(img_names);
    }
  } catch (_com_error& e) {
    cout << e.Description() << endl;
    recordset->Close();
    return -1;
  }
  recordset->Close();
  return img_count;
}

void DatabaseUtil::ParseXmlQuesToString(const std::string& ques_xml,
  std::string& stem, 
  vector<std::string>& options, 
  vector<int>& answers) {
    TiXmlDocument* doc = new TiXmlDocument();
    //cout << ques_xml.c_str() << endl;
    doc->Parse(ques_xml.c_str(), 0, TIXML_ENCODING_UTF8);
    TiXmlElement* root = doc->RootElement();
    TiXmlElement* body = root->FirstChildElement();
    stem = body->FirstChild()->Value();

    TiXmlElement* option = body->NextSiblingElement();
    int option_index = 0;
    while(option != NULL) {
      options.push_back(option->FirstChild()->Value());
      if(option->Attribute("answer")) answers.push_back(option_index);
      option = option->NextSiblingElement();
      option_index++;
    }
}

_variant_t DatabaseUtil::GetFieldValueByName(
    const _RecordsetPtr& recordset,
    _variant_t& field_name) {
  return recordset->Fields->GetItem(field_name)->Value;
}

std::string DatabaseUtil::ConvertVariantToString(_variant_t field) {
  return static_cast<char*>(static_cast<_bstr_t>(field));
}

double DatabaseUtil::ConvertVariantToDouble(_variant_t field) {
  return static_cast<double>(field);
}

int DatabaseUtil::ConvertVariantToInt32(_variant_t field) {
  return static_cast<int>(field);
}