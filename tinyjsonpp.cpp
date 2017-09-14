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

void tinyjsonpp::getValue(char* key, char* searchStart, unsigned int searchSize)
{
	clearKeyValue();
	this->key.start = static_cast<char*>(memmem(searchStart, searchSize, key, strlen(key)));

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
		bool embeddedJSON = false;
		while(!valueFound) {

			// Start of Value after ':"' or ':['
			if(this->json[location] == ':' && !embeddedJSON) {
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
					embeddedJSON = true;
					this->value.type = 'e';
				} else {
					this->value.type = 's';
				}
			}

			if(this->json[location] == ',' || (this->json[location] == '}' && !embeddedJSON)) {
				if(!array) {
					this->value.size = &this->json[location - 1] - this->value.start;
					valueFound = true;
				}
			}

			// Embedded JSON
			if(this->json[location] == '}' && embeddedJSON) {
				this->value.size = &this->json[location] - this->value.start;
				valueFound = true;
			}

			// Array
			if(this->json[location] == ']') {
				this->value.size = &this->json[location] - this->value.start;
				valueFound = true;
			}

			++location;
		}
	} else {
		clearKeyValue();
	}
}


Value tinyjsonpp::getValue(char* key) {
	getValue(key, this->json, this->jsonSize);
	return this->value;
}

Value tinyjsonpp::getValue(char* key, char* parent) {
	clearKeyValue();

	// Set initial memmem string search parameters.
	this->value.start = this->json;
	this->value.size = this->jsonSize;

	char* originalParent = static_cast<char*>(calloc(strlen(parent), sizeof(char)));
	originalParent = strcpy(originalParent, parent);

	char *token = strtok(originalParent, "/");
	while (token != NULL) {
		this->key.start = static_cast<char*>(memmem(this->value.start, this->value.size, token, strlen(token)));

		// The key was found in the JSON.
		if (this->key.start) {
			// Set the correct values.
			this->location = this->key.start - this->json;
			this->value.start = &this->json[location + 2];

			// Faking the value size. Good enough for simple cases.
			this->value.size = this->jsonSize - (location + 2);
		}
		token = strtok(NULL, "/");
	}

	// Standard getValue() can be called passing the key to be found along with the search range.
	getValue(key, this->value.start, this->value.size);
	free(originalParent);

	return this->value;
}

void tinyjsonpp::insert(char* key, char* value) {
	clearKeyValue();
	// Insert the key-value pair into the root JSON object.
}

void tinyjsonpp::insert(char* key, char* value, char* parent) {
	clearKeyValue();
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
	char* originalParent = static_cast<char*>(calloc(strlen(parent), sizeof(char)));
	originalParent = strcpy(originalParent, parent);
	originalParent[forwardSlash] = 0;

	// Creating a temporary value as if the key not in the value, this will be set to NULL.
	Value val = getValue(&originalParent[forwardSlash + 1], originalParent);

	// TODO: WRITE;
	// If the key is already in the JSON string then just change the value, else add the k-v pair to the JSON string.
	getValue(key, this->value.start, this->value.size);

	// The key does not already exists in the object.
	if (this->key.start == NULL) {
		// The space which will be freed up. (i.e. "<key>":"x").
		location = val.start - this->json + strlen(key) + 6;

		// If the key is the first to be inserted into the parent then no need for ,
		if(val.size > 1) {
			++location;
		}

		// Move original information to make space for new key.		
		memmove(&this->json[location], val.start, (val.start - this->json));

		location = val.start - this->json;
		this->json[location] = '"';
		memcpy(&this->json[location + 1], key, strlen(key));
		this->json[location + strlen(key) + 1] = '"';
		this->json[location + strlen(key) + 2] = ':';
		this->json[location + strlen(key) + 3] = '"';
		this->json[location + strlen(key) + 4] = 'x';
		this->json[location + strlen(key) + 5] = '"';
		
		if(val.size > 1) {
			this->json[location + strlen(key) + 6] = ',';
		}

		this->key.start = &this->json[location + 1];
		this->key.size = strlen(key);
		this->value.start = &this->json[location + strlen(key) + 4];
		this->value.size = 1;

		// Set the values key pointer (and vice versa).
		this->value.key = &this->key;
		this->key.value = &this->value;
	}
	// The key previously existed in the object/it does now.
	// See what the length of the new value is. strlen(value)
	// See what length of old value was. this->value.size
	// Move original information to make space for new value.
	location = this->value.start - this->json + strlen(value);

	// +2 for the "" or {} or [].
	memmove(&this->json[location], &this->json[this->value.start - this->json + this->value.size], (this->value.start - this->json));

	location = this->value.start - this->json;
	// Replace old value with new value.
	memcpy(&this->json[location], value, strlen(value));

	// If a [] or {} no need for "".
	if (value[0] == '[') {
		this->json[location - 1] = '[';
		this->json[location + strlen(value)] = ']';
	} else if(value[0] == '{') {
		this->json[location - 1] = '{';
		this->json[location + strlen(value)] = '}';
	} else {
		this->json[location - 1] = '"';
		this->json[location + strlen(value)] = '"';
	}
	this->jsonSize = strlen(this->json);

	free(originalParent);
}

void tinyjsonpp::clearKeyValue()
{
	this->key.start = NULL;
	this->key.size = 0;
	this->key.parent = NULL;
	this->key.value = NULL;
	this->value.start = NULL;
	this->value.size = 0;
	this->value.type = 'n';
	this->value.key = NULL;
}
