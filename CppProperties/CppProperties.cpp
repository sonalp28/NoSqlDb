//////////////////////////////////////////////////////////////////////////////////////////////////////////
// CppProperties.cpp: Defines read/write and read only properties										//
// ver 1.1																								//
// Platform         : Dell Inspiron 13 - Windows 10, Visual Studio 2017                                 //-|_ 
// Language         : C++ using the facilities of the standard C++ Libraries                            //-|  <----------Requirement 1---------->
// Application      : Project 1 [Key/Value Databse] Object Oriented Design CSE-687 Spring'18            //
// Author           : Dr. Jim Fawcett, EECS, SU                                                         //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* Public interface:
* -----------------
* Z class :
  // - demonstrates how to customize getter and setter functions
  //   without changing public interface of Z.
  // - That is important because it means that should we decide,
  //   after a class was published, that we should control the
  //   way the property behaves, we won't break existing clients.
*
* Public Interface :
* ------------------
* std::string toUpper(const std::string& str) - demo function for custom getter and setter
*
* Build Process:
* --------------
*   devenv NoSqldb.sln /debug rebuild
*
* Required Files:
* ---------------
*   CppProperties.h, CppProperties.cpp
*
* Maintenance History:
* --------------------
* ver 1.1 : Jan 28 2017
* - fixed bug in Property class by removing bound data from all lambdas.
* ver 1.0 : Jan 08 2017
* - first release
*/

#include "CppProperties.h"
#include <string>
#include <iostream>

class X
{
public:
	
	void show()
	{
		std::cout << "\n  name:      " << name;
		std::cout << "\n  isMutable: " << isMutable;
		std::cout << "\n  pi:        " << pi;
		std::cout << "\n  constName: " << constName;
		std::cout << "\n";
	}
private:
	Property<std::string> name;
	Property<double> isMutable;
	Property<const double> pi = 3.1415927;
	Property<const std::string> constName = "a const name";
};

/////////////////////////////////////////////////////////////////////
// Z class:
// - demonstrates how to customize getter and setter functions
//   without changing public interface of Z.
// - That is important because it means that should we decide,
//   after a class was published, that we should control the
//   way the property behaves, we won't break existing clients.

/*----< demo function for custom getter and setter >---------------*/

std::string toUpper(const std::string& str)
{
	std::string upperS = str;
	for (auto& c : upperS)
		c = toupper(c);
	return upperS;
}

/////////////////////////////////////////////////////////////////////
// - We can also modify the behavior of the propert by overriding
//   the property virtual functions.  That, however, modifies user
//   interface.
//
class UpperCaseProperty : public Property<std::string>  // changes property name
{
public:
	UpperCaseProperty(const std::string& s)
	{
		value = toUpper(s);
	}
	virtual Property<std::string>& operator=(const std::string &s)
	{
		value = toUpper(s);
		return *this;
	}
};

class Y
{
private:
	UpperCaseProperty name = "jim";
};

class Z
{
public:
	Z()
	{
		name.newSetter(
			[](const std::string& s) {
			// normally you won't want to use console output - this is just for demo
			std::cout << "\n  setter converting property to uppercase";
			return toUpper(s);
		}
		);
		// this also works:
		// name.setter(toUpper);

		name.newGetter(
			[](const std::string& val) {
			std::string s = val;
			s[0] = tolower(s[0]);
			// normally you won't want to use console output - this is just for demo
			std::cout << "\n  getter converting first character of property to lowercase";
			return s;
		}
		);
	}
	Property<std::string> name;
};

#ifdef TEST_CPPPROPERTIES
int main()
{
	std::cout << "\n  Demonstrate C++ Properties class";
	std::cout << "\n ==================================";
	X x;
	x.name = "Jim";
	x.show();
	std::cout << "\n  setting isMutable to pi:";
	std::cout << "\n --------------------------";
	x.isMutable = x.pi;
	x.show();
	//x.constName = "a new name";  // fails to compile - that's good
	std::cout << "\n  making a copy of x, called newX with new name:";
	std::cout << "\n ------------------------------------------------";
	X newX(x);
	newX.name = "joe";
	newX.show();
	std::cout << "\n";
	std::cout << "\n  demonstrate overriding property \"getter\" function to return capitalized name string";
	std::cout << "\n -------------------------------------------------------------------------------------";
	Y y;
	std::cout << "\n  y.name = " << y.name;
	std::cout << "\n";
	std::cout << "\n  demonstrate providing custom getter and setter functions";
	std::cout << "\n ----------------------------------------------------------";
	std::cout << "\n";
	Z z;
	z.name = "fawcett";
	std::string temp = z.name;
	std::cout << "\n  z.name = " << temp;
	temp = (z.name).getValue();
	std::cout << "\n  name property's internal store value is: " << temp;
	std::cout << "\n";
	std::cout << "\n  making a copy of z, called newZ:";
	std::cout << "\n ----------------------------------";
	Z newZ(z);
	temp = newZ.name;
	std::cout << "\n  newZ.name = " << temp;
	std::cout << "\n\n";
}
#endif


