//////////////////////////////////////////////////////////////////////////////////////////////////////////
// TestExec.h       : Defines the entry point for the console application and							//
//                    demonstrate NoSqlDb requirements													//
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
* NoSQLDB::NoSqlDb<std::string>& TestDBString::makeTestStringDb() - function that creates new elements and add them to the database
* NoSQLDB::NoSqlDb<std::string> TestDBString::makeProject1Db() - function that creates new elements for project package structure and add them to the database
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

#include "TestExec.h"
#include "../DateTime//DateTime.h"
#include "../Utilities/Utilities.h"
#include "../NoSqldb/NoSqldb.h"
#include <string>
#include <iostream>

using namespace NoSQLDB;
using namespace Utilities;

//<-----------------------------------function that creates new elements and add them to the database--------------------------------------->
NoSQLDB::NoSqlDb<std::string>& TestDBString::makeTestStringDb()
{
	// Ashok, Jiang, Joe, Sam, Veronica

	NoSQLDB::Element<std::string> elem1;
	elem1.name = "New York";
	elem1.category = "State";
	elem1.descr = "US state";
	elem1.path = "../file.cpp";
	elem1.dateTime = DateTime();
	elem1.children.getValue().push_back("Syracuse");
	elem1.children.getValue().push_back("Buffalo");

	NoSQLDB::Element<std::string> elem2;
	elem2.name = "Syracuse";
	elem2.category = "City";
	elem2.descr = "NY city";
	elem2.dateTime = DateTime();
	elem2.path = "../file.cpp";

	NoSQLDB::Element<std::string> elem3;
	elem3.name = "Buffalo";
	elem3.descr = "NY city";
	elem3.category = "City";
	elem3.dateTime = DateTime();
	elem3.path = "../file.cpp";

	NoSQLDB::Element<std::string> elem4;
	elem4.name = "California";
	elem4.category = "State";
	elem4.descr = "US state";
	elem4.dateTime = DateTime();
	elem4.path = "../file.cpp";
	elem4.children.getValue().push_back("San Jose");

	NoSQLDB::Element<std::string> elem5;
	elem5.name = "San Jose";
	elem5.descr = "CA city";
	elem5.category = "City";
	elem5.path = "../file.cpp";
	elem5.dateTime = DateTime();

	static NoSQLDB::NoSqlDb<std::string> db;
	db.saveRecord(elem1.name, elem1);
	db.saveRecord(elem2.name, elem2);
	db.saveRecord(elem3.name, elem3);
	db.saveRecord(elem4.name, elem4);
	db.saveRecord(elem5.name, elem5);

	return db;
}

//<----------------------function that creates new elements for project package structure and add them to the database---------------------->
NoSQLDB::NoSqlDb<std::string> TestDBString::makeProject1Db()
{
	using namespace NoSQLDB;
	NoSqlDb<std::string> projDB;
	Element<std::string> elem;	
	Key key = "CppProperties"; elem.name = key;
	elem.descr = "Package"; elem.category = "Package"; 	elem.dateTime = DateTime().now(); elem.path = "../../NoSqldb";
	projDB.saveRecord(key, elem);	
	key = "DateTime"; elem.name = key;
	elem.descr = "Package"; elem.category = "Package"; elem.dateTime = DateTime().now(); elem.path = "../../NoSqldb";
	elem.children.getValue().clear(); elem.saveChild("TestUtilities");
	projDB.saveRecord(key, elem);
	key = "Display"; elem.name = key;
	elem.descr = "Package"; elem.category = "Package"; elem.dateTime = DateTime().now(); elem.path = "../../NoSqldb";
	elem.children.getValue().clear();
	elem.saveChild("DateTime"); elem.saveChild("TestUtilities"); elem.saveChild("NoSqlDb");
	projDB.saveRecord(key, elem);		
	key = "Exec"; elem.name = key;
	elem.descr = "Package"; elem.category = "Package"; elem.dateTime = DateTime().now(); elem.path = "../../NoSqldb";
	elem.children.getValue().clear();
	elem.saveChild("Display"); 	elem.saveChild("DateTime");
	elem.saveChild("NoSqlDb"); 	elem.saveChild("CppProperties"); 	
	elem.saveChild("TestUtilities"); elem.saveChild("XmlDocument");		
	projDB.saveRecord(key, elem);	
	key = "NoSqlDb"; elem.name = key;
	elem.descr = "Package"; elem.category = "Package"; 	elem.dateTime = DateTime().now(); elem.path = "../../NoSqldb";
	elem.children.getValue().clear(); elem.saveChild("DateTime"); elem.saveChild("TestUtilities");
	projDB.saveRecord(key, elem);	
	key = "Persist"; elem.name = key;
	elem.descr = "Package"; elem.category = "Package"; elem.dateTime = DateTime().now(); elem.path = "../../NoSqldb";
	elem.children.getValue().clear();
	elem.saveChild("Display"); 	elem.saveChild("DateTime");
	elem.saveChild("NoSqlDb"); 	elem.saveChild("CppProperties");
	elem.saveChild("TestUtilities"); elem.saveChild("XmlDocument");
	projDB.saveRecord(key, elem);	
	key = "Queries"; elem.name = key;
	elem.descr = "Package"; elem.category = "Package"; elem.dateTime = DateTime().now(); elem.path = "../../NoSqldb";
	elem.children.getValue().clear();
	elem.saveChild("DateTime"); elem.saveChild("Display"); elem.saveChild("NoSqlDb"); elem.saveChild("TestUtilities");
	projDB.saveRecord(key, elem);		
	key = "TestUtilities"; elem.name = key;
	elem.descr = "Package"; elem.category = "Package"; elem.dateTime = DateTime().now(); elem.path = "../../NoSqldb";
	elem.children.getValue().clear();	
	projDB.saveRecord(key, elem);	
	key = "XmlDocument"; elem.name = key;
	elem.descr = "Package"; elem.category = "Package"; elem.dateTime = DateTime().now(); elem.path = "../../NoSqldb";
	elem.children.getValue().clear();
	projDB.saveRecord(key, elem);
	return projDB;
}

//<-----------------------------test stub----------------------------------------->
#ifdef TEST_EXEC
int main()
{
	Title("                                             Key/Value Database                                              ");
	putLines(1);

	TestDBString test;
	test.DemoReq1();
	test.DemoReq2();
	test.DemoReq3();
	test.DemoReq4();
	test.DemoReq5();
	test.DemoReq6();
	test.DemoReq7a();
	test.DemoReq7b();
	test.DemoReq8();
	test.DemoReq9();
	test.DemoReq10();
	test.DemoReq11();
	test.DemoReq12();
	test.DemoReq13();
	std::cout << "\n\n";
	std::cin.get();
}
#endif
