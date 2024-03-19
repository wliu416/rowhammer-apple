#include "shared.hh"
#include "params.hh"
#include "util.hh"


#define NS_PER_SEC (1000000000L)
// Seconds to Nanoseconds
#define SEC_TO_NS(sec) ((sec)*NS_PER_SEC)


// Base pointer to a large memory pool
void * allocated_mem;

/*
 * allocate_pages
 *
 * Allocates a memory block of size BUFFER_SIZE_MB
 *
 * Make sure to write something to each page in the block to ensure
 * that the memory has actually been allocated!
 *
 * Inputs: none
 * Outputs: A pointer to the beginning of the allocated memory block
 */
void * allocate_pages(uint64_t memory_size) {
  void * memory_block = mmap(NULL, memory_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE , -1, 0);
  assert(memory_block != (void*) - 1);

  for (uint64_t i = 0; i < memory_size; i += PAGE_SIZE) {
    uint64_t * addr = (uint64_t *) ((uint8_t *) (memory_block) + i);
    *addr = i;
  } 

  return memory_block;  
}


uint64_t get_timestamp() {
  struct timespec ts;    
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t) SEC_TO_NS(ts.tv_sec) + (uint64_t) ts.tv_nsec;
}

/*
 * measure_bank_latency
 *
 * Measures a (potential) bank collision between two addresses,
 * and returns its timing characteristics.
 *
 * Inputs: addr_A/addr_B - Two (virtual) addresses used to observe
 *                         potential contention
 * Output: Timing difference (derived by a scheme of your choice)
 *
 */
uint64_t measure_bank_latency(uint64_t addr_A, uint64_t addr_B) {
  //run clflush2(addr_A);
  //run clflush2(addr_B);
  arm_v8_cache_flush(addr_A);
  arm_v8_cache_flush(addr_B);

  uint8_t * addr_A_ptr = reinterpret_cast<uint8_t *>(addr_A);
  uint8_t * addr_B_ptr = reinterpret_cast<uint8_t *>(addr_B);

  (*addr_A_ptr);
  // run clflush2(addr_A);
  arm_v8_cache_flush(addr_A);

  // Credit: IAIK - ARMageddon Paper
  //lfence();
  arm_v8_memory_barrier();

  uint64_t t1 = get_timestamp();

  (*addr_A_ptr);
  (*addr_B_ptr);

  //lfence();
  arm_v8_memory_barrier();

  uint64_t t2 = get_timestamp();

  return (uint64_t) t2-t1;
}



char *int_to_binary(uint64_t num, int num_bits) {
    char *binary = (char *) calloc(num_bits + 1, 1);
	
    for (int i = num_bits - 1; i >= 0; i--) {
        binary[i] = (num & 1) + '0';
        num >>= 1;
    }
    binary[num_bits] = '\0';
    return binary;
}

