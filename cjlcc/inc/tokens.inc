#define DEF_PUNCT(name, str) DEF_TOKEN(name, str)
#include "cjlcc/inc/puncts.inc"
#undef DEF_PUNCT

#define DEF_KWORD(name, str) DEF_TOKEN(name, str)
#include "cjlcc/inc/kwords.inc"
#undef DEF_KWORD

DEF_TOKEN(END, "END")
DEF_TOKEN(ID, nullptr)
DEF_TOKEN(CONST_C, "CONST_C")
DEF_TOKEN(CONST_I, nullptr)
DEF_TOKEN(CONST_F, "CONST_F")
DEF_TOKEN(CONST_S, nullptr)