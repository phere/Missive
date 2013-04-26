//
//  Context.cpp
//  Missive
//
//  Created by Rowan James on 26/04/2013.
//  Copyright 2013 Rowan James. All rights reserved.
//

// this class' header
#import "Context.hpp"

// other headers within the project

// system and library headers
#include <boost/thread/once.hpp>
#include <zmq.h>

//-----------------------------------------------------------------------------
// static code and helpers
// an 'unnamed-namespace' (7.3.1.1) isolates code to this file
namespace
{
	// constants
	void *context = NULL;

	// helper functions and classes
	boost::once_flag initContextFlag;
	void initContext()
	{
		context = zmq_ctx_new();
	}
}

//-----------------------------------------------------------------------------
// Context class implementation
void* Missive::Context::getContext()
{
	boost::call_once(initContextFlag, initContext);
	return context;
}
