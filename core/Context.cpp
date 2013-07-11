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
#import "Dispatcher.hpp"

// system and library headers

//-----------------------------------------------------------------------------
// static code and helpers
// an 'unnamed-namespace' (7.3.1.1) isolates code to this file
namespace
{
}

//-----------------------------------------------------------------------------
// Context class implementation
Missive::Context& Missive::Context::sharedContext()
{
	static Missive::Context instance;
	return instance;
}

Missive::Context::Context()
: dispatcher(new Missive::Dispatcher())
{
}

Missive::Context::~Context()
{
	delete dispatcher;
}

Missive::Dispatcher& Missive::Context::getDispatcher()
{
	return *dispatcher;
}
