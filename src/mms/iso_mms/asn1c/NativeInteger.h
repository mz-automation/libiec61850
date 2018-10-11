/*-
 * Copyright (c) 2004 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
/*
 * This type differs from the standard INTEGER in that it is modelled using
 * the fixed machine type (long, int, short), so it can hold only values of
 * limited length. There is no type (i.e., NativeInteger_t, any integer type
 * will do).
 * This type may be used when integer range is limited by subtype constraints.
 */
#ifndef	_NativeInteger_H_
#define	_NativeInteger_H_

#include <asn_application.h>
#include <INTEGER.h>

#ifdef __cplusplus
extern "C" {
#endif

LIB61850_INTERNAL
extern asn_TYPE_descriptor_t asn_DEF_NativeInteger;

LIB61850_INTERNAL
asn_struct_free_f  NativeInteger_free;

LIB61850_INTERNAL
asn_struct_print_f NativeInteger_print;

LIB61850_INTERNAL
ber_type_decoder_f NativeInteger_decode_ber;

LIB61850_INTERNAL
der_type_encoder_f NativeInteger_encode_der;

LIB61850_INTERNAL
xer_type_decoder_f NativeInteger_decode_xer;

LIB61850_INTERNAL
xer_type_encoder_f NativeInteger_encode_xer;

LIB61850_INTERNAL
per_type_decoder_f NativeInteger_decode_uper;

LIB61850_INTERNAL
per_type_encoder_f NativeInteger_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _NativeInteger_H_ */
