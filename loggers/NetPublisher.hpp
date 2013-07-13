//
//  NetPublisher.hpp
//  Missive
//
//  Created by Rowan James on 10/07/13.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

namespace Missive
{
	class Dispatcher;
	class NetPublisher
	{
	public:
		NetPublisher(Missive::Dispatcher &dispatcher);
		virtual ~NetPublisher();
		
		int getPort();
		
		void run();
		
	private:
		Missive::Dispatcher &dispatcher;
		
		void *subSocket;
		void *pubSocket;
		int port;
	};
}
