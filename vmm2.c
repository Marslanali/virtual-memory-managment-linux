
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef unsigned long ulong;

// Data structure of meta_data
struct __attribute__((__packed__))  // compiler directive, avoid "gcc" padding bytes to struct
meta_data {
  size_t size;             // 8 bytes (in 64-bit OS)
  char free;               // 1 byte ('f' or 'o')
  struct meta_data *next;  // 8 bytes (in 64-bit OS)
  struct meta_data *prev;  // 8 bytes (in 64-bit OS)
};

#ifndef ALLOC_UNIT
#define ALLOC_UNIT 3 * sysconf(_SC_PAGESIZE)
#endif

#ifndef MIN_DEALLOC
#define MIN_DEALLOC 1 * sysconf(_SC_PAGESIZE)
#endif

#define BLOCK_MEM(ptr)    ((void *)((ulong)ptr + sizeof(struct meta_data)))
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
/*void list_add(struct meta_data *new, struct meta_data *prev, struct meta_data *next);*/
void list_add_tail(struct meta_data *new, struct meta_data *head);
void init_list(struct meta_data *list);

// Students are required to implement these functions below
void *mm_malloc(size_t size);
void mm_free(void *p);
void mm_print(struct meta_data *cur);

// new functions
void clean();
struct meta_data *split(struct meta_data *block, size_t size);
void scan_merge();
void list_add(struct meta_data *block);
void free_list_remove(struct meta_data *block);

int main(int argc, char const *argv[]) {
  printf("mem page size: %ld bytes\n", sysconf(_SC_PAGESIZE));
  printf("bytes allocated per malloc: %ld\n", ALLOC_UNIT);
  // Assume there are at most 26 different malloc/free
  // Here is the mapping rule
  // a=>0, b=>1, ..., z=>25
  void *pointers[26] = {NULL};
  char str1[20], str2[30], str3[10];

  scanf("%s %s %s", str1, str2, str3);
  printf("%s %s %s", str1, str2, str3);
  // TODO: Complete the main function here
  pointers[0] = (char *)mm_malloc(atoi(str3));
  mm_print((ulong)pointers[0] - sizeof(struct meta_data));
  mm_free(pointers[0]);

  /*pointers[1] = (char *)mm_malloc(2000);
  mm_print((ulong)pointers[1] - sizeof(struct meta_data));
  mm_free(pointers[1]);*/

/*  pointers[2] = (char *)mm_malloc(3000);
  mm_print((ulong)pointers[2] - sizeof(struct meta_data));
  mm_free(pointers[3]);

  pointers[3] = (char *)mm_malloc(4000);
  mm_print((ulong)pointers[3] - sizeof(struct meta_data));
  mm_free(pointers[3]);*/

  atexit(clean);
  return (EXIT_SUCCESS);
}

void free_list_remove(struct meta_data *block) {
  if (!block->prev) {
    if (block->next)
      head = block->next;
    else
      head = NULL;
  } else
    block->prev->next = block->next;
  if (block->next) block->next->prev = block->prev;
}

void list_add(struct meta_data *block) {
  block->prev = NULL;
  block->next = NULL;
  if (!head || (ulong)head > (ulong)block) {
    if (head) head->prev = block;
    block->next = head;
    head = block;
  } else {
    struct meta_data *curr = head;
    while (curr->next && (ulong)curr->next < (ulong)block) curr = curr->next;
    block->next = curr->next;
    curr->next = block;
  }
}

/*
 * Scans the free list to find out continuous free blocks that can be
 * merged and also checks if the last free block end where the program
 * break.
 *
 * If the free block is larger then MIN_DEALLOC then the block is released
 * to the Operating system by calling brk.
 *
 */

void scan_merge() {
  struct meta_data *curr = head;
  ulong header_curr, header_next;
  ulong program_break = (ulong)sbrk(0);
  if (program_break == 0) {
    printf("failed to get program break\n");
    return;
  }
  while (curr->next) {
    header_curr = (ulong)curr;
    header_next = (ulong)curr->next;
    if (header_curr + curr->size + sizeof(struct meta_data) == header_next) {
      /* here it found two continuous addressed blocks, merge them
       * and create a new meta_data with the sum of their sizes */
      curr->size += curr->next->size + sizeof(struct meta_data);
      curr->next = curr->next->next;
      if (curr->next)
        curr->next->prev = curr;
      else
        break;
    }
    curr = curr->next;
  }
  header_curr = (ulong)curr;
  /* last check if our last free block ends on the program break and is
   * big enough to be released to the operating system. By doing this check we reduce the
   * number of calls to sbrk/brk */
  if (header_curr + curr->size + sizeof(struct meta_data) == program_break && curr->size >= MIN_DEALLOC) {
    free_list_remove(curr);
    if (brk(curr) != 0) printf("error!!!! freeing memory\n");
  }
}

/*  This function will splits the block by creating a new block after size bytes, and then
 * this new block is returned */
struct meta_data *split(struct meta_data *block, size_t size) {
  void *mem_block = BLOCK_MEM(block);
  struct meta_data *newptr = (struct meta_data *)((ulong)mem_block + size);
  newptr->size = block->size - (size + sizeof(struct meta_data));
  block->size = size;
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
      if (ptr->size == size)
        // a perfect sized block is now found, and return it
        return block_mem;
      //  block is bigger then requested, split it and add
      // the spare to our free list
      newptr = split(ptr, size);
      list_add(newptr);
      return block_mem;
    } else
      ptr = ptr->next;
  }
  /* Here we are unable to find a free block on our free list, so we
   * should ask the operating system for memory using sbrk. We will alloc
   * more alloc_size bytes and then split the newly allocated block to keep
   * the spare space on our free list
   */
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
    list_add(newptr);
  }
  return BLOCK_MEM(ptr);
}

void mm_free(void *ptr) {
  list_add(BLOCK_HEADER(ptr));
  head->free = 'f';
  mm_print(head);
  scan_merge();
}

void clean() {
  /*  printf("cleaning up memory up\n");*/
  if (head) {
    if (brk(head) != 0) printf("failed to cleanup memory!!!");
  }
  head = NULL;
}

void mm_print(struct meta_data *cur) {
  /*struct meta_data* cur = head;*/
  while (cur) {
    printf("=== malloc Pointer[%i] %i === \n", i, cur->size);
    printf("Block %02d: [%s] size = %d bytes\n", i, (cur->free == 'f') ? "FREE" : "OCCP",
           cur->size);  //// block number - counting from bottom,  free or
    /// occupied, // size, in term of bytes
    cur = cur->next;
    i += 1;
    break;
  }
}
