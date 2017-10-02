/*
 * library.h
 *
 * Created: 11-Sept-17 12:37:59
 * Author : hydroflax
 */ 


#ifndef TINYJSONPP_H_
#define TINYJSONPP_H_

#include "stdlib.h"

//------------------------------------------------------------------------------------------------------------------------
//             Struct Definitions
//------------------------------------------------------------------------------------------------------------------------
struct Key;
struct Value;

typedef struct Key {
	char* start;       // A pointer to the start of the key (ignoring quotation marks).
	unsigned int size; // The amount of characters the key has.
	Key* parent;       // The parent of the key. (if it has one).
	Value* value;      // A pointer to the start of the value that this key has.
} Key;

typedef struct Value {
	char* start;       // A pointer to the start of the value (ignoring quotation marks).
	unsigned int size; // The amount of characters the value has.
	char type;         // The value type (int => 'i', string => 's', char => 'c', bool => 'b', array => 'a',
	                   // embedded JSON obj => 'e', null => 'n').
	Key* key;          // A pointer to this values key.
} Value;

//------------------------------------------------------------------------------------------------------------------------
//             TinyJSONpp
//------------------------------------------------------------------------------------------------------------------------

class tinyjsonpp {
protected:
	//--------------------------------------------------------------------------------------------------------------------
	//         Variables
	//--------------------------------------------------------------------------------------------------------------------

	char* json;                // The JSON string.
	unsigned int maxSize;      // The maximum allowable size of the JSON string.
	unsigned int jsonSize;     // The size of the json string.

	//--------------------------------------------------------------------------------------------------------------------
	//         Methods
	//--------------------------------------------------------------------------------------------------------------------
public:
	/**
	 * Constructor
	 *
	 * @param bool store                - Store in IRAM or in EEPROM (Depending on purpose). (1 = EEPROM, 0 = IRAM).
	 * @param unsigned int maxSize	    - Specify a maximum size that will be used by the JSON string (if known). If not 
	 *                                    chosen, the string will be of variable size (need to check to ensure string will
	 *                                    be stored in a row).
	 */
	tinyjsonpp(bool store, unsigned int maxSize);

	/**
	 * Full Constructor
	 *
	 * @param bool store                - Store in IRAM or in EEPROM (Depending on purpose). (1 = EEPROM, 0 = IRAM).
	 * @param bool ignoreQuotes         - Ignore quotation marks, saving space in memory. (To Be Implemented at a later 
	 *                                    date).
	 * @param unsigned int maxSize	    - Specify a maximum size that will be used by the JSON string (if known). If not 
	 *                                    chosen, the string will be of variable size (need to check to ensure string will
	 *                                    be stored in a row).
	 */
	tinyjsonpp(bool store, bool ignoreQuotes, unsigned int maxSize);
	
	/** 
	 * Construct New Object.
	 *
	 * @param size_t objsize            - The size of the tinyjsonpp object.
	 * @return void
	 */
	void* operator new(size_t objsize);

	/** 
	 * Delete Object.
	 *
	 * @param void* obj                 - The tinyjsonpp object to delete.
	 * @return void
	 */
	void operator delete(void* obj);

	/** 
	 * Empties the current JSON string.
	 *
	 * @return void
	 */
	void empty();

	/** 
	 * Resets the current JSON string to an empty JSON string.
	 *
	 * @return void
	 */
	void reset();

	/**
	 * Read from buffer and store JSON string in either IRAM or EEPROM.
	 * During the read, remove any whitespace, /r, /n characters to ensure the smallest possible stored JSON. (Potential 
	 * to remove the quotation marks as well). The character from the buffer will be added to the end of the JSON string.
	 *
	 * @param char* c                   - The character currently being passed by the buffer.
	 * @return void
	 */ 
	void addCharToJSONString(const char c);

	/** 
	 * Gets the value from the JSON string provided a key.
	 * Only the first value in the complete JSON string with the matching key will be found and placed in the value 
	 * property. 
	 *
	 * @param char* key                 - The key to find the value of (without quotation marks).
	 * @return Value                    - The value of key or NULL if none was found.
	 */
	Value getValue(const char* key) const;

	/**
	 * Gets the value from the JSON string provided a key.
	 * Only the first value in the complete JSON string with the matching key will be found and placed in the value 
	 * property.
	 *
	 * @param char* key                 - The key to find the value of (without quotation marks).
	 * @param char* parent              - The parent structure to find. This should be in form "<parent>/<parent>".
	 * @return Value                    - The value of key or NULL if none was found.
	 */
	Value getValue(const char* key, const char* parent) const;

	/**
	 * Inserts the key value pair into the JSON string in the root JSON object.
	 *
	 * @param char* key                 - The key to insert.
	 * @param char* value               - The value to insert.
	 */
	//void insert(const char* key, const char* value);
	
	/**
	 * Inserts the key value pair into the JSON string in the JSON object provided a parent schema.
	 *
	 * @param char* key                 - The key to insert.
	 * @param char* value               - The value to insert.
	 * @param char* parent              - The parent structure to insert in to. This should be in form "<parent>/<parent>".
	 */
	void insert(const char* key, const char* value, const char* parent);

	/** 
	 * Gets a character from the JSON string based on the integer provided.
	 *
	 * @param unsigned int i            - The index in the JSON string to return.
	 * @return char                     - The char at that index position.
	 */
	char getChar(const unsigned int i);

	/** 
	 * Returns a pointer to the JSON string.
	 *
	 * @return const char* const        - A pointer to the JSON string.
	 */
	const char* const getJSONString();

	/** 
	 * Converts a Value in the JSON String to a int. Will return zero if the value is not an int.
	 *
	 * @param Value v                   - The value to convert to an int.
	 * @return unsigned int             - The value as an integer.
	 */
	unsigned int convertValueToInt(const Value v) const;

	/**
	 * Returns the current size of the JSON string.
	 *
	 * @return unsigned int             - The size of the JSON string.
	 */ 
	unsigned int getJSONSize() const;

protected:
	/**
	 * Helper function used to search for the value within a provided memory search range.
	 *
	 * @param char* key                 - The key to find the value of (without quotation marks).
	 * @param char* searchStart         - The starting place to find the key in.
	 * @param unsigned int searchSize   - The length of the 'search' string.
	 * @return Value                    - The value of key or NULL if none was found.
	 */
	Value getValue(const char* key, const char* searchStart, const unsigned int searchSize) const;
};

#endif /* TINYJSONPP_H_ */
