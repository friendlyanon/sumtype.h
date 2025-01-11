#include <stdint.h>
#include <stdio.h>

#include "sumtype.h"
#include "tree.h"

Sumtype(Haha, (char const*, name), (uint16_t, id), (uint32_t, var))

// clang-format off
Sumtype(
  ManyVariants,
  (char const*, var0),
  (char const*, var1),
  (char const*, var2),
  (char const*, var3),
  (char const*, var4),
  (char const*, var5),
  (char const*, var6),
  (char const*, var7),
  (char const*, var8),
  (char const*, var9),
  (char const*, var10),
  (char const*, var11),
  (char const*, var12),
  (char const*, var13),
  (char const*, var14),
  (char const*, var15),
  (char const*, var16),
  (char const*, var17),
  (char const*, var18),
  (char const*, var19),
  (char const*, var20),
  (char const*, var21),
  (char const*, var22),
  (char const*, var23),
  (char const*, var24),
  (char const*, var25),
  (char const*, var26),
  (char const*, var27),
  (char const*, var28),
  (char const*, var29),
  (char const*, var30),
  (char const*, var31),
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
    let((const, name), string) {
      iflet((const, name), something, str)
        puts(*str);
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

  iflet(var11, many)
    puts("Never reached");
  else
    puts("else");

  iflet((const, var31), many, string) {
    puts(*string);
  }

  return 0;
}
