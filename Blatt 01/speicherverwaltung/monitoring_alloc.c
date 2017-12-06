/*
 * monitoring_alloc.c
 *
 * Author: Alexander Ploss <a.ploss@uni-muenster.de>
 *         Michel Steuwer <michel.steuwer@uni-muenster.de>
 *
 */

// Header einbinden.
#include "monitoring_alloc.h"

// Datenstrukturen verwendet von den monitoring_alloc Funktionen.
AllocatedMemoryBlock allocated_blocks[MAX_ALLOCATIONS];

// Schreiben Sie hier ihre Implementierung für Aufgabenteil a).

void init_monitoring_alloc() {
}

int shutdown_monitoring_alloc() {
  int leakingBytes = 0;

  for (int i = 0; i < MAX_ALLOCATIONS; ++i) {
      if(allocated_blocks[i].frame) {
          leakingBytes += /* sizeof(allocated_blocks[i].frame) * */ allocated_blocks[i].size;
      }
  }

  printf("ERROR: Leaking %lu bytes in total!\n",
      (unsigned long)leakingBytes);
  return leakingBytes;
}

void *monitoring_alloc_malloc(size_t size) {
  void *allocated = NULL;
  AllocatedMemoryBlock newMemoryBlock;

  // Find available position in memoryblock array
  int i;
  for (i = 0; i < MAX_ALLOCATIONS; ++i) {
      if (!(allocated_blocks[i].frame)) {
          break;
      }
  }

  // Throw error if no available pos is found
  if (i == MAX_ALLOCATIONS) {
      printf("ERROR: Amount of simultanously monitored allocations could not exceed %i.\n",
              MAX_ALLOCATIONS);
      return 0;
  } 

  // Throw error if allocation of memory fails
  if(!(allocated = malloc(size))) {
    printf("ERROR: Block of size %lu could not be allocated!\n",
        (unsigned long)size);
    return 0;
  } 

  // Initialize newMemoryBlock
  newMemoryBlock.frame = allocated;
  newMemoryBlock.size = size;
  newMemoryBlock.ordinal = 1;

  // safe newMemoryBlock in array
  allocated_blocks[i] = newMemoryBlock;

  return allocated;
}

void monitoring_alloc_free(void *ptr) {
  int found = 0;

  int i = 0;
  while (i < MAX_ALLOCATIONS) {
      if (allocated_blocks[i].frame) {
          if (allocated_blocks[i].frame == ptr) {
              free(allocated_blocks[i].frame);
              allocated_blocks[i].frame = NULL;
              allocated_blocks[i].size = 0;
              allocated_blocks[i].ordinal = 0;
              found = 1;
          }
      }
      i++;
  }

  if(!found) {
    printf("ERROR: Block %p not allocated!\n", ptr);
  }
}

