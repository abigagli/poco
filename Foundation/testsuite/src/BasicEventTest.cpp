//
// BasicEventTest.cpp
//
// $Id: //poco/1.2/Foundation/testsuite/src/BasicEventTest.cpp#1 $
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


#include "BasicEventTest.h"
#include "DummyDelegate.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Delegate.h"
#include "Poco/Expire.h"
#include "Poco/Thread.h"
#include "Poco/Exception.h"


using namespace Poco;


#define LARGEINC 100


BasicEventTest::BasicEventTest(const std::string& name ): CppUnit::TestCase(name)
{
}


BasicEventTest::~BasicEventTest()
{
}

void BasicEventTest::testNoDelegate()
{
	int tmp = 0;
	EventArgs args;

	poco_assert ( _count == 0 );
	Simple.notify ( this, tmp );
	poco_assert ( _count == 0 );

	Simple += Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple);
	Simple -= Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple);
	Simple.notify ( this, tmp );
	poco_assert ( _count == 0 );
	
	ConstSimple += Delegate < BasicEventTest, const int > (this, &BasicEventTest::onConstSimple);
	ConstSimple -= Delegate < BasicEventTest, const int > (this, &BasicEventTest::onConstSimple);
	ConstSimple.notify ( this, tmp );
	poco_assert ( _count == 0 );
	
	//Note: passing &args will not work due to &
	EventArgs* pArgs = &args;
	Complex += Delegate < BasicEventTest, Poco::EventArgs* > (this, &BasicEventTest::onComplex);
	Complex -= Delegate < BasicEventTest, Poco::EventArgs* > (this, &BasicEventTest::onComplex);
	Complex.notify ( this, pArgs );
	poco_assert ( _count == 0 );

	Complex2 += Delegate < BasicEventTest, Poco::EventArgs > (this, &BasicEventTest::onComplex2);
	Complex2 -= Delegate < BasicEventTest, Poco::EventArgs > (this, &BasicEventTest::onComplex2);
	Complex2.notify ( this, args );
	poco_assert ( _count == 0 );

	const EventArgs* pCArgs = &args;
	ConstComplex += Delegate < BasicEventTest, const Poco::EventArgs* > (this, &BasicEventTest::onConstComplex);
	ConstComplex -= Delegate < BasicEventTest, const Poco::EventArgs* > (this, &BasicEventTest::onConstComplex);
	ConstComplex.notify ( this, pCArgs );
	poco_assert ( _count == 0 );

	Const2Complex += Delegate < BasicEventTest, const Poco::EventArgs* const > (this, &BasicEventTest::onConst2Complex);
	Const2Complex -= Delegate < BasicEventTest, const Poco::EventArgs* const > (this, &BasicEventTest::onConst2Complex);
	Const2Complex.notify ( this, pArgs );
	poco_assert ( _count == 0 );
}

void BasicEventTest::testSingleDelegate()
{
	int tmp = 0;
	EventArgs args;

	poco_assert ( _count == 0 );

	Simple += Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple);
	Simple.notify ( this, tmp );
	poco_assert ( _count == 1 );
	
	ConstSimple += Delegate < BasicEventTest, const int > (this, &BasicEventTest::onConstSimple);
	ConstSimple.notify ( this, tmp );
	poco_assert ( _count == 2 );
	
	EventArgs* pArgs = &args;
	Complex += Delegate < BasicEventTest, Poco::EventArgs* > (this, &BasicEventTest::onComplex);
	Complex.notify ( this, pArgs );
	poco_assert ( _count == 3 );

	Complex2 += Delegate < BasicEventTest, Poco::EventArgs > (this, &BasicEventTest::onComplex2);
	Complex2.notify ( this, args );
	poco_assert ( _count == 4 );

	const EventArgs* pCArgs = &args;
	ConstComplex += Delegate < BasicEventTest, const Poco::EventArgs* > (this, &BasicEventTest::onConstComplex);
	ConstComplex.notify ( this, pCArgs );
	poco_assert ( _count == 5 );

	Const2Complex += Delegate < BasicEventTest, const Poco::EventArgs* const > (this, &BasicEventTest::onConst2Complex);
	Const2Complex.notify ( this, pArgs );
	poco_assert ( _count == 6 );
	// check if 2nd notify also works
	Const2Complex.notify ( this, pArgs );
	poco_assert ( _count == 7 );
	
}

void BasicEventTest::testDuplicateRegister ()
{
	int tmp = 0;
	
	poco_assert ( _count == 0 );

	Simple += Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple);
	Simple += Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple);
	Simple.notify ( this, tmp );
	poco_assert ( _count == 1 );
	Simple -= Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple);
	Simple.notify ( this, tmp );
	poco_assert ( _count == 1 );
}

void BasicEventTest::testDuplicateUnregister ()
{
	// duplicate unregister shouldn't give an error,
	int tmp = 0;
	
	poco_assert ( _count == 0 );

	Simple -= Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple); // should work
	Simple.notify ( this, tmp );
	poco_assert ( _count == 0 );

	Simple += Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple);
	Simple.notify ( this, tmp );
	poco_assert ( _count == 1 );

	Simple -= Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple);
	Simple.notify ( this, tmp );
	poco_assert ( _count == 1 );

	Simple -= Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple);
	Simple.notify ( this, tmp );
	poco_assert ( _count == 1 );
}

void BasicEventTest::testDisabling ()
{
	int tmp = 0;
	
	poco_assert ( _count == 0 );

	Simple += Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple);
	Simple.disable ();
	Simple.notify ( this, tmp );
	poco_assert ( _count == 0 );
	Simple.enable ();
	Simple.notify ( this, tmp );
	poco_assert ( _count == 1 );

	// unregister should also work with disabled event
	Simple.disable ();
	Simple -= Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple);
	Simple.enable ();
	Simple.notify ( this, tmp );
	poco_assert ( _count == 1 );
}

void BasicEventTest::testExpire ()
{
	int tmp = 0;
	
	poco_assert ( _count == 0 );

	Simple += Expire < int > (Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple), 500 );
	Simple.notify ( this, tmp );
	poco_assert ( _count == 1 );
	Poco::Thread::sleep ( 700 );
	Simple.notify ( this, tmp );
	poco_assert ( _count == 1 );
}

void BasicEventTest::testExpireReRegister()
{
	int tmp = 0;
	
	poco_assert ( _count == 0 );

	Simple += Expire < int > (Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple), 500 );
	Simple.notify ( this, tmp );
	poco_assert ( _count == 1 );
	Poco::Thread::sleep ( 200 );
	Simple.notify ( this, tmp );
	poco_assert ( _count == 2 );
	// renew registration
	Simple += Expire < int > (Delegate < BasicEventTest, int > (this, &BasicEventTest::onSimple), 600 );
	Poco::Thread::sleep( 400 );
	Simple.notify ( this, tmp );
	poco_assert ( _count == 3 );
	Poco::Thread::sleep( 300 );
	Simple.notify ( this, tmp );
	poco_assert ( _count == 3 );
}

void BasicEventTest::testReturnParams ()
{
	DummyDelegate o1;
	Simple += Delegate < DummyDelegate, int > (&o1, &DummyDelegate::onSimple);

	int tmp = 0;
	Simple.notify ( this, tmp );
	poco_assert ( tmp == 1 );
}

void BasicEventTest::testOverwriteDelegate ()
{
	DummyDelegate o1;
	Simple += Delegate < DummyDelegate, int > (&o1, &DummyDelegate::onSimple2);
	// o1 can only have one entry, thus the next line will replace the entry
	Simple += Delegate < DummyDelegate, int > (&o1, &DummyDelegate::onSimple);

	int tmp = 0; // onsimple requires 0 as input
	Simple.notify ( this, tmp );
	poco_assert ( tmp == 1 );
	// now overwrite with onsimple2 with requires as input tmp = 1
	Simple += Expire < int > ( Delegate < DummyDelegate, int > (&o1, &DummyDelegate::onSimple2), 23000);
	Simple.notify ( this, tmp );
	poco_assert ( tmp == 2 );
}

void BasicEventTest::testAsyncNotify ()
{
	Poco::BasicEvent < int >* pSimple= new Poco::BasicEvent < int >();
	(*pSimple) += Delegate < BasicEventTest, int > (this, &BasicEventTest::onAsync);
	poco_assert ( _count == 0 );
	int tmp = 0;
	Poco::ActiveResult < int > retArg = pSimple->notifyAsync ( this, tmp );
	delete pSimple; // must work even when the event got deleted!
	pSimple = NULL;
	poco_assert ( _count == 0 );
	retArg.wait ();
	poco_assert ( retArg.data() == tmp );
	poco_assert ( _count == LARGEINC );
}

void BasicEventTest::onSimple ( const void* pSender, int& i )
{
	_count++;
}

void BasicEventTest::onSimpleOther ( const void* pSender, int& i )
{
	_count+=100;
}

void BasicEventTest::onConstSimple ( const void* pSender, const int& i )
{
	_count++;
}

void BasicEventTest::onComplex ( const void* pSender, Poco::EventArgs* & i )
{
	_count++;
}

void BasicEventTest::onComplex2 ( const void* pSender, Poco::EventArgs & i )
{
	_count++;
}

void BasicEventTest::onConstComplex ( const void* pSender, const Poco::EventArgs*& i )
{
	_count++;
}

void BasicEventTest::onConst2Complex ( const void* pSender, const Poco::EventArgs * const & i )
{
	_count++;
}

void BasicEventTest::onAsync ( const void* pSender, int& i )
{
	Poco::Thread::sleep ( 700 );
	_count += LARGEINC ;
}

int BasicEventTest::getCount () const
{
	return _count;
}

void BasicEventTest::setUp()
{
	_count = 0;
	// must clear events, otherwise repeating test executions will fail
	// because tests are only created once, only setup is called before 
	// each test run
	Simple.clear ();
	ConstSimple.clear ();
	Complex.clear ();
	Complex2.clear ();
	ConstComplex.clear ();
	Const2Complex.clear ();
}


void BasicEventTest::tearDown()
{
}


CppUnit::Test* BasicEventTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("BasicEventTest");

	CppUnit_addTest(pSuite, BasicEventTest, testNoDelegate);
	CppUnit_addTest(pSuite, BasicEventTest, testSingleDelegate);
	CppUnit_addTest(pSuite, BasicEventTest, testReturnParams);
	CppUnit_addTest(pSuite, BasicEventTest, testDuplicateRegister);
	CppUnit_addTest(pSuite, BasicEventTest, testDuplicateUnregister);
	CppUnit_addTest(pSuite, BasicEventTest, testDisabling);
	CppUnit_addTest(pSuite, BasicEventTest, testExpire);
	CppUnit_addTest(pSuite, BasicEventTest, testExpireReRegister);
	CppUnit_addTest(pSuite, BasicEventTest, testOverwriteDelegate);
	CppUnit_addTest(pSuite, BasicEventTest, testAsyncNotify);
	return pSuite;
}
