#include "strmod.h"
#include <stdlib.h>

#define SM_END '\0'
#define SM_NUM '0'
#define SM_MIN '-'

SM_INT SM_LEN(SM_RAW rw_text) {
	SM_INT i_size = 0;
	for (i_size = 0; rw_text[i_size] != SM_END; i_size++) {}
	return i_size;
}
SM_LONG SM_TOINT(SM_ALIAS sm_input)
{
	SM_LONG i_value = 0,i_base= 1;
	for (SM_INT i = sm_input.i_length-1; i >=0; i--)
	{
		if (sm_input.c_text[i] == SM_MIN) {
			i_value *= -1;
			continue;
		}
		i_value += (sm_input.c_text[i] - SM_NUM)*i_base;
		i_base *= 10;
	}
	return i_value;
}
SM_ALIAS SM_TOSTRING(SM_LONG i_input)
{
	SM_LONG l_base = 1, l_div = 1,l_current = 0;
	SM_INT i_size = 0,i_negative = 0;
	if (i_input < 0) {
		i_input *= -1;
		i_negative = 1;
	}

	for (i_size = 0; i_input%l_base != i_input; i_size++)l_base *= 10;
	if (i_negative)i_size++;
	SM_RAW  rw_rawtext = malloc(i_size*sizeof(char));

	l_base = 10;
	for (SM_INT i = i_size-1; i >= 0; i--)
	{
		l_current = i_input % l_base;
		l_current /= l_div;
		i_input -= l_current;
		rw_rawtext[i] = SM_NUM + l_current;
		l_base *= 10;
		l_div *= 10;
	}
	rw_rawtext[i_size] = SM_END;
	if (i_negative)rw_rawtext[0] = SM_MIN;
	SM_ALIAS sm_text = { rw_rawtext , i_size };
	return sm_text;
}
SM_ALIAS SM_SET(SM_RAW rw_text) {
	SM_ALIAS sm_return = { rw_text,SM_LEN(rw_text) };
	return sm_return;
}
SM_INT SM_APPEND(SM_ALIAS * sm_text, SM_ALIAS sm_input) {
	return SM_PUSH(sm_text, sm_input, sm_text->i_length);
}
SM_INT SM_INFRONT(SM_ALIAS * sm_text, SM_ALIAS sm_input) {
	return SM_PUSH(sm_text, sm_input, 0);
}
SM_INT SM_PUSH(SM_ALIAS * sm_text, SM_ALIAS sm_input, SM_INT i_index) {
	if ((i_index <= 0 && i_index >= sm_text->i_length)||sm_input.i_length<=0)return 0;
	SM_INT i_size = sm_text->i_length + sm_input.i_length;
	SM_ALIAS sm_temp = *sm_text;
	sm_text->c_text = malloc(i_size*sizeof(char));
	sm_text->i_length = i_size;
	for (SM_INT i = 0; i < i_size; i++){	
		if (i > i_index) {
			sm_text->c_text[i] = sm_temp.c_text[i- sm_input.i_length];
		}else if (i == i_index){
			for (SM_INT i_sub = 0; i_sub < sm_input.i_length; i_sub++,i++){
				sm_text->c_text[i] = sm_input.c_text[i_sub];
			}
			i--;
		}else{
			sm_text->c_text[i] = sm_temp.c_text[i];
		}
	}
	sm_text->c_text[i_size] = SM_END;
	return SM_TRUE;
}
SM_INT SM_REMOVE(SM_ALIAS * sm_text, SM_INT i_once, SM_INT i_twice)
{
	if ((i_twice - i_once) <0)return 0;
	SM_ALIAS sm_temp = *sm_text;
	sm_text->c_text = malloc((sm_text ->i_length-(i_twice-i_once)) * sizeof(char));
	
	for (SM_INT i = 0; i < sm_text->i_length - (i_twice - i_once); i++)
	{
		if (i >= i_once) {
			sm_text->c_text[i] = sm_temp.c_text[i+ (i_twice - i_once)+1];
		}
		else {
			sm_text->c_text[i] = sm_temp.c_text[i];
		}
	}
	sm_text->i_length = sm_temp.i_length - (i_twice - i_once);
	sm_text->c_text[sm_text->i_length] = SM_END;
	return SM_TRUE;
}
SM_INT SM_REPLACE(SM_ALIAS * sm_text, SM_ALIAS sm_search, SM_ALIAS sm_replace)
{
	SM_INT i_stage = 0,i_found = 0,i_start=0,i_end=0,i_offset = sm_replace.i_length- sm_search.i_length;

	for (SM_INT i = 0; i < sm_text->i_length; i++){
		if (sm_text->c_text[i] == sm_search.c_text[0])i_start = i;
		if (sm_text->c_text[i] == sm_search.c_text[i_stage]) {		
			if (i_stage == sm_search.i_length-1) {
				i_found++;
				i_stage = 0;
				i_end = i;
				i += i_offset;
				SM_REMOVE(sm_text, i_start, i_end);
				SM_PUSH(sm_text, sm_replace, i_start);
				continue;
			}
			i_stage++; 
		}
		else i_stage = 0;
	}
	sm_text->i_length = SM_LEN(sm_text->c_text);
	return i_found;
}
SM_INT SM_SPLIT(SM_ALIAS sm_text, SM_ALIAS* sm_text_left, SM_ALIAS* sm_text_right, SM_INT i_index) {
	if (i_index >= sm_text.i_length || i_index * -1 >= sm_text.i_length || i_index == 0)return 0;
	sm_text_left->i_length = i_index*-1+1;
	sm_text_right->i_length = sm_text.i_length-i_index*-1-1;
	if (i_index > 0) 
	{
		sm_text_left->i_length = i_index;
		sm_text_right->i_length = sm_text.i_length - i_index;
	}

	sm_text_left->c_text = malloc(sizeof(char)*sm_text_left->i_length);
	sm_text_right->c_text = malloc(sizeof(char)*sm_text_right->i_length);

	for (SM_INT i = 0; i < sm_text_left->i_length; i++)
	{
		sm_text_left->c_text[i] = sm_text.c_text[i];
	}
	for (SM_INT i = 0; i < sm_text_right->i_length; i++)
	{
		sm_text_right->c_text[i] = sm_text.c_text[i+ sm_text_left->i_length];
	}

	sm_text_left->c_text[sm_text_left->i_length] = SM_END;
	sm_text_right->c_text[sm_text_right->i_length] = SM_END;

	return SM_TRUE;
}
SM_INT SM_CUT(SM_ALIAS * sm_text, SM_ALIAS sm_search)
{
	SM_INT i_stage = 0, i_found = 0, i_start = 0, i_end = 0 ;

	for (SM_INT i = 0; i < sm_text->i_length; i++) {
		if (sm_text->c_text[i] == sm_search.c_text[0])i_start = i;
		if (sm_text->c_text[i] == sm_search.c_text[i_stage]) {
			if (i_stage == sm_search.i_length - 1) {
				i_found++;
				i_stage = 0;
				i_end = i;

				SM_REMOVE(sm_text, i_start, i_end);

				break;
			}
			i_stage++;
		}
		else i_stage = 0;
	}
	sm_text->i_length = SM_LEN(sm_text->c_text);
	return i_found;
}
SM_INT SM_COUNT(SM_ALIAS sm_text, SM_ALIAS sm_search)
{
	SM_INT i_stage = 0, i_found = 0;

	for (SM_INT i = 0; i < sm_text.i_length; i++) {
		if (sm_text.c_text[i] == sm_search.c_text[i_stage]) {
			if (i_stage == sm_search.i_length - 1) {
				i_found++;
				break;
			}
			i_stage++;
		}
		else i_stage = 0;
	}
	return i_found;
}