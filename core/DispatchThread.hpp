//
//  DispatchThread.hpp
//  Missive
//
//  Created by Rowan James on 26/04/2013.
//  Copyright 2013 Rowan James. All rights reserved.
//

#include <string>

namespace Missive
{
	class DispatchThread
	{
	public:
		static void start();
		static std::string getPublisherEndpoint();
	};
}
