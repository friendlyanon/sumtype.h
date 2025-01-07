#include "tree.h"

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  include <stddef.h>
#  define UNREACHABLE() (unreachable())
#elif defined(__GNUC__) || defined(__clang__)
#  define UNREACHABLE() (__builtin_unreachable())
#elif defined(_MSC_VER)
#  define UNREACHABLE() (__assume(0))
#else
_Noreturn inline void unreachable_impl(void) {}
#  define UNREACHABLE() (unreachable_impl())
#endif

int sum(struct BinaryTree* tree)
{
#ifdef Sumtype_typeinference
  match_t(tree) {
    let_t(leaf)
      return *leaf;
    let_t(node)
      return sum(node->l) + node->x + sum(node->r);
  }
#else
  match(struct BinaryTree, tree) {
    let(int, leaf)
      return *leaf;
    let(struct Node, node)
      return sum(node->l) + node->x + sum(node->r);
  }
#endif

  UNREACHABLE();
}
