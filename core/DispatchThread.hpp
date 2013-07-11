//
//  DispatchThread.hpp
//  Missive
//
//  Created by Rowan James on 26/04/2013.
//  Copyright 2013 Rowan James. All rights reserved.
//

#include <string>

#include <thread>
#include <mutex>

namespace Missive
{
	namespace detail
	{
		class DispatchThread
		{
		public:
			DispatchThread(void *context);
			
			std::string getDispatchEndpoint();
			std::string getPublisherEndpoint();
			
		private:
			void launch();
			void main();
			
			std::string dispatchEndpoint;
			std::string publisherEndpoint;
			
			std::thread *dispatchThreadHandle;
			void *context;
			
			bool launched;
			std::mutex launchedMutex;
			std::condition_variable launchedCondition;
		};
	}
}
