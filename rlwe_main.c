/* This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * See LICENSE for complete information.
 */

#define MAIN 1

#include <stdio.h>
#include <inttypes.h>

#include "rlwe_params.h"
#include "rlwe_macros.h"
#include "fft.h"
#include "rlwe_kex.h"

int main(int argc, char *argv[]) 
{

	/*Exclusively For Alice*/
	uint16_t s_alice[2*n]; /* Alice's Private Key */
	uint64_t mu_alice[nwords]; /* Alice's recovered mu */

	/*Exclusively For Bob*/
	uint64_t mu_bob[nwords]; /* Bob's version of mu */

	/*Information that gets shared by Alice and Bob*/
	uint16_t b_alice[n]; /* Alice's Public Key */
	uint16_t u[n]; /* Bob's Ring Element from Encapsulation */
	uint64_t cr_v[nwords]; /* Cross Rounding of v */


	KEM1_Generate(s_alice, b_alice);

	KEM1_Encapsulate(u, cr_v, mu_bob, b_alice);

	KEM1_Decapsulate(mu_alice, u, s_alice + n, cr_v);

	int i, flag = 1;	
	for (i = 0; i < nwords; ++i) flag &= (mu_alice[i] == mu_bob[i]);
	if (flag) {
		printf("Successful Key Agreement!\n");
	}
	else {
		printf("Failure in Key Agreement :-(\n");
	}

	printf("Alice's version of mu\n");
	for (i = 0; i < nwords; ++i) {
		printf("%lu ", mu_alice[i]);
	}
	printf("\n\n");

	printf("Bob's version of mu\n");
	for (i = 0; i < nwords; ++i) {
		printf("%lu ", mu_bob[i]);
	}

	printf("\n\n");

}


