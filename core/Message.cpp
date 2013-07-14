//
//  Message.cpp
//  Missive
//
//  Created by Rowan James on 14/07/13.
//  Copyright (c) 2013 Rowan James. All rights reserved.
//

#include "Message.hpp"

Missive::Message::Message(Missive::MessagePart &&body)
{
	messageParts.emplace_back(std::forward<Missive::MessagePart>(body));
}

Missive::Message::Message(Missive::Message &&source)
: messageParts(std::move(source.messageParts))
{}

void Missive::Message::push(Missive::MessagePart &&part)
{
	messageParts.emplace_back(std::forward<Missive::MessagePart>(part));
}

Missive::MessagePart Missive::Message::pop()
{
	if (empty()) {
		throw std::underflow_error("Missive::Message::pop(): already empty()");
	}
	Missive::MessagePart result(std::move(messageParts.front()));
	messageParts.pop_front();
	return result;
}

bool Missive::Message::empty() const
{
	return messageParts.empty();
}