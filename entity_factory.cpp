#include "stdafx.h"

#include "entity_factory.h"
#include "entity.h"

//class role_operator;
//class commonEntity;
//class dist_fire;
//class dialog;

boost::shared_ptr<entity> entity_factory::createEntity(entity_facade* facade)
{
	if(facade->getClassName() == "role_operator")
		return boost::make_shared<role_operator>(facade);
	if(facade->getClassName() == "commonEntity")
		return boost::make_shared<commonEntity>(facade);
	if(facade->getClassName() == "valve")
		return boost::make_shared<valve>(facade);
	if(facade->getClassName() == "button")
		return boost::make_shared<button>(facade);
	if(facade->getClassName() == "dist_fire")
		return boost::make_shared<dist_fire>(facade);
	if(facade->getClassName() == "text")
		return boost::make_shared<textEntity>(facade);
	if(facade->getClassName() == "water")
		return boost::make_shared<water>(facade);
	if(facade->getClassName() == "knob")
		return boost::make_shared<knob>(facade);
	if(facade->getClassName() == "music")
		return boost::make_shared<musicEntity>(facade);

		
	/*
	std::map<std::string, createFun>::iterator iter = classMapping.find(className);
	if(iter != classMapping.end())
	{
		//entity_facade &fde = facade;
		ptr = iter->second(facade);
	}
	return ptr;*/
}

void entity_factory::resetEntity( boost::shared_ptr<entity> ent )
{

}

//void entity_factory::registerCreate()
//{
//	doRegister("role_operator", );
//	doRegister("commonEntity", );
//	doRegister("dist_fire", );
//}
//
//void entity_factory::doRegister(const std::string &className, createFun fun)
//{
//	classMapping[className] = fun;
//}