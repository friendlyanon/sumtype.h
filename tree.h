#ifndef TREE_H
#define TREE_H

#include "sumtype.h"

struct Node
{
  struct BinaryTree* l;
  int x;
  struct BinaryTree* r;
};

// clang-format off
Sumtype(
  BinaryTree,
  (int, leaf),
  (struct Node, node)
)
// clang-format on

int sum(struct BinaryTree const* tree);

#endif  // TREE_H
