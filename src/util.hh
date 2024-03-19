#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>

#ifndef __UTILITY_H__
#define __UTILITY_H__

#define SAMPLES 10


// Function to read the time stamp counter, which is called tsc for short
// "rdtscp" returns a 32bit unsigned integer
// "rdtscp64" return a 64 bit unsigned integer
// Details in https://www.felixcloutier.com/x86/rdtscp
// static inline uint32_t rdtscp() {
//   uint32_t rv;
//   asm volatile ("rdtscp": "=a" (rv) :: "edx", "ecx");
//   return rv;
// }

// static inline uint64_t rdtscp64() {
//   uint32_t low, high;
//   asm volatile ("rdtscp": "=a" (low), "=d" (high) :: "ecx");
//   return (((uint64_t)high) << 32) | low;
// }

// Function "lfence" wrap the assembly instruction lfence
// This function performs a serializing operation which ensures that
// the instructions after "lfence" start execution after
// all the instructions before "lfence" complete
// Details in https://www.felixcloutier.com/x86/lfence
// static inline void lfence() {
//   asm volatile("lfence");
// }

static inline void arm_v8_memory_barrier(void) {
  asm volatile ("DSB SY");
  asm volatile ("ISB");
}

// Here is an example of using "rdtsc" and "lfence" to
// measure the time it takes to access a block specified by its virtual address
// The corresponding pseudo code is
// =========
// t1 = rdtsc
// load addr
// t2 = rdtsc
// cycles = t2 - t1
// return cycles
// =========
// static inline uint64_t measure_one_block_access_time(uint64_t addr)
// {
// 	uint64_t cycles;

// 	asm volatile("mov %1, %%r8\n\t"
// 	"lfence\n\t"
// 	"rdtsc\n\t"
// 	"mov %%eax, %%edi\n\t"
// 	"mov (%%r8), %%r8\n\t"
// 	"lfence\n\t"
// 	"rdtsc\n\t"
// 	"sub %%edi, %%eax\n\t"
// 	: "=a"(cycles) /*output*/
// 	: "r"(addr)    /*input*/
// 	: "r8", "edi");	/*reserved register*/

// 	return cycles;
// }


static inline void arm_v8_cache_flush(uint64_t addr)
{
	//asm volatile ("":::);
	// DC CIVAC May not work, consider IC IVAU?
	// Some combination of memory barriers needed.
	asm volatile ("DC CIVAC, %0" :: "r"(addr));
	// asm volatile ("DC CVAC, %0" :: "r"(addr));
	asm volatile ("IC IVAU, %0" :: "r"(addr));
    asm volatile ("DSB SY");
    asm volatile ("ISB");
}



// static inline void one_block_access(uint64_t addr)
// {
// 	asm volatile("mov (%0), %%r8\n\t"
// 	: /*output*/
// 	: "r"(addr)    /*input*/
// 	: "r8");	/*reserved register*/

// }

// static inline uint64_t two_maccess_t(uint64_t addr_a, uint64_t addr_b)
// {
// 	uint64_t cycles;

// 	asm volatile(
// 		"mov %1, %%r8\n\t"
// 		"mov %2, %%r9\n\t"
// 		"lfence\n\t"
// 		"rdtsc\n\t"
// 		"mov %%eax, %%edi\n\t"
// 		"mov (%%r8), %%r8\n\t"
// 		"mov (%%r9), %%r9\n\t"
// 		"lfence\n\t"
// 		"rdtsc\n\t"
// 		"sub %%edi, %%eax\n\t"
// 		: "=a"(cycles) /*output*/
// 		: "r"(addr_a), "r"(addr_b)    /*input*/
// 		: "r8", "r9", "edi"
// 	);	/*reserved register*/

// 	return cycles;
// }


// A wrapper function of the clflush instruction
// The instruction evict the given address from the cache to DRAM
// so that the next time the line is accessed, it will be fetched from DRAM
// Details in https://www.felixcloutier.com/x86/clflush
// static inline void clflush(void *v) {
//   asm volatile ("clflush 0(%0)": : "r" (v):);
// }

// static inline void clflush2(uint64_t addr)
// {
//   asm volatile ("clflush (%0)"
// 		: /*output*/
// 		: /*input*/ "r"(addr)
// 		: /*clobbers*/ );
// }

// /* Ensure all instructions execute before before anything else can exec. */
// static inline void mfence()
// {
//   asm volatile ("mfence"
// 		: /*output*/
// 		: /*input*/
// 		: /*clobbers*/ );
// }




#endif // _UTILITY_H__ 

