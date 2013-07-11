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
#include <zmq.h>
#include <thread>

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
	std::__thread_specific_ptr<ZMQSocketWrapper> dispatchSocket;
	
	void* getDispatchSocket(void *context, Missive::detail::DispatchThread *dispatchThread)
	{
		ZMQSocketWrapper *wrapper = dispatchSocket.get();
		if (wrapper == NULL) {
			wrapper = new ZMQSocketWrapper;
			
			const std::string dispatchEndpoint(dispatchThread->getDispatchEndpoint());			
			
			wrapper->socket = zmq_socket(context, ZMQ_PUSH);
			zmq_connect(wrapper->socket, dispatchEndpoint.c_str());
			dispatchSocket.reset(wrapper);
		}
		return wrapper->socket;
	}
}

//-----------------------------------------------------------------------------
// Dispatcher class implementation
Missive::Dispatcher::Dispatcher()
: context(zmq_ctx_new())
, dispatchThread(new Missive::detail::DispatchThread(context))
{
	
}

void Missive::Dispatcher::send(std::string const& message)
{
	void* socket = getDispatchSocket(context, dispatchThread);
	zmq_send(socket, message.c_str(), message.length(), 0);
}

void *Missive::Dispatcher::subscribe()
{
	void *subSocket = zmq_socket(context, ZMQ_SUB);
	zmq_connect(subSocket, dispatchThread->getPublisherEndpoint().c_str());
	zmq_setsockopt(subSocket, ZMQ_SUBSCRIBE, NULL, 0);
	
	return subSocket;
}

void Missive::Dispatcher::unsubscribe(void *subscription)
{
	zmq_close(subscription);
}