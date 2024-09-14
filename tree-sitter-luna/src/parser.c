#include "tree_sitter/parser.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 14
#define STATE_COUNT 52
#define LARGE_STATE_COUNT 2
#define SYMBOL_COUNT 39
#define ALIAS_COUNT 0
#define TOKEN_COUNT 25
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 4
#define MAX_ALIAS_SEQUENCE_LENGTH 8
#define PRODUCTION_ID_COUNT 3

enum ts_symbol_identifiers {
  sym_comment = 1,
  anon_sym_fn = 2,
  anon_sym_LPAREN = 3,
  anon_sym_RPAREN = 4,
  anon_sym_COLON = 5,
  anon_sym_u = 6,
  anon_sym_8 = 7,
  anon_sym_16 = 8,
  anon_sym_i = 9,
  anon_sym_void = 10,
  anon_sym_if = 11,
  anon_sym_DASH = 12,
  anon_sym_BANG = 13,
  anon_sym_STAR = 14,
  anon_sym_SLASH = 15,
  anon_sym_PLUS = 16,
  anon_sym_EQ_EQ = 17,
  anon_sym_SEMI = 18,
  anon_sym_return = 19,
  anon_sym_LBRACE = 20,
  anon_sym_RBRACE = 21,
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
  sym__block_scope = 36,
  aux_sym_source_file_repeat1 = 37,
  aux_sym__block_scope_repeat1 = 38,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [sym_comment] = "comment",
  [anon_sym_fn] = "fn",
  [anon_sym_LPAREN] = "(",
  [anon_sym_RPAREN] = ")",
  [anon_sym_COLON] = ":",
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
  [anon_sym_LBRACE] = "{",
  [anon_sym_RBRACE] = "}",
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
  [sym__block_scope] = "_block_scope",
  [aux_sym_source_file_repeat1] = "source_file_repeat1",
  [aux_sym__block_scope_repeat1] = "_block_scope_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [sym_comment] = sym_comment,
  [anon_sym_fn] = anon_sym_fn,
  [anon_sym_LPAREN] = anon_sym_LPAREN,
  [anon_sym_RPAREN] = anon_sym_RPAREN,
  [anon_sym_COLON] = anon_sym_COLON,
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
  [anon_sym_LBRACE] = anon_sym_LBRACE,
  [anon_sym_RBRACE] = anon_sym_RBRACE,
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
  [sym__block_scope] = sym__block_scope,
  [aux_sym_source_file_repeat1] = aux_sym_source_file_repeat1,
  [aux_sym__block_scope_repeat1] = aux_sym__block_scope_repeat1,
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
  [anon_sym_LBRACE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RBRACE] = {
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
  [sym__block_scope] = {
    .visible = false,
    .named = true,
  },
  [aux_sym_source_file_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym__block_scope_repeat1] = {
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
  [1] = {.index = 0, .length = 3},
  [2] = {.index = 3, .length = 4},
};

static const TSFieldMapEntry ts_field_map_entries[] = {
  [0] =
    {field_body, 6},
    {field_name, 1},
    {field_ret_type, 5},
  [3] =
    {field_args, 3},
    {field_body, 7},
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
  [4] = 2,
  [5] = 3,
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
  [32] = 26,
  [33] = 33,
  [34] = 28,
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
        '!', 35,
        '"', 2,
        '(', 21,
        ')', 22,
        '*', 36,
        '+', 39,
        '-', 34,
        '/', 38,
        '1', 54,
        '8', 26,
        ':', 23,
        ';', 41,
        '=', 6,
        'f', 10,
        'i', 30,
        'r', 8,
        'u', 24,
        'v', 12,
        '{', 44,
        '}', 45,
      );
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(0);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(55);
      END_STATE();
    case 1:
      ADVANCE_MAP(
        '!', 35,
        '"', 2,
        '(', 21,
        ')', 22,
        '*', 36,
        '+', 39,
        '-', 34,
        '/', 37,
        ';', 41,
        '=', 6,
        '{', 44,
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
      if (lookahead == '/') ADVANCE(3);
      if (lookahead == 'i') ADVANCE(47);
      if (lookahead == 'r') ADVANCE(46);
      if (lookahead == '}') ADVANCE(45);
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(4);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 5:
      if (lookahead == '6') ADVANCE(27);
      END_STATE();
    case 6:
      if (lookahead == '=') ADVANCE(40);
      END_STATE();
    case 7:
      if (lookahead == 'd') ADVANCE(31);
      END_STATE();
    case 8:
      if (lookahead == 'e') ADVANCE(14);
      END_STATE();
    case 9:
      if (lookahead == 'i') ADVANCE(7);
      END_STATE();
    case 10:
      if (lookahead == 'n') ADVANCE(20);
      END_STATE();
    case 11:
      if (lookahead == 'n') ADVANCE(42);
      END_STATE();
    case 12:
      if (lookahead == 'o') ADVANCE(9);
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
      if (lookahead == '1') ADVANCE(5);
      if (lookahead == '8') ADVANCE(25);
      if (lookahead == 'f') ADVANCE(10);
      if (lookahead == 'i') ADVANCE(29);
      if (lookahead == 'u') ADVANCE(24);
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
      ACCEPT_TOKEN(anon_sym_u);
      END_STATE();
    case 25:
      ACCEPT_TOKEN(anon_sym_8);
      END_STATE();
    case 26:
      ACCEPT_TOKEN(anon_sym_8);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(55);
      END_STATE();
    case 27:
      ACCEPT_TOKEN(anon_sym_16);
      END_STATE();
    case 28:
      ACCEPT_TOKEN(anon_sym_16);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(55);
      END_STATE();
    case 29:
      ACCEPT_TOKEN(anon_sym_i);
      END_STATE();
    case 30:
      ACCEPT_TOKEN(anon_sym_i);
      if (lookahead == 'f') ADVANCE(32);
      END_STATE();
    case 31:
      ACCEPT_TOKEN(anon_sym_void);
      END_STATE();
    case 32:
      ACCEPT_TOKEN(anon_sym_if);
      END_STATE();
    case 33:
      ACCEPT_TOKEN(anon_sym_if);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 34:
      ACCEPT_TOKEN(anon_sym_DASH);
      END_STATE();
    case 35:
      ACCEPT_TOKEN(anon_sym_BANG);
      END_STATE();
    case 36:
      ACCEPT_TOKEN(anon_sym_STAR);
      END_STATE();
    case 37:
      ACCEPT_TOKEN(anon_sym_SLASH);
      END_STATE();
    case 38:
      ACCEPT_TOKEN(anon_sym_SLASH);
      if (lookahead == '/') ADVANCE(19);
      END_STATE();
    case 39:
      ACCEPT_TOKEN(anon_sym_PLUS);
      END_STATE();
    case 40:
      ACCEPT_TOKEN(anon_sym_EQ_EQ);
      END_STATE();
    case 41:
      ACCEPT_TOKEN(anon_sym_SEMI);
      END_STATE();
    case 42:
      ACCEPT_TOKEN(anon_sym_return);
      END_STATE();
    case 43:
      ACCEPT_TOKEN(anon_sym_return);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 44:
      ACCEPT_TOKEN(anon_sym_LBRACE);
      END_STATE();
    case 45:
      ACCEPT_TOKEN(anon_sym_RBRACE);
      END_STATE();
    case 46:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'e') ADVANCE(50);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 47:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'f') ADVANCE(33);
      if (('a' <= lookahead && lookahead <= 'z')) ADVANCE(52);
      END_STATE();
    case 48:
      ACCEPT_TOKEN(sym_identifier);
      if (lookahead == 'n') ADVANCE(43);
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
      if (lookahead == '6') ADVANCE(28);
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
  [2] = {.lex_state = 4},
  [3] = {.lex_state = 4},
  [4] = {.lex_state = 4},
  [5] = {.lex_state = 4},
  [6] = {.lex_state = 1},
  [7] = {.lex_state = 4},
  [8] = {.lex_state = 1},
  [9] = {.lex_state = 1},
  [10] = {.lex_state = 1},
  [11] = {.lex_state = 1},
  [12] = {.lex_state = 1},
  [13] = {.lex_state = 1},
  [14] = {.lex_state = 1},
  [15] = {.lex_state = 1},
  [16] = {.lex_state = 1},
  [17] = {.lex_state = 1},
  [18] = {.lex_state = 1},
  [19] = {.lex_state = 1},
  [20] = {.lex_state = 1},
  [21] = {.lex_state = 1},
  [22] = {.lex_state = 4},
  [23] = {.lex_state = 17},
  [24] = {.lex_state = 4},
  [25] = {.lex_state = 4},
  [26] = {.lex_state = 4},
  [27] = {.lex_state = 17},
  [28] = {.lex_state = 4},
  [29] = {.lex_state = 17},
  [30] = {.lex_state = 17},
  [31] = {.lex_state = 17},
  [32] = {.lex_state = 17},
  [33] = {.lex_state = 17},
  [34] = {.lex_state = 17},
  [35] = {.lex_state = 17},
  [36] = {.lex_state = 1},
  [37] = {.lex_state = 0},
  [38] = {.lex_state = 0},
  [39] = {.lex_state = 0},
  [40] = {.lex_state = 0},
  [41] = {.lex_state = 17},
  [42] = {.lex_state = 0},
  [43] = {.lex_state = 0},
  [44] = {.lex_state = 1},
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
    [anon_sym_LBRACE] = ACTIONS(1),
    [anon_sym_RBRACE] = ACTIONS(1),
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
  [0] = 8,
    ACTIONS(9), 1,
      sym_comment,
    ACTIONS(11), 1,
      anon_sym_if,
    ACTIONS(13), 1,
      anon_sym_return,
    ACTIONS(15), 1,
      anon_sym_RBRACE,
    ACTIONS(17), 1,
      sym_identifier,
    STATE(25), 1,
      sym_if,
    STATE(5), 2,
      sym_statement,
      aux_sym__block_scope_repeat1,
    STATE(42), 2,
      sym_return,
      sym_function_call,
  [27] = 8,
    ACTIONS(9), 1,
      sym_comment,
    ACTIONS(11), 1,
      anon_sym_if,
    ACTIONS(13), 1,
      anon_sym_return,
    ACTIONS(17), 1,
      sym_identifier,
    ACTIONS(19), 1,
      anon_sym_RBRACE,
    STATE(25), 1,
      sym_if,
    STATE(7), 2,
      sym_statement,
      aux_sym__block_scope_repeat1,
    STATE(42), 2,
      sym_return,
      sym_function_call,
  [54] = 8,
    ACTIONS(9), 1,
      sym_comment,
    ACTIONS(11), 1,
      anon_sym_if,
    ACTIONS(13), 1,
      anon_sym_return,
    ACTIONS(17), 1,
      sym_identifier,
    ACTIONS(21), 1,
      anon_sym_RBRACE,
    STATE(25), 1,
      sym_if,
    STATE(3), 2,
      sym_statement,
      aux_sym__block_scope_repeat1,
    STATE(42), 2,
      sym_return,
      sym_function_call,
  [81] = 8,
    ACTIONS(9), 1,
      sym_comment,
    ACTIONS(11), 1,
      anon_sym_if,
    ACTIONS(13), 1,
      anon_sym_return,
    ACTIONS(17), 1,
      sym_identifier,
    ACTIONS(23), 1,
      anon_sym_RBRACE,
    STATE(25), 1,
      sym_if,
    STATE(7), 2,
      sym_statement,
      aux_sym__block_scope_repeat1,
    STATE(42), 2,
      sym_return,
      sym_function_call,
  [108] = 5,
    ACTIONS(25), 1,
      anon_sym_RPAREN,
    ACTIONS(29), 1,
      sym_identifier,
    ACTIONS(27), 2,
      anon_sym_DASH,
      anon_sym_BANG,
    ACTIONS(31), 2,
      sym_string_literal,
      sym_number,
    STATE(20), 4,
      sym__expression,
      sym_unary_expression,
      sym_binary_expression,
      sym_function_call,
  [129] = 8,
    ACTIONS(33), 1,
      sym_comment,
    ACTIONS(36), 1,
      anon_sym_if,
    ACTIONS(39), 1,
      anon_sym_return,
    ACTIONS(42), 1,
      anon_sym_RBRACE,
    ACTIONS(44), 1,
      sym_identifier,
    STATE(25), 1,
      sym_if,
    STATE(7), 2,
      sym_statement,
      aux_sym__block_scope_repeat1,
    STATE(42), 2,
      sym_return,
      sym_function_call,
  [156] = 4,
    ACTIONS(29), 1,
      sym_identifier,
    ACTIONS(27), 2,
      anon_sym_DASH,
      anon_sym_BANG,
    ACTIONS(47), 2,
      sym_string_literal,
      sym_number,
    STATE(19), 4,
      sym__expression,
      sym_unary_expression,
      sym_binary_expression,
      sym_function_call,
  [174] = 4,
    ACTIONS(29), 1,
      sym_identifier,
    ACTIONS(27), 2,
      anon_sym_DASH,
      anon_sym_BANG,
    ACTIONS(49), 2,
      sym_string_literal,
      sym_number,
    STATE(16), 4,
      sym__expression,
      sym_unary_expression,
      sym_binary_expression,
      sym_function_call,
  [192] = 2,
    ACTIONS(51), 1,
      anon_sym_LPAREN,
    ACTIONS(53), 8,
      anon_sym_RPAREN,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
      anon_sym_SEMI,
      anon_sym_LBRACE,
  [206] = 4,
    ACTIONS(29), 1,
      sym_identifier,
    ACTIONS(27), 2,
      anon_sym_DASH,
      anon_sym_BANG,
    ACTIONS(55), 2,
      sym_string_literal,
      sym_number,
    STATE(14), 4,
      sym__expression,
      sym_unary_expression,
      sym_binary_expression,
      sym_function_call,
  [224] = 4,
    ACTIONS(29), 1,
      sym_identifier,
    ACTIONS(27), 2,
      anon_sym_DASH,
      anon_sym_BANG,
    ACTIONS(57), 2,
      sym_string_literal,
      sym_number,
    STATE(21), 4,
      sym__expression,
      sym_unary_expression,
      sym_binary_expression,
      sym_function_call,
  [242] = 4,
    ACTIONS(29), 1,
      sym_identifier,
    ACTIONS(27), 2,
      anon_sym_DASH,
      anon_sym_BANG,
    ACTIONS(59), 2,
      sym_string_literal,
      sym_number,
    STATE(18), 4,
      sym__expression,
      sym_unary_expression,
      sym_binary_expression,
      sym_function_call,
  [260] = 1,
    ACTIONS(61), 8,
      anon_sym_RPAREN,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
      anon_sym_SEMI,
      anon_sym_LBRACE,
  [271] = 1,
    ACTIONS(63), 8,
      anon_sym_RPAREN,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
      anon_sym_SEMI,
      anon_sym_LBRACE,
  [282] = 1,
    ACTIONS(65), 8,
      anon_sym_RPAREN,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
      anon_sym_SEMI,
      anon_sym_LBRACE,
  [293] = 1,
    ACTIONS(67), 8,
      anon_sym_RPAREN,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
      anon_sym_SEMI,
      anon_sym_LBRACE,
  [304] = 2,
    ACTIONS(69), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(65), 6,
      anon_sym_RPAREN,
      anon_sym_DASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
      anon_sym_SEMI,
      anon_sym_LBRACE,
  [317] = 4,
    ACTIONS(73), 1,
      anon_sym_LBRACE,
    STATE(22), 1,
      sym__block_scope,
    ACTIONS(69), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(71), 3,
      anon_sym_DASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
  [333] = 3,
    ACTIONS(75), 1,
      anon_sym_RPAREN,
    ACTIONS(69), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(71), 3,
      anon_sym_DASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
  [346] = 3,
    ACTIONS(77), 1,
      anon_sym_SEMI,
    ACTIONS(69), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(71), 3,
      anon_sym_DASH,
      anon_sym_PLUS,
      anon_sym_EQ_EQ,
  [359] = 2,
    ACTIONS(79), 2,
      sym_comment,
      anon_sym_RBRACE,
    ACTIONS(81), 3,
      anon_sym_if,
      anon_sym_return,
      sym_identifier,
  [369] = 4,
    ACTIONS(7), 1,
      anon_sym_fn,
    ACTIONS(83), 1,
      ts_builtin_sym_end,
    ACTIONS(85), 1,
      sym_comment,
    STATE(27), 2,
      sym_function,
      aux_sym_source_file_repeat1,
  [383] = 2,
    ACTIONS(87), 2,
      sym_comment,
      anon_sym_RBRACE,
    ACTIONS(89), 3,
      anon_sym_if,
      anon_sym_return,
      sym_identifier,
  [393] = 2,
    ACTIONS(91), 2,
      sym_comment,
      anon_sym_RBRACE,
    ACTIONS(93), 3,
      anon_sym_if,
      anon_sym_return,
      sym_identifier,
  [403] = 2,
    ACTIONS(95), 2,
      sym_comment,
      anon_sym_RBRACE,
    ACTIONS(97), 3,
      anon_sym_if,
      anon_sym_return,
      sym_identifier,
  [413] = 4,
    ACTIONS(99), 1,
      ts_builtin_sym_end,
    ACTIONS(101), 1,
      sym_comment,
    ACTIONS(104), 1,
      anon_sym_fn,
    STATE(27), 2,
      sym_function,
      aux_sym_source_file_repeat1,
  [427] = 2,
    ACTIONS(107), 2,
      sym_comment,
      anon_sym_RBRACE,
    ACTIONS(109), 3,
      anon_sym_if,
      anon_sym_return,
      sym_identifier,
  [437] = 3,
    ACTIONS(113), 1,
      anon_sym_void,
    STATE(37), 1,
      sym_type,
    ACTIONS(111), 2,
      anon_sym_u,
      anon_sym_i,
  [448] = 3,
    ACTIONS(113), 1,
      anon_sym_void,
    STATE(45), 1,
      sym_type,
    ACTIONS(111), 2,
      anon_sym_u,
      anon_sym_i,
  [459] = 3,
    ACTIONS(113), 1,
      anon_sym_void,
    STATE(39), 1,
      sym_type,
    ACTIONS(111), 2,
      anon_sym_u,
      anon_sym_i,
  [470] = 1,
    ACTIONS(95), 3,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_fn,
  [476] = 1,
    ACTIONS(115), 3,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_fn,
  [482] = 1,
    ACTIONS(107), 3,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_fn,
  [488] = 1,
    ACTIONS(117), 3,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_fn,
  [494] = 3,
    ACTIONS(119), 1,
      anon_sym_RPAREN,
    ACTIONS(121), 1,
      sym_identifier,
    STATE(47), 1,
      sym_function_args,
  [504] = 2,
    ACTIONS(123), 1,
      anon_sym_LBRACE,
    STATE(33), 1,
      sym__block_scope,
  [511] = 1,
    ACTIONS(125), 2,
      anon_sym_RPAREN,
      anon_sym_LBRACE,
  [516] = 2,
    ACTIONS(123), 1,
      anon_sym_LBRACE,
    STATE(35), 1,
      sym__block_scope,
  [523] = 1,
    ACTIONS(127), 2,
      anon_sym_RPAREN,
      anon_sym_LBRACE,
  [528] = 1,
    ACTIONS(129), 2,
      anon_sym_8,
      anon_sym_16,
  [533] = 1,
    ACTIONS(131), 1,
      anon_sym_SEMI,
  [537] = 1,
    ACTIONS(51), 1,
      anon_sym_LPAREN,
  [541] = 1,
    ACTIONS(133), 1,
      sym_identifier,
  [545] = 1,
    ACTIONS(135), 1,
      anon_sym_RPAREN,
  [549] = 1,
    ACTIONS(137), 1,
      anon_sym_COLON,
  [553] = 1,
    ACTIONS(139), 1,
      anon_sym_RPAREN,
  [557] = 1,
    ACTIONS(141), 1,
      anon_sym_COLON,
  [561] = 1,
    ACTIONS(143), 1,
      anon_sym_COLON,
  [565] = 1,
    ACTIONS(145), 1,
      anon_sym_LPAREN,
  [569] = 1,
    ACTIONS(147), 1,
      ts_builtin_sym_end,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(2)] = 0,
  [SMALL_STATE(3)] = 27,
  [SMALL_STATE(4)] = 54,
  [SMALL_STATE(5)] = 81,
  [SMALL_STATE(6)] = 108,
  [SMALL_STATE(7)] = 129,
  [SMALL_STATE(8)] = 156,
  [SMALL_STATE(9)] = 174,
  [SMALL_STATE(10)] = 192,
  [SMALL_STATE(11)] = 206,
  [SMALL_STATE(12)] = 224,
  [SMALL_STATE(13)] = 242,
  [SMALL_STATE(14)] = 260,
  [SMALL_STATE(15)] = 271,
  [SMALL_STATE(16)] = 282,
  [SMALL_STATE(17)] = 293,
  [SMALL_STATE(18)] = 304,
  [SMALL_STATE(19)] = 317,
  [SMALL_STATE(20)] = 333,
  [SMALL_STATE(21)] = 346,
  [SMALL_STATE(22)] = 359,
  [SMALL_STATE(23)] = 369,
  [SMALL_STATE(24)] = 383,
  [SMALL_STATE(25)] = 393,
  [SMALL_STATE(26)] = 403,
  [SMALL_STATE(27)] = 413,
  [SMALL_STATE(28)] = 427,
  [SMALL_STATE(29)] = 437,
  [SMALL_STATE(30)] = 448,
  [SMALL_STATE(31)] = 459,
  [SMALL_STATE(32)] = 470,
  [SMALL_STATE(33)] = 476,
  [SMALL_STATE(34)] = 482,
  [SMALL_STATE(35)] = 488,
  [SMALL_STATE(36)] = 494,
  [SMALL_STATE(37)] = 504,
  [SMALL_STATE(38)] = 511,
  [SMALL_STATE(39)] = 516,
  [SMALL_STATE(40)] = 523,
  [SMALL_STATE(41)] = 528,
  [SMALL_STATE(42)] = 533,
  [SMALL_STATE(43)] = 537,
  [SMALL_STATE(44)] = 541,
  [SMALL_STATE(45)] = 545,
  [SMALL_STATE(46)] = 549,
  [SMALL_STATE(47)] = 553,
  [SMALL_STATE(48)] = 557,
  [SMALL_STATE(49)] = 561,
  [SMALL_STATE(50)] = 565,
  [SMALL_STATE(51)] = 569,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 0, 0, 0),
  [5] = {.entry = {.count = 1, .reusable = true}}, SHIFT(23),
  [7] = {.entry = {.count = 1, .reusable = true}}, SHIFT(44),
  [9] = {.entry = {.count = 1, .reusable = true}}, SHIFT(25),
  [11] = {.entry = {.count = 1, .reusable = false}}, SHIFT(8),
  [13] = {.entry = {.count = 1, .reusable = false}}, SHIFT(12),
  [15] = {.entry = {.count = 1, .reusable = true}}, SHIFT(32),
  [17] = {.entry = {.count = 1, .reusable = false}}, SHIFT(43),
  [19] = {.entry = {.count = 1, .reusable = true}}, SHIFT(28),
  [21] = {.entry = {.count = 1, .reusable = true}}, SHIFT(26),
  [23] = {.entry = {.count = 1, .reusable = true}}, SHIFT(34),
  [25] = {.entry = {.count = 1, .reusable = true}}, SHIFT(15),
  [27] = {.entry = {.count = 1, .reusable = true}}, SHIFT(11),
  [29] = {.entry = {.count = 1, .reusable = true}}, SHIFT(10),
  [31] = {.entry = {.count = 1, .reusable = true}}, SHIFT(20),
  [33] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym__block_scope_repeat1, 2, 0, 0), SHIFT_REPEAT(25),
  [36] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym__block_scope_repeat1, 2, 0, 0), SHIFT_REPEAT(8),
  [39] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym__block_scope_repeat1, 2, 0, 0), SHIFT_REPEAT(12),
  [42] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym__block_scope_repeat1, 2, 0, 0),
  [44] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym__block_scope_repeat1, 2, 0, 0), SHIFT_REPEAT(43),
  [47] = {.entry = {.count = 1, .reusable = true}}, SHIFT(19),
  [49] = {.entry = {.count = 1, .reusable = true}}, SHIFT(16),
  [51] = {.entry = {.count = 1, .reusable = true}}, SHIFT(6),
  [53] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym__expression, 1, 0, 0),
  [55] = {.entry = {.count = 1, .reusable = true}}, SHIFT(14),
  [57] = {.entry = {.count = 1, .reusable = true}}, SHIFT(21),
  [59] = {.entry = {.count = 1, .reusable = true}}, SHIFT(18),
  [61] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_unary_expression, 2, 0, 0),
  [63] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function_call, 3, 0, 0),
  [65] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_binary_expression, 3, 0, 0),
  [67] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function_call, 4, 0, 0),
  [69] = {.entry = {.count = 1, .reusable = true}}, SHIFT(9),
  [71] = {.entry = {.count = 1, .reusable = true}}, SHIFT(13),
  [73] = {.entry = {.count = 1, .reusable = true}}, SHIFT(4),
  [75] = {.entry = {.count = 1, .reusable = true}}, SHIFT(17),
  [77] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_return, 2, 0, 0),
  [79] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_if, 3, 0, 0),
  [81] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_if, 3, 0, 0),
  [83] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 1, 0, 0),
  [85] = {.entry = {.count = 1, .reusable = true}}, SHIFT(27),
  [87] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_statement, 2, 0, 0),
  [89] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_statement, 2, 0, 0),
  [91] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_statement, 1, 0, 0),
  [93] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_statement, 1, 0, 0),
  [95] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym__block_scope, 2, 0, 0),
  [97] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym__block_scope, 2, 0, 0),
  [99] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0),
  [101] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(27),
  [104] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(44),
  [107] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym__block_scope, 3, 0, 0),
  [109] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym__block_scope, 3, 0, 0),
  [111] = {.entry = {.count = 1, .reusable = true}}, SHIFT(41),
  [113] = {.entry = {.count = 1, .reusable = true}}, SHIFT(40),
  [115] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function, 8, 0, 2),
  [117] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function, 7, 0, 1),
  [119] = {.entry = {.count = 1, .reusable = true}}, SHIFT(49),
  [121] = {.entry = {.count = 1, .reusable = true}}, SHIFT(48),
  [123] = {.entry = {.count = 1, .reusable = true}}, SHIFT(2),
  [125] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type, 2, 0, 0),
  [127] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type, 1, 0, 0),
  [129] = {.entry = {.count = 1, .reusable = true}}, SHIFT(38),
  [131] = {.entry = {.count = 1, .reusable = true}}, SHIFT(24),
  [133] = {.entry = {.count = 1, .reusable = true}}, SHIFT(50),
  [135] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function_args, 3, 0, 0),
  [137] = {.entry = {.count = 1, .reusable = true}}, SHIFT(29),
  [139] = {.entry = {.count = 1, .reusable = true}}, SHIFT(46),
  [141] = {.entry = {.count = 1, .reusable = true}}, SHIFT(30),
  [143] = {.entry = {.count = 1, .reusable = true}}, SHIFT(31),
  [145] = {.entry = {.count = 1, .reusable = true}}, SHIFT(36),
  [147] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
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
