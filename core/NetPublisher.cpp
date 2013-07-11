//
//  NetPublisher.cpp
//  Missive
//
//  Created by Rowan James on 10/07/13.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

#include "NetPublisher.hpp"

#import "Dispatcher.hpp"

#import <sstream>
#import <zmq.h>

Missive::NetPublisher::NetPublisher(Missive::Dispatcher &dispatcher_)
: dispatcher(dispatcher_)
, subSocket(nullptr)
, pubContext(zmq_ctx_new())
, pubSocket(nullptr)
, port(0)
{
	subSocket = dispatcher.subscribe();
	
	pubSocket = zmq_socket(pubContext, ZMQ_PUB);
	
	zmq_bind(pubSocket, "tcp://*:*");
	
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

}

Missive::NetPublisher::~NetPublisher()
{
	dispatcher.unsubscribe(subSocket);

	zmq_close(pubSocket);
	
	zmq_ctx_destroy(pubContext);
}

void Missive::NetPublisher::run()
{
	while (true) {
		zmq_msg_t messageIn;
		zmq_msg_recv(&messageIn, subSocket, 0);
	
		size_t length = zmq_msg_size(&messageIn);
		void *source = zmq_msg_data(&messageIn);
		
		while (zmq_msg_more(&messageIn)) {
			zmq_send(pubSocket, source, length, ZMQ_SNDMORE);
			
			zmq_msg_recv(&messageIn, subSocket, ZMQ_RCVMORE);
			length = zmq_msg_size(&messageIn);
			source = zmq_msg_data(&messageIn);
		}
		zmq_send(pubSocket, source, length, 0);
	}
}

int Missive::NetPublisher::getPort()
{
	return port;
}
