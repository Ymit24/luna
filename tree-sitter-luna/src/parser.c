#include "tree_sitter/parser.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 14
#define STATE_COUNT 52
#define LARGE_STATE_COUNT 2
#define SYMBOL_COUNT 38
#define ALIAS_COUNT 0
#define TOKEN_COUNT 25
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 4
#define MAX_ALIAS_SEQUENCE_LENGTH 10
#define PRODUCTION_ID_COUNT 5

enum ts_symbol_identifiers {
  sym_comment = 1,
  anon_sym_fn = 2,
  anon_sym_LPAREN = 3,
  anon_sym_RPAREN = 4,
  anon_sym_COLON = 5,
  anon_sym_LBRACE = 6,
  anon_sym_RBRACE = 7,
  anon_sym_u = 8,
  anon_sym_8 = 9,
  anon_sym_16 = 10,
  anon_sym_i = 11,
  anon_sym_void = 12,
  anon_sym_if = 13,
  anon_sym_DASH = 14,
  anon_sym_BANG = 15,
  anon_sym_STAR = 16,
  anon_sym_SLASH = 17,
  anon_sym_PLUS = 18,
  anon_sym_EQ_EQ = 19,
  anon_sym_SEMI = 20,
  anon_sym_return = 21,
  sym_identifier = 22,
  sym_string_literal = 23,
  sym_number = 24,
  sym_source_file = 25,
  sym_function = 26,
  sym_function_args = 27,
  sym_type = 28,
  sym_if = 29,
  sym__expression = 30,
  sym_unary_expression = 31,
  sym_binary_expression = 32,
  sym_statement = 33,
  sym_return = 34,
  sym_function_call = 35,
  aux_sym_source_file_repeat1 = 36,
  aux_sym_function_repeat1 = 37,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [sym_comment] = "comment",
  [anon_sym_fn] = "fn",
  [anon_sym_LPAREN] = "(",
  [anon_sym_RPAREN] = ")",
  [anon_sym_COLON] = ":",
  [anon_sym_LBRACE] = "{",
  [anon_sym_RBRACE] = "}",
  [anon_sym_u] = "u",
  [anon_sym_8] = "8",
  [anon_sym_16] = "16",
  [anon_sym_i] = "i",
  [anon_sym_void] = "void",
  [anon_sym_if] = "if",
  [anon_sym_DASH] = "-",
  [anon_sym_BANG] = "!",
  [anon_sym_STAR] = "*",
  [anon_sym_SLASH] = "/",
  [anon_sym_PLUS] = "+",
  [anon_sym_EQ_EQ] = "==",
  [anon_sym_SEMI] = ";",
  [anon_sym_return] = "return",
  [sym_identifier] = "identifier",
  [sym_string_literal] = "string_literal",
  [sym_number] = "number",
  [sym_source_file] = "source_file",
  [sym_function] = "function",
  [sym_function_args] = "function_args",
  [sym_type] = "type",
  [sym_if] = "if",
  [sym__expression] = "_expression",
  [sym_unary_expression] = "unary_expression",
  [sym_binary_expression] = "binary_expression",
  [sym_statement] = "statement",
  [sym_return] = "return",
  [sym_function_call] = "function_call",
  [aux_sym_source_file_repeat1] = "source_file_repeat1",
  [aux_sym_function_repeat1] = "function_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [sym_comment] = sym_comment,
  [anon_sym_fn] = anon_sym_fn,
  [anon_sym_LPAREN] = anon_sym_LPAREN,
  [anon_sym_RPAREN] = anon_sym_RPAREN,
  [anon_sym_COLON] = anon_sym_COLON,
  [anon_sym_LBRACE] = anon_sym_LBRACE,
  [anon_sym_RBRACE] = anon_sym_RBRACE,
  [anon_sym_u] = anon_sym_u,
  [anon_sym_8] = anon_sym_8,
  [anon_sym_16] = anon_sym_16,
  [anon_sym_i] = anon_sym_i,
  [anon_sym_void] = anon_sym_void,
  [anon_sym_if] = anon_sym_if,
  [anon_sym_DASH] = anon_sym_DASH,
  [anon_sym_BANG] = anon_sym_BANG,
  [anon_sym_STAR] = anon_sym_STAR,
  [anon_sym_SLASH] = anon_sym_SLASH,
  [anon_sym_PLUS] = anon_sym_PLUS,
  [anon_sym_EQ_EQ] = anon_sym_EQ_EQ,
  [anon_sym_SEMI] = anon_sym_SEMI,
  [anon_sym_return] = anon_sym_return,
  [sym_identifier] = sym_identifier,
  [sym_string_literal] = sym_string_literal,
  [sym_number] = sym_number,
  [sym_source_file] = sym_source_file,
  [sym_function] = sym_function,
  [sym_function_args] = sym_function_args,
  [sym_type] = sym_type,
  [sym_if] = sym_if,
  [sym__expression] = sym__expression,
  [sym_unary_expression] = sym_unary_expression,
  [sym_binary_expression] = sym_binary_expression,
  [sym_statement] = sym_statement,
  [sym_return] = sym_return,
  [sym_function_call] = sym_function_call,
  [aux_sym_source_file_repeat1] = aux_sym_source_file_repeat1,
  [aux_sym_function_repeat1] = aux_sym_function_repeat1,
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [sym_comment] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_fn] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LPAREN] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RPAREN] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COLON] = {
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
  [anon_sym_u] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_8] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_16] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_i] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_void] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_if] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DASH] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_BANG] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_STAR] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_SLASH] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PLUS] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_EQ_EQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_SEMI] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_return] = {
    .visible = true,
    .named = false,
  },
  [sym_identifier] = {
    .visible = true,
    .named = true,
  },
  [sym_string_literal] = {
    .visible = true,
    .named = true,
  },
  [sym_number] = {
    .visible = true,
    .named = true,
  },
  [sym_source_file] = {
    .visible = true,
    .named = true,
  },
  [sym_function] = {
    .visible = true,
    .named = true,
  },
  [sym_function_args] = {
    .visible = true,
    .named = true,
  },
  [sym_type] = {
    .visible = true,
    .named = true,
  },
  [sym_if] = {
    .visible = true,
    .named = true,
  },
  [sym__expression] = {
    .visible = false,
    .named = true,
  },
  [sym_unary_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_binary_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_statement] = {
    .visible = true,
    .named = true,
  },
  [sym_return] = {
    .visible = true,
    .named = true,
  },
  [sym_function_call] = {
    .visible = true,
    .named = true,
  },
  [aux_sym_source_file_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_function_repeat1] = {
    .visible = false,
    .named = false,
  },
};

enum ts_field_identifiers {
  field_args = 1,
  field_body = 2,
  field_name = 3,
  field_ret_type = 4,
};

static const char * const ts_field_names[] = {
  [0] = NULL,
  [field_args] = "args",
  [field_body] = "body",
  [field_name] = "name",
  [field_ret_type] = "ret_type",
};

static const TSFieldMapSlice ts_field_map_slices[PRODUCTION_ID_COUNT] = {
  [1] = {.index = 0, .length = 2},
  [2] = {.index = 2, .length = 3},
  [3] = {.index = 5, .length = 3},
  [4] = {.index = 8, .length = 4},
};

static const TSFieldMapEntry ts_field_map_entries[] = {
  [0] =
    {field_name, 1},
    {field_ret_type, 5},
  [2] =
    {field_body, 7},
    {field_name, 1},
    {field_ret_type, 5},
  [5] =
    {field_args, 3},
    {field_name, 1},
    {field_ret_type, 6},
  [8] =
    {field_args, 3},
    {field_body, 8},
    {field_name, 1},
    {field_ret_type, 6},
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
  [31] = 31,
  [32] = 32,
  [33] = 33,
  [34] = 34,
  [35] = 35,
  [36] = 36,
  [37] = 37,
  [38] = 38,
  [39] = 39,
  [40] = 40,
  [41] = 41,
  [42] = 42,
  [43] = 43,
  [44] = 44,
  [45] = 45,
  [46] = 46,
  [47] = 47,
  [48] = 48,
  [49] = 49,
  [50] = 50,
  [51] = 51,
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  eof = lexer->eof(lexer);
  switch (state) {
    case 0:
      if (eof) ADVANCE(18);
      ADVANCE_MAP(
        '!', 37,
        '"', 2,
        '(', 21,
        ')', 22,
        '*', 38,
        '+', 41,
        '-', 36,
        '/', 40,
        '1', 54,
        '8', 28,
        ':', 23,
        ';', 43,
        '=', 5,
        'f', 10,
        'i', 32,
        'r', 7,
        'u', 26,
        'v', 12,
        '{', 24,
        '}', 25,
      );
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(0);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(55);
      END_STATE();
    case 1:
      ADVANCE_MAP(
        '!', 37,
        '"', 2,
        '(', 21,
        ')', 22,
        '*', 38,
        '+', 41,
        '-', 36,
        '/', 39,
        ';', 43,
        '=', 5,
        '{', 24,
      );
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(1);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(55);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 2:
      if (lookahead == '"') ADVANCE(53);
      if (lookahead == '\\') ADVANCE(16);
      if (lookahead != 0 &&
          lookahead != '\n') ADVANCE(2);
      END_STATE();
    case 3:
      if (lookahead == '/') ADVANCE(19);
      END_STATE();
    case 4:
      if (lookahead == '6') ADVANCE(29);
      END_STATE();
    case 5:
      if (lookahead == '=') ADVANCE(42);
      END_STATE();
    case 6:
      if (lookahead == 'd') ADVANCE(33);
      END_STATE();
    case 7:
      if (lookahead == 'e') ADVANCE(14);
      END_STATE();
    case 8:
      if (lookahead == 'i') ADVANCE(6);
      END_STATE();
    case 9:
      if (lookahead == 'i') ADVANCE(47);
      if (lookahead == 'r') ADVANCE(46);
      if (lookahead == '}') ADVANCE(25);
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(9);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 10:
      if (lookahead == 'n') ADVANCE(20);
      END_STATE();
    case 11:
      if (lookahead == 'n') ADVANCE(44);
      END_STATE();
    case 12:
      if (lookahead == 'o') ADVANCE(8);
      END_STATE();
    case 13:
      if (lookahead == 'r') ADVANCE(11);
      END_STATE();
    case 14:
      if (lookahead == 't') ADVANCE(15);
      END_STATE();
    case 15:
      if (lookahead == 'u') ADVANCE(13);
      END_STATE();
    case 16:
      if (lookahead != 0 &&
          lookahead != '\n') ADVANCE(2);
      END_STATE();
    case 17:
      if (eof) ADVANCE(18);
      if (lookahead == '/') ADVANCE(3);
      if (lookahead == '1') ADVANCE(4);
      if (lookahead == '8') ADVANCE(27);
      if (lookahead == 'f') ADVANCE(10);
      if (lookahead == 'i') ADVANCE(31);
      if (lookahead == 'u') ADVANCE(26);
      if (lookahead == 'v') ADVANCE(12);
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(17);
      END_STATE();
    case 18:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 19:
      ACCEPT_TOKEN(sym_comment);
      if (lookahead != 0 &&
          lookahead != '\n') ADVANCE(19);
      END_STATE();
    case 20:
      ACCEPT_TOKEN(anon_sym_fn);
      END_STATE();
    case 21:
      ACCEPT_TOKEN(anon_sym_LPAREN);
      END_STATE();
    case 22:
      ACCEPT_TOKEN(anon_sym_RPAREN);
      END_STATE();
    case 23:
      ACCEPT_TOKEN(anon_sym_COLON);
      END_STATE();
    case 24:
      ACCEPT_TOKEN(anon_sym_LBRACE);
      END_STATE();
    case 25:
      ACCEPT_TOKEN(anon_sym_RBRACE);
      END_STATE();
    case 26:
      ACCEPT_TOKEN(anon_sym_u);
      END_STATE();
    case 27:
      ACCEPT_TOKEN(anon_sym_8);
      END_STATE();
    case 28:
      ACCEPT_TOKEN(anon_sym_8);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(55);
      END_STATE();
    case 29:
      ACCEPT_TOKEN(anon_sym_16);
      END_STATE();
    case 30:
      ACCEPT_TOKEN(anon_sym_16);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(55);
      END_STATE();
    case 31:
      ACCEPT_TOKEN(anon_sym_i);
      END_STATE();
    case 32:
      ACCEPT_TOKEN(anon_sym_i);
      if (lookahead == 'f') ADVANCE(34);
      END_STATE();
    case 33:
      ACCEPT_TOKEN(anon_sym_void);
      END_STATE();
    case 34:
      ACCEPT_TOKEN(anon_sym_if);
      END_STATE();
    case 35:
      ACCEPT_TOKEN(anon_sym_if);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 36:
      ACCEPT_TOKEN(anon_sym_DASH);
      END_STATE();
    case 37:
      ACCEPT_TOKEN(anon_sym_BANG);
      END_STATE();
    case 38:
      ACCEPT_TOKEN(anon_sym_STAR);
      END_STATE();
    case 39:
      ACCEPT_TOKEN(anon_sym_SLASH);
      END_STATE();
    case 40:
      ACCEPT_TOKEN(anon_sym_SLASH);
      if (lookahead == '/') ADVANCE(19);
      END_STATE();
    case 41:
      ACCEPT_TOKEN(anon_sym_PLUS);
      END_STATE();
    case 42:
      ACCEPT_TOKEN(anon_sym_EQ_EQ);
      END_STATE();
    case 43:
      ACCEPT_TOKEN(anon_sym_SEMI);
      END_STATE();
    case 44:
      ACCEPT_TOKEN(anon_sym_return);
      END_STATE();
    case 45:
      ACCEPT_TOKEN(anon_sym_return);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 46:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'e') ADVANCE(50);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 47:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'f') ADVANCE(35);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 48:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'n') ADVANCE(45);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 49:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'r') ADVANCE(48);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 50:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 't') ADVANCE(51);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 51:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'u') ADVANCE(49);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 52:
      ACCEPT_TOKEN(sym_identifier);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 53:
      ACCEPT_TOKEN(sym_string_literal);
      END_STATE();
    case 54:
      ACCEPT_TOKEN(sym_number);
      if (lookahead == '6') ADVANCE(30);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(55);
      END_STATE();
    case 55:
      ACCEPT_TOKEN(sym_number);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(55);
      END_STATE();
    default:
      return false;
  }
}

static const TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0},
  [1] = {.lex_state = 17},
  [2] = {.lex_state = 1},
  [3] = {.lex_state = 9},
  [4] = {.lex_state = 1},
  [5] = {.lex_state = 9},
  [6] = {.lex_state = 1},
  [7] = {.lex_state = 1},
  [8] = {.lex_state = 9},
  [9] = {.lex_state = 9},
  [10] = {.lex_state = 9},
  [11] = {.lex_state = 1},
  [12] = {.lex_state = 1},
  [13] = {.lex_state = 1},
  [14] = {.lex_state = 1},
  [15] = {.lex_state = 1},
  [16] = {.lex_state = 1},
  [17] = {.lex_state = 1},
  [18] = {.lex_state = 1},
  [19] = {.lex_state = 9},
  [20] = {.lex_state = 1},
  [21] = {.lex_state = 1},
  [22] = {.lex_state = 1},
  [23] = {.lex_state = 17},
  [24] = {.lex_state = 17},
  [25] = {.lex_state = 17},
  [26] = {.lex_state = 9},
  [27] = {.lex_state = 9},
  [28] = {.lex_state = 17},
  [29] = {.lex_state = 17},
  [30] = {.lex_state = 9},
  [31] = {.lex_state = 17},
  [32] = {.lex_state = 17},
  [33] = {.lex_state = 17},
  [34] = {.lex_state = 1},
  [35] = {.lex_state = 17},
  [36] = {.lex_state = 17},
  [37] = {.lex_state = 0},
  [38] = {.lex_state = 0},
  [39] = {.lex_state = 0},
  [40] = {.lex_state = 1},
  [41] = {.lex_state = 0},
  [42] = {.lex_state = 0},
  [43] = {.lex_state = 0},
  [44] = {.lex_state = 0},
  [45] = {.lex_state = 0},
  [46] = {.lex_state = 0},
  [47] = {.lex_state = 0},
  [48] = {.lex_state = 0},
  [49] = {.lex_state = 0},
  [50] = {.lex_state = 0},
  [51] = {.lex_state = 0},
};

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [sym_comment] = ACTIONS(1),
    [anon_sym_fn] = ACTIONS(1),
    [anon_sym_LPAREN] = ACTIONS(1),
    [anon_sym_RPAREN] = ACTIONS(1),
    [anon_sym_COLON] = ACTIONS(1),
    [anon_sym_LBRACE] = ACTIONS(1),
    [anon_sym_RBRACE] = ACTIONS(1),
    [anon_sym_u] = ACTIONS(1),
    [anon_sym_8] = ACTIONS(1),
    [anon_sym_16] = ACTIONS(1),
    [anon_sym_i] = ACTIONS(1),
    [anon_sym_void] = ACTIONS(1),
    [anon_sym_if] = ACTIONS(1),
    [anon_sym_DASH] = ACTIONS(1),
    [anon_sym_BANG] = ACTIONS(1),
    [anon_sym_STAR] = ACTIONS(1),
    [anon_sym_SLASH] = ACTIONS(1),
    [anon_sym_PLUS] = ACTIONS(1),
    [anon_sym_EQ_EQ] = ACTIONS(1),
    [anon_sym_SEMI] = ACTIONS(1),
    [anon_sym_return] = ACTIONS(1),
    [sym_string_literal] = ACTIONS(1),
    [sym_number] = ACTIONS(1),
  },
  [1] = {
    [sym_source_file] = STATE(51),
    [sym_function] = STATE(23),
    [aux_sym_source_file_repeat1] = STATE(23),
    [ts_builtin_sym_end] = ACTIONS(3),
    [sym_comment] = ACTIONS(5),
    [anon_sym_fn] = ACTIONS(7),
  },
};

static const uint16_t ts_small_parse_table[] = {
  [0] = 5,
    ACTIONS(9), 1,
      anon_sym_RPAREN,
    ACTIONS(13), 1,
      sym_identifier,
    ACTIONS(11), 2,
      anon_sym_DASH,
      anon_sym_BANG,
    ACTIONS(15), 2,
      sym_string_literal,
      sym_number,
    STATE(20), 4,
      sym__expression,
      sym_unary_expression,
      sym_binary_expression,
      sym_function_call,
  [21] = 7,
    ACTIONS(17), 1,
      anon_sym_RBRACE,
    ACTIONS(19), 1,
      anon_sym_if,
    ACTIONS(22), 1,
      anon_sym_return,
    ACTIONS(25), 1,
      sym_identifier,
    STATE(26), 1,
      sym_if,
    STATE(3), 2,
      sym_statement,
      aux_sym_function_repeat1,
    STATE(39), 2,
      sym_return,
      sym_function_call,
  [45] = 4,
    ACTIONS(13), 1,
      sym_identifier,
    ACTIONS(11), 2,
      anon_sym_DASH,
      anon_sym_BANG,
    ACTIONS(28), 2,
      sym_string_literal,
      sym_number,
    STATE(15), 4,
      sym__expression,
      sym_unary_expression,
      sym_binary_expression,
      sym_function_call,
  [63] = 7,
    ACTIONS(30), 1,
      anon_sym_RBRACE,
    ACTIONS(32), 1,
      anon_sym_if,
    ACTIONS(34), 1,
      anon_sym_return,
    ACTIONS(36), 1,
      sym_identifier,
    STATE(26), 1,
      sym_if,
    STATE(3), 2,
      sym_statement,
      aux_sym_function_repeat1,
    STATE(39), 2,
      sym_return,
      sym_function_call,
  [87] = 4,
    ACTIONS(13), 1,
      sym_identifier,
    ACTIONS(11), 2,
      anon_sym_DASH,
      anon_sym_BANG,
    ACTIONS(38), 2,
      sym_string_literal,
      sym_number,
    STATE(22), 4,
      sym__expression,
      sym_unary_expression,
      sym_binary_expression,
      sym_function_call,
  [105] = 4,
    ACTIONS(13), 1,
      sym_identifier,
    ACTIONS(11), 2,
      anon_sym_DASH,
      anon_sym_BANG,
    ACTIONS(40), 2,
      sym_string_literal,
      sym_number,
    STATE(21), 4,
      sym__expression,
      sym_unary_expression,
      sym_binary_expression,
      sym_function_call,
  [123] = 7,
    ACTIONS(32), 1,
      anon_sym_if,
    ACTIONS(34), 1,
      anon_sym_return,
    ACTIONS(36), 1,
      sym_identifier,
    ACTIONS(42), 1,
      anon_sym_RBRACE,
    STATE(26), 1,
      sym_if,
    STATE(3), 2,
      sym_statement,
      aux_sym_function_repeat1,
    STATE(39), 2,
      sym_return,
      sym_function_call,
  [147] = 7,
    ACTIONS(32), 1,
      anon_sym_if,
    ACTIONS(34), 1,
      anon_sym_return,
    ACTIONS(36), 1,
      sym_identifier,
    ACTIONS(44), 1,
      anon_sym_RBRACE,
    STATE(26), 1,
      sym_if,
    STATE(5), 2,
      sym_statement,
      aux_sym_function_repeat1,
    STATE(39), 2,
      sym_return,
      sym_function_call,
  [171] = 7,
    ACTIONS(32), 1,
      anon_sym_if,
    ACTIONS(34), 1,
      anon_sym_return,
    ACTIONS(36), 1,
      sym_identifier,
    ACTIONS(46), 1,
      anon_sym_RBRACE,
    STATE(26), 1,
      sym_if,
    STATE(8), 2,
      sym_statement,
      aux_sym_function_repeat1,
    STATE(39), 2,
      sym_return,
      sym_function_call,
  [195] = 2,
    ACTIONS(48), 1,
      anon_sym_LPAREN,
    ACTIONS(50), 8,
      anon_sym_RPAREN,
      anon_sym_LBRACE,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
      anon_sym_SEMI,
  [209] = 4,
    ACTIONS(13), 1,
      sym_identifier,
    ACTIONS(11), 2,
      anon_sym_DASH,
      anon_sym_BANG,
    ACTIONS(52), 2,
      sym_string_literal,
      sym_number,
    STATE(14), 4,
      sym__expression,
      sym_unary_expression,
      sym_binary_expression,
      sym_function_call,
  [227] = 4,
    ACTIONS(13), 1,
      sym_identifier,
    ACTIONS(11), 2,
      anon_sym_DASH,
      anon_sym_BANG,
    ACTIONS(54), 2,
      sym_string_literal,
      sym_number,
    STATE(18), 4,
      sym__expression,
      sym_unary_expression,
      sym_binary_expression,
      sym_function_call,
  [245] = 1,
    ACTIONS(56), 8,
      anon_sym_RPAREN,
      anon_sym_LBRACE,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
      anon_sym_SEMI,
  [256] = 1,
    ACTIONS(58), 8,
      anon_sym_RPAREN,
      anon_sym_LBRACE,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
      anon_sym_SEMI,
  [267] = 1,
    ACTIONS(60), 8,
      anon_sym_RPAREN,
      anon_sym_LBRACE,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
      anon_sym_SEMI,
  [278] = 1,
    ACTIONS(62), 8,
      anon_sym_RPAREN,
      anon_sym_LBRACE,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
      anon_sym_SEMI,
  [289] = 2,
    ACTIONS(64), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(58), 6,
      anon_sym_RPAREN,
      anon_sym_LBRACE,
      anon_sym_DASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
      anon_sym_SEMI,
  [302] = 6,
    ACTIONS(32), 1,
      anon_sym_if,
    ACTIONS(34), 1,
      anon_sym_return,
    ACTIONS(36), 1,
      sym_identifier,
    STATE(26), 1,
      sym_if,
    STATE(47), 1,
      sym_statement,
    STATE(39), 2,
      sym_return,
      sym_function_call,
  [322] = 3,
    ACTIONS(66), 1,
      anon_sym_RPAREN,
    ACTIONS(64), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(68), 3,
      anon_sym_DASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
  [335] = 3,
    ACTIONS(70), 1,
      anon_sym_LBRACE,
    ACTIONS(64), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(68), 3,
      anon_sym_DASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
  [348] = 3,
    ACTIONS(72), 1,
      anon_sym_SEMI,
    ACTIONS(64), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(68), 3,
      anon_sym_DASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
  [361] = 4,
    ACTIONS(7), 1,
      anon_sym_fn,
    ACTIONS(74), 1,
      ts_builtin_sym_end,
    ACTIONS(76), 1,
      sym_comment,
    STATE(24), 2,
      sym_function,
      aux_sym_source_file_repeat1,
  [375] = 4,
    ACTIONS(78), 1,
      ts_builtin_sym_end,
    ACTIONS(80), 1,
      sym_comment,
    ACTIONS(83), 1,
      anon_sym_fn,
    STATE(24), 2,
      sym_function,
      aux_sym_source_file_repeat1,
  [389] = 3,
    ACTIONS(88), 1,
      anon_sym_void,
    STATE(42), 1,
      sym_type,
    ACTIONS(86), 2,
      anon_sym_u,
      anon_sym_i,
  [400] = 2,
    ACTIONS(90), 1,
      anon_sym_RBRACE,
    ACTIONS(92), 3,
      anon_sym_if,
      anon_sym_return,
      sym_identifier,
  [409] = 2,
    ACTIONS(94), 1,
      anon_sym_RBRACE,
    ACTIONS(96), 3,
      anon_sym_if,
      anon_sym_return,
      sym_identifier,
  [418] = 3,
    ACTIONS(88), 1,
      anon_sym_void,
    STATE(43), 1,
      sym_type,
    ACTIONS(86), 2,
      anon_sym_u,
      anon_sym_i,
  [429] = 3,
    ACTIONS(88), 1,
      anon_sym_void,
    STATE(44), 1,
      sym_type,
    ACTIONS(86), 2,
      anon_sym_u,
      anon_sym_i,
  [440] = 2,
    ACTIONS(98), 1,
      anon_sym_RBRACE,
    ACTIONS(100), 3,
      anon_sym_if,
      anon_sym_return,
      sym_identifier,
  [449] = 1,
    ACTIONS(102), 3,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_fn,
  [455] = 1,
    ACTIONS(104), 3,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_fn,
  [461] = 1,
    ACTIONS(106), 3,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_fn,
  [467] = 3,
    ACTIONS(108), 1,
      anon_sym_RPAREN,
    ACTIONS(110), 1,
      sym_identifier,
    STATE(46), 1,
      sym_function_args,
  [477] = 1,
    ACTIONS(112), 3,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_fn,
  [483] = 1,
    ACTIONS(114), 2,
      anon_sym_8,
      anon_sym_16,
  [488] = 1,
    ACTIONS(116), 2,
      anon_sym_RPAREN,
      anon_sym_LBRACE,
  [493] = 1,
    ACTIONS(118), 2,
      anon_sym_RPAREN,
      anon_sym_LBRACE,
  [498] = 1,
    ACTIONS(120), 1,
      anon_sym_SEMI,
  [502] = 1,
    ACTIONS(122), 1,
      sym_identifier,
  [506] = 1,
    ACTIONS(48), 1,
      anon_sym_LPAREN,
  [510] = 1,
    ACTIONS(124), 1,
      anon_sym_LBRACE,
  [514] = 1,
    ACTIONS(126), 1,
      anon_sym_RPAREN,
  [518] = 1,
    ACTIONS(128), 1,
      anon_sym_LBRACE,
  [522] = 1,
    ACTIONS(130), 1,
      anon_sym_COLON,
  [526] = 1,
    ACTIONS(132), 1,
      anon_sym_RPAREN,
  [530] = 1,
    ACTIONS(134), 1,
      anon_sym_RBRACE,
  [534] = 1,
    ACTIONS(136), 1,
      anon_sym_COLON,
  [538] = 1,
    ACTIONS(138), 1,
      anon_sym_COLON,
  [542] = 1,
    ACTIONS(140), 1,
      anon_sym_LPAREN,
  [546] = 1,
    ACTIONS(142), 1,
      ts_builtin_sym_end,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(2)] = 0,
  [SMALL_STATE(3)] = 21,
  [SMALL_STATE(4)] = 45,
  [SMALL_STATE(5)] = 63,
  [SMALL_STATE(6)] = 87,
  [SMALL_STATE(7)] = 105,
  [SMALL_STATE(8)] = 123,
  [SMALL_STATE(9)] = 147,
  [SMALL_STATE(10)] = 171,
  [SMALL_STATE(11)] = 195,
  [SMALL_STATE(12)] = 209,
  [SMALL_STATE(13)] = 227,
  [SMALL_STATE(14)] = 245,
  [SMALL_STATE(15)] = 256,
  [SMALL_STATE(16)] = 267,
  [SMALL_STATE(17)] = 278,
  [SMALL_STATE(18)] = 289,
  [SMALL_STATE(19)] = 302,
  [SMALL_STATE(20)] = 322,
  [SMALL_STATE(21)] = 335,
  [SMALL_STATE(22)] = 348,
  [SMALL_STATE(23)] = 361,
  [SMALL_STATE(24)] = 375,
  [SMALL_STATE(25)] = 389,
  [SMALL_STATE(26)] = 400,
  [SMALL_STATE(27)] = 409,
  [SMALL_STATE(28)] = 418,
  [SMALL_STATE(29)] = 429,
  [SMALL_STATE(30)] = 440,
  [SMALL_STATE(31)] = 449,
  [SMALL_STATE(32)] = 455,
  [SMALL_STATE(33)] = 461,
  [SMALL_STATE(34)] = 467,
  [SMALL_STATE(35)] = 477,
  [SMALL_STATE(36)] = 483,
  [SMALL_STATE(37)] = 488,
  [SMALL_STATE(38)] = 493,
  [SMALL_STATE(39)] = 498,
  [SMALL_STATE(40)] = 502,
  [SMALL_STATE(41)] = 506,
  [SMALL_STATE(42)] = 510,
  [SMALL_STATE(43)] = 514,
  [SMALL_STATE(44)] = 518,
  [SMALL_STATE(45)] = 522,
  [SMALL_STATE(46)] = 526,
  [SMALL_STATE(47)] = 530,
  [SMALL_STATE(48)] = 534,
  [SMALL_STATE(49)] = 538,
  [SMALL_STATE(50)] = 542,
  [SMALL_STATE(51)] = 546,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 0, 0, 0),
  [5] = {.entry = {.count = 1, .reusable = true}}, SHIFT(23),
  [7] = {.entry = {.count = 1, .reusable = true}}, SHIFT(40),
  [9] = {.entry = {.count = 1, .reusable = true}}, SHIFT(17),
  [11] = {.entry = {.count = 1, .reusable = true}}, SHIFT(12),
  [13] = {.entry = {.count = 1, .reusable = true}}, SHIFT(11),
  [15] = {.entry = {.count = 1, .reusable = true}}, SHIFT(20),
  [17] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_function_repeat1, 2, 0, 0),
  [19] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_function_repeat1, 2, 0, 0), SHIFT_REPEAT(7),
  [22] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_function_repeat1, 2, 0, 0), SHIFT_REPEAT(6),
  [25] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_function_repeat1, 2, 0, 0), SHIFT_REPEAT(41),
  [28] = {.entry = {.count = 1, .reusable = true}}, SHIFT(15),
  [30] = {.entry = {.count = 1, .reusable = true}}, SHIFT(35),
  [32] = {.entry = {.count = 1, .reusable = false}}, SHIFT(7),
  [34] = {.entry = {.count = 1, .reusable = false}}, SHIFT(6),
  [36] = {.entry = {.count = 1, .reusable = false}}, SHIFT(41),
  [38] = {.entry = {.count = 1, .reusable = true}}, SHIFT(22),
  [40] = {.entry = {.count = 1, .reusable = true}}, SHIFT(21),
  [42] = {.entry = {.count = 1, .reusable = true}}, SHIFT(33),
  [44] = {.entry = {.count = 1, .reusable = true}}, SHIFT(31),
  [46] = {.entry = {.count = 1, .reusable = true}}, SHIFT(32),
  [48] = {.entry = {.count = 1, .reusable = true}}, SHIFT(2),
  [50] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym__expression, 1, 0, 0),
  [52] = {.entry = {.count = 1, .reusable = true}}, SHIFT(14),
  [54] = {.entry = {.count = 1, .reusable = true}}, SHIFT(18),
  [56] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_unary_expression, 2, 0, 0),
  [58] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_binary_expression, 3, 0, 0),
  [60] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function_call, 4, 0, 0),
  [62] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function_call, 3, 0, 0),
  [64] = {.entry = {.count = 1, .reusable = true}}, SHIFT(4),
  [66] = {.entry = {.count = 1, .reusable = true}}, SHIFT(16),
  [68] = {.entry = {.count = 1, .reusable = true}}, SHIFT(13),
  [70] = {.entry = {.count = 1, .reusable = true}}, SHIFT(19),
  [72] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_return, 2, 0, 0),
  [74] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 1, 0, 0),
  [76] = {.entry = {.count = 1, .reusable = true}}, SHIFT(24),
  [78] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0),
  [80] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(24),
  [83] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(40),
  [86] = {.entry = {.count = 1, .reusable = true}}, SHIFT(36),
  [88] = {.entry = {.count = 1, .reusable = true}}, SHIFT(38),
  [90] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_statement, 1, 0, 0),
  [92] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_statement, 1, 0, 0),
  [94] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_if, 5, 0, 0),
  [96] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_if, 5, 0, 0),
  [98] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_statement, 2, 0, 0),
  [100] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_statement, 2, 0, 0),
  [102] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function, 8, 0, 1),
  [104] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function, 9, 0, 3),
  [106] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function, 10, 0, 4),
  [108] = {.entry = {.count = 1, .reusable = true}}, SHIFT(49),
  [110] = {.entry = {.count = 1, .reusable = true}}, SHIFT(48),
  [112] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function, 9, 0, 2),
  [114] = {.entry = {.count = 1, .reusable = true}}, SHIFT(37),
  [116] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type, 2, 0, 0),
  [118] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type, 1, 0, 0),
  [120] = {.entry = {.count = 1, .reusable = true}}, SHIFT(30),
  [122] = {.entry = {.count = 1, .reusable = true}}, SHIFT(50),
  [124] = {.entry = {.count = 1, .reusable = true}}, SHIFT(10),
  [126] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function_args, 3, 0, 0),
  [128] = {.entry = {.count = 1, .reusable = true}}, SHIFT(9),
  [130] = {.entry = {.count = 1, .reusable = true}}, SHIFT(25),
  [132] = {.entry = {.count = 1, .reusable = true}}, SHIFT(45),
  [134] = {.entry = {.count = 1, .reusable = true}}, SHIFT(27),
  [136] = {.entry = {.count = 1, .reusable = true}}, SHIFT(28),
  [138] = {.entry = {.count = 1, .reusable = true}}, SHIFT(29),
  [140] = {.entry = {.count = 1, .reusable = true}}, SHIFT(34),
  [142] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
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

TS_PUBLIC const TSLanguage *tree_sitter_luna(void) {
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
    .field_names = ts_field_names,
    .field_map_slices = ts_field_map_slices,
    .field_map_entries = ts_field_map_entries,
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
