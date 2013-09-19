#ifndef __entity_facade_h_
#define __entity_facade_h_

class entity_facade
{
public:
	entity_facade(){};
	entity_facade(std::map<std::string, std::string>::const_iterator beg, std::map<std::string, std::string>::const_iterator end);
	std::string getProperty(std::string propName);
	std::string getClassName();
	std::string getType();
	std::string getName();
	std::string getId();
	std::string getRoleId();
	std::string getPosition();
	std::string getScale();
	std::string getRotation();
	std::string getUnitAngle();
	std::string getValveState();

	void setProperties(const std::map<std::string, std::string> &props)
	{
		this->properties = props;
	}
private:
	std::map<std::string, std::string> properties;
};

#endif