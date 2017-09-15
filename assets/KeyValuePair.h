#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <memory>


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
class KeyValuePair
{
public:

	KeyValuePair() = delete;					// prevent default constructor

	KeyValuePair(std::string key);

	virtual ~KeyValuePair();

	std::string key() const;

	virtual const type_info* type() const = 0;

	template<typename T>
	const T value() const
	{
		if (typeid(T) != *type())
		{
			std::string msg = "Bad Cast exception raised by KeyValuePair '";
			try
			{
				std::stringstream strm;
				strm << "Bad Cast exception raised by KeyValuePair '" << key() << "'. Can't cast type " << type()->name() << " to " << typeid(T).name();
				msg = strm.str();
			}
			catch (...)
			{
			}

			throw std::bad_cast(msg.c_str());
		}

		const void* ptr = get_value_ptr();
		return *(static_cast<T*>(const_cast<void*>(ptr)));
	}

	virtual const void* get_value_ptr() const = 0;


private:

	std::string m_key;
};


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
template<typename T>
class KeyValuePairExt : public KeyValuePair
{
public:

	KeyValuePairExt(std::string key, const T& value)
		: KeyValuePair(key)
	{
		m_value = value;
	}

	virtual const type_info* type() const
	{
		return &typeid(T);
	}

	virtual const void* get_value_ptr() const
	{
		return static_cast<const void*>(&m_value);
	}

private:

	T m_value;
};



//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
typedef std::shared_ptr<KeyValuePair>	KeyValuePtr;

