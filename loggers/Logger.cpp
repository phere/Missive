//
//  Logger.cpp
//  Missive
//
//  Created by Rowan James on 13/07/13.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

#include "Logger.hpp"

#include "Dispatcher.hpp"

Missive::Logger::Logger(Missive::Dispatcher &dispatcher_)
: dispatcher(dispatcher_)
, threadHandle(nullptr)
{}


Missive::Logger::~Logger()
{}

void Missive::Logger::start()
{
	if (threadHandle != nullptr) {
		return;
	}
	threadHandle = new std::thread(&Missive::Logger::run, this);
}

void Missive::Logger::run()
{
	subscription = dispatcher.subscribe();
	loop();
	dispatcher.unsubscribe(subscription);
	subscription = nullptr;
}
