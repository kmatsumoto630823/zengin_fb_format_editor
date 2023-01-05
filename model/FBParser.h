#ifndef FB_PARSER_H
#define FB_PARSER_H

#include "common/FBAttrs.h"

#include<string>
#include<vector>
#include<array>

using FBLine = std::array<char, FB_LINE_WIDTH + 1>;
using FBGridBase = std::vector<FBLine>;

class FBGrid : public FBGridBase
{
public:
    FBGrid(const FBAttrs& attrs);

    bool add_line(const FBLine& line);

    // For Grid Operation
    std::size_t get_number_rows() const;
    std::size_t get_number_cols() const; 

    const std::string& get_value(std::size_t row, std::size_t col);
    bool set_value(std::size_t row, std::size_t col, const std::string& value);

    bool assign_rows(std::size_t num_rows);
    bool append_rows(std::size_t num_rows);
    bool insert_rows(std::size_t row, std::size_t num_rows);
    bool delete_rows(std::size_t row, std::size_t num_rows);
    
private:
    FBAttrs m_attrs;
    std::string m_stdstr_buff;
};

class FBParser
{
public:
    FBParser(const FBAttrsArray& attrs_array);

    bool open_file(const std::string& path); 
    bool save_file(const std::string& path);
    bool from_text(const std::string& text);

    FBGrid& operator[](FBPart part){ return m_grid_array.at((FBEnumInt)part); };
    FBGrid& operator[](FBEnumInt part){ return m_grid_array.at(part); };

    FBParser& set_newline(FBNewLine newline);
    FBNewLine get_newline() const;

private:
    using FBGridArray = std::array<FBGrid, (FBEnumInt)FBPart::ITEM_NUM>;

    FBGridArray m_grid_array;    
    FBNewLine m_newline;
};

#endif //FB_PARSER_H