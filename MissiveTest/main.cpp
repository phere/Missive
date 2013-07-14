//
//  main.cpp
//  MissiveTest
//
//  Created by Rowan James on 13/07/13.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

#include <iostream>
#include <sstream>

#include <unistd.h>

#import <thread>

#import "Message.hpp"
#import "Dispatcher.hpp"
#import "ErrorReporter.hpp"
#import "NetPublisher.hpp"

namespace {
	void netLogger()
	{
		Missive::NetPublisher netPublisher(Missive::Dispatcher::sharedInstance());
		std::cout << "Starting NetPublisher on tcp port " << netPublisher.getPort() << std::endl;
		netPublisher.run();
	}
}

int main(int argc, const char * argv[])
{
	Missive::ErrorReporter::start();
	
	std::thread netPubThread(netLogger);
	
	int message = 0;
	while (true) {
		std::ostringstream os;
		os << "Test message #" << message++;
		
		Missive::Dispatcher::sharedInstance().send(Missive::Message(os.str()));
		sleep(1);
	}
	return 0;
}

