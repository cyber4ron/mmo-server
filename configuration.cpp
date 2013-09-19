#include "stdafx.h"

#include "configuration.h"
#include "entity_facade.h"

using namespace std;

configuration::configuration()
{}

configuration::configuration(const std::string &ph, const std::string &st): path(ph), script(st)
{
	/*this->path = path;
	this->script = script;*/
}

void configuration::setPath(const std::string &path)
{
	this->path = path;
}


void configuration::setScript(const std::string &script)
{
	this->script = script;
}

void configuration::configure()
{
	TiXmlDocument* doc; 
	TiXmlElement* root;
  
	doc = new TiXmlDocument(path.c_str());
 	if(!doc -> LoadFile ()) return;
	root = doc -> RootElement();
  
	configureEntities(root);
	configureStates(root);
	configureTransitions(root);
	configureExpression(root);
	configureExpressionTransitions(root);
	configureExpressionStateTransitions(root);

	delete doc;
}

void configuration::configure(const std::string &path, const std::string &script)
{
	setPath(path);
	setScript(script);
	configure();
}

void configuration::configureEntities(TiXmlElement* root)
{
	std::string prop, pvalue;
	TiXmlNode* entityNode = TinyXPath::XNp_xpath_node(root, "//entity");
	while(entityNode)
	{
	    //std::cout << entityNode->Value() << std::endl;
		TiXmlNode* propertyNode = TinyXPath::XNp_xpath_node(entityNode, "./*");
		std::map<std::string, std::string> facade;
		while(propertyNode)
		{
			prop = propertyNode->Value();
			TiXmlNode* vnode = TinyXPath::XNp_xpath_node(propertyNode, "./text()");
			facade[prop] = std::string(vnode->Value());
			propertyNode = propertyNode->NextSiblingElement();
		}
		this->entities.push_back(new entity_facade(facade.cbegin(), facade.cend()));	
		entityNode = entityNode->NextSiblingElement();
	}
}

void configuration::configureStates(TiXmlElement* root)
{	
	std::string stateName;
	TiXmlNode* stateNode = TinyXPath::XNp_xpath_node(root, "//state");
	while(stateNode)
	{
		TiXmlNode* nameNode = TinyXPath::XNp_xpath_node(stateNode, "./text()");
		TiXmlAttribute* func = TinyXPath::XAp_xpath_attribute(stateNode, "./@func");
		TiXmlAttribute* branch = TinyXPath::XAp_xpath_attribute(stateNode,"./@branch");
		this->states.push_back(std::string(nameNode->Value()) + "," + std::string(func->Value()) + "!" + std::string(branch->Value()));	
		stateNode = stateNode->NextSiblingElement();
	}
}

void configuration::configureTransitions(TiXmlElement* root)
{
	TiXmlNode* transition = TinyXPath::XNp_xpath_node(root, "//transition");
	while(transition)
	{
		TiXmlAttribute* backward = TinyXPath::XAp_xpath_attribute(transition, "./@backward");
		TiXmlAttribute* forward = TinyXPath::XAp_xpath_attribute(transition, "./@forward");
		try
		{
			int bw = boost::lexical_cast< int >(backward->Value());
			int fw = boost::lexical_cast< int >(forward->Value());
			this->transitions.push_back(std::make_pair(bw, fw));
		}
		catch(const boost::bad_lexical_cast & e)
		{
			std::cout << "can not cast to int!" << std::endl;
		}
		transition = transition->NextSiblingElement();
	}  			
} 	

void configuration::configureExpression( TiXmlElement* root )
{
	std::string exprName;
	TiXmlNode* exprNode = TinyXPath::XNp_xpath_node(root, "//expression");
	while(exprNode)
	{
		TiXmlNode* nameNode = TinyXPath::XNp_xpath_node(exprNode, "./text()");
		TiXmlAttribute* type = TinyXPath::XAp_xpath_attribute(exprNode, "./@type");
		TiXmlAttribute* fact = TinyXPath::XAp_xpath_attribute(exprNode,"./@fact");
		this->expressions.push_back(std::string(nameNode->Value()) + "," + std::string(type->Value()) + "!" + std::string(fact->Value()));	
		exprNode = exprNode->NextSiblingElement();
	}
}

void configuration::configureExpressionTransitions( TiXmlElement* root )
{
	TiXmlNode* transition = TinyXPath::XNp_xpath_node(root, "//extransition");
	while(transition)
	{
		TiXmlAttribute* type = TinyXPath::XAp_xpath_attribute(transition, "./@type");
		TiXmlAttribute* backward = TinyXPath::XAp_xpath_attribute(transition, "./@backward");
		TiXmlAttribute* forward = TinyXPath::XAp_xpath_attribute(transition, "./@forward");
		try
		{
			std::string typeName = std::string(type->Value());
			int bw = boost::lexical_cast< int >(backward->Value());
			int fw = boost::lexical_cast< int >(forward->Value());
			struct tranNode node = {typeName,bw,fw};
			this->extransitions.push_back(node);
		}
		catch(const boost::bad_lexical_cast & e)
		{
			std::cout << "the transition between expressions is failed!" << std::endl;
		}
		transition = transition->NextSiblingElement();
	}  
}

void configuration::configureExpressionStateTransitions( TiXmlElement* root )
{
	TiXmlNode* transition = TinyXPath::XNp_xpath_node(root, "//exstate");
	while(transition)
	{
		TiXmlAttribute* type = TinyXPath::XAp_xpath_attribute(transition, "./@type");
		TiXmlAttribute* backward = TinyXPath::XAp_xpath_attribute(transition, "./@backward");
		TiXmlAttribute* forward = TinyXPath::XAp_xpath_attribute(transition, "./@forward");
		try
		{
			std::string typeName = std::string(type->Value());
			int bw = boost::lexical_cast< int >(backward->Value());
			int fw = boost::lexical_cast< int >(forward->Value());
			struct tranNode node = {typeName,bw,fw};
			this->exstates.push_back(node);
		}
		catch(const boost::bad_lexical_cast & e)
		{
			std::cout << "the transition between expression and state is failed!" << std::endl;
		}
		transition = transition->NextSiblingElement();
	}  
}



