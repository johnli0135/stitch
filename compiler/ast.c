#include "misc.h"
#include "ast.h"
#include <stdlib.h>
#include <stdio.h>

node_t node_prgm(vec_t funcs) {
  node_t e = malloc(sizeof(*e));
  e->is = EXP_PRGM;
  e->as.prgm = funcs;
  return e;
}

node_t node_func(sid_t f, vec_t args, node_t ret, node_t body) {
  node_t e = malloc(sizeof(*e));
  e->is = EXP_FUNC;
  e->as.func.f = f;
  e->as.func.args = args;
  e->as.func.ret = ret;
  e->as.func.body = body;
  return e;
}

node_t node_let(sid_t x, node_t t, node_t e) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_LET;
  r->as.let.x = x;
  r->as.let.t = t;
  r->as.let.e = e;
  return r;
}

node_t node_set(node_t x, node_t e) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_SET;
  r->as.set.x = x;
  r->as.set.e = e;
  return r;
}

node_t node_body(vec_t stmts, node_t ret) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_BODY;
  r->as.body.stmts = stmts;
  r->as.body.ret = ret;
  return r;
}

node_t node_ty_record(vec_t fields) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_TY_RECORD;
  r->as.ty_record = fields;
  return r;
}

node_t node_ty_variant(vec_t fields) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_TY_VARIANT;
  r->as.ty_variant = fields;
  return r;
}

node_t node_ty_ptr(node_t ty) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_TY_PTR;
  r->as.ty_ptr = ty;
  return r;
}

node_t node_fptr(vec_t args, node_t ret) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_FPTR;
  r->as.fptr.args = args;
  r->as.fptr.ret = ret;
  return r;
}

node_t node_id(sid_t id) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_ID;
  r->as.id = id;
  return r;
}

node_t node_str(sid_t str) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_STR;
  r->as.str = str;
  return r;
}

node_t node_num(int num) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_NUM;
  r->as.num = num;
  return r;
}

node_t node_uop(uop_t op, node_t e) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_UOP;
  r->as.uop.op = op;
  r->as.uop.e = e;
  return r;
}

node_t node_bop(node_t l, bop_t op, node_t r) {
  node_t e = malloc(sizeof(*e));
  e->is = EXP_BOP;
  e->as.bop.l = l;
  e->as.bop.op = op;
  e->as.bop.r = r;
  return e;
}

node_t node_proj(node_t e, sid_t id) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_PROJ;
  r->as.proj.e = e;
  r->as.proj.id = id;
  return r;
}

node_t node_index(node_t e, node_t i) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_INDEX;
  r->as.index.e = e;
  r->as.index.i = i;
  return r;
}

node_t node_call(node_t f, vec_t args) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_CALL;
  r->as.call.f = f;
  r->as.call.args = args;
  return r;
}

node_t node_record(vec_t fields) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_RECORD;
  r->as.record = fields;
  return r;
}

node_t node_variant(sid_t name, node_t e) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_VARIANT;
  r->as.variant.name = name;
  r->as.variant.e = e;
  return r;
}

node_t node_match(node_t e, vec_t arms) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_MATCH;
  r->as.match.e = e;
  r->as.match.arms = arms;
  return r;
}

node_t node_arm(sid_t ctr, sid_t x, node_t e) {
  node_t r = malloc(sizeof(*r));
  r->is = EXP_ARM;
  r->as.arm.ctr = ctr;
  r->as.arm.x = x;
  r->as.arm.e = e;
  return r;
}

node_t node_vec(vec_t v) {
  node_t e = malloc(sizeof(*e));
  e->is = EXP_VEC;
  e->as.vec = v;
  return e;
}

node_t node_pair(pair_t p) {
  node_t e = malloc(sizeof(*e));
  e->is = EXP_PAIR;
  e->as.pair = p;
  return e;
}

void field_del(pair_t id_exp) {
  if (id_exp->b)
    node_del(id_exp->b);
  free(id_exp);
}

void node_del(node_t e) {
  switch (e->is) {
    case EXP_PRGM: vec_del(e->as.prgm, (del_t)node_del); break;
    case EXP_FUNC:
      vec_del(e->as.func.args, (del_t)field_del);
      if (e->as.func.ret)
        node_del(e->as.func.ret);
      node_del(e->as.func.body);
    break;
    case EXP_LET:
      if (e->as.let.t)
        node_del(e->as.let.t);
      node_del(e->as.let.e);
    break;
    case EXP_SET:
      node_del(e->as.set.x);
      node_del(e->as.set.e);
    break;
    case EXP_BODY:
      vec_del(e->as.body.stmts, (del_t)node_del);
      node_del(e->as.body.ret);
    break;
    case EXP_TY_RECORD: vec_del(e->as.ty_record, (del_t)field_del); break;
    case EXP_TY_VARIANT: vec_del(e->as.ty_variant, (del_t)field_del); break;
    case EXP_TY_PTR: node_del(e->as.ty_ptr); break;
    case EXP_FPTR:
      vec_del(e->as.fptr.args, (del_t)node_del);
      node_del(e->as.fptr.ret);
    break;
    case EXP_ID: break;
    case EXP_NUM: break;
    case EXP_STR: break;
    case EXP_UOP: node_del(e->as.uop.e); break;
    case EXP_BOP:
      node_del(e->as.bop.l);
      node_del(e->as.bop.r);
    break;
    case EXP_PROJ: node_del(e->as.proj.e); break;
    case EXP_INDEX:
      node_del(e->as.index.e);
      node_del(e->as.index.i);
    break;
    case EXP_CALL:
      node_del(e->as.call.f);
      vec_del(e->as.call.args, (del_t)node_del);
    break;
    case EXP_RECORD:
      vec_del(e->as.record, (del_t)field_del);
    break;
    case EXP_VARIANT: node_del(e->as.variant.e); break;
    case EXP_MATCH:
      node_del(e->as.match.e);
      vec_del(e->as.match.arms, (del_t)node_del);
    break;
    case EXP_ARM: node_del(e->as.arm.e); break;
  } 
  free(e);
}

void node_pp_newline(FILE *fp, int lvl) {
  fputc('\n', fp);
  for (int i = 0; i < lvl; ++i)
    fputc(' ', fp);
}

void uop_pp(FILE *fp, uop_t op) {
  switch (op) {
    case UOP_NEG: fputc('-', fp); break;
    case UOP_REF: fputc('&', fp); break;
    case UOP_DEREF: fputc('*', fp); break;
  }
}

void bop_pp(FILE *fp, bop_t op) {
  switch (op) {
    case BOP_ADD: fputc('+', fp); break;
    case BOP_MUL: fputc('*', fp); break;
  }
}

void node_pp_(stab_t t, FILE *fp, node_t e, int lvl);
void node_pp_rhs(stab_t t, FILE *fp, node_t e, int lvl, int semi) {
  switch (e->is) {
    case EXP_BODY:
      fputc('(', fp);
      node_pp_newline(fp, lvl + 2);
      node_pp_(t, fp, e, lvl + 2);
      node_pp_newline(fp, lvl);
      if (semi)
        fprintf(fp, ");");
      else
        fputc(')', fp);
      node_pp_newline(fp, lvl);
    break;
    default:
      node_pp_(t, fp, e, lvl);
      if (semi)
        fputc(';', fp);
      node_pp_newline(fp, lvl);
  }
}

void node_pp_field(stab_t t, FILE *fp, pair_t id_e, int lvl, char const *sep) {
  fprintf(fp, "%s", stab_get(t, (sid_t)(size_t)(id_e->a)));
  if (id_e->b) {
    fprintf(fp, "%s", sep);
    node_pp_(t, fp, id_e->b, lvl);
  }
}

void node_pp_(stab_t t, FILE *fp, node_t e, int lvl) {
  switch (e->is) {
    case EXP_PRGM:
      for (int i = 0; i < vec_len(e->as.prgm); ++i) {
        node_pp_(t, fp, e->as.prgm[i], lvl);
        node_pp_newline(fp, lvl);
      }
    break;
    case EXP_FUNC:
      fprintf(fp, "%s(", stab_get(t, e->as.func.f));
      for (int i = 0; i < vec_len(e->as.func.args); ++i) {
        node_pp_field(t, fp, e->as.func.args[i], lvl, ": ");
        if (i < vec_len(e->as.func.args) - 1)
          fprintf(fp, ", ");
      }
      fprintf(fp, ")");
      if (e->as.func.ret) {
        fprintf(fp, ": ");
        node_pp_(t, fp, e->as.func.ret, lvl);
      }
      fprintf(fp, " =");
      node_pp_newline(fp, lvl + 2);
      node_pp_(t, fp, e->as.func.body, lvl + 2);
    break;
    case EXP_LET:
      fprintf(fp, "%s", stab_get(t, e->as.let.x));
      if (e->as.let.t) {
        fprintf(fp, ": ");
        node_pp_(t, fp, e->as.let.t, lvl);
      }
      fprintf(fp, " = ");
      node_pp_rhs(t, fp, e->as.let.e, lvl, 1);
    break;
    case EXP_SET:
      node_pp_(t, fp, e->as.set.x, lvl);
      fprintf(fp, " := ");
      node_pp_rhs(t, fp, e->as.set.e, lvl, 1);
    break;
    case EXP_BODY:
      for (int i = 0; i < vec_len(e->as.body.stmts); ++i)
        node_pp_(t, fp, e->as.body.stmts[i], lvl);
      node_pp_(t, fp, e->as.body.ret, lvl);
    break;
    case EXP_TY_RECORD:
      fputc('{', fp);
      for (int i = 0; i < vec_len(e->as.ty_record); ++i) {
        node_pp_field(t, fp, e->as.ty_record[i], lvl, ": ");
        if (i < vec_len(e->as.ty_record) - 1)
          fprintf(fp, ", ");
      }
      fputc('}', fp);
    break;
    case EXP_TY_VARIANT:
      fputc('<', fp);
      for (int i = 0; i < vec_len(e->as.ty_variant); ++i) {
        node_pp_field(t, fp, e->as.ty_variant[i], lvl, ": ");
        if (i < vec_len(e->as.ty_variant) - 1)
          fprintf(fp, ", ");
      }
      fputc('>', fp);
    break;
    case EXP_TY_PTR:
      fputc('*', fp);
      node_pp_(t, fp, e->as.ty_ptr, lvl);
    break;
    case EXP_FPTR:
      fputc('(', fp);
      for (int i = 0; i < vec_len(e->as.fptr.args); ++i) {
        node_pp_(t, fp, e->as.fptr.args[i], lvl);
        if (i < vec_len(e->as.fptr.args) - 1)
          fprintf(fp, ", ");
      }
      fprintf(fp, ") -> ");
      node_pp_(t, fp, e->as.fptr.ret, lvl);
    break;
    case EXP_ID: fprintf(fp, "%s", stab_get(t, e->as.id)); break;
    case EXP_NUM: fprintf(fp, "%d", e->as.num); break;
    case EXP_STR: fprintf(fp, "%s", stab_get(t, e->as.str)); break;
    case EXP_UOP:
      fputc('(', fp);
      uop_pp(fp, e->as.uop.op);
      node_pp_(t, fp, e->as.uop.e, lvl);
      fputc(')', fp);
    break;
    case EXP_BOP:
      fputc('(', fp);
      node_pp_(t, fp, e->as.bop.l, lvl);
      fputc(' ', fp);
      bop_pp(fp, e->as.bop.op);
      fputc(' ', fp);
      node_pp_(t, fp, e->as.bop.r, lvl);
      fputc(')', fp);
    break;
    case EXP_PROJ:
      fputc('(', fp);
      node_pp_(t, fp, e->as.proj.e, lvl);
      fprintf(fp, ".%s)", stab_get(t, e->as.proj.id));
    break;
    case EXP_INDEX:
      fputc('(', fp);
      node_pp_(t, fp, e->as.index.e, lvl);
      fputc('[', fp);
      node_pp_(t, fp, e->as.index.i, lvl);
      fprintf(fp, "])");
    break;
    case EXP_CALL:
      node_pp_(t, fp, e->as.call.f, lvl);
      fputc('(', fp);
      for (int i = 0; i < vec_len(e->as.call.args); ++i) {
        node_pp_(t, fp, e->as.call.args[i], lvl);
        if (i < vec_len(e->as.call.args) - 1)
          fprintf(fp, ", ");
      }
      fputc(')', fp);
    break;
    case EXP_RECORD:
      fputc('{', fp);
      for (int i = 0; i < vec_len(e->as.record); ++i) {
        node_pp_field(t, fp, e->as.ty_variant[i], lvl, " = ");
        if (i < vec_len(e->as.record) - 1)
          fprintf(fp, ", ");
      }
      fputc('}', fp);
    break;
    case EXP_VARIANT:
      fprintf(fp, "%s@(", stab_get(t, e->as.variant.name));
      node_pp_(t, fp, e->as.variant.e, lvl);
      fputc(')', fp);
    break;
    case EXP_MATCH:
      fprintf(fp, "case ");
      node_pp_(t, fp, e->as.match.e, lvl);
      node_pp_newline(fp, lvl);
      for (int i = 0; i < vec_len(e->as.match.arms); ++i)
        node_pp_(t, fp, e->as.match.arms[i], lvl);
      fprintf(fp, "end");
      node_pp_newline(fp, lvl);
    break;
    case EXP_ARM:
      fprintf(fp, "| %s@%s -> ", 
        stab_get(t, e->as.arm.ctr),
        stab_get(t, e->as.arm.x));
      node_pp_rhs(t, fp, e->as.arm.e, lvl, 0);
    break;
  }
}

void node_pp(stab_t t, FILE *fp, node_t e) { node_pp_(t, fp, e, 0); }

int node_is_ty(node_t e) {
  switch (e->is) {
    case EXP_TY_RECORD:
    case EXP_TY_VARIANT:
    case EXP_TY_PTR:
    case EXP_FPTR:
    case EXP_ID: 
      return 1;
    default:
      return 0;
  }
}

int node_is_tm(node_t e) {
  switch (e->is) {
    case EXP_LET:
    case EXP_SET:
    case EXP_BODY:
    case EXP_ID:
    case EXP_NUM:
    case EXP_STR:
    case EXP_UOP:
    case EXP_BOP:
    case EXP_PROJ:
    case EXP_INDEX:
    case EXP_CALL:
    case EXP_RECORD:
    case EXP_VARIANT:
    case EXP_MATCH:
      return 1;
    default:
      return 0;
  }
}