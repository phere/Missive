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


#import "Dispatcher.hpp"
#import "ErrorReporter.hpp"

int main(int argc, const char * argv[])
{
	Missive::ErrorReporter::start();
	
	int message = 0;
	while (true) {
		std::ostringstream os;
		os << "Test message #" << message++;
		Missive::Dispatcher::sharedInstance().send(os.str());
		sleep(1);
	}
	return 0;
}

