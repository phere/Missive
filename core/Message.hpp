//
//  Message.hpp
//  Missive
//
//  Created by Rowan James on 14/07/13.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

#ifndef __Missive__Message__
#define __Missive__Message__

#include "MessagePart.hpp"

#include <list>

namespace Missive
{
	class Message
	{
	public:
		Message() = default;
		Message(Missive::MessagePart &&body);
		Message(Message &&source);
		
		void push(Missive::MessagePart &&part);
		MessagePart pop();
		
		bool empty() const;
		
	private:
		Message(Message const& rhs) = delete;
		Message& operator=(Message const& rhs) = delete;
		
		typedef	std::list<Missive::MessagePart> MessageParts;
		MessageParts messageParts;
	};
}

#endif /* defined(__Missive__Message__) */
