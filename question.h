#pragma once

#include "eval_manager.h"

class question_ans
{
public:
	std::string qid;
	std::vector<int> ans;
	boost::posix_time::ptime submission_time;
};

class eval_manager;

//题目内容，服务端启动时load
/*
class question
{
public:
	question();
	question(const question& ques);
	question(const std::string& id,
		const std::string& stem, 
		const std::vector<std::string>& options, 
		const std::vector<int>& answers,
		const std::vector<std::string>& img_names,
		const double& full_score);

	inline void set_ele_id(const std::string& id) { qid = id; }
	inline const std::string& ele_id() { return qid; }
	void set_stem(const std::string& stem);
	std::string& stem();
	void set_options(const std::vector<std::string>& options);
	std::vector<std::string>& options();
	void set_answers(const std::vector<int>& answers);
	std::vector<int>& answers();
	void set_img_names(const std::vector<std::string>& img_names);
	std::vector<std::string>& img_names();
	void set_full_score(double full_score);
	double full_score();

	void get_score(question_ans &ans, double &res);

	std::string qid;
	std::string stem_;//题目描述
	std::vector<std::string> options_;//选项
	std::vector<int> answers_;
	std::vector<std::string> img_names_;//带相对路径
	double full_score_;

	boost::shared_ptr<eval_manager> ele_mgr;
	std::string question_cut_id;
};*/

class question {
public:
  question();
  question(const question& ques);
  question(const std::string& id,
           const std::string& stem, 
           const std::vector<std::string>& options, 
           const std::vector<int>& answers,
           const std::vector<std::string>& img_names,
           const int& full_score,
           const boost::posix_time::ptime& entry_time);

  inline void set_qid(const std::string& id) { qid_ = id; }
  inline const std::string& qid() const { return qid_; }

  void set_stem(const std::string& stem);
  const std::string& stem() const;

  void set_options(const std::vector<std::string>& options);
  const std::vector<std::string>& options() const;

  void set_answers(const std::vector<int>& answers);
  const std::vector<int>& answers() const;

  void set_img_names(const std::vector<std::string>& img_names);
  const std::vector<std::string>& img_names() const;

  void set_full_score(const int full_score);
  const int full_score() const;

  void set_category(const std::string& categoty);
  const std::string& category() const;

  void set_role(const std::string& role);
  const std::string& role() const;

  void set_difficulty(const std::string& difficulty);
  const std::string& difficulty() const;

  void set_entry_time(const boost::posix_time::ptime& entry_time); // unimplemented
  void set_entry_time(const time_t& entry_time);
  const boost::posix_time::ptime& entry_time() const;

private:
  std::string qid_;
  std::string stem_;//题目描述
  std::string category_;
  std::string role_;
  std::string difficulty_;
  std::vector<std::string> options_;//选项
  std::vector<int> answers_;
  std::vector<std::string> img_names_;//带相对路径
  int full_score_;
  boost::posix_time::ptime entry_time_;

  boost::shared_ptr<eval_manager> ele_mgr_;
  std::string question_cut_id_;
};

typedef std::vector<question*>* QuestionListPtr;
typedef std::map<std::string,question*>* QuestionMapPtr;
