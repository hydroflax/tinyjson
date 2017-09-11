/*
 * tinyjsonpp.cpp
 *
 * Created: 11-Sept-17 12:25:10
 * Author : hydroflax
 */ 

#include <avr/io.h>
#include "tinyjsonpp.h"

tinyjsonpp::tinyjsonpp(bool store, unsigned int maxSize) {

}

tinyjsonpp::tinyjsonpp(bool store, bool ignoreQuotes, unsigned int maxSize) {
	
}

void tinyjsonpp::parse(char* c) {

}

void* tinyjsonpp::operator new(size_t objsize) {
	return malloc(objsize);
}

void tinyjsonpp::operator delete(void* obj) {
	free(obj);
}
