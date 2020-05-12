//
// Created by arslan on 11/5/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Data structure of meta_data
struct __attribute__((__packed__))  // compiler directive, avoid "gcc" padding bytes to struct
meta_data {
  size_t size;             // 8 bytes (in 64-bit OS)
  char free;               // 1 byte ('f' or 'o')
  struct meta_data *next;  // 8 bytes (in 64-bit OS)
  struct meta_data *prev;  // 8 bytes (in 64-bit OS)
};

typedef unsigned long ulong;

#ifndef ALLOC_UNIT
#define ALLOC_UNIT 3 * sysconf(_SC_PAGESIZE)
#endif

#ifndef MIN_DEALLOC
#define MIN_DEALLOC 1 * sysconf(_SC_PAGESIZE)
#endif

#define BLOCK_MEM(ptr) ((void *)((ulong)ptr + sizeof(struct meta_data)))
#define BLOCK_HEADER(ptr) ((void *)((ulong)ptr - sizeof(struct meta_data)))

static struct meta_data *head = NULL;

// Global variables
void *start_heap = NULL;           // pointing to the start of the heap, initialize in main()
struct meta_data dummy_head_node;  // dummy head node of a doubly linked list,
// initialize in main()
/*struct meta_data *head = &dummy_head_node;*/
int i, k = 0;

// calculate the meta data size and store as a constant (exactly 25 bytes)
const size_t meta_data_size = sizeof(struct meta_data);

// The implementation of the following functions are given:
void list_add(struct meta_data *new, struct meta_data *prev, struct meta_data *next);
void list_add_tail(struct meta_data *new, struct meta_data *head);
void init_list(struct meta_data *list);

// Students are required to implement these functions below
void *mm_malloc(size_t size);
void mm_free(void *p);
void mm_print();

// new functions
void clean();
struct meta_data *split(struct meta_data *b, size_t size);
void scan_merge();
void free_list_add(struct meta_data * b);
void free_list_remove(struct meta_data * b);

int main(int argc, char const *argv[]) {
  printf("mem page size: %ld bytes\n", sysconf(_SC_PAGESIZE));
  printf("bytes allocated per malloc: %ld\n", ALLOC_UNIT);
  // Assume there are at most 26 different malloc/free
  // Here is the mapping rule
  // a=>0, b=>1, ..., z=>25
  void *pointers[26] = {NULL};
  // TODO: Complete the main function here
  pointers[0] = (char *)mm_malloc(1000);
  mm_free(pointers[0]);
  pointers[1] = (char *)mm_malloc(300);
  mm_free(pointers[1]);
  pointers[2] = (char *)mm_malloc(300);
  mm_free(pointers[2]);
  pointers[3] = (char *)mm_malloc(200);
  mm_free(pointers[3]);
  pointers[4] = (char *)mm_malloc(100);
  mm_free(pointers[4]);
  return (EXIT_SUCCESS);
}

void free_list_remove(struct meta_data *b) {
  if (!b->prev) {
    if (b->next) {
      head = b->next;
    } else {
      head = NULL;
    }
  } else {
    b->prev->next = b->next;
  }
  if (b->next) {
    b->next->prev = b->prev;
  }
}

void free_list_add(struct meta_data *b) {

  b->prev = NULL;
  b->next = NULL;
  if (!head || (ulong)head > (ulong)b) {
    if (head) {
      head->prev = b;
    }
    b->next = head;
    head = b;
  } else {
    struct meta_data *curr = head;
    while (curr->next && (ulong)curr->next < (ulong)b) {
      curr = curr->next;
    }
    b->next = curr->next;
    curr->next = b;
  }
}

void scan_merge() {
  struct meta_data *curr = head;
  ulong header_curr, header_next;
  ulong program_break = (ulong)sbrk(0);
  if (program_break == 0) {
    printf("failed to retrieve program break\n");
    return;
  }
  while (curr->next) {
    header_curr = (ulong)curr;
    header_next = (ulong)curr->next;
    if (header_curr + curr->size + sizeof(struct meta_data) == header_next) {
      /* found two continuous addressed blocks, merge them
       * and create a new meta_data with the sum of their sizes */
      curr->size += curr->next->size + sizeof(struct meta_data);
      curr->next = curr->next->next;
      if (curr->next) {
        curr->next->prev = curr;
      } else {
        break;
      }
    }
    curr = curr->next;
  }
  header_curr = (ulong)curr;
  if (header_curr + curr->size + sizeof(struct meta_data) == program_break && curr->size >= MIN_DEALLOC) {
    free_list_remove(curr);
    if (brk(curr) != 0) {
      printf("error freeing memory\n");
    }
  }
}


struct meta_data *split(struct meta_data *b, size_t size) {
  void *mem_block = BLOCK_MEM(b);
  struct meta_data *newptr = (struct meta_data *)((ulong)mem_block + size);
  newptr->size = b->size - (size + sizeof(struct meta_data));
  b->size = size;
  return newptr;
}

void *mm_malloc(size_t size) {
  void *block_mem;
  struct meta_data *ptr, *newptr;
  size_t alloc_size = size >= ALLOC_UNIT ? size + sizeof(struct meta_data) : ALLOC_UNIT;
  ptr = head;
  while (ptr) {
    if (ptr->size >= size + sizeof(struct meta_data)) {
      block_mem = BLOCK_MEM(ptr);
      free_list_remove(ptr);
      if (ptr->size == size) {
        // we found a perfect sized meta_data, return it
        return block_mem;
      }
      newptr = split(ptr, size);
      free_list_add(newptr);
      return block_mem;
    } else {
      ptr = ptr->next;
    }
  }

  ptr = sbrk(alloc_size);
  if (!ptr) {
    printf("failed to alloc %ld\n", alloc_size);
    return NULL;
  }
  ptr->next = NULL;
  ptr->prev = NULL;
  ptr->size = alloc_size - sizeof(struct meta_data);
  if (alloc_size > size + sizeof(struct meta_data)) {
    newptr = split(ptr, size);
    free_list_add(newptr);
  }
  return BLOCK_MEM(ptr);
}

void mm_free(void *ptr) {
  free_list_add(BLOCK_HEADER(ptr));
  mm_print();
  scan_merge();
}

void clean() {
/*  printf("cleaning up memory up\n");*/
  if (head) {
    if (brk(head) != 0) {
      printf("failed to cleanup memory");
    }
  }
  head = NULL;

}

void mm_print() {
  struct meta_data* cur = head;
  while (cur) {
    printf("Block %02d: [%s] size = %d bytes\n", i, (cur->free == 'f') ? "FREE" : "OCCP", cur->size);  //// block number - counting from bottom,  free or
    /// occupied, // size, in term of bytes
    cur = cur->next;
    i+=1;
    break;
  }
}
