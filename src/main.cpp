#include <Arduino.h>
#include <IRReceiver.hpp>

const int ir_pin = 2;
const int working_led = 13;

volatile bool repeated = false;
volatile bool status = false;
volatile byte ir_addr;
volatile byte ir_addr_reversed;
volatile byte command;
volatile byte command_reversed;

IRReceiver aReceiver(ir_pin);
IRReceiverOutput anOutput;

void callbackIR() {
	if (status) return;

	// We do not want to be interrupted here
	noInterrupts();

	// We are working in, turn on the light!
	digitalWrite(working_led, HIGH);
	status = true;

	anOutput = aReceiver.recv();

	// Stop working, lights off..
	digitalWrite(working_led, LOW);

	// Now we want..
	interrupts();
}

void setup() {
	Serial.begin(9600);
	pinMode(ir_pin, INPUT);
	pinMode(working_led, OUTPUT);

	attachInterrupt(0, callbackIR, FALLING);
}

void loop() {
	if (status) {
		if (aReceiver.wasRepeated())
			Serial.println("Last command repeated");
		else
			Serial.println(anOutput);
		status = false;
	}
}
