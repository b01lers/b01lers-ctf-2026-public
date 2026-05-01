%{
#include "src/model.hpp"

#include <cstdlib>
#include <string>

extern int yylex();
extern int yylineno;
void yyerror(const char* message);

extern ParseContext* g_parse_context;

static std::string take_cstring(char* text) {
    std::string result = text == nullptr ? "" : text;
    std::free(text);
    return result;
}

static void append_with_space(std::string& base, const std::string& word) {
    if (!base.empty()) {
        base += " ";
    }
    base += word;
}
%}

%code requires {
#include "src/model.hpp"
#include <string>
}

%glr-parser
%define parse.error verbose

%union {
    char* sval;
    char ch;
    std::string* str;
    StringList* str_list;
    RawSentence* sentence;
    RawSentenceList* sentence_list;
    Event* event;
    EventList* event_list;
    Scene* scene;
    SceneList* scene_list;
    Act* act;
    ActList* act_list;
    Play* play;
}

%token <sval> WORD
%token <sval> KW_ACT KW_SCENE KW_ENTER KW_EXIT KW_EXEUNT KW_AND KW_A KW_PAUSE

%type <play> play
%type <str> name
%type <sval> wordish name_word
%type <str_list> text_words text_words_opt personae character_list sentence_word_seq
%type <str> sentence_word
%type <sentence> sentence
%type <sentence_list> sentence_list
%type <event> event line entrance exit exeunt breakpoint
%type <event_list> events
%type <scene> scene
%type <scene_list> scenes
%type <act> act
%type <act_list> acts
%type <ch> sentence_end

%%

input:
    play {
        g_parse_context->play = *$1;
        delete $1;
    }
;

play:
    text_words sentence_end personae acts {
        Play* result = new Play();
        result->title = "";
        if (!$1->empty()) {
            result->title = (*$1)[0];
            for (std::size_t i = 1; i < $1->size(); ++i) {
                append_with_space(result->title, (*$1)[i]);
            }
        }
        result->dramatis_personae = *$3;
        result->acts = *$4;
        delete $1;
        delete $3;
        delete $4;
        $$ = result;
    }
;

personae:
    /* empty */ {
        $$ = new StringList();
    }
    | personae name ',' text_words_opt sentence_end {
        std::string ignored_delimiter = ",";
        (void)ignored_delimiter;
        $1->push_back(*$2);
        delete $2;
        delete $4;
        $$ = $1;
    }
;

acts:
    /* empty */ {
        $$ = new ActList();
    }
    | acts act {
        $1->push_back(*$2);
        delete $2;
        $$ = $1;
    }
;

act:
    KW_ACT wordish ':' text_words_opt sentence_end scenes {
        Act* result = new Act();
        std::string ignored_keyword = take_cstring($1);
        (void)ignored_keyword;
        result->number = take_cstring($2);
        result->name = "";
        if (!$4->empty()) {
            result->name = (*$4)[0];
            for (std::size_t i = 1; i < $4->size(); ++i) {
                append_with_space(result->name, (*$4)[i]);
            }
        }
        result->scenes = *$6;
        delete $4;
        delete $6;
        $$ = result;
    }
;

scenes:
    /* empty */ {
        $$ = new SceneList();
    }
    | scenes scene {
        $1->push_back(*$2);
        delete $2;
        $$ = $1;
    }
;

scene:
    KW_SCENE wordish ':' text_words_opt sentence_end events {
        Scene* result = new Scene();
        std::string ignored_keyword = take_cstring($1);
        (void)ignored_keyword;
        result->number = take_cstring($2);
        result->name = "";
        if (!$4->empty()) {
            result->name = (*$4)[0];
            for (std::size_t i = 1; i < $4->size(); ++i) {
                append_with_space(result->name, (*$4)[i]);
            }
        }
        result->events = *$6;
        delete $4;
        delete $6;
        $$ = result;
    }
;

events:
    /* empty */ {
        $$ = new EventList();
    }
    | events event {
        $1->push_back(*$2);
        delete $2;
        $$ = $1;
    }
;

event:
    line { $$ = $1; }
    | entrance { $$ = $1; }
    | exit { $$ = $1; }
    | exeunt { $$ = $1; }
    | breakpoint { $$ = $1; }
;

line:
    name ':' sentence_list {
        Event* result = new Event();
        result->type = Event::Type::LINE;
        result->speaker = *$1;
        result->sentences = *$3;
        delete $1;
        delete $3;
        $$ = result;
    }
;

sentence_list:
    sentence {
        $$ = new RawSentenceList();
        $$->push_back(*$1);
        delete $1;
    }
    | sentence_list sentence {
        $1->push_back(*$2);
        delete $2;
        $$ = $1;
    }
;

sentence:
    sentence_word_seq sentence_end {
        RawSentence* result = new RawSentence();
        result->tokens = *$1;
        result->ending = $2;
        delete $1;
        $$ = result;
    }
;

sentence_word_seq:
    sentence_word {
        $$ = new StringList();
        $$->push_back(*$1);
        delete $1;
    }
    | sentence_word_seq sentence_word {
        $1->push_back(*$2);
        delete $2;
        $$ = $1;
    }
;

sentence_word:
    wordish {
        $$ = new std::string(take_cstring($1));
    }
    | ',' {
        $$ = new std::string(",");
    }
;

entrance:
    '[' KW_ENTER character_list ']' {
        Event* result = new Event();
        result->type = Event::Type::ENTRANCE;
        std::string ignored_keyword = take_cstring($2);
        (void)ignored_keyword;
        result->characters = *$3;
        delete $3;
        $$ = result;
    }
;

exit:
    '[' KW_EXIT name ']' {
        Event* result = new Event();
        result->type = Event::Type::EXIT;
        std::string ignored_keyword = take_cstring($2);
        (void)ignored_keyword;
        result->characters.push_back(*$3);
        delete $3;
        $$ = result;
    }
;

exeunt:
    '[' KW_EXEUNT ']' {
        Event* result = new Event();
        result->type = Event::Type::EXEUNT;
        std::string ignored_keyword = take_cstring($2);
        (void)ignored_keyword;
        $$ = result;
    }
    | '[' KW_EXEUNT character_list ']' {
        Event* result = new Event();
        result->type = Event::Type::EXEUNT;
        std::string ignored_keyword = take_cstring($2);
        (void)ignored_keyword;
        result->characters = *$3;
        delete $3;
        $$ = result;
    }
;

breakpoint:
    '[' KW_A KW_PAUSE ']' {
        Event* result = new Event();
        result->type = Event::Type::BREAKPOINT;
        std::string ignored_a = take_cstring($2);
        std::string ignored_pause = take_cstring($3);
        (void)ignored_a;
        (void)ignored_pause;
        $$ = result;
    }
;

character_list:
    name {
        $$ = new StringList();
        $$->push_back(*$1);
        delete $1;
    }
    | character_list ',' name {
        $1->push_back(*$3);
        delete $3;
        $$ = $1;
    }
    | character_list KW_AND name {
        std::string ignored_and = take_cstring($2);
        (void)ignored_and;
        $1->push_back(*$3);
        delete $3;
        $$ = $1;
    }
;

name:
    name_word {
        $$ = new std::string(take_cstring($1));
    }
    | name name_word {
        append_with_space(*$1, take_cstring($2));
        $$ = $1;
    }
;

name_word:
    WORD { $$ = $1; }
;

text_words_opt:
    /* empty */ {
        $$ = new StringList();
    }
    | text_words {
        $$ = $1;
    }
;

text_words:
    wordish {
        $$ = new StringList();
        $$->push_back(take_cstring($1));
    }
    | text_words wordish {
        $1->push_back(take_cstring($2));
        $$ = $1;
    }
    | text_words ',' {
        $1->push_back(",");
        $$ = $1;
    }
;

wordish:
    WORD { $$ = $1; }
    | KW_ACT { $$ = $1; }
    | KW_SCENE { $$ = $1; }
    | KW_ENTER { $$ = $1; }
    | KW_EXIT { $$ = $1; }
    | KW_EXEUNT { $$ = $1; }
    | KW_AND { $$ = $1; }
    | KW_A { $$ = $1; }
    | KW_PAUSE { $$ = $1; }
;

sentence_end:
    '!' { $$ = '!'; }
    | '.' { $$ = '.'; }
    | '?' { $$ = '?'; }
;

%%

void yyerror(const char* message) {
    if (g_parse_context != nullptr) {
        g_parse_context->error = "line " + std::to_string(yylineno) + ": " + message;
    }
}
