#include "stdafx.h"
#include "Interop.h"
#include <iostream>
#include <cctype>
#include <memory>
#include <objbase.h>
#include <string.h>


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
std::string Interop::serialise(const KeyValuePtr& kvp, char sep)
{
	std::string str;

	// serialise the key
	str += std::to_string(kvp->key().length());
	str += sep;
	str += kvp->key();
	str += sep;

	// map the value type
	std::string type;
	if (*kvp->type() == typeid(std::string))
		type = "string";
	else if (*kvp->type() == typeid(int))
		type = "int32";
	else if (*kvp->type() == typeid(double))
		type = "double";
	else if (*kvp->type() == typeid(float))
		type = "single";
	else if (*kvp->type() == typeid(bool))
		type = "boolean";
	else
	{
		std::string msg = "Don't know how to serialise a KeyValuePair with value type '";
		msg += kvp->type()->name();
		msg += "'";
		throw std::exception(msg.c_str());
	}

	// serialise the value type
	str += std::to_string(type.length());
	str += sep;
	str += type;
	str += sep;

	// encode the value as a string
	std::string value;
	if (*kvp->type() == typeid(std::string))
		value = kvp->value<std::string>();
	else if (*kvp->type() == typeid(int))
		value = std::to_string(kvp->value<int>());
	else if (*kvp->type() == typeid(double))
		value = std::to_string(kvp->value<double>());
	else if (*kvp->type() == typeid(float))
		value = std::to_string(kvp->value<float>());
	else if (*kvp->type() == typeid(bool))
		value = (kvp->value<bool>() ? "true" : "false");
	else
	{
		std::string msg = "Don't know how to serialise a KeyValuePair with value type '";
		msg += kvp->type()->name();
		msg += "'";
		throw std::exception(msg.c_str());
	}

	// serialise the value 
	str += std::to_string(value.length());
	str += sep;
	str += value;
	str += sep;

	return str;
}


std::ostream& Interop::serialise(std::ostream& strm, const KeyValuePtr& kvp, char sep)
{
	std::string tmp = serialise(kvp, sep);
	strm << tmp;
	return strm;
}

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
KeyValuePtr Interop::unserialise(std::istream& strm, char sep)
{
	std::string key, type, value;
	int len = -1;

	strm >> std::noskipws;		// this manipulator stops the skipping of whitespace by the formatted operator >>(). This stream manipulator is 'sticky', i.e. it remains active till explicitly disabled.

	// read the key string
	char ch = 'X';
	if ((strm >> len >> ch) && (ch == sep))
	{
		key.resize(len);
		strm.read(&key[0], len);
	}

	if ((strm >> ch) && (ch != sep))
		throw std::exception("Error unserialising a key-value pair");

	// read the type string
	if ((strm >> len >> ch) && (ch == sep))
	{
		type.resize(len);
		strm.read(&type[0], len);
	}

	if ((strm >> ch) && (ch != sep))
		throw std::exception("Error unserialising a key-value pair");

	for (auto& elem : type)
		elem = std::tolower(elem);

	// read the value string
	if ((strm >> len >> ch) && (ch == sep))
	{
		value.resize(len);
		strm.read(&value[0], len);
	}

	if (strm.peek() == ch)
	if ((strm >> ch) && (ch != sep))
		throw std::exception("Error unserialising a key-value pair. Orphaned characters detected after the unserialise operation.");

	// convert to a key-value object instance
	if (type.compare("string") == 0)
	{
		return std::shared_ptr< KeyValuePairExt<std::string> >(new KeyValuePairExt<std::string>(key, value));
	}
	else if (type.compare("int32") == 0)
	{
		return std::shared_ptr< KeyValuePairExt<int> >(new KeyValuePairExt<int>(key, stoi(value)));
	}
	else if (type.compare("double") == 0)
	{
		return std::shared_ptr< KeyValuePairExt<double> >(new KeyValuePairExt<double>(key, stod(value)));
	}
	else if (type.compare("single") == 0)
	{
		return std::shared_ptr< KeyValuePairExt<float> >(new KeyValuePairExt<float>(key, stof(value)));
	}
	else if (type.compare("boolean") == 0)
	{
		try
		{
			return std::shared_ptr< KeyValuePairExt<bool> >(new KeyValuePairExt<bool>(key, stoi(value) != 0));
		}
		catch (std::invalid_argument& ex)
		{
			// contains the keywords 'true/t/false/f' instead of a number?
			char ch = std::tolower(value[0]);
			if (ch != 't' && ch != 'f')
			{
				std::string msg = ex.what();
				msg += " Can't interpret boolean value of KeyValuePair '" + key + "' from raw data '" + value + "'";
				throw std::invalid_argument(msg.c_str());
			}

			return std::shared_ptr< KeyValuePairExt<bool> >(new KeyValuePairExt<bool>(key, ch == 't'));
		}
	}
	else
	{
		std::string msg = "Unknown key-value type or not implemented! (Type = '" + type + "'";
		throw std::exception(msg.c_str());
	}
}


KeyValuePtr Interop::unserialise(std::string& buffer, char sep)
{
	std::stringstream strm;
	strm << buffer;
	strm.seekg(0);
	return unserialise(strm, sep);
}


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
KeyValueList Interop::decodeParameterList(char**& inputParams, int* numInputParams)
{
	KeyValueList kvps;

	// check the incoming array
	std::cout << "Strings received in native call:" << std::endl;
	for (int i = 0; i < *numInputParams; i++)
	{
		std::string prm = inputParams[i];
		CoTaskMemFree((inputParams)[i]);

		std::cout << "raw string = [" << prm << std::endl;

		auto kvp = unserialise(prm);
		kvps.push_back(kvp);
	}
	CoTaskMemFree(inputParams);
	inputParams = NULL;
	*numInputParams = 0;

	return kvps;
}


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
void Interop::encodeParameterList(const KeyValueList& kvps, char**& outputParams, int* numOutputParams)
{
	// CoTaskMemAlloc must be used instead of new operator since code on managed side will call Marshal.FreeCoTaskMem to free this memory
	char** ptr = (char**)CoTaskMemAlloc(sizeof(char*)* kvps.size());

	// populate the output array
	int ndx = -1;
	for (auto& kvp : kvps)
	{
		std::string str = serialise(kvp);
		size_t len = str.length();
		ptr[++ndx] = (char*)CoTaskMemAlloc(sizeof(char)* (len + 1));
		::ZeroMemory(ptr[ndx], sizeof(char)* (len + 1));
#ifdef _WIN32
		strncpy_s(ptr[ndx], len + 1, str.c_str(), len);
#else
		strncpy(ptr[ndx], str.c_str(), len);
#endif // _WIN32
	}
	outputParams = ptr;
	*numOutputParams = static_cast<int>(kvps.size());
}


