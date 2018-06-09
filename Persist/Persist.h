#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Persist.h        : store and retrieve NoSqlDb contents		     									//
// ver 1.0																								//
// Platform         : Dell Inspiron 13 - Windows 10, Visual Studio 2017                                 //-|_ 
// Language         : C++ using the facilities of the standard C++ Libraries                            //-|  <----------Requirement 1---------->
// Application      : Project 1 [Key/Value Databse] Object Oriented Design CSE-687 Spring'18            //
// Author           : Sonal Patil, Syracuse University                                                  //
//                    spatil06@syr.edu (408)-416-6291                                                   //  
// Source           : Dr. Jim Fawcett, EECS, SU                                                         //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package defines a single class, Persist, that provides methods
* save and restore to save db contents in an XML string, and restore
* or create a new db from the XML.
*
* The Persist class also provides methods saveToFile and restoreFromFile
* that use save and restore.
*
* Public Interface :
* ------------------
* template<typename Data> bool Persist<Data>::saveToFile(const std::string& fileSpec) - saves db contents to file, using save
* template<typename Data> void Persist<Data>::compress(const Xml& xml, NoSqlDb<Data>& db_) - compresses xml string 
* template<typename Data> bool Persist<Data>::restoreFromFile(const std::string& fileSpec, NoSqlDb<Data>& db) - restores db contents from file, using restore
* template<typename Data> bool Persist<Data>::restoreFromXmlFile(const std::string& fileSpec) - restores db contents from file, using restoreFile
* template<typename Data> typename Persist<Data>::Xml Persist<Data>::save() - save db contents to XML string
* template<typename Data> std::string Persist<Data>::getTextBodyFromElement(SPtr sPtr) - retrive db contents from xml string
* template<typename Data> std::string Persist<Data>::getTextBodyFromElement(SPtrs sPtrs) - retrive db contents from xml string
* template<typename Data> void Persist<Data>::restore(const Xml& xml, NoSqlDb<Data>& db) - for restoration of db from xml file
* template<typename Data> void Persist<Data>::restoreFile(const Xml& xml) - for restoration of db from xml file	
*
* Build Process:
* --------------
*   devenv NoSqldb.sln /debug rebuild
*
* Required Files:
* ---------------
*   XmlDocument.h, XmlDocument.cpp,
*   Display.h, Display.cpp
*
* Maintenance History:
*---------------------
* ver 1.0 : 06 Feb 2018
* - first release
*/

#include "../NoSqldb/NoSqldb.h"
#include "../CppProperties/CppProperties.h"
#include "../XmlDocument/XmlUtilities.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement.h"
#include <fstream>
#include <vector>
#include <string>

namespace NoSQLDB
{
	// Record class is not being used, but is here as a reminder for future refactoring.
	template<typename Data>
	class Record
	{
	private:
		Property<Key> key;
		Property<Element<Data>> element;
	};

	// Persist class holds a reference to db and uses that to save and restore the db data.
	template<typename Data>
	class Persist
	{
	public:
		using Xml = std::string;
		using SPtr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;
		using SPtrs = std::vector<SPtr>;

		Persist(NoSqlDb<Data>& db) : db_(db) {}
		Xml save();
		void restore(const Xml& xml, NoSqlDb<Data>& db);
		void restoreFile(const Xml& xml);
		bool saveToFile(const std::string& fileSpec);
		bool restoreFromFile(const std::string& fileSpec, NoSqlDb<Data>& db);
		bool restoreFromXmlFile(const std::string& fileSpec);
		void compress(const Xml& xml, NoSqlDb<Data>& db);
	private:
		NoSqlDb<Data>& db_;
		std::string getTextBodyFromElement(SPtr sPtr);
		std::string getTextBodyFromElement(SPtrs sPtrs);
	};

	//----< saves db contents to file, using save >--------------------
	template<typename Data>
	bool Persist<Data>::saveToFile(const std::string& fileSpec)
	{
		std::ofstream out(fileSpec);
		if (!out.good())
			return false;
		Persist<Data> persist(db_);
		Xml xml = persist.save();
		out << xml;
		out.close();
		return true;
	}
	
	//----< compresses xml string >------------------------------------
	template<typename Data>
	void Persist<Data>::compress(const Xml& xml, NoSqlDb<Data>& db_)
	{
		std::string compressed;
		for (char ch : xml)
		{
			if (ch == '\n' || ch == '\t' || ch == ' ')
				continue;
			compressed += ch;
		}
		return compressed;
	}

	//----< restores db contents from file, using restore >------------
	template<typename Data>
	bool Persist<Data>::restoreFromFile(const std::string& fileSpec, NoSqlDb<Data>& db)
	{
		std::ifstream in(fileSpec);
		if (!in.good())
		{
			std::cout << "\n  failed to open file";
			return false;
		}
		Xml xml;
		// build XML string from file

		while (in.good())
		{
			char ch = in.get();
			// This code removes whitespace from XML contents 
			if (!in.good())
				break;
			xml += ch;
		}
		in.close();
		restore(xml,db);
		return true;
	}

	//----< restores db contents from file, using restore >------------
	template<typename Data>
	bool Persist<Data>::restoreFromXmlFile(const std::string& fileSpec)
	{
		std::ifstream in(fileSpec);
		if (!in.good())
		{
			std::cout << "\n  failed to open file";
			return false;
		}
		Xml xml;
		// build XML string from file

		while (in.good())
		{
			char ch = in.get();
			// This code removes whitespace from XML contents 
			if (!in.good())
				break;
			xml += ch;
		}
		in.close();
		restoreFile(xml);
		return true;
	}
	
	//----< save db contents to XML string >---------------------------
	template<typename Data>
	typename Persist<Data>::Xml Persist<Data>::save()
	{
		using namespace XmlProcessing;
		using SPtr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;
		SPtr pDbRoot = makeTaggedElement("noSqlDb");
		XmlProcessing::XmlDocument doc;
		doc.docElement()->addChild(pDbRoot);		
		Keys keys = db_.keys();
		for (Key k : keys){
			SPtr pDbRecord = XmlProcessing::makeTaggedElement("dbRecord");

			SPtr pKey = XmlProcessing::makeTaggedElement("key");
			SPtr pText = XmlProcessing::makeTextElement(Utilities::Convert<Key>::toString(k));
			pKey->addChild(pText);
			pDbRecord->addChild(pKey);
			SPtr pValue = XmlProcessing::makeTaggedElement("value");
			Element<Data> elem = db_.value(k);

			SPtr pName = XmlProcessing::makeTaggedElement("name", static_cast<std::string>(elem.name));
			pValue->addChild(pName);
			SPtr pDesc = XmlProcessing::makeTaggedElement("description", static_cast<std::string>(elem.descr));
			pValue->addChild(pDesc);
			DateTime dt = elem.dateTime;
			SPtr pDateTime = XmlProcessing::makeTaggedElement("dateTime", dt.time());
			pValue->addChild(pDateTime);
			SPtr pChildren = XmlProcessing::makeTaggedElement("children");
			Keys childKeys = elem.children;
			for (Key child : childKeys){
				SPtr pChild = XmlProcessing::makeTaggedElement("childKey", static_cast<std::string>(child));
				pChildren->addChild(pChild);
			}
			pValue->addChild(pChildren);
			pDbRecord->addChild(pValue);

			SPtr pPayload = XmlProcessing::makeTaggedElement("payload");
			SPtr pCata = XmlProcessing::makeTaggedElement("category", static_cast<std::string>(elem.category));
			pPayload->addChild(pCata);
			SPtr pFilepath = XmlProcessing::makeTaggedElement("filepath", static_cast<std::string>(elem.path));
			pPayload->addChild(pFilepath);
			pDbRecord->addChild(pPayload);
			pDbRoot->addChild(pDbRecord);
		}
		return doc.toString();
	}
	
	//----< retrive db contents from XML string >----------------------
	/*
	* Private helper function - provides safe extraction of element text
	* from single element pointer.
	*/
	template<typename Data>
	std::string Persist<Data>::getTextBodyFromElement(SPtr sPtr)
	{
		if (sPtr->children().size() == 0)
			return "";
		return Utilities::trim(sPtr->children()[0]->value());
	}
	
	/*
	* Private helper function - provides safe extraction of element text
	* from the first element pointer in a vector of element pointers.
	*/
	template<typename Data>
	std::string Persist<Data>::getTextBodyFromElement(SPtrs sPtrs)
	{
		if (sPtrs.size() == 0)
			return "";
		SPtr sPtr = sPtrs[0];
		if (sPtr->children().size() == 0)
			return "";
		return Utilities::trim(sPtr->children()[0]->value());
	}
	
	/*
	* This is where the restoration takes place.
	*/
	template<typename Data>
	void Persist<Data>::restore(const Xml& xml, NoSqlDb<Data>& db)
	{
		using namespace XmlProcessing;
		using SPtr = std::shared_ptr<AbstractXmlElement>;
		using SPtrs = std::vector<SPtr>;
		NoSQLDB::NoSqlDb<Data>* pDb = nullptr;
		pDb = &db;
		Keys keys = pDb->keys();
		XmlDocument doc(xml, XmlDocument::str);
		SPtrs pDbRoot = doc.descendents("dbRecord").select();
		for (SPtr pRecord : pDbRoot){
			XmlDocument doc;
			doc.docElement()->addChild(pRecord);
			Key key = getTextBodyFromElement(doc.element("key").select());
			if (key == "San Jose"){
				Name name;
				Description desc;
				DateTime dt;
				Children children;
				Category category;
				Filepath fp;
				SPtrs pValues = doc.descendents("value").select();
				for (SPtr pValue : pValues)
				{
					name = getTextBodyFromElement(doc.element("name").select());
					desc = getTextBodyFromElement(doc.element("description").select());
					dt = getTextBodyFromElement(doc.element("dateTime").select());
					SPtrs pChildren = doc.descendents("childKey").select();
					for (SPtr pChild : pChildren)
						children.push_back(getTextBodyFromElement(pChild));
				}
				SPtrs pPayloads = doc.descendents("payload").select();
				for (SPtr pPayload : pPayloads) {
					category = getTextBodyFromElement(doc.element("category").select());
					fp = getTextBodyFromElement(doc.element("filepath").select());
				}
				Element<Data> elem(name, category);
				elem.descr = desc;
				elem.path = fp;
				elem.dateTime = dt;
				elem.children = children;
				bool saved = db.saveRecord(key, elem);  // save new record
				if (!saved)
					db.saveValue(key, elem);  // overwrite old record element
				Display<std::string> display(db);
				display.showAll();
			}
			
		}
	}

	/*
	* This is where the restoration takes place.
	*/
	template<typename Data>
	void Persist<Data>::restoreFile(const Xml& xml)
	{
		using namespace XmlProcessing;
		using SPtr = std::shared_ptr<AbstractXmlElement>;
		using SPtrs = std::vector<SPtr>;
		XmlDocument doc(xml, XmlDocument::str);
		SPtrs pDbRoot = doc.descendents("dbRecord").select();
		for (SPtr pRecord : pDbRoot) {
			XmlDocument doc;
			doc.docElement()->addChild(pRecord);
			Key key = getTextBodyFromElement(doc.element("key").select());
			Name name;
			Description desc;
			DateTime dt;
			Children children;
			Category category;
			Filepath fp;
			SPtrs pValues = doc.descendents("value").select();
			for (SPtr pValue : pValues)
			{
				name = getTextBodyFromElement(doc.element("name").select());
				desc = getTextBodyFromElement(doc.element("description").select());
				dt = getTextBodyFromElement(doc.element("dateTime").select());
				SPtrs pChildren = doc.descendents("childKey").select();
				for (SPtr pChild : pChildren)
					children.push_back(getTextBodyFromElement(pChild));
			}
			SPtrs pPayloads = doc.descendents("payload").select();
			for (SPtr pPayload : pPayloads) {
				category = getTextBodyFromElement(doc.element("category").select());
				fp = getTextBodyFromElement(doc.element("filepath").select());
			}
			Element<Data> elem(name, category);
			elem.descr = desc;
			elem.path = fp;
			elem.dateTime = dt;
			elem.children = children;
			bool saved = db_.saveRecord(key, elem);  // save new record
			if (!saved)
				db_.saveValue(key, elem);  // overwrite old record element		

		}
	}
}
