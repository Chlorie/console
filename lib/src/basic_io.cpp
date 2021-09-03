#include "console/basic_io.h"

#include <cinttypes>
#include <cstdio>

#ifdef _WIN32
#   if !__has_include(<conio.h>)
#       error Cannot find <conio.h> for Windows
#   else
#       include <conio.h>
#       include <Windows.h>
#   endif
#else // Assume *nix
#   if !__has_include(<termios.h>) || !__has_include(<sys/ioctl.h>)
#       error Cannot find <termios.h> or <sys/ioctl.h> for *nix
#   else
#       include <termios.h>
#       include <sys/ioctl.h>
namespace
{
    template <typename Func>
    auto in_raw_mode(Func&& func, const bool echo = false)
    {
        termios prev{};
        tcgetattr(0, &prev);
        termios current = prev;
        current.c_lflag &= ~ICANON;
        if (!echo) current.c_lflag &= ~ECHO;
        tcsetattr(0, TCSANOW, &current);
        auto result = static_cast<Func&&>(func)();
        tcsetattr(0, TCSANOW, &prev);
        return result;
    }
}
#   endif
#endif

namespace cnsl
{
    console_size get_console_size()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
        return {
            static_cast<size_t>(info.srWindow.Bottom - info.srWindow.Top + 1),
            static_cast<size_t>(info.srWindow.Right - info.srWindow.Left + 1)
        };
#else
        winsize size{};
        ioctl(0, TIOCGWINSZ, &size);
        return { size.ws_row, size.ws_col };
#endif
    }

    void cursor_up(const size_t amount) { std::printf("\x1b[%zuA", amount); }
    void cursor_down(const size_t amount) { std::printf("\x1b[%zuB", amount); }
    void cursor_forward(const size_t amount) { std::printf("\x1b[%zuC", amount); }
    void cursor_back(const size_t amount) { std::printf("\x1b[%zuD", amount); }
    void cursor_next_line(const size_t amount) { std::printf("\x1b[%zuE", amount); }
    void cursor_prev_line(const size_t amount) { std::printf("\x1b[%zuF", amount); }
    void cursor_to_column(const size_t column) { std::printf("\x1b[%zuG", column); }
    void cursor_to(const size_t row, const size_t column) { std::printf("\x1b[%zu;%zuH", row, column); }

    void clear_display(const clear_type type) { std::printf("\x1b[%uJ", static_cast<unsigned>(type)); }
    void clear_line(const clear_type type) { std::printf("\x1b[%uK", static_cast<unsigned>(type)); }

    void scroll_up(const size_t amount) { std::printf("\x1b[%zuS", amount); }
    void scroll_down(const size_t amount) { std::printf("\x1b[%zuT", amount); }

    void set_foreground_color(const color16 color) { std::printf("\x1b[%dm", static_cast<int>(color) + 30); }
    void set_foreground_color(const uint8_t red, const uint8_t green, const uint8_t blue)
    {
        std::printf("\x1b[38;2;%" PRIu8 ";%" PRIu8 ";%" PRIu8 "m",
            red, green, blue);
    }
    void set_foreground_color(const uint32_t rgb)
    {
        set_foreground_color(
            static_cast<uint8_t>(rgb >> 16),
            static_cast<uint8_t>(rgb >> 8),
            static_cast<uint8_t>(rgb));
    }

    void set_background_color(const color16 color) { std::printf("\x1b[%dm", static_cast<int>(color) + 40); }
    void set_background_color(const uint8_t red, const uint8_t green, const uint8_t blue)
    {
        std::printf("\x1b[38;2;%" PRIu8 ";%" PRIu8 ";%" PRIu8 "m",
            red, green, blue);
    }
    void set_background_color(const uint32_t rgb)
    {
        set_background_color(
            static_cast<uint8_t>(rgb >> 16),
            static_cast<uint8_t>(rgb >> 8),
            static_cast<uint8_t>(rgb));
    }

    void save_cursor_position() { std::printf("\x1b[s"); }
    void restore_cursor_position() { std::printf("\x1b[u"); }

    void show_cursor() { std::printf("\x1b[?25h"); }
    void hide_cursor() { std::printf("\x1b[?25l"); }

    int getch()
    {
#ifdef _WIN32
        return _getch();
#else
        return in_raw_mode(std::getchar);
#endif
    }

    int getche()
    {
#ifdef _WIN32
        return _getche();
#else
        return in_raw_mode(std::getchar, true);
#endif
    }

    void ungetch(const int ch)
    {
#ifdef _WIN32
        (void)_ungetch(ch);
#else
        (void)in_raw_mode([ch] { return std::ungetc(ch, stdin); });
#endif
    }

    input get_input()
    {
        switch (const int ch = getch())
        {
            case '\r':
            case '\n': return input::enter;
            case '\x1b': // Typical Linux
            {
                if (const int ch2 = getch(); ch2 != 91) // '['
                {
                    ungetch(ch2);
                    return static_cast<input>(ch);
                }
                switch (const int ch3 = getch())
                {
                    case 'A': return input::up;
                    case 'B': return input::down;
                    case 'C': return input::right;
                    case 'D': return input::left;
                    default:
                        ungetch(ch3);
                        ungetch('[');
                        return static_cast<input>(ch);
                }
            }
            case 224: // Typical Windows
            {
                switch (const int ch2 = getch())
                {
                    case 'H': return input::up;
                    case 'P': return input::down;
                    case 'M': return input::right;
                    case 'K': return input::left;
                    default:
                        ungetch(ch2);
                        return static_cast<input>(ch);
                }
            }
            default:
            {
                if (ch >= 'a' && ch <= 'z')
                    return static_cast<input>(ch - 'a' + 'A');
                return static_cast<input>(ch);
            }
        }
    }
}
