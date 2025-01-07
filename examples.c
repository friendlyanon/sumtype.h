#include <stdint.h>
#include <stdio.h>

#include "sumtype.h"
#include "tree.h"

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
  {
    struct BinaryTree l = BinaryTree_leaf(-2);
    struct BinaryTree r = SumtypeLit(BinaryTree, leaf, -1);
    printf("%d\n", sum(&SumtypeLit(BinaryTree, node, {&l, 3, &r})));
  }

  struct Haha something = Haha_name("hello");

  match(something) {
    let(name) {
      puts(*name);
    }
    let(id) {
      printf("%d\n", *id);
    }
    otherwise {
      puts("var");
    }
  }

  if (MATCHES(name, something)) {
    puts("matches!");
  }

  struct ManyVariants many = ManyVariants_var31("damn");

  iflet(var31, many) {
    puts(*var31);
  }

  return 0;
}
