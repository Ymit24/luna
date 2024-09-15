#include "tree_sitter/parser.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 14
#define STATE_COUNT 23
#define LARGE_STATE_COUNT 2
#define SYMBOL_COUNT 25
#define ALIAS_COUNT 0
#define TOKEN_COUNT 15
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 0
#define MAX_ALIAS_SEQUENCE_LENGTH 3
#define PRODUCTION_ID_COUNT 1

enum ts_symbol_identifiers {
  anon_sym_push = 1,
  anon_sym_pop = 2,
  anon_sym_const = 3,
  anon_sym_args = 4,
  anon_sym_local = 5,
  anon_sym_PERCENT = 6,
  anon_sym_sp = 7,
  anon_sym_POUND = 8,
  aux_sym_comment_token1 = 9,
  anon_sym_0x = 10,
  aux_sym_constant_token1 = 11,
  anon_sym_DOLLAR = 12,
  anon_sym_COLON = 13,
  sym_identifier = 14,
  sym_source_file = 15,
  sym_push = 16,
  sym_pop = 17,
  sym_virtual_memory_segment = 18,
  sym_push_pop_arg = 19,
  sym_predefined_symbol = 20,
  sym_comment = 21,
  sym_constant = 22,
  sym_label_definition = 23,
  aux_sym_source_file_repeat1 = 24,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [anon_sym_push] = "push",
  [anon_sym_pop] = "pop",
  [anon_sym_const] = "const",
  [anon_sym_args] = "args",
  [anon_sym_local] = "local",
  [anon_sym_PERCENT] = "%",
  [anon_sym_sp] = "sp",
  [anon_sym_POUND] = "#",
  [aux_sym_comment_token1] = "comment_token1",
  [anon_sym_0x] = "0x",
  [aux_sym_constant_token1] = "constant_token1",
  [anon_sym_DOLLAR] = "$",
  [anon_sym_COLON] = ":",
  [sym_identifier] = "identifier",
  [sym_source_file] = "source_file",
  [sym_push] = "push",
  [sym_pop] = "pop",
  [sym_virtual_memory_segment] = "virtual_memory_segment",
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
  [anon_sym_const] = anon_sym_const,
  [anon_sym_args] = anon_sym_args,
  [anon_sym_local] = anon_sym_local,
  [anon_sym_PERCENT] = anon_sym_PERCENT,
  [anon_sym_sp] = anon_sym_sp,
  [anon_sym_POUND] = anon_sym_POUND,
  [aux_sym_comment_token1] = aux_sym_comment_token1,
  [anon_sym_0x] = anon_sym_0x,
  [aux_sym_constant_token1] = aux_sym_constant_token1,
  [anon_sym_DOLLAR] = anon_sym_DOLLAR,
  [anon_sym_COLON] = anon_sym_COLON,
  [sym_identifier] = sym_identifier,
  [sym_source_file] = sym_source_file,
  [sym_push] = sym_push,
  [sym_pop] = sym_pop,
  [sym_virtual_memory_segment] = sym_virtual_memory_segment,
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
  [anon_sym_PERCENT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_sp] = {
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
  [sym_virtual_memory_segment] = {
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
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  eof = lexer->eof(lexer);
  switch (state) {
    case 0:
      if (eof) ADVANCE(17);
      ADVANCE_MAP(
        '#', 25,
        '$', 31,
        '%', 23,
        '0', 29,
        ':', 32,
        'a', 12,
        'c', 7,
        'l', 8,
        'p', 9,
        's', 10,
      );
      if (('\t' <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') SKIP(0);
      if (('1' <= lookahead && lookahead <= '9')) ADVANCE(30);
      END_STATE();
    case 1:
      if (lookahead == 'a') ADVANCE(5);
      END_STATE();
    case 2:
      if (lookahead == 'c') ADVANCE(1);
      END_STATE();
    case 3:
      if (lookahead == 'g') ADVANCE(14);
      END_STATE();
    case 4:
      if (lookahead == 'h') ADVANCE(18);
      END_STATE();
    case 5:
      if (lookahead == 'l') ADVANCE(22);
      END_STATE();
    case 6:
      if (lookahead == 'n') ADVANCE(15);
      END_STATE();
    case 7:
      if (lookahead == 'o') ADVANCE(6);
      END_STATE();
    case 8:
      if (lookahead == 'o') ADVANCE(2);
      END_STATE();
    case 9:
      if (lookahead == 'o') ADVANCE(11);
      if (lookahead == 'u') ADVANCE(13);
      END_STATE();
    case 10:
      if (lookahead == 'p') ADVANCE(24);
      END_STATE();
    case 11:
      if (lookahead == 'p') ADVANCE(19);
      END_STATE();
    case 12:
      if (lookahead == 'r') ADVANCE(3);
      END_STATE();
    case 13:
      if (lookahead == 's') ADVANCE(4);
      END_STATE();
    case 14:
      if (lookahead == 's') ADVANCE(21);
      END_STATE();
    case 15:
      if (lookahead == 's') ADVANCE(16);
      END_STATE();
    case 16:
      if (lookahead == 't') ADVANCE(20);
      END_STATE();
    case 17:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 18:
      ACCEPT_TOKEN(anon_sym_push);
      END_STATE();
    case 19:
      ACCEPT_TOKEN(anon_sym_pop);
      END_STATE();
    case 20:
      ACCEPT_TOKEN(anon_sym_const);
      END_STATE();
    case 21:
      ACCEPT_TOKEN(anon_sym_args);
      END_STATE();
    case 22:
      ACCEPT_TOKEN(anon_sym_local);
      END_STATE();
    case 23:
      ACCEPT_TOKEN(anon_sym_PERCENT);
      END_STATE();
    case 24:
      ACCEPT_TOKEN(anon_sym_sp);
      END_STATE();
    case 25:
      ACCEPT_TOKEN(anon_sym_POUND);
      END_STATE();
    case 26:
      ACCEPT_TOKEN(aux_sym_comment_token1);
      if (lookahead == '\t' ||
          (0x0b <= lookahead && lookahead <= '\r') ||
          lookahead == ' ') ADVANCE(26);
      if (lookahead != 0 &&
          (lookahead < '\t' || '\r' < lookahead)) ADVANCE(27);
      END_STATE();
    case 27:
      ACCEPT_TOKEN(aux_sym_comment_token1);
      if (lookahead != 0 &&
          lookahead != '\n') ADVANCE(27);
      END_STATE();
    case 28:
      ACCEPT_TOKEN(anon_sym_0x);
      END_STATE();
    case 29:
      ACCEPT_TOKEN(aux_sym_constant_token1);
      if (lookahead == 'x') ADVANCE(28);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(30);
      END_STATE();
    case 30:
      ACCEPT_TOKEN(aux_sym_constant_token1);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(30);
      END_STATE();
    case 31:
      ACCEPT_TOKEN(anon_sym_DOLLAR);
      END_STATE();
    case 32:
      ACCEPT_TOKEN(anon_sym_COLON);
      END_STATE();
    case 33:
      ACCEPT_TOKEN(sym_identifier);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(33);
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
  [17] = {.lex_state = 33},
  [18] = {.lex_state = 0},
  [19] = {.lex_state = 0},
  [20] = {.lex_state = 26},
  [21] = {.lex_state = 0},
  [22] = {.lex_state = 0},
};

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [anon_sym_push] = ACTIONS(1),
    [anon_sym_pop] = ACTIONS(1),
    [anon_sym_const] = ACTIONS(1),
    [anon_sym_args] = ACTIONS(1),
    [anon_sym_local] = ACTIONS(1),
    [anon_sym_PERCENT] = ACTIONS(1),
    [anon_sym_sp] = ACTIONS(1),
    [anon_sym_POUND] = ACTIONS(1),
    [anon_sym_0x] = ACTIONS(1),
    [aux_sym_constant_token1] = ACTIONS(1),
    [anon_sym_DOLLAR] = ACTIONS(1),
    [anon_sym_COLON] = ACTIONS(1),
  },
  [1] = {
    [sym_source_file] = STATE(22),
    [sym_push] = STATE(2),
    [sym_pop] = STATE(2),
    [sym_comment] = STATE(2),
    [sym_label_definition] = STATE(2),
    [aux_sym_source_file_repeat1] = STATE(2),
    [ts_builtin_sym_end] = ACTIONS(3),
    [anon_sym_push] = ACTIONS(5),
    [anon_sym_pop] = ACTIONS(7),
    [anon_sym_POUND] = ACTIONS(9),
    [anon_sym_DOLLAR] = ACTIONS(11),
  },
};

static const uint16_t ts_small_parse_table[] = {
  [0] = 6,
    ACTIONS(5), 1,
      anon_sym_push,
    ACTIONS(7), 1,
      anon_sym_pop,
    ACTIONS(9), 1,
      anon_sym_POUND,
    ACTIONS(11), 1,
      anon_sym_DOLLAR,
    ACTIONS(13), 1,
      ts_builtin_sym_end,
    STATE(3), 5,
      sym_push,
      sym_pop,
      sym_comment,
      sym_label_definition,
      aux_sym_source_file_repeat1,
  [23] = 6,
    ACTIONS(15), 1,
      ts_builtin_sym_end,
    ACTIONS(17), 1,
      anon_sym_push,
    ACTIONS(20), 1,
      anon_sym_pop,
    ACTIONS(23), 1,
      anon_sym_POUND,
    ACTIONS(26), 1,
      anon_sym_DOLLAR,
    STATE(3), 5,
      sym_push,
      sym_pop,
      sym_comment,
      sym_label_definition,
      aux_sym_source_file_repeat1,
  [46] = 5,
    ACTIONS(29), 1,
      anon_sym_PERCENT,
    ACTIONS(31), 1,
      anon_sym_0x,
    ACTIONS(33), 1,
      aux_sym_constant_token1,
    STATE(12), 1,
      sym_push_pop_arg,
    STATE(13), 2,
      sym_predefined_symbol,
      sym_constant,
  [63] = 5,
    ACTIONS(29), 1,
      anon_sym_PERCENT,
    ACTIONS(31), 1,
      anon_sym_0x,
    ACTIONS(33), 1,
      aux_sym_constant_token1,
    STATE(6), 1,
      sym_push_pop_arg,
    STATE(13), 2,
      sym_predefined_symbol,
      sym_constant,
  [80] = 1,
    ACTIONS(35), 5,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [88] = 1,
    ACTIONS(37), 5,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [96] = 1,
    ACTIONS(39), 5,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [104] = 1,
    ACTIONS(41), 5,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [112] = 1,
    ACTIONS(43), 5,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [120] = 1,
    ACTIONS(45), 5,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [128] = 1,
    ACTIONS(47), 5,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [136] = 1,
    ACTIONS(49), 5,
      ts_builtin_sym_end,
      anon_sym_push,
      anon_sym_pop,
      anon_sym_POUND,
      anon_sym_DOLLAR,
  [144] = 2,
    STATE(4), 1,
      sym_virtual_memory_segment,
    ACTIONS(51), 3,
      anon_sym_const,
      anon_sym_args,
      anon_sym_local,
  [153] = 2,
    STATE(5), 1,
      sym_virtual_memory_segment,
    ACTIONS(51), 3,
      anon_sym_const,
      anon_sym_args,
      anon_sym_local,
  [162] = 2,
    ACTIONS(55), 1,
      aux_sym_constant_token1,
    ACTIONS(53), 2,
      anon_sym_PERCENT,
      anon_sym_0x,
  [170] = 1,
    ACTIONS(57), 1,
      sym_identifier,
  [174] = 1,
    ACTIONS(59), 1,
      anon_sym_sp,
  [178] = 1,
    ACTIONS(61), 1,
      aux_sym_constant_token1,
  [182] = 1,
    ACTIONS(63), 1,
      aux_sym_comment_token1,
  [186] = 1,
    ACTIONS(65), 1,
      anon_sym_COLON,
  [190] = 1,
    ACTIONS(67), 1,
      ts_builtin_sym_end,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(2)] = 0,
  [SMALL_STATE(3)] = 23,
  [SMALL_STATE(4)] = 46,
  [SMALL_STATE(5)] = 63,
  [SMALL_STATE(6)] = 80,
  [SMALL_STATE(7)] = 88,
  [SMALL_STATE(8)] = 96,
  [SMALL_STATE(9)] = 104,
  [SMALL_STATE(10)] = 112,
  [SMALL_STATE(11)] = 120,
  [SMALL_STATE(12)] = 128,
  [SMALL_STATE(13)] = 136,
  [SMALL_STATE(14)] = 144,
  [SMALL_STATE(15)] = 153,
  [SMALL_STATE(16)] = 162,
  [SMALL_STATE(17)] = 170,
  [SMALL_STATE(18)] = 174,
  [SMALL_STATE(19)] = 178,
  [SMALL_STATE(20)] = 182,
  [SMALL_STATE(21)] = 186,
  [SMALL_STATE(22)] = 190,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 0, 0, 0),
  [5] = {.entry = {.count = 1, .reusable = true}}, SHIFT(14),
  [7] = {.entry = {.count = 1, .reusable = true}}, SHIFT(15),
  [9] = {.entry = {.count = 1, .reusable = true}}, SHIFT(20),
  [11] = {.entry = {.count = 1, .reusable = true}}, SHIFT(17),
  [13] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 1, 0, 0),
  [15] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0),
  [17] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(14),
  [20] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(15),
  [23] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(20),
  [26] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0), SHIFT_REPEAT(17),
  [29] = {.entry = {.count = 1, .reusable = true}}, SHIFT(18),
  [31] = {.entry = {.count = 1, .reusable = true}}, SHIFT(19),
  [33] = {.entry = {.count = 1, .reusable = false}}, SHIFT(11),
  [35] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_pop, 3, 0, 0),
  [37] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_constant, 2, 0, 0),
  [39] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_predefined_symbol, 2, 0, 0),
  [41] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_comment, 2, 0, 0),
  [43] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_label_definition, 3, 0, 0),
  [45] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_constant, 1, 0, 0),
  [47] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_push, 3, 0, 0),
  [49] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_push_pop_arg, 1, 0, 0),
  [51] = {.entry = {.count = 1, .reusable = true}}, SHIFT(16),
  [53] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_virtual_memory_segment, 1, 0, 0),
  [55] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_virtual_memory_segment, 1, 0, 0),
  [57] = {.entry = {.count = 1, .reusable = true}}, SHIFT(21),
  [59] = {.entry = {.count = 1, .reusable = true}}, SHIFT(8),
  [61] = {.entry = {.count = 1, .reusable = true}}, SHIFT(7),
  [63] = {.entry = {.count = 1, .reusable = true}}, SHIFT(9),
  [65] = {.entry = {.count = 1, .reusable = true}}, SHIFT(10),
  [67] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
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
