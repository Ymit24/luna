#ifndef PRETTY_PRINT_H
#define PRETTY_PRINT_H

#include <stdint.h>

// ANSI color codes
#define RESET    "\033[0m"
#define BOLD     "\033[1m"
#define DIM      "\033[2m"
#define RED      "\033[31m"
#define GREEN    "\033[32m"
#define YELLOW   "\033[33m"
#define BLUE     "\033[34m"
#define MAGENTA  "\033[35m"
#define CYAN     "\033[36m"
#define WHITE    "\033[37m"

// Pretty printing functions
void pp_header(const char *title);
void pp_subheader(const char *title);
void pp_section_start(const char *title);
void pp_section_end(void);
void pp_step(const char *description);
void pp_info(const char *format, ...);
void pp_success(const char *format, ...);
void pp_warning(const char *format, ...);
void pp_error(const char *format, ...);
void pp_debug(const char *format, ...);
void pp_separator(void);
void pp_draw_progress_bar(double percentage, int width);
void pp_memory_usage(uint16_t used, uint16_t total);

#endif 
