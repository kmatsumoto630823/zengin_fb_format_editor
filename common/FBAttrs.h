#ifndef FB_ATTRS_H
#define FB_ATTRS_H

#include<vector>
#include<array>
#include<string>

constexpr std::size_t FB_LINE_WIDTH = 120;
constexpr std::size_t FB_PARTCODE_HEADER  = 1;
constexpr std::size_t FB_PARTCODE_DATA    = 2;
constexpr std::size_t FB_PARTCODE_TRAILER = 8;
constexpr std::size_t FB_PARTCODE_END     = 9;

using FBEnumInt = int;

enum class FBPart : FBEnumInt
{
    HEADER = 0,
    DATA,
    TRAILER,
    END,
    ITEM_NUM
};

constexpr std::array<const char*, (FBEnumInt)FBPart::ITEM_NUM> FBPartLabel=
{
    "ヘッダー",
    "データ",
    "トレーラー",
    "エンド"
};

enum class FBType : FBEnumInt
{
    SOHFURI = 0,
    KYUYO_SHOYO,
    FURIKAE,
    ITEM_NUM
};

constexpr std::array<const char*, (FBEnumInt)FBType::ITEM_NUM> FBTypeLabel=
{
    "総合振込",
    "給与・賞与振込",
    "預金口座振替"
};

enum class FBNewLine : FBEnumInt
{
    UNDEFINED = -1,
    CRLF = 0,
    CR,
    LF,
    NONE,
    ITEM_NUM
};

constexpr std::array<const char*, (FBEnumInt)FBNewLine::ITEM_NUM> FBNewLineLabel=
{
    "CRLF",
    "CR",
    "LF",
    "改行なし"
};

constexpr std::array<const char*, (FBEnumInt)FBNewLine::ITEM_NUM> FB_NEWLINE_CODE = 
{
    "\r\n",
    "\r",
    "\n",
    ""
};

struct FBAttr
{
    std::size_t  order;
    std::size_t  offset;
    std::size_t  length;
    const char*  char_includes;
    const char*  pad_info;
    const char*  initial_value;
    const char*  label;
    const char*  description;
};

using FBAttrs = std::vector<FBAttr>;
using FBAttrsArray = std::array<FBAttrs, (FBEnumInt)FBPart::ITEM_NUM>;

FBAttrsArray make_attrs_array
(
    FBType type = FBType::SOHFURI,
    const char* chars_kana = nullptr,
    const char* pad_kana   = nullptr,
    const char* chars_num  = nullptr,
    const char* pad_num    = nullptr
);

#endif //FB_ATTRS_H
