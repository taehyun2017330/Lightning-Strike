//------------------------------------------------------------------------------
//
// memtrace
//
// trace calls to the dynamic memory manager
//
#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memlog.h>
#include <memlist.h>

//
// function pointers to stdlib's memory management functions
//
static void *(*mallocp)(size_t size) = NULL;
static void (*freep)(void *ptr) = NULL;
static void *(*callocp)(size_t nmemb, size_t size);
static void *(*reallocp)(void *ptr, size_t size);

//
// statistics & other global variables
//
static unsigned long n_malloc  = 0;
static unsigned long n_calloc  = 0;
static unsigned long n_realloc = 0;
static unsigned long n_allocb  = 0;
static unsigned long n_freeb   = 0;
static item *list = NULL;

//
// init - this function is called once when the shared library is loaded
//
__attribute__((constructor))
void init(void)
{
  char *error;

  LOG_START();

  // initialize a new list to keep track of all memory (de-)allocations
  // (not needed for part 1)
  list = new_list();

  // ...
}

//
void *malloc(size_t size){ //create new methods for each command
    char *error;
    void *ptr;
    
    if (!mallocp) {
        mallocp = dlsym(RTLD_NEXT, "malloc");
        if ((error = dlerror()) != NULL) {
            mlog(error);
            exit(1);
} }
    ptr = mallocp(size);
    LOG_MALLOC(size,ptr);
    n_malloc++;
    n_allocb=n_allocb+size;
    return ptr;
}

void *calloc(size_t nmeb, size_t size)
{
    char *error;
    void *ptr;
    unsigned long val;
    
    if (!callocp) {
        callocp = dlsym(RTLD_NEXT, "calloc");
        if ((error = dlerror()) != NULL) {
            mlog(error);
            exit(1);
} }
    val=nmeb*size;
    ptr = callocp(nmeb,size);
    LOG_CALLOC(nmeb,size,ptr);
    n_calloc++;
    n_allocb+=val;
    return ptr;
}

void *realloc(void *ptr_n, size_t size)
{
    char *error;
    void *ptr;
    
    if (!reallocp) {
        reallocp = dlsym(RTLD_NEXT, "realloc");
        if ((error = dlerror()) != NULL) {
            mlog(error);
            exit(1);
} }
    ptr = reallocp(ptr_n,size);
    LOG_REALLOC(ptr_n,size,ptr);
    n_realloc++;
    n_allocb+=size;
    return ptr;
}

void free(void *ptr)
{
    char *error;
    
    if (!freep) {
        freep = dlsym(RTLD_NEXT, "free");
        if ((error = dlerror()) != NULL) {
            mlog(error);
            exit(1);
} }
    LOG_FREE(ptr);
    freep(ptr);
}

// fini - this function is called once when the shared library is unloaded
//
__attribute__((destructor))
void fini(void)
{
  // ...
   unsigned long average= n_allocb/(n_calloc+n_realloc+n_malloc);

   LOG_STATISTICS(n_allocb, average, 0L);

  LOG_STOP();

  // free list (not needed for part 1)
  free_list(list);
}

// ...
