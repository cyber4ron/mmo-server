#pragma once

#define EPS 1E-6

struct vector3
{
	double x, y, z;

	vector3()
	{
		x = y = z = 0;
	}

	vector3(double x_, double y_, double z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	vector3(const std::string &str)
	{
		std::vector<std::string> spvec;
		boost::split(spvec, str, boost::is_any_of(", "), boost::token_compress_on);
		if(spvec.size() != 3) return;
		try
		{
			std::vector<std::string>::iterator iter = spvec.begin();
			x = boost::lexical_cast<double>(*iter++);
			y = boost::lexical_cast<double>(*iter++);
			z = boost::lexical_cast<double>(*iter);		
		}
		catch(const boost::bad_lexical_cast & e)
		{

			std::cout << "can not cast to double from string!";
		}
	}

	bool equal_dbl(double a, double b)
	{
		return (a - b > -EPS) && (a - b < EPS);
	}

	bool operator == (vector3 a)
	{
		return equal_dbl(x, a.x) && equal_dbl(y, a.y) && equal_dbl(z, a.z);
	}

	vector3 operator- (const vector3 &a)
	{
		return vector3(x-a.x,y-a.y,z-a.z);
	}

	vector3 operator+ (const vector3 &a)
	{
		return vector3(x+a.x,y+a.y,z+a.z);
	}

	vector3 operator* (const double &val)
	{
		return vector3(x*val,y*val,z*val);
	}

	vector3 operator* (const vector3 &vec)
	{
		return vector3(x*vec.x,y*vec.y,z*vec.z);
	}

	vector3 &operator+= (const vector3 &vec)
	{
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;
		return *this;
	}

	vector3 normalise()
	{
		double tmpv = x*x + y*y + z*z;
		tmpv = sqrt(tmpv);
		return vector3(x/tmpv,y/tmpv,z/tmpv);
	}

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & x;
		ar & y;
		ar & z;
	}
};