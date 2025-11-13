#include "diagnostics.h"
#include "luna_string.h"
#include "source_spans.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct Diagnostic diagnostic_make(struct SourceFile *source,
                                  struct LunaString message, uint64_t start,
                                  uint64_t end) {
  return (struct Diagnostic){
      .source = source,
      .message = message,
      .start_offset = start,
      .end_offset = end,
  };
}

void print_header(struct LunaString filepath, uint32_t start_line,
                  uint32_t start_diagnostic_offset) {
  printf("  --> %s:%d:%d\n", filepath.data, start_line,
         start_diagnostic_offset);
}

void print_n_times(int num_spaces, char c) {
  for (int i = 0; i < num_spaces; i++) {
    printf("%c", c);
  }
}
//
// void diagnostic_print_multi_line(struct Diagnostic *diagnostic,
//                                  uint32_t start_line_number,
//                                  uint32_t start_line_number_start_offset,
//                                  uint32_t start_line_diagnostic_offset,
//                                  uint32_t end_line_number,
//                                  uint32_t end_line_number_start_offset,
//                                  uint32_t end_line_end_offset,
//                                  uint32_t num_spaces) {
//   printf("  --> %s:%d:%d-%d:%d\n", diagnostic->source->filepath.data, line_number,
//          diagnostic_line_start_offset);
// }

void diagnostic_print_single_line(struct Diagnostic *diagnostic,
                                  uint32_t line_number,
                                  uint32_t line_number_start_offset,
                                  uint32_t line_number_end_offset,
                                  uint32_t diagnostic_line_start_offset,
                                  uint32_t num_spaces) {
  printf("  --> %s:%d:%d\n", diagnostic->source->filepath.data, line_number,
         diagnostic_line_start_offset);

  print_n_times(num_spaces, ' ');
  puts("|");

  char buf[num_spaces];
  sprintf(buf, "%d", line_number);

  printf(" %s |", buf);

  int source_snippet_len =
      line_number_end_offset - line_number_start_offset - 1;
  char source_snippet[source_snippet_len + 1];

  memcpy(source_snippet,
         &diagnostic->source->content.data[line_number_start_offset + 1],
         source_snippet_len);
  source_snippet[source_snippet_len + 1] = '\0';

  printf("%s\n", source_snippet);

  print_n_times(num_spaces, ' ');
  printf("|");
  print_n_times(diagnostic_line_start_offset - 1, ' ');
  print_n_times(diagnostic->end_offset - diagnostic->start_offset, '^');
  puts("");
  print_n_times(num_spaces, ' ');
  puts("|");
}

void diagnostic_print(struct Diagnostic *diagnostic) {
  printf("\nerror: %s\n", diagnostic->message.data);

  uint32_t start_line;
  uint32_t start_line_offset;
  uint32_t start_line_end_offset;

  uint32_t end_line;
  uint32_t end_line_offset;
  uint32_t end_line_end_offset;

  line_map_query(&diagnostic->source->line_map, diagnostic->start_offset,
                 &start_line, &start_line_offset, &start_line_end_offset);
  line_map_query(&diagnostic->source->line_map, diagnostic->end_offset,
                 &end_line, &end_line_offset, &end_line_end_offset);

  // printf("start line: %d\n", start_line);
  // printf("start line offset: %d\n", start_line_offset);
  // printf("start line end offset line offset: %d\n", start_line_end_offset);
  //
  // printf("end line: %d\n", end_line);
  // printf("end line offset: %d\n", end_line_offset);
  // printf("end line end offset line offset: %d\n", end_line_end_offset);

  int start_diagnostic_offset = diagnostic->start_offset - start_line_offset;

  int length = snprintf(NULL, 0, "%d", start_line);
  int num_spaces = length + 2;

  if (start_line == end_line) {
    diagnostic_print_single_line(diagnostic, start_line, start_line_offset,
                                 start_line_end_offset, start_diagnostic_offset,
                                 num_spaces);
  } else {
    assert(0);
    // diagnostic_print_multi_line(
    //     diagnostic, start_line, start_line_offset, start_diagnostic_offset,
    //     end_line, end_line_offset, end_line_end_offset, num_spaces);
  }
}
