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
	this->json = static_cast<char*>(calloc(maxSize, sizeof(char)));

	this->jsonSize = maxSize;
	this->location = 0;
}

tinyjsonpp::tinyjsonpp(bool store, bool ignoreQuotes, unsigned int maxSize) {
	this->json = static_cast<char*>(calloc(maxSize, sizeof(char)));

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

Value tinyjsonpp::getvalue(char* key, unsigned int keySize) {
	this->key.start = static_cast<char*>(memmem(this->json, jsonSize, key, keySize));

	// The key was found in the JSON. (If not, key will be set to NULL).
	if(this->key.start) {
		this->key.size = keySize;

		// Set the current location
		this->location = this->key.start - this->json;

		// Set the values key pointer (and vice versa).
		this->value.key = &this->key;
		this->key.value = &this->value;

		// Get the entire value.
		// loop through each char in the string until a "," or "}" is found or if an opening "[", then a "]" is found.
		bool valueFound = false;
		bool array = false;
		//bool embeddedJSON = false;
		while(!valueFound) {

			// Start of Value after ':"' or ':['
			if(this->json[location] == ':') {
				this->value.start = &this->json[location + 2];

				if (this->json[location + 2] >= '0' && this->json[location + 2] <= '9') {
					// A int has been found (based off first letter).
					this->value.type = 'i';
				} else if(this->json[location + 1] == '[') {
					// An array has been found.
					array = true;
					this->value.type = 'a';
				} else if(this->json[location + 1] == '{') {
					// An embeddedJSON object has been found.
					//embeddedJSON = true;
					this->value.type = 'e';
				} else {
					this->value.type = 's';
				}
			}

			if(this->json[location] == ',' || this->json[location] == '}') {
				if(!array) {
					this->value.size = &this->json[location - 1] - this->value.start;
					valueFound = true;
				}
			}

			if(this->json[location] == ']') {
				this->value.size = &this->json[location] - this->value.start;
				valueFound = true;
			}

			++location;
		}

	} else {
		this->key.start = NULL;
		this->key.size = 0;
		this->value.start = NULL;
		this->value.size = 0;
		this->value.type = 'n';
	}

	return this->value;
}

void tinyjsonpp::insert(char* key, unsigned int keySize, char* value, unsigned int valueSize, char* parent, unsigned int parentLength) {
	// If the key is already in the JSON string then just change the value, else add the k-v pair to the JSON string.
}
