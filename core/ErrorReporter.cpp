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
		
		while (true) {
			size_t part = 0;
			zmq_msg_t messageIn;
			zmq_msg_recv(&messageIn, socket, 0); ++part;
			
			size_t length = zmq_msg_size(&messageIn);
			void *source = zmq_msg_data(&messageIn);
			
			while (zmq_msg_more(&messageIn)) {
				const std::string message((const char*)source, length);
				std::cerr << "part " << part << ":\t";
				std::cerr << message << std::endl;
				
				zmq_msg_recv(&messageIn, socket, ZMQ_RCVMORE); ++part;
				length = zmq_msg_size(&messageIn);
				source = zmq_msg_data(&messageIn);
			}
			const std::string message((const char*)source, length);
			if (part > 1) {
				std::cerr << "part " << part << ":\t";
			}
			std::cerr << message << std::endl;
			if (part > 1) {
				std::cerr << "(END)" << std::endl;
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
