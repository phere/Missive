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
	class Context;
	namespace detail
	{
		class DispatchThread;
	}
	
	class Dispatcher
	{
	public:
		static Dispatcher& sharedInstance();
		
		// front-end (client) interface
		void send(std::string const& message);
		
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
