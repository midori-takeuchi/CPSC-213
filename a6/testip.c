#include <stdlib.h>
#include <stdio.h>
#include "list.h"

int isEven (void* av) {
  int* a = av;
  return ! (*a & 1);
}

void add (void** rv, void* av, void* bv) {
  int* a = av;
  int* b = bv;
  int* r = malloc (sizeof(*r));
  *r = *a + *b;
  if (*rv)
    free (*rv);
  *rv = r;
}

void inc (void** rv, void* av) {
  int* a = av;
  int* r = malloc (sizeof (*r));
  *r = *a + 1;
  if (*rv)
    free (*rv);
  *rv = r;
}

void print (void* ev) {
  int* e = ev;
  printf ("%d\n", *e);
}

int main() {
  int a[]   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  int b[]   = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  int* ap[] = {&a[0],&a[1],&a[2],&a[3],&a[4],&a[5],&a[6],&a[7],&a[8],&a[9],&a[10]};
  int* bp[] = {&b[0],&b[1],&b[2],&b[3],&b[4],&b[5],&b[6],&b[7],&b[8],&b[9],&b[10]};

  list_t l0 = list_create();
  list_append_array (l0, (void**) ap, sizeof(ap)/sizeof(ap[0]));
  
  list_t l1 = list_create();
  list_append_array (l1, (void**) bp, sizeof(bp)/sizeof(bp[0]));
  
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
  
  int* sp = malloc (sizeof (*sp));
  list_foldl (add, (void**) &sp, l4);
  printf ("fold: %d\n", *sp);
  
  list_foreach (free, l4);
  free (sp);
  
  list_destroy (l0);
  list_destroy (l1);
  list_destroy (l2);
  list_destroy (l3);
  list_destroy (l4);
}