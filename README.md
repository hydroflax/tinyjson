# TinyJSONpp #

A JSON CPP library for use in memory constraint environments. 
This library was originally designed for use with the ATtiny841. 

**This library is still in development. A 'broken' release is scheduled on completion of insert functionality as another project I am 
working on requires this library.**

## Featured Bugs/Limitations ##

[*Please see issue tab for more information on potential bugs not listed here.*](https://github.com/hydroflax/tinyjsonpp/issues)

- Unable to access any of the variables within the class that are not pointers.
- The library is only able to deal with single embedded JSON objects/arrays.
- Only the first of a given key will be found by getvalue(). (This also does not discard

## Considerations ##

TinyJSONpp is allocated to the heap with a memory footprint of _ bytes. Please take into consideration the length of the JSON string that
needs to be stored in the heap and ensure that the stack will not overflow into the heap.

## Usage ##

This section will be updated over time.<br />
[The Wiki will eventually contain all usage information and examples.](https://github.com/hydroflax/tinyjsonpp/wiki)


## To Do ##

- Create unit tests and integrate with CI.
- Create usage documentation.
- Convert the current library in C code as most embedded programming is done using C.
- Make an extended (inherited) smallJSONpp class that extends the base functionality of the library for users who have a less constrained
memory environment.
