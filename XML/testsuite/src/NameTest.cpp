//
// NameTest.cpp
//
// $Id: //poco/1.2/XML/testsuite/src/NameTest.cpp#1 $
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


#include "NameTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/XML/Name.h"


using Poco::XML::Name;


NameTest::NameTest(const std::string& name): CppUnit::TestCase(name)
{
}


NameTest::~NameTest()
{
}


void NameTest::testSplit()
{
	std::string qname = "name";
	std::string prefix;
	std::string local;
	Name::split(qname, prefix, local);
	assert (prefix.empty());
	assert (local == "name");
	
	qname = "p:l";
	Name::split(qname, prefix, local);
	assert (prefix == "p");
	assert (local == "l");
	
	qname = "pre:local";
	Name::split(qname, prefix, local);
	assert (prefix == "pre");
	assert (local == "local");
}


void NameTest::testLocalName()
{
	std::string qname = "name";
	std::string local = Name::localName(qname);
	assert (local == "name");
	qname = "p:l";
	local = Name::localName(qname);
	assert (local == "l");
	qname = "pre:local";
	local = Name::localName(qname);
	assert (local == "local");
}


void NameTest::testPrefix()
{
	std::string qname = "name";
	std::string prefix = Name::prefix(qname);
	assert (prefix.empty());
	qname = "p:l";
	prefix = Name::prefix(qname);
	assert (prefix == "p");
	qname = "pre:local";
	prefix = Name::prefix(qname);
	assert (prefix == "pre");
}


void NameTest::testName()
{
	std::string qname = "name";
	Name name(qname);
	assert (name.qname() == "name");
	assert (name.prefix().empty());
	assert (name.namespaceURI().empty());
	assert (name.localName().empty());

	qname.clear();
	name.assign(qname, "http://www.appinf.com/", "local");
	assert (name.qname().empty());
	assert (name.prefix().empty());
	assert (name.namespaceURI() == "http://www.appinf.com/");
	assert (name.localName() == "local");

	Name name2("pre:local", "http://www.appinf.com/");
	assert (name2.qname() == "pre:local");
	assert (name2.prefix() == "pre");
	assert (name2.namespaceURI() == "http://www.appinf.com/");
	assert (name2.localName() == "local");

	name2.assign("PRE:Local", "http://www.appinf.com/");
	assert (name2.qname() == "PRE:Local");
	assert (name2.prefix() == "PRE");
	assert (name2.namespaceURI() == "http://www.appinf.com/");
	assert (name2.localName() == "Local");
}


void NameTest::testCompare()
{
	Name n1("pre:local");
	Name n2(n1);
	Name n3("pre:local2");
	
	assert (n1.equals(n2));
	assert (!n1.equals(n3));
	
	n1.assign("pre:local", "http://www.appinf.com", "local");
	n2.assign("pre:local", "http://www.appinf.com", "local");
	n3.assign("pre:local2", "http://www.appinf.com", "local2");
	
	assert (n1.equals(n2));
	assert (!n1.equals(n3));
	
	assert (n1.equals("pre:local", "http://www.appinf.com", "local"));
	assert (!n1.equals("pre:local", "", ""));
	assert (n1.equalsWeakly("pre:local", "", ""));
	assert (!n1.equalsWeakly("pre:local2", "", ""));
	assert (!n1.equals("", "http://www.appinf.com", "local"));
	assert (n1.equalsWeakly("", "http://www.appinf.com", "local"));
	assert (!n1.equalsWeakly("", "http://www.appinf.com", "local2"));
}


void NameTest::testSwap()
{
	Name n1("ns:name1", "http://www.appinf.com");
	Name n2("ns:name2", "http://www.foobar.com");
	n1.swap(n2);
	assert (n1.qname() == "ns:name2");
	assert (n1.namespaceURI() == "http://www.foobar.com");
	assert (n1.localName() == "name2");
	assert (n2.qname() == "ns:name1");
	assert (n2.namespaceURI() == "http://www.appinf.com");
	assert (n2.localName() == "name1");
}


void NameTest::setUp()
{
}


void NameTest::tearDown()
{
}


CppUnit::Test* NameTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("NameTest");

	CppUnit_addTest(pSuite, NameTest, testSplit);
	CppUnit_addTest(pSuite, NameTest, testLocalName);
	CppUnit_addTest(pSuite, NameTest, testPrefix);
	CppUnit_addTest(pSuite, NameTest, testName);
	CppUnit_addTest(pSuite, NameTest, testCompare);
	CppUnit_addTest(pSuite, NameTest, testSwap);

	return pSuite;
}
