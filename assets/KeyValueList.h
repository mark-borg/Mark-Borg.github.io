#pragma once

#include "KeyValuePair.h"


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
class KeyValueList : public std::vector<KeyValuePtr>
{
public:

	void display() const;

	// finds the key-value pair with the given key. Case-sensitive.
	// throws an exception if the key is not in the list.
	const KeyValuePtr find(std::string key) const;

	// finds the key-value pair with the given key. Case-sensitive.
	// Returns an empty pointer if not found in the list.
	const KeyValuePtr try_find(std::string key) const throw();
};

