#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// PersistHelp.h    : demo one way to persist objects to XML file										//
// Platform         : Dell Inspiron 13 - Windows 10, Visual Studio 2017                                 //-|_ 
// Language         : C++ using the facilities of the standard C++ Libraries                            //-|  <----------Requirement 1---------->
// Application      : Project 1 [Key/Value Databse] Object Oriented Design CSE-687 Spring'18            //
// Author           : Dr. Jim Fawcett, EECS, SU                                                         //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* This package builds wrapper classes for strings, doubles, and Widgets.
* They provide the same functionality as the wrapped type, but also
* attach methods save() and restore() to persist the wrapped type.
*
* Package operations:
* -------------------
* This package contains:
* - IPersist interface - defines the methods needed to
*   serialize to XML and deserialize from XML
* - StringWrapper class that persists strings
* - DoubleWrapper class that persists doubles
* - Widget class with int and string properties
* - WidgetWrapper class that persits Widgets
*
* 
* Required Files:
* ---------------
*   PersistHelp.h, PersistHelp.cpp,
*   Display.h, Display.cpp,
*   XmlDocument.h, XmlDocument.cpp
*  ---- required to build XmlDocument from file or string ----
*   XmlParser.h, XmlParser.cpp,
*   XmlElementParts.h, XmlElementParts.cpp,
*   ITokenizer.h, Tokenizer.h, Tokenizer.cpp,
*
* Build Instructions:
* -------------------
* - Uses XmlDocument, so build XmlDocument project as static library
*   after undefining its test stub
*
* Maintenance History:
*---------------------
* ver 1.0 : 06 Feb 2017
* - first release
*/
#include <string>
#include <vector>
#include "../Utilities/Utilities.h"
#include "../CppProperties/CppProperties.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement.h"
#include "../NoSqldb/NoSqldb.h"

using namespace XmlProcessing;
using namespace Utilities;

namespace NoSQLDB
{
	/////////////////////////////////////////////////////////////////////
	// IPersist interface
	// - declares methods Persistance Wrappers must use
	//
	template<typename T>
	struct IPersist
	{
		using Xml = std::string;

		virtual ~IPersist() {}
		virtual IPersist<T>& operator=(const T& t) = 0;
		virtual operator T() = 0;
		virtual Xml save() = 0;
		virtual void restore(const Xml& xml) = 0;
		std::string id() { return typeid(*this).name(); }
	};

	/////////////////////////////////////////////////////////////////////
	// StringWrapper class
	// - wrapper classes implement the persistance interface
	// - they almost certainly should be non-template classes
	//   because they have to be type specific
	//
	class StringWrapper : public IPersist<std::string>
	{
	public:
		StringWrapper(const std::string& str = "") : str_(str) {}
		StringWrapper& operator=(const std::string& s);  // covariant return type
		Xml save();
		void restore(const Xml& xml);
		operator std::string();
	private:
		std::string str_;
	};
	//----< assignment overload >----------------------------------------

	inline StringWrapper& StringWrapper::operator=(const std::string& s)
	{
		str_ = s;
		return *this;
	}
	//----< saves instance state as XML string >-------------------------

	inline StringWrapper::Xml StringWrapper::save()
	{
		return "<StringWrapper>" + str_ + "</StringWrapper>";
	}
	//---< restores instance state from XML string >---------------------

	inline void StringWrapper::restore(const Xml& xml)
	{
		size_t posStart = xml.find_first_of(">") + 1;
		size_t posEnd = xml.find_last_of("</");
		str_ = xml.substr(posStart, posEnd - posStart - 1);
	}

	//----< cast operator returns string member >------------------------

	inline StringWrapper::operator std::string()
	{
		return str_;
	}

	/////////////////////////////////////////////////////////////////////
	// DoubleWrapper class
	//
	class DoubleWrapper : public IPersist<double>
	{
	public:
		DoubleWrapper(double d = 0) : d_(d) {}
		DoubleWrapper& operator=(const double& d);
		Xml save();
		void restore(const Xml& xml);
		operator double();
	private:
		double d_;
	};
	//----< assignment overload >----------------------------------------

	inline DoubleWrapper& DoubleWrapper::operator=(const double& d)
	{
		d_ = d;
		return *this;
	}
	//----< saves instance state as XML string >-------------------------

	inline DoubleWrapper::Xml DoubleWrapper::save()
	{
		return "<DoubleWrapper>" + Utilities::Convert<double>::toString(d_) + "</DoubleWrapper>";
	}
	//---< restores instance state from XML string >---------------------

	inline void DoubleWrapper::restore(const Xml& xml)
	{
		size_t posStart = xml.find_first_of(">") + 1;
		size_t posEnd = xml.find_last_of("</");
		std::string bodyString = xml.substr(posStart, posEnd - posStart - 1);
		d_ = Utilities::Convert<double>::fromString(bodyString);
	}
	//----< cast operator returns double member >------------------------

	inline DoubleWrapper::operator double()
	{
		return d_;
	}
	/////////////////////////////////////////////////////////////////////
	// ElementWrapper class
	//
	template<typename Data>
	class ElementWrapper : public IPersist<Element<Data>>
	{
	public:
		ElementWrapper(Element<Data>& elem) : elem_(elem) {}
		ElementWrapper& operator=(const Element<Data>& elem);
		Xml save();
		void restore(const Xml& xml);
		operator Element<Data>() { return elem_; }
	private:
		Element<Data> elem_;

	};
	//----< assignment overload >----------------------------------------

	template<typename Data>
	ElementWrapper<Data>& ElementWrapper<Data>::operator=(const Element<Data>& elem)
	{
		elem_ = elem;
		return *this;
	}
	//----< saves instance state as XML string >-------------------------

	template<typename Data>
	typename ElementWrapper<Data>::Xml ElementWrapper<Data>::save()
	{
		XmlDocument doc;

		// make and add root element to doc
		SPtr root = makeTaggedElement("Element");
		doc.docElement() = root;

		// make Property<std::string> name element and add to root
		std::string nameStr = Element<Data>(*this).name;  // force cast
		SPtr pNameElem = makeTaggedElement("name");
		pNameElem->addChild(makeTextElement(nameStr));
		root->addChild(pNameElem);

		// make Property<std::string> category element and add to root
		std::string categoryStr = Element<Data>(*this).category;
		SPtr pCategoryElem = makeTaggedElement("category");
		pCategoryElem->addChild(makeTextElement(categoryStr));
		root->addChild(pCategoryElem);

		// make Property<std::string> dateTime element and add to root
		DateTime dt = (Element<Data>(*this).dateTime);
		std::string dateTimeStr = dt.time();
		SPtr pDateTimeElem = makeTaggedElement("dateTime");
		pDateTimeElem->addChild(makeTextElement(dateTimeStr));
		root->addChild(pDateTimeElem);

		// make Property<Children> children element and add to root

		NoSQLDB::Children children = Element<Data>(*this).children;
		SPtr pChildrenElem = makeTaggedElement("children");
		for (Key k : children)
		{
			SPtr pChild = makeTaggedElement("childKey");
			pChild->addChild(makeTextElement(k));
			pChildrenElem->addChild(pChild);
		}
		root->addChild(pChildrenElem);

		// make Property<Data> data element and add to root

		Data data = Element<Data>(*this).data;
		SPtr pDataElem = makeTaggedElement("data");
		pDataElem->addChild(makeTextElement(Convert<Data>::toString(data)));
		root->addChild(pDataElem);

		return doc.toString();
	}
	//---< restores instance state from XML string >---------------------

	using SPtr = std::shared_ptr<AbstractXmlElement>;
	using SPtrs = std::vector<SPtr>;

	inline std::string getNthChildText(size_t n, SPtrs sptrs)
	{
		if (sptrs.size() < n + 1)
			return "";
		if (sptrs[n]->children().size() == 0)
			return "";
		return trim(sptrs[n]->children()[0]->value());
	}
	inline std::string getFirstChildText(SPtrs sptrs)
	{
		return getNthChildText(0, sptrs);
	}

	template<typename Data>
	void ElementWrapper<Data>::restore(const Xml& xml)
	{
		XmlDocument doc(xml, XmlDocument::str);
		elem_.name = getFirstChildText(doc.descendents("name").select());
		elem_.category = getFirstChildText(doc.descendents("category").select());
		std::string dateTimeStr = getFirstChildText(doc.descendents("dateTime").select());
		elem_.dateTime = DateTime(dateTimeStr);
		SPtrs sPtrs = doc.descendents("childKey").select();
		for (size_t i = 0; i < sPtrs.size(); ++i)
		{
			Children& children = elem_.children.getValue();
			children.push_back(getNthChildText(i, sPtrs));
		}
		std::string dataText = getFirstChildText(doc.descendents("data").select());
		elem_.data = Utilities::Convert<Data>::fromString(dataText);
	}

	/////////////////////////////////////////////////////////////////////
	// NoSqlDb Record
	//
	template<typename Data>
	class NoSQLDBRecord
	{
	private:
		Property<Key> key;
		Property<Element<Data>> elem;
	};

	/////////////////////////////////////////////////////////////////////
	// NoSqlDb RecordWrapper
	//
	template<typename Data>
	class NoSQLDBRecordWrapper : IPersist<NoSQLDBRecord<Data>>
	{
	public:
		virtual NoSQLDBRecordWrapper<Data>& operator=(const NoSQLDBRecord<Data>& t) = 0;
		virtual operator NoSQLDBRecord<Data>() = 0;
		virtual Xml save() = 0;
		virtual void restore(const Xml& xml) = 0;
	private:
		NoSQLDBRecord<Data> record_;
	};
	/////////////////////////////////////////////////////////////////////
	// Widget and WidgetWrapper classes
	//
	class Widget
	{
	private:
		Property<int> wint;
		Property<std::string> wstr;
	};

	class WidgetWrapper : public IPersist<Widget>
	{
	public:
		WidgetWrapper() {};
		WidgetWrapper(Widget widget) : widget_(widget) {}
		WidgetWrapper& operator=(const Widget& widget);
		Xml save();
		void restore(const Xml& xml);
		operator Widget();
	private:
		Widget widget_;
	};
	//----< assignment operator overload >-------------------------------

	inline WidgetWrapper& WidgetWrapper::operator=(const Widget& widget)
	{
		widget_ = widget;
		return *this;
	}
}
