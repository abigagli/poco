//
// BasicEventTest.h
//
// $Id: //poco/1.2/Foundation/testsuite/src/BasicEventTest.h#1 $
//
// Tests for BasicEvent
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#ifndef BasicEventTest_INCLUDED
#define BasicEventTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"
#include "Poco/BasicEvent.h"
#include "Poco/EventArgs.h"


class BasicEventTest: public CppUnit::TestCase
{
	Poco::BasicEvent<int> Simple;
	Poco::BasicEvent<const int> ConstSimple;
	Poco::BasicEvent<Poco::EventArgs*> Complex;
	Poco::BasicEvent<Poco::EventArgs> Complex2;
	Poco::BasicEvent<const Poco::EventArgs*> ConstComplex;
	Poco::BasicEvent<const Poco::EventArgs * const> Const2Complex;
public:
	BasicEventTest(const std::string& name);
	~BasicEventTest();

	void testNoDelegate();
	void testSingleDelegate();
	void testDuplicateRegister ();
	void testDuplicateUnregister ();
	void testDisabling ();
	void testExpire();
	void testExpireReRegister();
	void testReturnParams ();
	void testOverwriteDelegate ();
	void testAsyncNotify ();
	
	void setUp();
	void tearDown();
	static CppUnit::Test* suite();

protected:

	void onSimple ( const void* pSender, int& i );
	void onSimpleOther ( const void* pSender, int& i );
	void onConstSimple ( const void* pSender, const int& i );
	void onComplex ( const void* pSender, Poco::EventArgs* & i );
	void onComplex2 ( const void* pSender, Poco::EventArgs & i );
	void onConstComplex ( const void* pSender, const Poco::EventArgs*& i );
	void onConst2Complex ( const void* pSender, const Poco::EventArgs * const & i );
	void onAsync ( const void* pSender, int& i );

	int getCount () const;
private:
	int		_count;
};


#endif // BasicEventTest_INCLUDED
