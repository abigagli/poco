//
// ZLibTest.cpp
//
// $Id: //poco/1.2/Foundation/testsuite/src/ZLibTest.cpp#1 $
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


#include "ZLibTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/InflatingStream.h"
#include "Poco/DeflatingStream.h"
#include "Poco/StreamCopier.h"
#include <sstream>


using Poco::InflatingInputStream;
using Poco::InflatingOutputStream;
using Poco::DeflatingOutputStream;
using Poco::DeflatingInputStream;
using Poco::InflatingStreamBuf;
using Poco::DeflatingStreamBuf;
using Poco::StreamCopier;


ZLibTest::ZLibTest(const std::string& name): CppUnit::TestCase(name)
{
}


ZLibTest::~ZLibTest()
{
}


void ZLibTest::testDeflate1()
{
	std::stringstream buffer;
	DeflatingOutputStream deflater(buffer);
	deflater << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater.close();
	InflatingInputStream inflater(buffer);
	std::string data;
	inflater >> data;
	assert (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
	inflater >> data;
	assert (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
}


void ZLibTest::testDeflate2()
{
	std::stringstream buffer;
	DeflatingOutputStream deflater(buffer);
	deflater << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater.close();
	std::stringstream buffer2;
	InflatingOutputStream inflater(buffer2);
	StreamCopier::copyStream(buffer, inflater);
	inflater.close();
	std::string data;
	buffer2 >> data;
	assert (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
	buffer2 >> data;
	assert (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
}


void ZLibTest::testDeflate3()
{
	std::stringstream buffer;
	buffer << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	buffer << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	DeflatingInputStream deflater(buffer);
	std::stringstream buffer2;
	StreamCopier::copyStream(deflater, buffer2);
	std::stringstream buffer3;
	InflatingOutputStream inflater(buffer3);
	StreamCopier::copyStream(buffer2, inflater);
	inflater.close();
	std::string data;
	buffer3 >> data;
	assert (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
	buffer3 >> data;
	assert (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
}


void ZLibTest::testGzip1()
{
	std::stringstream buffer;
	DeflatingOutputStream deflater(buffer, DeflatingStreamBuf::STREAM_GZIP);
	deflater << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater << "abcdefabcdefabcdefabcdefabcdefabcdef" << std::endl;
	deflater.close();
	InflatingInputStream inflater(buffer, InflatingStreamBuf::STREAM_GZIP);
	std::string data;
	inflater >> data;
	assert (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
	inflater >> data;
	assert (data == "abcdefabcdefabcdefabcdefabcdefabcdef");
}


void ZLibTest::testGzip2()
{
	// created with gzip ("Hello, world!"):
	const unsigned char gzdata[] = 
	{
		0x1f, 0x8b, 0x08, 0x08, 0xb0, 0x73, 0xd0, 0x41, 0x00, 0x03, 0x68, 0x77, 0x00, 0xf3, 0x48, 0xcd, 
		0xc9, 0xc9, 0xd7, 0x51, 0x28, 0xcf, 0x2f, 0xca, 0x49, 0x51, 0xe4, 0x02, 0x00, 0x18, 0xa7, 0x55, 
		0x7b, 0x0e, 0x00, 0x00, 0x00, 0x00
	};
	
	std::string gzstr((char*) gzdata, sizeof(gzdata));
	std::istringstream istr(gzstr);
	InflatingInputStream inflater(istr, InflatingStreamBuf::STREAM_GZIP);
	std::string data;
	inflater >> data;
	assert (data == "Hello,");
	inflater >> data;
	assert (data == "world!");	
}


void ZLibTest::setUp()
{
}


void ZLibTest::tearDown()
{
}


CppUnit::Test* ZLibTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ZLibTest");

	CppUnit_addTest(pSuite, ZLibTest, testDeflate1);
	CppUnit_addTest(pSuite, ZLibTest, testDeflate2);
	CppUnit_addTest(pSuite, ZLibTest, testDeflate3);
	CppUnit_addTest(pSuite, ZLibTest, testGzip1);
	CppUnit_addTest(pSuite, ZLibTest, testGzip2);

	return pSuite;
}
