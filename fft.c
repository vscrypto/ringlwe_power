/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * See LICENSE for complete information.
 */

#include <stdio.h>
#include <inttypes.h>

#include "rlwe_params.h"
#include "rlwe_macros.h"
#include "fft.h"

/*
We use Gentleman-Sande, decimation-in-frequency FFT, for the forward fft.
We premultiply x by the 2n'th roots of unity to affect a Discrete Weighted Fourier Transform, 
so when we apply pointwise multiplication we obtain the negacyclic convolution, i.e. multiplication 
modulo x^n+1.
Note that we will not perform the usual scrambling / bit-reversal procedure here because we will invert 
the fourier transform using decimation-in-time.
*/
void FFT_forward(uint16_t x[n]) {
	uint16_t index, step;
	uint16_t i,j,m;
	uint16_t t0,t1;

	//Pre multiplication for DWT
	for (i = 0; i < n; ++i) {
		MUL_MODq(x[i], x[i], W_sqrt[i]);
	}

	step = 1;
	for (m = n>>1; m >= 1; m=m>>1) {
		index = 0;
		for (j = 0 ; j < m; ++j) {
			for (i = j; i < n; i += (m<<1)) {				
				ADD_MODq(t0, x[i], x[i+m]);
				SUB_MODq(t1, x[i], x[i+m]);
				MUL_MODq(x[i+m], t1, W[index]);
				x[i] = t0;
			}
			SUB_MODn(index, index, step);
		}
		step = step << 1;
	}
}



/*
We use Cooley-Tukey, decimation-in-time FFT, for the inverse fft.
We postmultiply x by the inverse of the 2n'th roots of unity * n^-1 to affect a Discrete Weighted Fourier Transform, 
so when we apply pointwise multiplication we obtain the negacyclic convolution, i.e. multiplication 
modulo x^n+1.
Note that we will not perform the usual scrambling / bit-reversal procedure here because we will the forward
fourier transform is using decimation-in-frequency.
*/
void FFT_backward(uint16_t x[n]) {
	uint16_t index, step;
	uint16_t i,j,m;
	uint16_t t0,t1;

	step = n>>1;
	for (m = 1; m < n; m=m<<1) {
		index = 0;
		for (j = 0 ; j < m; ++j) {
			for (i = j; i < n; i += (m<<1)) {				
				t0 = x[i];
				MUL_MODq(t1, x[i+m], W_rev[index]);
				ADD_MODq(x[i], t0, t1);
				SUB_MODq(x[i+m], t0, t1);
			}
			SUB_MODn(index, index, step);
		}
		step = step >> 1;
	}

	//Post multiplication for DWT
	for (i = 0; i < n; ++i) {
		MUL_MODq(x[i], x[i], W_sqrt_rev_div_N[i]);
	}

}

/*
FFT Mul. Never actually used explicitly.
*/
void FFT_MUL( uint16_t z[n], uint16_t x[n], uint16_t y[n]) {
	FFT_forward(x);
	FFT_forward(y);
	POINTWISE_MUL((z), (x), (y));
	FFT_backward(z);
}








