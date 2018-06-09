#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// XmlUtilities.h   : Utility class to convert template types to XML                                    //
// ver 1.1																								//
// Platform         : Dell Inspiron 13 - Windows 10, Visual Studio 2017                                 //-|_ 
// Language         : C++ using the facilities of the standard C++ Libraries                            //-|  <----------Requirement 1---------->
// Application      : Project 1 [Key/Value Databse] Object Oriented Design CSE-687 Spring'18            //
// Author           : Dr. Jim Fawcett, EECS, SU                                                         //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package defines a single class, XmlConvert<T> that supports convering
* instances of the T type to and from XML strings.
*
* Required Files:
* ---------------
*   XmlUtilities.h, XmlUtilities.cpp,
*   XmlDocument static library
*   Utilities.h, Utilities.cpp
*
* Build Process:
* --------------
*   devenv NoSqldb.sln /debug rebuild
*
* Maintenance History:
*---------------------
* ver 1.1 : 22 Feb 2017
* - added these comments
* ver 1.0 : 06 Feb 2017
* - first release
*/


#include "XmlDocument.h"
#include "XmlElement.h"
#include "../Utilities/Utilities.h"
#include <vector>

namespace XmlProcessing
{
	template<typename T>
	class XmlConvert
	{
	public:
		using SPtr = std::shared_ptr<AbstractXmlElement>;

		static SPtr toXmlElement(const T& t, const std::string& tag);
		static T fromXmlElement(SPtr sElem);
	};

	template<typename T>
	typename XmlConvert<T>::SPtr XmlConvert<T>::toXmlElement(const T& t, const std::string& tag)
	{
		using SPtr = XmlConvert<T>::SPtr;
		SPtr pElem = XmlProcessing::makeTaggedElement(tag);
		SPtr pText = XmlProcessing::makeTextElement(Utilities::Convert<T>::toString(t));
		pElem->addChild(pText);
		return pElem;
	}

	template<typename T>
	T XmlConvert<T>::fromXmlElement(SPtr pElem)
	{
		std::vector<SPtr> children = pElem->children();
		if (children.size() == 0)
			throw std::exception("text node does not exist");
		SPtr pText = children[0];
		std::string text = pText->value();
		T t = Utilities::Convert<T>::fromString(text);
		return t;
	}
}
