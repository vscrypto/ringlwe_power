/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * See LICENSE for complete information.
 */

/*
Various MACROS for modular reduction.
Note that q and n are defined in rlwe_params.h
*/

#ifndef _MACROS_H_
#define _MACROS_H_


#define ADD_MODq(x, a, b) \
do {\
		uint32_t _T = ((uint32_t) (a)) + ((uint32_t) (b));\
		_T -= (_T >= q) ? q : 0;\
		x = (uint16_t) (_T);\
} while (0)

#define SUB_MODq(x, a, b) \
do {\
                uint32_t _T = ((uint32_t) (a)) + ((uint32_t) (q-(b)));	\
		_T -= (_T >= q) ? q : 0;\
		x = (uint16_t) (_T);\
} while (0)

#define MUL_MODq(x, a, b) \
do {\
		uint32_t _T = ((uint32_t) (a)) * ((uint32_t) (b));\
		_T = _T % q;\
		x = (uint16_t) (_T);\
} while (0)

/*Needed for indexing in the FFT*/
#define SUB_MODn(x, a, b) \
do {\
		uint32_t _T = ((uint32_t) (a)) + ((uint32_t) (n-(b)));\
		_T -= (_T >= n) ? n : 0;\
		x = (uint16_t) (_T);\
} while (0)

/*v = e0*b, multiply and add in the ring. All done in the FFT / CRT domain, so point-wise multiplication and addition*/
#define POINTWISE_MUL(v, b, e0) \
do {\
	for (uint16_t _i = 0; _i < n; ++_i) {\
		MUL_MODq((v)[_i], (e0)[_i], (b)[_i]);\
	}\
} while(0)

/*v = e0+b, multiply and add in the ring. All done in the FFT / CRT domain, so point-wise multiplication and addition*/
#define POINTWISE_ADD(v, b, e0) \
do {\
	for (uint16_t _i = 0; _i < n; ++_i) {\
		ADD_MODq((v)[_i], (e0)[_i], (b)[_i]);\
	}\
} while(0)


/*v = e0*b+e1, multiply and add in the ring. All done in the FFT / CRT domain, so point-wise multiplication and addition*/
#define POINTWISE_MUL_ADD(v, b, e0, e1) \
do {\
	for (uint16_t _i = 0; _i < n; ++_i) {\
		MUL_MODq((v)[_i], (e0)[_i], (b)[_i]);\
		ADD_MODq((v)[_i], (v)[_i], (e1)[_i]);\
	}\
} while(0)



#endif /* _MACROS_H_ */




