//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Utilities.cpp	: general purpose utility functions and classes										//
// ver 1.0																								//
// Platform         : Dell Inspiron 13 - Windows 10, Visual Studio 2017                                 //-|_ 
// Language         : C++ using the facilities of the standard C++ Libraries                            //-|  <----------Requirement 1---------->
// Application      : Project 1 [Key/Value Databse] Object Oriented Design CSE-687 Spring'18            //
// Author           : Dr. Jim Fawcett, EECS, SU                                                         //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package defines a single class, Convert<T> that supports convering
* instances of the T type to and from strings.  Also included are a few
* global functions intended to simplify code (slightly).
*
* Public Interface :
* ------------------
* template<typename T> std::string Convert<T>::toString(const T& t) - serialize t to a std::string
* template<typename T> T Convert<T>::fromString(const std::string& str) - create an instance of T from a std::string
* template<typename T> std::string Convert<T>::id(const T& t) - create an id string from an instance of T using RTTI
* template<> inline std::string Convert<std::string>::toString(const std::string& str) - specialization for std::strings
* template<> inline std::string Convert<std::string>::fromString(const std::string& str) - specialization for std::strings
* inline void putLine() - put blank line on command prompt
* inline void putLines(size_t n) - put blank lines of given number on command prompt
* inline void titleCore(const std::string& msg, char underline = '-') - convert title into proper format for command prompt
* inline void Title(const std::string& msg)  -  global function that gets the title
* inline void subTitle(const std::string& msg) - global function that gets the sub-title
* static inline int isspaceHelper(int ch) - string trim function:
* static inline std::string &ltrim(std::string &s) - trim from start
* static inline std::string &rtrim(std::string &s) - trim from end
* inline std::string &trim(std::string &s) - trim from both end
*
* Build Process:
* --------------
*   devenv NoSqldb.sln /debug rebuild
*
* Required Files:
* ---------------
*   Utilities.h, Utilities.cpp
*
* Maintenance History:
*---------------------
* ver 1.0 : 06 Feb 2017
* - first release
*/
#include "Utilities.h"
using namespace Utilities;

//<-----------------------test stub----------------------------------->
#ifdef TEST_UTILITIES
int main()
{
	Title("this is a lead title");
	putLine();
	subTitle("this is a sub-title");
	putLines(4);
}
#endif

