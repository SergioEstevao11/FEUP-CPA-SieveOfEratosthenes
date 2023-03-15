# Sieve of Erastothenes

The algorithm to compute the primes up to N.

## Dependencies

1. A C++ compiler compatible with C++20
    1. We use [`g++`](https://gcc.gnu.org/) but others should work fine
2. [PAPI](https://icl.utk.edu/papi/)
3. [OpenMP](https://www.openmp.org/)
    1. Only required to compile `parallel`
4. [GNU Make](https://www.gnu.org/software/make/)
    1. *Optional* - just for compilation convinience## Usage

## Usage

```bash
make [all | basic | multiples | segmented | parallel] # builds a target
./bin/basic.out     # naïve sieve of erastothenes
./bin/multiples.out # fast-marking based sieve of erastothenes
./bin/segmented.out # segmented sieve of erastothenes
./bin/parallel.out  # parallel segmented sieve of erastothenes
```

---
Authors:
- [Miguel Rodrigues](mailto:up201906042@edu.fe.up.pt)
- [Sérgio Estêvão](mailto:up201905680@edu.fe.up.pt)

[CPA @ M.EIC](https://sigarra.up.pt/feup/pt/ucurr_geral.ficha_uc_view?pv_ocorrencia_id=486270)
