/*
 * tinyjsonpp.cpp
 *
 * Created: 11-Sept-17 12:25:10
 * Author : hydroflax
 */ 

#include <avr/io.h>
#include <string.h>
#include "tinyjsonpp.h"

tinyjsonpp::tinyjsonpp(bool store, unsigned int maxSize) {
	char* newchar = static_cast<char*>(malloc(maxSize));
	this->json = newchar;

	this->jsonSize = maxSize;
	this->location = 0;
}

tinyjsonpp::tinyjsonpp(bool store, bool ignoreQuotes, unsigned int maxSize) {
	char* newchar = static_cast<char*>(malloc(maxSize));
	this->json = newchar;

	this->jsonSize = maxSize;
	this->location = 0;
}

void* tinyjsonpp::operator new(size_t objsize) {
	return malloc(objsize);
}

void tinyjsonpp::operator delete(void* obj) {
	free(obj);
}

void tinyjsonpp::empty() {
	this->location = 0;
	memset(this->json, 0, this->jsonSize);
	// Alternative to save compute time is to just set the first character to 0.
}

void tinyjsonpp::reset() {
	this->location = 0;
	memset(this->json, 0, this->jsonSize);
	this->json[0] = '{';
	this->json[1] = '}';
}

void tinyjsonpp::parse(char c) {
	// Ignore Newline, Space, Return Carriage Characters.
	if (c <= 0x20) {
		return;
	}

	this->json[this->location] = c;
	++this->location;
}

void tinyjsonpp::getvalue(char* key, unsigned int keySize) {
	this->key = static_cast<char*>(memmem(this->json, jsonSize, key, keySize));

	// The key was found in the JSON. (If not, key will be set to NULL).
	if(this->key) {
		this->keySize = keySize;

		// Set the current location
		this->location = this->key - this->json;

		// Get the entire value.
		// loop through each char in the string until a "," or "}" is found or if an opening "[", then a "]" is found.
		bool valueFound = false;
		bool array = false;
		while(!valueFound) {

			// Start of Value after ':"' or ':['
			if(this->json[location] == ':') {
				this->value = &this->json[location + 2];
				if(this->json[location + 1] == '[') {
					array = true;
				}
			}

			if(this->json[location] == ',' || this->json[location] == '}') {
				if(!array) {
					this->valueSize = &this->json[location - 1] - this->value ;
					valueFound = true;
				}
			}

			if(this->json[location] == ']') {
				this->valueSize = &this->json[location] - this->value;
				valueFound = true;
			}

			++location;
		}

	} else {
		this->key = NULL;
		this->keySize = 0;
		this->value = NULL;
		this->valueSize = 0;
		this->valueType = 'n';
	}
}