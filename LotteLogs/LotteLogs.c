#include <stdio.h>

void llog_helper(char* type, char* message) {
    printf("[%s] %s\n", type, message);
}

void llog_trace(char* message) { llog_helper("TRACE", message); }
void llog_debug(char* message) { llog_helper("DEBUG", message); }
void llog_info(char* message) { llog_helper("INFO ", message); }
void llog_warn(char* message) { llog_helper("WARN ", message); }
void llog_error(char* message) { llog_helper("ERROR", message); }
void llog_fatal(char* message) { llog_helper("FATAL", message); }