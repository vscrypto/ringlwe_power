
header_files = rlwe_params.h rlwe_macros.h fft.h rlwe_kex.h

c_files = rlwe_params.c fft.c rlwe_kex.c

random_files = rlwe_rand_c.c rlwe_rand_openssl_aes.c rlwe_rand_openssl_rand.c rlwe_rand_openssl_rc4.c

main_file = rlwe_main.c

benchmark_file = rlwe_benchmark.c

testvec_file = rlwe_testvec.c

C = gcc

# if OpenSSL available use this line
CFLAGS = -O3 -std=c99 -lcrypto
# otherwise use this line
#CFLAGS = -O3 -std=c99 -DRLWE_RANDOMNESS_USE_C_RANDOM_INSECURE

all: rlwe1024_uniform rlwe1024_gaussian rlwe1024_uniform_benchmark rlwe1024_gaussian_benchmark rlwe1024_uniform_testvec rlwe1024_gaussian_testvec rlwe512_uniform rlwe512_gaussian rlwe512_uniform_benchmark rlwe512_gaussian_benchmark rlwe512_uniform_testvec rlwe512_gaussian_testvec rlwe256_uniform rlwe256_gaussian rlwe256_uniform_benchmark rlwe256_gaussian_benchmark rlwe256_uniform_testvec rlwe256_gaussian_testvec

rlwe1024_uniform: $(c_files) $(header_files) $(random_files) $(main_file)
	${C} $(CFLAGS) -DN1024 -DUNIFORM -o rlwe1024_uniform $(main_file) $(c_files)

rlwe1024_gaussian: $(c_files) $(header_files) $(random_files) $(main_file)
	${C} $(CFLAGS) -DN1024 -DGAUSSIAN -o rlwe1024_gaussian $(main_file) $(c_files)

rlwe1024_uniform_benchmark: $(c_files) $(header_files) $(random_files) $(benchmark_file)
	${C} $(CFLAGS) -DN1024 -DUNIFORM -o rlwe1024_uniform_benchmark $(benchmark_file) $(c_files)

rlwe1024_gaussian_benchmark: $(c_files) $(header_files) $(random_files) $(benchmark_file)
	${C} $(CFLAGS) -DN1024 -DGAUSSIAN -o rlwe1024_gaussian_benchmark $(benchmark_file) $(c_files)

rlwe1024_uniform_testvec: $(c_files) $(header_files) $(random_files) $(testvec_file)
	${C} $(CFLAGS) -DN1024 -DUNIFORM -o rlwe1024_uniform_testvec $(testvec_file) $(c_files)

rlwe1024_gaussian_testvec: $(c_files) $(header_files) $(random_files) $(testvec_file)
	${C} $(CFLAGS) -DN1024 -DGAUSSIAN -o rlwe1024_gaussian_testvec $(testvec_file) $(c_files)

rlwe512_uniform: $(c_files) $(header_files) $(random_files) $(main_file)
	${C} $(CFLAGS) -DN512 -DUNIFORM -o rlwe512_uniform $(main_file) $(c_files)

rlwe512_gaussian: $(c_files) $(header_files) $(random_files) $(main_file)
	${C} $(CFLAGS) -DN512 -DGAUSSIAN -o rlwe512_gaussian $(main_file) $(c_files)

rlwe512_uniform_benchmark: $(c_files) $(header_files) $(random_files) $(benchmark_file)
	${C} $(CFLAGS) -DN512 -DUNIFORM -o rlwe512_uniform_benchmark $(benchmark_file) $(c_files)

rlwe512_gaussian_benchmark: $(c_files) $(header_files) $(random_files) $(benchmark_file)
	${C} $(CFLAGS) -DN512 -DGAUSSIAN -o rlwe512_gaussian_benchmark $(benchmark_file) $(c_files)

rlwe512_uniform_testvec: $(c_files) $(header_files) $(random_files) $(testvec_file)
	${C} $(CFLAGS) -DN512 -DUNIFORM -o rlwe512_uniform_testvec $(testvec_file) $(c_files)

rlwe512_gaussian_testvec: $(c_files) $(header_files) $(random_files) $(testvec_file)
	${C} $(CFLAGS) -DN512 -DGAUSSIAN -o rlwe512_gaussian_testvec $(testvec_file) $(c_files)

rlwe256_uniform: $(c_files) $(header_files) $(random_files) $(main_file)
	${C} $(CFLAGS) -DN256 -DUNIFORM -o rlwe256_uniform $(main_file) $(c_files)

rlwe256_gaussian: $(c_files) $(header_files) $(random_files) $(main_file)
	${C} $(CFLAGS) -DN256 -DGAUSSIAN -o rlwe256_gaussian $(main_file) $(c_files)

rlwe256_uniform_benchmark: $(c_files) $(header_files) $(random_files) $(benchmark_file)
	${C} $(CFLAGS) -DN256 -DUNIFORM -o rlwe256_uniform_benchmark $(benchmark_file) $(c_files)

rlwe256_gaussian_benchmark: $(c_files) $(header_files) $(random_files) $(benchmark_file)
	${C} $(CFLAGS) -DN256 -DGAUSSIAN -o rlwe256_gaussian_benchmark $(benchmark_file) $(c_files)

rlwe256_uniform_testvec: $(c_files) $(header_files) $(random_files) $(testvec_file)
	${C} $(CFLAGS) -DN256 -DUNIFORM -o rlwe256_uniform_testvec $(testvec_file) $(c_files)

rlwe256_gaussian_testvec: $(c_files) $(header_files) $(random_files) $(testvec_file)
	${C} $(CFLAGS) -DN256 -DGAUSSIAN -o rlwe256_gaussian_testvec $(testvec_file) $(c_files)


test: all
	@echo "Running testing code"
	@echo "Running rlwe1024_uniform"
	./rlwe1024_uniform >rlwe1024_uniform.test.log
	@echo "Running rlwe1024_gaussian"
	./rlwe1024_gaussian >rlwe1024_gaussian.test.log
	@echo "Running rlwe512_uniform"
	./rlwe512_uniform >rlwe512_uniform.test.log
	@echo "Running rlwe512_gaussian"
	./rlwe512_gaussian >rlwe512_gaussian.test.log
	@echo "Running rlwe256_uniform"
	./rlwe256_uniform >rlwe256_uniform.test.log
	@echo "Running rlwe256_gaussian"
	./rlwe256_gaussian >rlwe256_gaussian.test.log
	@echo "Running rlwe_benchmarking code"
	./rlwe1024_uniform_benchmark >rlwe1024_uniform_benchmark.test.log
	@echo "Running rlwe1024_gaussian"
	./rlwe1024_gaussian_benchmark >rlwe1024_gaussian_benchmark.test.log
	@echo "Running rlwe512_uniform"
	./rlwe512_uniform_benchmark >rlwe512_uniform_benchmark.test.log
	@echo "Running rlwe512_gaussian"
	./rlwe512_gaussian_benchmark >rlwe512_gaussian_benchmark.test.log
	@echo "Running rlwe256_uniform"
	./rlwe256_uniform_benchmark >rlwe256_uniform_benchmark.test.log
	@echo "Running rlwe256_gaussian"
	./rlwe256_gaussian_benchmark >rlwe256_gaussian_benchmark.test.log
	@echo "Running rlwe_testing code"
	./rlwe1024_uniform_testvec >rlwe1024_uniform_testvec.test.log
	@echo "Running rlwe1024_gaussian"
	./rlwe1024_gaussian_testvec >rlwe1024_gaussian_testvec.test.log
	@echo "Running rlwe512_uniform"
	./rlwe512_uniform_testvec >rlwe512_uniform_testvec.test.log
	@echo "Running rlwe512_gaussian"
	./rlwe512_gaussian_testvec >rlwe512_gaussian_testvec.test.log
	@echo "Running rlwe256_uniform"
	./rlwe256_uniform_testvec >rlwe256_uniform_testvec.test.log
	@echo "Running rlwe256_gaussian"
	./rlwe256_gaussian_testvec >rlwe256_gaussian_testvec.test.log


clean:
	rm -f *~ *.o *.test.log rlwe1024_uniform rlwe1024_gaussian rlwe512_uniform rlwe512_gaussian rlwe256_uniform rlwe256_gaussian rlwe1024_uniform_benchmark rlwe1024_gaussian_benchmark rlwe512_uniform_benchmark rlwe512_gaussian_benchmark rlwe256_uniform_benchmark rlwe256_gaussian_benchmark rlwe1024_uniform_testvec rlwe1024_gaussian_testvec rlwe512_uniform_testvec rlwe512_gaussian_testvec rlwe256_uniform_testvec rlwe256_gaussian_testvec

