/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * See LICENSE for complete information.
 */

/* Construct Alice's private / public key pair. Return all elements in the Fourier Domain
 * input:  none
 * output: private key s_1=s[n]...s[2*n-1] in Fourier Domain
 *         noise term s_0=s[0]...s[n-1] in Fourier Domain, not needed again
 *         public key b in Fourier Domain
 */	
void KEM1_Generate(uint16_t s[2*n], uint16_t b[n]);

/* Encapsulation routine. Returns an element in R_q x R_2
 * input:  Alice's public key b in Fourier Domain
 * output: Bob's public key u in Fourier Domain
 *         reconciliation data cr_v
 *         shared secret mu
 */
void KEM1_Encapsulate(uint16_t u[n], uint64_t cr_v[nwords], uint64_t mu[nwords], uint16_t b[n]);

/* Decapsulation routine.
 * input:  Bob's public key u in Fourier Domain
 *         Alice's private key s_1 in Fourier Domain
 *         reconciliation data cr_v
 * output: shared secret mu
 */
void KEM1_Decapsulate(uint64_t mu[nwords], uint16_t u[n], uint16_t s_1[n], uint64_t cr_v[nwords]);


/* Sample secret. Only needed externally for benchmarking. */
void sample_secret(uint16_t s[n]);

/* Round and cross-round. Only needed externally for benchmarking. */
void round_and_cross_round(uint64_t modular_rnd[nwords], uint64_t cross_rnd[nwords], const uint16_t v[n]);

/* Reconcile. Only needed externally for benchmarking. */
void rec(uint64_t r[nwords], uint16_t w[n], uint64_t b[nwords]);

