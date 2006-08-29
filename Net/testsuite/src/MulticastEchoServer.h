//
// MulticastEchoServer.h
//
// $Id: //poco/1.2/Net/testsuite/src/MulticastEchoServer.h#1 $
//
// Definition of the MulticastEchoServer class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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


#ifndef MulticastEchoServer_INCLUDED
#define MulticastEchoServer_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/MulticastSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetworkInterface.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"


class MulticastEchoServer: public Poco::Runnable
	/// A simple sequential Multicast echo server.
{
public:
	MulticastEchoServer();
		/// Creates the MulticastEchoServer.

	~MulticastEchoServer();
		/// Destroys the MulticastEchoServer.

	Poco::UInt16 port() const;
		/// Returns the port the echo server is
		/// listening on.
		
	void run();
		/// Does the work.
		
	const Poco::Net::SocketAddress& group() const;
		/// Returns the group address where the server listens.
	
	const Poco::Net::NetworkInterface& interface() const;
		/// Returns the network interface for multicasting.
	
protected:	
	static Poco::Net::NetworkInterface findInterface();
		/// Finds an appropriate network interface for
		/// multicasting.
	
private:
	Poco::Net::MulticastSocket  _socket;
	Poco::Net::SocketAddress    _group;
	Poco::Net::NetworkInterface _if;
	Poco::Thread _thread;
	Poco::Event  _ready;
	bool         _stop;
};


#endif // MulticastEchoServer_INCLUDED
