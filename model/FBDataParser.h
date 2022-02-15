#ifndef FBDATAPARSER_H
#define FBDATAPARSER_H

#include<string>
#include<vector>
#include<tuple>

using FBInfo = std::vector<std::tuple<
    std::size_t,  // num
    const char *, // label
    std::size_t,  // offset
    std::size_t,  // length
    const char *, // char includes
    const char *, // pad info
    const char *, // default value
    const char *  // description
>>;

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

    std::size_t get_fb_header_row_size() { return get_fb_row_size(fb_header_lines, fb_header_info); }
    std::size_t get_fb_data_row_size()   { return get_fb_row_size(fb_data_lines, fb_data_info); }
    std::size_t get_fb_trailer_row_size(){ return get_fb_row_size(fb_trailer_lines, fb_trailer_info); }
    std::size_t get_fb_end_row_size()    { return get_fb_row_size(fb_end_lines, fb_end_info); }

    bool assign_fb_header_line (std::size_t  num){ return assign_fb_line(num, fb_header_lines, fb_header_info); }
    bool assign_fb_data_line   (std::size_t  num){ return assign_fb_line(num, fb_data_lines, fb_data_info); }
    bool assign_fb_trailer_line(std::size_t  num){ return assign_fb_line(num, fb_trailer_lines, fb_trailer_info); }
    bool assign_fb_end_line    (std::size_t  num){ return assign_fb_line(num, fb_end_lines, fb_end_info); }

    std::string_view get_fb_header_value (std::size_t row, std::size_t col){ return get_fb_value(row, col, fb_header_lines, fb_header_info); }
    std::string_view get_fb_data_value   (std::size_t row, std::size_t col){ return get_fb_value(row, col, fb_data_lines, fb_data_info); }
    std::string_view get_fb_trailer_value(std::size_t row, std::size_t col){ return get_fb_value(row, col, fb_trailer_lines, fb_trailer_info); }
    std::string_view get_fb_end_value    (std::size_t row, std::size_t col){ return get_fb_value(row, col, fb_end_lines, fb_end_info); }

    bool set_fb_header_value (std::size_t row, std::size_t col, std::string_view value){ return set_fb_value(row, col, value, fb_header_lines, fb_header_info); }
    bool set_fb_data_value   (std::size_t row, std::size_t col, std::string_view value){ return set_fb_value(row, col, value, fb_data_lines, fb_data_info); };
    bool set_fb_trailer_value(std::size_t row, std::size_t col, std::string_view value){ return set_fb_value(row, col, value, fb_trailer_lines, fb_trailer_info); }
    bool set_fb_end_value    (std::size_t row, std::size_t col, std::string_view value){ return set_fb_value(row, col, value, fb_end_lines, fb_end_info); }

    const FBInfo& get_fb_header_info() { return fb_header_info; }
    const FBInfo& get_fb_data_info()   { return fb_data_info; }
    const FBInfo& get_fb_trailer_info(){ return fb_trailer_info; }  
    const FBInfo& get_fb_end_info()    { return fb_end_info; }

private:
    std::size_t get_fb_row_size(FBLines&, FBInfo&);
    bool assign_fb_line(std::size_t, FBLines&, FBInfo&);
    std::string_view get_fb_value(std::size_t, std::size_t, FBLines&, FBInfo&);
    bool set_fb_value(std::size_t, std::size_t, std::string_view, FBLines&, FBInfo&);

    FBLines fb_header_lines;
    FBLines fb_data_lines;
    FBLines fb_trailer_lines;
    FBLines fb_end_lines;

    FBInfo fb_header_info;
    FBInfo fb_data_info;
    FBInfo fb_trailer_info;
    FBInfo fb_end_info;

    const std::string m_null_str;
    std::string m_newline_code;

    static const FBInfo fb_header_info_sohfuri;
    static const FBInfo fb_data_info_sohfuri;
    static const FBInfo fb_trailer_info_sohfuri;
    static const FBInfo fb_end_info_sohfuri;

    FBDataParser(const FBDataParser&) = delete;
    FBDataParser& operator=(const FBDataParser&) = delete;

    FBDataParser(FBDataParser&&) = delete;
    FBDataParser& operator=(FBDataParser&&) = delete;
};

#endif //FBDATAPARSER_H
