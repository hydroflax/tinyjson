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

	this->maxSize = maxSize;
	this->jsonSize = 0;
}

tinyjsonpp::tinyjsonpp(bool store, bool ignoreQuotes, unsigned int maxSize) {
	this->json = static_cast<char*>(calloc(maxSize, sizeof(char)));

	this->maxSize = maxSize;
	this->jsonSize = 0;
}

void* tinyjsonpp::operator new(size_t objsize) {
	return malloc(objsize);
}

void tinyjsonpp::operator delete(void* obj) {
	free(obj);
}

void tinyjsonpp::empty() {
	this->jsonSize = 0;
	memset(this->json, 0, this->maxSize);
	// Alternative to save compute time is to just set the first character to 0.
}

void tinyjsonpp::reset() {
	this->jsonSize = 0;
	memset(this->json, 0, this->maxSize);
	this->json[0] = '{';
	this->json[1] = '}';
}

void tinyjsonpp::parse(char c) {
	// Ignore Newline, Space, Return Carriage Characters.
	if (c <= 0x20) {
		return;
	}

	this->json[this->jsonSize] = c;
	++this->jsonSize;
}

Value tinyjsonpp::getvalue(char* key) {
	this->key.start = NULL;
	this->key.start = static_cast<char*>(memmem(this->json, this->maxSize, key, strlen(key)));

	// The key was found in the JSON. (If not, key will be set to NULL).
	if(this->key.start) {
		this->key.size = strlen(key);

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

Value tinyjsonpp::getvalue(char* key, char* parent) {
	// See how many / in parent to see how deep we need to go.
	// Find the parent start. i.e. '"<parent>" : {'
	// Count how many open '{' before the first '}' If there are multiple then find then minus the count until at zero again.
	// This is the embedded json value.
	// do the same until the parent string has completed and a value has been found.

	// Currently no error checking!
	//bool parentComplete = false;
		
	// Start searching the whole JSON string.
	this->key.start = this->json;
	this->key.size = this->jsonSize;

	// Separate the parent string.
	//char* search = strsep(&parent, '/');

	// The start of the parent.
	//this->key.start = static_cast<char*>(memmem(this->json, this->maxSize, search, strlen(search)));

	// The parent was found in the JSON. (If not, key will be set to NULL).
	if(this->key.start) {
		
	}
	return this->value;
}

void tinyjsonpp::insert(char* key, char* value) {
	 // Insert the key-value pair into the root JSON object.
}

void tinyjsonpp::insert(char* key, char* value, char* parent) {
	// Uses getValue() for the insert place. e.g. if the parent was "3/user"and the k-v to insert was "hello": "world" then can getvalue of "user" in parent "3". 

	// Separate the parent string into the parent and the key to get the value of.
	unsigned int forwardSlash = 0;	
	location = 0;

	while(location < strlen(parent)) {
		// Store the location of the last known '/'
		if(parent[location] == '/') {
			forwardSlash = location;
		}
		++location;
	}

	// Set the forwardSlash to the end of the parent string.
	char* getValueKey = &parent[forwardSlash + 1];
	parent[forwardSlash] = 0;

	// The below will store the results in this->value so no need to create a new Value.
	getvalue(getValueKey, parent);

	// TODO: WRITE;
	// If the key is already in the JSON string then just change the value, else add the k-v pair to the JSON string.
	// then add the key value pair to the value string. (will need to memmove first (maxsize-jsonsize = free space at the end of the JSON string.).

	// If the parent does not exist then these should be created.
}
