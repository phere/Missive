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
#import "Context.hpp"

// system and library headers
#include <boost/thread/once.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <zmq.h>

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
{
	launch(); // does not return until the thread is launched and the endpoints assigned
}

void Missive::detail::DispatchThread::launch()
{
	boost::barrier launchBarrier(2);
	dispatchThreadHandle = new boost::thread(boost::bind(&Missive::detail::DispatchThread::main, this,
														 boost::ref(launchBarrier)));
	launchBarrier.wait();
}

void Missive::detail::DispatchThread::main(boost::barrier &launchBarrier)
{
	dispatchEndpoint = "inproc://dispatch";
	void *dispatchReceiveSocket = zmq_socket(context, ZMQ_PULL);
	zmq_bind(dispatchReceiveSocket, dispatchEndpoint.c_str());
	
	publisherEndpoint = "inproc://publish";
	void *dispatchPubSocket = zmq_socket(context, ZMQ_PUB);
	zmq_bind(dispatchPubSocket, publisherEndpoint.c_str());
	
	launchBarrier.wait();
	
	char buffer[4096];
	while (true) {
		size_t msgSize = zmq_recv(dispatchReceiveSocket, buffer, sizeof(buffer), 0);
		if (msgSize > 0) {
			zmq_send(dispatchPubSocket, buffer, msgSize, 0);
		}
	}
}

std::string Missive::detail::DispatchThread::getDispatchEndpoint()
{
	return dispatchEndpoint;
}

std::string Missive::detail::DispatchThread::getPublisherEndpoint()
{
	return publisherEndpoint;
}
