//
// ICMPClient.h
//
// $Id: //poco/1.2/Net/include/Poco/Net/ICMPClient.h#1 $
//
// Library: Net
// Package: ICMP
// Module:  ICMPClient
//
// Definition of the ICMPClient class.
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


#ifndef Net_ICMPClient_INCLUDED
#define Net_ICMPClient_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/ICMPSocket.h"
#include "Poco/Net/ICMPEventArgs.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/BasicEvent.h"


namespace Poco {
namespace Net {


class Net_API ICMPClient
	/// This class provides ICMP Ping functionality.
	///
	/// The events are available when class is instantiated
	///	and non-static member functions are called.
	/// 
	///	A "lightweight" alternative is direct (without instantiation)
	///	use of static member functions.
{
public:
	mutable Poco::BasicEvent<ICMPEventArgs> pingBegin;
	mutable Poco::BasicEvent<ICMPEventArgs> pingReply;
	mutable Poco::BasicEvent<ICMPEventArgs> pingError;
	mutable Poco::BasicEvent<ICMPEventArgs> pingEnd;

	explicit ICMPClient(IPAddress::Family family);
		/// Creates an ICMP client.

	~ICMPClient();
		/// Destroys the ICMP client.

	int ping(SocketAddress& address, int repeat = 1) const;
		/// Pings the specified address [repeat] times.
		/// Notifications are posted for events.
		/// 
		/// Returns the number of valid replies.

	int ping(const std::string& address, int repeat = 1) const;
		/// Calls ICMPClient::ping(SocketAddress&, int) and
		/// returns the result.
		/// 
		/// Returns the number of valid replies.

	static int ping(SocketAddress& address, IPAddress::Family family, int repeat = 1);
		/// Pings the specified address [repeat] times.
		/// Notifications are not posted for events.
		/// 
		/// Returns the number of valid replies.

	static int pingIPv4(const std::string& address, int repeat = 1);
		/// Calls ICMPClient::ping(SocketAddress&, int) and
		/// returns the result.
		/// 
		/// Returns the number of valid replies.

private:
	mutable IPAddress::Family _family;
};


} } // namespace Poco::Net


#endif // Net_ICMPClient_INCLUDED
