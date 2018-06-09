#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Display.h        : display NoSqlDb contents															//
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

#include "../NoSqldb/NoSqldb.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>

namespace NoSQLDB
{
	template <typename Data>
	class Display
	{
	public:
		enum Style { summary, detailed };

		Display(NoSqlDb<Data>& db) : db_(db) {}

		void show(Key key, Style style = summary, size_t fieldwidth = 10, std::ostream& out = std::cout);
		void showAll(Style style = summary, size_t fieldwidth = 25, std::ostream& out = std::cout);
		void showAllproj1(Style style = summary, size_t fieldwidth = 25, std::ostream& out = std::cout);
		void showHeader(Style style = summary, size_t fieldwidth = 25, std::ostream& out = std::cout);
		void showQuery(Keys keys, Style style = summary, size_t fieldwidth = 10, std::ostream& out = std::cout);

	private:
		void showSummary(Key key, size_t fieldwidth, std::ostream& out);
		void showSummaryproj1(Key key, size_t fieldwidth, std::ostream& out);
		void showDetails(Key key, size_t fieldwidth, std::ostream& out);
		NoSqlDb<Data>& db_;
	};
	
	//----< show single record in one-line format >----------------------
	template<typename Data>
	void Display<Data>::showSummary(Key key, size_t fieldwidth, std::ostream& out)
	{
		if (!db_.hasKey(key))
			return;
		int count = 0;
		Element<Data> elem = db_.value(key);
		int save = out.setf(std::ios::left, std::ios::adjustfield);
		out << "  ";
		out << std::setw(10) << key.substr(0, 10) << " ";
		out << std::setw(10) << static_cast<std::string>(elem.name).substr(0, 10) << " ";
		out << std::setw(12) << static_cast<std::string>(elem.descr).substr(0, 12) << " ";
		DateTime dt = elem.dateTime;
		out << std::setw(fieldwidth) << static_cast<std::string>(dt).substr(0, fieldwidth) << " ";
		if (elem.children.getValue().size() > 0)
		{
			count = elem.children.getValue().size();
			for (Key k : elem.children.getValue())
				out << std::setw(10) << static_cast<std::string>(k).substr(0, 10) << " ";
			if (count < 2)
				out << "           ";
		}
		else
			out << "                      ";
		out << std::setw(10) << static_cast<std::string>(elem.category).substr(0, 10) << " ";
		out << std::setw(12) << static_cast<std::string>(elem.path).substr(0, 12) << " ";
		out << "\n";
		out.setf(save);
	}
	
	//----< show single record in one-line format >----------------------
	template<typename Data>
	void Display<Data>::showSummaryproj1(Key key, size_t fieldwidth, std::ostream& out)
	{
		if (!db_.hasKey(key))
			return;
		int count = 0;
		Element<Data> elem = db_.value(key);
		int save = out.setf(std::ios::left, std::ios::adjustfield);
		out << "  ";
		out << std::setw(10) << key.substr(0, 10) << " ";
		out << std::setw(10) << static_cast<std::string>(elem.name).substr(0, 10) << " ";
		out << std::setw(12) << static_cast<std::string>(elem.descr).substr(0, 12) << " ";
		DateTime dt = elem.dateTime;
		out << std::setw(fieldwidth) << static_cast<std::string>(dt).substr(0, fieldwidth) << " ";
		if (elem.children.getValue().size() > 0)
		{
			count = elem.children.getValue().size();
			for (Key k : elem.children.getValue())
				out << std::setw(10) << static_cast<std::string>(k).substr(0, 10) << " ";
			if (count < 6) {
				for(int i = 6-count; i > 0; i--)
					out << "           ";
			}				
		}
		else {
			for(int i = 0; i < 6; i++)
				out << "           ";		
		}			
		out << std::setw(10) << static_cast<std::string>(elem.category).substr(0, 10) << " ";
		out << std::setw(12) << static_cast<std::string>(elem.path).substr(0, 12) << " ";
		out << "\n";
		out.setf(save);
	}

	//----< show single record with detailed, multi-line, format >-------
	template<typename Data>
	void Display<Data>::showDetails(Key key, size_t fieldwidth, std::ostream& out)
	{
		if (!db_.hasKey(key))
			return;
		Element<Data> elem = db_.value(key);
		int save = out.setf(std::ios::left, std::ios::adjustfield);
		out << "  ";
		out << "key      : " << key << "\n  ";
		out << "Name     : " << static_cast<std::string>(elem.name) << "\n  ";
		out << "Category : " << static_cast<std::string>(elem.category) << "\n  ";
		DateTime dt = elem.dateTime;
		out << "DateTime : " << static_cast<std::string>(dt) << "\n  ";
		out << "Children :\n    ";
		if (elem.children.getValue().size() > 0)
		{
			for (Key k : elem.children.getValue())
				out << std::setw(fieldwidth) << static_cast<std::string>(k).substr(0, fieldwidth) << " ";
		}
		out << "\n";
		out.setf(save);
	}
	
	//----< show single record >-----------------------------------------
	/*
	* Style: Display<Data>::summary or Display<Data>::detailed
	* fieldwidth: number of characters for each field
	* out: stream for output - typically std::cout (default) or std::ostringstream
	*/
	template<typename Data>
	void Display<Data>::show(Key key, Style style, size_t fieldwidth, std::ostream& out)
	{
		if (!db_.hasKey(key))
			return;
		if (style == Display<Data>::Style::summary)
			showSummary(key, fieldwidth, out);
		else
			showDetails(key, fieldwidth, out);
	}
	
	//----< show all records >-------------------------------------------
	/*
	* Style: Display<Data>::summary or Display<Data>::detailed
	* fieldwidth: number of characters for each field
	* out: stream for output - typically std::cout (default) or std::ostringstream
	*/
	template<typename Data>
	void Display<Data>::showAll(Style style, size_t fieldwidth, std::ostream& out)
	{
		Keys keys = db_.keys();
		if (style == Display<Data>::Style::summary)
		{
			showHeader(style, fieldwidth, out);
			for (Key k : keys)
				showSummary(k, fieldwidth, out);
		}
		else
		{
			for (Key k : keys)
				showDetails(k, fieldwidth, out);
		}
	}
	

	//----< show all records >-------------------------------------------
	/*
	* Style: Display<Data>::summary or Display<Data>::detailed
	* fieldwidth: number of characters for each field
	* out: stream for output - typically std::cout (default) or std::ostringstream
	*/
	template<typename Data>
	void Display<Data>::showAllproj1(Style style, size_t fieldwidth, std::ostream& out)
	{
		Keys keys = db_.keys();
		if (style == Display<Data>::Style::summary)
		{
			showHeader(style, fieldwidth, out);
			for (Key k : keys)
				showSummaryproj1(k, fieldwidth, out);
		}
		else
		{
			for (Key k : keys)
				showDetails(k, fieldwidth, out);
		}
	}

	//----< show all records in query >----------------------------------
	/*
	* Style: Display<Data>::summary or Display<Data>::detailed
	* fieldwidth: number of characters for each field
	* out: stream for output - typically std::cout (default) or std::ostringstream
	*/
	template<typename Data>
	void Display<Data>::showQuery(Keys keys, Style style, size_t fieldwidth, std::ostream& out)
	{
		if (style == Display<Data>::Style::summary)
		{
			showHeader(style, fieldwidth, out);
			for (Key k : keys)
				showSummary(k, fieldwidth, out);
		}
		else
		{
			for (Key k : keys)
				showDetails(k, fieldwidth, out);
		}
	}
	
	//----< shows underlined names of each field for summary output >----
	template<typename Data>
	void Display<Data>::showHeader(Style style, size_t fieldwidth, std::ostream& out)
	{
		size_t numChildren = 0;
		Keys keys = db_.keys();
		for (Key k : keys)
		{
			size_t size = db_.value(k).children.getValue().size();
			if (numChildren < size)
				numChildren = size;
		}
		int save = out.setf(std::ios::left, std::ios::adjustfield);
		out << "  ";
		out << std::setw(10) << std::string("Key").substr(0, fieldwidth) << " ";
		out << std::setw(10) << std::string("Name").substr(0, fieldwidth) << " ";
		out << std::setw(12) << std::string("Description").substr(0, fieldwidth) << " ";
		out << std::setw(fieldwidth) << std::string("DateTime").substr(0, fieldwidth) << " ";
		if (numChildren > 0)
		{
			for (size_t i = 0; i < numChildren; ++i)
				out << std::setw(10) << std::string("Child").substr(0, 10) << " ";
		}
		out << std::setw(10) << std::string("Category").substr(0, 10) << " ";
		out << std::setw(12) << std::string("FilePath").substr(0, 12) << " ";
		out << "\n  ";
		for (size_t i = 0; i < numChildren + 6; ++i) {
			if(i == 3)
				out << std::setw(27) << std::string("  -------------------------").substr(0, 27) << " ";
			else
				out << std::setw(10) << std::string("----------").substr(0, 10) << " ";
		}			
		out << "\n";
		out.setf(save);
	}
}
