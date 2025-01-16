typedef struct ITEM {
  char *key;
  int value;
  struct ITEM *next;
} item;

typedef item hash_map[];
