//
// GlobTest.cpp
//
// $Id: //poco/1.2/Foundation/testsuite/src/GlobTest.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "GlobTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Glob.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include <fstream>


using Poco::Glob;
using Poco::File;
using Poco::Path;


GlobTest::GlobTest(const std::string& name): CppUnit::TestCase(name)
{
}


GlobTest::~GlobTest()
{
}


void GlobTest::testMatchChars()
{
	Glob g1("a");
	assert (g1.match("a"));
	assert (!g1.match("b"));
	assert (!g1.match("aa"));
	assert (!g1.match(""));
	
	Glob g2("ab");
	assert (g2.match("ab"));
	assert (!g2.match("aab"));
	assert (!g2.match("abab"));
}


void GlobTest::testMatchQM()
{
	Glob g1("?");
	assert (g1.match("a"));
	assert (g1.match("b"));
	assert (!g1.match("aa"));
	assert (g1.match("."));
	
	Glob g2("\\?");
	assert (g2.match("?"));
	assert (!g2.match("a"));
	assert (!g2.match("ab"));
	
	Glob g3("a?");
	assert (g3.match("aa"));
	assert (g3.match("az"));
	assert (!g3.match("a"));
	assert (!g3.match("aaa"));
	
	Glob g4("??");
	assert (g4.match("aa"));
	assert (g4.match("ab"));
	assert (!g4.match("a"));
	assert (!g4.match("abc"));
	
	Glob g5("?a?");
	assert (g5.match("aaa"));
	assert (g5.match("bac"));
	assert (!g5.match("bbc"));
	assert (!g5.match("ba"));
	assert (!g5.match("ab"));

	Glob g6("a\\?");
	assert (g6.match("a?"));
	assert (!g6.match("az"));
	assert (!g6.match("a"));
	
	Glob g7("?", Glob::GLOB_DOT_SPECIAL);
	assert (g7.match("a"));
	assert (g7.match("b"));
	assert (!g7.match("aa"));
	assert (!g7.match("."));
}


void GlobTest::testMatchAsterisk()
{
	Glob g1("*");
	assert (g1.match(""));
	assert (g1.match("a"));
	assert (g1.match("ab"));
	assert (g1.match("abc"));
	assert (g1.match("."));
	
	Glob g2("a*");
	assert (g2.match("a"));
	assert (g2.match("aa"));
	assert (g2.match("abc"));
	assert (!g2.match("b"));
	assert (!g2.match("ba"));

	Glob g3("ab*");
	assert (g3.match("ab"));
	assert (g3.match("abc"));
	assert (g3.match("abab"));
	assert (!g3.match("ac"));
	assert (!g3.match("baab"));
	
	Glob g4("*a");
	assert (g4.match("a"));
	assert (g4.match("ba"));
	assert (g4.match("aa"));
	assert (g4.match("aaaaaa"));
	assert (g4.match("bbbbba"));
	assert (!g4.match("b"));
	assert (!g4.match("ab"));
	assert (!g4.match("aaab"));
	
	Glob g5("a*a");
	assert (g5.match("aa"));
	assert (g5.match("aba"));
	assert (g5.match("abba"));
	assert (!g5.match("aab"));
	assert (!g5.match("aaab"));
	assert (!g5.match("baaaa"));
	
	Glob g6("a*b*c");
	assert (g6.match("abc"));
	assert (g6.match("aabbcc"));
	assert (g6.match("abcbbc"));
	assert (g6.match("aaaabbbbcccc"));
	assert (!g6.match("aaaabbbcb"));
	
	Glob g7("a*b*");
	assert (g7.match("aaabbb"));
	assert (g7.match("abababab"));
	assert (g7.match("ab"));
	assert (g7.match("aaaaab"));
	assert (!g7.match("a"));
	assert (!g7.match("aa"));
	assert (!g7.match("aaa"));

	Glob g8("**");
	assert (g1.match(""));
	assert (g1.match("a"));
	assert (g1.match("ab"));
	assert (g1.match("abc"));
	
	Glob g9("a\\*");
	assert (g9.match("a*"));
	assert (!g9.match("aa"));
	assert (!g9.match("a"));
	
	Glob g10("a*\\*");
	assert (g10.match("a*"));
	assert (g10.match("aaa*"));
	assert (!g10.match("a"));
	assert (!g10.match("aa"));
	
	Glob g11("*", Glob::GLOB_DOT_SPECIAL);
	assert (g11.match(""));
	assert (g11.match("a"));
	assert (g11.match("ab"));
	assert (g11.match("abc"));
	assert (!g11.match("."));
}


void GlobTest::testMatchRange()
{
	Glob g1("[a]");
	assert (g1.match("a"));
	assert (!g1.match("b"));
	assert (!g1.match("aa"));
	
	Glob g2("[ab]");
	assert (g2.match("a"));
	assert (g2.match("b"));
	assert (!g2.match("c"));
	assert (!g2.match("ab"));
	
	Glob g3("[abc]");
	assert (g3.match("a"));
	assert (g3.match("b"));
	assert (g3.match("c"));
	assert (!g3.match("ab"));
	
	Glob g4("[a-z]");
	assert (g4.match("a"));
	assert (g4.match("z"));
	assert (!g4.match("A"));
	
	Glob g5("[!a]");
	assert (g5.match("b"));
	assert (g5.match("c"));
	assert (!g5.match("a"));
	assert (!g5.match("bb"));
	
	Glob g6("[!a-z]");
	assert (g6.match("A"));
	assert (!g6.match("a"));
	assert (!g6.match("z"));
	
	Glob g7("[0-9a-zA-Z_]");
	assert (g7.match("0"));
	assert (g7.match("1"));
	assert (g7.match("8"));
	assert (g7.match("9"));
	assert (g7.match("a"));
	assert (g7.match("b"));
	assert (g7.match("z"));
	assert (g7.match("A"));
	assert (g7.match("Z"));
	assert (g7.match("_"));
	assert (!g7.match("-"));
	
	Glob g8("[1-3]");
	assert (g8.match("1"));
	assert (g8.match("2"));
	assert (g8.match("3"));
	assert (!g8.match("0"));
	assert (!g8.match("4"));
	
	Glob g9("[!1-3]");
	assert (g9.match("0"));
	assert (g9.match("4"));
	assert (!g9.match("1"));
	assert (!g9.match("2"));
	assert (!g9.match("3"));
	
	Glob g10("[\\!a]");
	assert (g10.match("!"));
	assert (g10.match("a"));
	assert (!g10.match("x"));
	
	Glob g11("[a\\-c]");
	assert (g11.match("a"));
	assert (g11.match("c"));
	assert (g11.match("-"));
	assert (!g11.match("b"));
	
	Glob g12("[\\]]");
	assert (g12.match("]"));
	assert (!g12.match("["));
	
	Glob g13("[[\\]]");
	assert (g13.match("["));
	assert (g13.match("]"));
	assert (!g13.match("x"));
	
	Glob g14("\\[]");
	assert (g14.match("[]"));
	assert (!g14.match("[["));
	
	Glob g15("a[bc]");
	assert (g15.match("ab"));
	assert (g15.match("ac"));
	assert (!g15.match("a"));
	assert (!g15.match("aa"));
	
	Glob g16("[ab]c");
	assert (g16.match("ac"));
	assert (g16.match("bc"));
	assert (!g16.match("a"));
	assert (!g16.match("b"));
	assert (!g16.match("c"));
	assert (!g16.match("aa"));
}


void GlobTest::testMisc()
{
	Glob g1("*.cpp");
	assert (g1.match("Glob.cpp"));
	assert (!g1.match("Glob.h"));
	
	Glob g2("*.[hc]");
	assert (g2.match("foo.c"));
	assert (g2.match("foo.h"));
	assert (!g2.match("foo.i"));
	
	Glob g3("*.*");
	assert (g3.match("foo.cpp"));
	assert (g3.match("foo.h"));
	assert (g3.match("foo."));
	assert (!g3.match("foo"));
	
	Glob g4("File*.?pp");
	assert (g4.match("File.hpp"));
	assert (g4.match("File.cpp"));
	assert (g4.match("Filesystem.hpp"));
	assert (!g4.match("File.h"));
	
	Glob g5("File*.[ch]*");
	assert (g5.match("File.hpp"));
	assert (g5.match("File.cpp"));
	assert (g5.match("Filesystem.hpp"));
	assert (g5.match("File.h"));
	assert (g5.match("Filesystem.cp"));
}


void GlobTest::testGlob()
{
	createFile("globtest/Makefile");
	createFile("globtest/.hidden");
	createFile("globtest/include/one.h");
	createFile("globtest/include/two.h");
	createFile("globtest/src/one.c");
	createFile("globtest/src/two.c");
	createFile("globtest/src/main.c");
	createFile("globtest/testsuite/src/test.h");
	createFile("globtest/testsuite/src/test.c");
	createFile("globtest/testsuite/src/main.c");
	
	std::set<std::string> files;
	Glob::glob("globtest/*", files);
	translatePaths(files);
	assert (files.size() == 5);
	assert (files.find("globtest/Makefile") != files.end());
	assert (files.find("globtest/.hidden") != files.end());
	assert (files.find("globtest/include/") != files.end());
	assert (files.find("globtest/src/") != files.end());
	assert (files.find("globtest/testsuite/") != files.end());
	
	files.clear();
	Glob::glob("globtest/*/*.[hc]", files);
	translatePaths(files);
	assert (files.size() == 5);
	assert (files.find("globtest/include/one.h") != files.end());
	assert (files.find("globtest/include/two.h") != files.end());
	assert (files.find("globtest/src/one.c") != files.end());
	assert (files.find("globtest/src/one.c") != files.end());
	assert (files.find("globtest/src/main.c") != files.end());
	
	files.clear();
	Glob::glob("gl?bt?st/*/*/*.c", files);
	translatePaths(files);
	assert (files.size() == 2);
	assert (files.find("globtest/testsuite/src/test.c") != files.end());
	assert (files.find("globtest/testsuite/src/main.c") != files.end());
	
	files.clear();
	Glob::glob("globtest/*/src/*", files);
	translatePaths(files);
	assert (files.size() == 3);
	assert (files.find("globtest/testsuite/src/test.h") != files.end());
	assert (files.find("globtest/testsuite/src/test.c") != files.end());
	assert (files.find("globtest/testsuite/src/main.c") != files.end());
	
	files.clear();
	Glob::glob("globtest/*/", files);
	translatePaths(files);
	assert (files.size() == 3);
	assert (files.find("globtest/include/") != files.end());
	assert (files.find("globtest/src/") != files.end());
	assert (files.find("globtest/testsuite/") != files.end());

	files.clear();
	Glob::glob("../*/globtest/*/", files);
	translatePaths(files);
	assert (files.size() == 3);
	
	File dir("globtest");
	dir.remove(true);
}


void GlobTest::createFile(const std::string& path)
{
	Path p(path, Path::PATH_UNIX);
	File dir(p.parent());
	dir.createDirectories();
	std::ofstream ostr(path.c_str());
	ostr << path << std::endl;
}


void GlobTest::translatePaths(std::set<std::string>& paths)
{
	std::set<std::string> translated;
	for (std::set<std::string>::const_iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Path p(*it);
		std::string tp(p.toString(Path::PATH_UNIX));
		translated.insert(tp);
	}
	paths = translated;
}


void GlobTest::setUp()
{
}


void GlobTest::tearDown()
{
}


CppUnit::Test* GlobTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("GlobTest");

	CppUnit_addTest(pSuite, GlobTest, testMatchChars);
	CppUnit_addTest(pSuite, GlobTest, testMatchQM);
	CppUnit_addTest(pSuite, GlobTest, testMatchAsterisk);
	CppUnit_addTest(pSuite, GlobTest, testMatchRange);
	CppUnit_addTest(pSuite, GlobTest, testMisc);
	CppUnit_addTest(pSuite, GlobTest, testGlob);

	return pSuite;
}
