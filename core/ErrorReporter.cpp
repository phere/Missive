//
//  ErrorReporter.cpp
//  Missive
//
//  Created by Rowan James on 26/04/2013.
//  Copyright 2013 Rowan James. All rights reserved.
//

// this class' header
#import "ErrorReporter.hpp"

// other headers within the project
#include "Context.hpp"
#include "Dispatcher.hpp"

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
	boost::thread* errorReporterThreadHandle = NULL;
	void errorReporterThread()
	{
		Missive::Dispatcher& dispatcher = Missive::Context::sharedContext().getDispatcher();
		void *socket = dispatcher.subscribe();
		
		char buffer[4096];
		while (true) {
			size_t size = zmq_recv(socket, buffer, sizeof(buffer), 0);
			if (size > 0) {
				std::string message(buffer, size);
				std::cerr << message << std::endl;
			}
		}
		
		dispatcher.unsubscribe(socket);
	}
	
	boost::once_flag errorReporterStartedFlag;
	void startErrorReporter()
	{
		errorReporterThreadHandle = new boost::thread(errorReporterThread);
	}
}

//-----------------------------------------------------------------------------
// ErrorReporter class implementation
void Missive::ErrorReporter::start()
{
	boost::call_once(errorReporterStartedFlag, startErrorReporter);
}
