//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tokenizer.cpp    : Reads words from a file															//
// ver 1.0																								//
// Platform         : Dell Inspiron 13 - Windows 10, Visual Studio 2017                                 //-|_ 
// Language         : C++ using the facilities of the standard C++ Libraries                            //-|  <----------Requirement 1---------->
// Application      : Project 1 [Key/Value Databse] Object Oriented Design CSE-687 Spring'18            //
// Author           : Dr. Jim Fawcett, EECS, SU                                                         //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a tokenizer class.  Its instances read words from
an attached file or string.  Word boundaries occur when a character sequence
read from the file or string:
- changes between any of the character types: alphanumeric, punctuator,
or white space.
- certain characters are treated as single character tokens, e.g.,
"(", ")", "{", "}", "[". "]", ";", ".", and "\n".
You can change the selection with Toker::setMode(Toker::code) or
Toker::setMode(Toker::xml), or Toker::setSingleCharTokens(somestring);
- Toker::code is the default tokenizing mode.
A tokenizer is an important part of a scanner, used to read and interpret
source code or XML.

Public Interface:
=================
Toker t;                        // create tokenizer instance
returnComments();               // request comments return as tokens
if(t.attach(someFileName))      // select file for tokenizing
string tok = t.getTok();		// extract first token
int numLines = t.lines();       // return number of lines encountered
t.lines() = 0;                  // reset line count

Build Process:
==============
Required files
- tokenizer.h, tokenizer.cpp

Maintenance History:
====================
ver 1.9 : 18 Mar 10
- added thrown exception if get to end of file in eatQuote()
ver 1.8 : 02 Mar 10
- fixed leak of allocated stream.  That required changes to ~Toker() and attach(...).
ver 1.7 : 31 Jan 09
- made putback(int) a public, rather than private, member function
ver 1.6 : 22 Jan 09
- added Toker::setSingleTokenChars(somestring)
- added notes about setMode and setSingleTokenChars to Manual
and Maintenance information.
ver 1.5 : 17 Jan 09
- added Toker::mode, either code or xml
- changed container for single char token ids from vector<char>
to string
- added test for end of file in isTokEnd() to fix bug, found by
Jim DelloStritto that sometimes caused Toker to fail returning
last character if not a singleCharToken
ver 1.4 : 10 Feb 07
- fixed bug in braceCount to eliminate changing count when brace
is in a quoted string or comment
ver 1.3 : 24 Feb 06
- fixed bug in eat comment that hung on ending comment with no
newline, by testing for stream state good.
ver 1.2 : 06 Feb 06
- added stream closing to destructor and attach memeber functions
ver 1.1 : 01 Feb 06
- added if test at end of getTok() to avoid returning space after
C comment as a token
ver 1.0 : 12 Jan 06
- first release

*/
#include <cctype>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <assert.h>
#include "Tokenizer.h"

//----< constructor may be called with no argument >-----------

Toker::Toker(const std::string& src, bool isFile)
	: prevprevChar(0), prevChar(0), currChar(0), nextChar(0),
	doReturnComments(false), numLines(0), braceCount(0), pIn(0),
	_state(default_state)
{
	if (src.length() > 0)
	{
		if (!attach(src, isFile))
		{
			std::string temp = std::string("can't open ") + src;
			throw std::exception(temp.c_str());
		}
	}
	scTok = "()[]{};.\n";
	if (_mode == xml)
		scTok = "<>!" + scTok;
}

//----< destructor >-------------------------------------------
Toker::~Toker()
{
	if (pIn)
	{
		pIn->clear();
		std::ifstream* pFs = dynamic_cast<std::ifstream*>(pIn);
		if (pFs)
		{
			pFs->close();
		}
		delete pIn;
	}
}

//----< set mode of tokenizing, e.g., code or xml >------------
void Toker::setMode(Toker::mode md)
{
	_mode = md;
	scTok = "()[]{};.\n";
	if (_mode == xml)
		scTok = "<>!" + scTok;
}

//----< set mode of tokenizing, e.g., code or xml >------------
void Toker::setSingleCharTokens(std::string tokChars)
{
	_mode = custom;
	scTok = tokChars;
}

//----< attach tokenizer to a source file or string >----------
bool Toker::attach(const std::string& name, bool srcIsFile)
{
	if (pIn && srcIsFile)
	{
		pIn->clear();
		std::ifstream* pFs = dynamic_cast<std::ifstream*>(pIn);
		if (pFs)
		{
			pFs->close();
		}
		delete pIn;
	}
	if (srcIsFile)
		pIn = new std::ifstream(name.c_str());
	else
		pIn = new std::istringstream(name.c_str());
	return pIn->good();
}

//----< peek function that works with multiple putbacks >------
int Toker::peek()
{
	if (putbacks.size() > 0)
		return putbacks[putbacks.size() - 1];
	else
		return pIn->peek();

}

//----< multiple putBack that won't break peek >---------------
void Toker::putback(int ch)
{
	putbacks.push_back(ch);
	nextChar = ch;
	currChar = prevChar;
	prevChar = prevprevChar;
}

//----< get consistent with peek and putback >-----------------
int Toker::get()
{
	if (putbacks.size() > 0)
	{
		char ch = putbacks.front();
		putbacks.pop_back();
		return ch;
	}
	return pIn->get();
}


//----< extract character from attached stream >---------------
bool Toker::getChar()
{
	char oldNext = nextChar;
	prevprevChar = prevChar;
	prevChar = currChar;
	currChar = this->get();
	nextChar = this->peek();
	_ASSERT(currChar == oldNext || oldNext == 0);
	if (currChar == '\n')
		++numLines;
	if (currChar == '{' && _state == default_state)
		++braceCount;
	if (currChar == '}' && _state == default_state)
		--braceCount;
	return !pIn->eof();
}

//----< is this char a single char token? >--------------------
bool Toker::isSingleCharTok(char ch)
{
	if (scTok.find(ch) < scTok.length())
		return true;
	return false;
}

//----< remove contiguous white space except for newline >-----
void Toker::stripWhiteSpace()
{
	if (nextChar == '\n')
		return;
	while (isspace(nextChar) && nextChar != '\n')
	{
		getChar();
	}
}

//----< is this an indentifier character? >--------------------
bool Toker::isIdentifierChar(char ch)
{
	if (isalpha(ch) || ch == '_' || isdigit(ch))
		return true;
	return false;
}

//----< is this the end of a token? >--------------------------
bool Toker::isTokEnd()
{
	if (isspace(nextChar))
		return true;
	if (isSingleCharTok(nextChar) || isSingleCharTok(currChar))
		return true;
	if (isIdentifierChar(currChar) && !isIdentifierChar(nextChar))
		return true;
	if (!isIdentifierChar(currChar) && isIdentifierChar(nextChar))
		return true;
	if (isFileEnd())
		return true;
	return false;
}

//----< is this the beginning of a comment? >------------------
bool Toker::isBeginComment()
{
	if (prevChar != '\\' && currChar == '/' && nextChar == '*')
	{
		aCppComment = false;
		return true;
	}
	if (prevChar != '\\' && currChar == '/' && nextChar == '/')
	{
		aCppComment = true;
		return true;
	}
	return false;
}

//----< is this the end of a comment? >------------------------
bool Toker::isEndComment()
{
	if (aCppComment && currChar != '\\' && nextChar == '\n')
		return true;
	if (!aCppComment && prevChar != '\\' && currChar == '*' && nextChar == '/')
		return true;
	return false;
}

//----< return comment as a token >----------------------------
std::string Toker::eatComment()
{
	_state = comment_state;
	std::string tok(1, currChar);
	while (!isEndComment() && pIn->good())
	{
		getChar();
		tok.append(1, currChar);
	}

	if (!aCppComment)
	{
		getChar();
		tok.append(1, currChar);
	}
	_state = default_state;
	return tok;
}

//----< is this the beginning of a quote? >--------------------
bool Toker::isBeginQuote()
{
	if (prevChar != '\\' && currChar == '\'')
	{
		aSingleQuote = true;
		return true;
	}
	if (prevChar != '\\' && currChar == '\"')
	{
		aSingleQuote = false;
		return true;
	}
	return false;
}

//----< is this the end of quote? >----------------------------
bool Toker::isEndQuote()
{
	if (prevChar == '\\' || currChar != '\\')
	{
		if (aSingleQuote && nextChar == '\'')
			return true;
		if (!aSingleQuote && nextChar == '\"')
			return true;
	}
	return false;
}

//----< return single or double quote as token >---------------
std::string Toker::eatQuote()
{
	_state = quote_state;
	std::string tok(1, currChar);
	while (!isEndQuote())
	{
		if (!pIn->good())
			throw std::exception("missing end of quote");
		getChar();
		tok.append(1, currChar);
	}
	getChar();
	tok.append(1, currChar);
	_state = default_state;
	return tok;
}

//----< read token from attached file >------------------------
std::string Toker::getTok()
{
	std::string tok = "";
	stripWhiteSpace();
	if (isSingleCharTok(nextChar))
	{
		getChar();
		tok.append(1, currChar);
		return tok;
	}
	do
	{
		if (isFileEnd())
			return tok;

		getChar();
		if (isBeginComment())
		{
			if (tok.length() > 0)
			{
				this->putback(currChar);
				return tok;
			}
			tok = eatComment();
			if (doReturnComments)
				return tok;
			else
			{
				tok = "";
				continue;
			}
		}
		if (isBeginQuote())
		{
			if (tok.length() > 0)
			{
				this->putback(currChar);
				return tok;
			}
			tok = eatQuote();
			return tok;
		}
		if (!isspace(currChar))
			tok.append(1, currChar);
	} while (!isTokEnd() || tok.length() == 0);
	return tok;
}

//----< test stub >--------------------------------------------
#ifdef TEST_TOKENIZER

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing Tokenizer class\n " << std::string(25, '=') << std::endl;
	std::cout << "\n  Note that comments and quotes are returned as single tokens\n\n";

	Toker t_fromStr("tokens from a string: -> int x; /* a comment */", false);
	std::string tok;
	do
	{
		tok = t_fromStr.getTok();
		std::cout << "  " << tok;
	} while (tok != "");
	std::cout << "\n\n";
	// collecting tokens from files, named on the command line
	if (argc < 2){
		std::cout << "\n  please enter name of file to process on command line\n\n";
		return 1;
	}
	for (int i = 1; i<argc; ++i)
	{
		std::cout << "\n  Processing file " << argv[i];
		std::cout << "\n  " << std::string(16 + strlen(argv[i]), '-') << "\n";
		try
		{
			Toker t;
			t.setMode(Toker::xml);        // comment out to show tokenizing for code  //    t.setSingleCharTokens("<>");  // will return same results as above for XML
			if (!t.attach(argv[i])){
				std::cout << "\n    can't open file " << argv[i] << "\n\n";
				continue;
			}
			t.returnComments();  // remove this statement to discard comment tokens
			std::string temp;
			do
			{
				temp = t.getTok();
				std::cout << "  ln: " << t.lines() << ", br lev: "
					<< t.braceLevel() << ", tok size: " << std::setw(3) << temp.length() << " -- ";
				if (temp != "\n")
					std::cout << temp << std::endl;
				else
					std::cout << "newline\n";
			} while (temp != "");
		}
		catch (std::exception& ex){
			std::cout << "  " << ex.what() << "\n\n";
		}
	}
}

#endif