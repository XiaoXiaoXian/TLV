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
#ifndef _TLV_BOX_H_
#define _TLV_BOX_H_

#include "key_list.h"
#include <stdint.h>

typedef uint8_t type_t;

#define get_type(ptype ) (*((uint8_t*)ptype))
#define set_type( type_x,pval ) (type_x=*(uint8_t*)(pval))

typedef uint8_t length_t;

#define get_length( plength ) (*((uint8_t*)plength))
#define set_length( length,pval ) (length=*(uint8_t*)(pval))

typedef struct _tlv {
    type_t type;
    length_t length;
    uint8_t value[0];
} tlv_t;

typedef struct _tlv_box {
    key_list_t *m_list;
    unsigned char *m_serialized_buffer;
    int m_serialized_bytes;
} tlv_box_t;

tlv_box_t *tlv_box_create();
tlv_box_t *tlv_box_parse(unsigned char *buffer,int buffersize);
int tlv_box_destroy(tlv_box_t *box);

unsigned char *tlv_box_get_buffer(tlv_box_t *box);
int tlv_box_get_size(tlv_box_t *box);

int tlv_box_put_char(tlv_box_t *box,type_t type,char value);
int tlv_box_put_short(tlv_box_t *box,type_t type,short value);
int tlv_box_put_int(tlv_box_t *box,type_t type,int value);
int tlv_box_put_long(tlv_box_t *box,type_t type,long value);
int tlv_box_put_longlong(tlv_box_t *box,type_t type,long long value);
int tlv_box_put_float(tlv_box_t *box,type_t type,float value);
int tlv_box_put_double(tlv_box_t *box,type_t type,double value);
int tlv_box_put_string(tlv_box_t *box,type_t type,char* value);
int tlv_box_put_bytes(tlv_box_t *box,type_t type,unsigned char *value,length_t length);
int tlv_box_put_object(tlv_box_t *box,type_t type,tlv_box_t *object);
int tlv_box_serialize(tlv_box_t *box);

int tlv_box_get_char(tlv_box_t *box,type_t type,char *value);
int tlv_box_get_short(tlv_box_t *box,type_t type,short *value);
int tlv_box_get_int(tlv_box_t *box,type_t type,int *value);
int tlv_box_get_long(tlv_box_t *box,type_t type,long *value);
int tlv_box_get_longlong(tlv_box_t *box,type_t type,long long *value);
int tlv_box_get_float(tlv_box_t *box,type_t type,float *value);
int tlv_box_get_double(tlv_box_t *box,type_t type,double *value);
int tlv_box_get_string(tlv_box_t *box,type_t type,char *value,length_t* length);
int tlv_box_get_bytes(tlv_box_t *box,type_t type,unsigned char *value,length_t* length);
int tlv_box_get_bytes_ptr(tlv_box_t *box,type_t type,unsigned char **value,length_t* length);
int tlv_box_get_object(tlv_box_t *box,type_t type,tlv_box_t **object);

#endif //_TLV_BOX_H_
