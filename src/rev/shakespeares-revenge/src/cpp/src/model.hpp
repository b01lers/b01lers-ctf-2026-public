#ifndef SHAKESPEARE_CPP_MODEL_HPP
#define SHAKESPEARE_CPP_MODEL_HPP

#include <memory>
#include <optional>
#include <string>
#include <vector>

struct RawSentence {
    std::vector<std::string> tokens;
    char ending = '.';
};

struct Event {
    enum class Type {
        LINE,
        ENTRANCE,
        EXIT,
        EXEUNT,
        BREAKPOINT,
    };

    Type type = Type::BREAKPOINT;
    std::string speaker;
    std::vector<RawSentence> sentences;
    std::vector<std::string> characters;
};

struct Scene {
    std::string number;
    std::string name;
    std::vector<Event> events;
};

struct Act {
    std::string number;
    std::string name;
    std::vector<Scene> scenes;
};

struct Play {
    std::string title;
    std::vector<std::string> dramatis_personae;
    std::vector<Act> acts;
};

using StringList = std::vector<std::string>;
using RawSentenceList = std::vector<RawSentence>;
using EventList = std::vector<Event>;
using SceneList = std::vector<Scene>;
using ActList = std::vector<Act>;

struct Expr {
    enum class Kind {
        LITERAL,
        FIRST_PERSON,
        SECOND_PERSON,
        CHARACTER,
        UNARY,
        BINARY,
    };

    Kind kind = Kind::LITERAL;
    long long literal = 0;
    std::string text;
    std::shared_ptr<Expr> lhs;
    std::shared_ptr<Expr> rhs;
};

enum class CompareType {
    GREATER,
    LESS,
    EQUAL,
};

struct Operation {
    enum class Type {
        ENTER,
        EXIT,
        EXEUNT,
        BREAKPOINT,
        QUESTION,
        ASSIGN,
        INPUT,
        OUTPUT,
        PUSH,
        POP,
        REFERENCE,
        GOTO,
        SYSCALL,
    };

    Type type = Type::BREAKPOINT;
    std::string speaker;
    std::optional<bool> condition_positive;

    std::vector<std::string> characters;

    std::shared_ptr<Expr> expr_a;
    std::shared_ptr<Expr> expr_b;
    CompareType compare = CompareType::EQUAL;

    bool number_io = false;
    std::string destination_scene;
    std::string reference_character;
    std::string syscall_character;
};

struct ParseContext {
    Play play;
    std::string error;
};

bool parse_play_source(const std::string& source, Play& out_play, std::string& out_error);

#endif
