//
// UUID.cpp
//
// $Id: //poco/1.2/Foundation/src/UUID.cpp#1 $
//
// Library: Foundation
// Package: UUID
// Module:  UUID
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


#include "Poco/UUID.h"
#include "Poco/ByteOrder.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <string.h>


namespace Poco {


UUID::UUID(): 
	_timeLow(0), 
	_timeMid(0),
	_timeHiAndVersion(0),
	_clockSeq(0)
{
	memset(_node, 0, sizeof(_node));
}


UUID::UUID(const UUID& uuid):
	_timeLow(uuid._timeLow), 
	_timeMid(uuid._timeMid),
	_timeHiAndVersion(uuid._timeHiAndVersion),
	_clockSeq(uuid._clockSeq)
{
	memcpy(_node, uuid._node, sizeof(_node));
}


UUID::UUID(const std::string& uuid)
{
	parse(uuid);
}

	
UUID::UUID(const char* uuid)
{
	poco_check_ptr (uuid);
	parse(std::string(uuid));
}


UUID::UUID(UInt32 timeLow, UInt32 timeMid, UInt32 timeHiAndVersion, UInt16 clockSeq, UInt8 node[]):
	_timeLow(timeLow),
	_timeMid(timeMid),
	_timeHiAndVersion(timeHiAndVersion),
	_clockSeq(clockSeq)
{
	memcpy(_node, node, sizeof(_node));
}


UUID::UUID(const char* bytes, Version version)
{
	UInt32 i32;
	UInt16 i16;
	memcpy(&i32, bytes, sizeof(i32));
	_timeLow = ByteOrder::fromNetwork(i32);
	bytes += sizeof(i32);
	memcpy(&i16, bytes, sizeof(i16));
	_timeMid = ByteOrder::fromNetwork(i16);
	bytes += sizeof(i16);
	memcpy(&i16, bytes, sizeof(i16));
	_timeHiAndVersion = ByteOrder::fromNetwork(i16);
	bytes += sizeof(i16);
	memcpy(&i16, bytes, sizeof(i16));
	_clockSeq = ByteOrder::fromNetwork(i16);
	bytes += sizeof(i16);
	memcpy(_node, bytes, sizeof(_node));

	_timeHiAndVersion &= 0x0FFF;
	_timeHiAndVersion |= (version << 12);
	_clockSeq &= 0x3FFF;
	_clockSeq |= 0x8000;
}


UUID::~UUID()
{
}


UUID& UUID::operator = (const UUID& uuid)
{
	if (&uuid != this)
	{
		_timeLow = uuid._timeLow;
		_timeMid = uuid._timeMid;
		_timeHiAndVersion = uuid._timeHiAndVersion;
		_clockSeq         = uuid._clockSeq;
		memcpy(_node, uuid._node, sizeof(_node));
	}
	return *this;
}


void UUID::swap(UUID& uuid)
{
	std::swap(_timeLow, uuid._timeLow);
	std::swap(_timeMid, uuid._timeMid);
	std::swap(_timeHiAndVersion, uuid._timeHiAndVersion);
	std::swap(_clockSeq, uuid._clockSeq);
	std::swap_ranges(_node, _node + 6, uuid._node);
}


void UUID::parse(const std::string& uuid)
{
	UInt32 timeLow = 0;
	UInt16 timeMid = 0;
	UInt16 timeHiAndVersion = 0;
	UInt16 clockSeq = 0;
	UInt8  node[6] = {0, 0, 0, 0, 0, 0};
	std::string::const_iterator it  = uuid.begin();
	std::string::const_iterator end = uuid.end();
	if (it != end)
	{
		for (int i = 0; i < 8; ++i)
		{
			timeLow = timeLow*16 + nibble(*it++);
			if (it == end) throw SyntaxException(uuid);
		}
		if (*it != '-') throw SyntaxException(uuid);
		++it;
		for (int i = 0; i < 4; ++i)
		{
			timeMid = timeMid*16 + nibble(*it++);
			if (it == end) throw SyntaxException(uuid);
		}
		if (*it != '-') throw SyntaxException(uuid);
		++it;
		for (int i = 0; i < 4; ++i)
		{
			timeHiAndVersion = timeHiAndVersion*16 + nibble(*it++);
			if (it == end) throw SyntaxException(uuid);
		}
		if (*it != '-') throw SyntaxException(uuid);
		++it;
		for (int i = 0; i < 4; ++i)
		{
			clockSeq = clockSeq*16 + nibble(*it++);
			if (it == end) throw SyntaxException(uuid);
		}
		if (*it != '-') throw SyntaxException(uuid);
		++it;
		for (int i = 0; i < 6; ++i)
		{
			if (it == end) throw SyntaxException(uuid);
			node[i] = node[i]*16 + nibble(*it++);
			if (it == end) throw SyntaxException(uuid);
			node[i] = node[i]*16 + nibble(*it++);
		}
		_timeLow = timeLow;
		_timeMid = timeMid;
		_timeHiAndVersion = timeHiAndVersion;
		_clockSeq = clockSeq;
		memcpy(_node, node, sizeof(_node));
	}
}


std::string UUID::toString() const
{
	std::string result;
	result.reserve(36);
	appendHex(result, _timeLow);
	result += '-';
	appendHex(result, _timeMid);
	result += '-';
	appendHex(result, _timeHiAndVersion);
	result += '-';
	appendHex(result, _clockSeq);
	result += '-';
	for (int i = 0; i < sizeof(_node); ++i)
		appendHex(result, _node[i]);
	return result;
}


void UUID::copyFrom(const char* buffer)
{
	UInt32 i32;
	UInt16 i16;
	memcpy(&i32, buffer, sizeof(i32));
	_timeLow = ByteOrder::fromNetwork(i32);
	buffer += sizeof(i32);
	memcpy(&i16, buffer, sizeof(i16));
	_timeMid = ByteOrder::fromNetwork(i16);
	buffer += sizeof(i16);
	memcpy(&i16, buffer, sizeof(i16));
	_timeHiAndVersion = ByteOrder::fromNetwork(i16);
	buffer += sizeof(i16);
	memcpy(&i16, buffer, sizeof(i16));
	_clockSeq = ByteOrder::fromNetwork(i16);
	buffer += sizeof(i16);
	memcpy(_node, buffer, sizeof(_node));
}


void UUID::copyTo(char* buffer) const
{
	UInt32 i32 = ByteOrder::toNetwork(_timeLow);
	memcpy(buffer, &i32, sizeof(i32));
	buffer += sizeof(i32);
	UInt16 i16 = ByteOrder::toNetwork(_timeMid);
	memcpy(buffer, &i16, sizeof(i16));
	buffer += sizeof(i16);
	i16 = ByteOrder::toNetwork(_timeHiAndVersion);
	memcpy(buffer, &i16, sizeof(i16));
	buffer += sizeof(i16);
	i16 = ByteOrder::toNetwork(_clockSeq);
	memcpy(buffer, &i16, sizeof(i16));
	buffer += sizeof(i16);
	memcpy(buffer, _node, sizeof(_node));
}


int UUID::variant() const
{
	int v = _clockSeq >> 13;
	if ((v & 6) == 6)
		return v;
	else if (v & 4)
		return 2;
	else
		return 0;
}


int UUID::compare(const UUID& uuid) const
{
	if (_timeLow != uuid._timeLow) return _timeLow < uuid._timeLow ? -1 : 1;
	if (_timeMid != uuid._timeMid) return _timeMid < uuid._timeMid ? -1 : 1;
	if (_timeHiAndVersion != uuid._timeHiAndVersion) return _timeHiAndVersion < uuid._timeHiAndVersion ? -1 : 1;
	if (_clockSeq != uuid._clockSeq) return _clockSeq < uuid._clockSeq ? -1 : 1;
	for (int i = 0; i < sizeof(_node); ++i)
	{
		if (_node[i] < uuid._node[i]) 
			return -1;
		else if (_node[i] > uuid._node[i])
			return 1;	
	}
	return 0;
}


void UUID::appendHex(std::string& str, UInt8 n) 
{
	static const char* digits = "0123456789abcdef";
	str += digits[(n >> 4) & 0xF];
	str += digits[n & 0xF];
}


void UUID::appendHex(std::string& str, UInt16 n)
{
	appendHex(str, UInt8(n >> 8));
	appendHex(str, UInt8(n & 0xFF));
}


void UUID::appendHex(std::string& str, UInt32 n)
{
	appendHex(str, UInt16(n >> 16));
	appendHex(str, UInt16(n & 0xFFFF));
}


int UUID::nibble(char hex)
{
	if (hex >= 'a' && hex <= 'f')
		return hex - 'a' + 10;
	else if (hex >= 'A' && hex <= 'F')
		return hex - 'A' + 10;
	else if (hex >= '0' && hex <= '9')
		return hex - '0';
	else
		return 0;
}


void UUID::fromNetwork()
{
	_timeLow = ByteOrder::fromNetwork(_timeLow);
	_timeMid = ByteOrder::fromNetwork(_timeMid);
	_timeHiAndVersion = ByteOrder::fromNetwork(_timeHiAndVersion);
	_clockSeq = ByteOrder::fromNetwork(_clockSeq);
}


void UUID::toNetwork()
{
	_timeLow = ByteOrder::toNetwork(_timeLow);
	_timeMid = ByteOrder::toNetwork(_timeMid);
	_timeHiAndVersion = ByteOrder::toNetwork(_timeHiAndVersion);
	_clockSeq = ByteOrder::toNetwork(_clockSeq);
}


const UUID& UUID::nil()
{
	static UUID nil;
	return nil;
}


const UUID& UUID::dns()
{
	static UUID uuidDNS("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
	return uuidDNS;
}

	
const UUID& UUID::uri()
{
	static UUID uuidURI("6ba7b811-9dad-11d1-80b4-00c04fd430c8");
	return uuidURI;
}


const UUID& UUID::oid()
{
	static UUID uuidOID("6ba7b812-9dad-11d1-80b4-00c04fd430c8");
	return uuidOID;
}


const UUID& UUID::x500()
{
	static UUID uuidX500("6ba7b814-9dad-11d1-80b4-00c04fd430c8");
	return uuidX500;
}


} // namespace Poco
