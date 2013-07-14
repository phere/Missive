//
//  Dispatcher.hpp
//  Missive
//
//  Created by Rowan James on 26/04/2013.
//  Copyright 2013 Rowan James. All rights reserved.
//

#include <string>

namespace Missive
{
	namespace detail
	{
		class DispatchThread;
	}
	class Message;
	
	class Dispatcher
	{
	public:
		static Dispatcher& sharedInstance();
		
		// front-end (client) interface
		void send(Message &&message);
		
		// back-end (library) interface
		void *subscribe();
		void unsubscribe(void *subscription);
		
		void *open_socket(int type);
		void close_socket(void *socket);
		
	protected:
		
	private:
		Dispatcher();
		
		void *context;
		Missive::detail::DispatchThread *dispatchThread;
	};
}
