/*-
 * Copyright (c) 2003 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_CONSTR_SET_OF_H_
#define	_CONSTR_SET_OF_H_

#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct asn_SET_OF_specifics_s {
	/*
	 * Target structure description.
	 */
	int struct_size;	/* Size of the target structure. */
	int ctx_offset;		/* Offset of the asn_struct_ctx_t member */

	/* XER-specific stuff */
	int as_XMLValueList;	/* The member type must be encoded like this */
} asn_SET_OF_specifics_t;

/*
 * A set specialized functions dealing with the SET OF type.
 */
LIB61850_INTERNAL asn_struct_free_f SET_OF_free;
LIB61850_INTERNAL asn_struct_print_f SET_OF_print;
LIB61850_INTERNAL asn_constr_check_f SET_OF_constraint;
LIB61850_INTERNAL ber_type_decoder_f SET_OF_decode_ber;
LIB61850_INTERNAL der_type_encoder_f SET_OF_encode_der;
LIB61850_INTERNAL xer_type_decoder_f SET_OF_decode_xer;
LIB61850_INTERNAL xer_type_encoder_f SET_OF_encode_xer;
LIB61850_INTERNAL per_type_decoder_f SET_OF_decode_uper;
LIB61850_INTERNAL per_type_encoder_f SET_OF_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _CONSTR_SET_OF_H_ */
