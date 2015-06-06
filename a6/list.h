#ifndef __list_h__
#define __list_h__

struct list;
typedef struct list* list_t;

list_t list_create       ();
void   list_destroy      (list_t l);
void   list_append       (list_t l, void* e);
void   list_append_array (list_t l, void** a, int n);
void   list_insert       (list_t l, int pos, void* e);
void   list_remove       (list_t l, int pos);
void*  list_get          (list_t l, int pos);
int    list_index        (list_t l, void* e, int (*equal) (void*, void*));
int    list_len          (list_t l);

void   list_map1    (void (*f) (void**, void*),        list_t r, list_t l);
void   list_map2    (void (*f) (void**, void*, void*), list_t r, list_t l0, list_t l1);
void   list_foldl   (void (*f) (void**, void*, void*), void** r, list_t l);
void   list_filter  (int  (*f) (void*),                list_t r, list_t l);
void   list_foreach (void (*f) (void*),                list_t l);

#endif /* __list_h__ */
