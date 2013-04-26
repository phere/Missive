//
//  Dispatcher.cpp
//  Missive
//
//  Created by Rowan James on 26/04/2013.
//  Copyright 2013 Rowan James. All rights reserved.
//

// this class' header
#import "Dispatcher.hpp"

// other headers within the project
#include "Context.hpp"
#include "DispatchThread.hpp"

// system and library headers
#include <boost/thread/tss.hpp>
#include <zmq.h>

//-----------------------------------------------------------------------------
// static code and helpers
// an 'unnamed-namespace' (7.3.1.1) isolates code to this file
namespace
{
	// constants

	// helper functions and classes
	struct ZMQSocketWrapper {
		ZMQSocketWrapper()
		: socket(NULL)
		{}
		~ZMQSocketWrapper() {
			zmq_close(socket);
		}
		void *socket;
	};
	boost::thread_specific_ptr<ZMQSocketWrapper> dispatchSocket;
	
	void* getDispatchSocket()
	{
		Missive::DispatchThread::start();
		
		Missive::Context context;
		ZMQSocketWrapper *wrapper = dispatchSocket.get();
		if (wrapper == NULL) {
			wrapper = new ZMQSocketWrapper;
			wrapper->socket = zmq_socket(context.getContext(), ZMQ_PUSH);
			zmq_connect(wrapper->socket, "inproc://dispatch");
			dispatchSocket.reset(wrapper);
		}
		return wrapper->socket;
	}
}

//-----------------------------------------------------------------------------
// Dispatcher class implementation

Missive::Dispatcher::Dispatcher()
{}

Missive::Dispatcher::~Dispatcher()
{}

void Missive::Dispatcher::send(std::string const& message)
{
	void* socket = getDispatchSocket();
	zmq_send(socket, message.c_str(), message.length(), 0);
}
