//
//  NetPublisher.cpp
//  Missive
//
//  Created by Rowan James on 10/07/13.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

#include "NetPublisher.hpp"

#import "Dispatcher.hpp"
#import <iostream>
#import <sstream>
#import <zmq.h>

Missive::NetPublisher::NetPublisher(Missive::Dispatcher &dispatcher_)
: dispatcher(dispatcher_)
, subSocket(nullptr)
, pubSocket(nullptr)
, port(0)
{
	pubSocket = dispatcher.open_socket(ZMQ_PUB);
	
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
	dispatcher.close_socket(pubSocket);
}

void Missive::NetPublisher::run()
{
	subSocket = dispatcher.subscribe();
	
	zmq_proxy(subSocket, pubSocket, nullptr);
}

int Missive::NetPublisher::getPort()
{
	return port;
}
