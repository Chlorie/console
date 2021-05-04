#include "console/utility.h"
#include "console/basic_io.h"

#include <cassert>
#include <iostream>

namespace cnsl
{
    namespace
    {
        void show_prompt_message(const std::string_view message, const std::string_view hint)
        {
            set_foreground_color(color16::bright_green);
            std::cout << "? ";
            set_foreground_color(color16::bright_white);
            std::cout << message;
            set_foreground_color(color16::cyan);
            std::cout << "  " << hint << '\n';
            set_foreground_color(color16::white);
        }

        void show_selections(const std::vector<std::string>& selections, const size_t selected)
        {
            for (size_t i = 0; i < selections.size(); i++)
            {
                if (i == selected)
                {
                    set_foreground_color(color16::cyan);
                    std::cout << "> " << selections[i] << '\n';
                }
                else
                {
                    set_foreground_color(color16::white);
                    std::cout << "  " << selections[i] << '\n';
                }
            }
            cursor_prev_line(selections.size());
        }
    }

    size_t selection(const std::string_view message, const std::vector<std::string>& selections)
    {
        assert(!selections.empty());

        hide_cursor();
        show_prompt_message(message, "[Use arrows to move]");

        size_t current = 0;
        while (true)
        {
            show_selections(selections, current);
            switch (get_input())
            {
                case input::up:
                case input::left:
                    current = (current - 1 + selections.size()) % selections.size();
                    continue;
                case input::down:
                case input::right:
                    current = (current + 1) % selections.size();
                    continue;
                case input::enter: break;
                default: continue;
            }
            break;
        }

        cursor_prev_line();
        clear_display();
        show_prompt_message(message, selections[current]);
        show_cursor();

        return current;
    }

    bool boolean(const std::string_view message)
    {
        show_prompt_message(message, "[Y/N]");
        hide_cursor();

        const bool result = []
        {
            while (true)
            {
                switch (get_input())
                {
                    case input::y: return true;
                    case input::n: return false;
                    default: continue;
                }
            }
        }();

        cursor_prev_line();
        clear_display();
        show_prompt_message(message, result ? "Yes" : "No");
        show_cursor();

        return result;
    }

    std::string read_line(const std::string_view message, const std::string_view default_line)
    {
        set_foreground_color(color16::bright_green);
        std::cout << "? ";
        set_foreground_color(color16::bright_white);
        std::cout << message << ' ';
        save_cursor_position();
        set_foreground_color(color16::yellow);
        std::cout << '[' << default_line << "]  ";
        set_foreground_color(color16::cyan);

        std::string line;
        getline(std::cin, line);

        std::string result = line.empty() ? std::string(default_line) : std::move(line);
        restore_cursor_position();
        clear_display();
        set_foreground_color(color16::cyan);
        std::cout << ' ' << result << '\n';
        set_foreground_color(color16::white);

        return result;
    }

    void success_message(const std::string_view message)
    {
        set_foreground_color(color16::bright_green);
        std::cout << "\u2713 ";
        set_foreground_color(color16::bright_white);
        std::cout << message << '\n';
        set_foreground_color(color16::white);
    }

    void error_message(const std::string_view message)
    {
        set_foreground_color(color16::bright_red);
        std::cout << "\u2717 " << message << '\n';
        set_foreground_color(color16::white);
    }
}
