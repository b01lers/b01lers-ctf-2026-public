%option noyywrap
%option yylineno

%{
#include "build/parser.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <string>

static std::string lower_copy(const char* text) {
    std::string value(text);
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}
%}

%%

[ \t\r\n]+            ;
":"                   return ':';
","                   return ',';
"!"                   return '!';
"\."                  return '.';
"\?"                  return '?';
"\["                  return '[';
"\]"                  return ']';

[A-Za-z][A-Za-z'\-]* {
    std::string lower = lower_copy(yytext);
    yylval.sval = ::strdup(yytext);

    if (lower == "act") {
        return KW_ACT;
    }
    if (lower == "scene") {
        return KW_SCENE;
    }
    if (lower == "enter") {
        return KW_ENTER;
    }
    if (lower == "exit") {
        return KW_EXIT;
    }
    if (lower == "exeunt") {
        return KW_EXEUNT;
    }
    if (lower == "and") {
        return KW_AND;
    }
    if (lower == "a") {
        return KW_A;
    }
    if (lower == "pause") {
        return KW_PAUSE;
    }

    return WORD;
}

.                       ;

%%
