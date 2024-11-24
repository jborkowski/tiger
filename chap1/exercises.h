#include "util.h"
typedef struct tree *T_tree;
bool member(string key, T_tree t);
T_tree insert(string key, void *binding, T_tree t);
void *lookup_t(string key, T_tree t);
