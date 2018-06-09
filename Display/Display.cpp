//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Display.cpp      : display NoSqlDb contents															//
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
* This package defines a single class, Display that shows contents
* of an instance of NoSqlDb<Data>.  It accepts a reference to a NoSqlDb<Data>
* database from which it displays records.
*
* Public Interface :
* ------------------
* template<typename Data> void Display<Data>::showSummary(Key key, size_t fieldwidth, std::ostream& out) - show single record in one-line format
* template<typename Data> void Display<Data>::showDetails(Key key, size_t fieldwidth, std::ostream& out) - show single record with detailed, multi-line, format
* template<typename Data> void Display<Data>::show(Key key, Style style, size_t fieldwidth, std::ostream& out) - show single record
* template<typename Data> void Display<Data>::showAll(Style style, size_t fieldwidth, std::ostream& out) - show all records
* template<typename Data> void Display<Data>::showQuery(Keys keys, Style style, size_t fieldwidth, std::ostream& out) - show all records in query
* template<typename Data> void Display<Data>::showHeader(Style style, size_t fieldwidth, std::ostream& out) - shows underlined names of each field for summary output >----
*
* Build Process:
* --------------
*   devenv NoSqldb.sln /debug rebuild
*
* Required Files:
* ---------------
*   Display.h, Display.cpp,
*   NoSqlDb.h, NoSqlDb.cpp,
*   CppProperties.h, CppProperties.cpp
*   DateTime.h, DateTime.cpp
*
* Maintenance History:
*---------------------
* ver 1.0 : 06 Feb 2018
* - first release
*/
#include "Display.h"

//<-------------------------test stub----------------------------------------->
#ifdef TEST_DISPLAY

using namespace NoSQLDB;
using StrData = std::string;
using intData = int;

int main()
{
	std::cout << "\n  Demonstrating Display Package";
	std::cout << "\n ===============================\n";
	std::cout << "\n  Creating and saving NoSqlDb elements with string data";
	std::cout << "\n -------------------------------------------------------\n";
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

	Display<StrData> display(db);
	display.showHeader();
	display.show(elem1.name);
	std::cout << "\n";
	display.show(elem2.name, Display<StrData>::detailed);
	std::cout << "\n";
	display.showAll(Display<StrData>::summary, 12);

	std::cout << "\n  Retrieving elements from NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";
	std::cout << "\n  size of db = " << db.count() << "\n";
	Keys keys = db.keys();
	for (Key key : keys){
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
	std::cout << "\n\n";	
}
#endif



