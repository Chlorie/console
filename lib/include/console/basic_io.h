#pragma once

#include <cstdint>
#include <cstddef>

namespace cnsl
{
    using std::size_t;
    using std::uint8_t;
    using std::uint32_t;

    void cursor_up(size_t amount = 1);
    void cursor_down(size_t amount = 1);
    void cursor_forward(size_t amount = 1);
    void cursor_back(size_t amount = 1);
    void cursor_next_line(size_t amount = 1);
    void cursor_prev_line(size_t amount = 1);
    void cursor_to_column(size_t column = 1);
    void cursor_to(size_t row = 1, size_t column = 1);

    enum class clear_type : unsigned { till_end = 0, till_begin = 1, all = 2 };
    void clear_display(clear_type type = clear_type::till_end);
    void clear_line(clear_type type = clear_type::till_end);

    void scroll_up(size_t amount = 1);
    void scroll_down(size_t amount = 1);

    enum class color16 : uint8_t
    {
        black, red, green, yellow, blue, magenta, cyan, white,
        grey = 60, bright_red, bright_green, bright_yellow, bright_blue, bright_magenta, bright_cyan, bright_white
    };

    void set_foreground_color(color16 color);
    void set_foreground_color(uint8_t red, uint8_t green, uint8_t blue);
    void set_foreground_color(uint32_t rgb);
    void set_background_color(color16 color);
    void set_background_color(uint8_t red, uint8_t green, uint8_t blue);
    void set_background_color(uint32_t rgb);

    void save_cursor_position();
    void restore_cursor_position();

    void show_cursor();
    void hide_cursor();

    enum class input : int
    {
        backspace = 8, tab = 9, enter = 13, space = 32,
        num_0 = 48, num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9,
        a = 65, b, c, d, e, f, g, h, i, j, k, l, m,
        n, o, p, q, r, s, t, u, v, w, x, y, z,
        up = 255 + 65, down, right, left
    };

    int getch();
    int getche();
    void ungetch(int ch);
    input get_input();
}
