#include "tree_sitter/parser.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 14
#define STATE_COUNT 26
#define LARGE_STATE_COUNT 2
#define SYMBOL_COUNT 38
#define ALIAS_COUNT 0
#define TOKEN_COUNT 27
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 0
#define MAX_ALIAS_SEQUENCE_LENGTH 5
#define PRODUCTION_ID_COUNT 1

enum ts_symbol_identifiers {
  anon_sym_DOLLAR = 1,
  anon_sym_EQ = 2,
  anon_sym_SEMI = 3,
  anon_sym_COLON = 4,
  anon_sym__ = 5,
  anon_sym_A = 6,
  anon_sym_D = 7,
  anon_sym_M = 8,
  sym_label = 9,
  anon_sym_1 = 10,
  anon_sym_JMP = 11,
  anon_sym_JEQ = 12,
  anon_sym_JLT = 13,
  anon_sym_JNQ = 14,
  anon_sym_JGT = 15,
  anon_sym_JGQ = 16,
  anon_sym_JLQ = 17,
  anon_sym_PLUS = 18,
  anon_sym_DASH = 19,
  anon_sym_AMP = 20,
  anon_sym_PIPE = 21,
  anon_sym_TILDE = 22,
  anon_sym_POUND = 23,
  aux_sym_comment_token1 = 24,
  sym_number = 25,
  sym_at_sign = 26,
  sym_source_file = 27,
  sym_address_instruction = 28,
  sym_compute_instruction = 29,
  sym_label_instruction = 30,
  sym_destination = 31,
  sym_register = 32,
  sym_jmp = 33,
  sym_operator = 34,
  sym_comment = 35,
  sym_expression = 36,
  aux_sym_source_file_repeat1 = 37,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [anon_sym_DOLLAR] = "$",
  [anon_sym_EQ] = "=",
  [anon_sym_SEMI] = ";",
  [anon_sym_COLON] = ":",
  [anon_sym__] = "_",
  [anon_sym_A] = "A",
  [anon_sym_D] = "D",
  [anon_sym_M] = "M",
  [sym_label] = "label",
  [anon_sym_1] = "1",
  [anon_sym_JMP] = "JMP",
  [anon_sym_JEQ] = "JEQ",
  [anon_sym_JLT] = "JLT",
  [anon_sym_JNQ] = "JNQ",
  [anon_sym_JGT] = "JGT",
  [anon_sym_JGQ] = "JGQ",
  [anon_sym_JLQ] = "JLQ",
  [anon_sym_PLUS] = "+",
  [anon_sym_DASH] = "-",
  [anon_sym_AMP] = "&",
  [anon_sym_PIPE] = "|",
  [anon_sym_TILDE] = "~",
  [anon_sym_POUND] = "#",
  [aux_sym_comment_token1] = "comment_token1",
  [sym_number] = "number",
  [sym_at_sign] = "at_sign",
  [sym_source_file] = "source_file",
  [sym_address_instruction] = "address_instruction",
  [sym_compute_instruction] = "compute_instruction",
  [sym_label_instruction] = "label_instruction",
  [sym_destination] = "destination",
  [sym_register] = "register",
  [sym_jmp] = "jmp",
  [sym_operator] = "operator",
  [sym_comment] = "comment",
  [sym_expression] = "expression",
  [aux_sym_source_file_repeat1] = "source_file_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [anon_sym_DOLLAR] = anon_sym_DOLLAR,
  [anon_sym_EQ] = anon_sym_EQ,
  [anon_sym_SEMI] = anon_sym_SEMI,
  [anon_sym_COLON] = anon_sym_COLON,
  [anon_sym__] = anon_sym__,
  [anon_sym_A] = anon_sym_A,
  [anon_sym_D] = anon_sym_D,
  [anon_sym_M] = anon_sym_M,
  [sym_label] = sym_label,
  [anon_sym_1] = anon_sym_1,
  [anon_sym_JMP] = anon_sym_JMP,
  [anon_sym_JEQ] = anon_sym_JEQ,
  [anon_sym_JLT] = anon_sym_JLT,
  [anon_sym_JNQ] = anon_sym_JNQ,
  [anon_sym_JGT] = anon_sym_JGT,
  [anon_sym_JGQ] = anon_sym_JGQ,
  [anon_sym_JLQ] = anon_sym_JLQ,
  [anon_sym_PLUS] = anon_sym_PLUS,
  [anon_sym_DASH] = anon_sym_DASH,
  [anon_sym_AMP] = anon_sym_AMP,
  [anon_sym_PIPE] = anon_sym_PIPE,
  [anon_sym_TILDE] = anon_sym_TILDE,
  [anon_sym_POUND] = anon_sym_POUND,
  [aux_sym_comment_token1] = aux_sym_comment_token1,
  [sym_number] = sym_number,
  [sym_at_sign] = sym_at_sign,
  [sym_source_file] = sym_source_file,
  [sym_address_instruction] = sym_address_instruction,
  [sym_compute_instruction] = sym_compute_instruction,
  [sym_label_instruction] = sym_label_instruction,
  [sym_destination] = sym_destination,
  [sym_register] = sym_register,
  [sym_jmp] = sym_jmp,
  [sym_operator] = sym_operator,
  [sym_comment] = sym_comment,
  [sym_expression] = sym_expression,
  [aux_sym_source_file_repeat1] = aux_sym_source_file_repeat1,
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [anon_sym_DOLLAR] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_EQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_SEMI] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COLON] = {
    .visible = true,
    .named = false,
  },
  [anon_sym__] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_A] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_D] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_M] = {
    .visible = true,
    .named = false,
  },
  [sym_label] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_1] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_JMP] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_JEQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_JLT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_JNQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_JGT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_JGQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_JLQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PLUS] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DASH] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_AMP] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PIPE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_TILDE] = {
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
  [sym_number] = {
    .visible = true,
    .named = true,
  },
  [sym_at_sign] = {
    .visible = true,
    .named = true,
  },
  [sym_source_file] = {
    .visible = true,
    .named = true,
  },
  [sym_address_instruction] = {
    .visible = true,
    .named = true,
  },
  [sym_compute_instruction] = {
    .visible = true,
    .named = true,
  },
  [sym_label_instruction] = {
    .visible = true,
    .named = true,
  },
  [sym_destination] = {
    .visible = true,
    .named = true,
  },
  [sym_register] = {
    .visible = true,
    .named = true,
  },
  [sym_jmp] = {
    .visible = true,
    .named = true,
  },
  [sym_operator] = {
    .visible = true,
    .named = true,
  },
  [sym_comment] = {
    .visible = true,
    .named = true,
  },
  [sym_expression] = {
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
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  eof = lexer->eof(lexer);
  switch (state) {
    case 0:
      if (eof) ADVANCE(9);
      ADVANCE_MAP(
        '#', 33,
        '$', 10,
        '&', 30,
        '+', 28,
        '-', 29,
        '1', 20,
        ':', 13,
        ';', 12,
        '=', 11,
        '@', 37,
        'A', 15,
        'D', 16,
        'J', 2,
        'M', 17,
        '_', 14,
        '|', 31,
        '~', 32,
      );
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(0);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(36);
      END_STATE();
    case 1:
      if (lookahead == '$') ADVANCE(10);
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(1);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(36);
      END_STATE();
    case 2:
      if (lookahead == 'E') ADVANCE(4);
      if (lookahead == 'G') ADVANCE(5);
      if (lookahead == 'L') ADVANCE(6);
      if (lookahead == 'M') ADVANCE(3);
      if (lookahead == 'N') ADVANCE(7);
      END_STATE();
    case 3:
      if (lookahead == 'P') ADVANCE(21);
      END_STATE();
    case 4:
      if (lookahead == 'Q') ADVANCE(22);
      END_STATE();
    case 5:
      if (lookahead == 'Q') ADVANCE(26);
      if (lookahead == 'T') ADVANCE(25);
      END_STATE();
    case 6:
      if (lookahead == 'Q') ADVANCE(27);
      if (lookahead == 'T') ADVANCE(23);
      END_STATE();
    case 7:
      if (lookahead == 'Q') ADVANCE(24);
      END_STATE();
    case 8:
      if (eof) ADVANCE(9);
      ADVANCE_MAP(
        '#', 33,
        '$', 10,
        '1', 19,
        ';', 12,
        '@', 37,
        'A', 15,
        'D', 16,
        'M', 17,
        '_', 14,
      );
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(8);
      END_STATE();
    case 9:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 10:
      ACCEPT_TOKEN(anon_sym_DOLLAR);
      END_STATE();
    case 11:
      ACCEPT_TOKEN(anon_sym_EQ);
      END_STATE();
    case 12:
      ACCEPT_TOKEN(anon_sym_SEMI);
      END_STATE();
    case 13:
      ACCEPT_TOKEN(anon_sym_COLON);
      END_STATE();
    case 14:
      ACCEPT_TOKEN(anon_sym__);
      END_STATE();
    case 15:
      ACCEPT_TOKEN(anon_sym_A);
      END_STATE();
    case 16:
      ACCEPT_TOKEN(anon_sym_D);
      END_STATE();
    case 17:
      ACCEPT_TOKEN(anon_sym_M);
      END_STATE();
    case 18:
      ACCEPT_TOKEN(sym_label);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(18);
      END_STATE();
    case 19:
      ACCEPT_TOKEN(anon_sym_1);
      END_STATE();
    case 20:
      ACCEPT_TOKEN(anon_sym_1);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(36);
      END_STATE();
    case 21:
      ACCEPT_TOKEN(anon_sym_JMP);
      END_STATE();
    case 22:
      ACCEPT_TOKEN(anon_sym_JEQ);
      END_STATE();
    case 23:
      ACCEPT_TOKEN(anon_sym_JLT);
      END_STATE();
    case 24:
      ACCEPT_TOKEN(anon_sym_JNQ);
      END_STATE();
    case 25:
      ACCEPT_TOKEN(anon_sym_JGT);
      END_STATE();
    case 26:
      ACCEPT_TOKEN(anon_sym_JGQ);
      END_STATE();
    case 27:
      ACCEPT_TOKEN(anon_sym_JLQ);
      END_STATE();
    case 28:
      ACCEPT_TOKEN(anon_sym_PLUS);
      END_STATE();
    case 29:
      ACCEPT_TOKEN(anon_sym_DASH);
      END_STATE();
    case 30:
      ACCEPT_TOKEN(anon_sym_AMP);
      END_STATE();
    case 31:
      ACCEPT_TOKEN(anon_sym_PIPE);
      END_STATE();
    case 32:
      ACCEPT_TOKEN(anon_sym_TILDE);
      END_STATE();
    case 33:
      ACCEPT_TOKEN(anon_sym_POUND);
      END_STATE();
    case 34:
      ACCEPT_TOKEN(aux_sym_comment_token1);
      if (lookahead == '\t' ||
          (0x0b <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') ADVANCE(34);
      if (lookahead != 0 &&
          (lookahead < '\t' || '\r' < lookahead)) ADVANCE(35);
      END_STATE();
    case 35:
      ACCEPT_TOKEN(aux_sym_comment_token1);
      if (lookahead != 0 &&
          lookahead != '\n') ADVANCE(35);
      END_STATE();
    case 36:
      ACCEPT_TOKEN(sym_number);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(36);
      END_STATE();
    case 37:
      ACCEPT_TOKEN(sym_at_sign);
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
  [6] = {.lex_state = 8},
  [7] = {.lex_state = 8},
  [8] = {.lex_state = 0},
  [9] = {.lex_state = 0},
  [10] = {.lex_state = 0},
  [11] = {.lex_state = 0},
  [12] = {.lex_state = 0},
  [13] = {.lex_state = 0},
  [14] = {.lex_state = 0},
  [15] = {.lex_state = 0},
  [16] = {.lex_state = 0},
  [17] = {.lex_state = 8},
  [18] = {.lex_state = 1},
  [19] = {.lex_state = 18},
  [20] = {.lex_state = 18},
  [21] = {.lex_state = 0},
  [22] = {.lex_state = 0},
  [23] = {.lex_state = 0},
  [24] = {.lex_state = 34},
  [25] = {.lex_state = 0},
};

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [anon_sym_DOLLAR] = ACTIONS(1),
    [anon_sym_EQ] = ACTIONS(1),
    [anon_sym_SEMI] = ACTIONS(1),
    [anon_sym_COLON] = ACTIONS(1),
    [anon_sym__] = ACTIONS(1),
    [anon_sym_A] = ACTIONS(1),
    [anon_sym_D] = ACTIONS(1),
    [anon_sym_M] = ACTIONS(1),
    [anon_sym_1] = ACTIONS(1),
    [anon_sym_JMP] = ACTIONS(1),
    [anon_sym_JEQ] = ACTIONS(1),
    [anon_sym_JLT] = ACTIONS(1),
    [anon_sym_JNQ] = ACTIONS(1),
    [anon_sym_JGT] = ACTIONS(1),
    [anon_sym_JGQ] = ACTIONS(1),
    [anon_sym_JLQ] = ACTIONS(1),
    [anon_sym_PLUS] = ACTIONS(1),
    [anon_sym_DASH] = ACTIONS(1),
    [anon_sym_AMP] = ACTIONS(1),
    [anon_sym_PIPE] = ACTIONS(1),
    [anon_sym_TILDE] = ACTIONS(1),
    [anon_sym_POUND] = ACTIONS(1),
    [sym_number] = ACTIONS(1),
    [sym_at_sign] = ACTIONS(1),
  },
  [1] = {
    [sym_source_file] = STATE(23),
    [sym_address_instruction] = STATE(4),
    [sym_compute_instruction] = STATE(4),
    [sym_label_instruction] = STATE(4),
    [sym_destination] = STATE(22),
    [sym_comment] = STATE(4),
    [aux_sym_source_file_repeat1] = STATE(4),
    [ts_builtin_sym_end] = ACTIONS(3),
    [anon_sym_DOLLAR] = ACTIONS(5),
    [anon_sym__] = ACTIONS(7),
    [anon_sym_A] = ACTIONS(7),
    [anon_sym_D] = ACTIONS(7),
    [anon_sym_M] = ACTIONS(7),
    [anon_sym_POUND] = ACTIONS(9),
    [sym_at_sign] = ACTIONS(11),
  },
};

static const uint16_t ts_small_parse_table[] = {
  [0] = 3,
    STATE(6), 1,
      sym_operator,
    ACTIONS(15), 5,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_AMP,
      anon_sym_PIPE,
      anon_sym_TILDE,
    ACTIONS(13), 9,
      ts_builtin_sym_end,
      anon_sym_DOLLAR,
      anon_sym_SEMI,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_POUND,
      sym_at_sign,
  [22] = 1,
    ACTIONS(17), 14,
      ts_builtin_sym_end,
      anon_sym_DOLLAR,
      anon_sym_SEMI,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_AMP,
      anon_sym_PIPE,
      anon_sym_TILDE,
      anon_sym_POUND,
      sym_at_sign,
  [39] = 7,
    ACTIONS(5), 1,
      anon_sym_DOLLAR,
    ACTIONS(9), 1,
      anon_sym_POUND,
    ACTIONS(11), 1,
      sym_at_sign,
    ACTIONS(19), 1,
      ts_builtin_sym_end,
    STATE(22), 1,
      sym_destination,
    ACTIONS(7), 4,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
    STATE(5), 5,
      sym_address_instruction,
      sym_compute_instruction,
      sym_label_instruction,
      sym_comment,
      aux_sym_source_file_repeat1,
  [68] = 7,
    ACTIONS(21), 1,
      ts_builtin_sym_end,
    ACTIONS(23), 1,
      anon_sym_DOLLAR,
    ACTIONS(29), 1,
      anon_sym_POUND,
    ACTIONS(32), 1,
      sym_at_sign,
    STATE(22), 1,
      sym_destination,
    ACTIONS(26), 4,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
    STATE(5), 5,
      sym_address_instruction,
      sym_compute_instruction,
      sym_label_instruction,
      sym_comment,
      aux_sym_source_file_repeat1,
  [97] = 3,
    ACTIONS(37), 1,
      anon_sym_1,
    STATE(8), 1,
      sym_register,
    ACTIONS(35), 9,
      ts_builtin_sym_end,
      anon_sym_DOLLAR,
      anon_sym_SEMI,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_POUND,
      sym_at_sign,
  [115] = 1,
    ACTIONS(39), 10,
      ts_builtin_sym_end,
      anon_sym_DOLLAR,
      anon_sym_SEMI,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_1,
      anon_sym_POUND,
      sym_at_sign,
  [128] = 1,
    ACTIONS(41), 9,
      ts_builtin_sym_end,
      anon_sym_DOLLAR,
      anon_sym_SEMI,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_POUND,
      sym_at_sign,
  [140] = 2,
    ACTIONS(45), 1,
      anon_sym_SEMI,
    ACTIONS(43), 8,
      ts_builtin_sym_end,
      anon_sym_DOLLAR,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_POUND,
      sym_at_sign,
  [154] = 1,
    ACTIONS(47), 8,
      ts_builtin_sym_end,
      anon_sym_DOLLAR,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_POUND,
      sym_at_sign,
  [165] = 1,
    ACTIONS(49), 8,
      ts_builtin_sym_end,
      anon_sym_DOLLAR,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_POUND,
      sym_at_sign,
  [176] = 1,
    ACTIONS(51), 8,
      ts_builtin_sym_end,
      anon_sym_DOLLAR,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_POUND,
      sym_at_sign,
  [187] = 1,
    ACTIONS(53), 8,
      ts_builtin_sym_end,
      anon_sym_DOLLAR,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_POUND,
      sym_at_sign,
  [198] = 2,
    STATE(16), 1,
      sym_jmp,
    ACTIONS(55), 7,
      anon_sym_JMP,
      anon_sym_JEQ,
      anon_sym_JLT,
      anon_sym_JNQ,
      anon_sym_JGT,
      anon_sym_JGQ,
      anon_sym_JLQ,
  [211] = 1,
    ACTIONS(57), 8,
      ts_builtin_sym_end,
      anon_sym_DOLLAR,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_POUND,
      sym_at_sign,
  [222] = 1,
    ACTIONS(59), 8,
      ts_builtin_sym_end,
      anon_sym_DOLLAR,
      anon_sym__,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_POUND,
      sym_at_sign,
  [233] = 3,
    STATE(2), 1,
      sym_register,
    STATE(9), 1,
      sym_expression,
    ACTIONS(37), 4,
      anon_sym_A,
      anon_sym_D,
      anon_sym_M,
      anon_sym_1,
  [246] = 2,
    ACTIONS(61), 1,
      anon_sym_DOLLAR,
    ACTIONS(63), 1,
      sym_number,
  [253] = 1,
    ACTIONS(65), 1,
      sym_label,
  [257] = 1,
    ACTIONS(67), 1,
      sym_label,
  [261] = 1,
    ACTIONS(69), 1,
      anon_sym_COLON,
  [265] = 1,
    ACTIONS(71), 1,
      anon_sym_EQ,
  [269] = 1,
    ACTIONS(73), 1,
      ts_builtin_sym_end,
  [273] = 1,
    ACTIONS(75), 1,
      aux_sym_comment_token1,
  [277] = 1,
    ACTIONS(77), 1,
      anon_sym_EQ,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(2)] = 0,
  [SMALL_STATE(3)] = 22,
  [SMALL_STATE(4)] = 39,
  [SMALL_STATE(5)] = 68,
  [SMALL_STATE(6)] = 97,
  [SMALL_STATE(7)] = 115,
  [SMALL_STATE(8)] = 128,
  [SMALL_STATE(9)] = 140,
  [SMALL_STATE(10)] = 154,
  [SMALL_STATE(11)] = 165,
  [SMALL_STATE(12)] = 176,
  [SMALL_STATE(13)] = 187,
  [SMALL_STATE(14)] = 198,
  [SMALL_STATE(15)] = 211,
  [SMALL_STATE(16)] = 222,
  [SMALL_STATE(17)] = 233,
  [SMALL_STATE(18)] = 246,
  [SMALL_STATE(19)] = 253,
  [SMALL_STATE(20)] = 257,
  [SMALL_STATE(21)] = 261,
  [SMALL_STATE(22)] = 265,
  [SMALL_STATE(23)] = 269,
  [SMALL_STATE(24)] = 273,
  [SMALL_STATE(25)] = 277,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 0, 0, 0),
  [5] = {.entry = {.count = 1, .reusable = true}}, SHIFT(20),
  [7] = {.entry = {.count = 1, .reusable = true}}, SHIFT(25),
  [9] = {.entry = {.count = 1, .reusable = true}}, SHIFT(24),
  [11] = {.entry = {.count = 1, .reusable = true}}, SHIFT(18),
  [13] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_expression, 1, 0, 0),
  [15] = {.entry = {.count = 1, .reusable = true}}, SHIFT(7),
  [17] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_register, 1, 0, 0),
  [19] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 1, 0, 0),
  [21] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0),
  [23] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(20),
  [26] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(25),
  [29] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(24),
  [32] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(18),
  [35] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_expression, 2, 0, 0),
  [37] = {.entry = {.count = 1, .reusable = true}}, SHIFT(3),
  [39] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_operator, 1, 0, 0),
  [41] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_expression, 3, 0, 0),
  [43] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_compute_instruction, 3, 0, 0),
  [45] = {.entry = {.count = 1, .reusable = true}}, SHIFT(14),
  [47] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_address_instruction, 2, 0, 0),
  [49] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_comment, 2, 0, 0),
  [51] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_label_instruction, 3, 0, 0),
  [53] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_address_instruction, 3, 0, 0),
  [55] = {.entry = {.count = 1, .reusable = true}}, SHIFT(15),
  [57] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_jmp, 1, 0, 0),
  [59] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_compute_instruction, 5, 0, 0),
  [61] = {.entry = {.count = 1, .reusable = true}}, SHIFT(19),
  [63] = {.entry = {.count = 1, .reusable = true}}, SHIFT(10),
  [65] = {.entry = {.count = 1, .reusable = true}}, SHIFT(13),
  [67] = {.entry = {.count = 1, .reusable = true}}, SHIFT(21),
  [69] = {.entry = {.count = 1, .reusable = true}}, SHIFT(12),
  [71] = {.entry = {.count = 1, .reusable = true}}, SHIFT(17),
  [73] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
  [75] = {.entry = {.count = 1, .reusable = true}}, SHIFT(11),
  [77] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_destination, 1, 0, 0),
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

TS_PUBLIC const TSLanguage *tree_sitter_shadow(void) {
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