//
//  DispatchThread.cpp
//  Missive
//
//  Created by Rowan James on 26/04/2013.
//  Copyright 2013 Rowan James. All rights reserved.
//

// this class' header
#import "DispatchThread.hpp"

// other headers within the project

// system and library headers
#include <zmq.h>
#include <functional>

//-----------------------------------------------------------------------------
// static code and helpers
// an 'unnamed-namespace' (7.3.1.1) isolates code to this file
namespace
{
	// constants

	// helper functions and classes

}

//-----------------------------------------------------------------------------
// DispatchThread class implementation
Missive::detail::DispatchThread::DispatchThread(void *context_)
: dispatchThreadHandle(nullptr)
, context(context_)
, launched(false)
{
	launch(); // does not return until the thread is launched and the endpoints assigned
}

void Missive::detail::DispatchThread::launch()
{
	dispatchThreadHandle = new std::thread(&Missive::detail::DispatchThread::main, this);
	
	std::unique_lock<std::mutex> launchedLock(launchedMutex);
	while (!launched) {
		launchedCondition.wait(launchedLock);
	}
}

void Missive::detail::DispatchThread::main()
{
	dispatchEndpoint = "inproc://dispatch";
	void *dispatchReceiveSocket = zmq_socket(context, ZMQ_PULL);
	zmq_bind(dispatchReceiveSocket, dispatchEndpoint.c_str());
	
	publisherEndpoint = "inproc://publish";
	void *dispatchPubSocket = zmq_socket(context, ZMQ_PUB);
	zmq_bind(dispatchPubSocket, publisherEndpoint.c_str());
	
	{
		std::unique_lock<std::mutex> launchLock(launchedMutex);
		launched = true;
		launchedCondition.notify_all();
	}
	
	zmq_proxy(dispatchReceiveSocket, dispatchPubSocket, nullptr);
}

std::string Missive::detail::DispatchThread::getDispatchEndpoint()
{
	return dispatchEndpoint;
}

std::string Missive::detail::DispatchThread::getPublisherEndpoint()
{
	return publisherEndpoint;
}
