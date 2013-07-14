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
#include "Message.hpp"
#include "DispatchThread.hpp"

// system and library headers
#include <zmq.h>
#include <thread>
#include <iostream>

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

Missive::Dispatcher& Missive::Dispatcher::sharedInstance()
{
	static Missive::Dispatcher instance;
	return instance;
}

//-----------------------------------------------------------------------------
// Dispatcher class implementation
Missive::Dispatcher::Dispatcher()
: context(zmq_ctx_new())
, dispatchThread(new Missive::detail::DispatchThread(context))
{
	
}

void Missive::Dispatcher::send(Missive::Message &&message)
{
	void* socket = getDispatchSocket(context, dispatchThread);
	
	while (!message.empty()) {
		Missive::MessagePart part(message.pop());
		int flags = message.empty() ? 0 : ZMQ_SNDMORE;
		zmq_send(socket, part.getData(), part.length(), flags);
	}
}

void *Missive::Dispatcher::subscribe()
{
	void *subSocket = zmq_socket(context, ZMQ_SUB);
	int connectResult = zmq_connect(subSocket, dispatchThread->getPublisherEndpoint().c_str());
	if (connectResult != 0) {
		std::cerr << "Could not connect socket to dispatcher. Connect result: " << connectResult << " errno:" << errno << std::endl;
		return nullptr;
	}
	int subscribeResult = zmq_setsockopt(subSocket, ZMQ_SUBSCRIBE, NULL, 0);
	if (subscribeResult != 0) {
		std::cerr << "Could not subscribe to messages. Subscribe result: " << subscribeResult << " errno:" << strerror(errno) << std::endl;
		return nullptr;
	}
	
	return subSocket;
}

void Missive::Dispatcher::unsubscribe(void *subscription)
{
	zmq_close(subscription);
}

void *Missive::Dispatcher::open_socket(int type)
{
	return zmq_socket(context, type);
}

void Missive::Dispatcher::close_socket(void *socket)
{
	zmq_close(socket);
}
