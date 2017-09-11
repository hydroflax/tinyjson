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

void tinyjsonpp::parse(char c) {
	// Ignore Newline, Space, Return Carriage Characters.
	if (c <= 0x20) {
		return;
	}

	this->json[this->location] = c;
	++this->location;
}

void tinyjsonpp::empty() {
	this->location = 0;
	memset(this->json, 0, this->jsonSize);
	// Alternative to save compute time is to just set the first character to 0.
}