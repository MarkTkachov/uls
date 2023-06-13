#pragma once

#define RECURSIVE (1)
#define COLORED (2)
#define START_WITH_DOT (4)
#define SHOW_DOT_DDOT (8)
#define LONG_FORMAT (16)
#define TERM_OUTPUT (32)
#define FORCE_NEWLINE_AFTER_PRINT (64)
#define NO_TOTAL_LONGF (128)
#define NO_SORTING (256)
#define FORCE_MULTICOLUMN (512)

#define MX_ADD_FLAG(params, flag) params = params | flag
#define MX_DEL_FLAG(params, flag) params = params & ~flag
#define MX_HAS_FLAG(params, flag) ((params & flag) == flag)

#define USAGE "usage: uls [-ACGRafl1] [file ...]\n"

//ANSI color codes
#define BLK   "\x1B[30m"
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"

#define BG_BLK   "\x1B[40m"
#define BG_RED   "\x1B[41m"
#define BG_GRN   "\x1B[42m"
#define BG_YEL   "\x1B[43m"
#define BG_BLU   "\x1B[44m"
#define BG_MAG   "\x1B[45m"
#define BG_CYN   "\x1B[46m"
#define BG_WHT   "\x1B[47m"

#define RESET "\x1B[0m"

