#include "FBParser.h"

#include <fstream>
#include <algorithm>
#include <wx/log.h>

FBParser::FBParser(const FBAttrsArray& attrs_array)
: m_attrs_array(attrs_array)
{
    set_current_part();
    set_newline();
}

FBParser::~FBParser()
{

}

bool FBParser::open_file(const std::string& path)
{
    std::ifstream fb_ifs(path, std::ios::binary);

    if(!fb_ifs)
    {
        wxLogMessage("fb_ifs : false");
        return false;
    }

    fb_ifs.seekg(0, std::ios_base::end);
    auto file_size = fb_ifs.tellg();
    if(file_size > 104'857'600)
    {
        wxLogMessage("file_size > 100MB");
        return false;
    }

    fb_ifs.seekg(0, std::ios_base::beg);

    for(auto& block : m_block_array)block.clear();

    std::string fb_str;
    fb_str.reserve(file_size);
    auto fb_ifs_it = std::istreambuf_iterator<char>(fb_ifs);
    auto fb_ifs_last = std::istreambuf_iterator<char>();
    fb_str.assign(fb_ifs_it, fb_ifs_last);

    return from_text(fb_str);
}

bool FBParser::save_file(const std::string& path)
{
    std::ofstream fb_ofs(path, std::ios::binary);
    if(!fb_ofs)
    {
        wxLogMessage("fb_ofs : false");
        return false;
    }

    for(auto& block : m_block_array)
    {
        for(auto& line : block)
        fb_ofs << std::string_view(line.data()) << FB_NEWLINE_CODE[(std::underlying_type_t<FBNewLine>)m_newline];
    }


    return true;
};

bool FBParser::from_text(const std::string& text)
{
    auto fb_str = text;
    fb_str.erase(std::remove(fb_str.begin(), fb_str.end(), '\r'), fb_str.end());
    fb_str.erase(std::remove(fb_str.begin(), fb_str.end(), '\n'), fb_str.end());
    fb_str.erase(std::remove(fb_str.begin(), fb_str.end(), '\t'), fb_str.end());

    if(fb_str.size() % FB_WIDTH != 0)
    {
        wxLogMessage("fb_str.size() mod FB_WIDTH != 0");
        return false;
    }


    int record_kb = 0;
    for(int i = 0; i < fb_str.size(); i += FB_WIDTH)
    {
        FBLine line;
        line.fill(' ');
        line.back() = '\0';
        auto first = fb_str.begin() + i;
        auto last = first + FB_WIDTH;
        std::copy(first, last, line.begin());

        std::string_view line_view(line.data());

        if(record_kb > line_view.at(0) - '0')
        {
            wxLogMessage("record_type > fb_str_line_view.at(0) - '0'");
            return false;
        }

        record_kb = line_view.at(0) - '0';

        const FBAttrs* attrs_ref;
        FBBlock* block_ref;

        switch(record_kb)
        {
            case FB_PART_HEADER :
                attrs_ref = &m_attrs_array.at((std::underlying_type_t<FBPart>)FBPart::HEADER);
                block_ref = &m_block_array.at((std::underlying_type_t<FBPart>)FBPart::HEADER);
                break;

            case FB_PART_DATA :
                attrs_ref = &m_attrs_array.at((std::underlying_type_t<FBPart>)FBPart::DATA);
                block_ref = &m_block_array.at((std::underlying_type_t<FBPart>)FBPart::DATA);
                break;
            
            case FB_PART_TRAILER :
                attrs_ref = &m_attrs_array.at((std::underlying_type_t<FBPart>)FBPart::TRAILER);
                block_ref = &m_block_array.at((std::underlying_type_t<FBPart>)FBPart::TRAILER);
                break;

            case FB_PART_END :
                attrs_ref = &m_attrs_array.at((std::underlying_type_t<FBPart>)FBPart::END);
                block_ref = &m_block_array.at((std::underlying_type_t<FBPart>)FBPart::END);
                break;

            default:
                wxLogMessage("record_kb : undefined");
                return false;
        }

        for(auto& attr : *attrs_ref)
        {
            auto value = line_view.substr(attr.offset, attr.length);

            if(value.find_first_not_of(attr.char_includes) != std::string_view::npos)
            {
                if(attr.initial_value == nullptr || value != std::string_view(attr.initial_value)){
                    wxLogMessage("value.find_first_not_of(attr.char_includes) != std::string_view::npos");
                    wxLogMessage(wxString(wxT("invalid value : ") + std::string(value)));
                    return false;
                }
            }
        }

        block_ref->push_back(line);
    }

    return true;
}

FBParser& FBParser::set_current_part(FBPart part)
{
    if(part == FBPart::CURRENT) part = m_current_part;
    m_current_part = part;

    return *this;
}

FBPart FBParser::get_current_part() const
{
    return m_current_part;
}

FBParser& FBParser::set_newline(FBNewLine newline)
{
    if(newline == FBNewLine::CURRENT) newline = m_newline;
    m_newline = newline;

    return *this;
}

FBNewLine FBParser::get_newline() const
{
    return m_newline;
}

std::size_t FBParser::get_number_rows(FBPart part) const
{
    if(part == FBPart::CURRENT) part = m_current_part;
    auto& block = m_block_array.at((std::underlying_type_t<FBPart>)part);

    return block.size();
}

std::size_t FBParser::get_number_cols(FBPart part) const
{
    if(part == FBPart::CURRENT) part = m_current_part;
    auto& attrs = m_attrs_array.at((std::underlying_type_t<FBPart>)part);

    return attrs.size();
}

const char EMPTY_STRING [] = "";

std::string_view FBParser::get_value(std::size_t row, std::size_t col, FBPart part) const
{
    if(part == FBPart::CURRENT) part = m_current_part;

    auto& block = m_block_array.at((std::underlying_type_t<FBPart>)part);
    auto& attrs = m_attrs_array.at((std::underlying_type_t<FBPart>)part);

    if(row >= block.size() || col >= attrs.size())
    {
        wxLogMessage("row >= block.size() || col >= attrs.size()");
        return EMPTY_STRING;
    }
    
    auto& attr = attrs.at(col);

    std::string_view line(block.at(row).data());
    auto value = line.substr(attr.offset, attr.length);
    
    return value;
 
}

bool FBParser::set_value(std::size_t row, std::size_t col, std::string_view value, FBPart part)
{
    if(part == FBPart::CURRENT) part = m_current_part;

    auto& block = m_block_array.at((std::underlying_type_t<FBPart>)part);
    auto& attrs = m_attrs_array.at((std::underlying_type_t<FBPart>)part);

    if(row >= block.size() || col >= attrs.size())
    {
        wxLogMessage("row >= block.size() || col >= attrs.size()");
        return false;
    }

    auto& attr = attrs.at(col);
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

    auto& line = block.at(row);
    std::copy(value.begin(), value.end(), line.begin() + attr.offset);

    return true;
}

bool FBParser::assign_rows(std::size_t num, FBPart part)
{
    if(part == FBPart::CURRENT) part = m_current_part;

    auto& block = m_block_array.at((std::underlying_type_t<FBPart>)part);

    FBLine space_line;
    space_line.fill(' ');
    space_line.back() = '\0';

    block.assign(num, space_line);

    return true;
}

bool FBParser::append_rows(std::size_t num, FBPart part)
{
    if(part == FBPart::CURRENT) part = m_current_part;

    auto& block = m_block_array.at((std::underlying_type_t<FBPart>)part);

    FBLine space_line;
    space_line.fill(' ');
    space_line.back() = '\0';

    for(int i = 0; i < num; ++i)
    block.push_back(space_line);

    return true;
}

bool FBParser::insert_rows(std::size_t row, std::size_t num, FBPart part)
{
    if(part == FBPart::CURRENT) part = m_current_part;

    auto& block = m_block_array.at((std::underlying_type_t<FBPart>)part);

    auto first = block.begin() + row;

    if(first > block.end()){
        wxLogMessage("first > block.end()");
        return false;
    }

    FBLine space_line;
    space_line.fill(' ');
    space_line.back() = '\0';

    block.insert(first, num, space_line);

    return true;
}

bool FBParser::delete_rows(std::size_t row, std::size_t num, FBPart part)
{
    if(part == FBPart::CURRENT) part = m_current_part;

    auto& block = m_block_array.at((std::underlying_type_t<FBPart>)part);
    
    auto first = block.begin() + row;
    auto last = first + num;

    if(last > block.end()){
        wxLogMessage("last > block.end()");
        return false;
    }


    block.erase(first, last);

    return true;
}
