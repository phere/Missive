//
//  MessagePart.cpp
//  Missive
//
//  Created by Rowan James on 14/07/13.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

#include "MessagePart.hpp"

Missive::MessagePart::MessagePart()
: data(nullptr)
, dataLength(0)
{}

Missive::MessagePart::MessagePart(std::string &text)
: MessagePart((void*)text.c_str(), text.length())
{}

Missive::MessagePart::MessagePart(std::string &&text)
: MessagePart((void*)text.c_str(), text.length())
{}

Missive::MessagePart::MessagePart(void *sourceData, size_t sourceLength)
: MessagePart()
{
	if (sourceLength > 0) {
		void *dataCopy = new uint8_t[sourceLength];
		if (!dataCopy) {
			throw std::bad_alloc();
		}
		memcpy(dataCopy, sourceData, sourceLength);
		data = dataCopy;
		dataLength = sourceLength;
	}
}

Missive::MessagePart::MessagePart(Missive::MessagePart &&source)
: data(source.data)
, dataLength(source.dataLength)
{
	source.dataLength = 0;
	source.data = nullptr;
}

Missive::MessagePart::~MessagePart()
{
	dataLength = 0;
	delete[] (uint8_t*)data;
	data = nullptr;
}

void const * Missive::MessagePart::getData() const
{
	return data;
}

size_t Missive::MessagePart::length() const
{
	return dataLength;
}
