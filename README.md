# TinyJSONpp [![Build Status](https://travis-ci.org/hydroflax/tinyjsonpp.svg?branch=master)](https://travis-ci.org/hydroflax/tinyjsonpp)

A JSON CPP library for use in memory constraint environments. 
This library was originally designed for use with the ATtiny841.

**This library is still in development. A 'broken' release is available but please ensure the functionality you require is implemented correctly and that it is the correctly built library for your device.**

## Featured Bugs/Limitations ##

[*Please see issue tab for more information on potential bugs not listed here.*](https://github.com/hydroflax/tinyjsonpp/issues)

- The library is only able to deal with single embedded JSON objects/arrays.
- Only the first of a given key will be found by getvalue().

## Considerations ##

TinyJSONpp at maximum will have a memory footprint of _ bytes. Please take into consideration the length of the JSON string that needs to be stored in the heap and ensure that the stack will not overflow into the heap.

## Usage ##

This section will be updated over time.<br />
[The Wiki will eventually contain all usage information and examples.](https://github.com/hydroflax/tinyjsonpp/wiki)

## Device Support ##

As of Release v0.1.6-alpha, the libtinyjsonpp.a file and .h can be used with the following device family.

### avr25 ###

ata5272, ata6616c, attiny13, attiny13a, attiny2313, attiny2313a, attiny24, attiny24a, attiny25, attiny261, attiny261a, attiny43u, attiny4313, attiny44, attiny44a, attiny441, attiny45, attiny461, attiny461a, attiny48, attiny828, attiny84, attiny84a, attiny841, attiny85, attiny861, attiny861a, attiny87, attiny88, at86rf401

**More device support will be added in due course.**

## To Do ##

- Create unit tests and integrate with CI (Build the library for other devices, zip up the library for each device family).
- Create usage documentation.
- Convert the current library in C code as most embedded programming is done using C.
- Make an extended (inherited) smallJSONpp class that extends the base functionality of the library for users who have a less constrained memory environment.
- Update README to contain an accurate memory footprint value.
