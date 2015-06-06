#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "list.h"

/**
 * Return true iff *av is even.
 */
int isEven (void* av) {
  intptr_t a = (intptr_t) av;
  return ! (a & 1);
}

/**
 * Set *rv = *av + *bv
 */
void add (void** rv, void* av, void* bv) {
  intptr_t  a = (intptr_t)  av;
  intptr_t  b = (intptr_t)  bv;
  intptr_t* r = (intptr_t*) rv;
  *r = a + b;
}

/**
 * Set *rv = *av + *bv
 */
void inc (void** rv, void* av) {
  intptr_t  a = (intptr_t)  av;
  intptr_t* r = (intptr_t*) rv;
  *r = a + 1;
}

/**
 * Print the value of *ev
 */
void print (void* ev) {
  intptr_t e = (intptr_t) ev;
  printf ("%ld\n", e);
}

/**
 * Test filter, map, foldl, and foreach on two small integer lists.
 */
int main() {
  intptr_t a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  intptr_t b[] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  
  list_t l0 = list_create();
  list_append_array (l0, (void**) a, sizeof(a)/sizeof(a[0]));
  
  list_t l1 = list_create();
  list_append_array (l1, (void**) b, sizeof(b)/sizeof(b[0]));
  
  list_t l2 = list_create();
  list_filter (isEven, l2, l0);
  printf ("filter:\n");
  list_foreach (print, l2);
  
  list_t l3 = list_create();
  list_map1 (inc, l3, l0);
  printf ("map1:\n");
  list_foreach (print, l3);
  
  list_t l4 = list_create();
  list_map2 (add, l4, l3, l2);
  printf ("map2:\n");
  list_foreach (print, l4);
  
  intptr_t s = 0;
  list_foldl (add, (void**) &s, l4);
  printf ("fold: %ld\n", s);

  list_destroy (l0);
  list_destroy (l1);
  list_destroy (l2);
  list_destroy (l3);
  list_destroy (l4);
}
