#ifndef FBATTRS_H
#define FBATTRS_H

#include<vector>
#include<array>

enum class FBPart : int
{
    CURRENT = -1,
    HEADER = 0,
    DATA,
    TRAILER,
    END,
    ITEM_NUM
};

constexpr std::size_t FB_WIDTH = 120;
constexpr std::size_t FB_PART_HEADER  = 1;
constexpr std::size_t FB_PART_DATA    = 2;
constexpr std::size_t FB_PART_TRAILER = 8;
constexpr std::size_t FB_PART_END     = 9;

enum class FBType : int
{
    CURRENT = -1,
    SOHFURI = 0,
    KYUYO_SHOYO,
    FURIKAE,
    ITEM_NUM
};

enum FBNewLine : int
{
    CURRENT = -1,
    CRLF = 0,
    CR,
    LF,
    NONE,
    ITEM_NUM
};

constexpr std::array<const char *, (int)FBNewLine::ITEM_NUM > FB_NEWLINE_CODE = 
{
    "\r\n",
    "\r",
    "\n",
    ""
};

struct FBAttr
{
    std::size_t  num;
    const char*  label;
    std::size_t  offset;
    std::size_t  length;
    const char*  char_includes;
    const char*  pad_info;
    const char*  initial_value;
    const char*  description;
};

using FBAttrs = std::vector<FBAttr>;
using FBAttrsArray = std::array<FBAttrs, (int)FBPart::ITEM_NUM>;

extern const char CHARS_NUM[];
extern const char PAD_NUM[];

extern const char CHARS_KANA[];
extern const char PAD_KANA[];

extern const char CHARS_DUMMY[];
extern const char PAD_DUMMY[];

extern const FBAttrsArray ATTRS_ARRAY_SOHFURI;
extern const FBAttrsArray ATTRS_ARRAY_KYUYO_SHOYO;
extern const FBAttrsArray ATTRS_ARRAY_FURIKAE;

#endif //FBATTRS_H
