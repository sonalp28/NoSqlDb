//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Queries.cpp		: retrieve NoSqlDb contents														    //
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
* This package defines a single class, Query that provides an execute(...)
* to execute a query callable object.  It also defines a type for
* callable object queries that return keysets.
*
* Public Interface :
* ------------------
* template<typename Data> Keys Query<Data>::execute(QueryType q, const Arg& arg, const Keys& keys) - returns keyset of elements that match query
* template<typename Data> void TestQueries<Data>::doQueries(NoSQLDB::NoSqlDb<Data>& db) - run all query tests
* template<typename Data> void TestQueries<Data>::TestQuery1(const std::string& arg) - query for key
* template<typename Data> void TestQueries<Data>::TestQuery2(const std::string& arg) - query for children of specified element
* template<typename Data> void TestQueries<Data>::TestQuery3(const std::string& arg) - get keys that match regex
* template<typename Data> void TestQueries<Data>::TestQuery4(const std::string& arg) - get set of all keys that contain a specified string in their metadata section
* template<typename Data> void TestQueries<Data>::TestQuery5(DateTime dt1, DateTime dt2) - get elements with DateTime in specified interval

* Build Process:
* --------------
*   devenv NoSqldb.sln /debug rebuild
*
* Required Files:
* ---------------
*   Queries.h, Queries.cpp,
*   dbCore.h, dbCore.cpp,
*   Display.h, Display.cpp,
*	DateTime.h, DateTime.cpp
*
* Maintenance History:
*---------------------
* ver 1.0 : 06 Feb 2018
* - first release
*/
#include "Queries.h"
#include "../NoSqldb/NoSqldb.h"
#include "../Display/Display.h"
#include <iostream>
#include <string>

//<---------------------------------test stub[testing purposes]--------------------------------------->
#ifdef TEST_QUERIES

using namespace NoSQLDB;

int main()
{
	using StrData = std::string;
	std::cout << "\n  Demonstrating NoSql Queries";
	NoSqlDb<StrData> db;
	Element<StrData> elem1;
	elem1.name = "elem1"; elem1.category = "test";
	db.saveRecord(elem1.name, elem1);
	Element<StrData> elem2;
	elem2.name = "elem2"; elem2.category = "test";
	db.saveRecord(elem2.name, elem2);
	elem1.saveChild(elem2.name);
	db.saveValue(elem1.name, elem1);
	db.saveValue(elem2.name, elem2);
	std::cout << "\n  Displaying elements from NoSqlDb<string>";
	Display<StrData> display(db);
	display.showAll();
	std::cout << "\n  Testing Queries";
	Query<StrData> query(db);
	Query<StrData>::QueryType debugNoQuery = [](const std::string& arg, Keys keys){
		std::cout << "\n  debugNoQuery:" << "\n  " << arg;
		return keys;
	};
	query.execute(debugNoQuery, "Hello query world!", db.keys());
	Query<StrData>::QueryType names = [&db](const std::string& arg, Keys keys){
		std::cout << "\n  names NoQuery:" << "\n  names in db: ";
		for (Key k : keys)
			std::cout << db.value(k).name << " ";
		return keys;
	};
	query.execute(names, "", db.keys());
	Query<StrData>::QueryType getElem = [&db](const Query<StrData>::Arg& arg, Keys keys){
		Keys returnKeys;
		std::cout << "\n  getElem Query";
		for (Key k : keys){
			if ((db.value(k)).name.getValue() == arg)
				returnKeys.push_back(arg);
		}
		return returnKeys;
	};
	Keys qkeys = query.execute(getElem, "elem2", db.keys());
	std::cout << "\n  size of returned keyset = " << qkeys.size() << "\n  returned keys are: ";
	for (Key k : qkeys)
		std::cout << k << " ";
	display.showQuery(qkeys);
	TestQueries<std::string> tq(std::cout);
	tq.doQueries(db);
}
#endif

