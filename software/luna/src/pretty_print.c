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

void pp_draw_progress_bar(double percentage, int width) {
  const char *color = (percentage < 50) ? GREEN : (percentage < 80) ? YELLOW : RED;
  const char *emoji = (percentage < 25) ? "🟢" : (percentage < 50) ? "🟡" : (percentage < 75) ? "🟠" : "🔴";
  
  printf("%s[", color);
  int filled = (int)((percentage / 100.0) * width);
  
  for (int i = 0; i < width; i++) {
    if (i < filled) {
      printf("█");
    } else {
      printf("░");
    }
  }
  printf("]" RESET " %s %.1f%%", emoji, percentage);
}

void pp_memory_usage(uint16_t used, uint16_t total) {
  double percentage = (double)used / (double)total * 100.0;
  const char *status_color = (percentage < 50) ? GREEN : (percentage < 80) ? YELLOW : RED;
  const char *status_text = (percentage < 25) ? "Excellent" : 
                           (percentage < 50) ? "Good" : 
                           (percentage < 75) ? "Moderate" : 
                           (percentage < 90) ? "High" : "Critical";
  
  // Header
  printf(BOLD MAGENTA "\n🗂️  Arena Allocator Report\n" RESET);
  printf(MAGENTA "══════════════════════════════════════════════════════════════════\n" RESET);
  
  // Main usage display
  printf(BOLD WHITE "  📊 Allocator Status: " RESET);
  printf("%s%s" RESET "\n\n", status_color, status_text);
  
  // Progress bar
  printf(WHITE "  📈 Usage: " RESET);
  pp_draw_progress_bar(percentage, 30);
  printf("\n\n");
  
  // Detailed statistics
  printf(BOLD CYAN "  📋 Statistics:\n" RESET);
  printf(WHITE "    • Total Capacity: " RESET BOLD "%d bytes" RESET " (%.1f KB)\n", 
         total, (double)total / 1024.0);
  printf(WHITE "    • Memory Used:    " RESET "%s%d bytes" RESET " (%.1f KB)\n", 
         status_color, used, (double)used / 1024.0);
  printf(WHITE "    • Memory Free:    " RESET GREEN "%d bytes" RESET " (%.1f KB)\n", 
         total - used, (double)(total - used) / 1024.0);
  printf(WHITE "    • Efficiency:     " RESET "%s%.2f%%" RESET "\n", status_color, percentage);
  
  // Performance indicators
  printf(BOLD CYAN "\n  ⚡ Performance Indicators:\n" RESET);
  
  if (percentage < 25) {
    printf(GREEN "    ✅ Optimal compiler memory usage\n" RESET);
    printf(GREEN "    ✅ Excellent allocation efficiency\n" RESET);
    printf(GREEN "    ✅ Plenty of headroom available\n" RESET);
  } else if (percentage < 50) {
    printf(GREEN "    ✅ Good compiler memory usage\n" RESET);
    printf(YELLOW "    ⚠️  Moderate allocation load\n" RESET);
    printf(GREEN "    ✅ Sufficient memory remaining\n" RESET);
  } else if (percentage < 75) {
    printf(YELLOW "    ⚠️  High compiler memory usage\n" RESET);
    printf(YELLOW "    ⚠️  Consider optimizing allocations\n" RESET);
    printf(YELLOW "    ⚠️  Limited headroom remaining\n" RESET);
  } else if (percentage < 90) {
    printf(RED "    ⚠️  Very high compiler memory usage\n" RESET);
    printf(RED "    ⚠️  Risk of allocation failures\n" RESET);
    printf(RED "    ⚠️  Immediate optimization needed\n" RESET);
  } else {
    printf(RED "    ❌ Critical compiler memory usage\n" RESET);
    printf(RED "    ❌ High risk of out-of-memory\n" RESET);
    printf(RED "    ❌ Urgent optimization required\n" RESET);
  }
  
  // Footer
  printf(MAGENTA "\n══════════════════════════════════════════════════════════════════\n" RESET);
  printf(DIM "  Compilation completed successfully • Luna Compiler v1.0\n" RESET);
} 

