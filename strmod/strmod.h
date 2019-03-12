#pragma once

#define SM_TRUE 0xFF
#define SM_FALSE 0x00

#define SM_ALIAS	string
#define SM_INT		short
#define SM_LONG		int
#define SM_RAW		char*


typedef struct SM_ALIAS SM_ALIAS;
struct SM_ALIAS {
	SM_RAW c_text;
	int i_length;
};

//Pushes a string into another string to a certain position. 
SM_INT		SM_PUSH(SM_ALIAS * sm_text, SM_ALIAS sm_input, SM_INT i_index);

//Appends a String to another string.(Derived from SM_PUSH)
SM_INT		SM_APPEND(SM_ALIAS * sm_text, SM_ALIAS sm_input); 

//Places a string in front of the first string.(Derived from SM_PUSH)
SM_INT		SM_INFRONT(SM_ALIAS * sm_text, SM_ALIAS sm_input);

//Sets the SM_ALIAS datatype with C-Type string.
SM_ALIAS	SM_SET(SM_RAW rw_text);

//Used to get number of chars in a string.
SM_INT		SM_LEN(SM_RAW rw_text);

//Returns the value of the string in the integer format (negative and positive)
SM_LONG		SM_TOINT(SM_ALIAS sm_input);

//Returns the string with the given value of the integer
SM_ALIAS	SM_TOSTRING(SM_LONG i_input);

//Removes a certain part of the string. Note that the indices are also getting removed.
SM_INT		SM_REMOVE(SM_ALIAS * sm_text, SM_INT i_once, SM_INT i_twice);

//Replaces a word with another word.(Extension of SM_REMOVE and SM_PUSH)
SM_INT		SM_REPLACE(SM_ALIAS * sm_text, SM_ALIAS sm_search, SM_ALIAS sm_replace);

//Positive index means the index is on the right string and a negative index means the index is on the left side.
SM_INT		SM_SPLIT(SM_ALIAS sm_text, SM_ALIAS* sm_text_left, SM_ALIAS* sm_text_right, SM_INT i_index);

//Removes a string from another string.
SM_INT		SM_CUT(SM_ALIAS * sm_text, SM_ALIAS sm_search);

//Counts how many times a string is contained in a other string.
SM_INT		SM_COUNT(SM_ALIAS sm_text, SM_ALIAS sm_search);