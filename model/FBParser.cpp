#include "FBParser.h"

#include <fstream>
#include <algorithm>
#include <wx/log.h>

FBParser::FBParser()
{
    set_fbtype();
    set_fbpart();
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
    if(file_size > 104857600)
    {
        wxLogMessage("file_size > 104857600");
        return false;
    }

    fb_ifs.seekg(0, std::ios_base::beg);

    for(auto& grid : m_grid_array)grid.clear();

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

        FBAttrs* attrs_ref;
        FBGrid* grid_ref;

        switch(record_kb)
        {
            case FB_PART_HEADER :
                attrs_ref = &m_attrs_array.at((int)FBPart::HEADER);
                grid_ref = &m_grid_array.at((int)FBPart::HEADER);
                break;

            case FB_PART_DATA :
                attrs_ref = &m_attrs_array.at((int)FBPart::DATA);
                grid_ref = &m_grid_array.at((int)FBPart::DATA);
                break;
            
            case FB_PART_TRAILER :
                attrs_ref = &m_attrs_array.at((int)FBPart::TRAILER);
                grid_ref = &m_grid_array.at((int)FBPart::TRAILER);
                break;

            case FB_PART_END :
                attrs_ref = &m_attrs_array.at((int)FBPart::END);
                grid_ref = &m_grid_array.at((int)FBPart::END);
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

        grid_ref->push_back(line);
    }

    return true;
}

bool FBParser::save_file(const std::string& path)
{
    std::ofstream fb_ofs(path, std::ios::binary);
    if(!fb_ofs)
    {
        wxLogMessage("fb_ofs : false");
        return false;
    }

    for(auto& grid : m_grid_array)
    {
        for(auto& line : grid)
        fb_ofs << std::string_view(line.data()) << FB_NEWLINE_CODE[m_newline];
    }


    return true;
};

FBParser& FBParser::set_fbtype(FBType type, std::string_view chars_kana)
{
    if(type == FBType::CURRENT) type = m_fbtype;

    switch(type)
    {
        case FBType::SOHFURI:
            m_attrs_array = ATTRS_ARRAY_SOHFURI;          
            break;

        case FBType::KYUYO_SHOYO:
            m_attrs_array = ATTRS_ARRAY_KYUYO_SHOYO;  
            break;

        case FBType::FURIKAE:
            m_attrs_array = ATTRS_ARRAY_FURIKAE;  
            break;

        default:
            return *this;
    }

    m_fbtype = type;

    m_chars_kana = chars_kana;
    if(m_chars_kana.empty()) return *this;
    
    for(auto& attrs : m_attrs_array)
    {
        for(auto& attr : attrs)
        if(attr.char_includes == CHARS_KANA) attr.char_includes = m_chars_kana.data();
    }

    return *this;
}

FBType FBParser::get_fbtype()
{
    return m_fbtype;
}

FBParser& FBParser::set_fbpart(FBPart part)
{
    if(part == FBPart::CURRENT) part = m_fbpart;
    m_fbpart = part;

    return *this;
}

FBPart FBParser::get_fbpart()
{
    return m_fbpart;
}

FBParser& FBParser::set_newline(FBNewLine newline)
{
    if(newline == FBNewLine::CURRENT) newline = m_newline;
    m_newline = newline;

    return *this;
}

FBNewLine FBParser::get_newline()
{
    return m_newline;
}

const FBAttrs& FBParser::get_attrs(FBPart part)
{
    if(part == FBPart::CURRENT) part = m_fbpart;
    return m_attrs_array.at((int)part);
}

const FBAttrsArray& FBParser::get_attrs_array()
{
    return m_attrs_array;
}


std::size_t FBParser::get_number_rows(FBPart part)
{
    if(part == FBPart::CURRENT) part = m_fbpart;
    auto& grid = m_grid_array.at((int)part);

    return grid.size();
}

std::size_t FBParser::get_number_cols(FBPart part)
{
    if(part == FBPart::CURRENT) part = m_fbpart;
    auto& attrs = m_attrs_array.at((int)part);

    return attrs.size();
}

const char EMPTY_STRING [] = "";

std::string_view FBParser::get_value(std::size_t row, std::size_t col, FBPart part)
{
    if(part == FBPart::CURRENT) part = m_fbpart;

    auto& grid = m_grid_array.at((int)part);
    auto& attrs = m_attrs_array.at((int)part);

    if(row >= grid.size() || col >= attrs.size())
    {
        wxLogMessage("row >= grid.size() || col >= attrs.size()");
        return EMPTY_STRING;
    }
    
    auto& attr = attrs.at(col);

    std::string_view line(grid.at(row).data());
    auto value = line.substr(attr.offset, attr.length);
    
    return value;
 
}

bool FBParser::set_value(std::size_t row, std::size_t col, std::string_view value, FBPart part)
{
    if(part == FBPart::CURRENT) part = m_fbpart;

    auto& grid = m_grid_array.at((int)part);
    auto& attrs = m_attrs_array.at((int)part);

    if(row >= grid.size() || col >= attrs.size())
    {
        wxLogMessage("row >= grid.size() || col >= attrs.size()");
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

    auto& line = grid.at(row);
    std::copy(value.begin(), value.end(), line.begin() + attr.offset);

    return true;
}

bool FBParser::assign_rows(std::size_t num, FBPart part)
{
    if(part == FBPart::CURRENT) part = m_fbpart;

    auto& grid = m_grid_array.at((int)part);

    FBLine space_line;
    space_line.fill(' ');
    space_line.back() = '\0';

    grid.assign(num, space_line);

    return true;
}

bool FBParser::append_rows(std::size_t num, FBPart part)
{
    if(part == FBPart::CURRENT) part = m_fbpart;

    auto& grid = m_grid_array.at((int)part);

    FBLine space_line;
    space_line.fill(' ');
    space_line.back() = '\0';

    for(int i = 0; i < num; ++i)
    grid.push_back(space_line);

    return true;
}

bool FBParser::insert_rows(std::size_t row, std::size_t num, FBPart part)
{
    if(part == FBPart::CURRENT) part = m_fbpart;

    auto& grid = m_grid_array.at((int)part);

    auto first = grid.begin() + row;

    if(first > grid.end()){
        wxLogMessage("first > grid.end()");
        return false;
    }

    FBLine space_line;
    space_line.fill(' ');
    space_line.back() = '\0';

    grid.insert(first, num, space_line);

    return true;
}

bool FBParser::delete_rows(std::size_t row, std::size_t num, FBPart part)
{
    if(part == FBPart::CURRENT) part = m_fbpart;

    auto& grid = m_grid_array.at((int)part);
    
    auto first = grid.begin() + row;
    auto last = first + num;

    if(last > grid.end()){
        wxLogMessage("last > grid.end()");
        return false;
    }


    grid.erase(first, last);


    return true;
}
