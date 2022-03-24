#include "FBParser.h"

#include <fstream>
#include <algorithm>
#include <wx/log.h>

constexpr std::size_t FB_WIDTH = 120;
constexpr std::size_t FB_HEADER_KB  = 1;
constexpr std::size_t FB_DATA_KB    = 2;
constexpr std::size_t FB_TRAILER_KB = 8;
constexpr std::size_t FB_END_KB     = 9;

FBParser::FBParser() : m_null_str("")
{
    m_newline_code = "\r\n";

    set_fb_sohfuri();
}

FBParser::~FBParser()
{
}

bool FBParser::open_fb_file(const std::string &path)
{
    std::ifstream fb_ifs(path, std::ios::binary);

    if(!fb_ifs)
    {
        wxLogMessage("fb_ifs : false");
        return false;
    }

    fb_ifs.seekg(0, std::ios_base::end);
    auto file_size = fb_ifs.tellg();
    if(file_size > 104857600){
        wxLogMessage("file_size > 104857600");
        return false;
    }

    fb_ifs.seekg(0, std::ios_base::beg);

    m_fb_header_lines.clear();
    m_fb_data_lines.clear();
    m_fb_trailer_lines.clear();
    m_fb_end_lines.clear();


    std::string fb_str;
    fb_str.reserve(file_size);
    auto fb_ifs_it = std::istreambuf_iterator<char>(fb_ifs);
    auto fb_ifs_last = std::istreambuf_iterator<char>();
    fb_str.assign(fb_ifs_it, fb_ifs_last);

    fb_str.erase(std::remove(fb_str.begin(), fb_str.end(), '\r'), fb_str.end());
    fb_str.erase(std::remove(fb_str.begin(), fb_str.end(), '\n'), fb_str.end());


    if(fb_str.size() % FB_WIDTH != 0)
    {
        wxLogMessage("fb_str.size() mod FB_WIDTH != 0");
        return false;
    }


    int record_kb = 0;
    for(int i = 0; i < fb_str.size(); i += FB_WIDTH)
    {       
        std::string_view fb_str_line(fb_str.data() + i, FB_WIDTH);

        if(record_kb > fb_str_line.at(0) - '0')
        {
            wxLogMessage("record_type > fb_str_line_view.at(0) - '0'");
            return false;
        }

        record_kb = fb_str_line.at(0) - '0';

        FBAttrs *fb_attrs_ref;
        FBLines *fb_lines_ref;

        switch(record_kb)
        {
            case FB_HEADER_KB:
                fb_attrs_ref = &m_fb_header_attrs;
                fb_lines_ref = &m_fb_header_lines;
                break;

            case FB_DATA_KB:
                fb_attrs_ref = &m_fb_data_attrs;
                fb_lines_ref = &m_fb_data_lines;
                break;
            
            case FB_TRAILER_KB:
                fb_attrs_ref = &m_fb_trailer_attrs;
                fb_lines_ref = &m_fb_trailer_lines;
                break;

            case FB_END_KB:
                fb_attrs_ref = &m_fb_end_attrs;
                fb_lines_ref = &m_fb_end_lines;
                break;

            default:
                wxLogMessage("record_kb : undefined");
                return false;
        }

        for(const auto &attr : *fb_attrs_ref)
        {
            auto fb_str_sub = fb_str_line.substr(attr.offset, attr.length);

            if(fb_str_sub.find_first_not_of(attr.char_includes) != std::string_view::npos)
            {
                if(attr.initial_value == nullptr || fb_str_sub != std::string_view(attr.initial_value)){
                    wxLogMessage("fb_str_sub.find_first_not_of(char_includes) != std::string_view::npos");
                    wxLogMessage(wxString(wxT("invalid value : ") + std::string(fb_str_sub)));
                    return false;
                }
            }
        }

        fb_lines_ref->emplace_back(fb_str_line);
    }

    return true;
}

bool FBParser::save_fb_file(const std::string &path)
{
    std::ofstream fb_ofs(path, std::ios::binary);
    if(!fb_ofs)
    {
        wxLogMessage("fb_ofs : false");
        return false;
    }
    
    for(const auto & line : m_fb_header_lines )fb_ofs << line << m_newline_code;
    for(const auto & line : m_fb_data_lines   )fb_ofs << line << m_newline_code;
    for(const auto & line : m_fb_trailer_lines)fb_ofs << line << m_newline_code;
    for(const auto & line : m_fb_end_lines    )fb_ofs << line << m_newline_code;

    return true;
};

bool FBParser::set_fb_newline_code(std::string_view newline_code)
{
    m_newline_code = newline_code;
    return true;
}

std::string_view FBParser::get_fb_newline_code()
{
    return m_newline_code;
}


std::size_t FBParser::get_fb_row_size(FBLines& fb_lines, FBAttrs& fb_attrs)
{
    return fb_lines.size();
}

void FBParser::assign_fb_line(std::size_t num, FBLines& fb_lines, FBAttrs& fb_attrs)
{
    fb_lines.assign(num, std::string(FB_WIDTH, ' '));
}


std::string_view FBParser::get_fb_value(std::size_t row, std::size_t col, FBLines& fb_lines, FBAttrs& fb_attrs)
{
    if(row >= fb_lines.size() || col >= fb_attrs.size())
    {
        wxLogMessage("row >= fb_lines.size() || col >= fb_attr.size()");
        return m_null_str;
    }
    
    const auto &attr = fb_attrs.at(col);

    std::string_view line = fb_lines.at(row);
    auto value = line.substr(attr.offset, attr.length);
    
    return value;
 
}

bool FBParser::set_fb_value(std::size_t row, std::size_t col, std::string_view value, FBLines& fb_lines, FBAttrs& fb_attrs)
{
    if(row >= fb_lines.size() || col >= fb_attrs.size())
    {
        wxLogMessage("row >= fb_lines.size() || col >= fb_attr.size()");
        return false;
    }

    const auto &attr = fb_attrs.at(col);
    if(value.size() != attr.length)
    {
        wxLogMessage("value.size() != length");
        return false;
    }


    if(value.find_first_not_of(attr.char_includes) != std::string_view::npos)
    {
        if(attr.initial_value == nullptr || value != std::string_view(attr.initial_value))
        {
            wxLogMessage("value.find_first_not_of(char_includes) != std::string_view::npos");
            return false;
        }
    }

    auto &line = fb_lines.at(row);
    std::copy(value.begin(), value.end(), line.begin() + attr.offset);

    return true;
}

void FBParser::set_fb_sohfuri()
{
    m_fb_header_attrs = fb_header_attrs_sohfuri;
    m_fb_data_attrs = fb_data_attrs_sohfuri;
    m_fb_trailer_attrs = fb_trailer_attrs_sohfuri;
    m_fb_end_attrs = fb_end_attrs_sohfuri;
}

void FBParser::set_fb_kyuyo_shoyo()
{
    m_fb_header_attrs = fb_header_attrs_kyuyo_shoyo;
    m_fb_data_attrs = fb_data_attrs_kyuyo_shoyo;
    m_fb_trailer_attrs = fb_trailer_attrs_kyuyo_shoyo;
    m_fb_end_attrs = fb_end_attrs_kyuyo_shoyo;
}

void FBParser::set_fb_furikae()
{
    m_fb_header_attrs = fb_header_attrs_furikae;
    m_fb_data_attrs = fb_data_attrs_furikae;
    m_fb_trailer_attrs = fb_trailer_attrs_furikae;
    m_fb_end_attrs = fb_end_attrs_furikae;
}