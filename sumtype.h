// Public domain sum types for C
// Inspired by datatype99.h by Hirrolot, preprocessor hacks from
// https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms

#ifndef SUMTYPE_H
#define SUMTYPE_H

// Indirect concatenation that fully expands all the arguments
#define ST_PRIMITIVE_CAT(a, ...) a##__VA_ARGS__
#define ST_CAT(a, ...) ST_PRIMITIVE_CAT(a, __VA_ARGS__)

// Conditionals
#define ST_IF(c) ST_PRIMITIVE_CAT(ST_IF_, c)
#define ST_IF_0(t, ...) __VA_ARGS__
#define ST_IF_1(t, ...) t
#define ST_EAT(...)
#define ST_EXPAND(...) __VA_ARGS__
#define ST_WHEN(c) ST_IF(c)(ST_EXPAND, ST_EAT)

#define ST_CHECK_N(x, n, ...) n
#define ST_CHECK(...) ST_CHECK_N(__VA_ARGS__, 0, )
#define ST_PROBE(x) x, 1,
// Check if x is something in parentheses
#define ST_IS_PAREN(x) ST_CHECK(ST_IS_PAREN_PROBE x)
#define ST_IS_PAREN_PROBE(...) ST_PROBE(~)

// As macros cannot expand recursively, we need to force multiple evaluations
// which gradually expand to an indirect alias to the original macro
// #define A 123
// DEFER(A)() expands to "A()"
// EXPAND(DEFER(A)()) expands to "123"
#define ST_EMPTY()
#define ST_DEFER(id) id ST_EMPTY()
#define ST_OBSTRUCT(...) __VA_ARGS__ ST_DEFER(ST_EMPTY)()

#define ST_EVAL(...) ST_EVAL1(ST_EVAL1(ST_EVAL1(ST_EVAL1(__VA_ARGS__))))
#define ST_EVAL1(...) ST_EVAL2(ST_EVAL2(ST_EVAL2(ST_EVAL2(__VA_ARGS__))))
#define ST_EVAL2(...) ST_EVAL3(ST_EVAL3(__VA_ARGS__))
#define ST_EVAL3(...) __VA_ARGS__

#define ST_FOREACH(BODY, INDIRECT, FIRST, ...) \
  ST_WHEN(ST_IS_PAREN(FIRST))(BODY FIRST ST_OBSTRUCT(INDIRECT)()(__VA_ARGS__))
#define ST_FOREACH_TYPED(BODY, INDIRECT, TY, FIRST, ...) \
  ST_WHEN(ST_IS_PAREN(FIRST)) \
  (BODY(TY, FIRST) ST_OBSTRUCT(INDIRECT)()(TY, __VA_ARGS__))

#define Sumtype_Tag(ty, name) SUMTYPE_TAG_##name
#define Sumtype_Tag_Comma(ty, name) Sumtype_Tag(ty, name),
#define Sumtype_Tags(...) \
  ST_FOREACH(Sumtype_Tag_Comma, Sumtype_T_Indirect, __VA_ARGS__, )
#define Sumtype_T_Indirect() Sumtype_Tags

#define Sumtype_Field(ty, name) ty name;
#define Sumtype_Fields(...) \
  ST_FOREACH(Sumtype_Field, Sumtype_F_Indirect, __VA_ARGS__, )
#define Sumtype_F_Indirect() Sumtype_Fields

#if defined(__GNUC__) || defined(__clang__)
#  define Sumtype_GCC_or_Clang
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define Sumtype_Constructor_Warn_Unused_Result [[nodiscard]]
#elif defined(Sumtype_GCC_or_Clang)
#  define Sumtype_Constructor_Warn_Unused_Result \
    __attribute__((__warn_unused_result__))
#else
#  define Sumtype_Constructor_Warn_Unused_Result
#endif

#ifdef Sumtype_GCC_or_Clang
#  define Sumtype_Constructor_Const __attribute__((__const__))
#else
#  define Sumtype_Constructor_Const
#endif

#ifdef __clang__
#  define Sumtype_Diag_Push _Pragma("clang diagnostic push")
#  define Sumtype_Diag_Unused_Function \
    _Pragma("clang diagnostic ignored \"-Wunused-function\"")
#  define Sumtype_Diag_Pop _Pragma("clang diagnostic pop")
#else
#  define Sumtype_Diag_Push
#  define Sumtype_Diag_Unused_Function
#  define Sumtype_Diag_Pop
#endif

#define Sumtype_Constructor_Attributes \
  Sumtype_Diag_Unused_Function Sumtype_Constructor_Warn_Unused_Result \
      Sumtype_Constructor_Const

#define Sumtype_Constructor_Fields(ty, name) \
  .tag = Sumtype_Tag(ty, name), .variant = {.name = name}
#define Sumtype_Decl(ty, name) ty name
#define Sumtype_Constructor(A, args) \
  Sumtype_Diag_Push Sumtype_Constructor_Attributes static inline struct A \
  ST_CAT(A, ST_CAT(_, ST_IF_0 args))(Sumtype_Decl args) \
  { \
    return (struct A) {Sumtype_Constructor_Fields args}; \
  } \
  Sumtype_Diag_Pop
#define Sumtype_Constructors(A, ...) \
  ST_FOREACH_TYPED(Sumtype_Constructor, Sumtype_C_Indirect, A, __VA_ARGS__, )
#define Sumtype_C_Indirect() Sumtype_Constructors

#define Sumtype_Impl(A, ...) \
  struct A \
  { \
    enum \
    { \
      Sumtype_Tags(__VA_ARGS__) \
    } tag; \
    union \
    { \
      Sumtype_Fields(__VA_ARGS__) \
    } variant; \
  }; \
  Sumtype_Constructors(A, __VA_ARGS__)

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define Sumtype_typeof typeof
#elif defined(Sumtype_GCC_or_Clang)
#  define Sumtype_typeof __typeof__
#endif

// Public API:

#define Sumtype(A, ...) ST_EVAL(Sumtype_Impl(A, __VA_ARGS__, ))

#define SumtypeLit(A, name, ...) \
  ((struct A) {.tag = Sumtype_Tag(, name), .variant = {.name = __VA_ARGS__}})

#define match(ty, expr) \
  for (ty* sumtype_priv_matched_val = (expr); \
       sumtype_priv_matched_val != (ty*)0; \
       sumtype_priv_matched_val = (ty*)0) \
    switch (sumtype_priv_matched_val->tag)

#define let(ty, name) \
  break; \
  case Sumtype_Tag(ty, name): \
    for (ty* name = &sumtype_priv_matched_val->variant.name; name != (ty*)0; \
         name = (ty*)0)

#ifdef Sumtype_typeof
#  define Sumtype_typeinference
#  define match_t(expr) match(Sumtype_typeof(*expr), expr)
#  define let_t(name) \
    let(Sumtype_typeof(sumtype_priv_matched_val->variant.name), name)
#  define iflet_t(name, expr) \
    iflet(Sumtype_typeof(*expr), \
          Sumtype_typeof(sumtype_priv_matched_val->variant.name), \
          name, \
          expr)
#endif

#define otherwise \
  break; \
  default:

#define iflet(mainty, ty, name, expr) \
  for (mainty* sumtype_priv_matched_val = (expr); \
       sumtype_priv_matched_val != (mainty*)0; \
       sumtype_priv_matched_val = (mainty*)0) \
    if (sumtype_priv_matched_val->tag == Sumtype_Tag(ty, name)) \
      for (ty* name = &sumtype_priv_matched_val->variant.name; name != (ty*)0; \
           name = (ty*)0)

#define MATCHES(name, expr) ((expr)->tag == Sumtype_Tag(, name))

#endif  // SUMTYPE_H
