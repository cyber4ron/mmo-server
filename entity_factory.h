#ifndef __entity_factory_h_
#define __entity_factory_h_

#include "entity.h"
#include "entity_facade.h"

class entity;
class entity_facade;

class entity_factory
{
public: 
	boost::shared_ptr<entity> createEntity(entity_facade* facade);

	//��������Ϊ��ʼֵ
	void resetEntity(boost::shared_ptr<entity> ent);
private:
};
#endif