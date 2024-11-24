#include "util.h"
#include <string.h>

typedef struct tree *T_tree;
struct tree {
  T_tree left;
  string key;
  void *binding;
  T_tree right;
};

T_tree Tree(T_tree l, string k, void *binding, T_tree r) {
  T_tree t = checked_malloc(sizeof(*t));
  t->left = l;
  t->key = k;
  t->right = r;
  t->binding = binding;
  return t;
}

T_tree insert(string key, void *binding, T_tree t) {
  if (t == NULL)
    return Tree(NULL, key, binding, NULL);

  else if (strcmp(key, t->key) < 0)
    return Tree(insert(key, binding, t->left), t->key, t->binding, t->right);

  else if (strcmp(key, t->key) > 0)
    return Tree(t->left, t->key, t->binding, insert(key, binding, t->right));
  else
    return Tree(t->left, t->key, t->binding, t->right);
}

bool member(string key, T_tree t) {
  if (t == NULL)
    return FALSE;
  else if (strcmp(key, t->key) < 0)
    return member(key, t->left);
  else if (strcmp(key, t->key) > 0)
    return member(key, t->right);
  else
    return TRUE;
}

void *lookup_t(string key, T_tree t) {
  if (t == NULL)
    return NULL;
  else if (strcmp(key, t->key) < 0)
    return lookup_t(key, t->left);
  else if (strcmp(key, t->key) > 0)
    return lookup_t(key, t->right);
  else
    return t->binding;
}
