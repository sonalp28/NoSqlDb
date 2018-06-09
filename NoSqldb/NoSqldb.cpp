//////////////////////////////////////////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - key/value pair in-memory database														//
// ver 1.2																								//
// Platform         : Dell Inspiron 13 - Windows 10, Visual Studio 2017                                 //-|_ 
// Language         : C++ using the facilities of the standard C++ Libraries                            //-|  <----------Requirement 1---------->
// Application      : Project 1 [Key/Value Databse] Object Oriented Design CSE-687 Spring'18            //
// Author           : Dr. Jim Fawcett, EECS, SU                                                         //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package defines two classes that support key/value storage:
* - Element<Data> defines a single value stored in the database.
*   It provides metadata properties:
* - name, description, datetime, children, categories, filepath
* - manual information
* - maintenance information
* - query mechanism
* - persistance mechanism
* - TestExecutive that demonstrates the requirements you meet
*
* Build Process:
* --------------
*   devenv NoSqldb.sln /debug rebuild
*
* Required Files:
* ---------------
*   NoSqldb.h, NoSqldb.cpp, DateTime.h, DateTime.cpp
*
* Maintenance History:
*---------------------
* ver 1.2 : 06 Feb 2017
* - added children
* - modified Element<Data>::show()
* - added these comments
* ver 1.1 : 28 Jan 2017
* - fixed bug in NoSqlDb::count() by replacing
*   unordered_map<key,Value>::count() with
*   unordered_map<key,Value>::size();
* Ver 1.0 : 25 Jan 2017
* - first release
*/

#include "NoSqlDb.h"
#include <iostream>

#ifdef TEST_NOSQLDB

using namespace NoSQLDB;
using StrData = std::string;
using intData = int;

int main()
{
	std::cout << "\n  Demonstrating NoSql Helper Code";
	std::cout << "\n  Creating and saving NoSqlDb elements with string data";
	NoSqlDb<StrData> db;
	Element<StrData> elem1;
	elem1.name = "elem1";
	elem1.category = "test";
	db.saveRecord(elem1.name, elem1);
	Element<StrData> elem2;
	elem2.name = "elem2";
	elem2.category = "test";
	db.saveRecord(elem2.name, elem2);
	elem1.saveChild(elem2.name);
	db.saveValue(elem1.name, elem1);
	db.saveValue(elem2.name, elem2);
	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n  size of db = " << db.count() << "\n";
	Keys keys = db.keys();
	for (Key key : keys){
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
	std::cout << "\n  Erasing element with key elem2";
	if (db.removeRecord("elem2"))
		std::cout << "\n  removed record and all children references to this record";
	std::cout << "\n  size of db = " << db.count() << "\n";
	keys = db.keys();
	for (Key key : keys){
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
}
#endif
