#ifndef __RAPIDXML_HELPER__
#define __RAPIDXML_HELPER__

#include "include/rapidxml-1.13/rapidxml.hpp"

typedef rapidxml::file<> RapidFile;
typedef rapidxml::xml_document<> RapidDoc;
typedef rapidxml::xml_node<>* RapidNodePtr;
typedef rapidxml::xml_attribute<>* RapidAttrPtr;

class CRapidxmlHelper
{
public:
	inline static unsigned int GetUIntAttr(const rapidxml::xml_node<>* node, char* name, int base = 10)
	{
		rapidxml::xml_attribute<>* attr = node->first_attribute(name);
		if (attr != NULL)
		{
			if (attr->value_size() > 0)
			{
				return static_cast<unsigned int>(strtoul(attr->value(), NULL, base));
			}
		}

		return 0;
	}
	inline static string GetStringAttr(const rapidxml::xml_node<>* node, char* name)
	{
		rapidxml::xml_attribute<>* attr = node->first_attribute(name);
		if (attr != NULL)
		{
			if (attr->value_size() > 0)
			{
				return string(attr->value());
			}
		}

		return string("");
	}

	inline static unsigned int GetUInt(const rapidxml::xml_node<>* nd, const char* name)
	{
		rapidxml::xml_node<>* tmp_nd = nd->first_node(name);
		if (tmp_nd != NULL)
		{
			if (tmp_nd->value_size() > 0)
			{
				return static_cast<unsigned int>(strtoul(tmp_nd->value(), NULL, 10));
			}
		}

		return 0;
	}


	inline static string GetString(const rapidxml::xml_node<>* nd, const char* name)
	{
		rapidxml::xml_node<>* tmp_nd = nd->first_node(name);
		if (tmp_nd != NULL)
		{
			return string(tmp_nd->value());
		}

		return string("");
	}

	inline static unsigned int GetUInt(const rapidxml::xml_node<>* nd)
	{
		if (nd != NULL)
		{
			if (nd->value_size() > 0)
			{
				return static_cast<unsigned int>(strtoul(nd->value(), NULL, 10));
			}
		}

		return 0;
	}


	inline static string GetString(const rapidxml::xml_node<>* nd)
	{
		if (nd != NULL)
		{
			return string(nd->value());
		}

		return string("");
	}

	inline static void AppendUintAtr(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* name, unsigned int value)
	{
		if (node != NULL && name != NULL)
		{
			char tmp[32];
			snprintf(tmp, sizeof(tmp), "%u", value);
			char* pvalue = doc.allocate_string(tmp);
			rapidxml::xml_attribute<>* attr = doc.allocate_attribute(name, pvalue);
			node->append_attribute(attr);
		}
	}

	inline static void AppendStringAtr(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* name, const char* value)
	{
		if (node != NULL && value != NULL && name != NULL)
		{
			char* pvalue = doc.allocate_string(value);
			rapidxml::xml_attribute<>* attr = doc.allocate_attribute(name, pvalue);
			node->append_attribute(attr);
		}
	}


};

#endif

