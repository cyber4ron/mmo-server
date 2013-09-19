#pragma once

template<typename Type>
bool check_vector(const std::vector<Type> &vec1, const std::vector<Type> &vec2)
{
	if(vec1.size() != vec2.size()) return false;

	for(int i=0;i<vec1.size();i++)
	{
		if(vec1[i] != vec2[i])return false;
	}

	return true;
}