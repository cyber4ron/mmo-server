#include "stdafx.h"

#include "entity_facade.h"

entity_facade::entity_facade(std::map<std::string, std::string>::const_iterator beg, std::map<std::string, std::string>::const_iterator end)
{
	while(beg != end)
	{
		this->properties[beg->first] = beg->second;
		beg++;
	}
}

std::string entity_facade::getProperty(std::string propName)
{
	std::map<std::string,std::string>::iterator iter = properties.find(propName);
	if( iter != properties.end())
		return iter->second;
	return "";
}

std::string entity_facade::getClassName()
{
	return this->getProperty("classname");
}

std::string entity_facade::getType()
{
	return getProperty("type");
}
std::string entity_facade::getName()
{
	return getProperty("name");
}
std::string entity_facade::getId()
{
	return getProperty("id");
}
std::string entity_facade::getRoleId()
{
	return getProperty("roleId");
}

std::string entity_facade::getPosition()
{
	return getProperty("position");
}

std::string entity_facade::getRotation()
{
	return getProperty("rotation");
}

std::string entity_facade::getScale()
{
	return getProperty("scale");
}

std::string entity_facade::getUnitAngle()
{
	return getProperty("unitangle");
}

std::string entity_facade::getValveState()
{
	return getProperty("valvestate");
}
