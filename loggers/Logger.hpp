//
//  Logger.h
//  Missive
//
//  Created by Rowan James on 13/07/13.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

#ifndef __Missive__Logger__
#define __Missive__Logger__

#include <thread>

namespace Missive
{
	class Dispatcher;
	
	class Logger
	{
	public:
		Logger(Missive::Dispatcher &dispatcher);
		void start();

	private:
		virtual ~Logger();
		
		void run();
		virtual void loop() = 0;
		
		Missive::Dispatcher &dispatcher;
		std::thread *threadHandle;
		void *subscription;
	};
}

#endif /* defined(__Missive__Logger__) */
