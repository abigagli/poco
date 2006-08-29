//
// ConfigurationViewTest.cpp
//
// $Id: //poco/1.2/Util/testsuite/src/ConfigurationViewTest.cpp#1 $
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


#include "ConfigurationViewTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/MapConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include <algorithm>


using Poco::Util::AbstractConfiguration;
using Poco::Util::MapConfiguration;
using Poco::AutoPtr;


ConfigurationViewTest::ConfigurationViewTest(const std::string& name): CppUnit::TestCase(name)
{
}


ConfigurationViewTest::~ConfigurationViewTest()
{
}


void ConfigurationViewTest::testView()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();
	AutoPtr<AbstractConfiguration> pView = pConf->createView("");
	assert (pView->hasProperty("prop1"));
	assert (pView->hasProperty("prop2"));

	AbstractConfiguration::Keys keys;
	pView->keys(keys);
	assert (keys.size() == 4);
	assert (std::find(keys.begin(), keys.end(), "prop1") != keys.end());
	assert (std::find(keys.begin(), keys.end(), "prop2") != keys.end());
	assert (std::find(keys.begin(), keys.end(), "prop3") != keys.end());
	assert (std::find(keys.begin(), keys.end(), "prop4") != keys.end());
	
	assert (pView->getString("prop1") == "foo");
	assert (pView->getString("prop3.string1") == "foo");
	
	pView->setString("prop5", "foobar");
	assert (pConf->getString("prop5") == "foobar");
	
	pView = pConf->createView("prop1");
	pView->keys(keys);
	assert (keys.empty());
	assert (pView->hasProperty("prop1"));

	pView->setString("prop11", "foobar");
	assert (pConf->getString("prop1.prop11") == "foobar");

	pView = pConf->createView("prop3");
	pView->keys(keys);
	assert (keys.size() == 2);
	assert (std::find(keys.begin(), keys.end(), "string1") != keys.end());
	assert (std::find(keys.begin(), keys.end(), "string2") != keys.end());
	
	assert (pView->getString("string1") == "foo");
	assert (pView->getString("string2") == "bar");

	pView->setString("string3", "foobar");
	assert (pConf->getString("prop3.string3") == "foobar");

	pView = pConf->createView("prop4");
	pView->keys(keys);
	assert (keys.size() == 2);
	assert (std::find(keys.begin(), keys.end(), "prop41") != keys.end());
	assert (std::find(keys.begin(), keys.end(), "prop42") != keys.end());
	
	assert (pView->getString("prop41.string1") == "FOO");
	assert (pView->getString("prop42.string2") == "Bar");

	pView = pConf->createView("prop4.prop41");
	pView->keys(keys);
	assert (keys.size() == 2);
	assert (std::find(keys.begin(), keys.end(), "string1") != keys.end());
	assert (std::find(keys.begin(), keys.end(), "string2") != keys.end());
	
	assert (pView->getString("string1") == "FOO");
	assert (pView->getString("string2") == "BAR");
	
	pView->setString("string3", "foobar");
	assert (pConf->getString("prop4.prop41.string3") == "foobar");
}


AbstractConfiguration* ConfigurationViewTest::createConfiguration() const
{
	AbstractConfiguration* pConfig = new MapConfiguration;
	
	pConfig->setString("prop1", "foo");
	pConfig->setString("prop2", "bar");
	pConfig->setString("prop3.string1", "foo");
	pConfig->setString("prop3.string2", "bar");
	pConfig->setString("prop4.prop41.string1", "FOO");
	pConfig->setString("prop4.prop41.string2", "BAR");
	pConfig->setString("prop4.prop42.string1", "Foo");
	pConfig->setString("prop4.prop42.string2", "Bar");
	
	return pConfig;
}


void ConfigurationViewTest::setUp()
{
}


void ConfigurationViewTest::tearDown()
{
}


CppUnit::Test* ConfigurationViewTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ConfigurationViewTest");

	CppUnit_addTest(pSuite, ConfigurationViewTest, testView);

	return pSuite;
}
