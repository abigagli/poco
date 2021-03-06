//
// Database.cpp
//
// $Id$
//
// Library: MongoDB
// Package: MongoDB
// Module:  Database
//
// Implementation of the Database class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
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

#include "Poco/MongoDB/Database.h"

namespace Poco
{
namespace MongoDB
{


Database::Database( const std::string& db) : _dbname(db)
{
}

Database::~Database()
{
}


double Database::count(Connection& connection, const std::string& collectionName) const
{
	Poco::SharedPtr<Poco::MongoDB::QueryRequest> countRequest = createCountRequest(collectionName);

	Poco::MongoDB::ResponseMessage response;
	connection.sendRequest(*countRequest, response);

	if ( response.documents().size() > 0 )
	{
		Poco::MongoDB::Document::Ptr doc = response.documents()[0];
		return doc->get<double>("n");
	}

	return -1;
}


Document::Ptr Database::getLastErrorDoc(Connection& connection) const
{
	Document::Ptr errorDoc;

	Poco::SharedPtr<Poco::MongoDB::QueryRequest> request = createQueryRequest("$cmd");
	request->setNumberToReturn(1);
	request->query().add("getLastError", 1);

	Poco::MongoDB::ResponseMessage response;
	connection.sendRequest(*request, response);

	if ( response.documents().size() > 0 )
	{
		errorDoc = response.documents()[0];
	}

	return errorDoc;
}

std::string Database::getLastError(Connection& connection) const
{
	Document::Ptr errorDoc = getLastErrorDoc(connection);
	if ( !errorDoc.isNull() && errorDoc->isType<std::string>("err") )
	{
		return errorDoc->get<std::string>("err");
	}

	return "";
}


Poco::SharedPtr<Poco::MongoDB::QueryRequest> Database::createQueryRequest(const std::string& collectionName) const
{
	return new Poco::MongoDB::QueryRequest(_dbname + '.' + collectionName);
}


Poco::SharedPtr<Poco::MongoDB::QueryRequest> Database::createCountRequest(const std::string& collectionName) const
{
	Poco::SharedPtr<Poco::MongoDB::QueryRequest> request = createQueryRequest("$cmd");
	request->setNumberToReturn(1);
	request->query().add("count", collectionName);
	return request;
}


Poco::SharedPtr<Poco::MongoDB::InsertRequest> Database::createInsertRequest(const std::string& collectionName) const
{
	return new Poco::MongoDB::InsertRequest(_dbname + '.' + collectionName);
}


}} // Namespace Poco::MongoDB
