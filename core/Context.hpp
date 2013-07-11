//
//  Context.hpp
//  Missive
//
//  Created by Rowan James on 26/04/2013.
//  Copyright 2013 Rowan James. All rights reserved.
//

namespace Missive
{
	class Dispatcher;
	
	class Context
	{
	public:
		static Context& sharedContext();
		~Context();
		
		Missive::Dispatcher& getDispatcher();
		
	private:
		Context();
		static void init();
		Missive::Dispatcher* dispatcher;
	};
}
