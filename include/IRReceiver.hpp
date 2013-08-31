/*
 * IRReceiver.hpp
 *
 *  Created on: Aug 31, 2013
 *      Author: massi
 */

#ifndef IRRECEIVER_HPP_
#define IRRECEIVER_HPP_

#include <Arduino.h>
#include <Printable.h>

// This is the output of IRReceiver
struct IRReceiverOutput : public Printable {
	virtual ~IRReceiverOutput() {};
	bool operator==(IRReceiverOutput const & right) const;
//	IRReceiverOutput operator=(IRReceiverOutput & right) const;

	volatile byte IRAddress;
	volatile byte IRAddressReversed;
	volatile byte Command;
	volatile byte CommandReversed;

	virtual size_t printTo(Print& p) const ;
};

class IRReceiver : public Printable {
public:
	IRReceiver(uint32_t irPin);
	virtual ~IRReceiver();
	virtual size_t printTo(Print& p) const;

	// This method has to be triggered when the IR receiver first changes state
	// It is thread-safe and can be inserted in an interrupt callback.
	IRReceiverOutput const & recv();
	bool const & wasRepeated() const;

private:
	byte const getByte() const;

	uint32_t _pin;
	IRReceiverOutput _lastResult;
	bool _wasRepeated;
};


#endif /* IRRECEIVER_HPP_ */
