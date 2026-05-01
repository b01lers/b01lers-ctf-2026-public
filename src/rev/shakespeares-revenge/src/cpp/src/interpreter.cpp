#include "interpreter.hpp"

#include <algorithm>
#include <cerrno>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <sys/syscall.h>
#ifndef SPL_ENABLE_IO_STYLES
#define SPL_ENABLE_IO_STYLES 0
#endif

#include <unordered_map>
#include <unordered_set>
#include <unistd.h>
#include <utility>

namespace {

std::string to_lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

std::string to_upper(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });
    return value;
}

std::string join_words(const std::vector<std::string>& words, const std::string& separator = " ") {
    std::ostringstream stream;
    for (std::size_t i = 0; i < words.size(); ++i) {
        if (i > 0) {
            stream << separator;
        }
        stream << words[i];
    }
    return stream.str();
}

std::vector<std::string> split_words(const std::string& text) {
    std::istringstream stream(text);
    std::vector<std::string> words;
    std::string part;
    while (stream >> part) {
        words.push_back(part);
    }
    return words;
}

std::string normalize_name(const std::string& name) {
    std::vector<std::string> words = split_words(name);
    for (std::string& word : words) {
        std::string lower = to_lower(word);
        if (lower == "of") {
            word = "of";
            continue;
        }
        if (!lower.empty()) {
            lower[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(lower[0])));
        }
        word = lower;
    }
    return join_words(words);
}

bool is_valid_roman(const std::string& value) {
    static const std::regex roman_pattern(
        R"(^M{0,4}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$)",
        std::regex_constants::icase
    );
    return std::regex_match(value, roman_pattern) && !value.empty();
}

std::string normalize_roman(std::string value) {
    value.erase(std::remove_if(value.begin(), value.end(), [](unsigned char c) { return std::isspace(c) != 0; }), value.end());
    if (!is_valid_roman(value)) {
        throw std::runtime_error("Invalid Roman numeral: " + value);
    }
    return to_upper(value);
}

bool is_be_word(const std::string& token) {
    static const std::unordered_set<std::string> be_words = {"am", "are", "art", "be", "is"};
    return be_words.count(token) > 0;
}

bool is_second_person(const std::string& token) {
    static const std::unordered_set<std::string> words = {"thee", "thou", "you", "thyself", "yourself"};
    return words.count(token) > 0;
}

bool is_second_person_possessive(const std::string& token) {
    static const std::unordered_set<std::string> words = {"thine", "thy", "your"};
    return words.count(token) > 0;
}

bool is_first_person(const std::string& token) {
    static const std::unordered_set<std::string> words = {"i", "me", "myself"};
    return words.count(token) > 0;
}

bool is_possessive(const std::string& token) {
    static const std::unordered_set<std::string> words = {
        "mine", "my", "thine", "thy", "your", "his", "her", "its", "their"
    };
    return words.count(token) > 0;
}

bool is_article(const std::string& token) {
    static const std::unordered_set<std::string> words = {"a", "an", "the"};
    return words.count(token) > 0;
}

bool is_positive_adjective(const std::string& token) {
    static const std::unordered_set<std::string> words = {
        "amazing", "beautiful", "blossoming", "bold", "brave", "charming", "clearest", "cunning", "cute", "delicious",
        "embroidered", "fair", "fine", "gentle", "golden", "good", "handsome", "happy", "healthy", "honest",
        "lovely", "loving", "mighty", "noble", "peaceful", "pretty", "prompt", "proud", "reddest", "rich",
        "smooth", "sunny", "sweet", "sweetest", "trustworthy", "warm"
    };
    return words.count(token) > 0;
}

bool is_negative_adjective(const std::string& token) {
    static const std::unordered_set<std::string> words = {
        "bad", "cowardly", "cursed", "damned", "dirty", "disgusting", "distasteful", "dusty", "evil", "fat-kidneyed",
        "fatherless", "fat", "foul", "hairy", "half-witted", "horrible", "horrid", "infected", "lying", "miserable",
        "misused", "oozing", "rotten", "smelly", "snotty", "sorry", "stinking", "stuffed", "stupid", "vile",
        "villainous", "worried"
    };
    return words.count(token) > 0;
}

bool is_neutral_adjective(const std::string& token) {
    static const std::unordered_set<std::string> words = {
        "big", "black", "blue", "bluest", "bottomless", "furry", "green", "hard", "huge", "large",
        "little", "normal", "old", "purple", "red", "rural", "small", "tiny", "white", "yellow"
    };
    return words.count(token) > 0;
}

bool is_any_adjective(const std::string& token) {
    return is_positive_adjective(token) || is_negative_adjective(token) || is_neutral_adjective(token);
}

bool is_negative_noun(const std::string& token) {
    static const std::unordered_set<std::string> words = {
        "hell", "microsoft", "bastard", "beggar", "blister", "codpiece", "coward", "curse", "death", "devil",
        "draught", "famine", "flirt-gill", "goat", "hate", "hog", "hound", "leech", "lie", "pig", "plague",
        "starvation", "toad", "war", "wolf"
    };
    return words.count(token) > 0;
}

bool is_positive_or_neutral_noun(const std::string& token) {
    static const std::unordered_set<std::string> words = {
        "animal", "aunt", "brother", "cat", "chihuahua", "cousin", "cow", "daughter", "door", "face",
        "father", "fellow", "granddaughter", "grandfather", "grandmother", "grandson", "hair", "hamster", "horse", "lamp",
        "lantern", "mistletoe", "moon", "morning", "mother", "nephew", "niece", "nose", "purse", "road",
        "roman", "sister", "sky", "son", "squirrel", "thing", "town", "tree", "uncle", "wind",
        "heaven", "king", "lord", "angel", "flower", "happiness", "joy", "plum", "hero", "rose", "kingdom", "pony"
    };
    return words.count(token) > 0;
}

bool is_negative_noun_phrase(const std::string& combined) {
    return is_negative_noun(combined);
}

bool is_positive_or_neutral_noun_phrase(const std::string& combined) {
    static const std::unordered_set<std::string> two_word_nouns = {
        "stone wall", "summer's day"
    };
    return is_positive_or_neutral_noun(combined) || two_word_nouns.count(combined) > 0;
}

long long power_of_two(std::size_t exponent) {
    long long value = 1;
    for (std::size_t i = 0; i < exponent; ++i) {
        if (value > std::numeric_limits<long long>::max() / 2) {
            throw std::runtime_error("Numeric overflow while evaluating noun phrase");
        }
        value *= 2;
    }
    return value;
}

std::string printable_char(long long code) {
    if (code < 0 || code > 0x10FFFF) {
        throw std::runtime_error("Invalid character code: " + std::to_string(code));
    }
    return std::string(1, static_cast<char>(code));
}

std::optional<int> syscall_argument_count(long long syscall_number) {
    static const std::unordered_map<long long, int> table = [] {
    std::unordered_map<long long, int> map;

#ifdef SYS_read
    map.emplace(SYS_read, 3);
#endif
#ifdef SYS_write
    map.emplace(SYS_write, 3);
#endif
#ifdef SYS_open
    map.emplace(SYS_open, 3);
#endif
#ifdef SYS_close
    map.emplace(SYS_close, 1);
#endif
#ifdef SYS_stat
    map.emplace(SYS_stat, 2);
#endif
#ifdef SYS_fstat
    map.emplace(SYS_fstat, 2);
#endif
#ifdef SYS_lstat
    map.emplace(SYS_lstat, 2);
#endif
#ifdef SYS_poll
    map.emplace(SYS_poll, 3);
#endif
#ifdef SYS_lseek
    map.emplace(SYS_lseek, 3);
#endif
#ifdef SYS_mmap
    map.emplace(SYS_mmap, 6);
#endif
#ifdef SYS_mprotect
    map.emplace(SYS_mprotect, 3);
#endif
#ifdef SYS_munmap
    map.emplace(SYS_munmap, 2);
#endif
#ifdef SYS_brk
    map.emplace(SYS_brk, 1);
#endif
#ifdef SYS_rt_sigaction
    map.emplace(SYS_rt_sigaction, 4);
#endif
#ifdef SYS_rt_sigprocmask
    map.emplace(SYS_rt_sigprocmask, 4);
#endif
#ifdef SYS_rt_sigreturn
    map.emplace(SYS_rt_sigreturn, 0);
#endif
#ifdef SYS_ioctl
    map.emplace(SYS_ioctl, 3);
#endif
#ifdef SYS_pread64
    map.emplace(SYS_pread64, 4);
#endif
#ifdef SYS_pwrite64
    map.emplace(SYS_pwrite64, 4);
#endif
#ifdef SYS_readv
    map.emplace(SYS_readv, 3);
#endif
#ifdef SYS_writev
    map.emplace(SYS_writev, 3);
#endif
#ifdef SYS_access
    map.emplace(SYS_access, 2);
#endif
#ifdef SYS_pipe
    map.emplace(SYS_pipe, 1);
#endif
#ifdef SYS_select
    map.emplace(SYS_select, 5);
#endif
#ifdef SYS_sched_yield
    map.emplace(SYS_sched_yield, 0);
#endif
#ifdef SYS_mremap
    map.emplace(SYS_mremap, 5);
#endif
#ifdef SYS_msync
    map.emplace(SYS_msync, 3);
#endif
#ifdef SYS_mincore
    map.emplace(SYS_mincore, 3);
#endif
#ifdef SYS_madvise
    map.emplace(SYS_madvise, 3);
#endif
#ifdef SYS_dup
    map.emplace(SYS_dup, 1);
#endif
#ifdef SYS_dup2
    map.emplace(SYS_dup2, 2);
#endif
#ifdef SYS_pause
    map.emplace(SYS_pause, 0);
#endif
#ifdef SYS_nanosleep
    map.emplace(SYS_nanosleep, 2);
#endif
#ifdef SYS_getpid
    map.emplace(SYS_getpid, 0);
#endif
#ifdef SYS_sendfile
    map.emplace(SYS_sendfile, 4);
#endif
#ifdef SYS_socket
    map.emplace(SYS_socket, 3);
#endif
#ifdef SYS_connect
    map.emplace(SYS_connect, 3);
#endif
#ifdef SYS_accept
    map.emplace(SYS_accept, 3);
#endif
#ifdef SYS_sendto
    map.emplace(SYS_sendto, 6);
#endif
#ifdef SYS_recvfrom
    map.emplace(SYS_recvfrom, 6);
#endif
#ifdef SYS_sendmsg
    map.emplace(SYS_sendmsg, 3);
#endif
#ifdef SYS_recvmsg
    map.emplace(SYS_recvmsg, 3);
#endif
#ifdef SYS_shutdown
    map.emplace(SYS_shutdown, 2);
#endif
#ifdef SYS_bind
    map.emplace(SYS_bind, 3);
#endif
#ifdef SYS_listen
    map.emplace(SYS_listen, 2);
#endif
#ifdef SYS_getsockname
    map.emplace(SYS_getsockname, 3);
#endif
#ifdef SYS_getpeername
    map.emplace(SYS_getpeername, 3);
#endif
#ifdef SYS_socketpair
    map.emplace(SYS_socketpair, 4);
#endif
#ifdef SYS_setsockopt
    map.emplace(SYS_setsockopt, 5);
#endif
#ifdef SYS_getsockopt
    map.emplace(SYS_getsockopt, 5);
#endif
#ifdef SYS_clone
    map.emplace(SYS_clone, 5);
#endif
#ifdef SYS_fork
    map.emplace(SYS_fork, 0);
#endif
#ifdef SYS_vfork
    map.emplace(SYS_vfork, 0);
#endif
#ifdef SYS_execve
    map.emplace(SYS_execve, 3);
#endif
#ifdef SYS_exit
    map.emplace(SYS_exit, 1);
#endif
#ifdef SYS_wait4
    map.emplace(SYS_wait4, 4);
#endif
#ifdef SYS_kill
    map.emplace(SYS_kill, 2);
#endif
#ifdef SYS_uname
    map.emplace(SYS_uname, 1);
#endif
#ifdef SYS_getuid
    map.emplace(SYS_getuid, 0);
#endif
#ifdef SYS_getgid
    map.emplace(SYS_getgid, 0);
#endif
#ifdef SYS_geteuid
    map.emplace(SYS_geteuid, 0);
#endif
#ifdef SYS_getegid
    map.emplace(SYS_getegid, 0);
#endif
#ifdef SYS_setuid
    map.emplace(SYS_setuid, 1);
#endif
#ifdef SYS_setgid
    map.emplace(SYS_setgid, 1);
#endif
#ifdef SYS_getppid
    map.emplace(SYS_getppid, 0);
#endif
#ifdef SYS_getpgrp
    map.emplace(SYS_getpgrp, 0);
#endif
#ifdef SYS_setsid
    map.emplace(SYS_setsid, 0);
#endif
#ifdef SYS_gettid
    map.emplace(SYS_gettid, 0);
#endif
#ifdef SYS_openat
    map.emplace(SYS_openat, 4);
#endif
#ifdef SYS_newfstatat
    map.emplace(SYS_newfstatat, 4);
#endif
#ifdef SYS_exit_group
    map.emplace(SYS_exit_group, 1);
#endif
#ifdef SYS_clock_gettime
    map.emplace(SYS_clock_gettime, 2);
#endif
#ifdef SYS_clock_nanosleep
    map.emplace(SYS_clock_nanosleep, 4);
#endif
#ifdef SYS_futex
    map.emplace(SYS_futex, 6);
#endif
#ifdef SYS_epoll_create1
    map.emplace(SYS_epoll_create1, 1);
#endif
#ifdef SYS_epoll_ctl
    map.emplace(SYS_epoll_ctl, 4);
#endif
#ifdef SYS_epoll_pwait
    map.emplace(SYS_epoll_pwait, 6);
#endif
#ifdef SYS_eventfd2
    map.emplace(SYS_eventfd2, 2);
#endif
#ifdef SYS_getrandom
    map.emplace(SYS_getrandom, 3);
#endif
#ifdef SYS_prlimit64
    map.emplace(SYS_prlimit64, 4);
#endif

    return map;
    }();

    const auto it = table.find(syscall_number);
    if (it == table.end()) {
    return std::nullopt;
    }
    return it->second;
}

long long invoke_syscall(long long syscall_number, const std::vector<long long>& args) {
    switch (args.size()) {
    case 0:
        return ::syscall(syscall_number);
    case 1:
        return ::syscall(syscall_number, args[0]);
    case 2:
        return ::syscall(syscall_number, args[0], args[1]);
    case 3:
        return ::syscall(syscall_number, args[0], args[1], args[2]);
    case 4:
        return ::syscall(syscall_number, args[0], args[1], args[2], args[3]);
    case 5:
        return ::syscall(syscall_number, args[0], args[1], args[2], args[3], args[4]);
    case 6:
        return ::syscall(syscall_number, args[0], args[1], args[2], args[3], args[4], args[5]);
    default:
        throw std::runtime_error("Unsupported syscall argument count: " + std::to_string(args.size()));
    }
}

class RuntimeCharacter {
public:
    using ValueType = std::uint32_t;

    RuntimeCharacter() = default;

    static ValueType encode(long long value) {
        return static_cast<ValueType>(static_cast<std::int64_t>(value));
    }

    static long long decode(ValueType value) {
        return static_cast<long long>(static_cast<std::int64_t>(value));
    }

    long long value() const {
        return value_;
    }

    void set_value(long long value) {
        value_ = value;
#ifdef CHARACTER_STATE_DEBUG
        log_state("set_value", value);
#endif
    }

    void set_reference(RuntimeCharacter* source) {
        reference_source_ = source;
        reference_ = source != nullptr ? source->stack_pointer_value() : nullptr;
        value_ = pointer_to_long_long(reference_pointer());
#ifdef CHARACTER_STATE_DEBUG
        log_state("reference", value_);
#endif
    }

    void push(long long value) {
        const std::uint64_t raw = static_cast<std::uint64_t>(static_cast<std::int64_t>(value));
        const ValueType high = static_cast<ValueType>((raw >> 32) & 0xFFFFFFFFull);
        const ValueType low = static_cast<ValueType>(raw & 0xFFFFFFFFull);
        if (high != 0) {
            stack_.push_back(high);
        }
        stack_.push_back(low);
#ifdef CHARACTER_STATE_DEBUG
        log_state("push", value);
#endif
    }

    bool stack_empty() const {
        return stack_.empty();
    }

    std::size_t stack_size() const {
        return stack_.size();
    }

    long long stack_top() const {
        if (stack_.empty()) {
            throw std::runtime_error("Tried to pop from an empty stack.");
        }
        return decode(stack_.back());
    }

    long long pop() {
        if (stack_.empty()) {
            throw std::runtime_error("Tried to pop from an empty stack.");
        }
        const long long popped = decode(stack_.back());
        stack_.pop_back();
        value_ = popped;
#ifdef CHARACTER_STATE_DEBUG
        log_state("pop", popped);
#endif
        return popped;
    }

    const void* stack_pointer_value() const {
        if (stack_.empty()) {
            return nullptr;
        }
        return static_cast<const void*>(stack_.data());
    }

    const std::vector<ValueType>& raw_stack() const {
        return stack_;
    }

    const void* reference_pointer() const {
        if (reference_source_ != nullptr) {
            return reference_source_->stack_pointer_value();
        }
        return reference_;
    }

    std::string reference_stack_cstring() const {
        if (reference_source_ == nullptr) {
            throw std::runtime_error("No referenced stack available for cstring substitution");
        }
        return reference_source_->stack_cstring();
    }

private:
    std::string stack_cstring() const {
        std::string result;
        result.reserve(stack_.size());
        for (std::size_t i = 0; i < stack_.size(); ++i) {
            std::size_t idx = stack_.size() - 1 - i;
            const long long code = decode(stack_[idx]);
            if (code == 0) {
                break;
            }
            if (code < 0 || code > 255) {
                throw std::runtime_error(
                    "Cannot build cstring from stack value outside byte range: " + std::to_string(code)
                );
            }
            result.push_back(static_cast<char>(static_cast<unsigned char>(code)));
        }
        return result;
    }

    static long long pointer_to_long_long(const void* pointer) {
        const auto pointer_value = reinterpret_cast<std::uintptr_t>(pointer);
        if (pointer_value > static_cast<std::uintptr_t>(std::numeric_limits<long long>::max())) {
            throw std::runtime_error("Stack pointer is not representable as a signed 64-bit integer");
        }
        return static_cast<long long>(pointer_value);
    }

#ifdef CHARACTER_STATE_DEBUG
    void log_state(const char* operation, long long operand) const {
        std::ostringstream reference_hex;
        const void* live_reference = reference_pointer();
        reference_hex << std::hex << std::showbase
                      << reinterpret_cast<std::uintptr_t>(live_reference);

        std::string stack_ascii;
        for (std::size_t i = 0; i < stack_.size(); ++i) {
            std::size_t idx = stack_.size() - 1 - i;
            const long long code = decode(stack_[idx]);
            if (code < 0 || code > 127) {
                stack_ascii.push_back('.');
                continue;
            }

            const unsigned char ch = static_cast<unsigned char>(code);
            if (ch == '\n') {
                stack_ascii += "\\n";
            } else if (ch == '\t') {
                stack_ascii += "\\t";
            } else if (ch == '\r') {
                stack_ascii += "\\r";
            } else if (std::isprint(ch) != 0) {
                stack_ascii.push_back(static_cast<char>(ch));
            } else {
                stack_ascii.push_back('.');
            }
        }

        std::cerr << "[character-state-debug] op=" << operation
                  << " operand=" << operand
                  << " value=" << value()
                  << " reference=" << reference_hex.str()
                  << " stack(top->bottom)=[";
        for (std::size_t i = 0; i < stack_.size(); ++i) {
            if (i > 0) {
                std::cerr << " ";
            }
            std::size_t idx = stack_.size() - 1 - i;
            std::cerr << decode(stack_[idx]);
        }
        std::cerr << "] raw(u32 top->bottom)=[";
        for (std::size_t i = 0; i < stack_.size(); ++i) {
            if (i > 0) {
                std::cerr << " ";
            }
            std::size_t idx = stack_.size() - 1 - i;
            std::cerr << stack_[idx];
        }
        std::cerr << "]"
                  << " stack_ascii(top->bottom)=\"" << stack_ascii << "\"\n";
    }
#endif

    long long value_ = 0;
    std::vector<ValueType> stack_;

    RuntimeCharacter* reference_source_ = nullptr;
    const void* reference_ = nullptr;
};

class RuntimeScene {
public:
    RuntimeScene(std::string number, std::size_t operation_index)
        : number_(std::move(number)), operation_index_(operation_index) {}

    const std::string& number() const {
        return number_;
    }

    std::size_t operation_index() const {
        return operation_index_;
    }

private:
    std::string number_;
    std::size_t operation_index_ = 0;
};

class RuntimeAct {
public:
    explicit RuntimeAct(std::string number) : number_(std::move(number)) {}

    const std::string& number() const {
        return number_;
    }

    const std::vector<RuntimeScene>& scenes() const {
        return scenes_;
    }

    void add_scene(const std::string& scene_number, std::size_t operation_index) {
        for (const RuntimeScene& scene : scenes_) {
            if (scene.number() == scene_number) {
                throw std::runtime_error("Scene numeral " + scene_number + " is not unique in " + number_);
            }
        }
        scenes_.emplace_back(scene_number, operation_index);
    }

    const RuntimeScene& scene_by_number(const std::string& scene_number) const {
        for (const RuntimeScene& scene : scenes_) {
            if (scene.number() == scene_number) {
                return scene;
            }
        }
        throw std::runtime_error("Scene " + scene_number + " does not exist.");
    }

private:
    std::string number_;
    std::vector<RuntimeScene> scenes_;
};

class RuntimePlay {
public:
    RuntimeAct& add_act(const std::string& act_number) {
        for (const RuntimeAct& act : acts_) {
            if (act.number() == act_number) {
                throw std::runtime_error("Act numeral " + act_number + " is not unique");
            }
        }
        acts_.emplace_back(act_number);
        return acts_.back();
    }

    void add_operation(const Operation& operation) {
        operations_.push_back(operation);
    }

    const std::vector<Operation>& operations() const {
        return operations_;
    }

    std::size_t operation_count() const {
        return operations_.size();
    }

    const RuntimeAct& act_by_number(const std::string& act_number) const {
        for (const RuntimeAct& act : acts_) {
            if (act.number() == act_number) {
                return act;
            }
        }
        throw std::runtime_error("Act " + act_number + " does not exist.");
    }

    std::string current_act_number(std::size_t current_position) const {
        if (acts_.empty()) {
            throw std::runtime_error("No acts in play");
        }

        std::size_t act_index = 0;
        for (std::size_t i = 0; i + 1 < acts_.size(); ++i) {
            const RuntimeAct& next_act = acts_[i + 1];
            if (next_act.scenes().empty()) {
                continue;
            }
            if (next_act.scenes().front().operation_index() <= current_position) {
                act_index = i + 1;
            }
        }
        return acts_[act_index].number();
    }

private:
    std::vector<Operation> operations_;
    std::vector<RuntimeAct> acts_;
};

struct RuntimeState {
    bool global_boolean = false;
    std::unordered_map<std::string, RuntimeCharacter> characters;
    std::vector<std::string> on_stage;

    explicit RuntimeState(const std::vector<std::string>& personae) {
        for (const std::string& person : personae) {
            std::string normalized = normalize_name(person);
            if (characters.count(normalized) > 0) {
                throw std::runtime_error(normalized + " already initialized");
            }
            characters.emplace(normalized, RuntimeCharacter{});
        }
    }

    bool is_on_stage(const std::string& name) const {
        return std::find(on_stage.begin(), on_stage.end(), name) != on_stage.end();
    }

    RuntimeCharacter& character_by_name(const std::string& name) {
        auto it = characters.find(name);
        if (it == characters.end()) {
            throw std::runtime_error(name + " was not initialized!");
        }
        return it->second;
    }

    void assert_character_on_stage(const std::string& name) {
        if (!is_on_stage(name)) {
            if (characters.count(name) == 0) {
                throw std::runtime_error(name + " was not initialized!");
            }
            throw std::runtime_error(name + " is not on stage!");
        }
    }

    void assert_character_off_stage(const std::string& name) {
        if (is_on_stage(name)) {
            throw std::runtime_error(name + " is already on stage!");
        }
        if (characters.count(name) == 0) {
            throw std::runtime_error(name + " was not initialized!");
        }
    }

    void enter_characters(const std::vector<std::string>& names) {
        for (const std::string& name : names) {
            assert_character_off_stage(name);
        }
        for (const std::string& name : names) {
            on_stage.push_back(name);
        }
    }

    void exit_character(const std::string& name) {
        assert_character_on_stage(name);
        auto it = std::find(on_stage.begin(), on_stage.end(), name);
        on_stage.erase(it);
    }

    void exeunt_characters(const std::vector<std::string>& names) {
        for (const std::string& name : names) {
            assert_character_on_stage(name);
        }
        for (const std::string& name : names) {
            exit_character(name);
        }
    }

    void exeunt_all() {
        on_stage.clear();
    }

    std::string character_opposite(const std::string& speaker) {
        if (!is_on_stage(speaker)) {
            throw std::runtime_error(speaker + " is not on stage!");
        }
        if (on_stage.size() > 2) {
            throw std::runtime_error("Ambiguous second-person pronoun");
        }
        if (on_stage.size() < 2) {
            throw std::runtime_error(speaker + " is talking to nobody!");
        }
        if (on_stage[0] == speaker) {
            return on_stage[1];
        }
        return on_stage[0];
    }

    std::string to_debug_string() const {
        std::ostringstream out;
        out << "global boolean = " << (global_boolean ? "True" : "False") << "\n";
        out << "on stage:\n";
        for (const std::string& name : on_stage) {
            auto it = characters.find(name);
            out << "  " << name << " = " << it->second.value() << " (";
            const auto& stack = it->second.raw_stack();
            for (std::size_t i = 0; i < stack.size(); ++i) {
                if (i > 0) {
                    out << " ";
                }
                std::size_t idx = stack.size() - 1 - i;
                out << RuntimeCharacter::decode(stack[idx]);
            }
            out << ")\n";
        }
        out << "off stage:\n";
        for (const auto& pair : characters) {
            if (!is_on_stage(pair.first)) {
                out << "  " << pair.first << " = " << pair.second.value() << " (";
                const auto& stack = pair.second.raw_stack();
                for (std::size_t i = 0; i < stack.size(); ++i) {
                    if (i > 0) {
                        out << " ";
                    }
                    std::size_t idx = stack.size() - 1 - i;
                    out << RuntimeCharacter::decode(stack[idx]);
                }
                out << ")\n";
            }
        }
        return out.str();
    }
};

struct ParseResult {
    bool ok = false;
    std::shared_ptr<Expr> expr;
    std::size_t next = 0;
};

struct ComparativeParse {
    bool ok = false;
    CompareType type = CompareType::EQUAL;
    std::size_t next = 0;
};

class SentenceCompiler {
public:
    explicit SentenceCompiler(const std::vector<std::string>& declared_names) {
        for (const std::string& name : declared_names) {
            std::vector<std::string> parts = split_words(to_lower(normalize_name(name)));
            names_.push_back({parts, normalize_name(name)});
        }
        std::sort(names_.begin(), names_.end(), [](const auto& a, const auto& b) {
            return a.first.size() > b.first.size();
        });
    }

    Operation compile(const std::string& speaker_raw, const RawSentence& sentence) {
        std::string speaker = normalize_name(speaker_raw);

        std::vector<std::string> tokens;
        for (const std::string& token : sentence.tokens) {
            tokens.push_back(to_lower(token));
        }

        std::optional<bool> condition;
        std::size_t position = 0;
        if (tokens.size() >= 3 && tokens[0] == "if" && tokens[1] == "so" && tokens[2] == ",") {
            condition = true;
            position = 3;
        } else if (tokens.size() >= 3 && tokens[0] == "if" && tokens[1] == "not" && tokens[2] == ",") {
            condition = false;
            position = 3;
        }

        std::vector<std::string> body(tokens.begin() + static_cast<long long>(position), tokens.end());
        if (body.empty()) {
            throw std::runtime_error("Could not parse sentence with empty body");
        }

        Operation operation;
        operation.speaker = speaker;
        operation.condition_positive = condition;

        if (sentence.ending == '?') {
            compile_question(body, operation);
            return operation;
        }

        if (compile_goto(body, operation)) {
            return operation;
        }
        if (compile_output(body, operation)) {
            return operation;
        }
        if (compile_input(body, operation)) {
            return operation;
        }
        if (compile_push(body, operation)) {
            return operation;
        }
        if (compile_pop(body, operation)) {
            return operation;
        }
        if (compile_reference(body, operation)) {
            return operation;
        }
        if (compile_syscall(body, operation)) {
            return operation;
        }
        if (compile_assignment(body, operation)) {
            return operation;
        }

        throw std::runtime_error("Could not parse sentence: " + join_words(body));
    }

private:
    std::vector<std::pair<std::vector<std::string>, std::string>> names_;

    static bool match_seq(const std::vector<std::string>& tokens, std::size_t pos, const std::vector<std::string>& seq) {
        if (pos + seq.size() > tokens.size()) {
            return false;
        }
        for (std::size_t i = 0; i < seq.size(); ++i) {
            if (tokens[pos + i] != seq[i]) {
                return false;
            }
        }
        return true;
    }

    ParseResult parse_character_name(const std::vector<std::string>& tokens, std::size_t pos) {
        for (const auto& item : names_) {
            const std::vector<std::string>& parts = item.first;
            if (pos + parts.size() > tokens.size()) {
                continue;
            }

            bool matches = true;
            for (std::size_t i = 0; i < parts.size(); ++i) {
                if (tokens[pos + i] != parts[i]) {
                    matches = false;
                    break;
                }
            }
            if (!matches) {
                continue;
            }

            auto expr = std::make_shared<Expr>();
            expr->kind = Expr::Kind::CHARACTER;
            expr->text = item.second;
            return {true, expr, pos + parts.size()};
        }

        return {};
    }

    ParseResult parse_noun_phrase(const std::vector<std::string>& tokens, std::size_t pos) {
        std::size_t i = pos;
        if (i < tokens.size() && (is_article(tokens[i]) || is_possessive(tokens[i]))) {
            ++i;
        }

        std::size_t adjective_count = 0;
        bool has_positive_adjective = false;
        bool has_negative_adjective = false;

        while (i < tokens.size() && is_any_adjective(tokens[i])) {
            if (is_positive_adjective(tokens[i])) {
                has_positive_adjective = true;
            }
            if (is_negative_adjective(tokens[i])) {
                has_negative_adjective = true;
            }
            ++adjective_count;
            ++i;
        }

        if (i >= tokens.size()) {
            return {};
        }

        std::string noun;
        std::size_t noun_words = 1;
        if (i + 1 < tokens.size()) {
            std::string two_word = tokens[i] + " " + tokens[i + 1];
            if (is_negative_noun_phrase(two_word) || is_positive_or_neutral_noun_phrase(two_word)) {
                noun = two_word;
                noun_words = 2;
            }
        }
        if (noun.empty()) {
            noun = tokens[i];
        }

        bool noun_is_negative = is_negative_noun_phrase(noun);
        bool noun_is_positive_or_neutral = is_positive_or_neutral_noun_phrase(noun);

        if (!noun_is_negative && !noun_is_positive_or_neutral) {
            return {};
        }

        if (noun_is_negative && has_positive_adjective) {
            return {};
        }

        if (noun_is_positive_or_neutral && has_negative_adjective) {
            return {};
        }

        auto expr = std::make_shared<Expr>();
        expr->kind = Expr::Kind::LITERAL;
        long long magnitude = power_of_two(adjective_count);
        expr->literal = noun_is_negative ? -magnitude : magnitude;

        return {true, expr, i + noun_words};
    }

    ParseResult parse_expression(const std::vector<std::string>& tokens, std::size_t pos) {
        static const std::vector<std::pair<std::vector<std::string>, std::string>> binary_operations = {
            {{"the", "remainder", "of", "the", "quotient", "between"}, "remainder"},
            {{"the", "difference", "between"}, "difference"},
            {{"the", "product", "of"}, "product"},
            {{"the", "quotient", "between"}, "quotient"},
            {{"the", "sum", "of"}, "sum"},
        };

        for (const auto& item : binary_operations) {
            const std::vector<std::string>& prefix = item.first;
            if (!match_seq(tokens, pos, prefix)) {
                continue;
            }

            ParseResult first = parse_value(tokens, pos + prefix.size());
            if (!first.ok || first.next >= tokens.size() || tokens[first.next] != "and") {
                continue;
            }

            ParseResult second = parse_value(tokens, first.next + 1);
            if (!second.ok) {
                continue;
            }

            auto expr = std::make_shared<Expr>();
            expr->kind = Expr::Kind::BINARY;
            expr->text = item.second;
            expr->lhs = first.expr;
            expr->rhs = second.expr;
            return {true, expr, second.next};
        }

        static const std::vector<std::pair<std::vector<std::string>, std::string>> unary_operations = {
            {{"the", "square", "root", "of"}, "sqrt"},
            {{"the", "factorial", "of"}, "factorial"},
            {{"the", "square", "of"}, "square"},
            {{"the", "cube", "of"}, "cube"},
            {{"twice"}, "twice"},
        };

        for (const auto& item : unary_operations) {
            const std::vector<std::string>& prefix = item.first;
            if (!match_seq(tokens, pos, prefix)) {
                continue;
            }

            ParseResult operand = parse_value(tokens, pos + prefix.size());
            if (!operand.ok) {
                continue;
            }

            auto expr = std::make_shared<Expr>();
            expr->kind = Expr::Kind::UNARY;
            expr->text = item.second;
            expr->lhs = operand.expr;
            return {true, expr, operand.next};
        }

        return {};
    }

    ParseResult parse_value(const std::vector<std::string>& tokens, std::size_t pos) {
        if (pos >= tokens.size()) {
            return {};
        }

        ParseResult expression = parse_expression(tokens, pos);
        if (expression.ok) {
            return expression;
        }

        if (is_first_person(tokens[pos])) {
            auto expr = std::make_shared<Expr>();
            expr->kind = Expr::Kind::FIRST_PERSON;
            return {true, expr, pos + 1};
        }

        if (is_second_person(tokens[pos])) {
            auto expr = std::make_shared<Expr>();
            expr->kind = Expr::Kind::SECOND_PERSON;
            return {true, expr, pos + 1};
        }

        if (tokens[pos] == "nothing" || tokens[pos] == "zero") {
            auto expr = std::make_shared<Expr>();
            expr->kind = Expr::Kind::LITERAL;
            expr->literal = 0;
            return {true, expr, pos + 1};
        }

        ParseResult noun_phrase = parse_noun_phrase(tokens, pos);
        if (noun_phrase.ok) {
            return noun_phrase;
        }

        ParseResult character_name = parse_character_name(tokens, pos);
        if (character_name.ok) {
            return character_name;
        }

        return {};
    }

    ComparativeParse parse_comparative(const std::vector<std::string>& tokens, std::size_t pos) {
        if (pos >= tokens.size()) {
            return {};
        }

        static const std::unordered_set<std::string> positive_comparatives = {
            "better", "bigger", "fresher", "friendlier", "nicer", "jollier"
        };
        static const std::unordered_set<std::string> negative_comparatives = {
            "punier", "smaller", "worse"
        };

        if (positive_comparatives.count(tokens[pos]) > 0 && pos + 1 < tokens.size() && tokens[pos + 1] == "than") {
            return {true, CompareType::GREATER, pos + 2};
        }

        if (negative_comparatives.count(tokens[pos]) > 0 && pos + 1 < tokens.size() && tokens[pos + 1] == "than") {
            return {true, CompareType::LESS, pos + 2};
        }

        if (tokens[pos] == "more" && pos + 2 < tokens.size() && tokens[pos + 2] == "than") {
            if (is_positive_adjective(tokens[pos + 1])) {
                return {true, CompareType::GREATER, pos + 3};
            }
            if (is_negative_adjective(tokens[pos + 1])) {
                return {true, CompareType::LESS, pos + 3};
            }
        }

        if (tokens[pos] == "as" && pos + 2 < tokens.size() && tokens[pos + 2] == "as") {
            const std::string& adjective = tokens[pos + 1];
            if (is_negative_adjective(adjective) || is_positive_adjective(adjective) || is_neutral_adjective(adjective)) {
                return {true, CompareType::EQUAL, pos + 3};
            }
        }

        return {};
    }

    void compile_question(const std::vector<std::string>& body, Operation& operation) {
        if (body.empty() || !is_be_word(body[0])) {
            throw std::runtime_error("Question must start with a be-verb");
        }

        ParseResult first = parse_value(body, 1);
        if (!first.ok) {
            throw std::runtime_error("Could not parse first question value");
        }

        ComparativeParse comparative = parse_comparative(body, first.next);
        if (!comparative.ok) {
            throw std::runtime_error("Could not parse question comparative");
        }

        ParseResult second = parse_value(body, comparative.next);
        if (!second.ok || second.next != body.size()) {
            throw std::runtime_error("Could not parse second question value");
        }

        operation.type = Operation::Type::QUESTION;
        operation.expr_a = first.expr;
        operation.expr_b = second.expr;
        operation.compare = comparative.type;
    }

    bool compile_assignment(const std::vector<std::string>& body, Operation& operation) {
        if (body.empty() || !is_second_person(body[0])) {
            return false;
        }

        std::size_t pos = 1;
        if (pos < body.size() && is_be_word(body[pos])) {
            ++pos;
        }

        if (pos < body.size() && body[pos] == "as") {
            ++pos;
            if (pos + 1 < body.size() && is_any_adjective(body[pos]) && body[pos + 1] == "as") {
                pos += 2;
            }
        }

        ParseResult value = parse_value(body, pos);
        if (!value.ok || value.next != body.size()) {
            return false;
        }

        operation.type = Operation::Type::ASSIGN;
        operation.expr_a = value.expr;
        return true;
    }

    bool compile_goto(const std::vector<std::string>& body, Operation& operation) {
        std::size_t pos = 0;

        bool matched_let_us = false;
        if (match_seq(body, pos, {"let", "us"})) {
            pos += 2;
            matched_let_us = true;
        } else if (match_seq(body, pos, {"we", "shall"}) || match_seq(body, pos, {"we", "must"})) {
            pos += 2;
            matched_let_us = true;
        }

        if (!matched_let_us) {
            return false;
        }

        if (match_seq(body, pos, {"proceed", "to"}) || match_seq(body, pos, {"return", "to"})) {
            pos += 2;
        } else {
            return false;
        }

        if (pos >= body.size() || body[pos] != "scene") {
            return false;
        }
        ++pos;

        if (pos >= body.size()) {
            return false;
        }

        std::string destination = normalize_roman(body[pos]);
        ++pos;

        if (pos != body.size()) {
            return false;
        }

        operation.type = Operation::Type::GOTO;
        operation.destination_scene = destination;
        return true;
    }

    bool compile_output(const std::vector<std::string>& body, Operation& operation) {
        if (body.size() == 3 && body[0] == "open" && is_second_person_possessive(body[1]) && body[2] == "heart") {
            operation.type = Operation::Type::OUTPUT;
            operation.number_io = true;
            return true;
        }
        if (body.size() == 3 && body[0] == "speak" && is_second_person_possessive(body[1]) && body[2] == "mind") {
            operation.type = Operation::Type::OUTPUT;
            operation.number_io = false;
            return true;
        }
        return false;
    }

    bool compile_input(const std::vector<std::string>& body, Operation& operation) {
        if (body.size() == 4 && body[0] == "listen" && body[1] == "to" && is_second_person_possessive(body[2]) && body[3] == "heart") {
            operation.type = Operation::Type::INPUT;
            operation.number_io = true;
            return true;
        }
        if (body.size() == 3 && body[0] == "open" && is_second_person_possessive(body[1]) && body[2] == "mind") {
            operation.type = Operation::Type::INPUT;
            operation.number_io = false;
            return true;
        }
        return false;
    }

    bool compile_push(const std::vector<std::string>& body, Operation& operation) {
        if (body.empty() || body[0] != "remember") {
            return false;
        }

        ParseResult value = parse_value(body, 1);
        if (!value.ok || value.next != body.size()) {
            return false;
        }

        operation.type = Operation::Type::PUSH;
        operation.expr_a = value.expr;
        return true;
    }

    bool compile_pop(const std::vector<std::string>& body, Operation& operation) {
        if (body.empty() || body[0] != "recall") {
            return false;
        }
        operation.type = Operation::Type::POP;
        return true;
    }

    bool compile_reference(const std::vector<std::string>& body, Operation& operation) {
        if (body.empty() || body[0] != "reference") {
            return false;
        }

        ParseResult target = parse_character_name(body, 1);
        if (!target.ok || target.next != body.size()) {
            return false;
        }

        operation.type = Operation::Type::REFERENCE;
        operation.reference_character = target.expr->text;
        return true;
    }

    bool compile_syscall(const std::vector<std::string>& body, Operation& operation) {
        if (body.size() < 4) {
            return false;
        }
        if (!match_seq(body, 0, {"revere", "your", "player"})) {
            return false;
        }

        ParseResult target = parse_character_name(body, 3);
        if (!target.ok || target.next != body.size()) {
            return false;
        }

        operation.type = Operation::Type::SYSCALL;
        operation.syscall_character = target.expr->text;
        return true;
    }
};

long long factorial(long long value) {
    if (value < 0) {
        throw std::runtime_error("Cannot take the factorial of a negative number: " + std::to_string(value));
    }

    long long result = 1;
    for (long long i = 2; i <= value; ++i) {
        if (result > std::numeric_limits<long long>::max() / i) {
            throw std::runtime_error("Numeric overflow in factorial");
        }
        result *= i;
    }
    return result;
}

}  // namespace

struct ShakespeareInterpreter::Impl {
    RuntimePlay runtime_play;

    RuntimeState state;
    std::string input_style;
    std::string output_style;
    std::size_t current_position = 0;
    bool jumped_this_step = false;

    std::string basic_input_buffer;

    explicit Impl(const Play& play, const std::string& input_style_in, const std::string& output_style_in)
        : state(build_personae(play)), input_style(input_style_in), output_style(output_style_in) {
#if SPL_ENABLE_IO_STYLES
        if (input_style != "basic" && input_style != "interactive") {
            throw std::runtime_error("Unknown input style: " + input_style);
        }
        if (output_style != "basic" && output_style != "verbose" && output_style != "debug") {
            throw std::runtime_error("Unknown output style: " + output_style);
        }
#else
        if (input_style != "basic") {
            throw std::runtime_error("Unknown input style: " + input_style);
        }
        if (output_style != "basic") {
            throw std::runtime_error("Unknown output style: " + output_style);
        }
#endif

        compile_play(play);
    }

    static std::vector<std::string> build_personae(const Play& play) {
        std::vector<std::string> result;
        for (const std::string& name : play.dramatis_personae) {
            result.push_back(normalize_name(name));
        }
        return result;
    }

    bool is_verbose() const {
#if SPL_ENABLE_IO_STYLES
        return output_style == "verbose" || output_style == "debug";
#else
        return false;
#endif
    }

    bool is_debug() const {
#if SPL_ENABLE_IO_STYLES
        return output_style == "debug";
#else
        return false;
#endif
    }

    void compile_play(const Play& play) {
        SentenceCompiler compiler(play.dramatis_personae);

        for (const Act& act : play.acts) {
            std::string act_number = normalize_roman(act.number);
            RuntimeAct& runtime_act = runtime_play.add_act(act_number);

            for (const Scene& scene : act.scenes) {
                std::string scene_number = normalize_roman(scene.number);
                runtime_act.add_scene(scene_number, runtime_play.operation_count());

                for (const Event& event : scene.events) {
                    if (event.type == Event::Type::LINE) {
                        std::string speaker = normalize_name(event.speaker);
                        for (const RawSentence& sentence : event.sentences) {
                            runtime_play.add_operation(compiler.compile(speaker, sentence));
                        }
                        continue;
                    }

                    Operation operation;
                    if (event.type == Event::Type::ENTRANCE) {
                        operation.type = Operation::Type::ENTER;
                        for (const std::string& character : event.characters) {
                            operation.characters.push_back(normalize_name(character));
                        }
                    } else if (event.type == Event::Type::EXIT) {
                        operation.type = Operation::Type::EXIT;
                        for (const std::string& character : event.characters) {
                            operation.characters.push_back(normalize_name(character));
                        }
                    } else if (event.type == Event::Type::EXEUNT) {
                        operation.type = Operation::Type::EXEUNT;
                        for (const std::string& character : event.characters) {
                            operation.characters.push_back(normalize_name(character));
                        }
                    } else if (event.type == Event::Type::BREAKPOINT) {
                        operation.type = Operation::Type::BREAKPOINT;
                    }
                    runtime_play.add_operation(operation);
                }
            }
        }
    }

    std::string current_act() const {
        return runtime_play.current_act_number(current_position);
    }

    long long consume_numeric_input() {
#if SPL_ENABLE_IO_STYLES
        if (input_style == "interactive") {
            std::cout << "Taking input number: ";
            std::string line;
            if (!std::getline(std::cin, line)) {
                throw std::runtime_error("End of file encountered.");
            }
            try {
                return std::stoll(line);
            } catch (const std::exception&) {
                throw std::runtime_error("No numeric input was given.");
            }
        }
#endif

        while (basic_input_buffer.empty()) {
            std::string line;
            if (!std::getline(std::cin, line)) {
                throw std::runtime_error("End of file encountered.");
            }
            basic_input_buffer = line + "\n";
        }

        std::string number;
        while (!basic_input_buffer.empty() && std::isdigit(static_cast<unsigned char>(basic_input_buffer[0])) != 0) {
            number.push_back(basic_input_buffer[0]);
            basic_input_buffer.erase(0, 1);
        }

        if (number.empty()) {
            throw std::runtime_error("No numeric input was given.");
        }

        if (!basic_input_buffer.empty() && basic_input_buffer[0] == '\n') {
            basic_input_buffer.erase(0, 1);
        }

        return std::stoll(number);
    }

    long long consume_character_input() {
#if SPL_ENABLE_IO_STYLES
        if (input_style == "interactive") {
            std::cout << "Taking input character: ";
            std::string line;
            if (!std::getline(std::cin, line)) {
                return -1;
            }
            if (line == "EOF") {
                return -1;
            }
            if (line.empty()) {
                return static_cast<long long>('\n');
            }
            return static_cast<long long>(line[0]);
        }
#endif

        while (basic_input_buffer.empty()) {
            std::string line;
            if (!std::getline(std::cin, line)) {
                return -1;
            }
            basic_input_buffer = line + "\n";
        }

        long long result = static_cast<unsigned char>(basic_input_buffer[0]);
        basic_input_buffer.erase(0, 1);
        return result;
    }

    void output_number(long long value) {
#if SPL_ENABLE_IO_STYLES
        if (output_style == "basic") {
            std::cout << value;
            return;
        }
        std::cout << "Outputting number: " << value << "\n";
#else
        std::cout << value;
#endif
    }

    void output_character(long long value) {
        std::string character = printable_char(value);
#if SPL_ENABLE_IO_STYLES
        if (output_style == "basic") {
            std::cout << character;
            return;
        }

        std::ostringstream quoted;
        quoted << "'";
        if (character[0] == '\n') {
            quoted << "\\n";
        } else if (character[0] == '\t') {
            quoted << "\\t";
        } else {
            quoted << character;
        }
        quoted << "'";
        std::cout << "Outputting character: " << quoted.str() << "\n";
    #else
        std::cout << character;
    #endif
    }

    long long evaluate_expression(const std::shared_ptr<Expr>& expression, const std::string& speaker) {
        if (!expression) {
            throw std::runtime_error("Attempted to evaluate an empty expression");
        }

        switch (expression->kind) {
            case Expr::Kind::LITERAL:
                return expression->literal;
            case Expr::Kind::FIRST_PERSON:
                state.assert_character_on_stage(speaker);
                return state.character_by_name(speaker).value();
            case Expr::Kind::SECOND_PERSON: {
                state.assert_character_on_stage(speaker);
                std::string opposite = state.character_opposite(speaker);
                return state.character_by_name(opposite).value();
            }
            case Expr::Kind::CHARACTER:
                return state.character_by_name(expression->text).value();
            case Expr::Kind::UNARY: {
                long long operand = evaluate_expression(expression->lhs, speaker);
                if (expression->text == "cube") {
                    return operand * operand * operand;
                }
                if (expression->text == "factorial") {
                    return factorial(operand);
                }
                if (expression->text == "square") {
                    return operand * operand;
                }
                if (expression->text == "sqrt") {
                    if (operand < 0) {
                        throw std::runtime_error("Cannot take the square root of a negative number: " + std::to_string(operand));
                    }
                    return static_cast<long long>(std::sqrt(static_cast<double>(operand)));
                }
                if (expression->text == "twice") {
                    return operand * 2;
                }
                throw std::runtime_error("Unknown unary operator: " + expression->text);
            }
            case Expr::Kind::BINARY: {
                long long lhs = evaluate_expression(expression->lhs, speaker);
                long long rhs = evaluate_expression(expression->rhs, speaker);
                if (expression->text == "difference") {
                    return lhs - rhs;
                }
                if (expression->text == "product") {
                    return lhs * rhs;
                }
                if (expression->text == "quotient") {
                    if (rhs == 0) {
                        throw std::runtime_error("Cannot divide by zero");
                    }
                    return static_cast<long long>(lhs / rhs);
                }
                if (expression->text == "remainder") {
                    if (rhs == 0) {
                        throw std::runtime_error("Cannot divide by zero");
                    }
                    return lhs % rhs;
                }
                if (expression->text == "sum") {
                    return lhs + rhs;
                }
                throw std::runtime_error("Unknown binary operator: " + expression->text);
            }
        }

        throw std::runtime_error("Unknown expression type");
    }

    bool should_skip(const Operation& operation) const {
        if (!operation.condition_positive.has_value()) {
            return false;
        }
        return operation.condition_positive.value() != state.global_boolean;
    }

    void execute_operation(const Operation& operation) {
        switch (operation.type) {
            case Operation::Type::ENTER:
                if (is_verbose()) {
                    std::cout << "Enter " << join_words(operation.characters, ", ") << "\n";
                }
                state.enter_characters(operation.characters);
                return;
            case Operation::Type::EXIT:
                if (is_verbose()) {
                    std::cout << "Exit " << operation.characters.front() << "\n";
                }
                state.exit_character(operation.characters.front());
                return;
            case Operation::Type::EXEUNT:
                if (operation.characters.empty()) {
                    if (is_verbose()) {
                        std::cout << "Exeunt all\n";
                    }
                    state.exeunt_all();
                } else {
                    if (is_verbose()) {
                        std::cout << "Exeunt " << join_words(operation.characters, ", ") << "\n";
                    }
                    state.exeunt_characters(operation.characters);
                }
                return;
            case Operation::Type::BREAKPOINT:
                if (is_verbose()) {
                    std::cout << "Breakpoint\n";
                }
                return;
            default:
                break;
        }

        state.assert_character_on_stage(operation.speaker);

        if (should_skip(operation)) {
            if (is_verbose()) {
                std::cout << "Not executing conditional operation, global boolean is "
                          << (state.global_boolean ? "True" : "False") << "\n";
            }
            return;
        }

        if (operation.type == Operation::Type::QUESTION) {
            long long first = evaluate_expression(operation.expr_a, operation.speaker);
            long long second = evaluate_expression(operation.expr_b, operation.speaker);
            bool result = false;
            if (operation.compare == CompareType::GREATER) {
                result = first > second;
            } else if (operation.compare == CompareType::LESS) {
                result = first < second;
            } else {
                result = first == second;
            }
            state.global_boolean = result;
            if (is_verbose()) {
                std::cout << "Setting global boolean to " << (result ? "True" : "False") << "\n";
            }
            return;
        }

        if (operation.type == Operation::Type::ASSIGN) {
            std::string opposite = state.character_opposite(operation.speaker);
            long long value = evaluate_expression(operation.expr_a, operation.speaker);
            state.character_by_name(opposite).set_value(value);
            if (is_verbose()) {
                std::cout << opposite << " set to " << value << "\n";
            }
            return;
        }

        if (operation.type == Operation::Type::INPUT) {
            std::string opposite = state.character_opposite(operation.speaker);
            long long value = operation.number_io ? consume_numeric_input() : consume_character_input();
            state.character_by_name(opposite).set_value(value);
            if (is_verbose()) {
                std::cout << "Setting " << opposite << " to input value " << value << "\n";
            }
            return;
        }

        if (operation.type == Operation::Type::OUTPUT) {
            std::string opposite = state.character_opposite(operation.speaker);
            long long value = state.character_by_name(opposite).value();
            if (is_verbose()) {
                std::cout << "Outputting " << opposite << "\n";
            }
            if (operation.number_io) {
                output_number(value);
            } else {
                output_character(value);
            }
            return;
        }

        if (operation.type == Operation::Type::PUSH) {
            std::string opposite = state.character_opposite(operation.speaker);
            long long value = evaluate_expression(operation.expr_a, operation.speaker);
            state.character_by_name(opposite).push(value);
            if (is_verbose()) {
                std::cout << opposite << " pushed " << value << "\n";
            }
            return;
        }

        if (operation.type == Operation::Type::POP) {
            std::string opposite = state.character_opposite(operation.speaker);
            RuntimeCharacter& character = state.character_by_name(opposite);
            character.pop();
            if (is_verbose()) {
                std::cout << "Popping stack of " << opposite << "\n";
            }
            return;
        }

        if (operation.type == Operation::Type::REFERENCE) {
            std::string opposite = state.character_opposite(operation.speaker);
            RuntimeCharacter& target = state.character_by_name(opposite);
            RuntimeCharacter& source = state.character_by_name(operation.reference_character);

            target.set_reference(&source);

            if (is_verbose()) {
                std::cout << opposite << " now references stack of "
                          << operation.reference_character << " at " << source.stack_pointer_value() << "\n";
            }
            return;
        }

        if (operation.type == Operation::Type::SYSCALL) {
            RuntimeCharacter& character = state.character_by_name(operation.syscall_character);
            const RuntimeCharacter& referenced_character = character;
            if (character.stack_empty()) {
                throw std::runtime_error("Tried to pop from an empty stack.");
            }

            const long long syscall_number = character.stack_top();
            std::optional<int> arg_count = syscall_argument_count(syscall_number);
            if (!arg_count.has_value()) {
                throw std::runtime_error(
                    "Unknown syscall number for argument count: " + std::to_string(syscall_number)
                );
            }

            if (character.stack_size() < static_cast<std::size_t>(1 + arg_count.value())) {
                throw std::runtime_error(
                    "Not enough values on stack for syscall " + std::to_string(syscall_number)
                );
            }

            character.pop();
            std::vector<long long> args;
            args.reserve(static_cast<std::size_t>(arg_count.value()));
            constexpr long long u32_max_sentinel = static_cast<long long>(std::numeric_limits<std::uint32_t>::max());
            std::optional<std::string> referenced_cstring;
            for (int i = 0; i < arg_count.value(); ++i) {
                long long argument = character.pop();
                if (argument == u32_max_sentinel) {
                    if (!referenced_cstring.has_value()) {
                        referenced_cstring = referenced_character.reference_stack_cstring();
                    }
                    argument = static_cast<long long>(
                        reinterpret_cast<std::uintptr_t>(referenced_cstring->c_str())
                    );
                }
                args.push_back(argument);
            }

#ifdef CHARACTER_STATE_DEBUG          
            std::cout << "Invoking syscall on " << operation.syscall_character
                      << " number " << syscall_number
                      << " with args [";
            for (std::size_t i = 0; i < args.size(); ++i) {
                if (i > 0) {
                    std::cout << ", ";
                }
                std::cout << args[i];
            }
            std::cout << "]\n";
#endif   

            errno = 0;
            const long long result = invoke_syscall(syscall_number, args);
            character.set_value(result);

            if (is_verbose()) {
                std::cout << "Syscall on " << operation.syscall_character
                          << " number " << syscall_number
                          << " returned " << result;
                if (result == -1 && errno != 0) {
                    std::cout << " (errno=" << errno << ")";
                }
                std::cout << "\n";
            }
            return;
        }

        if (operation.type == Operation::Type::GOTO) {
            if (is_verbose()) {
                std::cout << "Jumping to Scene " << operation.destination_scene << "\n";
            }
            std::string act = current_act();
            const RuntimeAct& runtime_act = runtime_play.act_by_number(act);
            current_position = runtime_act.scene_by_number(operation.destination_scene).operation_index();
            jumped_this_step = true;
            return;
        }

        throw std::runtime_error("Unknown operation type");
    }

    void run() {
        const std::vector<Operation>& operations = runtime_play.operations();
        while (current_position < operations.size()) {
            const Operation& operation = operations[current_position];

#if SPL_ENABLE_IO_STYLES
            if (is_debug()) {
                std::cout << "----------\n";
                std::cout << state.to_debug_string();
                std::cout << "----------\n";
            }
#endif

            std::size_t before = current_position;
            jumped_this_step = false;
            execute_operation(operation);
            if (!jumped_this_step && current_position == before) {
                ++current_position;
            }
        }

        std::cout.flush();
    }
};

ShakespeareInterpreter::ShakespeareInterpreter(
    const Play& play,
    const std::string& input_style,
    const std::string& output_style
) : play_(play), input_style_(input_style), output_style_(output_style), impl_(nullptr) {
    impl_ = new Impl(play_, input_style_, output_style_);
}

ShakespeareInterpreter::~ShakespeareInterpreter() {
    delete impl_;
}

void ShakespeareInterpreter::run() {
    impl_->run();
}
