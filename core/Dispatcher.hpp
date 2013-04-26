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
	class Dispatcher
	{
	public:
		Dispatcher();
		virtual ~Dispatcher();
		
		void send(std::string const& message);
		
	protected:
		
	private:
	};
}
