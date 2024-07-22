#ifndef __LIST_H__
#define __LIST_H__

struct list_head {
  struct list_head *next;
  struct list_head *prev;
};

#ifdef LIST_DEBUG
#include <stdio.h>
#include <stdlib.h>
#define panic(str) __panic(__func__, str)
void __panic(const char* caller, const char* str)
{
  fprintf(stderr, "%s: %s", caller, str);
  exit(-1);
}
#else
#define panic(str) ((void)0)
#endif /* LIST_DEBUG */

#ifndef offsetof
#define offsetof(type, member) ((size_t)&(((type*) 0)->member))
#endif /* offsetof */

#ifndef container_of
#define container_of(ptr, type, member) ({				\
  void *__mptr = (void *)(ptr);					\
 ((type *)(__mptr - offsetof(type, member))); \
})
#endif /* container_of */


/* start */
#define list_entry(ptr, type, member) container_of(ptr, type, member)
#define list_entry_next(pos, member) list_entry((pos)->member.next, typeof(*(pos)), member)
#define list_entry_prev(pos, member) list_entry((pos)->member.prev, typeof(*(pos)), member)
#define list_first_entry(ptr, type, member) list_entry((ptr)->next, type, member)
#define list_for_each_entry(pos, head, member) \
  for (pos = list_first_entry(head, typeof(*(pos)), member); \
    &pos->member != head; \
    pos = list_entry_next(pos, member) \
  )

static inline void INIT_LIST_HEAD(struct list_head *list)
{
  list->next = list;
  list->prev = list;
}

static inline void __list_add( struct list_head *new_entry,
                        struct list_head *prev,
                        struct list_head *next
)
{
  if (!new_entry || !prev || !next) {
    if (!new_entry) { panic("empty new entry"); }
    if (!new_entry) { panic("empty prev"); }
    panic("empty next");
  }
  next->prev = new_entry;
  new_entry->next = next;
  new_entry->prev = prev;
  prev->next = new_entry;
}

static inline void list_add(struct list_head *new_entry, struct list_head *head)
{
  __list_add(new_entry, head, head->next);
}

static inline void list_add_tail(struct list_head *new_entry, struct list_head *head)
{
  __list_add(new_entry, head->prev, head);
}

static inline void __list_del(struct list_head *entry)
{
  struct list_head* prev = entry->prev;
  struct list_head* next = entry->next;
  prev->next = next;
  next->prev = prev;
}

static inline void list_del_init(struct list_head *entry) {
  __list_del(entry);
  INIT_LIST_HEAD(entry);
}


#endif /* __LIST_H__ */