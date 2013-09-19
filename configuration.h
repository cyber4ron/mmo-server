#ifndef __configuration_h_
#define __configuration_h_

class entity_facade;


struct tranNode 
{
	std::string type;
	int backward;
	int forward;
};

class configuration
{
public :
	configuration();
	configuration(const std::string &path, const std::string &script);
	void configure();
	void configure(const std::string &path, const std::string &script);

	void setPath(const std::string &path);
	std::string getPath() const {return path;}
	void setScript(const std::string &script);
	std::string getScript() const {return script;}

	std::vector< std::pair<int, int> > transitions;
	std::vector<std::string> states;
	std::vector<entity_facade*> entities;
	std::vector<std::string> expressions;
	std::vector<struct tranNode> extransitions;
	std::vector<struct tranNode> exstates;

private:
	void configureEntities(TiXmlElement* root);
	void configureStates(TiXmlElement* root);
	void configureTransitions(TiXmlElement* root);
	void configureExpression(TiXmlElement* root);
	void configureExpressionTransitions(TiXmlElement* root);
	void configureExpressionStateTransitions(TiXmlElement* root);

	std::string path;
	std::string script;
};

#endif