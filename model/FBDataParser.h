#ifndef FBDATAPARSER_H
#define FBDATAPARSER_H

#include<string>
#include<vector>
#include<tuple>

struct FBAttr {
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
using FBLines = std::vector<std::string>;

class FBDataParser final
{
public:
    FBDataParser();
    ~FBDataParser();

    bool open_fb_file(const std::string &path); 
    bool save_fb_file(const std::string &path);
    
    bool set_fb_newline_code(std::string_view newline_code);
    std::string_view get_fb_newline_code();

    std::size_t get_fb_header_row_size() { return get_fb_row_size(fb_header_lines,  fb_header_attrs); }
    std::size_t get_fb_data_row_size()   { return get_fb_row_size(fb_data_lines,    fb_data_attrs); }
    std::size_t get_fb_trailer_row_size(){ return get_fb_row_size(fb_trailer_lines, fb_trailer_attrs); }
    std::size_t get_fb_end_row_size()    { return get_fb_row_size(fb_end_lines,     fb_end_attrs); }

    bool assign_fb_header_line (std::size_t  num){ return assign_fb_line(num, fb_header_lines,  fb_header_attrs); }
    bool assign_fb_data_line   (std::size_t  num){ return assign_fb_line(num, fb_data_lines,    fb_data_attrs); }
    bool assign_fb_trailer_line(std::size_t  num){ return assign_fb_line(num, fb_trailer_lines, fb_trailer_attrs); }
    bool assign_fb_end_line    (std::size_t  num){ return assign_fb_line(num, fb_end_lines,     fb_end_attrs); }

    std::string_view get_fb_header_value (std::size_t row, std::size_t col){ return get_fb_value(row, col, fb_header_lines,  fb_header_attrs); }
    std::string_view get_fb_data_value   (std::size_t row, std::size_t col){ return get_fb_value(row, col, fb_data_lines,    fb_data_attrs); }
    std::string_view get_fb_trailer_value(std::size_t row, std::size_t col){ return get_fb_value(row, col, fb_trailer_lines, fb_trailer_attrs); }
    std::string_view get_fb_end_value    (std::size_t row, std::size_t col){ return get_fb_value(row, col, fb_end_lines,     fb_end_attrs); }

    bool set_fb_header_value (std::size_t row, std::size_t col, std::string_view value){ return set_fb_value(row, col, value, fb_header_lines,  fb_header_attrs); }
    bool set_fb_data_value   (std::size_t row, std::size_t col, std::string_view value){ return set_fb_value(row, col, value, fb_data_lines,    fb_data_attrs); };
    bool set_fb_trailer_value(std::size_t row, std::size_t col, std::string_view value){ return set_fb_value(row, col, value, fb_trailer_lines, fb_trailer_attrs); }
    bool set_fb_end_value    (std::size_t row, std::size_t col, std::string_view value){ return set_fb_value(row, col, value, fb_end_lines,     fb_end_attrs); }

    const FBAttrs& get_fb_header_attrs() { return fb_header_attrs; }
    const FBAttrs& get_fb_data_attrs()   { return fb_data_attrs; }
    const FBAttrs& get_fb_trailer_attrs(){ return fb_trailer_attrs; }  
    const FBAttrs& get_fb_end_attrs()    { return fb_end_attrs; }

private:
    std::size_t get_fb_row_size(FBLines&, FBAttrs&);
    bool assign_fb_line(std::size_t, FBLines&, FBAttrs&);
    std::string_view get_fb_value(std::size_t, std::size_t, FBLines&, FBAttrs&);
    bool set_fb_value(std::size_t, std::size_t, std::string_view, FBLines&, FBAttrs&);

    FBLines fb_header_lines;
    FBLines fb_data_lines;
    FBLines fb_trailer_lines;
    FBLines fb_end_lines;

    FBAttrs fb_header_attrs;
    FBAttrs fb_data_attrs;
    FBAttrs fb_trailer_attrs;
    FBAttrs fb_end_attrs;

    const std::string m_null_str;
    std::string m_newline_code;

    static const FBAttrs fb_header_attrs_sohfuri;
    static const FBAttrs fb_data_attrs_sohfuri;
    static const FBAttrs fb_trailer_attrs_sohfuri;
    static const FBAttrs fb_end_attrs_sohfuri;
};

#endif //FBDATAPARSER_H
