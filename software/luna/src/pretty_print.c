#include "pretty_print.h"
#include <stdarg.h>
#include <stdio.h>

void pp_header(const char *title) {
  printf(BOLD CYAN "\n🚀 %s\n" RESET, title);
  printf(CYAN "══════════════════════════════════════════════════════════════════\n" RESET);
}

void pp_subheader(const char *title) {
  printf(BOLD YELLOW "\n✨ %s\n" RESET, title);
  printf(YELLOW "──────────────────────────────────────────\n" RESET);
}

void pp_section_start(const char *title) {
  printf(BOLD GREEN "\n📋 %s\n" RESET, title);
}

void pp_section_end(void) {
  printf(DIM "────────────────────────────────────────\n" RESET);
}

void pp_step(const char *description) {
  printf(BLUE "  🔹 %s\n" RESET, description);
}

void pp_info(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf(WHITE "  ℹ️  ");
  vprintf(format, args);
  printf(RESET "\n");
  va_end(args);
}

void pp_success(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf(GREEN "  ✅ ");
  vprintf(format, args);
  printf(RESET "\n");
  va_end(args);
}

void pp_warning(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf(YELLOW "  ⚠️  ");
  vprintf(format, args);
  printf(RESET "\n");
  va_end(args);
}

void pp_error(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf(RED "  ❌ ");
  vprintf(format, args);
  printf(RESET "\n");
  va_end(args);
}

void pp_debug(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf(DIM "  🔧 ");
  vprintf(format, args);
  printf(RESET "\n");
  va_end(args);
}

void pp_separator(void) {
  printf(DIM "─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─\n" RESET);
}

void pp_memory_usage(uint16_t used, uint16_t total) {
  double percentage = (double)used / (double)total * 100.0;
  const char *color = (percentage < 50) ? GREEN : (percentage < 80) ? YELLOW : RED;
  printf("%s  💾 Arena: %d/%d (%.2f%%) memory used\n" RESET, color, used, total, percentage);
} 
