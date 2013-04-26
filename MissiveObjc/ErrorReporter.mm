//
//  ErrorReporter.mm
//  Missive
//
//  Created by Rowan James on 26/04/2013.
//  Copyright 2013 Rowan James. All rights reserved.
//

// this class' header
#import "ErrorReporter.h"

// other headers within the project
#import "ErrorReporter.hpp"

// system and library headers

//-----------------------------------------------------------------------------
// static code and helpers
// an 'unnamed-namespace' (7.3.1.1) isolates code to this file
namespace
{
	// constants

	// helper functions and classes
}

//-----------------------------------------------------------------------------
// hidden interface (anonymous category)
@interface ErrorReporter ()
@end

//-----------------------------------------------------------------------------
// ErrorReporter class implementation
@implementation ErrorReporter

+ (void)start
{
	Missive::ErrorReporter::start();
}

@end
