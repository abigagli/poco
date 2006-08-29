//
// TextIteratorTest.cpp
//
// $Id: //poco/1.2/Foundation/testsuite/src/TextIteratorTest.cpp#1 $
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


#include "TextIteratorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/TextIterator.h"
#include "Poco/Latin1Encoding.h"
#include "Poco/UTF8Encoding.h"


using Poco::TextIterator;
using Poco::Latin1Encoding;
using Poco::UTF8Encoding;


TextIteratorTest::TextIteratorTest(const std::string& name): CppUnit::TestCase(name)
{
}


TextIteratorTest::~TextIteratorTest()
{
}


void TextIteratorTest::testEmptyLatin1()
{
	Latin1Encoding encoding;
	std::string text;
	TextIterator it(text, encoding);
	TextIterator end(text);
	
	assert (it == end);
}


void TextIteratorTest::testOneLatin1()
{
	Latin1Encoding encoding;
	std::string text("x");
	TextIterator it(text, encoding);
	TextIterator end(text);
	
	assert (it != end);
	assert (*it == 'x');
	++it;
	assert (it == end);
}


void TextIteratorTest::testLatin1()
{
	Latin1Encoding encoding;
	std::string text("Latin1");
	TextIterator it(text, encoding);
	TextIterator end(text);
	
	assert (it != end);
	assert (*it++ == 'L');
	assert (it != end);
	assert (*it++ == 'a');
	assert (it != end);
	assert (*it++ == 't');
	assert (it != end);
	assert (*it++ == 'i');
	assert (it != end);
	assert (*it++ == 'n');
	assert (it != end);
	assert (*it++ == '1');
	assert (it == end);
	
	std::string empty;
	it  = TextIterator(empty, encoding);
	end = TextIterator(empty);
	assert (it == end);
}


void TextIteratorTest::testEmptyUTF8()
{
	UTF8Encoding encoding;
	std::string text;
	TextIterator it(text, encoding);
	TextIterator end(text);
	
	assert (it == end);
}


void TextIteratorTest::testOneUTF8()
{
	UTF8Encoding encoding;
	
	// 1 byte sequence
	std::string text("x");
	TextIterator it(text, encoding);
	TextIterator end(text);
	
	assert (it != end);
	assert (*it == 'x');
	++it;
	assert (it == end);
	
	unsigned char data[Poco::TextEncoding::MAX_SEQUENCE_LENGTH];
	
	// 2 byte sequence
	int n = encoding.convert(0xab, data, sizeof(data));
	assert (n == 2);
	text.assign((char*) data, n);
	it  = TextIterator(text, encoding);
	end = TextIterator(text);
	
	assert (it != end);
	assert (*it++ == 0xab);
	assert (it == end);

	// 3 byte sequence
	n = encoding.convert(0xabcd, data, sizeof(data));
	assert (n == 3);
	text.assign((char*) data, n);
	it  = TextIterator(text, encoding);
	end = TextIterator(text);
	
	assert (it != end);
	assert (*it++ == 0xabcd);
	assert (it == end);

	// 4 byte sequence
	n = encoding.convert(0xabcde, data, sizeof(data));
	assert (n == 4);
	text.assign((char*) data, n);
	it  = TextIterator(text, encoding);
	end = TextIterator(text);
	
	assert (it != end);
	assert (*it++ == 0xabcde);
	assert (it == end);
	
	// 5 byte sequence
	n = encoding.convert(0xabcdef, data, sizeof(data));
	assert (n == 5);
	text.assign((char*) data, n);
	it  = TextIterator(text, encoding);
	end = TextIterator(text);
	
	assert (it != end);
	assert (*it++ == 0xabcdef);
	assert (it == end);

	// 6 byte sequence
	n = encoding.convert(0xfabcdef, data, sizeof(data));
	assert (n == 6);
	text.assign((char*) data, n);
	it  = TextIterator(text, encoding);
	end = TextIterator(text);
	
	assert (it != end);
	assert (*it++ == 0xfabcdef);
	assert (it == end);
}


void TextIteratorTest::testUTF8()
{
	UTF8Encoding encoding;
	const unsigned char greek[] = {0x20, 0xce, 0xba, 0xe1, 0xbd, 0xb9, 0xcf, 0x83, 0xce, 0xbc, 0xce, 0xb5, 0x20, 0x00};
	std::string text((const char*) greek);
	TextIterator it(text, encoding);
	TextIterator end(text);
	
	assert (it != end);
	assert (*it++ == 0x0020);
	assert (it != end);
	assert (*it++ == 0x03ba);
	assert (it != end);
	assert (*it++ == 0x1f79);
	assert (it != end);
	assert (*it++ == 0x03c3);
	assert (it != end);
	assert (*it++ == 0x03bc);
	assert (it != end);
	assert (*it++ == 0x03b5);
	assert (it != end);
	assert (*it++ == 0x0020);
	assert (it == end);
}


void TextIteratorTest::testSwap()
{
	Latin1Encoding encoding;
	std::string text("x");
	TextIterator it1(text, encoding);
	TextIterator it2(text, encoding);
	TextIterator end(text);
	
	assert (it1 == it2);
	it2.swap(end);
	assert (it1 != it2);
	it2.swap(end);
	assert (it1 == it2);
}


void TextIteratorTest::setUp()
{
}


void TextIteratorTest::tearDown()
{
}


CppUnit::Test* TextIteratorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TextIteratorTest");

	CppUnit_addTest(pSuite, TextIteratorTest, testEmptyLatin1);
	CppUnit_addTest(pSuite, TextIteratorTest, testOneLatin1);
	CppUnit_addTest(pSuite, TextIteratorTest, testLatin1);
	CppUnit_addTest(pSuite, TextIteratorTest, testEmptyUTF8);
	CppUnit_addTest(pSuite, TextIteratorTest, testOneUTF8);
	CppUnit_addTest(pSuite, TextIteratorTest, testUTF8);
	CppUnit_addTest(pSuite, TextIteratorTest, testSwap);

	return pSuite;
}
