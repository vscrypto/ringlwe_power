A Practical Ring-LWE Key Exchange Implementation
------------------------------------------------

This software is plain C (C99 standard). Compilation has been tested using gcc on Linux. Dependencies are glibc-devel and openssl-devel, or equivalents on other platforms. The Makefile can be editted to compile without openssl-devel but this is not secure, see below.

To compile: 
make

To run tests:
make test


This software provides an implementation of a practical and efficient Diffie-Hellman-like key exchange which is provably passively secure against quantum and classical adversaries. It is based on the ring learning with errors (ring-LWE) problem. It can readily be extended to an actively secure version and an authenticated version.

For each algorithm variant, we provide:
* an example key exchange program in rlwe_main.c
* a benchmarking program in rlwe_benchmark.c
* a check of test vectors in rlwe_testvec.c

We recommend the uniform sampling variants because they run significantly faster than the gaussian sampling equivalents. 

Algorithm         Security       Public key size
------------------------------------------------
rlwe256_uniform   toy            3561 bits
rlwe512_uniform   128 bits       7498 bits
rlwe1024_uniform  256 bits       15690 bits
------------------------------------------------ 

The algorithm is described in the paper "A Practical Key Exchange for the Internet using Lattice Cryptography" by Vikram Singh, available at http://eprint.iacr.org/2015/138. This in turn is a version of the passively secure key exchange described in the paper "Lattice Cryptography for the Internet" by Chris Peikert, available at http://eprint.iacr.org/2014/070.

Substantial parts of this code are copied from the software published by Joppe W. Bos, Craig Costello, Michael Naehrig, and Douglas Stebila at https://github.com/dstebila/rlwekex, and described in their paper "Post-quantum key exchange for the TLS protocol from the ring learning with errors problem", available at http://eprint.iacr.org/2014/599.


Cryptographically secure random number generation
-------------------------------------------------
Note that the key generation and key exchange algorithms make use of a random number generator during execution.  The sampling code is configured by default to use OpenSSL's PRNG to generate a seed and expand it using AES.  Several other options are available; see `rlwe_kex.c`.  C's `random()` can be used for testing purposes by defining the macro `RLWE_RANDOMNESS_USE_C_RANDOM_INSECURE`, but this is **not secure**.  Developers can also define the `RANDOM_VARS`, `RANDOM8`, `RANDOM32`, `RANDOM64` macros with a cryptographically secure pseudorandom number generator of their own choosing.  

License
-------
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.

See the file LICENSE for complete information.
