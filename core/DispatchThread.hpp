//
//  DispatchThread.hpp
//  Missive
//
//  Created by Rowan James on 26/04/2013.
//  Copyright 2013 Rowan James. All rights reserved.
//

#include <string>

namespace boost
{
	class thread;
	class barrier;
}

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
			void main(boost::barrier &launchBarrier);
			
			std::string dispatchEndpoint;
			std::string publisherEndpoint;
			
			boost::thread *dispatchThreadHandle;
			void *context;
		};
	}
}
