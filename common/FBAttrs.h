#ifndef FBATTRS_H
#define FBATTRS_H

#include<vector>
#include<array>
#include<string>

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

enum class FBNewLine : int
{
    CURRENT = -1,
    CRLF = 0,
    CR,
    LF,
    NONE,
    ITEM_NUM
};

constexpr std::array<const char*, (std::underlying_type_t<FBNewLine>)FBNewLine::ITEM_NUM> FB_NEWLINE_CODE = 
{
    "\r\n",
    "\r",
    "\n",
    ""
};

struct FBAttr
{
    std::size_t  order;
    const char*  label;
    std::size_t  offset;
    std::size_t  length;
    const char*  char_includes;
    const char*  pad_info;
    const char*  initial_value;
    const char*  description;
};

using FBAttrs = std::vector<FBAttr>;
using FBAttrsArray = std::array<FBAttrs, (std::underlying_type_t<FBPart>)FBPart::ITEM_NUM>;

FBAttrsArray make_attrs_array
(
    FBType type = FBType::SOHFURI,
    const char* chars_kana = nullptr,
    const char* pad_kana   = nullptr,
    const char* chars_num  = nullptr,
    const char* pad_num    = nullptr
);


#endif //FBATTRS_H
