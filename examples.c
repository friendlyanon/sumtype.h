#include <stdint.h>
#include <stdio.h>

#include "sumtype.h"

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  include <stddef.h>
#  define UNREACHABLE(x) (unreachable())
#elif defined(__GNUC__)
#  define UNREACHABLE() (__builtin_unreachable())
#elif defined(_MSC_VER)
#  define UNREACHABLE() (__assume(false))
#else
_Noreturn inline void unreachable_impl(void) {}
#  define UNREACHABLE() (unreachable_impl())
#endif

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

static int sum(struct BinaryTree* tree)
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

Sumtype(Haha, (char*, name), (uint16_t, id), (uint32_t, var))

// clang-format off
Sumtype(
  ManyVariants,
  (char*, var0),
  (char*, var1),
  (char*, var2),
  (char*, var3),
  (char*, var4),
  (char*, var5),
  (char*, var6),
  (char*, var7),
  (char*, var8),
  (char*, var9),
  (char*, var10),
  (char*, var11),
  (char*, var12),
  (char*, var13),
  (char*, var14),
  (char*, var15),
  (char*, var16),
  (char*, var17),
  (char*, var18),
  (char*, var19),
  (char*, var20),
  (char*, var21),
  (char*, var22),
  (char*, var23),
  (char*, var24),
  (char*, var25),
  (char*, var26),
  (char*, var27),
  (char*, var28),
  (char*, var29),
  (char*, var30),
  (char*, var31),
)
// clang-format on

int main(void)
{
  struct Haha something = Haha_name("hello");

#ifdef Sumtype_typeinference
  match_t(&something) {
    let_t(name) {
      puts(*name);
    }
    let_t(id) {
      printf("%d\n", *id);
    }
    otherwise {
      puts("var");
    }
  }
#else
  match(struct Haha, &something) {
    let(char*, name) {
      puts(*name);
    }
    let(uint16_t, id) {
      printf("%d\n", *id);
    }
    otherwise {
      puts("var");
    }
  }
#endif

  if (MATCHES(name, &something)) {
    puts("matches!");
  }

  struct ManyVariants many = ManyVariants_var31("damn");

#ifdef Sumtype_typeinference
  iflet_t(var31, &many) {
    puts(*var31);
  }
#else
  iflet(struct ManyVariants, char*, var31, &many) {
    puts(*var31);
  }
#endif

  return 0;
}
