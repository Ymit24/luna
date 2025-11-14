#include "lexer.h"
#include "arena_allocator.h"
#include "luna_string.h"
#include "source_spans.h"
#include "token.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Lexer lexer_make(struct ArenaAllocator *allocator,
                        struct SourceFile source) {
  return (struct Lexer){
      .allocator = allocator, .source = source, .position = 0};
}

char lexer_peek(struct Lexer *lexer) {
  if (lexer->position > lexer->source.content.length) {
    return 0;
  }

  return lexer->source.content.data[lexer->position];
}

uint16_t lexer_read_integer(struct Lexer *lexer) {
  char buf[16] = {0};
  uint8_t index = 0;

  while (isdigit(lexer_peek(lexer)) && index < 16) {
    buf[index++] = lexer_peek(lexer);
    lexer->position++;
  }

  buf[index] = 0;

  return atoi(buf);
}

struct LunaString lexer_read_string(struct Lexer *lexer) {
  char *buf = arena_alloc(lexer->allocator, 256 * sizeof(char));
  uint16_t index = 0;

  while (lexer_peek(lexer) != '"' && index < 256) {
    char c = lexer_peek(lexer);
    if (c == '\\') {
      lexer->position++;
      switch (lexer_peek(lexer)) {
      case 'n':
        buf[index++] = '\n';
        break;
      case 't':
        buf[index++] = '\t';
        break;
      case 'r':
        buf[index++] = '\r';
        break;
      case 'b':
        buf[index++] = '\b';
        break;
      case 'f':
        buf[index++] = '\f';
        break;
      case 'v':
        buf[index++] = '\v';
        break;
      case 'a':
        buf[index++] = '\a';
        break;
      case '\\':
        buf[index++] = '\\';
        break;
      case '\'':
        buf[index++] = '\'';
        break;
      case '"':
        buf[index++] = '"';
        break;
      }
    } else {
      buf[index++] = c;
    }
    lexer->position++;
  }

  if (index >= 256) {
    puts("Lexer: string length surpassed 256.");
    assert(0);
  }

  buf[index] = 0;
  return (struct LunaString){.data = &buf[0], .length = index};
}

struct LunaString lexer_read_symbol(struct Lexer *lexer) {
  char *buf = arena_alloc(lexer->allocator, 64 * sizeof(char));
  uint8_t index = 0;

  while ((isalpha(lexer_peek(lexer)) || isdigit(lexer_peek(lexer)) ||
          lexer_peek(lexer) == '_') &&
         index < 64) {
    buf[index++] = lexer_peek(lexer);
    lexer->position++;
  }

  buf[index] = 0;
  return (struct LunaString){.data = &buf[0], .length = index};
}

void lexer_skip_whitespace(struct Lexer *lexer) {
  while (isspace(lexer_peek(lexer))) {
    lexer->position++;
  }
}

bool lexer_next(struct Lexer *lexer, struct Token *out_token) {
  lexer_skip_whitespace(lexer);
  char current = lexer_peek(lexer);

  if (current == 0) {
    out_token->type = T_EOF;
    out_token->span =
        span_make(&lexer->source, lexer->position, lexer->position);
    return false;
  }

  out_token->type = T_ILLEGAL;

  switch (current) {
  case '+':
    out_token->type = T_PLUS;
    break;
  case '-':
    out_token->type = T_MINUS;
    break;
  case '*':
    out_token->type = T_STAR;
    break;
  case '&':
    out_token->type = T_AMPERSAND;
    break;
  case '/':
    lexer->position++;
    if (lexer_peek(lexer) == '/') {
      char ch;
      while (ch = lexer_peek(lexer), (ch != '\n' && ch != 0)) {
        lexer->position++;
      }
      return lexer_next(lexer, out_token);
    }

    out_token->span =
        span_make(&lexer->source, lexer->position - 1, lexer->position - 1);
    out_token->type = T_SLASH;
    return true;
  case '(':
    out_token->type = T_LPAREN;
    break;
  case ')':
    out_token->type = T_RPAREN;
    break;
  case '{':
    out_token->type = T_LBRACE;
    break;
  case '}':
    out_token->type = T_RBRACE;
    break;
  case '[':
    out_token->type = T_LBRACK;
    break;
  case ']':
    out_token->type = T_RBRACK;
    break;
  case '!':
    out_token->type = T_EXCLAMATION;
    lexer->position++;
    if (lexer_peek(lexer) == '=') {
      out_token->type = T_NTEQ;

      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position);
      lexer->position++;
    } else {
      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position - 1);
    }
    return true;
  case '<':
    out_token->type = T_LANGLE;
    lexer->position++;
    if (lexer_peek(lexer) == '=') {
      out_token->type = T_LEQ;
      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position);
      lexer->position++;
    } else {
      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position - 1);
    }
    return true;
  case '>':
    out_token->type = T_RANGLE;
    lexer->position++;
    if (lexer_peek(lexer) == '=') {
      out_token->type = T_GEQ;
      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position);
      lexer->position++;
    } else {
      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position - 1);
    }
    return true;
  case ';':
    out_token->type = T_SEMICOLON;
    break;
  case ':':
    out_token->type = T_COLON;
    break;
  case ',':
    out_token->type = T_COMMA;
    break;
  case '.':
    out_token->type = T_PERIOD;
    break;
  case '=':
    out_token->type = T_EQUALS;
    lexer->position++;
    if (lexer_peek(lexer) == '=') {
      out_token->type = T_EQEQ;
      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position);
      lexer->position++;
    } else {
      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position - 1);
    }
    return true;
  case '"':
    out_token->type = T_STRING;

    uint32_t start = lexer->position;
    lexer->position++;

    out_token->value.symbol = lexer_read_string(lexer);
    assert(lexer_peek(lexer) == '"');
    out_token->span = span_make(&lexer->source, start, lexer->position);
    lexer->position++;
    return true;
  case '@':
    lexer->position++;
    if (lexer->source.content.length - lexer->position >= 6 &&
        strncmp("extern", &lexer->source.content.data[lexer->position], 6) ==
            0) {
      out_token->type = T_EXTERN;
      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position + 6);
      lexer->position += 6;
      return true;
    } else if (lexer->source.content.length - lexer->position >= 8 &&
               strncmp("variadic", &lexer->source.content.data[lexer->position],
                       8) == 0) {
      out_token->type = T_VARIADIC;
      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position + 8);
      lexer->position += 8;
      return true;
    } else if (lexer->source.content.length - lexer->position >= 4 &&
               strncmp("cast", &lexer->source.content.data[lexer->position],
                       4) == 0) {
      out_token->type = T_CAST;
      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position + 4);
      lexer->position += 4;
      return true;
    } else if (lexer->source.content.length - lexer->position >= 9 &&
               strncmp("valuesize",
                       &lexer->source.content.data[lexer->position], 9) == 0) {
      out_token->type = T_VALUESIZE;
      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position + 9);
      lexer->position += 9;
      return true;
    } else if (lexer->source.content.length - lexer->position >= 8 &&
               strncmp("typesize", &lexer->source.content.data[lexer->position],
                       8) == 0) {
      out_token->type = T_TYPESIZE;
      out_token->span =
          span_make(&lexer->source, lexer->position - 1, lexer->position + 8);
      lexer->position += 8;
      return true;
    }
    break;
  default: {
    if (isdigit(current)) {
      out_token->type = T_INTEGER;
      uint32_t start = lexer->position;
      out_token->value.integer = lexer_read_integer(lexer);
      out_token->span = span_make(&lexer->source, start, lexer->position - 1);
      return true;
    } else {
      if (lexer->source.content.length - lexer->position >= 3 &&
          strncmp("let", &lexer->source.content.data[lexer->position], 3) ==
              0) {
        out_token->type = T_LET;
        out_token->span =
            span_make(&lexer->source, lexer->position, lexer->position + 3);
        lexer->position += 3;
        return true;
      } else if (lexer->source.content.length - lexer->position >= 5 &&
                 strncmp("const", &lexer->source.content.data[lexer->position],
                         5) == 0) {
        out_token->type = T_CONST;
        out_token->span =
            span_make(&lexer->source, lexer->position, lexer->position + 5);
        lexer->position += 5;
        return true;
      } else if (lexer->source.content.length - lexer->position >= 2 &&
                 strncmp("fn", &lexer->source.content.data[lexer->position],
                         2) == 0) {
        out_token->type = T_FN;
        out_token->span =
            span_make(&lexer->source, lexer->position, lexer->position + 2);
        lexer->position += 2;
        return true;
      } else if (lexer->source.content.length - lexer->position >= 2 &&
                 strncmp("if", &lexer->source.content.data[lexer->position],
                         2) == 0) {
        out_token->type = T_IF;
        out_token->span =
            span_make(&lexer->source, lexer->position, lexer->position + 2);
        lexer->position += 2;
        return true;
      } else if (lexer->source.content.length - lexer->position >= 4 &&
                 strncmp("else", &lexer->source.content.data[lexer->position],
                         4) == 0) {
        out_token->type = T_ELSE;
        out_token->span =
            span_make(&lexer->source, lexer->position, lexer->position + 4);
        lexer->position += 4;
        return true;
      } else if (lexer->source.content.length - lexer->position >= 6 &&
                 strncmp("return", &lexer->source.content.data[lexer->position],
                         6) == 0) {
        out_token->type = T_RETURN;
        out_token->span =
            span_make(&lexer->source, lexer->position, lexer->position + 6);
        lexer->position += 6;
        return true;
      } else if (lexer->source.content.length - lexer->position >= 6 &&
                 strncmp("struct", &lexer->source.content.data[lexer->position],
                         6) == 0) {
        out_token->type = T_STRUCT;
        out_token->span =
            span_make(&lexer->source, lexer->position, lexer->position + 6);
        lexer->position += 6;
        return true;
      } else if (lexer->source.content.length - lexer->position >= 5 &&
                 strncmp("while", &lexer->source.content.data[lexer->position],
                         5) == 0) {
        out_token->type = T_WHILE;
        out_token->span =
            span_make(&lexer->source, lexer->position, lexer->position + 5);
        lexer->position += 5;
        return true;
      } else if (lexer->source.content.length - lexer->position >= 3 &&
                 strncmp("mod", &lexer->source.content.data[lexer->position],
                         3) == 0) {
        out_token->type = T_MOD;
        out_token->span =
            span_make(&lexer->source, lexer->position, lexer->position + 3);
        lexer->position += 3;
        return true;
      } else {
        out_token->type = T_SYMBOL;
        uint32_t start = lexer->position;
        out_token->value.symbol = lexer_read_symbol(lexer);
        out_token->span = span_make(&lexer->source, start, lexer->position);
        return true;
      }
    }
  }
  }

  out_token->span = span_make(&lexer->source, lexer->position, lexer->position);

  lexer->position++;
  return true;
}
