#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// TestExec.h       : demonstrate NoSqlDb requirements													//
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
* This package defines class TestDBString, that demonstrates each of
* the requirements met.  
*
* Public Interface :
* ------------------
* void TestDBString::DemoReq1() - function to demonstrate requirment
* void TestDBString::DemoReq2() - function to demonstrate requirment
* void TestDBString::DemoReq3() - function to demonstrate requirment
* void TestDBString::DemoReq4() - function to demonstrate requirment
* void TestDBString::DemoReq5() - function to demonstrate requirment
* void TestDBString::DemoReq6() - function to demonstrate requirment
* void TestDBString::DemoReq7a() - function to demonstrate requirment
* void TestDBString::DemoReq7b() - function to demonstrate requirment
* void TestDBString::DemoReq8() - function to demonstrate requirment
* void TestDBString::DemoReq9() - function to demonstrate requirment
* void TestDBString::DemoReq10() - function to demonstrate requirment
* void TestDBString::DemoReq11() - function to demonstrate requirment
* void TestDBString::DemoReq12() - function to demonstrate requirment
* void TestDBString::DemoReq13() - function to demonstrate requirment
*
* Required Files:
* ---------------
*   TestExec.h, TestExec.cpp,
*   Queries.h, Queries.cpp,
*   Persist.h, Persist.cpp,
*   Display.h, Display.cpp,
*   DateTime.h, DateTime.cpp,
*   Test.h, Test.cpp,
*   Utilities.h, Utilities.cpp,
*
* Build Process:
* --------------
*   devenv NoSqldb.sln /debug rebuild
*
* Maintenance History:
*---------------------
* ver 1.0 : 06 Feb 2018
* - first release
*/

#include "TestQueries.h"
#include "../NoSqldb/NoSqldb.h"
#include "../Queries/Queries.h"
#include "../Persist/Persist.h"
#include "../Display/Display.h"
#include "../Utilities/Utilities.h"
#include <regex>
#include <string>
#include <conio.h>
#include <vector>
#include <fstream>

class TestDBString
{
public:
	NoSQLDB::NoSqlDb<std::string>& makeTestStringDb();
	NoSQLDB::NoSqlDb<std::string> makeProject1Db();
	void DemoReq1();
	void DemoReq2();
	void DemoReq3();
	void DemoReq4();
	void DemoReq5();
	void DemoReq6();
	void DemoReq7a();
	void DemoReq7b();
	void DemoReq8();
	void DemoReq9();
	void DemoReq10();
	void DemoReq11();
	void DemoReq12();
	void DemoReq13();
private:
	NoSQLDB::NoSqlDb<std::string> db_;
};

//<-----------------------------------------------------Function to demonstrate requriment 1--------------------------------------------------------->
void TestDBString::DemoReq1()
{
	Utilities::subTitle("\n Demonstrating Req #1 - Implemented in C++ using facilities of C++ STL and Visual Studio 2017");
	std::cout << "\n " << typeid(std::function<bool()>).name() << ", declared in this function, " << "is only valid for C++11 and later versions.\n";
}

//<-----------------------------------------------------Function to demonstrate requriment 2--------------------------------------------------------->
void TestDBString::DemoReq2()
{
	Utilities::subTitle("\n Demonstrating Req #2 - Use of C++ STL for all console I/O, new and delete[all heap-based memory management]");
	std::cout << "\n A visual examination of all the submitted code will show only use of streams and operators new and delete.\n";
}

//<-----------------------------------------------------Function to demonstrate requriment 3--------------------------------------------------------->
void TestDBString::DemoReq3()
{
	Utilities::subTitle("\n Demonstrating Req #3 - A generic key/value in-memory database");
	Utilities::putLine();
	db_ = makeTestStringDb();
	NoSQLDB::Display<std::string> display(db_);
	display.showAll();
	Utilities::putLine();
}

//<-----------------------------------------------------Function to demonstrate requriment 4--------------------------------------------------------->
void TestDBString::DemoReq4()
{
	Utilities::subTitle("\n Demonstrating Req #4 -  Addition and Deletion of key/value pairs");
	std::cout << "\n  Inserting new elements: \"newElem1\", \"newElem2\" and \"newElem3\"\n\n";
	NoSQLDB::Element<std::string> newElem("newElem1", "test");
	newElem.descr = "new_elem_1";
	newElem.path = "../file.cpp";
	db_.saveRecord("newElem1", newElem);
	NoSQLDB::Element<std::string> newerElem("newElem2", "test");
	newerElem.saveChild("newElem1");
	newerElem.saveChild("newElem3");
	newerElem.descr = "new_elem_2";
	newerElem.path = "../file.cpp";
	db_.saveRecord("newElem2", newerElem);
	NoSQLDB::Element<std::string> newestElem("newElem3", "test");
	newestElem.descr = "new_elem_3";
	newestElem.path = "../file.cpp";
	db_.saveRecord("newElem3", newestElem);
	NoSQLDB::Display<std::string> display(db_);
	display.showAll();

	std::cout << "\n  Removing element \"San Jose\":\n\n";
	db_.removeRecord("San Jose");
	display.showAll();
	std::cout << "\n  Note: When \"San Jose\" element was removed, \"California\"'s child (San Jose) was also removed.\n";
	
	Utilities::subTitle("\n Demonstrating Req #5b - If deleted child relationships, forbidden to delete elements accessed from those keys");
	std::cout << "\n  Removing element \"newElem2\":\n\n";
	db_.removeRecord("newElem2");
	display.showAll();
	std::cout << "\n  Note: When \"newElem2\" was removed, it's children \"newElem1\" and \"newElem3\" are not removed.\n";
}

//<-----------------------------------------------------Function to demonstrate requriment 5--------------------------------------------------------->
void TestDBString::DemoReq5()
{
	Utilities::subTitle("\n Demonstrating Req #5a - Editing records");
	std::cout << "\n  Editing element with key \"newElem3\" : Editing Metadata - Name, Description & Payload - Category, Filepath\n\n";
	NoSQLDB::Element<std::string> elem = db_.value("newElem3");
	elem.name = "Las Vegas";
	elem.category = "City";
	elem.descr = "CA City";
	std::vector<std::string>& children = elem.children.getValue();
	if (children.size() > 0)
		children.erase(children.begin());
	db_.saveValue("newElem3", elem);
	NoSQLDB::Display<std::string> display(db_);
	display.showAll();
}

//<-----------------------------------------------------Function to demonstrate requriment 6--------------------------------------------------------->
void TestDBString::DemoReq6()
{
	Utilities::subTitle("\n Demonstrating Req #6 - Implementing Queries");
	using namespace NoSQLDB;
	Query<std::string> query(db_);
	Display<std::string> display(db_);
	TestExQueries<std::string> tq(std::cout);
	tq.doQueries(db_);
}

//<-----------------------------------------------------Function to demonstrate requriment 7a-------------------------------------------------------->
void TestDBString::DemoReq7a()
{
	Utilities::subTitle("\n Demonstrating Req #7a -  Queries on the set of keys returned by a previous query [\"AND\"ing Queries]");

	NoSQLDB::Query<std::string> query(db_);
	NoSQLDB::Display<std::string> display(db_);
	NoSQLDB::NoSqlDb<std::string>& db = db_;

	NoSQLDB::Query<std::string>::QueryType getElem
		= [&db](const NoSQLDB::Query<std::string>::Arg& arg, NoSQLDB::Keys keys)
	{
		NoSQLDB::Keys returnKeys;
		std::cout << "\n  2nd query : Get Element for key = \"" << arg << "\"";
		for (NoSQLDB::Key k : keys)
		{
			if ((db.value(k)).name.getValue() == arg)
				returnKeys.push_back(arg);
		}
		return returnKeys;
	};

	NoSQLDB::Query<std::string>::QueryType hasChildren = [&db](const std::string& arg, NoSQLDB::Keys keys)
	{
		NoSQLDB::Keys returnKeys;
		std::cout << "\n  1st query : Get set of all keys who has children";
		for (NoSQLDB::Key k : keys)
		{
			if ((db.value(k)).children.getValue().size() > 0)
				returnKeys.push_back(k);
		}
		return returnKeys;
	};

	std::cout << "\n  \"AND\"ing Query - Elements with Children && Element with key \"California\"\n";
	NoSQLDB::Keys qkeys = query.execute(hasChildren, "", db.keys());
	std::cout << "\n  Size of 1st query result keyset = " << qkeys.size();
	std::cout << "    1st query Result keys : ";
	for (NoSQLDB::Key k : qkeys)
		std::cout << k << ", ";
	std::cout << "\n  1st query operated database : \n";
	display.showQuery(qkeys);

	qkeys = query.execute(getElem, "California", qkeys);
	std::cout << "\n  Size of 2nd query result keyset = " << qkeys.size();
	std::cout << "    2nd query Result keys : ";
	for (NoSQLDB::Key k : qkeys)
		std::cout << k << ", ";
	std::cout << "\n  Final \"AND\"ing query result database : \n";
	display.showQuery(qkeys);

}

//<-----------------------------------------------------Function to demonstrate requriment 7b-------------------------------------------------------->
void TestDBString::DemoReq7b()
{
	Utilities::subTitle("\n Demonstrating Req #7b - Queries on the union of results of one or more previous queries [\"OR\"ing Queries]");
	Utilities::putLine();
	NoSQLDB::Query<std::string> query(db_);
	NoSQLDB::Display<std::string> display(db_);
	NoSQLDB::NoSqlDb<std::string>& db = db_;
	NoSQLDB::Query<std::string>::QueryType hasChildren = [&db](const std::string& arg, NoSQLDB::Keys keys){
		NoSQLDB::Keys returnKeys;
		std::cout << "\n  1st query : Get set of all keys who has children";
		for (NoSQLDB::Key k : keys){
			if ((db.value(k)).children.getValue().size() > 0)
				returnKeys.push_back(k);
		}
		return returnKeys;
	};
	NoSQLDB::Query<std::string>::QueryType getElem = [&db](const NoSQLDB::Query<std::string>::Arg& arg, NoSQLDB::Keys keys){
		NoSQLDB::Keys returnKeys;
		std::regex re(arg);
		std::cout << "\n  2nd query : Get Element for name = \"" << arg << "\"";
		for (NoSQLDB::Key k : keys){
			std::string name = db.value(k).name;
			if (std::regex_match(name, re))
				returnKeys.push_back(k);
		}
		return returnKeys;
	};
	std::cout << "\n  \"OR\"ing Query - Elements with Children || Element with name \"Las*\"\n";

	NoSQLDB::Keys qkeys = query.execute(hasChildren, "", db.keys());
	std::cout << "\n  Size of 1st query result keyset = " << qkeys.size();
	std::cout << "    1st query Result keys : ";
	for (NoSQLDB::Key k : qkeys)
		std::cout << k << ", ";
	std::cout << "\n  1st query operated database : \n";
	display.showQuery(qkeys);
	NoSQLDB::Keys qkeys2 = query.execute(getElem, "Las.*", db.keys());
	std::cout << "\n  Size of 2nd query result keyset = " << qkeys.size();
	std::cout << "    2nd query Result keys : ";
	for (NoSQLDB::Key k : qkeys2) {
		std::cout << k << ", ";
		qkeys.push_back(k);
	}
	std::sort(qkeys.begin(), qkeys.end());
	qkeys.erase(std::unique(qkeys.begin(), qkeys.end()), qkeys.end());
	Utilities::putLine();
	std::cout << "\n  Final \"OR\"ing query result database : \n";
	display.showQuery(qkeys);
}

//<-----------------------------------------------------Function to demonstrate requriment 8--------------------------------------------------------->
void TestDBString::DemoReq8()
{
	using namespace NoSQLDB;
	Utilities::subTitle("\nDemonstrating Req #8a - persisting original db[db before implementing requirement 4,5] to XML file");
	Utilities::subTitle("Persisting NoSqlDb<Travel> to XML string:");
	std::cout << "\n  Stored the XML file of Original Database at ../../TestExec/Travel.xml";
	NoSqlDb<std::string> testDb = makeTestStringDb();
	NoSQLDB::Persist<std::string> persist(testDb);
	/*std::cout << "\n" << persist.save();*/
	std::cout << "\n\n";
	persist.saveToFile("Travel.xml");

	Utilities::subTitle("Demonstrating Req #8b - Augmented database from an existing XML file [original + modified and no overwriting]");
	Utilities::putLine();
	Utilities::subTitle("Current Database");
	Utilities::putLine();
	NoSQLDB::Display<std::string> display(db_);
	display.showAll();
	Utilities::putLine();
	std::cout << "  Element = \"San Jose\" was deleted from original database for requirement 5 and \n" << 
		"  It's now restored from the original database Travel.xml file and added into the current databse. \n" << 
		"  [Note : No other elements or records changed.[Duplicate keys didn't get overwrite] \n \n";
	Utilities::subTitle("Augmented Database");
	Utilities::putLine();
	persist.restoreFromFile("Travel.xml",db_);	
	std::cout << "\n";
}

//<-----------------------------------------------------Function to demonstrate requriment 9--------------------------------------------------------->
void TestDBString::DemoReq9() 
{
	Utilities::subTitle("Demonstrating Req #9 - Shall implement a Payload type");
	std::cout << "\n  Implemented a payload with category and file path that specifies the path for the file specified in the name\n";
	using namespace NoSQLDB;
	Query<std::string> query(db_);
	Display<std::string> display(db_);
	TestExQueries<std::string> tq(std::cout);
	tq.doPayloadQueries(db_, "State", "../file.cpp");
	std::cout << "\n\n";
}

//<-----------------------------------------------------Function to demonstrate requriment 10-------------------------------------------------------->
void TestDBString::DemoReq10()
{
	Utilities::subTitle("Demonstrating Req #10 - Implementation of at least following packages: Executive, DBCore, Query, Test.");
	std::cout << "\n  Please see the package structure diagram/ package structure XML file for this requirment.\n\n";
}

//<-----------------------------------------------------Function to demonstrate requriment 11-------------------------------------------------------->
void TestDBString::DemoReq11()
{
	Utilities::subTitle("Demonstrating Req #11 - Reading Project1 Package Structure");
	std::cout << "\n  Stored the XML file of Project1 package structure at ../../TestExec/Project1.xml\n\n";
	using namespace NoSQLDB;
	NoSqlDb<std::string> testDb = makeProject1Db();
	NoSQLDB::Persist<std::string> persist(testDb);
	persist.saveToFile("Project1.xml");

	Utilities::subTitle("Creating Project1 Package DB from XML file:");
	Utilities::putLine();
	persist.restoreFromXmlFile("Project1.xml");
	Display<std::string> display(testDb);
	display.showAllproj1();
	std::cout << "\n";
}

//<-----------------------------------------------------Function to demonstrate requriment 12-------------------------------------------------------->
void TestDBString::DemoReq12()
{
	Utilities::subTitle("Demonstrating Req #12 - test executive that clearly demonstrates you've met all functional requirements #2-#9.");
	std::cout << "\n  A visual examination of all the submitted code will show full functional demonstatrion of requirements #2-#9.\n\n"; 
}

//<-----------------------------------------------------Function to demonstrate requriment 13-------------------------------------------------------->
void TestDBString::DemoReq13()
{
	Utilities::subTitle("Demonstrating optional Req #13 - pdf file containing a package diagram of your project");
	std::cout << "\n  Please see the file \'Project1_PackageStr.pdf\' in project zip folder for this requirement.\n\n";
}