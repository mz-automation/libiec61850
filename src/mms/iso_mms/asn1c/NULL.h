/*-
 * Copyright (c) 2003 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	ASN_TYPE_NULL_H
#define	ASN_TYPE_NULL_H

#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The value of the NULL type is meaningless: see BOOLEAN if you want to
 * carry true/false semantics.
 */
typedef int NULL_t;

LIB61850_INTERNAL extern asn_TYPE_descriptor_t asn_DEF_NULL;

LIB61850_INTERNAL asn_struct_print_f NULL_print;
LIB61850_INTERNAL der_type_encoder_f NULL_encode_der;
LIB61850_INTERNAL xer_type_decoder_f NULL_decode_xer;
LIB61850_INTERNAL xer_type_encoder_f NULL_encode_xer;
LIB61850_INTERNAL per_type_decoder_f NULL_decode_uper;
LIB61850_INTERNAL per_type_encoder_f NULL_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* NULL_H */
