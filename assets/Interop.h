#pragma once

#include "KeyValueList.h"


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
class Interop
{
public:

	Interop() = delete;		// can't instantiate
	Interop& operator=(const Interop&) = delete;	// can't copy
	Interop(const Interop&) = delete;


	static std::string serialise(const KeyValuePtr& kvp, char sep = '|');

	static std::ostream& serialise(std::ostream& strm, const KeyValuePtr& kvp, char sep = '|');


	static KeyValuePtr unserialise(std::istream& strm, char sep = '|');

	static KeyValuePtr unserialise(std::string& buffer, char sep = '|');


	static KeyValueList decodeParameterList(char**& inputParams, int* numInputParams);

	static void encodeParameterList(const KeyValueList& kvps, char**& outputParams, int* numOutputParams);
};

