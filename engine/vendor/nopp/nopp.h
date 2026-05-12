//
// Created by Nico Russo on 5/5/26.
//

#ifndef NOPP_NOPP_H
#define NOPP_NOPP_H

#include <filesystem>
#include <optional>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <span>

#ifdef _WIN32
#  include <windows.h>
#elif __APPLE__
#  include <mach-o/dyld.h>
#elif __FreeBSD__
#  include <sys/sysctl.h>
#else
#  include <unistd.h>
#endif

#define cur_file nopp::absolute(nopp::path(__FILE__))
#define cur_dir  cur_file.parent_path()

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

constexpr i8  min_i8  = static_cast<i8>(0x80);
constexpr i16 min_i16 = static_cast<i16>(0x8000);
constexpr i32 min_i32 = static_cast<i32>(0x80000000);
constexpr i64 min_i64 = static_cast<i64>(0x8000000000000000);

constexpr i8  max_i8  = 0x7F;
constexpr i16 max_i16 = 0x7FFF;
constexpr i32 max_i32 = 0x7FFFFFFF;
constexpr i64 max_i64 = 0x7FFFFFFFFFFFFFFF;

constexpr u8  max_u8  = 0xFF;
constexpr u16 max_u16 = 0xFFFF;
constexpr u32 max_u32 = 0xFFFFFFFF;
constexpr u64 max_u64 = 0xFFFFFFFFFFFFFFFF;

constexpr f32 machine_epsilon_f32  = 1.1920929e-7f;
constexpr f32 pi_f32               = 3.14159265359f;
constexpr f32 tau_f32              = 6.28318530718f;
constexpr f32 e_f32                = 2.71828182846f;
constexpr f32 gold_big_f32         = 1.61803398875f;
constexpr f32 gold_small_f32       = 0.61803398875f;

constexpr f64 machine_epsilon_f64  = 2.220446e-16;
constexpr f64 pi_f64               = 3.14159265359;
constexpr f64 tau_f64              = 6.28318530718;
constexpr f64 two_pi_f64           = 6.28318530718;
constexpr f64 e_f64                = 2.71828182846;
constexpr f64 gold_big_f64         = 1.61803398875;
constexpr f64 gold_small_f64       = 0.61803398875;

namespace nopp {
    using namespace std::filesystem;
}



/* ===================================================== */
/*                   LOGGING SYSTEM                      */
/* ===================================================== */
namespace nopp {
    enum LogLevel { FATAL, ERROR, WARN, INFO, DEBUG, TRACE };

    struct LogEvent {
        LogLevel level;
        std::string message;

        LogEvent(const LogLevel level, std::string message)
            : level(level), message(std::move(message)) {}
    };

    inline std::vector<LogEvent> log_handler;
    inline path log_path;

    constexpr auto ANSI_BLACK     =  "\033[30m";
    constexpr auto ANSI_RED       =  "\033[31m";
    constexpr auto ANSI_GREEN     =  "\033[32m";
    constexpr auto ANSI_YELLOW    =  "\033[33m";
    constexpr auto ANSI_BLUE      =  "\033[34m";
    constexpr auto ANSI_MAGENTA   =  "\033[35m";
    constexpr auto ANSI_CYAN      =  "\033[36m";
    constexpr auto ANSI_WHITE     =  "\033[37m";
    constexpr auto ANSI_GREY      =  "\033[90m";
    constexpr auto ANSI_BOLD      =  "\033[1m";
    constexpr auto ANSI_ITALIC    =  "\033[3m";
    constexpr auto ANSI_UNDERLINE =  "\033[4m";
    constexpr auto ANSI_RESET     =  "\033[0m";

    template<typename... Args>
    std::string stringPrint(const std::string& fmt, Args&&... args);

    template<typename... Args>
    void print(const std::string& fmt, Args&&... args);

    template<typename... Args>
    void print(std::ostream& os, const std::string& fmt, Args&&... args);

    template<typename... Args>
    void println(const std::string& fmt, Args&&... args);

    template<typename... Args>
    void println(std::ostream& os, const std::string& fmt, Args&&... args);

#define LOG_FATAL(fmt, ...) nopp::_impl_log_fatal(fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) nopp::_impl_log_error(fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#ifdef LOG_INCLUDE_LINE_INFO
#define LOG_WARN(fmt, ...)  nopp::_impl_log_warn(fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  nopp::_impl_log_info(fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) nopp::_impl_log_debug(fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) nopp::_impl_log_trace(fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define LOG_WARN(fmt, ...)  nopp::_impl_log_warn(fmt, nullptr, 0, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  nopp::_impl_log_info(fmt, nullptr, 0, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) nopp::_impl_log_debug(fmt, nullptr, 0, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) nopp::_impl_log_trace(fmt, nullptr, 0, ##__VA_ARGS__)
#endif

#define LOG_ASSERT(x, ...) do { if (!(x)) { LOG_FATAL("Assertion failed: " #x); std::terminate(); } } while(0)
#define UNREACHABLE()      do { LOG_FATAL("UNREACHABLE: {}:{}", __FILE__, __LINE__); std::terminate(); } while(0)
#define TODO(fmt, ...) LOG_DEBUG(fmt, args...)

#ifdef LOG_STRIP
#define LOG_STRIP_FATAL
#define LOG_STRIP_ERROR
#define LOG_STRIP_WARN
#define LOG_STRIP_INFO
#define LOG_STRIP_DEBUG
#define LOG_STRIP_TRACE
#endif

#ifdef LOG_STRIP_FATAL
#define LOG_FATAL()
#endif
#ifdef LOG_STRIP_ERROR
#define LOG_ERROR()
#endif
#ifdef LOG_STRIP_WARN
#define LOG_WARN()
#endif
#ifdef LOG_STRIP_INFO
#define LOG_INFO()
#endif
#ifdef LOG_STRIP_DEBUG
#define LOG_DEBUG()
#endif
#ifdef LOG_STRIP_TRACE
#define LOG_TRACE()
#endif

}

/* ===================================================== */
/* LOGGING TEMPLATE IMPLEMENTATIONS AND HELPER FUNCTIONS */
/*    Just ignore this. It's here for dumb C++ reasons   */
/* ===================================================== */
namespace nopp {
    struct FormatSpec {
        char fill      = ' ';
        char align     = '\0';  // '<', '>', '^'
        char sign      = '\0';  // '+', '-', ' '
        int  width     = -1;
        int  precision = -1;
        char type      = '\0';  // 'f', 'e', 'g', 'd', 's', 'x', 'o', etc.
    };

    struct LogMeta {
        LogLevel level;
        const char* label;      // e.g. " [FATAL] "
        const char* ansi_color;
        const char* ansi_style; // "" if unused
        const char* ansi_post;  // codes after label (e.g. ANSI_ITALIC), or ""
    };

    template<typename T>
    void apply_spec(std::ostream& os, const T& value, const FormatSpec& fs);
    inline FormatSpec parse_spec(const std::string& spec);
    inline size_t emit_literal_until_placeholder(std::ostream& os, const std::string& fmt, size_t pos, std::string& spec_out);
    inline void fprint_impl(std::ostream& os, const std::string& fmt, size_t pos);
    template<typename T, typename... Args>
    void fprint_impl(std::ostream& os, const std::string& fmt, size_t pos, const T& value, Args&&... args);
    inline std::string fStringPrint(std::ostringstream& ss, const std::string& fmt, size_t pos);
    template<typename T, typename... Args>
    std::string fStringPrint(std::ostringstream& ss, const std::string& fmt, size_t pos, T& value, Args&&... args);

    template<typename... Args>
    void _impl_log(const LogMeta& meta, const std::string& fmt, const char* file, const int line, const Args&... args) {
        if (file)
            std::cerr << file << ":" << line;

        std::cerr << meta.ansi_color << meta.ansi_style << meta.label << meta.ansi_post;
        print(std::cerr, fmt, args...);
        std::cerr << ANSI_RESET << '\n';

        const std::string msg = stringPrint(fmt, args...);
        log_handler.emplace_back(meta.level, msg);

        if (!log_path.empty()) {
            std::ofstream os(log_path, std::ios_base::app);
            if (file)
                os << file << ":" << line;
            os << meta.label;
            print(os, fmt, args...);
            os << '\n';
        }
    }
    template<typename... Args>
    void _impl_log_fatal(const std::string& fmt, const char* file, const int line, const Args&... args) {
        _impl_log( {LogLevel::FATAL, " [FATAL] ", ANSI_RED, ANSI_BOLD, ""}, fmt, file, line, args...);
    }
    template<typename... Args>
    void _impl_log_error(const std::string& fmt, const char* file, const int line, const Args&... args) {
        _impl_log({LogLevel::ERROR, " [ERROR] ", ANSI_RED, "", ANSI_ITALIC}, fmt, file, line, args...);
    }
    template<typename... Args>
    void _impl_log_warn(const std::string& fmt, const char* file, const int line, const Args&... args) {
        _impl_log({LogLevel::WARN, "  [WARN] ", ANSI_MAGENTA, "", ""}, fmt, file, line, args...);
    }
    template<typename... Args>
    void _impl_log_info(const std::string& fmt, const char* file, const int line, const Args&... args) {
        _impl_log({LogLevel::INFO, "  [INFO] ", ANSI_GREEN, "", ""}, fmt, file, line, args...);
    }
    template<typename... Args>
    void _impl_log_debug(const std::string& fmt, const char* file, const int line, const Args&... args) {
        _impl_log({LogLevel::DEBUG, " [DEBUG] ", ANSI_CYAN, "", ANSI_ITALIC}, fmt, file, line, args...);
    }
    template<typename... Args>
    void _impl_log_trace(const std::string& fmt, const char* file, const int line, const Args&... args) {
        _impl_log({LogLevel::TRACE, " [TRACE] ", ANSI_GREY, "", ANSI_ITALIC}, fmt, file, line, args...);
    }
    template<typename T>
    void apply_spec(std::ostream& os, const T& value, const FormatSpec& fs) {
        // Capture into a string first so we can handle alignment on arbitrary types
        std::ostringstream tmp;

        if (fs.sign == '+') tmp << std::showpos;
        if (fs.precision >= 0) tmp << std::setprecision(fs.precision);

        switch (fs.type) {
            case 'f': tmp << std::fixed                           << value; break;
            case 'e': tmp << std::scientific                      << value; break;
            case 'E': tmp << std::scientific << std::uppercase    << value; break;
            case 'g': tmp << std::defaultfloat                    << value; break;
            case 'x': tmp << std::hex                             << value; break;
            case 'X': tmp << std::hex << std::uppercase           << value; break;
            case 'o': tmp << std::oct                             << value; break;
            case 'd': case 'i': tmp << std::dec                   << value; break;
            default:  tmp                                         << value; break;
        }

        std::string s = tmp.str();

        if (fs.width > 0 && static_cast<int>(s.size()) < fs.width) {
            const int pad = fs.width - static_cast<int>(s.size());
            const char fill = fs.fill ? fs.fill : ' ';
            switch (fs.align) {
                case '>': os << std::string(pad, fill) << s;                             break;
                case '^': os << std::string(pad/2, fill) << s << std::string(pad - pad/2, fill); break;
                case '<':
                default:  os << s << std::string(pad, fill);                             break;
            }
        } else {
            os << s;
        }
    }
    template<typename T, typename... Args>
    void fprint_impl(std::ostream& os, const std::string& fmt, const size_t pos, const T& value, Args&&... args) {
        std::string spec;
        size_t next = emit_literal_until_placeholder(os, fmt, pos, spec);

        if (next == std::string::npos)
            throw std::runtime_error("Too many arguments for format string");

        if (spec.empty()) {
            os << value;
        } else {
            apply_spec(os, value, parse_spec(spec));
        }

        fprint_impl(os, fmt, next, std::forward<Args>(args)...);
    }
    template<typename T, typename... Args>
    std::string fStringPrint(std::ostringstream& ss, const std::string& fmt, const size_t pos, T& value, Args&&... args) {
        fprint_impl(ss, fmt, pos, value, std::forward<Args>(args)...);
        return ss.str();
    }
    template<typename... Args>
    std::string stringPrint(const std::string& fmt, Args&&... args) {
        std::ostringstream ss;
        return fStringPrint(ss, fmt, 0, std::forward<Args>(args)...);
    }
    template<typename... Args>
    void print(const std::string& fmt, Args&&... args) {
        fprint_impl(std::cout, fmt, 0, args...);
    }
    template<typename... Args>
    void print(std::ostream& os, const std::string& fmt, Args&&... args) {
        fprint_impl(os, fmt, 0, args...);
    }
    template<typename... Args>
    void println(const std::string& fmt, Args&&... args) {
        fprint_impl(std::cout, fmt, 0, args...);
        std::cout << std::endl;
    }
    template<typename... Args>
    void println(std::ostream& os, const std::string& fmt, Args&&... args) {
        fprint_impl(os, fmt, 0, args...);
        os << std::endl;
    }

}




/* ===================================================== */
/*                     FILE SYSTEM                       */
/* ===================================================== */
namespace nopp {
    bool mkdir_if_not_exists(const path & p);

    bool delete_file(const path & p);

    bool copy_directory_recursive(const path& src, const path& dst);

    bool file_exists(const path& p);

    file_type get_file_type(const path& p);

    path get_current_dir();

    void set_current_dir(const path& p);

    path running_executable_path();

    bool needs_rebuild(const path& output, std::span<path> inputs);

    std::vector<path> read_dir(const path& p);

    enum class WalkAction { Cont, Skip, Stop };

    void walk_dir(const path& root, auto callback);
}




/* ===================================================== */
/*                     BUILD SYSTEM                      */
/* ===================================================== */
namespace nopp {
    class Process;
    using ProcessList = std::vector<Process>;

    struct RunOpts {
        ProcessList* async = nullptr;
        size_t max_procs = 0;
        bool reset = true;
    };

    class Cmd {
    public:
        Cmd() = default;
        template<typename... Args>
        explicit Cmd(Args&&... args)                     { (args_.emplace_back(std::forward<Args>(args)), ...); }

        Cmd& append(std::string arg);
        template<typename... Args>
        Cmd& append(std::string first, Args&&... rest)   { append(std::move(first)); return append(std::forward<Args>(rest)...); }

        [[nodiscard]] inline bool run(const RunOpts &opts = {});

                      void        clear();
        [[nodiscard]] bool        empty() const;
        [[nodiscard]] std::string render() const;

        // Build a null-terminated argv for execvp. Valid only as long as *this lives.
        std::vector<char*> argv();
    private:
        std::vector<std::string> args_;
        [[nodiscard]] const std::string& program() const;
    };

    inline std::ostream& operator<<(std::ostream& os, const Cmd& cmd) {
        return os << cmd.render();
    }

    class Process {
    public:
        static constexpr pid_t invalid = -1;

        explicit Process(const pid_t pid = invalid) : pid_(pid) {}

        // Non-copyable, movable
        Process(const Process&)            = delete;
        Process& operator=(const Process&) = delete;
        Process(Process&& o) noexcept : pid_(std::exchange(o.pid_, invalid)) {}
        Process& operator=(Process&& o) noexcept {
            pid_ = std::exchange(o.pid_, invalid);
            return *this;
        }

        [[nodiscard]] bool valid() const;
        [[nodiscard]] pid_t get()  const;

        // Block until the process exits. Returns true on clean exit (code 0).
        [[nodiscard]] bool wait() const &&;

        enum class PollResult { Done = 1, Running = 0, Error = -1 };

        [[nodiscard]] PollResult poll(std::chrono::milliseconds delay = std::chrono::milliseconds{1}) const;
    private:
        pid_t pid_;
        static bool wait_impl(pid_t pid);
        static bool interpret_status(int wstatus);
        static void sleep_ns(long ns);
    };

    [[nodiscard]] std::optional<Process> spawn(Cmd& cmd);

    int num_procs();

    [[nodiscard]] bool procs_wait(std::vector<Process>& procs);

    void go_rebuild_urself(int argc, char** argv, std::initializer_list<path> sources);

#define NOPP_REBUILD_URSELF(argc, argv, ...) \
    nopp::go_rebuild_urself((argc), (argv), { __FILE__ }, { __VA_ARGS__ })
}




/* ===================================================== */
/*              BENCHMARKING AND PROFILING               */
/* ===================================================== */
namespace nopp {

#define BENCHMARK nopp::Benchmark::Get()

    struct ProfileResult {
        std::string name;
        i64 start, end;
    };

    struct BenchmarkSession {
        std::string name;
    };

    class Benchmark {
    private:
        BenchmarkSession* current_session;
        std::ofstream output_stream;
        i32 profile_count;

        std::chrono::time_point<std::chrono::high_resolution_clock> session_start_time;

    public:
        Benchmark() : current_session(nullptr), profile_count(0) {}

        void BeginSession(const std::string& name, const std::string& filepath = "results.json");
        void EndSession();

        i64 GetTimeSinceStartMicroseconds() const;

        void WriteProfile(const ProfileResult& result);
        void WriteHeader();
        void WriteFooter();
        static Benchmark& Get();
    };

    struct ScopedBenchmarkTimer {
        const char* label;
        i64 start;

        explicit ScopedBenchmarkTimer(const char* label) : label(label), start(Benchmark::Get().GetTimeSinceStartMicroseconds()) {}

        ~ScopedBenchmarkTimer() {
            const i64 end = Benchmark::Get().GetTimeSinceStartMicroseconds();
            Benchmark::Get().WriteProfile({label, start, end});
        }
    };

#ifndef STRIP_PROFILING
#define PROFILE_SCOPE(name) ScopedBenchmarkTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif

    struct ScopedTimer {
        const char* label;
        std::chrono::high_resolution_clock::time_point start;

        explicit ScopedTimer(const char* label) : label(label), start(std::chrono::high_resolution_clock::now()) {}
        ~ScopedTimer() {
            const std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::high_resolution_clock::now();
            const f64 ms = std::chrono::duration<double, std::milli>(end - start).count();
            LOG_INFO("{} took {:.3f} ms", label, ms);
        }
    };

    struct Timer {
        using Clock = std::chrono::high_resolution_clock;
        Clock::time_point start;

        Timer() { reset(); }

        void reset() { start = Clock::now(); }

        [[nodiscard]]
        double elapsed_ms() const { return std::chrono::duration<double, std::milli>(Clock::now() - start).count(); }

        [[nodiscard]]
        double elapsed_sec() const { return std::chrono::duration<double>(Clock::now() - start).count(); }
    };
}





enum class TargetType {
    STATIC_LIB,
    EXECUTABLE
};

inline std::ostream& operator<<(std::ostream& os, const TargetType& t) {
    switch (t) {
        case TargetType::STATIC_LIB:
            return os << "STATIC_LIB";
        case TargetType::EXECUTABLE:
            return os << "EXECUTABLE";
    }
    return os << "UNKNOWN";
}

class Target {
public:
    std::string target_name;
    nopp::path  output_path;
    TargetType  target_type;

    bool publish_includes = false;
    bool publish_defines = false;

    Target(const std::string& _target_name, const nopp::path& _output_path, const TargetType _target_type) :
    target_name(_target_name), output_path(_output_path), target_type(_target_type) {
        reformat_output_path();
    }

private:
    std::vector<nopp::path>    include_paths;
    std::vector<nopp::path>    src_files;
    std::vector<std::string>   defines;
    std::vector<const Target*> dependencies;

    bool built = false;

public:
    void reformat_output_path() {
        nopp::mkdir_if_not_exists(output_path);
        LOG_ASSERT(nopp::is_directory(output_path) && "OUTPUT PATH MUST BE A DIRECTORY");
        if (!output_path.is_absolute()) {
            output_path = nopp::absolute(output_path);
        }
    }

    nopp::path obj_path(const nopp::path& src) const {
        return nopp::absolute(output_path) / (src.filename().stem().string() + ".o");
    }

    bool compile_objects(nopp::ProcessList& procs) {
        LOG_INFO("    {}: building .o files", target_name);
        nopp::Cmd cmd;
        for (nopp::path& src_file : src_files) {
            cmd.append("g++");
            cmd.append("-o", obj_path(src_file).string());
            cmd.append("-c", src_file.string());
            for (auto& p : include_paths) cmd.append("-I" + p.string());
            _include_dependencies_(cmd);
            for (auto& d : defines) cmd.append("-D" + d);
            _include_defines_(cmd);
            if (!cmd.run({.async = &procs})) return false;
        }
        return true;
    }

    void print_all() {
        nopp::println("{} {} - {}", target_type, target_name, output_path.string());
        nopp::println("    Include Paths:");
        for (nopp::path& p : include_paths) {
            nopp::println("        {}", p.string());
        }
        nopp::println("    Source Files:");
        for (nopp::path& p : src_files) {
            nopp::println("        {}", p.string());
        }
        nopp::println("    Defines:");
        for (std::string& s : defines) {
            nopp::println("        {}", s);
        }
        nopp::println("    Dependencies:");
        for (const Target* t : dependencies) {
            nopp::println("        library: {}", t->target_name);
            nopp::println("        located: {}", t->output_path);
        }
    }

    bool add_include_path(const nopp::path& include_path) {
        LOG_ASSERT(nopp::is_directory(include_path) && "INCLUDE PATH MUST BE A DIRECTORY");
        include_paths.emplace_back(nopp::absolute(include_path));
        return true;
    }

    bool add_src(const nopp::path& src_dir) {
        if (nopp::is_directory(src_dir)) {
            nopp::walk_dir(src_dir, [this](const auto& entry) {
                if (entry.path().extension() == ".cpp")
                    src_files.emplace_back(nopp::absolute(entry.path()));
                return nopp::WalkAction::Cont;
            });
        } else {
            src_files.emplace_back(nopp::absolute(src_dir));
        }
        return true;
    }

    bool add_dependency(const Target* dependency) {
        dependencies.emplace_back(dependency);
        return true;
    }

    bool add_dependencies(const std::vector<Target*>& dependency_list) {
        for (const Target* t : dependency_list) {
            add_dependency(t);
        }
        return true;
    }

    void _include_defines_(nopp::Cmd& cmd) const {
        if (publish_defines) {
            for (const std::string& define : defines) {
                std::stringstream ss;
                ss << "-D" << define;
                cmd.append(ss.str());
            }
        }

        for (const Target* t : dependencies) {
            t->_include_defines_(cmd);
        }
    }

    void _include_dependencies_(nopp::Cmd& cmd) const {
        if (publish_includes) {
            for (const nopp::path& include_path : include_paths) {
                std::stringstream ss;
                ss << "-I" << include_path.string();
                cmd.append(ss.str());
            }
        }

        for (const Target* t : dependencies) {
            t->_include_dependencies_(cmd);
        }
    }

    void _link_dependencies_(nopp::Cmd& cmd) const {
        if (dependencies.empty()) {
            LOG_ASSERT(built && "dependency not built before linking");
            std::stringstream ss;
            ss << "-L" << output_path.string();
            cmd.append(ss.str());
            ss.str("");
            ss.clear();
            ss << "-l" << target_name;
            cmd.append(ss.str());
            return;
        }

        for (const Target* t : dependencies) {
            LOG_ASSERT(t->built && "dependency not built before linking");
            t->_link_dependencies_(cmd);
        }
    }

    bool build() {
        LOG_INFO("Building {} {}", target_type, target_name);

        nopp::Cmd cmd;
        nopp::ProcessList procs;

        bool success = true;

        compile_objects(procs);

        switch (target_type) {
            case TargetType::STATIC_LIB: {
                LOG_INFO("    {}: combining into .a library", target_name);
                cmd.append("ar", "-rcs");
                cmd.append((output_path / ("lib" + target_name + ".a")).string());
                for (auto& src : src_files)
                    cmd.append(obj_path(src).string());
                success = cmd.run();
            } break;

            case TargetType::EXECUTABLE: {
                LOG_INFO("    {}: linking executable", target_name);
                cmd.append("g++");
                cmd.append("-o", (output_path / target_name).string());
                for (auto& src : src_files)
                    cmd.append(obj_path(src).string());
                _link_dependencies_(cmd);
                success = cmd.run();
            } break;

            default:
                UNREACHABLE();
        }

        if (success) { built = true; }

        return success;
    }

    [[nodiscard]] bool run() const {
        LOG_ASSERT(target_type == TargetType::EXECUTABLE && "TARGET_TYPE MUST BE EXECUTABLE TO RUN");
        nopp::Cmd cmd;
        const nopp::path executable_path = output_path / target_name;
        cmd.append(executable_path.string());
        return cmd.run();
    }
};























//#endif //NOPP_NOPP_H



























//#ifdef NOPP_IMPLEMENTATION

namespace nopp {
    inline bool mkdir_if_not_exists(const path & p) {
        try {
            create_directories(p);
        } catch (filesystem_error& e) {
            LOG_ERROR(e.what());
            return false;
        }
        return true;
    }

    inline bool delete_file(const path & p) {
        try {
            remove(p);
        } catch (filesystem_error& e) {
            LOG_ERROR(e.what());
            return false;
        }
        return true;
    }

    inline bool copy_directory_recursive(const path& src, const path& dst) {
        try {
            std::filesystem::copy(src, dst, std::filesystem::copy_options::recursive);
        } catch (filesystem_error& e) {
            LOG_ERROR(e.what());
            return false;
        }
        return true;
    }

    inline bool file_exists(const path& p) {
        return exists(p);
    }

    inline file_type get_file_type(const path& p) {
        return status(p).type();
    }

    inline path get_current_dir() {
        return current_path();
    }

    inline void set_current_dir(const path& p) {
        current_path(p);
    }

    inline path running_executable_path() {
#ifdef _WIN32
        char buf[MAX_PATH];
        DWORD len = GetModuleFileNameA(NULL, buf, MAX_PATH);
        if (len == 0) throw std::runtime_error("GetModuleFileNameA failed");
        return path(buf, buf + len);

#elif __APPLE__
        uint32_t size = 0;
        _NSGetExecutablePath(nullptr, &size);
        std::string buf(size, '\0');
        if (_NSGetExecutablePath(buf.data(), &size) != 0)
            throw std::runtime_error("_NSGetExecutablePath failed");
        return {buf};

#elif __FreeBSD__
        int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
        size_t size = 0;
        sysctl(mib, 4, nullptr, &size, nullptr, 0);
        std::string buf(size, '\0');
        if (sysctl(mib, 4, buf.data(), &size, nullptr, 0) != 0)
            throw std::runtime_error("sysctl failed");
        return path(buf);

#elif __HAIKU__
        image_info info;
        int32 cookie = 0;
        while (get_next_image_info(B_CURRENT_TEAM, &cookie, &info) == B_OK) {
            if (info.type == B_APP_IMAGE)
                return path(info.name);
        }
        throw std::runtime_error("get_next_image_info failed");

#else
        std::string buf(1024, '\0');
        ssize_t len = readlink("/proc/self/exe", buf.data(), buf.size());
        if (len == -1) throw std::runtime_error("readlink failed");
        buf.resize(len);
        return path(buf);
#endif
    }

    inline bool needs_rebuild(const path& output, std::span<path> inputs) {
        if (!exists(output)) return true;
        const auto out_time = last_write_time(output);
        return std::ranges::any_of(inputs, [&](const path& p) {
            return last_write_time(p) > out_time;
        });
    }

    inline std::vector<path> read_dir(const path& p) {
        return { directory_iterator(p), directory_iterator{} };
    }

    inline void walk_dir(const path& root, auto callback) {
        recursive_directory_iterator it(root);
        const recursive_directory_iterator end;
        while (it != end) {
            const WalkAction action = callback(*it);
            if (action == WalkAction::Stop) break;
            if (action == WalkAction::Skip && it->is_directory())
                it.disable_recursion_pending(); // key std::filesystem hook for this
            ++it;
        }
    }
}

namespace nopp {
    inline Cmd& Cmd::append(std::string arg) {
        args_.emplace_back(std::move(arg));
        return *this;
    }

    inline void Cmd::clear() { args_.clear(); }

    [[nodiscard]] inline bool Cmd::empty() const { return args_.empty(); }

    inline std::string Cmd::render() const {
        std::ostringstream ss;
        for (const auto& arg : args_) { ss << arg << ' '; }
        return ss.str();
    }

    inline std::vector<char*> Cmd::argv() {
        std::vector<char*> out;
        out.reserve(args_.size() + 1);
        for (auto& s : args_) out.push_back(s.data());
        out.push_back(nullptr);
        return out;
    }

    inline const std::string& Cmd::program() const { return args_.front(); }

    inline bool Process::valid() const { return pid_ != invalid; }
    inline pid_t Process::get()  const { return pid_; }

    // Block until the process exits. Returns true on clean exit (code 0).
    inline bool Process::wait() const && {
        if (!valid()) return false;
        return wait_impl(pid_);
    }

    inline Process::PollResult Process::poll(const std::chrono::milliseconds delay) const {
        if (!valid()) return PollResult::Error;

        int wstatus = 0;
        const pid_t pid = ::waitpid(pid_, &wstatus, WNOHANG);

        if (pid < 0) {
            LOG_ERROR("waitpid({}): {}", pid_, strerror(errno));
            return PollResult::Error;
        }

        if (pid == 0) {
            sleep_ns(std::chrono::duration_cast<std::chrono::nanoseconds>(delay).count());
            return PollResult::Running;
        }

        return interpret_status(wstatus) ? PollResult::Done : PollResult::Error;
    }

    inline bool Process::wait_impl(const pid_t pid) {
        for (;;) {
            int wstatus = 0;
            if (::waitpid(pid, &wstatus, 0) < 0) {
                LOG_ERROR("waitpid({}): {}", pid, strerror(errno));
                return false;
            }
            if (WIFEXITED(wstatus) || WIFSIGNALED(wstatus))
                return interpret_status(wstatus);
        }
    }

    inline bool Process::interpret_status(int wstatus) {
        if (WIFEXITED(wstatus)) {
            const int code = WEXITSTATUS(wstatus);
            if (code != 0) {
                LOG_ERROR("Process exited with code {}", code);
                return false;
            }
            return true;
        }
        if (WIFSIGNALED(wstatus)) {
            LOG_ERROR("Process terminated by signal {}", WTERMSIG(wstatus));
            return false;
        }
        return false;
    }

    inline void Process::sleep_ns(const long ns) {
        const struct timespec ts{ ns / 1'000'000'000L, ns % 1'000'000'000L };
        ::nanosleep(&ts, nullptr);
    }

    inline std::optional<Process> spawn(Cmd& cmd) {
        if (cmd.empty()) {
            LOG_ERROR("Cannot spawn: empty command");
            return std::nullopt;
        }

        LOG_DEBUG("CMD: {}", cmd.render());

        const pid_t c_pid = ::fork();
        if (c_pid < 0) {
            LOG_ERROR("fork(): {}", strerror(errno));
            return std::nullopt;
        }

        if (c_pid == 0) {
            // child
            const auto argv = cmd.argv();
            ::execvp(argv[0], argv.data());
            LOG_ERROR("execvp({}): {}", c_pid, strerror(errno));
            ::_exit(1);
        }

        return Process{c_pid};
    }

    inline int num_procs() {
#ifdef _WIN32
        SYSTEM_INFO si{};
        ::GetSystemInfo(&si);
        return static_cast<int>(si.dwNumberOfProcessors);
#else
        return static_cast<int>(::sysconf(_SC_NPROCESSORS_ONLN));
#endif
    }

    inline bool procs_wait(std::vector<Process>& procs)
    {
        bool success = true;
        for (auto& proc : procs) {
            const bool ok = std::move(proc).wait();
            success = success && ok;
        }
        procs.clear();
        return success;
    }

    inline void go_rebuild_urself(
        const int argc, char** argv,
        const std::initializer_list<path> sources,
        const std::initializer_list<path> watch = {})
    {
        path binary_path = argv[0];

#ifdef _WIN32
        if (binary_path.extension() != ".exe")
            binary_path += ".exe";
#endif

        const auto bin_time = std::filesystem::last_write_time(binary_path);
        bool needs_rebuild  = false;

        auto check = [&](const path& p) {
            std::error_code ec;
            const auto t = std::filesystem::last_write_time(p, ec);
            if (!ec && t > bin_time) needs_rebuild = true;
        };

        for (const auto& src : sources) check(src);
        for (const auto& w : watch)     check(w);

        if (!needs_rebuild) return;

        const path old_binary = path{binary_path} += ".old";

        std::error_code ec;
        std::filesystem::rename(binary_path, old_binary, ec);
        if (ec) {
            LOG_ERROR("Could not rename {}: {}", binary_path, ec.message());
            std::exit(1);
        }

        Cmd cmd;
        cmd.append("c++", "-std=c++20", "-o", binary_path.string());
        for (const auto& src : sources)
            cmd.append(src.string());

        if (!cmd.run()) {
            std::filesystem::rename(old_binary, binary_path);
            std::exit(1);
        }

        std::filesystem::remove(old_binary, ec);

        Cmd reexec;
        reexec.append(binary_path.string());
        for (int i = 1; i < argc; ++i)
            reexec.append(argv[i]);

        if (!reexec.run()) std::exit(1);
        std::exit(0);
    }

    inline bool Cmd::run(const RunOpts &opts) {
        const std::size_t limit = opts.max_procs > 0
                                ? opts.max_procs
                                : static_cast<std::size_t>(num_procs()) + 1;

        // Drain finished async processes until we have room for one more.
        if (opts.async) {
            while (opts.async->size() >= limit) {
                bool reaped = false;
                for (std::size_t i = 0; i < opts.async->size(); ++i) {
                    const auto result = (*opts.async)[i].poll();
                    if (result == Process::PollResult::Error) {
                        if (opts.reset) clear();
                        return false;
                    }
                    if (result == Process::PollResult::Done) {
                        // Swap-and-pop (order doesn't matter here)
                        (*opts.async)[i] = std::move(opts.async->back());
                        opts.async->pop_back();
                        reaped = true;
                        break;
                    }
                }
                (void)reaped; // loop again if nothing freed yet
            }
        }

        auto proc = spawn(*this);
        if (opts.reset) clear();

        if (!proc) return false;

        if (opts.async) {
            opts.async->emplace_back(std::move(*proc));
            return true;
        }

        return std::move(*proc).wait();
    }



     inline FormatSpec parse_spec(const std::string& spec) {
        FormatSpec fs;
        size_t i = 0;

        // Optional fill+align (two chars) or just align (one char)
        if (spec.size() >= 2 && (spec[1] == '<' || spec[1] == '>' || spec[1] == '^')) {
            fs.fill  = spec[0];
            fs.align = spec[1];
            i = 2;
        } else if (!spec.empty() && (spec[0] == '<' || spec[0] == '>' || spec[0] == '^')) {
            fs.align = spec[0];
            i = 1;
        }

        // Optional sign
        if (i < spec.size() && (spec[i] == '+' || spec[i] == '-' || spec[i] == ' ')) {
            fs.sign = spec[i++];
        }

        // Optional width
        if (i < spec.size() && std::isdigit(spec[i])) {
            fs.width = 0;
            while (i < spec.size() && std::isdigit(spec[i]))
                fs.width = fs.width * 10 + (spec[i++] - '0');
        }

        // Optional .precision
        if (i < spec.size() && spec[i] == '.') {
            ++i;
            fs.precision = 0;
            while (i < spec.size() && std::isdigit(spec[i]))
                fs.precision = fs.precision * 10 + (spec[i++] - '0');
        }

        // Optional type
        if (i < spec.size())
            fs.type = spec[i];

        return fs;
    }

    inline size_t emit_literal_until_placeholder(
            std::ostream& os,
            const std::string& fmt,
            size_t pos,
            std::string& spec_out)
    {
        while (pos < fmt.size()) {
            if (fmt[pos] == '{') {
                if (pos + 1 < fmt.size() && fmt[pos+1] == '{') {
                    os << '{';      // escaped {
                    pos += 2;
                } else {
                    // Find matching closing brace
                    const size_t close = fmt.find('}', pos + 1);
                    if (close == std::string::npos)
                        throw std::runtime_error("Unmatched '{' in format string");

                    // spec is everything between { and }
                    // strip leading ':' if present  e.g. "{:.2f}" -> ".2f", "{}" -> ""
                    std::string inner = fmt.substr(pos + 1, close - pos - 1);
                    spec_out = inner.empty() || inner[0] != ':' ? inner : inner.substr(1);
                    return close + 1;   // position after the closing brace
                }
            } else if (fmt[pos] == '}') {
                if (pos + 1 < fmt.size() && fmt[pos+1] == '}') {
                    os << '}';      // escaped }
                    pos += 2;
                } else {
                    throw std::runtime_error("Unmatched '}' in format string");
                }
            } else {
                os << fmt[pos++];
            }
        }
        return std::string::npos;   // reached end without finding a placeholder
    }

    inline std::string fStringPrint(std::ostringstream& ss, const std::string& fmt, const size_t pos) {
        fprint_impl(ss, fmt, pos);
        return ss.str();
    }

    inline void fprint_impl(std::ostream& os, const std::string& fmt, const size_t pos) {
        std::string spec;
        const size_t next = emit_literal_until_placeholder(os, fmt, pos, spec);
        if (next != std::string::npos)
            throw std::runtime_error("Too few arguments for format string");
    }

    inline void Benchmark::BeginSession(const std::string& name, const std::string& filepath) {
        session_start_time = std::chrono::high_resolution_clock::now();

        output_stream.open(filepath);
        if (!output_stream.is_open()) {
            LOG_WARN("Failed to open file '{}'", filepath);
        }
        WriteHeader();
        current_session = new BenchmarkSession{name};
    }

    inline void Benchmark::EndSession() {
        WriteFooter();
        output_stream.close();
        delete current_session;
        current_session = nullptr;
        profile_count = 0;
    }

    inline i64 Benchmark::GetTimeSinceStartMicroseconds() const {
        const auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(now - session_start_time).count();
    }

    inline void Benchmark::WriteProfile(const ProfileResult& result) {
        if (profile_count++ > 0) {
            output_stream << ",";
        }

        std::string name = result.name;
        std::ranges::replace(name, '"', '\'');

        output_stream << "{";
        output_stream << R"("cat":"function",)";
        output_stream << "\"dur\":" << (result.end - result.start) << ',';
        output_stream << R"("name":")" << name << "\",";
        output_stream << R"("ph":"X",)";
        output_stream << "\"pid\":0,";
        output_stream << "\"tid\":0,";
        output_stream << "\"ts\":" << result.start;
        output_stream << "}";

        output_stream.flush();
    }

    inline void Benchmark::WriteHeader() {
        output_stream << R"({"otherData": {},"traceEvents":[)";
        output_stream.flush();
    }

    inline void Benchmark::WriteFooter() {
        output_stream << "]}";
        output_stream.flush();
    }

    inline Benchmark& Benchmark::Get() {
        static auto* instance = new Benchmark();
        return *instance;
    }

}
#endif