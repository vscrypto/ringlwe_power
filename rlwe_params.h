/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * See LICENSE for complete information.
 */

#ifndef _PARAMS_H_
#define _PARAMS_H_


#if MAIN
#define EXTERN
#else
#define EXTERN extern
#endif

#if !defined(N1024) && !defined(N512) && !defined(N256)
	#error "Have not specified N = 1024, or N = 512, or N = 256 on compile line"
#endif

#ifdef N1024
#define N 1024
#endif
#ifdef N512
#define N 512
#endif
#ifdef N256
#define N 256
#endif


/*Parameters*/
EXTERN const uint16_t n, q, qmod4, B, BB, LOG2B, BMASK, nwords;

/*Quadrants defining I_0, I_'1, I'_0, I_1*/
EXTERN const uint16_t q_1_4, q_2_4, q_3_4;

/*Define intervals for rec routine*/
EXTERN const uint16_t r0_u, r0_l, r1_u, r1_l;

/*[-B..B] modulo q*/
EXTERN const uint16_t small_coeff_table[11];

/*N-th roots of unity w^i*/
EXTERN const uint16_t W[N];

/*N-th roots of unity w^(-i)*/
EXTERN const uint16_t W_rev[N];

/*2N-th roots of unity w^(i/2). Needed for negacyclic convolution.*/
EXTERN const uint16_t W_sqrt[N];

/*2N-th roots of unity w^(-i/2). Needed for negacyclic convolution.*/
EXTERN const uint16_t W_sqrt_rev_div_N[N];

/*Public parameter a. Note that this is held in the FFT / CRT basis.*/
EXTERN const uint16_t a[N];

#endif

