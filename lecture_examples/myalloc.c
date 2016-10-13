/*
Compile: make myalloc
Run: ./myalloc
make myalloc && ./myalloc
*/

#include "base.h"

/**
Memory is allocated in units of blocks. A free-block header points to the next free block and stores its size.
*/
struct Block {
    struct Block *next; // next free block
    int size; // block size (unit is sizeof(struct Block))
};
typedef struct Block Block;

/**
The actual memory array.
*/
#define BLOCKS_COUNT 100
Block memory[BLOCKS_COUNT];

/**
Pointer to the first free block.
*/
Block *free_block = NULL;

void init_memory(void);
void* myalloc(int n);
void myfree(void* p);
void print_memory(void);

/**
Initialize memory. Needs to be called before calling myalloc or myfree.
*/
void init_memory(void) {
    memset(memory, 0, BLOCKS_COUNT * sizeof(Block));
    Block *b = (Block*)memory; // initially, memory is one large free-block
    b->next = b; // points to itself
    b->size = BLOCKS_COUNT - 1;
    free_block = b;
}

/**
Request n bytes of memory.
@param[in] n the number of bytes needed
@return a pointer to the start of the memory block or NULL if no such block is available
*/
void *myalloc(int n) {
    if (n <= 0 || free_block == NULL) return NULL;
    int size = (n + sizeof(Block) - 1) / sizeof(Block);
    
    for (Block *b = free_block; b != NULL; b = b->next) {
        if (b->next->size >= size) {
            Block *use = b->next;
            if (b->next->size == size) { // requested size equals block size
                b->next = use->next;
            } else /* b->next->size > size */ { // split requested size from this block
                b->next = use + 1 + size;
                b->next->size = use->size - size - 1;
                b->next->next = use->next;
                use->size = size;
            }
            if (use == free_block) { // check if free_block is no longer free
                free_block = use->next;
                if (use == free_block) {
                    free_block = NULL;
                }
            }
            return (void*)(use + 1); // user memory starts after block header
        }
        if (b->next == free_block) break;
    }

    return NULL;
}

/**
Frees the memory block.
@param[in] a pointer to the start of the memory block to free
*/
void myfree(void* p) {
    if (p == NULL) return;
    Block *f = (Block*)p - 1; // block header is on unit left of user memory
    if (f < memory || f >= (Block*)memory + BLOCKS_COUNT) return;

    if (free_block == NULL) {
        free_block = f;
        f->next = f;
        return;
    }

    for (Block *h = free_block; h != NULL; h = h->next) {
        if ((h->next > h && f > h && f < h->next) || // f between h and h->next
            (h->next <= h && f > h) || // f above wrap
            (h->next <= h && f < h->next)) // f below wrap
        {
            f->next = h->next;
            h->next = f;
            if (f + 1 + f->size == f->next) { // merge right
                f->size += 1 + f->next->size;
                f->next = f->next->next;
                free_block = f;
            }
            if (h + 1 + h->size == f) { // merge left
                h->size += 1 + f->size;
                h->next = f->next;
                free_block = h;
            }
            return;
        }
    }
}

/**
Print the current state of the memory.
*/
void print_memory(void) {
    prints("[");
    for (Block *h = free_block; h != NULL; h = h->next) {
        printf("%li: %d", h - (Block*) memory, h->size);
        if (h->next == free_block) break;
        prints(", ");
    }
    printsln("]");
}

void myalloc_test(void) {
    printf("sizeof(Block) = %ld\n", sizeof(Block));
    init_memory();
    print_memory();

    void *p1 = myalloc(1 * sizeof(Block));
    printf("p = %p = %li\n", p1, (Block*)p1 - (Block*)memory);
    print_memory();

    void *p2 = myalloc(1 * sizeof(Block));
    printf("p = %p = %li\n", p2, (Block*)p2 - (Block*)memory);
    print_memory();

    void *p3 = myalloc(1 * sizeof(Block));
    printf("p = %p = %li\n", p3, (Block*)p3 - (Block*)memory);
    print_memory();

    void *p4 = myalloc(1 * sizeof(Block));
    printf("p = %p = %li\n", p4, (Block*)p4 - (Block*)memory);
    print_memory();

    myfree(p1);
    print_memory();

    myfree(p3);
    print_memory();

    myfree(p2);
    print_memory();

    myfree(p4);
    print_memory();
}

int main(void) {
    myalloc_test();
    
    /*
    printf("%p\n", malloc(1));
    printf("%p\n", malloc(0));
    printf("%p\n", malloc(-1));
    printf("%p\n", malloc(-2));
    */
    
    return 0;
}
