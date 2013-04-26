//
//  Publisher.mm
//  Missive
//
//  Created by Rowan James on 26/04/2013.
//  Copyright 2013 Rowan James. All rights reserved.
//

// this class' header
#import "Publisher.h"

// other headers within the project
#import "Context.hpp"
#import "DispatchThread.hpp"

// system and library headers
#include <zmq.h>
#include <sstream>

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
@interface Publisher ()
< NSNetServiceDelegate >

@property (nonatomic, strong) NSNetService *netService;

@end

//-----------------------------------------------------------------------------
// Publisher class implementation
@implementation Publisher

+ (Publisher*)sharedInstance
{
	static Publisher *instance = nil;
	if (instance == nil) {
		instance = [Publisher new];
	}
	return instance;
}

+ (NSThread*)publisherThread
{
	static NSThread *thread = nil;
	if (!thread) {
		Publisher *publisher = [[self class] sharedInstance];
		thread = [[NSThread alloc] initWithTarget:publisher
										 selector:@selector(doRun)
										   object:nil];
	}
	return thread;
}

+ (void)start
{
	NSThread *thread = [[self class] publisherThread];
	if (!thread.isExecuting) {
		[thread start];
	}
}

- (void)doRun
{
	Missive::Context context;
	void *subSocket = zmq_socket(context.getContext(), ZMQ_SUB);
	zmq_connect(subSocket, Missive::DispatchThread::getPublisherEndpoint().c_str());
	zmq_setsockopt(subSocket, ZMQ_SUBSCRIBE, NULL, 0);
	
	void *pubSocket = zmq_socket(context.getContext(), ZMQ_PUB);
	zmq_bind(pubSocket, "tcp://*:*");
	
	int port = 0;
	{ // discover which port we were actually bound to
		char endpointBuffer[256];
		size_t endpointLength = sizeof(endpointBuffer);
		zmq_getsockopt(pubSocket, ZMQ_LAST_ENDPOINT, endpointBuffer, &endpointLength);
		std::string endpoint(endpointBuffer, endpointLength);
		size_t portPos = endpoint.find_last_of(':');
		std::string portStr(endpoint.substr(portPos + 1));
		
		std::stringstream ss;
		ss << portStr;
		ss >> port;
	}
	
	self.netService = [[NSNetService alloc] initWithDomain:@""
													  type:@"_missive._tcp"
													  name:@"Missive publisher"
													  port:port];
	[self.netService publish];
	
	char buffer[4096];
	while (true) {
		size_t length = zmq_recv(subSocket, buffer, sizeof(buffer), 0);
		if (length > 0) {
			zmq_send(pubSocket, buffer, length, 0);
		}
	}
	
	zmq_close(subSocket);
	zmq_close(pubSocket);
}



@end
