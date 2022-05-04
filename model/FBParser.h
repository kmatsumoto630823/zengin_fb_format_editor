#ifndef FBDATAPARSER_H
#define FBDATAPARSER_H

#include "common/FBAttrs.h"

#include<string>
#include<vector>
#include<array>

using FBLine = std::array<char, FB_WIDTH + 1>;
using FBGrid = std::vector<FBLine>;
using FBGridArray = std::array<FBGrid, (int)FBPart::ITEM_NUM>;

class FBParser final
{
public:
    FBParser();
    ~FBParser();

    bool open_file(const std::string& path); 
    bool save_file(const std::string& path);

    FBParser& set_newline(FBNewLine newline = FBNewLine::CRLF);
    FBNewLine get_newline();

    FBParser& set_fbtype(FBType type = FBType::SOHFURI, std::string_view chars_kana = "");
    FBType get_fbtype();

    FBParser& set_fbpart(FBPart part = FBPart::HEADER);
    FBPart get_fbpart();

    const FBAttrs& get_attrs(FBPart part = FBPart::CURRENT);
    const FBAttrsArray& get_attrs_array();

    // For Gid Operation
    std::size_t get_number_rows(FBPart part = FBPart::CURRENT);
    std::size_t get_number_cols(FBPart part = FBPart::CURRENT); 

    std::string_view get_value(std::size_t row, std::size_t col, FBPart part = FBPart::CURRENT);
    bool set_value(std::size_t row, std::size_t col, std::string_view value, FBPart part = FBPart::CURRENT);

    bool assign_rows(std::size_t num, FBPart part = FBPart::CURRENT);
    bool append_rows(std::size_t num = 1, FBPart part = FBPart::CURRENT);
    bool insert_rows(std::size_t row, std::size_t num = 1, FBPart part = FBPart::CURRENT);
    bool delete_rows(std::size_t row, std::size_t num = 1, FBPart part = FBPart::CURRENT);

private:
    FBNewLine m_newline;

    FBType m_fbtype;
    FBPart m_fbpart;

    FBGridArray m_grid_array;
    FBAttrsArray m_attrs_array;

    std::string m_chars_kana;
};

#endif //FBDATAPARSER_H
