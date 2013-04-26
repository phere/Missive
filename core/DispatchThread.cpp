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
#include <zmq.h>

//-----------------------------------------------------------------------------
// static code and helpers
// an 'unnamed-namespace' (7.3.1.1) isolates code to this file
namespace
{
	// constants

	// helper functions and classes
	
	boost::thread* dispatchThreadHandle = NULL;
	boost::once_flag dispatchThreadFlag;
	boost::mutex dispatchThreadReadyMutex;
	boost::condition_variable dispatchThreadReadyCV;
	volatile bool dispatchThreadReady = false;
	
	std::string publisherEndpoint;
	
	void dispatchThread()
	{
		Missive::Context context;
		void *dispatchReceiveSocket = zmq_socket(context.getContext(), ZMQ_PULL);
		zmq_bind(dispatchReceiveSocket, "inproc://dispatch");
		
		publisherEndpoint = "inproc://publish";
		void *dispatchPubSocket = zmq_socket(context.getContext(), ZMQ_PUB);
		zmq_bind(dispatchPubSocket, publisherEndpoint.c_str());
		
		
		{// release waiting threads
			boost::unique_lock<boost::mutex> dispatchThreadReadyLock(dispatchThreadReadyMutex);
			dispatchThreadReady = true;
			dispatchThreadReadyCV.notify_all();
		}
		char buffer[4096];
		while (true) {
			size_t msgSize = zmq_recv(dispatchReceiveSocket, buffer, sizeof(buffer), 0);
			if (msgSize > 0) {
				zmq_send(dispatchPubSocket, buffer, msgSize, 0);
			}
		}
	}
	
	void launchDispatchThread()
	{
		dispatchThreadHandle = new boost::thread(dispatchThread);
		{// wait for the socket to be bound
			boost::unique_lock<boost::mutex> dispatchThreadReadyLock(dispatchThreadReadyMutex);
			while (!dispatchThreadReady) {
				dispatchThreadReadyCV.wait(dispatchThreadReadyLock);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// DispatchThread class implementation
void Missive::DispatchThread::start()
{
	boost::call_once(dispatchThreadFlag, launchDispatchThread);
}

std::string Missive::DispatchThread::getPublisherEndpoint()
{
	start();
	return publisherEndpoint;
}
