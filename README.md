# list.h

Linux kernel style list.h

see test.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct foo {
  int data;
  struct list_head list;
};

struct foo* init_foo(int data) {
  struct foo* r = (struct foo*)malloc(sizeof(*r));
  r->data = data;
  return r;
}

int main()
{
  {
    struct foo* head = init_foo(0);
    INIT_LIST_HEAD(&head->list);
    for (int i = 1; i < 10; i++) {
      struct foo* b = init_foo(i);
      list_add(&b->list, &head->list);
    }

    /* test list_for_each_entry */
    struct foo* it;
    int expected = 9;
    list_for_each_entry(it, &head->list, list) {
      if (it->data != expected)
        printf("got: %d, expected: %d\n", it->data, expected);
      expected--;
    }

    /* test list_del_init */
    struct foo* new_head = list_entry_next(head, list);
    list_del_init(&head->list);
    expected = 8;
    list_for_each_entry(it, &head->list, list) {
      if (it->data != expected)
        printf("got: %d, expected: %d\n", it->data, expected);
      expected--;
    }
  }
  return 0;
}
```