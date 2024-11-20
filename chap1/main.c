#include "prog1.h"
#include <stdio.h>
#include <stdlib.h>

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

typedef struct table *Table_;
struct table {
  string id;
  int value;
  Table_ tail;
};

Table_ Table(string id, int value, struct table *tail) {
  Table_ t = malloc(sizeof(*t));
  t->id = id;
  t->value = value;
  t->tail = tail;
  return t;
}

void print_table(Table_ t) {
  if (t == NULL)
    return;
  if (t->id != NULL) {
    printf("%s = %d\n", t->id, t->value);
  }
  print_table(t->tail);
}

struct IntAndTable {
  int i;
  Table_ t;
};

void interp(A_stm);
struct IntAndTable interpExp(A_exp exp, Table_ t);
struct IntAndTable interpExpList(A_expList expList, struct IntAndTable it);

Table_ interpStm(A_stm stm, Table_ t);

void interp(A_stm stm) {
  Table_ t = interpStm(stm, NULL);

  printf("\nFinal state of variables:\n");
  print_table(t);
}

Table_ interpStm(A_stm stm, Table_ t) {
  struct IntAndTable it;
  switch (stm->kind) {
  case A_compoundStm: {
    Table_ stm_t = interpStm(stm->u.compound.stm1, t);
    return interpStm(stm->u.compound.stm2, stm_t);
  }
  case A_assignStm: {
    struct IntAndTable it = interpExp(stm->u.assign.exp, t);
    return Table(stm->u.assign.id, it.i, it.t);
  }
  case A_printStm:
    it.t = t;
    it.i = -1;
    interpExpList(stm->u.print.exps, it);
  }
  return t;
}

struct IntAndTable interpExp(A_exp exp, Table_ t) {
  struct IntAndTable r;
  switch (exp->kind) {
  case A_idExp: {
    r.t = Table(exp->u.id, -1, t);
    break;
  }
  case A_numExp: {
    r.i = exp->u.num;
    break;
  }
  case A_opExp: {
    struct IntAndTable left = interpExp(exp->u.op.left, t);
    struct IntAndTable right = interpExp(exp->u.op.right, left.t);

    switch (exp->u.op.oper) {
    case A_plus: {
      r.i = left.i + right.i;
      break;
    }
    case A_minus: {
      r.i = left.i + right.i;
      break;
    }
    case A_times: {
      r.i = left.i * right.i;
      break;
    }
    case A_div: {
      r.i = left.i / right.i;
      break;
    }
    }

    r.t = right.t;
    break;
  }
  case A_eseqExp: {
    Table_ stm_t = interpStm(exp->u.eseq.stm, t);
    return interpExp(exp->u.eseq.exp, stm_t);
  }
  }
  return r;
}

struct IntAndTable interpExpList(A_expList expList, struct IntAndTable it) {
  switch (expList->kind) {
  case A_pairExpList: {
    struct IntAndTable head = interpExp(expList->u.pair.head, it.t);
    return interpExpList(expList->u.pair.tail, head);
  }
  case A_lastExpList:
    return interpExp(expList->u.last, it.t);
  }
  return it;
}

int main() {
  A_stm p = prog();

  int max = maxargs(p);

  printf("Print max args result: %d\n", max);

  interp(prog());

  return 0;
}
