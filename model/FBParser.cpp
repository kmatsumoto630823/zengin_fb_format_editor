#include "FBParser.h"

#include <fstream>
#include <algorithm>
#include <wx/log.h>

consteval auto MAKE_SPACE_LINE()
{
    FBLine line;
    line.fill(' ');
    line.back() = '\0';

    return line;
}

constexpr auto SPACE_LINE = MAKE_SPACE_LINE();

FBGrid::FBGrid(const FBAttrs& attrs)
{
    m_attrs = attrs;
};

bool FBGrid::add_line(const FBLine& line)
{
    for(auto& attr : m_attrs)
    {
        std::string_view value(line.data() + attr.offset, attr.length);
        if(!attr.check_value(value))
        {
            wxLogMessage("check_value(attr, value) : false");
            return false;
        }
    }

    this->push_back(line);
    return true;
}

std::size_t FBGrid::get_number_rows() const
{
    return this->size();
}

std::size_t FBGrid::get_number_cols() const
{
    return m_attrs.size();
}

const std::string& FBGrid::get_value(std::size_t row, std::size_t col)
{
    if(row >= this->size() || col >= m_attrs.size())
    {
        wxLogMessage("row >= this->size() || col >= m_attrs.size()");
        m_stdstr_buff = "";

        return m_stdstr_buff;
    }
    
    auto& attr = m_attrs.at(col);
    auto& line = this->at(row);
    
    std::string_view str_view(line.data());
    m_stdstr_buff = str_view.substr(attr.offset, attr.length);
    
    return m_stdstr_buff;
}

bool FBGrid::set_value(std::size_t row, std::size_t col, const std::string& value)
{
    if(row >= this->size() || col >= m_attrs.size())
    {
        wxLogMessage("row >= this->size() || col >= m_attrs.size()");
        return false;
    }

    auto& attr = m_attrs.at(col);

    if(!attr.check_value(value))
    {
        wxLogMessage("attr.check_value(value) : false");
        return false;
    }

    auto& line = this->at(row);
    std::copy(value.begin(), value.end(), line.begin() + attr.offset);

    return true;
}

bool FBGrid::assign_rows(std::size_t num_rows)
{
    this->assign(num_rows, SPACE_LINE);
    return true;
}

bool FBGrid::append_rows(std::size_t num_rows)
{
    this->resize(size() + num_rows, SPACE_LINE);
    return true;
}

bool FBGrid::insert_rows(std::size_t row, std::size_t num_rows)
{
    auto first = this->begin() + row;

    if(first > this->end())
    {
        wxLogMessage("first > this->end()");
        return false;
    }

    this->insert(first, num_rows, SPACE_LINE);
    return true;
}

bool FBGrid::delete_rows(std::size_t row, std::size_t num_rows)
{   
    auto first = this->begin() + row;
    auto last = first + num_rows;

    if(last > this->end())
    {
        wxLogMessage("last > this->end()");
        return false;
    }

    this->erase(first, last);
    return true;
}

FBParser::FBParser(const FBAttrsArray& attrs_array) : 
m_grid_array
{
    attrs_array[(FBEnumInt)FBPart::HEADER],
    attrs_array[(FBEnumInt)FBPart::DATA],
    attrs_array[(FBEnumInt)FBPart::TRAILER],
    attrs_array[(FBEnumInt)FBPart::END]
}
{
    m_newline = FBNewLine::CRLF;
}

bool FBParser::open_file(const std::string& path)
{
    std::ifstream ifs(path, std::ios::binary);

    if(!ifs)
    {
        wxLogMessage("ifs : false");
        return false;
    }

    ifs.seekg(0, std::ios_base::end);
    auto file_size = ifs.tellg();
    if(file_size > 104'857'600)
    {
        wxLogMessage("file_size > 100MB");
        return false;
    }

    ifs.seekg(0, std::ios_base::beg);

    std::string text;
    text.reserve(file_size);
    auto ifs_it = std::istreambuf_iterator<char>(ifs);
    auto ifs_last = std::istreambuf_iterator<char>();
    text.assign(ifs_it, ifs_last);

    return from_text(text);
}

bool FBParser::save_file(const std::string& path)
{
    std::ofstream ofs(path, std::ios::binary);
    if(!ofs)
    {
        wxLogMessage("ofs : false");
        return false;
    }

    for(auto& grid : m_grid_array)
    {
        for(auto& line : grid)
        ofs << line.data() << FB_NEWLINE_CODE[(FBEnumInt)m_newline];
    }

    return true;
};

bool FBParser::from_text(const std::string& text)
{
    auto mod_text = text;

    for(auto& grid : m_grid_array) 
    {
        grid.clear();
    }

    std::string_view del_chars = "\r\n\t,";
    auto isDel = [del_chars](auto&& x)
    {
        return del_chars.find(x) != std::string_view::npos;
    };

    std::erase_if(mod_text, isDel);

    if(mod_text.size() % FB_LINE_WIDTH != 0)
    {
        wxLogMessage("text.size() mod FB_WIDTH != 0");
        return false;
    }

    std::size_t part_code = 0;
    for(decltype(mod_text.size()) offset = 0; offset < mod_text.size(); offset += FB_LINE_WIDTH)
    {
        auto line = SPACE_LINE;

        auto first = mod_text.begin() + offset;
        auto last = first + FB_LINE_WIDTH;
        std::copy(first, last, line.begin());

        std::size_t next_code = line.at(0) - '0';

        if(part_code > next_code)
        {
            wxLogMessage("part_code > next_code");
            return false;
        }

        part_code = next_code;
        FBPart part;

        switch(part_code)
        {
            case FB_PARTCODE_HEADER :
                part = FBPart::HEADER;
                break;

            case FB_PARTCODE_DATA :
                part = FBPart::DATA;
                break;
            
            case FB_PARTCODE_TRAILER :
                part = FBPart::TRAILER;
                break;

            case FB_PARTCODE_END :
                part = FBPart::END;
                break;

            default:
                wxLogMessage("part_code : undefined");
                return false;                
        }

        auto& grid = m_grid_array.at((FBEnumInt)part);

        if(!grid.add_line(line))
        {
            wxLogMessage("grid.add_line(line) : false");
            return false;
        }

    }

    return true;
}

FBParser& FBParser::set_newline(FBNewLine newline)
{
    m_newline = newline;
    return *this;
}

FBNewLine FBParser::get_newline() const
{
    return m_newline;
}