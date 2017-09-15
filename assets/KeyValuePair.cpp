#include "stdafx.h"
#include "KeyValuePair.h"



//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
KeyValuePair::KeyValuePair(std::string key)
{
	m_key = key;
}


KeyValuePair::~KeyValuePair()
{
}


std::string KeyValuePair::key() const
{
	return m_key;
}



