#include "stdafx.h"

#include "eval_manager.h"
#include "database_util.h"
#include "datafile_util.h"
#include "question.h"

using namespace std;

void eval_manager::load_quesitons()
{
  /*
	vector<question*> questions;
	std::map<std::string, question*> question_map;
	DatabaseUtil database;
	database.set_connection_string(_bstr_t("Provider=SQLOLEDB.1;Integrated Security=SSPI;"
		"Data Source=127.0.0.1,1433;Initial Catalog=Questions;Persist Security Info=False"));
	database.GetAllQuestions(questions, question_map);
  */
  vector<question*>* questions = new vector<question*>;
  std::map<std::string, question*>* question_map = new std::map<std::string, question*>;
  DatafileUtil data;
  data.set_file_path("questions.xml");
  data.GetAllQuestions(questions, question_map);
}

void eval_manager::push_user_ans( question_ans ans )
{
	boost::mutex::scoped_lock lock(user_answers_mutex);
	boost::mutex::scoped_lock lock2(ans_map_mutex);

	user_answers.push_back(ans);
	lock.unlock();

	ans_map[ans.qid] = &user_answers[user_answers.size() - 1];
	lock2.unlock();
}

void eval_manager::push_user_operaiton( operation op )
{
	boost::mutex::scoped_lock lock(user_operations_mutex);
	boost::mutex::scoped_lock lock2(opid_map_mutex);

	user_operations.insert(op);
	lock.unlock();

  operation* op_ptr = const_cast<operation*>(&(*user_operations.find(op)));
  opid_map.insert(make_pair<string, operation*>(op.opid, op_ptr));
  lock2.unlock();
}

void eval_manager::push_user_operaiton(std::vector<operation> ops)
{
  boost::mutex::scoped_lock lock(user_operations_mutex);
  boost::mutex::scoped_lock lock2(opid_map_mutex);

  for(size_t i = 0; i < ops.size(); i++)
  {
    user_operations.insert(ops[i]);
    operation* op = const_cast<operation*>(&(*user_operations.find(ops[i])));
    opid_map.insert(make_pair<string, operation*>(ops[i].opid, op));
  }
}

void eval_manager::delete_user_operation_by_id( std::string opid )
{
	boost::mutex::scoped_lock lock(user_operations_mutex);
	boost::mutex::scoped_lock lock2(opid_map_mutex);

	operation *op;
	user_operations.erase(op);
}

void eval_manager::get_all_evals()
{
	//foreach evals, check if NPC
}

bool eval_manager::translateStr2Op( const std::vector<std::string> &segs, std::vector<operation> &ops )
{
  vector<string> op_parts;
  for(int i=0;i<segs.size();i++)
  {
    op_parts.clear();
    TextFileUtil::split(segs[i], op_parts, ';');

    operation op;
    vector<string> strs;
    
    for(int j=0;j<op_parts.size();j++)
    {
      switch (j)
      {
      case 0://opid
        op.opid = op_parts[j];
        break;
      case 1://sub_opid
        break;
      case 6://string parameters
        break;
      case 7://double parameters
        strs.clear();
        TextFileUtil::split(op_parts[j], strs, ',');
        for(int k=0;k<strs.size();k++)
        {
          op.parameters2.push_back(boost::lexical_cast<double>(strs[k]));
        }
        break;
      default:
        break;
      }
    }

    ops.push_back(op);
  }

  return true;
}

void eval_manager::constr_evals(const std::vector<operation> &ops)
{
  for(int i=0;i<ops.size();i++)
  {
    map<string, person_evals*>::iterator it_pevals = personid_map.find(ops[i].personid);
    if (it_pevals != personid_map.end())
    {
      std::hash_map<std::string, criteria_op*>::iterator it_cri = 
        it_pevals->second->opid_map.find(ops[i].opid);

      if(it_cri != it_pevals->second->opid_map.end())
      {
        //composite
        it_cri->second->std_ops.push_back(ops[i]);
      }
      else
      {
        criteria_op cri_op;
        if(ops[i].sub_opid == "")//single
        {
          cri_op.op_id = ops[i].opid;
          cri_op.evl_type = eval_type::single;
          cri_op.std_op = ops[i];
        }
        else//composite
        {
          cri_op.op_id = ops[i].opid;
          cri_op.evl_type = eval_type::composite;
          cri_op.std_ops.push_back(ops[i]);
        }

        it_pevals->second->std_ops.push_back(cri_op);
        it_pevals->second->opid_map.insert(make_pair<string, criteria_op*>(cri_op.op_id, 
          &it_pevals->second->std_ops[it_pevals->second->std_ops.size()-1]));
      }
    }
    else
    {
      person_evals pevals;
      pevals.personid = ops[i].personid;

      criteria_op cri_op;
      if(ops[i].sub_opid == "")//single
      {
        cri_op.op_id = ops[i].opid;
        cri_op.evl_type = eval_type::single;
        cri_op.std_op = ops[i];
      }
      else//composite
      {
        cri_op.op_id = ops[i].opid;
        cri_op.evl_type = eval_type::composite;
        cri_op.std_ops.push_back(ops[i]);
      }

      pevals.std_ops.push_back(cri_op);
      pevals.opid_map.insert(make_pair<string, criteria_op*>(cri_op.op_id, 
        &pevals.std_ops[pevals.std_ops.size()-1]));

      evals.push_back(pevals);
      personid_map.insert(make_pair<string, person_evals*>(pevals.personid, &evals[evals.size()-1]));
    }
  }
}