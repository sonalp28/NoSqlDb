#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// TestQueries.h    : Provides Testing for Queries in TestNoSqlDb										//
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
* This package defines class TestExQueries, that demonstrates each of
* the query requirements.
*
* Public Interface :
* ------------------
* template<typename Data> void TestQueries<Data>::doQueries(NoSQLDB::NoSqlDb<Data>& db) - run all query tests
* template<typename Data> void TestQueries<Data>::TestQuery1(const std::string& arg) - query for key
* template<typename Data> void TestQueries<Data>::TestQuery2(const std::string& arg) - query for children of specified element
* template<typename Data> void TestQueries<Data>::TestQuery3(const std::string& arg) - get keys that match regex
* template<typename Data> void TestQueries<Data>::TestQuery4(const std::string& arg) - get set of all keys that contain a specified string in their metadata section
* template<typename Data> void TestQueries<Data>::TestQuery5(DateTime dt1, DateTime dt2) - get elements with DateTime in specified interval
*
* Required Files:
* ---------------
*   TestQueries.h
*   Queries.h, Queries.cpp
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
#include "../Queries/Queries.h"
#include <string>
#include <iostream>

template<typename Data>
class TestExQueries
{
public:
	TestExQueries(std::ostream& out) : out_(out) {};
	void doQueries(NoSQLDB::NoSqlDb<Data>& db);
	void doPayloadQueries(NoSQLDB::NoSqlDb<Data>& db, const std::string& arg1, const std::string& arg2);
private:
	NoSQLDB::NoSqlDb<Data>* pDb = nullptr;
	std::ostream& out_;
	void TestQuery1(const std::string& arg);
	void TestQuery2(const std::string& arg);
	void TestQuery3(const std::string& arg);
	void TestQuery4(const std::string& arg);
	void TestQuery5(DateTime dt1, DateTime dt2);
};

//----< run all query tests >----------------------------------------
template<typename Data>
void TestExQueries<Data>::doQueries(NoSQLDB::NoSqlDb<Data>& db)
{
	pDb = &db;
	Utilities::subTitle("Current Database");
	Utilities::putLine();
	Display<Data> display(*pDb);
	display.showAll();

	TestQuery1("Buffalo");
	TestQuery2("New York");
	std::string regEx = ".*e.*";
	TestQuery3(regEx);
	regEx = "City";
	TestQuery4(regEx);
	DateTime dt = DateTime().now();
	DateTime::Duration dur = DateTime::makeDuration(1, 0, 0, 0);
	TestQuery5(dt - dur, dt + dur);
	std::cout << "\n";
}

//----< query for key >----------------------------------------------
template<typename Data>
void TestExQueries<Data>::TestQuery1(const std::string& arg)
{
	Query<Data>::QueryType getKeyIfExists = [](const std::string& arg, Keys keys)
	{
		NoSQLDB::Keys returnKeys;
		for (NoSQLDB::Key k : keys)
		{
			if (k == arg)
			{
				returnKeys.push_back(static_cast<std::string>(k));
				break;
			}
		}
		return returnKeys;
	};

	std::cout << "\n  Query #1 : Value of a specified key\n             [Get Value for Key =\"" << arg << "\"]";
	Keys keys = pDb->keys();
	keys = getKeyIfExists(arg, keys);
	std::cout << "\n  Result - ";
	if (keys.size() == 1) {
		Element<Data> elem;
		for (NoSQLDB::Key k : keys)
			elem = pDb->value(k);
		std::cout << "  Values for the given key - ";
		std::cout << "\n             Name        - " << elem.name;
		std::cout << "\n             Description - " << elem.descr;
		DateTime dt = elem.dateTime;
		std::cout << "\n             DateTime    - " << dt.time();
		std::cout << "\n             Category    - " << elem.category;
		std::cout << "\n             File Path   - " << elem.path;
	}
	else
		std::cout << "\n    Sorry! Didn't find the specified key! \"" << arg << "\"";
}

//----< query for children of specified element >--------------------
template<typename Data>
void TestExQueries<Data>::TestQuery2(const std::string& arg)
{
	Query<Data>::QueryType getChildren = [&](const std::string& arg, Keys keys)
	{
		NoSQLDB::Keys returnKeys;
		//std::cout << "\n    2. Query #2 - Children of a specified key \n     Get all children for Key =\"" << arg << "\"";
		for (NoSQLDB::Key k : keys)
		{
			if (k == arg)
			{
				returnKeys = pDb->value(k).children;
				break;
			}
		}
		return returnKeys;
	};

	std::cout << "\n\n  Query #2 : Children of a specified key\n             [Get all children for Key =\"" << arg << "\"]";
	Keys keys = pDb->keys();
	keys = getChildren(arg, keys);
	std::cout << "\n  Result :";
	if (keys.size() > 0)
	{
		std::cout << "   Children : ";
		for (Key k : keys)
			std::cout << k << ", ";
	}
	else
		std::cout << "\n    Sorry! Didn't find children for key = \"" << arg << "\"!";
}

//----< get keys that match regex >----------------------------------
template<typename Data>
void TestExQueries<Data>::TestQuery3(const std::string& arg)
{
	Query<Data>::QueryType getRegexKeyMatch = [&](const std::string& arg, Keys keys)
	{
		NoSQLDB::Keys returnKeys;
		std::regex regx(arg);
		for (NoSQLDB::Key k : keys)
		{
			if (std::regex_match(k, regx))
			{
				returnKeys.push_back(k);
			}
		}
		return returnKeys;
	};

	std::cout << "\n\n  Query #3 : Set of all keys matching a specified regular-expression pattern\n             [Get set of all keys matching regex =\"" << arg << "\"]";
	Keys keys = pDb->keys();
	keys = getRegexKeyMatch(arg, keys);
	std::cout << "\n  Result :";
	if (keys.size() > 0)
	{
		std::cout << "   Matches : ";
		for (Key k : keys)
			std::cout << k << ", ";
	}
	else
		std::cout << "\n    Sorry! Didn't find match for key = \"" << arg << "\"!";
}

template<typename Data>
void TestExQueries<Data>::TestQuery4(const std::string& arg)
{
	Query<Data>::QueryType getRegexCategoryMatch = [&](const std::string& arg, Keys keys)
	{
		NoSQLDB::Keys returnKeys;
		//std::cout << "\n    category pattern match Query for \"" << arg << "\"";
		std::regex regx(arg);
		for (NoSQLDB::Key k : keys)
		{
			Element<Data> elem = pDb->value(k);
			if (std::regex_match(static_cast<std::string>(elem.category), regx))
			{
				returnKeys.push_back(k);
			}
		}
		return returnKeys;
	};

	std::cout << "\n\n  Query #4 : All Keys that contain a specified string in their metadata section\n             [Get all keys matching category regex pattern for = \"" << arg << "\"]";
	Keys keys = pDb->keys();
	keys = getRegexCategoryMatch(arg, keys);
	std::cout << "\n  Result :";
	if (keys.size() > 0)
	{
		std::cout << "   Matches : ";
		for (Key k : keys)
			std::cout << k << ", ";
	}
	else
		std::cout << "\n    Sorry! Didn't find match for category regex pattern = \"" << arg << "\"!";
}

//----< get elements with DateTime in specified interval >-----------
template<typename Data>
void TestExQueries<Data>::TestQuery5(DateTime dt1, DateTime dt2)
{
	Query<Data>::QueryType getDateIntervalMatch = [&](const std::string& arg, Keys keys)
	{
		NoSQLDB::Keys returnKeys;
		Element<Data> elem1 = pDb->value(keys[0]);
		DateTime dt;
		elem1.dateTime = (dt + DateTime::makeDuration(2, 0, 0, 0));
		pDb->saveValue(keys[0], elem1);
		//std::cout << "\n    set dateTime of " << keys[0] << " to " << static_cast<DateTime>(elem1.dateTime).time();
		std::cout << "\n             [Get all keys that contain values written in between : " << dt1.time() << " & " << dt2.time() << "]";

		for (NoSQLDB::Key k : keys)
		{
			Element<Data> elem = pDb->value(k);
			DateTime dt = elem.dateTime;
			if (dt1 < dt || dt < dt2)
			{
				returnKeys.push_back(k);
			}
		}
		return returnKeys;
	};

	std::cout << "\n\n  Query #5 : All keys that contain values written within a specified time-date interval ";
	Keys keys = pDb->keys();
	keys = getDateIntervalMatch("", keys);
	std::cout << "\n  Result :";
	if (keys.size() > 0)
	{
		std::cout << "   Matches : ";
		for (Key k : keys)
			std::cout << k << ", ";
	}
	else
		std::cout << "\n    Sorry! Didn't find match for given DateTime interval!";
}

//-----------< do payload queries >----------------------------------
template<typename Data>
void TestExQueries<Data>::doPayloadQueries(NoSQLDB::NoSqlDb<Data>& db, const std::string& arg1, const std::string& arg2)
{
	pDb = &db;
	Utilities::subTitle("Current Database");
	Utilities::putLine();
	Display<Data> display(*pDb);
	display.showAll();
	Query<Data>::QueryType getRegexCategoryMatch = [&](const std::string& arg, Keys keys)
	{
		NoSQLDB::Keys returnKeys;
		std::regex regx(arg);
		for (NoSQLDB::Key k : keys){
			Element<Data> elem = pDb->value(k);
			if (std::regex_match(static_cast<std::string>(elem.category), regx))
				returnKeys.push_back(k);	
			else if (std::regex_match(static_cast<std::string>(elem.path), regx))
				returnKeys.push_back(k);
		}
		return returnKeys;
	};
	std::cout << "\n\n  Payload Query #1 : Set of all Keys that have specified payload category\n                      [Get all keys matching payload category = \"" << arg1 << "\"]";
	Keys keys = pDb->keys();
	keys = getRegexCategoryMatch(arg1, keys);
	std::cout << "\n  Result :";
	if (keys.size() > 0){
		std::cout << "   Matches : ";
		for (Key k : keys)
			std::cout << k << ", ";
	}
	else
		std::cout << "\n    Sorry! Didn't find match for category regex pattern = \"" << arg1 << "\"!";
	std::cout << "\n\n  Payload Query #2 : Set of all Keys that have specified payload file path\n                     [Get all keys matching payload file path = \"" << arg2 << "\"]";
	keys = pDb->keys();
	keys = getRegexCategoryMatch(arg2, keys);
	std::cout << "\n  Result :";
	if (keys.size() > 0){
		std::cout << "   Matches : ";
		for (Key k : keys)
			std::cout << k << ", ";
	}
	else
		std::cout << "\n    Sorry! Didn't find match for category regex pattern = \"" << arg2 << "\"!";
}

