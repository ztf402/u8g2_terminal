/**
 * @file terminal.cpp
 * @brief Terminal class implementation for displaying text on a U8G2 display.
 * This file contains the implementation of the Terminal class, which provides methods
 * for initializing the terminal, printing text, clearing the display, and drawing the
 * terminal content on a U8G2 display.
 * @author ztf402
 * @date 2025-05-04
 * @version 0.2
 * @update 
 * 0.2 added support for multiple font sizes
 *     added Terminal_setFontSize() to change font size dynamically
 *     added deinit() method to clean up resources
 * 0.1 initial version
 * @note This code is designed to work with the U8G2 library for graphics display
 * 
 */
#include "terminal.hpp"

static const uint8_t* getFontBySize(uint8_t size) {
    switch (size) {
        case 1: return u8g2_font_6x12_tf;
        case 2: return u8g2_font_7x13_tf;
        case 3: return u8g2_font_9x15_tf;
        case 4: return u8g2_font_10x20_tf;
        default: return u8g2_font_6x12_tf;
    }
}

void Terminal_init(Terminal &term, U8G2 &u8g2_ref,
                   uint8_t x, uint8_t y,
                   uint8_t width, uint8_t height,
                   uint8_t font_height) {
    term.u8g2 = &u8g2_ref;
    term.x = x;
    term.y = y;
    term.width = width;
    term.height = height;
    term.font_height = font_height;
    term.max_lines = height / font_height;
    term.buffer.clear();
}

void Terminal_deinit(Terminal &term) {
    term.u8g2 = nullptr;
    term.buffer.clear();
    term.x = term.y = term.width = term.height = 0;
    term.font_height = 0;
    term.max_lines = 0;
    term.font_size = 1;
}

void Terminal_clear(Terminal &term) {
    term.buffer.clear();
}


void Terminal_setFontSize(Terminal &term, uint8_t size) {
    if (size < 1) size = 1;
    if (size > 4) size = 4;
    term.font_size = size;
    // 可选：根据字号自动调整 font_height
    switch (size) {
        case 1: term.font_height = 12; break;
        case 2: term.font_height = 13; break;
        case 3: term.font_height = 15; break;
        case 4: term.font_height = 20; break;
        default: term.font_height = 12; break;
    }
    term.max_lines = term.height / term.font_height;
}

void Terminal_print(Terminal &term, const char* msg) {
    std::string s(msg);
    size_t pos = 0, next;
    while ((next = s.find('\n', pos)) != std::string::npos) {
        term.buffer.push_back({s.substr(pos, next - pos), term.font_size});
        pos = next + 1;
    }
    term.buffer.push_back({s.substr(pos), term.font_size});

    // 超出行数时滚动
    while (term.buffer.size() > term.max_lines) {
        term.buffer.erase(term.buffer.begin());
    }
}

void Terminal_draw(const Terminal &term) {
    term.u8g2->setDrawColor(1);
    term.u8g2->firstPage();
    do {
        term.u8g2->drawFrame(term.x, term.y, term.width, term.height);

        // 1. 反向累加，找到能显示的最后几行
        int total_height = 0;
        std::vector<const TerminalLine*> lines_to_draw;
        for (auto it = term.buffer.rbegin(); it != term.buffer.rend(); ++it) {
            uint8_t line_height = 12;
            switch (it->font_size) {
                case 1: line_height = 12; break;
                case 2: line_height = 13; break;
                case 3: line_height = 15; break;
                case 4: line_height = 20; break;
                default: line_height = 12; break;
            }
            if (total_height + line_height > term.height) break;
            total_height += line_height;
            lines_to_draw.push_back(&(*it));
        }
        // 2. 正序绘制，从顶部开始
        std::reverse(lines_to_draw.begin(), lines_to_draw.end());
        uint8_t y_pos = term.y;
        for (const auto* line : lines_to_draw) {
            term.u8g2->setFont(getFontBySize(line->font_size));
            uint8_t line_height = 12;
            switch (line->font_size) {
                case 1: line_height = 12; break;
                case 2: line_height = 13; break;
                case 3: line_height = 15; break;
                case 4: line_height = 20; break;
                default: line_height = 12; break;
            }
            y_pos += line_height;
            term.u8g2->drawStr(term.x + 2, y_pos, line->text.c_str());
        }
    } while (term.u8g2->nextPage());
}
