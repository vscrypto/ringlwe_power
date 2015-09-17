/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * See LICENSE for complete information.
 */

void FFT_forward(uint16_t x[n]);

void FFT_backward(uint16_t x[n]);

void FFT_MUL( uint16_t z[n], uint16_t x[n], uint16_t y[n]);
