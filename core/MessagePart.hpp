//
//  MessagePart.hpp
//  Missive
//
//  Created by Rowan James on 14/07/13.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

#ifndef __Missive__MessagePart__
#define __Missive__MessagePart__

#include <string>

namespace Missive
{
	class MessagePart
	{
	public:
		MessagePart();
		MessagePart(std::string &text);
		MessagePart(std::string &&text);
		MessagePart(void *sourceData, size_t sourceLength);

		MessagePart(MessagePart &&source);
		~MessagePart();
		
		void const *getData() const;
		size_t length() const;
		
	private:
		MessagePart(MessagePart const& rhs) = delete;
		MessagePart& operator=(MessagePart const& rhs) = delete;
		
		void const *data;
		size_t dataLength;
	};
}

#endif /* defined(__Missive__MessagePart__) */
