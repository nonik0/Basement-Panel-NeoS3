#pragma once

// ChatGPT couldn't cope so fixed up with https://tchapi.github.io/Adafruit-GFX-Font-Customiser/

#include <Adafruit_GFX.h>

// Bitmap data for the 3x4 font
const uint8_t Font3x4Bitmaps[] PROGMEM = {
  0xD0, 0xA0, 0x2E, 0x74, 0x79, 0xE0, 0xA9, 0x50, 0x53, 0x50, 0xC0, 0x69, 
  0x96, 0xDD, 0x80, 0x5D, 0x00, 0x60, 0xC0, 0xE0, 0x29, 0x40, 0xF6, 0xF0, 
  0x59, 0x70, 0xE7, 0x70, 0xEC, 0xF0, 0xB7, 0x90, 0xF8, 0xF0, 0x9E, 0xF0, 
  0xE5, 0x20, 0xFE, 0xF0, 0xF7, 0x90, 0xA0, 0xA0, 0x71, 0x80, 0xE3, 0x80, 
  0xC7, 0x00, 0xE5, 0x20, 0xFE, 0x70, 0x57, 0xD0, 0xDE, 0xF0, 0xF2, 0x70, 
  0xD6, 0xE0, 0xFA, 0x70, 0xF3, 0x40, 0xF2, 0x70, 0xBE, 0xD0, 0xE9, 0x70, 
  0x26, 0xF0, 0xBB, 0xD0, 0x92, 0x70, 0xFE, 0xD0, 0xD6, 0xD0, 0xF6, 0xF0, 
  0xF7, 0xC0, 0xF6, 0xE0, 0xF7, 0x50, 0xF1, 0xF0, 0xE9, 0x20, 0xB6, 0xF0, 
  0xB6, 0xA0, 0xB7, 0xF0, 0xAA, 0xD0, 0xB5, 0x20, 0xE7, 0x70, 0xEB, 0x89, 
  0x10, 0xD7, 0x56, 0x80, 0xE0, 0x90, 0x0E, 0xF0, 0x9A, 0xF0, 0x0E, 0xF0, 
  0x2E, 0xF0, 0x1F, 0x70, 0x73, 0xC0, 0x1F, 0xB0, 0x9A, 0xD0, 0x8B, 0x20, 
  0xE0, 0x97, 0x50, 0xF0, 0x1F, 0xD0, 0x1A, 0xD0, 0x1E, 0xF0, 0x1F, 0xC0, 
  0x1F, 0x90, 0x0E, 0x40, 0x0D, 0x60, 0x5D, 0x30, 0x16, 0xF0, 0x16, 0xA0, 
  0x17, 0xF0, 0x15, 0x50, 0x15, 0x20, 0x19, 0x30, 0x6B, 0x30, 0xF0, 0xC9, 
  0xE0, 0x78, 0x00
};

const GFXglyph Font3x4Glyphs[] PROGMEM = {
  {     0,   0,   0,   1,    0,   -3 },   // 0x20 ' '
  {     0,   1,   4,   1,    0,   -3 },   // 0x21 '!'
  {     1,   3,   1,   3,    0,   -3 },   // 0x22 '"'
  {     2,   4,   4,   4,    0,   -3 },   // 0x23 '#'
  {     4,   3,   4,   3,    0,   -3 },   // 0x24 '$'
  {     6,   3,   4,   3,    0,   -3 },   // 0x25 '%'
  {     8,   3,   4,   3,    0,   -3 },   // 0x26 '&'
  {    10,   1,   2,   1,    0,   -3 },   // 0x27 '''
  {    11,   2,   4,   2,    0,   -3 },   // 0x28 '('
  {    12,   2,   4,   2,    0,   -3 },   // 0x29 ')'
  {    13,   3,   3,   3,    0,   -3 },   // 0x2A '*'
  {    15,   3,   3,   3,    0,   -3 },   // 0x2B '+'
  {    17,   2,   2,   2,    0,   -1 },   // 0x2C ','
  {    18,   2,   1,   3,    1,   -2 },   // 0x2D '-'
  {    19,   1,   1,   1,    0,    0 },   // 0x2E '.'
  {    20,   3,   4,   3,    0,   -3 },   // 0x2F '/'
  {    22,   3,   4,   3,    0,   -3 },   // 0x30 '0'
  {    24,   3,   4,   3,    0,   -3 },   // 0x31 '1'
  {    26,   3,   4,   3,    0,   -3 },   // 0x32 '2'
  {    28,   3,   4,   3,    0,   -3 },   // 0x33 '3'
  {    30,   3,   4,   3,    0,   -3 },   // 0x34 '4'
  {    32,   3,   4,   3,    0,   -3 },   // 0x35 '5'
  {    34,   3,   4,   3,    0,   -3 },   // 0x36 '6'
  {    36,   3,   4,   3,    0,   -3 },   // 0x37 '7'
  {    38,   3,   4,   3,    0,   -3 },   // 0x38 '8'
  {    40,   3,   4,   3,    0,   -3 },   // 0x39 '9'
  {    42,   1,   4,   1,    0,   -3 },   // 0x3A ':'
  {    43,   1,   4,   1,    0,   -3 },   // 0x3B ';'
  {    44,   3,   3,   3,    0,   -3 },   // 0x3C '<'
  {    46,   3,   3,   3,    0,   -3 },   // 0x3D '='
  {    48,   3,   3,   3,    0,   -3 },   // 0x3E '>'
  {    50,   3,   4,   3,    0,   -3 },   // 0x3F '?'
  {    52,   3,   4,   3,    0,   -3 },   // 0x40 '@'
  {    54,   3,   4,   3,    0,   -3 },   // 0x41 'A'
  {    56,   3,   4,   3,    0,   -3 },   // 0x42 'B'
  {    58,   3,   4,   3,    0,   -3 },   // 0x43 'C'
  {    60,   3,   4,   3,    0,   -3 },   // 0x44 'D'
  {    62,   3,   4,   3,    0,   -3 },   // 0x45 'E'
  {    64,   3,   4,   3,    0,   -3 },   // 0x46 'F'
  {    66,   3,   4,   3,    0,   -3 },   // 0x47 'G'
  {    68,   3,   4,   3,    0,   -3 },   // 0x48 'H'
  {    70,   3,   4,   3,    0,   -3 },   // 0x49 'I'
  {    72,   3,   4,   3,    0,   -3 },   // 0x4A 'J'
  {    74,   3,   4,   3,    0,   -3 },   // 0x4B 'K'
  {    76,   3,   4,   3,    0,   -3 },   // 0x4C 'L'
  {    78,   3,   4,   3,    0,   -3 },   // 0x4D 'M'
  {    80,   3,   4,   3,    0,   -3 },   // 0x4E 'N'
  {    82,   3,   4,   3,    0,   -3 },   // 0x4F 'O'
  {    84,   3,   4,   3,    0,   -3 },   // 0x50 'P'
  {    86,   3,   4,   3,    0,   -3 },   // 0x51 'Q'
  {    88,   3,   4,   3,    0,   -3 },   // 0x52 'R'
  {    90,   3,   4,   3,    0,   -3 },   // 0x53 'S'
  {    92,   3,   4,   3,    0,   -3 },   // 0x54 'T'
  {    94,   3,   4,   3,    0,   -3 },   // 0x55 'U'
  {    96,   3,   4,   3,    0,   -3 },   // 0x56 'V'
  {    98,   3,   4,   3,    0,   -3 },   // 0x57 'W'
  {   100,   3,   4,   3,    0,   -3 },   // 0x58 'X'
  {   102,   3,   4,   3,    0,   -3 },   // 0x59 'Y'
  {   104,   3,   4,   3,    0,   -3 },   // 0x5A 'Z'
  {   106,   2,   4,   3,    0,   -3 },   // 0x5B '['
  {   107,   3,   4,   3,    0,   -3 },   // 0x5C '\'
  {   109,   2,   4,   3,    0,   -3 },   // 0x5D ']'
  {   110,   3,   3,   3,    0,   -3 },   // 0x5E '^'
  {   112,   3,   1,   3,    0,   -1 },   // 0x5F '_'
  {   113,   2,   2,   2,    0,   -3 },   // 0x60 '`'
  {   114,   3,   4,   3,    0,   -3 },   // 0x61 'a'
  {   116,   3,   4,   3,    0,   -3 },   // 0x62 'b'
  {   118,   3,   4,   3,    0,   -3 },   // 0x63 'c'
  {   120,   3,   4,   3,    0,   -3 },   // 0x64 'd'
  {   122,   3,   4,   3,    0,   -3 },   // 0x65 'e'
  {   124,   3,   4,   3,    0,   -3 },   // 0x66 'f'
  {   126,   3,   4,   3,    0,   -3 },   // 0x67 'g'
  {   128,   3,   4,   3,    0,   -3 },   // 0x68 'h'
  {   130,   2,   4,   3,    1,   -3 },   // 0x69 'i'
  {   131,   3,   4,   3,    0,   -3 },   // 0x6A 'j'
  {   133,   3,   4,   3,    0,   -3 },   // 0x6B 'k'
  {   135,   1,   4,   1,    0,   -3 },   // 0x6C 'l'
  {   136,   3,   4,   3,    0,   -3 },   // 0x6D 'm'
  {   138,   3,   4,   3,    0,   -3 },   // 0x6E 'n'
  {   140,   3,   4,   3,    0,   -3 },   // 0x6F 'o'
  {   142,   3,   4,   3,    0,   -3 },   // 0x70 'p'
  {   144,   3,   4,   3,    0,   -3 },   // 0x71 'q'
  {   146,   3,   4,   3,    0,   -3 },   // 0x72 'r'
  {   148,   3,   4,   3,    0,   -3 },   // 0x73 's'
  {   150,   3,   4,   3,    0,   -3 },   // 0x74 't'
  {   152,   3,   4,   3,    0,   -3 },   // 0x75 'u'
  {   154,   3,   4,   3,    0,   -3 },   // 0x76 'v'
  {   156,   3,   4,   3,    0,   -3 },   // 0x77 'w'
  {   158,   3,   4,   3,    0,   -3 },   // 0x78 'x'
  {   160,   3,   4,   3,    0,   -3 },   // 0x79 'y'
  {   162,   3,   4,   3,    0,   -3 },   // 0x7A 'z'
  {   164,   3,   4,   3,    0,   -3 },   // 0x7B '{'
  {   166,   1,   4,   1,    0,   -3 },   // 0x7C '|'
  {   167,   3,   4,   3,    0,   -3 },   // 0x7D '}'
  {   169,   3,   2,   3,    0,   -2 }    // 0x7E '~'
};

const GFXfont Font3x4 PROGMEM = {
    (uint8_t *)Font3x4Bitmaps,      // Pointer to the bitmap data
    (GFXglyph *)Font3x4Glyphs, 0x20, 0x7E,                            // Last ASCII character ('~')
    4                           // Font height in pixels
};
