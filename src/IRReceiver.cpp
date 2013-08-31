/*
 * IRReceiver.cpp
 *
 *  Created on: Aug 31, 2013
 *      Author: massi
 */

#include <Arduino.h>
#include <IRReceiver.hpp>
#include <Print.h>
#include <Printable.h>

IRReceiver::IRReceiver(uint32_t irPin) :
Printable(),
_pin(irPin),
_wasRepeated(false) {

}

IRReceiver::~IRReceiver() {

}

size_t IRReceiver::printTo(Print & p) const {
	return _lastResult.printTo(p);
}

IRReceiverOutput const & IRReceiver::recv() {
	// Re-read everything only if the first pulse lasts more than 4.5 ms
	if (pulseIn(_pin, HIGH, 500000) >= 4400) {
		_wasRepeated = false;
		_lastResult.IRAddress = getByte();
		_lastResult.IRAddressReversed = getByte();
		_lastResult.Command = getByte();
		_lastResult.CommandReversed = getByte();
	}
	else
		_wasRepeated = true;

	return _lastResult;
}

bool const & IRReceiver::wasRepeated() const {
	return _wasRepeated;
}

byte const IRReceiver::getByte() const {
	byte return_value = 0x0;

	for (int i = 0; i < 8; i++) {
		// If the pulse lasts ~1.5ms it's a 1, let's write it in our byte
		if (pulseIn(_pin, HIGH, 5000) >= 1500) {
			return_value |= (1 << i);
		}
	}

	return return_value;
}

bool IRReceiverOutput::operator ==(const IRReceiverOutput & right) const {
	return (Command == right.Command && IRAddress == right.IRAddress);
}

size_t IRReceiverOutput::printTo(Print& p) const {
	char buffer[1024];
	sprintf(buffer,
			"Remote Address:         0x%x\n"
			"Remote Reverse Address: 0x%x\n"
			"Remote Command:         0x%x\n"
			"Remote Reverse Command: 0x%x\n",
			IRAddress, IRAddressReversed, Command, CommandReversed);

	return p.print(buffer);
}
