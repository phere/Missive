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
		Dispatcher();
		
		void send(std::string const& message);
		
		void *subscribe();
		void unsubscribe(void *subscription);
		
	protected:
		
	private:
		void *context;
		Missive::detail::DispatchThread *dispatchThread;
	};
}
