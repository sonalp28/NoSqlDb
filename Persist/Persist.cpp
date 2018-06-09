//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Persist.cpp      : store and retrieve NoSqlDb contents												//	
// ver 1.0																			                    //
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

#include "Persist.h"
#include "../Display/Display.h"
#include <string>

//<------------------------------------test stub[testing purpose]---------------------------->
#ifdef TEST_PERSIST

using namespace NoSQLDB;

int main()
{
	Utilities::Title("Testing NoSqlDb Persistance");
	Utilities::putLine();
	using Element = Element<std::string>;
	NoSqlDb<std::string> db;

	Element test1("test1", "test");
	Element test2("test2", "test");
	test2.saveChild("test1");

	db.saveRecord("test1", test1);
	db.saveRecord("test2", test2);

	Utilities::subTitle("displaying test db");
	Utilities::putLines(2);
	Display<std::string> display(db);
	display.showAll();

	Persist<std::string> persist(db);
	Persist<std::string>::Xml xml = persist.save();
	Utilities::subTitle("Persisting test db to XML");
	std::cout << "\n" << xml << "\n";
	Utilities::subTitle("Persisting test db to file DbXml.txt");
	if (persist.saveToFile("DbXml.txt"))
		std::cout << "\n  db saved";
	else
		std::cout << "\n  filesave failed";
	Utilities::putLine();
	Utilities::subTitle("creating new db from persisted XML");
	Utilities::putLines(2);
	NoSqlDb<std::string> newDb;
	Persist<std::string> persist2(newDb);
	Display<std::string> display2(newDb);
	display2.showAll();

	Utilities::subTitle("creating newer db from DbXml.txt");
	Utilities::putLines(2);
	NoSqlDb<std::string> newerDb;
	Persist<std::string> persist3(newerDb);
	Display<std::string> display3(newerDb);
	display3.showAll();
	std::cout << "\n\n";
}
#endif

