#include "diagnostics.h"
#include "source_spans.h"
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

void diagnostic_print(struct Diagnostic *diagnostic) {
  printf("\nerror: %s\n", diagnostic->message.data);

  uint32_t start_line;
  uint32_t start_line_offset;

  line_map_query(&diagnostic->source->line_map, diagnostic->start_offset,
                 &start_line, &start_line_offset);

  char current;
  int end_line_offset = diagnostic->end_offset;

  while ((current = diagnostic->source->content.data[end_line_offset],
          current != '\n' &&
              end_line_offset < diagnostic->source->content.length)) {
    end_line_offset++;
  }

  int start_diagnostic_offset = diagnostic->start_offset - start_line_offset;

  printf("  --> %s:%d:%d\n", diagnostic->source->filepath.data, start_line,
         start_diagnostic_offset);

  int length = snprintf(NULL, 0, "%d", 42);
  int num_spaces = length + 1;

  for (int i = 0; i < num_spaces; i++) {
    printf(" ");
  }
  puts("|");

  char buf[length + 1];
  sprintf(buf, "%d", start_line);

  printf(" %s |", buf);

  int source_snippet_len = end_line_offset - start_line_offset - 1;
  char source_snippet[source_snippet_len + 1];

  memcpy(source_snippet, &diagnostic->source->content.data[start_line_offset+1],
         source_snippet_len);
  source_snippet[source_snippet_len + 1] = '\0';

  printf("%s\n", source_snippet);

  for (int i = 0; i < num_spaces; i++) {
    printf(" ");
  }
  printf("|");
  for (int i = 1; i < start_diagnostic_offset; i++) {
    printf(" ");
  }
  for (int i = diagnostic->start_offset; i < (int)diagnostic->end_offset; i++) {
    printf("^");
  }
  puts("");

  for (int i = 0; i < num_spaces; i++) {
    printf(" ");
  }
  puts("|");
}
