using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


public static class Interop
{
	#region List<string> serialisation

	public static string serialiseStringList(List<string> msgs)
	{
		string valStr = "";
		foreach (string str in msgs)
			valStr += string.Format("{0}|{1}|", str.Length, str);

		return valStr;
	}

	public static List<string> unserialiseStringList(string str)
	{
		List<string> msgs = new List<string>();

		// read the key string
		int pos = 0;
		while (pos < str.Length)
		{
			int pos2 = str.IndexOf('|', pos);
			if (pos2 == -1)
				throw new Exception("Error unserialising a list of strings.");
			int len = 0;
			if (!Int32.TryParse(str.Substring(pos, pos2 - pos), out len))
				throw new Exception("Error unserialising a list of strings. Expecting a type length value.");
			string s = str.Substring(pos2 + 1, len);
			pos = pos2 + len + 1;

			// separator
			if (pos >= str.Length || str[pos] != '|')
				throw new Exception("Error unserialising a list of strings. Expecting a separator character.");
			++pos;

			msgs.Add(s);
		}

		return msgs;
	}

	#endregion    
	
	#region KeyValuePair serialisation

	public static string serialise(KeyValuePair<string, object> kvp)
	{
		string valStr = string.Format(CultureInfo.InvariantCulture,"{0}", kvp.Value);
		return string.Format("{0}|{1}|{2}|{3}|{4}|{5}|", kvp.Key.Length, kvp.Key, kvp.Value.GetType().Name.Length, kvp.Value.GetType().Name, valStr.Length, valStr);
	}

	public static KeyValuePair<string, object> unserialise(string str)
	{
		// read the key string
		int pos = str.IndexOf('|', 0);
		if (pos == -1)
			throw new Exception("Error unserialising a key-value pair.");
		int len = 0;
		if (!Int32.TryParse(str.Substring(0, pos), out len))
			throw new Exception("Error unserialising a key-value pair. Expecting a key length value.");
		string key = str.Substring(pos + 1, len);
		pos += len + 1;

		// separator
		if (pos >= str.Length || str[pos] != '|')
			throw new Exception("Error unserialising a key-value pair. Expecting a separator character.");
		++pos;

		// read the value type string
		int pos2 = str.IndexOf('|', pos);
		if (pos2 == -1)
			throw new Exception("Error unserialising a key-value pair. Expecting more data.");
		if (!Int32.TryParse(str.Substring(pos, pos2 - pos), out len))
			throw new Exception("Error unserialising a key-value pair. Expecting a type length value.");

		string typeStr = str.Substring(pos2 + 1, len);
		pos = pos2 + len + 1;

		// separator
		if (pos >= str.Length || str[pos] != '|')
			throw new Exception("Error unserialising a key-value pair. Expecting a separator character.");
		++pos;

		// read the value string
		pos2 = str.IndexOf('|', pos);
		if (pos2 == -1)
			throw new Exception("Error unserialising a key-value pair. Expecting more data.");
		if (!Int32.TryParse(str.Substring(pos, pos2 - pos), out len))
			throw new Exception("Error unserialising a key-value pair. Expecting a type length value.");

		string valueStr = str.Substring(pos2 + 1, len);
		pos = pos2 + len + 1;

		if (pos < str.Length && str[pos] != '|')
			throw new Exception("Error unserialising a key-value pair. Orphaned characters detected!");

		// convert value from a string representation to the correct object type
		object value;
		if (typeStr.Equals("String", StringComparison.OrdinalIgnoreCase))
			value = valueStr;
		else if (typeStr.Equals("Int32", StringComparison.OrdinalIgnoreCase))
			value = Convert.ToInt32(valueStr);
		else if (typeStr.Equals("Boolean", StringComparison.OrdinalIgnoreCase))
			value = Convert.ToBoolean(valueStr);
		else
			throw new Exception("Unknown key-value type or not implemented!");

		return new KeyValuePair<string, object>(key, value);
	}

	#endregion

	#region KeyValuePair Interop Marshalling & Serialisation

	public static void encodeParameterList(IDictionary<string, object> kvps, out IntPtr paramsRoot, out int paramsSize)
	{
		string[] ips = new string[kvps.Count];
		int ndx = -1;
		foreach (KeyValuePair<string, object> kvp in kvps)
			ips[++ndx] = serialise(kvp);

		paramsRoot = GenericMarshaller.StringArrayToIntPtr<byte>(ips);
		paramsSize = kvps.Count;
	}

	public static IDictionary<string, object> decodeParameterList(IntPtr paramsRoot, int paramsSize)
	{
		string[] ips = GenericMarshaller.IntPtrToStringArray<byte>(paramsSize, paramsRoot);

		IDictionary<string, object> prms = new Dictionary<string, object>();

		foreach (string str in ips)
		{
			KeyValuePair<string, object> kvp = unserialise(str);
			prms.Add(kvp);
		}

		return prms;
	}

	#endregion
}


