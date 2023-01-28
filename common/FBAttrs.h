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

constexpr std::size_t FB_ORDER_HEADER_TORIKUMIBI = 5;
constexpr std::size_t FB_ORDER_DATA_KINGAKU      = 9;
constexpr std::size_t FB_ORDER_TRAILER_KENSU     = 1;
constexpr std::size_t FB_ORDER_TRAILER_KINGAKU   = 2;

using FBEnumInt = int;

enum class FBPart : FBEnumInt
{
    UNDEFINED = -1,
    HEADER = 0,
    DATA,
    TRAILER,
    END,
    ITEM_NUM
};



constexpr std::array<const char*, (FBEnumInt)FBPart::ITEM_NUM> FBPartLabel =
{
    "ヘッダーレコード",
    "データレコード",
    "トレーラーレコード",
    "エンドレコード"
};

enum class FBType : FBEnumInt
{
    UNDEFINED = -1,
    SOHFURI = 0,
    KYUYO_SHOYO,
    FURIKAE,
    ITEM_NUM
};

constexpr std::array<const char*, (FBEnumInt)FBType::ITEM_NUM> FBTypeLabel =
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

constexpr std::array<const char*, (FBEnumInt)FBNewLine::ITEM_NUM> FBNewLineLabel =
{
    "CRLF",
    "CR",
    "LF",
    "改行しない"
};

constexpr std::array<const char*, (FBEnumInt)FBNewLine::ITEM_NUM> FB_NEWLINE_CODE = 
{
    "\r\n",
    "\r",
    "\n",
    ""
};

#include <concepts>

template<typename T>
concept StringCheckable = requires (T x, std::string str, std::size_t siz)
{
    { x.find_first_not_of(str) != T::npos} -> std::same_as<bool>;    
    { x.length() != siz } -> std::same_as<bool>;
};

template<typename T>
concept StringPaddable = requires (T x, std::size_t siz, char c)
{
    { x.append(siz, c) };
    { x.insert(siz, siz, c) };
    { x.length() > siz } -> std::same_as<bool>;
};

struct FBAttr
{
    std::size_t  order;
    std::size_t  offset;
    std::size_t  length;

    char         char_kind;
    std::string  char_includes;

    char         optionality;    
    std::string  padding_conf;

    std::string  label;
    std::string  descript;

    template<typename T>
    requires StringCheckable<T>
    bool check_value(const T& value) const
    {
        if(value.length() != length)
        {
            return false; 
        }

        if(value.find_first_not_of(char_includes) != value.npos)
        {
            if(optionality != 'O' || value.find_first_not_of(" ") != value.npos)
            {
                return false;
            }
        }

        return true;    
    }

    template<typename T>
    requires StringPaddable<T>
    void pad_value(T& value) const
    {
        if(value.length() > length)
        {
            return;
        }

        if(optionality == 'O' && value.length() == 0)
        {
            value.assign(length, ' ');
        }

        auto pad_side = this->padding_conf.at(0);
        auto pad_char = this->padding_conf.at(1);

        if(pad_side == 'R')
        {
            value.append(length - value.length(), pad_char);
        }
        else if(pad_side == 'L')
        {
            value.insert(0, length - value.length(), pad_char);
        }

        return;
    }
};

using FBAttrs = std::vector<FBAttr>;
using FBAttrsArray = std::array<FBAttrs, (FBEnumInt)FBPart::ITEM_NUM>;

FBAttrsArray make_attrs_array
(
    FBType type = FBType::SOHFURI,
    std::string chars_kana   = "",
    std::string padding_kana = "",
    std::string chars_num    = "",
    std::string padding_num  = ""
);

#endif //FB_ATTRS_H
