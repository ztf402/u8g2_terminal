#pragma once

#include <U8g2lib.h>
#include <vector>
#include <string>

struct TerminalLine {
    std::string text;
    uint8_t font_size;
};

struct Terminal {
    U8G2* u8g2;
    uint8_t x, y;
    uint8_t width, height;
    uint8_t font_height;
    uint8_t max_lines;
    uint8_t font_size = 1;
    std::vector<TerminalLine> buffer; // 改为存储 TerminalLine
};

/**
 * 初始化一个终端实例
 * @param term         终端实例引用
 * @param u8g2_ref     U8g2 对象引用
 * @param x            终端区域左上角 X 坐标
 * @param y            终端区域左上角 Y 坐标
 * @param width        终端宽度
 * @param height       终端高度
 * @param font_height  字体行高，用于计算最大行数
 */
void Terminal_init(Terminal &term, U8G2 &u8g2_ref,
                   uint8_t x, uint8_t y,
                   uint8_t width, uint8_t height,
                   uint8_t font_height);

/**
 * 释放终端资源，重置所有参数
 * @param term  终端实例
 */
void Terminal_deinit(Terminal &term);
/**
 * 清除终端历史数据
 * @param term  终端实例
 */
void Terminal_clear(Terminal &term);
/**
 * 向终端打印文本，可包含多行，超出自动滚动
 * @param term  终端实例
 * @param msg   C 风格字符串，支持 '\n' 换行
 */
void Terminal_print(Terminal &term, const char* msg);

/**
 * 绘制终端内容
 * @param term  终端实例
 */
void Terminal_draw(const Terminal &term);

/**
 * 设置终端字体大小
 * @param term  终端实例
 * @param size  字体大小
 */
void Terminal_setFontSize(Terminal &term, uint8_t size);