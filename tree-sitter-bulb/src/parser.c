#include "tree_sitter/parser.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 14
#define STATE_COUNT 31
#define LARGE_STATE_COUNT 4
#define SYMBOL_COUNT 49
#define ALIAS_COUNT 0
#define TOKEN_COUNT 36
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 0
#define MAX_ALIAS_SEQUENCE_LENGTH 4
#define PRODUCTION_ID_COUNT 1

enum ts_symbol_identifiers {
  anon_sym_push = 1,
  anon_sym_pop = 2,
  sym_add = 3,
  sym_sub = 4,
  sym_and = 5,
  sym_or = 6,
  anon_sym_call = 7,
  sym_ret = 8,
  anon_sym_asm = 9,
  anon_sym_LBRACE = 10,
  anon_sym_RBRACE = 11,
  sym_asm_raw = 12,
  anon_sym_const = 13,
  anon_sym_args = 14,
  anon_sym_local = 15,
  anon_sym_jmp = 16,
  anon_sym_jeq = 17,
  anon_sym_jnq = 18,
  anon_sym_jlt = 19,
  anon_sym_jlq = 20,
  anon_sym_jgt = 21,
  anon_sym_jgq = 22,
  anon_sym_PERCENT = 23,
  anon_sym_sp = 24,
  anon_sym_bsp = 25,
  anon_sym_x = 26,
  anon_sym_pc = 27,
  anon_sym_POUND = 28,
  aux_sym_comment_token1 = 29,
  anon_sym_0x = 30,
  aux_sym_constant_token1 = 31,
  aux_sym_constant_token2 = 32,
  anon_sym_DOLLAR = 33,
  anon_sym_COLON = 34,
  sym_identifier = 35,
  sym_source_file = 36,
  sym_push = 37,
  sym_pop = 38,
  sym_call = 39,
  sym_asm = 40,
  sym_virtual_memory_segment = 41,
  sym_jmp = 42,
  sym_push_pop_arg = 43,
  sym_predefined_symbol = 44,
  sym_comment = 45,
  sym_constant = 46,
  sym_label_definition = 47,
  aux_sym_source_file_repeat1 = 48,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [anon_sym_push] = "push",
  [anon_sym_pop] = "pop",
  [sym_add] = "add",
  [sym_sub] = "sub",
  [sym_and] = "and",
  [sym_or] = "or",
  [anon_sym_call] = "call",
  [sym_ret] = "ret",
  [anon_sym_asm] = "asm",
  [anon_sym_LBRACE] = "{",
  [anon_sym_RBRACE] = "}",
  [sym_asm_raw] = "asm_raw",
  [anon_sym_const] = "const",
  [anon_sym_args] = "args",
  [anon_sym_local] = "local",
  [anon_sym_jmp] = "jmp",
  [anon_sym_jeq] = "jeq",
  [anon_sym_jnq] = "jnq",
  [anon_sym_jlt] = "jlt",
  [anon_sym_jlq] = "jlq",
  [anon_sym_jgt] = "jgt",
  [anon_sym_jgq] = "jgq",
  [anon_sym_PERCENT] = "%",
  [anon_sym_sp] = "sp",
  [anon_sym_bsp] = "bsp",
  [anon_sym_x] = "x",
  [anon_sym_pc] = "pc",
  [anon_sym_POUND] = "#",
  [aux_sym_comment_token1] = "comment_token1",
  [anon_sym_0x] = "0x",
  [aux_sym_constant_token1] = "constant_token1",
  [aux_sym_constant_token2] = "constant_token2",
  [anon_sym_DOLLAR] = "$",
  [anon_sym_COLON] = ":",
  [sym_identifier] = "identifier",
  [sym_source_file] = "source_file",
  [sym_push] = "push",
  [sym_pop] = "pop",
  [sym_call] = "call",
  [sym_asm] = "asm",
  [sym_virtual_memory_segment] = "virtual_memory_segment",
  [sym_jmp] = "jmp",
  [sym_push_pop_arg] = "push_pop_arg",
  [sym_predefined_symbol] = "predefined_symbol",
  [sym_comment] = "comment",
  [sym_constant] = "constant",
  [sym_label_definition] = "label_definition",
  [aux_sym_source_file_repeat1] = "source_file_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [anon_sym_push] = anon_sym_push,
  [anon_sym_pop] = anon_sym_pop,
  [sym_add] = sym_add,
  [sym_sub] = sym_sub,
  [sym_and] = sym_and,
  [sym_or] = sym_or,
  [anon_sym_call] = anon_sym_call,
  [sym_ret] = sym_ret,
  [anon_sym_asm] = anon_sym_asm,
  [anon_sym_LBRACE] = anon_sym_LBRACE,
  [anon_sym_RBRACE] = anon_sym_RBRACE,
  [sym_asm_raw] = sym_asm_raw,
  [anon_sym_const] = anon_sym_const,
  [anon_sym_args] = anon_sym_args,
  [anon_sym_local] = anon_sym_local,
  [anon_sym_jmp] = anon_sym_jmp,
  [anon_sym_jeq] = anon_sym_jeq,
  [anon_sym_jnq] = anon_sym_jnq,
  [anon_sym_jlt] = anon_sym_jlt,
  [anon_sym_jlq] = anon_sym_jlq,
  [anon_sym_jgt] = anon_sym_jgt,
  [anon_sym_jgq] = anon_sym_jgq,
  [anon_sym_PERCENT] = anon_sym_PERCENT,
  [anon_sym_sp] = anon_sym_sp,
  [anon_sym_bsp] = anon_sym_bsp,
  [anon_sym_x] = anon_sym_x,
  [anon_sym_pc] = anon_sym_pc,
  [anon_sym_POUND] = anon_sym_POUND,
  [aux_sym_comment_token1] = aux_sym_comment_token1,
  [anon_sym_0x] = anon_sym_0x,
  [aux_sym_constant_token1] = aux_sym_constant_token1,
  [aux_sym_constant_token2] = aux_sym_constant_token2,
  [anon_sym_DOLLAR] = anon_sym_DOLLAR,
  [anon_sym_COLON] = anon_sym_COLON,
  [sym_identifier] = sym_identifier,
  [sym_source_file] = sym_source_file,
  [sym_push] = sym_push,
  [sym_pop] = sym_pop,
  [sym_call] = sym_call,
  [sym_asm] = sym_asm,
  [sym_virtual_memory_segment] = sym_virtual_memory_segment,
  [sym_jmp] = sym_jmp,
  [sym_push_pop_arg] = sym_push_pop_arg,
  [sym_predefined_symbol] = sym_predefined_symbol,
  [sym_comment] = sym_comment,
  [sym_constant] = sym_constant,
  [sym_label_definition] = sym_label_definition,
  [aux_sym_source_file_repeat1] = aux_sym_source_file_repeat1,
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [anon_sym_push] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_pop] = {
    .visible = true,
    .named = false,
  },
  [sym_add] = {
    .visible = true,
    .named = true,
  },
  [sym_sub] = {
    .visible = true,
    .named = true,
  },
  [sym_and] = {
    .visible = true,
    .named = true,
  },
  [sym_or] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_call] = {
    .visible = true,
    .named = false,
  },
  [sym_ret] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_asm] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LBRACE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RBRACE] = {
    .visible = true,
    .named = false,
  },
  [sym_asm_raw] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_const] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_args] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_local] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_jmp] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_jeq] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_jnq] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_jlt] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_jlq] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_jgt] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_jgq] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PERCENT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_sp] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_bsp] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_x] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_pc] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_POUND] = {
    .visible = true,
    .named = false,
  },
  [aux_sym_comment_token1] = {
    .visible = false,
    .named = false,
  },
  [anon_sym_0x] = {
    .visible = true,
    .named = false,
  },
  [aux_sym_constant_token1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_constant_token2] = {
    .visible = false,
    .named = false,
  },
  [anon_sym_DOLLAR] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COLON] = {
    .visible = true,
    .named = false,
  },
  [sym_identifier] = {
    .visible = true,
    .named = true,
  },
  [sym_source_file] = {
    .visible = true,
    .named = true,
  },
  [sym_push] = {
    .visible = true,
    .named = true,
  },
  [sym_pop] = {
    .visible = true,
    .named = true,
  },
  [sym_call] = {
    .visible = true,
    .named = true,
  },
  [sym_asm] = {
    .visible = true,
    .named = true,
  },
  [sym_virtual_memory_segment] = {
    .visible = true,
    .named = true,
  },
  [sym_jmp] = {
    .visible = true,
    .named = true,
  },
  [sym_push_pop_arg] = {
    .visible = true,
    .named = true,
  },
  [sym_predefined_symbol] = {
    .visible = true,
    .named = true,
  },
  [sym_comment] = {
    .visible = true,
    .named = true,
  },
  [sym_constant] = {
    .visible = true,
    .named = true,
  },
  [sym_label_definition] = {
    .visible = true,
    .named = true,
  },
  [aux_sym_source_file_repeat1] = {
    .visible = false,
    .named = false,
  },
};

static const TSSymbol ts_alias_sequences[PRODUCTION_ID_COUNT][MAX_ALIAS_SEQUENCE_LENGTH] = {
  [0] = {0},
};

static const uint16_t ts_non_terminal_alias_map[] = {
  0,
};

static const TSStateId ts_primary_state_ids[STATE_COUNT] = {
  [0] = 0,
  [1] = 1,
  [2] = 2,
  [3] = 3,
  [4] = 4,
  [5] = 5,
  [6] = 6,
  [7] = 7,
  [8] = 8,
  [9] = 9,
  [10] = 10,
  [11] = 11,
  [12] = 12,
  [13] = 13,
  [14] = 14,
  [15] = 15,
  [16] = 16,
  [17] = 17,
  [18] = 18,
  [19] = 19,
  [20] = 20,
  [21] = 21,
  [22] = 22,
  [23] = 23,
  [24] = 24,
  [25] = 25,
  [26] = 26,
  [27] = 27,
  [28] = 28,
  [29] = 29,
  [30] = 30,
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  eof = lexer->eof(lexer);
  switch (state) {
    case 0:
      if (eof) ADVANCE(35);
      ADVANCE_MAP(
        '#', 64,
        '$', 71,
        '%', 59,
        '0', 69,
        ':', 72,
        'a', 6,
        'b', 28,
        'c', 1,
        'j', 9,
        'l', 18,
        'o', 27,
        'p', 4,
        'r', 10,
        's', 19,
        'x', 62,
        '{', 45,
        '}', 46,
      );
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(0);
      if (('1' <= lookahead && lookahead <= '9')) ADVANCE(70);
      END_STATE();
    case 1:
      if (lookahead == 'a') ADVANCE(15);
      if (lookahead == 'o') ADVANCE(17);
      END_STATE();
    case 2:
      if (lookahead == 'a') ADVANCE(14);
      END_STATE();
    case 3:
      if (lookahead == 'b') ADVANCE(39);
      END_STATE();
    case 4:
      if (lookahead == 'c') ADVANCE(63);
      if (lookahead == 'o') ADVANCE(22);
      if (lookahead == 'u') ADVANCE(29);
      END_STATE();
    case 5:
      if (lookahead == 'c') ADVANCE(2);
      END_STATE();
    case 6:
      if (lookahead == 'd') ADVANCE(7);
      if (lookahead == 'n') ADVANCE(8);
      if (lookahead == 'r') ADVANCE(11);
      if (lookahead == 's') ADVANCE(16);
      END_STATE();
    case 7:
      if (lookahead == 'd') ADVANCE(38);
      END_STATE();
    case 8:
      if (lookahead == 'd') ADVANCE(40);
      END_STATE();
    case 9:
      if (lookahead == 'e') ADVANCE(23);
      if (lookahead == 'g') ADVANCE(24);
      if (lookahead == 'l') ADVANCE(25);
      if (lookahead == 'm') ADVANCE(21);
      if (lookahead == 'n') ADVANCE(26);
      END_STATE();
    case 10:
      if (lookahead == 'e') ADVANCE(32);
      END_STATE();
    case 11:
      if (lookahead == 'g') ADVANCE(30);
      END_STATE();
    case 12:
      if (lookahead == 'h') ADVANCE(36);
      END_STATE();
    case 13:
      if (lookahead == 'l') ADVANCE(42);
      END_STATE();
    case 14:
      if (lookahead == 'l') ADVANCE(51);
      END_STATE();
    case 15:
      if (lookahead == 'l') ADVANCE(13);
      END_STATE();
    case 16:
      if (lookahead == 'm') ADVANCE(44);
      END_STATE();
    case 17:
      if (lookahead == 'n') ADVANCE(31);
      END_STATE();
    case 18:
      if (lookahead == 'o') ADVANCE(5);
      END_STATE();
    case 19:
      if (lookahead == 'p') ADVANCE(60);
      if (lookahead == 'u') ADVANCE(3);
      END_STATE();
    case 20:
      if (lookahead == 'p') ADVANCE(61);
      END_STATE();
    case 21:
      if (lookahead == 'p') ADVANCE(52);
      END_STATE();
    case 22:
      if (lookahead == 'p') ADVANCE(37);
      END_STATE();
    case 23:
      if (lookahead == 'q') ADVANCE(53);
      END_STATE();
    case 24:
      if (lookahead == 'q') ADVANCE(58);
      if (lookahead == 't') ADVANCE(57);
      END_STATE();
    case 25:
      if (lookahead == 'q') ADVANCE(56);
      if (lookahead == 't') ADVANCE(55);
      END_STATE();
    case 26:
      if (lookahead == 'q') ADVANCE(54);
      END_STATE();
    case 27:
      if (lookahead == 'r') ADVANCE(41);
      END_STATE();
    case 28:
      if (lookahead == 's') ADVANCE(20);
      END_STATE();
    case 29:
      if (lookahead == 's') ADVANCE(12);
      END_STATE();
    case 30:
      if (lookahead == 's') ADVANCE(50);
      END_STATE();
    case 31:
      if (lookahead == 's') ADVANCE(33);
      END_STATE();
    case 32:
      if (lookahead == 't') ADVANCE(43);
      END_STATE();
    case 33:
      if (lookahead == 't') ADVANCE(49);
      END_STATE();
    case 34:
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(34);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'F') ||
          ('a' <= lookahead && lookahead <= 'f')) ADVANCE(68);
      END_STATE();
    case 35:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 36:
      ACCEPT_TOKEN(anon_sym_push);
      END_STATE();
    case 37:
      ACCEPT_TOKEN(anon_sym_pop);
      END_STATE();
    case 38:
      ACCEPT_TOKEN(sym_add);
      END_STATE();
    case 39:
      ACCEPT_TOKEN(sym_sub);
      END_STATE();
    case 40:
      ACCEPT_TOKEN(sym_and);
      END_STATE();
    case 41:
      ACCEPT_TOKEN(sym_or);
      END_STATE();
    case 42:
      ACCEPT_TOKEN(anon_sym_call);
      END_STATE();
    case 43:
      ACCEPT_TOKEN(sym_ret);
      END_STATE();
    case 44:
      ACCEPT_TOKEN(anon_sym_asm);
      END_STATE();
    case 45:
      ACCEPT_TOKEN(anon_sym_LBRACE);
      END_STATE();
    case 46:
      ACCEPT_TOKEN(anon_sym_RBRACE);
      END_STATE();
    case 47:
      ACCEPT_TOKEN(sym_asm_raw);
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') ADVANCE(47);
      if (lookahead != 0 &&
          lookahead != '}') ADVANCE(48);
      END_STATE();
    case 48:
      ACCEPT_TOKEN(sym_asm_raw);
      if (lookahead != 0 &&
          lookahead != '}') ADVANCE(48);
      END_STATE();
    case 49:
      ACCEPT_TOKEN(anon_sym_const);
      END_STATE();
    case 50:
      ACCEPT_TOKEN(anon_sym_args);
      END_STATE();
    case 51:
      ACCEPT_TOKEN(anon_sym_local);
      END_STATE();
    case 52:
      ACCEPT_TOKEN(anon_sym_jmp);
      END_STATE();
    case 53:
      ACCEPT_TOKEN(anon_sym_jeq);
      END_STATE();
    case 54:
      ACCEPT_TOKEN(anon_sym_jnq);
      END_STATE();
    case 55:
      ACCEPT_TOKEN(anon_sym_jlt);
      END_STATE();
    case 56:
      ACCEPT_TOKEN(anon_sym_jlq);
      END_STATE();
    case 57:
      ACCEPT_TOKEN(anon_sym_jgt);
      END_STATE();
    case 58:
      ACCEPT_TOKEN(anon_sym_jgq);
      END_STATE();
    case 59:
      ACCEPT_TOKEN(anon_sym_PERCENT);
      END_STATE();
    case 60:
      ACCEPT_TOKEN(anon_sym_sp);
      END_STATE();
    case 61:
      ACCEPT_TOKEN(anon_sym_bsp);
      END_STATE();
    case 62:
      ACCEPT_TOKEN(anon_sym_x);
      END_STATE();
    case 63:
      ACCEPT_TOKEN(anon_sym_pc);
      END_STATE();
    case 64:
      ACCEPT_TOKEN(anon_sym_POUND);
      END_STATE();
    case 65:
      ACCEPT_TOKEN(aux_sym_comment_token1);
      if (lookahead == '\t' ||
          (0x0b <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') ADVANCE(65);
      if (lookahead != 0 &&
          (lookahead < '\t' || '\r' < lookahead)) ADVANCE(66);
      END_STATE();
    case 66:
      ACCEPT_TOKEN(aux_sym_comment_token1);
      if (lookahead != 0 &&
          lookahead != '\n') ADVANCE(66);
      END_STATE();
    case 67:
      ACCEPT_TOKEN(anon_sym_0x);
      END_STATE();
    case 68:
      ACCEPT_TOKEN(aux_sym_constant_token1);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'F') ||
          ('a' <= lookahead && lookahead <= 'f')) ADVANCE(68);
      END_STATE();
    case 69:
      ACCEPT_TOKEN(aux_sym_constant_token2);
      if (lookahead == 'x') ADVANCE(67);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(70);
      END_STATE();
    case 70:
      ACCEPT_TOKEN(aux_sym_constant_token2);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(70);
      END_STATE();
    case 71:
      ACCEPT_TOKEN(anon_sym_DOLLAR);
      END_STATE();
    case 72:
      ACCEPT_TOKEN(anon_sym_COLON);
      END_STATE();
    case 73:
      ACCEPT_TOKEN(sym_identifier);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(73);
      END_STATE();
    default:
      return false;
  }
}

static const TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0},
  [1] = {.lex_state = 0},
  [2] = {.lex_state = 0},
  [3] = {.lex_state = 0},
  [4] = {.lex_state = 0},
  [5] = {.lex_state = 0},
  [6] = {.lex_state = 0},
  [7] = {.lex_state = 0},
  [8] = {.lex_state = 0},
  [9] = {.lex_state = 0},
  [10] = {.lex_state = 0},
  [11] = {.lex_state = 0},
  [12] = {.lex_state = 0},
  [13] = {.lex_state = 0},
  [14] = {.lex_state = 0},
  [15] = {.lex_state = 0},
  [16] = {.lex_state = 0},
  [17] = {.lex_state = 0},
  [18] = {.lex_state = 0},
  [19] = {.lex_state = 0},
  [20] = {.lex_state = 0},
  [21] = {.lex_state = 0},
  [22] = {.lex_state = 65},
  [23] = {.lex_state = 34},
  [24] = {.lex_state = 0},
  [25] = {.lex_state = 73},
  [26] = {.lex_state = 0},
  [27] = {.lex_state = 47},
  [28] = {.lex_state = 0},
  [29] = {.lex_state = 73},
  [30] = {.lex_state = 0},
};

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [anon_sym_push] = ACTIONS(1),
    [anon_sym_pop] = ACTIONS(1),
    [sym_add] = ACTIONS(1),
    [sym_sub] = ACTIONS(1),
    [sym_and] = ACTIONS(1),
    [sym_or] = ACTIONS(1),
    [anon_sym_call] = ACTIONS(1),
    [sym_ret] = ACTIONS(1),
    [anon_sym_asm] = ACTIONS(1),
    [anon_sym_LBRACE] = ACTIONS(1),
    [anon_sym_RBRACE] = ACTIONS(1),
    [anon_sym_const] = ACTIONS(1),
    [anon_sym_args] = ACTIONS(1),
    [anon_sym_local] = ACTIONS(1),
    [anon_sym_jmp] = ACTIONS(1),
    [anon_sym_jeq] = ACTIONS(1),
    [anon_sym_jnq] = ACTIONS(1),
    [anon_sym_jlt] = ACTIONS(1),
    [anon_sym_jlq] = ACTIONS(1),
    [anon_sym_jgt] = ACTIONS(1),
    [anon_sym_jgq] = ACTIONS(1),
    [anon_sym_PERCENT] = ACTIONS(1),
    [anon_sym_sp] = ACTIONS(1),
    [anon_sym_bsp] = ACTIONS(1),
    [anon_sym_x] = ACTIONS(1),
    [anon_sym_pc] = ACTIONS(1),
    [anon_sym_POUND] = ACTIONS(1),
    [anon_sym_0x] = ACTIONS(1),
    [aux_sym_constant_token2] = ACTIONS(1),
    [anon_sym_DOLLAR] = ACTIONS(1),
    [anon_sym_COLON] = ACTIONS(1),
  },
  [1] = {
    [sym_source_file] = STATE(28),
    [sym_push] = STATE(3),
    [sym_pop] = STATE(3),
    [sym_call] = STATE(3),
    [sym_asm] = STATE(3),
    [sym_jmp] = STATE(3),
    [sym_comment] = STATE(3),
    [sym_label_definition] = STATE(3),
    [aux_sym_source_file_repeat1] = STATE(3),
    [ts_builtin_sym_end] = ACTIONS(3),
    [anon_sym_push] = ACTIONS(5),
    [anon_sym_pop] = ACTIONS(7),
    [sym_add] = ACTIONS(9),
    [sym_sub] = ACTIONS(9),
    [sym_and] = ACTIONS(9),
    [sym_or] = ACTIONS(9),
    [anon_sym_call] = ACTIONS(11),
    [sym_ret] = ACTIONS(9),
    [anon_sym_asm] = ACTIONS(13),
    [anon_sym_jmp] = ACTIONS(15),
    [anon_sym_jeq] = ACTIONS(15),
    [anon_sym_jnq] = ACTIONS(15),
    [anon_sym_jlt] = ACTIONS(15),
    [anon_sym_jlq] = ACTIONS(15),
    [anon_sym_jgt] = ACTIONS(15),
    [anon_sym_jgq] = ACTIONS(15),
    [anon_sym_POUND] = ACTIONS(17),
    [anon_sym_DOLLAR] = ACTIONS(19),
  },
  [2] = {
    [sym_push] = STATE(2),
    [sym_pop] = STATE(2),
    [sym_call] = STATE(2),
    [sym_asm] = STATE(2),
    [sym_jmp] = STATE(2),
    [sym_comment] = STATE(2),
    [sym_label_definition] = STATE(2),
    [aux_sym_source_file_repeat1] = STATE(2),
    [ts_builtin_sym_end] = ACTIONS(21),
    [anon_sym_push] = ACTIONS(23),
    [anon_sym_pop] = ACTIONS(26),
    [sym_add] = ACTIONS(29),
    [sym_sub] = ACTIONS(29),
    [sym_and] = ACTIONS(29),
    [sym_or] = ACTIONS(29),
    [anon_sym_call] = ACTIONS(32),
    [sym_ret] = ACTIONS(29),
    [anon_sym_asm] = ACTIONS(35),
    [anon_sym_jmp] = ACTIONS(38),
    [anon_sym_jeq] = ACTIONS(38),
    [anon_sym_jnq] = ACTIONS(38),
    [anon_sym_jlt] = ACTIONS(38),
    [anon_sym_jlq] = ACTIONS(38),
    [anon_sym_jgt] = ACTIONS(38),
    [anon_sym_jgq] = ACTIONS(38),
    [anon_sym_POUND] = ACTIONS(41),
    [anon_sym_DOLLAR] = ACTIONS(44),
  },
  [3] = {
    [sym_push] = STATE(2),
    [sym_pop] = STATE(2),
    [sym_call] = STATE(2),
    [sym_asm] = STATE(2),
    [sym_jmp] = STATE(2),
    [sym_comment] = STATE(2),
    [sym_label_definition] = STATE(2),
    [aux_sym_source_file_repeat1] = STATE(2),
    [ts_builtin_sym_end] = ACTIONS(47),
    [anon_sym_push] = ACTIONS(5),
    [anon_sym_pop] = ACTIONS(7),
    [sym_add] = ACTIONS(49),
    [sym_sub] = ACTIONS(49),
    [sym_and] = ACTIONS(49),
    [sym_or] = ACTIONS(49),
    [anon_sym_call] = ACTIONS(11),
    [sym_ret] = ACTIONS(49),
    [anon_sym_asm] = ACTIONS(13),
    [anon_sym_jmp] = ACTIONS(15),
    [anon_sym_jeq] = ACTIONS(15),
    [anon_sym_jnq] = ACTIONS(15),
    [anon_sym_jlt] = ACTIONS(15),
    [anon_sym_jlq] = ACTIONS(15),
    [anon_sym_jgt] = ACTIONS(15),
    [anon_sym_jgq] = ACTIONS(15),
    [anon_sym_POUND] = ACTIONS(17),
    [anon_sym_DOLLAR] = ACTIONS(19),
  },
};

static const uint16_t ts_small_parse_table[] = {
  [0] = 1,
    ACTIONS(51), 19,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      sym_add,
      sym_sub,
      sym_and,
      sym_or,
      anon_sym_call,
      sym_ret,
      anon_sym_asm,
      anon_sym_jmp,
      anon_sym_jeq,
      anon_sym_jnq,
      anon_sym_jlt,
      anon_sym_jlq,
      anon_sym_jgt,
      anon_sym_jgq,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [22] = 1,
    ACTIONS(53), 19,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      sym_add,
      sym_sub,
      sym_and,
      sym_or,
      anon_sym_call,
      sym_ret,
      anon_sym_asm,
      anon_sym_jmp,
      anon_sym_jeq,
      anon_sym_jnq,
      anon_sym_jlt,
      anon_sym_jlq,
      anon_sym_jgt,
      anon_sym_jgq,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [44] = 1,
    ACTIONS(55), 19,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      sym_add,
      sym_sub,
      sym_and,
      sym_or,
      anon_sym_call,
      sym_ret,
      anon_sym_asm,
      anon_sym_jmp,
      anon_sym_jeq,
      anon_sym_jnq,
      anon_sym_jlt,
      anon_sym_jlq,
      anon_sym_jgt,
      anon_sym_jgq,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [66] = 1,
    ACTIONS(57), 19,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      sym_add,
      sym_sub,
      sym_and,
      sym_or,
      anon_sym_call,
      sym_ret,
      anon_sym_asm,
      anon_sym_jmp,
      anon_sym_jeq,
      anon_sym_jnq,
      anon_sym_jlt,
      anon_sym_jlq,
      anon_sym_jgt,
      anon_sym_jgq,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [88] = 1,
    ACTIONS(59), 19,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      sym_add,
      sym_sub,
      sym_and,
      sym_or,
      anon_sym_call,
      sym_ret,
      anon_sym_asm,
      anon_sym_jmp,
      anon_sym_jeq,
      anon_sym_jnq,
      anon_sym_jlt,
      anon_sym_jlq,
      anon_sym_jgt,
      anon_sym_jgq,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [110] = 1,
    ACTIONS(61), 19,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      sym_add,
      sym_sub,
      sym_and,
      sym_or,
      anon_sym_call,
      sym_ret,
      anon_sym_asm,
      anon_sym_jmp,
      anon_sym_jeq,
      anon_sym_jnq,
      anon_sym_jlt,
      anon_sym_jlq,
      anon_sym_jgt,
      anon_sym_jgq,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [132] = 1,
    ACTIONS(63), 19,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      sym_add,
      sym_sub,
      sym_and,
      sym_or,
      anon_sym_call,
      sym_ret,
      anon_sym_asm,
      anon_sym_jmp,
      anon_sym_jeq,
      anon_sym_jnq,
      anon_sym_jlt,
      anon_sym_jlq,
      anon_sym_jgt,
      anon_sym_jgq,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [154] = 1,
    ACTIONS(65), 19,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      sym_add,
      sym_sub,
      sym_and,
      sym_or,
      anon_sym_call,
      sym_ret,
      anon_sym_asm,
      anon_sym_jmp,
      anon_sym_jeq,
      anon_sym_jnq,
      anon_sym_jlt,
      anon_sym_jlq,
      anon_sym_jgt,
      anon_sym_jgq,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [176] = 1,
    ACTIONS(67), 19,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      sym_add,
      sym_sub,
      sym_and,
      sym_or,
      anon_sym_call,
      sym_ret,
      anon_sym_asm,
      anon_sym_jmp,
      anon_sym_jeq,
      anon_sym_jnq,
      anon_sym_jlt,
      anon_sym_jlq,
      anon_sym_jgt,
      anon_sym_jgq,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [198] = 1,
    ACTIONS(69), 19,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      sym_add,
      sym_sub,
      sym_and,
      sym_or,
      anon_sym_call,
      sym_ret,
      anon_sym_asm,
      anon_sym_jmp,
      anon_sym_jeq,
      anon_sym_jnq,
      anon_sym_jlt,
      anon_sym_jlq,
      anon_sym_jgt,
      anon_sym_jgq,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [220] = 1,
    ACTIONS(71), 19,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      sym_add,
      sym_sub,
      sym_and,
      sym_or,
      anon_sym_call,
      sym_ret,
      anon_sym_asm,
      anon_sym_jmp,
      anon_sym_jeq,
      anon_sym_jnq,
      anon_sym_jlt,
      anon_sym_jlq,
      anon_sym_jgt,
      anon_sym_jgq,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [242] = 5,
    ACTIONS(73), 1,
      anon_sym_PERCENT,
    ACTIONS(75), 1,
      anon_sym_0x,
    ACTIONS(77), 1,
      aux_sym_constant_token2,
    STATE(14), 1,
      sym_push_pop_arg,
    STATE(6), 2,
      sym_predefined_symbol,
      sym_constant,
  [259] = 5,
    ACTIONS(73), 1,
      anon_sym_PERCENT,
    ACTIONS(75), 1,
      anon_sym_0x,
    ACTIONS(77), 1,
      aux_sym_constant_token2,
    STATE(10), 1,
      sym_push_pop_arg,
    STATE(6), 2,
      sym_predefined_symbol,
      sym_constant,
  [276] = 2,
    STATE(15), 1,
      sym_virtual_memory_segment,
    ACTIONS(79), 3,
      anon_sym_const,
      anon_sym_args,
      anon_sym_local,
  [285] = 2,
    STATE(16), 1,
      sym_virtual_memory_segment,
    ACTIONS(79), 3,
      anon_sym_const,
      anon_sym_args,
      anon_sym_local,
  [294] = 1,
    ACTIONS(81), 4,
      anon_sym_sp,
      anon_sym_bsp,
      anon_sym_x,
      anon_sym_pc,
  [301] = 2,
    ACTIONS(85), 1,
      aux_sym_constant_token2,
    ACTIONS(83), 2,
      anon_sym_PERCENT,
      anon_sym_0x,
  [309] = 2,
    ACTIONS(19), 1,
      anon_sym_DOLLAR,
    STATE(11), 1,
      sym_label_definition,
  [316] = 1,
    ACTIONS(87), 1,
      aux_sym_comment_token1,
  [320] = 1,
    ACTIONS(89), 1,
      aux_sym_constant_token1,
  [324] = 1,
    ACTIONS(91), 1,
      anon_sym_COLON,
  [328] = 1,
    ACTIONS(93), 1,
      sym_identifier,
  [332] = 1,
    ACTIONS(95), 1,
      anon_sym_RBRACE,
  [336] = 1,
    ACTIONS(97), 1,
      sym_asm_raw,
  [340] = 1,
    ACTIONS(99), 1,
      ts_builtin_sym_end,
  [344] = 1,
    ACTIONS(101), 1,
      sym_identifier,
  [348] = 1,
    ACTIONS(103), 1,
      anon_sym_LBRACE,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(4)] = 0,
  [SMALL_STATE(5)] = 22,
  [SMALL_STATE(6)] = 44,
  [SMALL_STATE(7)] = 66,
  [SMALL_STATE(8)] = 88,
  [SMALL_STATE(9)] = 110,
  [SMALL_STATE(10)] = 132,
  [SMALL_STATE(11)] = 154,
  [SMALL_STATE(12)] = 176,
  [SMALL_STATE(13)] = 198,
  [SMALL_STATE(14)] = 220,
  [SMALL_STATE(15)] = 242,
  [SMALL_STATE(16)] = 259,
  [SMALL_STATE(17)] = 276,
  [SMALL_STATE(18)] = 285,
  [SMALL_STATE(19)] = 294,
  [SMALL_STATE(20)] = 301,
  [SMALL_STATE(21)] = 309,
  [SMALL_STATE(22)] = 316,
  [SMALL_STATE(23)] = 320,
  [SMALL_STATE(24)] = 324,
  [SMALL_STATE(25)] = 328,
  [SMALL_STATE(26)] = 332,
  [SMALL_STATE(27)] = 336,
  [SMALL_STATE(28)] = 340,
  [SMALL_STATE(29)] = 344,
  [SMALL_STATE(30)] = 348,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 0, 0, 0),
  [5] = {.entry = {.count = 1, .reusable = true}}, SHIFT(17),
  [7] = {.entry = {.count = 1, .reusable = true}}, SHIFT(18),
  [9] = {.entry = {.count = 1, .reusable = true}}, SHIFT(3),
  [11] = {.entry = {.count = 1, .reusable = true}}, SHIFT(25),
  [13] = {.entry = {.count = 1, .reusable = true}}, SHIFT(30),
  [15] = {.entry = {.count = 1, .reusable = true}}, SHIFT(21),
  [17] = {.entry = {.count = 1, .reusable = true}}, SHIFT(22),
  [19] = {.entry = {.count = 1, .reusable = true}}, SHIFT(29),
  [21] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0),
  [23] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(17),
  [26] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(18),
  [29] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(2),
  [32] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(25),
  [35] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(30),
  [38] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(21),
  [41] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(22),
  [44] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(29),
  [47] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 1, 0, 0),
  [49] = {.entry = {.count = 1, .reusable = true}}, SHIFT(2),
  [51] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_constant, 2, 0, 0),
  [53] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_asm, 4, 0, 0),
  [55] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_push_pop_arg, 1, 0, 0),
  [57] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_predefined_symbol, 2, 0, 0),
  [59] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_label_definition, 3, 0, 0),
  [61] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_call, 2, 0, 0),
  [63] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_pop, 3, 0, 0),
  [65] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_jmp, 2, 0, 0),
  [67] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_comment, 2, 0, 0),
  [69] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_constant, 1, 0, 0),
  [71] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_push, 3, 0, 0),
  [73] = {.entry = {.count = 1, .reusable = true}}, SHIFT(19),
  [75] = {.entry = {.count = 1, .reusable = true}}, SHIFT(23),
  [77] = {.entry = {.count = 1, .reusable = false}}, SHIFT(13),
  [79] = {.entry = {.count = 1, .reusable = true}}, SHIFT(20),
  [81] = {.entry = {.count = 1, .reusable = true}}, SHIFT(7),
  [83] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_virtual_memory_segment, 1, 0, 0),
  [85] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_virtual_memory_segment, 1, 0, 0),
  [87] = {.entry = {.count = 1, .reusable = true}}, SHIFT(12),
  [89] = {.entry = {.count = 1, .reusable = true}}, SHIFT(4),
  [91] = {.entry = {.count = 1, .reusable = true}}, SHIFT(8),
  [93] = {.entry = {.count = 1, .reusable = true}}, SHIFT(9),
  [95] = {.entry = {.count = 1, .reusable = true}}, SHIFT(5),
  [97] = {.entry = {.count = 1, .reusable = true}}, SHIFT(26),
  [99] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
  [101] = {.entry = {.count = 1, .reusable = true}}, SHIFT(24),
  [103] = {.entry = {.count = 1, .reusable = true}}, SHIFT(27),
};

#ifdef __cplusplus
extern "C" {
#endif
#ifdef TREE_SITTER_HIDE_SYMBOLS
#define TS_PUBLIC
#elif defined(_WIN32)
#define TS_PUBLIC __declspec(dllexport)
#else
#define TS_PUBLIC __attribute__((visibility("default")))
#endif

TS_PUBLIC const TSLanguage *tree_sitter_bulb(void) {
  static const TSLanguage language = {
    .version = LANGUAGE_VERSION,
    .symbol_count = SYMBOL_COUNT,
    .alias_count = ALIAS_COUNT,
    .token_count = TOKEN_COUNT,
    .external_token_count = EXTERNAL_TOKEN_COUNT,
    .state_count = STATE_COUNT,
    .large_state_count = LARGE_STATE_COUNT,
    .production_id_count = PRODUCTION_ID_COUNT,
    .field_count = FIELD_COUNT,
    .max_alias_sequence_length = MAX_ALIAS_SEQUENCE_LENGTH,
    .parse_table = &ts_parse_table[0][0],
    .small_parse_table = ts_small_parse_table,
    .small_parse_table_map = ts_small_parse_table_map,
    .parse_actions = ts_parse_actions,
    .symbol_names = ts_symbol_names,
    .symbol_metadata = ts_symbol_metadata,
    .public_symbol_map = ts_symbol_map,
    .alias_map = ts_non_terminal_alias_map,
    .alias_sequences = &ts_alias_sequences[0][0],
    .lex_modes = ts_lex_modes,
    .lex_fn = ts_lex,
    .primary_state_ids = ts_primary_state_ids,
  };
  return &language;
}
#ifdef __cplusplus
}
#endif
