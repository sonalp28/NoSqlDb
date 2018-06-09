#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// NoSqldb.h		: key/value pair in-memory database										            //
// ver 1.0										                                                        //
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
* ver 1.0 : 06 Feb 2018
* - first release
*/

#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include "../CppProperties/CppProperties.h"
#include "../DateTime/DateTime.h"

namespace NoSQLDB
{
	using Key = std::string;
	using Keys = std::vector<Key>;
	using Name = std::string;
	using Description = std::string;
	using Category = std::string;
	using Filepath = std::string;
	using Children = std::vector<Key>;

	/////////////////////////////////////////////////////////////////////
	// Element class represents a data record in our NoSql database
	// - in our NoSql database that is just the value in a key/value pair
	// - it needs to store child data, something for you to implement
	//
	template<typename Data>
	class Element
	{
	public:

		Element() {}
		Element(Name aName, Category aCat) : name(aName), category(aCat) {};

		Property<Name> name;                  // metadata
		Property<Description> descr;		  // metadata
		Property<Category> category;          // metadata
		Property<DateTime> dateTime;          // metadata
		Property<Filepath> path;
		Property<Children> children;          // metadata

		void saveChild(Key);
		std::string show();
	};

	template<typename Data>
	void Element<Data>::saveChild(Key key)
	{
		children.getValue().push_back(key);
	}

	template<typename Data>
	std::string Element<Data>::show()
	{
		std::ostringstream out;
		out.setf(std::ios::adjustfield, std::ios::left);
		out << "\n    " << std::setw(8) << "name" << " : " << name;
		out << "\n    " << std::setw(8) << "category" << " : " << category;
		out << "\n    " << std::setw(8) << "dateTime" << " : " << DateTime(dateTime).time();
		Children children_ = static_cast<Children>(children);
		if (children_.size() > 0)
		{
			out << "\n    " << std::setw(8) << "children" << " : ";
			for (size_t i = 0; i < children_.size(); ++i)
			{
				out << children_[i];
				if (i < children_.size())
					out << ", ";
			}
		}
//		out << "\n    " << std::setw(8) << "data" << " : " << data;
		out << "\n";
		return out.str();
	}

	/////////////////////////////////////////////////////////////////////
	// NoSqlDb class is a key/value pair in-memory database
	// - stores and retrieves elements
	// - you will need to add query capability
	//   That should probably be another class that uses NoSqlDb
	//
	template<typename Data>
	class NoSqlDb
	{
	public:
		using Key = std::string;
		using Keys = std::vector<Key>;

		Keys keys();
		bool hasKey(Key key);
		bool saveRecord(Key key, Element<Data> elem);
		bool saveValue(Key key, Element<Data> elem);
		bool removeRecord(Key key);
		Element<Data> value(Key key);
		size_t count();
	private:
		using Item = std::pair<Key, Element<Data>>;

		std::unordered_map<Key, Element<Data>> store;
	};

	//<----------------------key() function that gives keys--------------------------------->
	template<typename Data>
	typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
	{
		Keys keys;
		for (Item item : store)
		{
			keys.push_back(item.first);
		}
		return keys;
	}

	//<-----------------------haskey() function checks whether key is present or not & returns bool result-------------------------------->
	template<typename Data>
	bool NoSqlDb<Data>::hasKey(Key key)
	{
		if (store.find(key) == store.end())
			return false;
		return true;
	}

	//<-----------------------saveRecord() function is used to save new record in the database-------------------------------->
	template<typename Data>
	bool NoSqlDb<Data>::saveRecord(Key key, Element<Data> elem)
	{
		if (store.find(key) != store.end())
			return false;
		store[key] = elem;
		return true;
	}

	//<-------------------------removeRecord() function is used to delete a exisiting record in the database------------------------------>
	template<typename Data>
	bool NoSqlDb<Data>::removeRecord(Key key)
	{
		if (store.find(key) == store.end())
			return false;
		size_t numErased = store.erase(key);
		if (numErased == 1)
		{
			// remove key from all other element's children collections
			for (Key k : keys())
			{
				Keys& children = store[k].children.getValue();  // note Keys& - we don't want copy of children
				Keys::iterator iter;
				for (iter = children.begin(); iter != children.end(); ++iter)
				{
					if ((*iter) == key)
					{
						children.erase(iter);
						break;
					}
				}
			}
			return true;
		}
		return false;
	}

	//<-------------------------this function is used to store a value for given key in database element------------------------------>
	template<typename Data>
	bool NoSqlDb<Data>::saveValue(Key key, Element<Data> elem)
	{
		if (store.find(key) == store.end())
			return false;
		store[key] = elem;
		return true;
	}

	//<--------------------------this function matches the key and gives back the value for that key----------------------------->
	template<typename Data>
	Element<Data> NoSqlDb<Data>::value(Key key)
	{
		if (store.find(key) != store.end())
			return store[key];
		return Element<Data>();
	}

	//<---------------------------this function gives back the count in the database---------------------------->
	template<typename Data>
	size_t NoSqlDb<Data>::count()
	{
		return store.size();
	}
}
