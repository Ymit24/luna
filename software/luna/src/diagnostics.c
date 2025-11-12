#include "diagnostics.h"
#include <stdio.h>
#include <string.h>

struct Diagnostic diagnostic_entry_make(struct SourceFile *source,
                                        struct LunaString message,
                                        uint64_t start, uint64_t end) {
  return (struct Diagnostic){
      .source = source,
      .message = message,
      .start_offset = start,
      .end_offset = end,
  };
}

void diagnostic_entry_print(struct Diagnostic *diagnostic) {
  printf("\nerror: %s\n", diagnostic->message.data);
  int start_line = 2;
  int start_line_offset = diagnostic->start_offset;

  char current;
  while ((current = diagnostic->source->content.data[start_line_offset - 1],
          current != '\n' && start_line_offset > 0)) {
    start_line_offset--;
  }
  int end_line_offset = diagnostic->end_offset;

  while ((current = diagnostic->source->content.data[end_line_offset],
          current != '\n' &&
              end_line_offset < diagnostic->source->content.length)) {
    end_line_offset++;
  }

  int start_diagnostic_offset = diagnostic->start_offset;

  printf("  --> %s:%d:%d\n", diagnostic->source->filepath.data, start_line,
         start_diagnostic_offset - start_line_offset);

  int length = snprintf(NULL, 0, "%d", 42);
  int num_spaces = length + 1;

  for (int i = 0; i < num_spaces; i++) {
    printf(" ");
  }
  puts("|");

  // int buflen = (int)((ceil(log10(start_line)) + 1) * sizeof(char));
  // char buf[buflen];

  char buf[length + 1];

  sprintf(buf, "%d", start_line);

  printf(" %s |", buf);

  int source_snippet_len = end_line_offset - start_line_offset;
  char source_snippet[source_snippet_len + 1];

  memcpy(source_snippet, &diagnostic->source->content.data[start_line_offset],
         source_snippet_len);
  source_snippet[source_snippet_len + 1] = '\0';

  printf("%s\n", source_snippet);

  for (int i = 0; i < num_spaces; i++) {
    printf(" ");
  }
  printf("|");
  for (int i = start_line_offset; i < start_diagnostic_offset; i++) {
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

  // int number_of_spaces = strlen() puts("   | ");
}
