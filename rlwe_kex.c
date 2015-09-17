/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * See LICENSE for complete information.
 */

#if !defined(RLWE_RANDOMNESS_USE_OPENSSL_AES) && !defined(RLWE_RANDOMNESS_USE_OPENSSL_RC4) && !defined(RLWE_RANDOMNESS_USE_OPENSSL_RAND) && !defined(RLWE_RANDOMNESS_USE_C_RANDOM_INSECURE)
	#warning "No PRNG specified. Using OpenSSL AES";
	#define RLWE_RANDOMNESS_USE_OPENSSL_AES
#endif


#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#if defined(RLWE_RANDOMNESS_USE_OPENSSL_AES)
#include "rlwe_rand_openssl_aes.c"
#elif defined(RLWE_RANDOMNESS_USE_OPENSSL_RC4)
#include "rlwe_rand_openssl_rc4.c"
#elif defined(RLWE_RANDOMNESS_USE_OPENSSL_RAND)
#include "rlwe_rand_openssl_rand.c"
#elif defined(RLWE_RANDOMNESS_USE_C_RANDOM_INSECURE)
#include "rlwe_rand_c.c"
#else
#error "No randomness generation algorithm defined."
#endif

#include "rlwe_params.h"
#include "rlwe_kex.h"
#include "rlwe_macros.h"
#include "fft.h"


#ifdef UNIFORM
/*
Sample the secret key. Each coefficient uniform in [-B,B].
*/

void sample_secret(uint16_t s[n]) {
	RANDOM_VARS;
	uint16_t i = 0;
	uint64_t r = RANDOM64;
	uint64_t l, shifts = 0;

	while (i < n) {
		l = r & BMASK;
		if (l < BB) {
		/*Take this sample*/
			s[i] = small_coeff_table[l];
			i++;
		}
		/*Shift r along and continue*/
		shifts++;
		if (shifts * LOG2B >= 64) {
			/*Need a new random value*/
			r = RANDOM64;
			shifts = 0;
		}
		else r = (r >> LOG2B);
	}		
}


#endif

#ifdef GAUSSIAN
#include "rlwe_table.h"
#define RANDOM192(c) c[0] = RANDOM64; c[1] = RANDOM64; c[2] = RANDOM64
/* Returns 0 if a >= b
 * Returns 1 if a < b
 * Where a and b are both 3-limb 64-bit integers.
 * This function runs in constant time.
 */
static int cmplt_ct(uint64_t *a, uint64_t *b) {
	int m;
	m = (a[0] >= b[0]);
	m = ((a[1] >= b[1]) && (!(a[1] == b[1]) || m));
	m = ((a[2] >= b[2]) && (!(a[2] == b[2]) || m));
	return (m == 0);
}

static uint32_t single_sample(uint64_t *in) {
	uint32_t lower_index = 0, this_index = 32, upper_index = 64;
	int i;
	for (i = 0; i < 6; i++) {
		if (cmplt_ct(in, rlwe_table[this_index])) {
			upper_index = this_index;
		} else {
			lower_index = this_index;
		}
		this_index = (lower_index + upper_index) / 2;
	}
	return lower_index;
}

void sample_secret(uint16_t s[n]) {
	RANDOM_VARS
	int i, j;
	for (i = 0; i < nwords; i++) {
		uint64_t r = RANDOM64;
		for (j = 0; j < 64; j++) {
			uint64_t rnd[3];
			int32_t m;
			RANDOM192(rnd);
			m = (r & 1);
			r >>= 1;
			m = 2 * m - 1;
			s[i * 64 + j] = single_sample(rnd);
			if (m == -1) {
				s[i * 64 + j] = q - s[i * 64 + j];
			}
		}
	}
}

#endif

/* Round and cross-round */
void round_and_cross_round(uint64_t modular_rnd[nwords], uint64_t cross_rnd[nwords], const uint16_t v[n]) {
	RANDOM_VARS;
	uint16_t i = 0;
	uint64_t r = RANDOM64;
	uint16_t word = 0, pos = 0, rbit = 0, val;

	memset((void *) modular_rnd, 0, nwords*sizeof(uint64_t));
	memset((void *) cross_rnd, 0, nwords*sizeof(uint64_t));

	for (i = 0; i < n; ++i) {
		val = v[i];
		/*Randomize rounding procedure - probabilistic nudge*/
		if (qmod4 == 1) {
			if (val == 0) {
				if (r & 1) val = (q-1);
				rbit++;
				if (rbit >= 64) {
					r = RANDOM64; rbit = 0;
				}
				else r = (r >> 1);
			}
			else if (val == q_1_4-1) {
				if (r & 1) val = q_1_4;
				rbit++;
				if (rbit >= 64) {
					r = RANDOM64; rbit = 0;
				}
				else r = (r >> 1);
			}
		}
		else {
			if (val == 0) {
				if (r & 1) val = (q-1);
				rbit++;
				if (rbit >= 64) {
					r = RANDOM64; rbit = 0;
				}
				else r = (r >> 1);
			}
			else if (val == q_3_4-1) {
				if (r & 1) val = q_3_4;
				rbit++;
				if (rbit >= 64) {
					r = RANDOM64; rbit = 0;
				}
				else r = (r >> 1);
			}
		}

		/*Modular rounding process*/
		if (val >= q_1_4 && val < q_3_4) modular_rnd[word] |= (((uint64_t) 1) << pos);

		/*Cross Rounding process*/
		if ((val >= q_1_4 && val < q_2_4) || val >= q_3_4) cross_rnd[word] |= (((uint64_t) 1) << pos);
		
		pos++;
		if (pos == 64) {
			word++; pos = 0;
		}

	}


}

/* Reconcile */ 
void rec(uint64_t r[nwords], uint16_t w[n], uint64_t b[nwords]) {
	uint16_t i = 0;
	uint16_t word = 0, pos = 0;

	memset((void *) r, 0, nwords*sizeof(uint64_t));

	for (i = 0; i < n; ++i) {
		if ((b[word] >> pos) & ((uint64_t) 1)) {
			if (!( (w[i] < r1_u) || (w[i] > r1_l) )) r[word] |= (((uint64_t) 1) << pos);
		}
		else {
			if (!( (w[i] < r0_u) || (w[i] > r0_l) )) r[word] |= (((uint64_t) 1) << pos);
		}
		pos++;
		if (pos == 64) {
			word++; pos = 0;
		}
	}
}

/* Construct Alice's private / public key pair. Return all elements in the Fourier Domain
 * input:  none
 * output: private key s_1=s[n]...s[2*n-1] in Fourier Domain
 *         noise term s_0=s[0]...s[n-1] in Fourier Domain, not needed again
 *         public key b in Fourier Domain
 */	
void KEM1_Generate(uint16_t s[2*n], uint16_t b[n]) {

	sample_secret(s);	sample_secret(s+n);	
	/*Fourier Transform secret keys*/
	FFT_forward(s); FFT_forward(s+n);
	POINTWISE_MUL_ADD(b, a, s+n, s); //Combine with a to produce s_1*a+s_0 in the Fourier domain. Alice's public key.
}

/* Encapsulation routine. Returns an element in R_q x R_2
 * input:  Alice's public key b in Fourier Domain
 * output: Bob's public key u in Fourier Domain
 *         reconciliation data cr_v
 *         shared secret mu
 */
void KEM1_Encapsulate(uint16_t u[n], uint64_t cr_v[nwords], uint64_t mu[nwords], uint16_t b[n]) {
	uint16_t e[3*n];
	uint16_t v[n];

	/*Sample Bob's ephemeral keys*/	
	sample_secret(e);	sample_secret(e+n); sample_secret(e+2*n);	
	/*Fourier Transform e_0 and e_1*/
	FFT_forward(e); FFT_forward(e+n);
	POINTWISE_MUL_ADD(u, a, e, e+n); //Combine with a to produce e_0*a+e_1 in the Fourier domain. Bob's public key.

	POINTWISE_MUL(v, b, e); //Create v = e_0*b
	FFT_backward(v); //Undo the Fourier Transform
	
	POINTWISE_ADD(v, v, e+2*n); //Create v = e_0*b+e_2
	
	round_and_cross_round(mu, cr_v, v);
}

/* Decapsulation routine.
 * input:  Bob's public key u in Fourier Domain
 *         Alice's private key s_1 in Fourier Domain
 *         reconciliation data cr_v
 * output: shared secret mu
 */
void KEM1_Decapsulate(uint64_t mu[nwords], uint16_t u[n], uint16_t s_1[n], uint64_t cr_v[nwords]) {
	uint16_t w[n];

	POINTWISE_MUL(w, s_1, u); //Create w = s_1*u
	FFT_backward(w); //Undo the Fourier Transform
	
	rec(mu, w, cr_v);

}


