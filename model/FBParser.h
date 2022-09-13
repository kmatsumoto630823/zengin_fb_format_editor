#ifndef FBDATAPARSER_H
#define FBDATAPARSER_H

#include "common/FBAttrs.h"

#include<string>
#include<vector>
#include<array>

using FBLine = std::array<char, FB_WIDTH + 1>;
using FBBlock = std::vector<FBLine>;
using FBBlockArray = std::array<FBBlock, (int)FBPart::ITEM_NUM>;

class FBParser final
{
public:
    FBParser(const FBAttrsArray& attrs_array);
    ~FBParser();

    bool open_file(const std::string& path); 
    bool save_file(const std::string& path);
    bool from_text(const std::string& text);

    FBParser& set_newline(FBNewLine newline = FBNewLine::CRLF);
    FBNewLine get_newline();

    FBParser& set_current_part(FBPart part = FBPart::HEADER);
    FBPart get_current_part();

    // For Grid Operation
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
    FBPart m_current_part;

    FBBlockArray m_block_array;
    const FBAttrsArray& m_attrs_array;
};

#endif //FBDATAPARSER_H
