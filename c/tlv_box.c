/*
 *  COPYRIGHT NOTICE  
 *  Copyright (C) 2015, Jhuster, All Rights Reserved
 *  Author: Jhuster(lujun.hust@gmail.com)
 *  
 *  https://github.com/Jhuster/TLV
 *   
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation; either version 2.1 of the License, 
 *  or (at your option) any later version.
 */
#include <stdio.h>
#include <string.h>
#include "tlv_box.h"

static void tlv_box_release_tlv(value_t value)
{
    tlv_t *tlv = (tlv_t *)value.value;
    port_free(tlv);
}

tlv_box_t *tlv_box_create()
{
    tlv_box_t* box = (tlv_box_t*)port_malloc(sizeof(tlv_box_t));
    box->m_list = key_list_create(tlv_box_release_tlv);
    box->m_serialized_buffer = NULL;
    box->m_serialized_bytes = 0;
    return box;
}

tlv_box_t *tlv_box_parse(unsigned char *buffer, int buffersize)
{
    tlv_box_t *box = tlv_box_create();

   // unsigned char *cached = (unsigned char*) port_malloc(buffersize);
   // memcpy(cached, buffer, buffersize);

    int offset = 0, length = 0;
    while (offset < buffersize) {
        type_t type ;
		set_type(type ,buffer + offset);
        offset += sizeof(type_t);
        length_t length ;
		set_length(length, buffer + offset);
        offset += sizeof(length_t);
        tlv_box_putobject(box, type, buffer+offset, length);
        offset += length;
    }

    box->m_serialized_buffer = 0;
    box->m_serialized_bytes = 0;

    return box;
}

int tlv_box_destroy(tlv_box_t *box)
{
    key_list_destroy(box->m_list);
    if (box->m_serialized_buffer != NULL) {
        port_free(box->m_serialized_buffer);        
    }

    port_free(box);

    return 0;
}

unsigned char *tlv_box_get_buffer(tlv_box_t *box)
{
    return box->m_serialized_buffer;
}

int tlv_box_get_size(tlv_box_t *box)
{
    return box->m_serialized_bytes;
}

int tlv_box_putobject(tlv_box_t *box, type_t type, void *value, length_t length)
{
    if (box->m_serialized_buffer != NULL) {
        return -1;
    }

    tlv_t *tlv = (tlv_t *)port_malloc(sizeof(tlv_t)+length);
    tlv->type = type;
    tlv->length = length;
    memcpy(tlv->value, value, length);

    value_t object;
    object.value = tlv;

    if (key_list_add(box->m_list, type, object) != 0) {
        return -1;
    }    
    box->m_serialized_bytes += sizeof(type_t) + sizeof(length_t) + length;
    
    return 0;
}

int tlv_box_put_char(tlv_box_t *box, type_t type, char value)
{
    return tlv_box_putobject(box, type, &value, sizeof(char));
}

int tlv_box_put_short(tlv_box_t *box, type_t type, short value)
{
    return tlv_box_putobject(box, type, &value, sizeof(short));
}

int tlv_box_put_int(tlv_box_t *box, type_t type, int value)
{
    return tlv_box_putobject(box, type, &value, sizeof(int));
}

int tlv_box_put_long(tlv_box_t *box, type_t type, long value)
{
    return tlv_box_putobject(box, type, &value, sizeof(long));
}

int tlv_box_put_longlong(tlv_box_t *box, type_t type, long long value)
{
    return tlv_box_putobject(box, type, &value, sizeof(long long));
}

int tlv_box_put_float(tlv_box_t *box, type_t type, float value)
{
    return tlv_box_putobject(box, type, &value, sizeof(float));
}

int tlv_box_put_double(tlv_box_t *box, type_t type, double value)
{
    return tlv_box_putobject(box, type, &value, sizeof(double));
}

int tlv_box_put_string(tlv_box_t *box, type_t type, char *value)
{
    return tlv_box_putobject(box, type, value, strlen(value)+1);
}

int tlv_box_put_bytes(tlv_box_t *box, type_t type, unsigned char *value, length_t length)
{
    return tlv_box_putobject(box, type, value, length);
}

int tlv_box_put_object(tlv_box_t *box, type_t type, tlv_box_t *object)
{
    return tlv_box_putobject(box, type, tlv_box_get_buffer(object), tlv_box_get_size(object));
}

int tlv_box_serialize(tlv_box_t *box)
{
    if (box->m_serialized_buffer != NULL) {
        return -1;
    }

    int offset = 0;
    unsigned char* buffer = (unsigned char*) port_malloc(box->m_serialized_bytes);    
    key_list_foreach(box->m_list, node) {
        tlv_t *tlv = (tlv_t *) node->value.value;        
        memcpy(buffer+offset, &tlv->type, sizeof(type_t));
        offset += sizeof(type_t);        
        memcpy(buffer+offset, &tlv->length, sizeof(length_t));
        offset += sizeof(length_t);
        memcpy(buffer+offset, tlv->value, tlv->length);        
        offset += tlv->length;
    }

    box->m_serialized_buffer = buffer;

    return 0;
}

int tlv_box_get_char(tlv_box_t *box, type_t type, char *value)
{
    value_t object;
    if (key_list_get(box->m_list, type, &object) != 0) {
        return -1;
    }
    tlv_t *tlv = (tlv_t *) object.value;
    *value = (*(char *)(tlv->value));
    return 0;
}

int tlv_box_get_short(tlv_box_t *box, type_t type, short *value)
{
    value_t object;
    if (key_list_get(box->m_list, type, &object) != 0) {
        return -1;
    }
    tlv_t *tlv = (tlv_t *) object.value;
	if( value  ){
		memcpy(value,tlv->value,sizeof( short ) );
	}
    return 0;
}

int tlv_box_get_int(tlv_box_t *box, type_t type, int *value)
{
    value_t object;
    if (key_list_get(box->m_list, type, &object) != 0) {
        return -1;
    }
    tlv_t *tlv = (tlv_t *) object.value;
	if( value  ){
		memcpy(value,tlv->value,sizeof( int ) );
	}
    return 0;
}

int tlv_box_get_long(tlv_box_t *box, type_t type, long *value)
{
    value_t object;
    if (key_list_get(box->m_list, type, &object) != 0) {
        return -1;
    }
    tlv_t *tlv = (tlv_t *) object.value;
	if( value  ){
		memcpy(value,tlv->value,sizeof( long ) );
	}
    return 0;
}

int tlv_box_get_longlong(tlv_box_t *box, type_t type, long long *value)
{
    value_t object;
    if (key_list_get(box->m_list, type, &object) != 0) {
        return -1;
    }
    tlv_t *tlv = (tlv_t *) object.value;
	if( value  ){
		memcpy(value,tlv->value,sizeof( long long ) );
	}
    return 0;
}

int tlv_box_get_float(tlv_box_t *box, type_t type, float *value)
{
    value_t object;
    if (key_list_get(box->m_list, type, &object) != 0) {
        return -1;
    }
    tlv_t *tlv = (tlv_t *) object.value;
	if( value  ){
		memcpy(value,tlv->value,sizeof( float) );
	}
    return 0;
}

int tlv_box_get_double(tlv_box_t *box, type_t type, double *value)
{
    value_t object;
    if (key_list_get(box->m_list,type,&object) != 0) {
        return -1;
    }
    tlv_t *tlv = (tlv_t *) object.value;
	if( value  ){
		memcpy(value,tlv->value,sizeof( double) );
	}
    return 0;
}

int tlv_box_get_string(tlv_box_t *box, type_t type, char *value, length_t* length)
{
    return tlv_box_get_bytes(box,type,value,length);
}

int tlv_box_get_bytes(tlv_box_t *box, type_t type, unsigned char *value, length_t* length)
{
    value_t object;
    if (key_list_get(box->m_list,type,&object) != 0) {
        return -1;
    }
    tlv_t *tlv = (tlv_t *) object.value;
    if (*length < tlv->length) {
        return -1;
    }
    memset(value, 0, *length);
    *length = tlv->length;
    memcpy(value, tlv->value, tlv->length);
    return 0;
}

int tlv_box_get_bytes_ptr(tlv_box_t *box, type_t type, unsigned char **value, length_t* length)
{
    value_t object;
    if (key_list_get(box->m_list, type, &object) != 0) {
        return -1;
    }
    tlv_t *tlv = (tlv_t *) object.value;
    *value = tlv->value;
    *length = tlv->length;
    return 0;
}

int tlv_box_get_object(tlv_box_t *box, type_t type, tlv_box_t **object)
{
    value_t value;
    if (key_list_get(box->m_list, type, &value) != 0) {
        return -1;
    }
    tlv_t *tlv = (tlv_t *) value.value;
    *object = (tlv_box_t *)tlv_box_parse(tlv->value, tlv->length);
    return 0;
}
