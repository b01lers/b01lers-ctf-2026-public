#include "model.hpp"

#include <string>

struct yy_buffer_state;
typedef yy_buffer_state* YY_BUFFER_STATE;

extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char* text);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yylineno;

ParseContext* g_parse_context = nullptr;

bool parse_play_source(const std::string& source, Play& out_play, std::string& out_error) {
    ParseContext context;
    g_parse_context = &context;
    yylineno = 1;

    YY_BUFFER_STATE buffer = yy_scan_string(source.c_str());
    if (buffer == nullptr) {
        g_parse_context = nullptr;
        out_error = "Failed to initialize lexer buffer";
        return false;
    }

    const int parse_result = yyparse();
    yy_delete_buffer(buffer);
    g_parse_context = nullptr;

    if (parse_result != 0) {
        if (context.error.empty()) {
            out_error = "Unknown parse error";
        } else {
            out_error = context.error;
        }
        return false;
    }

    out_play = context.play;
    return true;
}
