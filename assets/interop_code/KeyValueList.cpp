#include "stdafx.h"
#include "KeyValueList.h"
#include <iostream>
#include <string>


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
void KeyValueList::display() const
{
	std::cout << "--- KeyValueList (" << size() << " element(s)) ---" << std::endl;
	for (auto& kvp : *this)
	{
		try
		{
			std::cout << "   " << kvp->key() << " =  ";
			std::cout << "(" << kvp->type()->name() << ")  ";
			if (*kvp->type() == typeid(std::string))
				std::cout << "[" << kvp->value<std::string>() << "]";
			else if (*kvp->type() == typeid(int))
				std::cout << "[" << kvp->value<int>() << "]";
			else if (*kvp->type() == typeid(double))
				std::cout << "[" << kvp->value<double>() << "]";
			else if (*kvp->type() == typeid(float))
				std::cout << "[" << kvp->value<float>() << "]";
			else if (*kvp->type() == typeid(bool))
				std::cout << "[" << kvp->value<bool>() << "]";
			else
				std::cout << "???";
			std::cout << std::endl;
		}
		catch (...)
		{
			std::cout << "** Exception thrown in KeyValueList::display()!" << std::endl;
		}
	}
	std::cout << "--- ---" << std::endl;
}


const KeyValuePtr KeyValueList::find(std::string key) const
{
	for (auto& kvp : *this)
	{
		if (kvp->key().compare(key) == 0)		// case sensitive!
			return kvp;
	}

	std::string msg = "A KeyValuePair with key '" + key + "' is missing!";
	throw std::exception(msg.c_str());
}


const KeyValuePtr KeyValueList::try_find(std::string key) const throw()
{
	for (auto& kvp : *this)
	{
		if (kvp->key().compare(key) == 0)		// case sensitive!
			return kvp;
	}

	return KeyValuePtr();
}

