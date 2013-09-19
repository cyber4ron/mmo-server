#include "stdafx.h"

#include "script_manager.h"

using namespace luabind;
using namespace std;

script_manager::script_manager( boost::shared_ptr<server> svr_p ) : m_taskQueue(), m_worker(m_taskQueue,1)
{
	server_ptr = svr_p;
	io_s_ptr = svr_p->io_service_ptr;
}

void script_manager::attach_scripts()//by config
{
	FILE *fp = freopen("drill\\luafunctions.txt", "r", stdin); 
	std::string str_func, str_ent_ids;
	while (std::cin >> str_func >> str_ent_ids)
	{
		vector<string> ids;
		if ("#" == str_ent_ids)
		{
			// Ã»ÓÐ½ÇÉ«ID
		}
		else
		{
			string tmp_str = "";
			for (int i=0,len=str_ent_ids.length();i<=len;i++)
			{
				if (i==len || str_ent_ids[i]==',')
				{
					ids.push_back(tmp_str);
					tmp_str = "";
				}
				else tmp_str += str_ent_ids[i];
			}
		}
		attach_script(str_func + ".lua", str_func, ids);
	}
	fclose(fp);
}

void script_manager::attach_script(std::string file_name, std::string entry, std::vector<std::string> entity_ids)
{
	boost::shared_ptr<script> scp = boost::make_shared<script>(shared_from_this(), file_name, entry, server_ptr->ent_interface, server_ptr->ie_ptr);
	scripts.push_back(scp);

	script_map[file_name+"::"+entry] = scp;

	for(int i=0;i<entity_ids.size();i++)
		entity_id_map.insert(make_pair<string, boost::weak_ptr<script>>(entity_ids[i], scp));

	function_map[entry] = scp;
}

void script_manager::resume_script_by_name( std::string script_name )
{
	boost::shared_ptr<script> scp = script_map[script_name].lock();
	if(scp) scp->resume();
}

void script_manager::resume_script_by_name_async( std::string script_name )
{
	boost::shared_ptr<script> scp = script_map[script_name].lock();
	if(scp) async_call(boost::bind(&script::resume, scp));
}

void script_manager::resume_script_by_name_async( std::string script_name, std::string branch )
{
	boost::shared_ptr<script> scp = script_map[script_name].lock();
	if(scp)
	{
		scp->set_func_branch(get_func_name_by_script(script_name), branch);
		async_call(boost::bind(&script::resume, scp));
	}
}

void script_manager::yield_script_by_name(std::string script_name)
{
	boost::shared_ptr<script> scp = script_map[script_name].lock();
	if(scp) scp->yield();
}

void script_manager::end_script_by_name( std::string script_name )
{
	boost::shared_ptr<script> scp = script_map[script_name].lock();
	if(scp) scp->end();
}

void script_manager::run()
{
	m_worker.start();
}

void script_manager::async_call( task_type task )
{
	m_taskQueue.push(task);
}

std::string script_manager::get_func_name_by_script( std::string scr_name )
{
	int idx = scr_name.find("::");
	return scr_name.substr(idx + 2, scr_name.length() - idx - 2);
}

void script_manager::set_func_real_person( std::string ent_id )
{
	std::multimap<std::string, boost::weak_ptr<script>>::iterator it = entity_id_map.lower_bound(ent_id);
	for(;it!=entity_id_map.upper_bound(ent_id);it++)
	{
		boost::shared_ptr<script> scp = it->second.lock();
		if(scp) scp->set_var("is_real_person_" + ent_id, "true");
	}
}

void script_manager::reset_func_real_person( std::string ent_id )
{
	std::multimap<std::string, boost::weak_ptr<script>>::iterator it = entity_id_map.lower_bound(ent_id);
	for(;it!=entity_id_map.upper_bound(ent_id);it++)
	{
		boost::shared_ptr<script> scp = it->second.lock();
		if(scp) scp->set_var("is_real_person_" + ent_id, "false");
	}
}

void script_manager::set_func_branch( std::string func_name, std::string branch )
{
	boost::shared_ptr<script> scp = function_map[func_name].lock();
	if(scp) scp->set_var(func_name+"_branch_label", branch);
}
