#ifndef LINKED_LIST
#define LINKED_LIST

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define make_linked_list(type) make_linked_list_##type()

#define linked_list_append(type, list, value)                                  \
  linked_list_##type##_append(list, value)
#define linked_list_element_at(type, list, index, out)                         \
  linked_list_##type##_element_at(list, index, out)

#define LINKED_LIST_PROTOTYPE(TYPE)                                            \
  typedef struct ITEM {                                                        \
    TYPE value;                                                                \
    struct ITEM *next;                                                         \
  } linked_list_##TYPE##_item;                                                 \
  typedef struct {                                                             \
    linked_list_##TYPE##_item *head;                                           \
    size_t length;                                                             \
  } linked_list_##TYPE;                                                        \
  linked_list_##TYPE make_linked_list##TYPE(void);                             \
  void linked_list_##TYPE##_append(linked_list_##TYPE *, TYPE);                \
  int linked_list_##TYPE##_element_at(linked_list_##TYPE *, size_t, TYPE *out);

#define LINKED_LIST_IMPL(TYPE)                                                 \
  linked_list_##TYPE make_linked_list_##TYPE(void) {                           \
    return (linked_list_##TYPE){.head = NULL, .length = 0};                    \
  }                                                                            \
  linked_list_##TYPE##_item *make_linked_list_##TYPE##_item(int value) {       \
    linked_list_##TYPE##_item *item =                                          \
        malloc(sizeof(linked_list_##TYPE##_item));                             \
    item->value = value;                                                       \
    item->next = NULL;                                                         \
    return item;                                                               \
  }                                                                            \
  void linked_list_##TYPE##_append(linked_list_##TYPE *list, TYPE value) {     \
    if (list->head == NULL) {                                                  \
      list->head = make_linked_list_##TYPE##_item(value);                      \
      list->length = 1;                                                        \
      return;                                                                  \
    }                                                                          \
    linked_list_##TYPE##_item *current = list->head;                           \
    while (current->next != NULL) {                                            \
      current = current->next;                                                 \
    }                                                                          \
    current->next = make_linked_list_##TYPE##_item(value);                     \
    list->length++;                                                            \
  }                                                                            \
  int linked_list_##TYPE##_element_at(linked_list_##TYPE *list, size_t index,  \
                                      TYPE *out) {                             \
    if (list->length == 0) {                                                   \
      return -1;                                                               \
    }                                                                          \
    size_t current_index = 0;                                                  \
    linked_list_##TYPE##_item *current_item = list->head;                      \
    do {                                                                       \
      if (current_index == index) {                                            \
        *out = current_item->value;                                            \
        return 0;                                                              \
      }                                                                        \
      current_index++;                                                         \
      current_item = current_item->next;                                       \
    } while (current_item != NULL);                                            \
    return 1;                                                                  \
  }

#endif
