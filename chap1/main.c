#include "prog1.h"
#include <stdio.h>

int maxargs_exp(A_exp exp);
int maxargs(A_stm stm);
int maxargs_expList(A_expList expList);

int maxargs(A_stm stm) {
  switch (stm->kind) {
  case A_compoundStm: {
    int m1 = maxargs(stm->u.compound.stm1);
    int m2 = maxargs(stm->u.compound.stm2);
    return m1 > m2 ? m1 : m2;
  }
  case A_assignStm:
    return maxargs_exp(stm->u.assign.exp);
  case A_printStm:
    return maxargs_expList(stm->u.print.exps);
  }

  return 0;
}

int maxargs_exp(A_exp exp) {
  switch (exp->kind) {
  case A_idExp:
    return 0;
  case A_numExp:
    return 0;
  case A_opExp: {
    int l = maxargs_exp(exp->u.op.left);
    int r = maxargs_exp(exp->u.op.right);
    return l > r ? l : r;
  }
  case A_eseqExp: {
    int m1 = maxargs(exp->u.eseq.stm);
    int m2 = maxargs_exp(exp->u.eseq.exp);
    return m1 > m2 ? m1 : m2;
  }
  }
  return 0;
}

int maxargs_expList(A_expList expList) {
  switch (expList->kind) {
  case A_pairExpList: {
    int count = 1;
    int m1 = maxargs_exp(expList->u.pair.head);
    int m2 = maxargs_expList(expList->u.pair.tail);
    count += m2;
    int max_subexp = m1 > m2 ? m1 : m2;
    return count > max_subexp ? count : max_subexp;
  }

  case A_lastExpList:
    return 1 + maxargs_exp(expList->u.last);
  }

  return 0;
}

int main() {
  A_stm p = prog();

  int max = maxargs(p);

  printf("Print max args result: %d\n", max);

  return 0;
}
