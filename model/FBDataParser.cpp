#include "FBDataParser.h"

#include <fstream>
#include <algorithm>
#include <wx/log.h>

constexpr std::size_t FB_WIDTH = 120;
constexpr std::size_t FB_HEADER_KB  = 1;
constexpr std::size_t FB_DATA_KB    = 2;
constexpr std::size_t FB_TRAILER_KB = 8;
constexpr std::size_t FB_END_KB     = 9;

constexpr char fb_char_includes_num []   = "0123456789";
constexpr char fb_pad_num []  = "L0";

constexpr char fb_char_includes_kana []  = " ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ�����������������������������������������������";
constexpr char fb_pad_kana [] = "R ";

constexpr char fb_char_includes_dummy []  = " ";
constexpr char fb_pad_dummy [] = "R ";

const FBInfo FBDataParser::fb_header_info_sohfuri =
{
    std::make_tuple( 0, "�f�[�^�敪",     0,  1, "1", "L1", nullptr, "1�F�w�b�_�[���R�[�h�i�Œ�j"),
    std::make_tuple( 1, "��ʃR�[�h",     1,  2, fb_char_includes_num,   fb_pad_num,   nullptr,   "21�F�����U��"),
    std::make_tuple( 2, "�R�[�h�敪",     3,  1, fb_char_includes_num,   fb_pad_num,   nullptr,   "0�FJIS\r\n1�FEBCDIC"),
    std::make_tuple( 3, "�˗��l�R�[�h",   4, 10, fb_char_includes_num,   fb_pad_num,   nullptr,   "�U���˗��l���ʂ̂��ߋ�s���̔Ԃ����R�[�h"),
    std::make_tuple( 4, "�˗��l��",      14, 40, fb_char_includes_kana,  fb_pad_kana,  nullptr,   nullptr),
    std::make_tuple( 5, "��g��",        54,  4, fb_char_includes_num,   fb_pad_num,   nullptr,   "MM��DD��"),
    std::make_tuple( 6, "�d����s�ԍ�",  58,  4, fb_char_includes_num,   fb_pad_num,   nullptr,   nullptr),
    std::make_tuple( 7, "�d����s��",    62, 15, fb_char_includes_kana,  fb_pad_kana,  nullptr,   nullptr),
    std::make_tuple( 8, "�d���x�X�ԍ�",  77,  3, fb_char_includes_num,   fb_pad_num,   nullptr,   nullptr),
    std::make_tuple( 9, "�d���x�X��",    80, 15, fb_char_includes_kana,  fb_pad_kana,  nullptr,   nullptr),
    std::make_tuple(10, "�a�����",      95,  1, fb_char_includes_num,   fb_pad_num,   " ",       "1�F���ʗa��\r\n2�F�����a��\r\n9�F���̑�"),
    std::make_tuple(11, "�����ԍ�",      96,  7, fb_char_includes_num,   fb_pad_num,   "       ", nullptr),
    std::make_tuple(12, "�_�~�[",       103, 17, fb_char_includes_dummy, fb_pad_dummy, nullptr,   nullptr)
};

const FBInfo FBDataParser::fb_data_info_sohfuri =
{
    std::make_tuple( 0, "�f�[�^�敪",      0,  1, "2", "L2", nullptr, "2�F�f�[�^���R�[�h�i�Œ�j"),
    std::make_tuple( 1, "��s�ԍ�",        1,  4, fb_char_includes_num,   fb_pad_num,   nullptr, nullptr),
    std::make_tuple( 2, "��s��",          5, 15, fb_char_includes_kana,  fb_pad_kana,  nullptr, nullptr),
    std::make_tuple( 3, "�x�X�ԍ�",       20,  3, fb_char_includes_num,   fb_pad_num,   nullptr, nullptr),
    std::make_tuple( 4, "�x�X��",         23, 15, fb_char_includes_kana,  fb_pad_kana,  nullptr, nullptr),
    std::make_tuple( 5, "��`�������ԍ�", 38,  4, fb_char_includes_num,   fb_pad_num,   "    ",  nullptr),
    std::make_tuple( 6, "�a�����",       42,  1, fb_char_includes_num,   fb_pad_num,   nullptr, "1�F���ʗa��\r\n2�F�����a��\r\n4�F���~�a��\r\n9�F���̑�"),
    std::make_tuple( 7, "�����ԍ�",       43,  7, fb_char_includes_num,   fb_pad_num,   nullptr, nullptr),
    std::make_tuple( 8, "���l��",       50, 30, fb_char_includes_kana,  fb_pad_kana,  nullptr, nullptr),
    std::make_tuple( 9, "�U�����z",       80, 10, fb_char_includes_num,   fb_pad_num,   nullptr, nullptr),
    std::make_tuple(10, "�V�K�R�[�h",     90,  1, fb_char_includes_num,   fb_pad_num,   " ",     "1�F��P��U����\r\n2�F�ύX���i��s�E�x�X�A�a����ځE�����ԍ��j\r\n0�F���̑�"),
    std::make_tuple(11, "�ڋq�R�[�h1",    91, 10, fb_char_includes_kana,  fb_pad_kana,  nullptr, nullptr),
    std::make_tuple(12, "�ڋq�R�[�h2",   101, 10, fb_char_includes_kana,  fb_pad_kana,  nullptr, nullptr),
    std::make_tuple(13, "�U���敪",      111,  1, fb_char_includes_num,   fb_pad_num,   " ",     "7�F�e���U��\r\n8�F�����U��"),
    std::make_tuple(14, "���ʕ\\��",     112,  1, fb_char_includes_kana,  fb_pad_kana,  nullptr, "�uY�v�܂��̓X�y�[�X\r\n�uY�v�̏ꍇ�A�u�ڋq�R�[�h1�v�u�ڋq�R�[�h2�v�́uEDI���v�ƂȂ�"),
    std::make_tuple(15, "�_�~�[",        113,  7, fb_char_includes_dummy, fb_pad_dummy, nullptr, nullptr)
};

const FBInfo FBDataParser::fb_trailer_info_sohfuri =
{
    std::make_tuple( 0, "�f�[�^�敪",  0,   1, "8", "L8", nullptr,  "8�F�g���[���[���R�[�h�i�Œ�j"),
    std::make_tuple( 1, "���v����",    1,   6, fb_char_includes_num,   fb_pad_num,   nullptr, nullptr),
    std::make_tuple( 2, "���v���z",    7,  12, fb_char_includes_num,   fb_pad_num,   nullptr, nullptr),
    std::make_tuple( 3, "�_�~�[",     19, 101, fb_char_includes_dummy, fb_pad_dummy, nullptr, nullptr)
};

const FBInfo FBDataParser::fb_end_info_sohfuri =
{
    std::make_tuple( 0, "�f�[�^�敪", 0,   1, "9", "L9", nullptr, "9�F�G���h���R�[�h�i�Œ�j"),
    std::make_tuple( 1, "�_�~�[",     1, 119, fb_char_includes_dummy, fb_pad_dummy, nullptr, nullptr)
};

FBDataParser::FBDataParser() : m_null_str("")
{
    m_newline_code = "\r\n";

    for(auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] : FBDataParser::fb_header_info){
        if(initial_value != nullptr){
            if(length != std::string_view(initial_value).length()){
                wxLogMessage("length != std::string_view(initial_value).length()");
            }
        }
    }

    for(auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] : FBDataParser::fb_data_info){
        if(initial_value != nullptr){
            if(length != std::string_view(initial_value).length()){
                wxLogMessage("length != std::string_view(initial_value).length()");
            }
        }
    }

    for(auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] : FBDataParser::fb_trailer_info){
        if(initial_value != nullptr){
            if(length != std::string_view(initial_value).length()){
                wxLogMessage("length != std::string_view(initial_value).length()");
            }
        }
    }

    for(auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] : FBDataParser::fb_end_info){
        if(initial_value != nullptr){
            if(length != std::string_view(initial_value).length()){
                wxLogMessage("length != std::string_view(initial_value).length()");
            }
        }
    }

    fb_header_info = fb_header_info_sohfuri;
    fb_data_info = fb_data_info_sohfuri;
    fb_trailer_info = fb_trailer_info_sohfuri;
    fb_end_info = fb_end_info_sohfuri;
}

FBDataParser::~FBDataParser()
{
}

bool FBDataParser::open_fb_file(const std::string &path)
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

    fb_header_lines.clear();
    fb_data_lines.clear();
    fb_trailer_lines.clear();
    fb_end_lines.clear();


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

        FBInfo *fb_info_ref;
        FBLines *fb_lines_ref;

        switch(record_kb)
        {
            case FB_HEADER_KB:
                fb_info_ref = &fb_header_info;
                fb_lines_ref = &fb_header_lines;
                break;

            case FB_DATA_KB:
                fb_info_ref = &fb_data_info;
                fb_lines_ref = &fb_data_lines;
                break;
            
            case FB_TRAILER_KB:
                fb_info_ref = &fb_trailer_info;
                fb_lines_ref = &fb_trailer_lines;
                break;

            case FB_END_KB:
                fb_info_ref = &fb_end_info;
                fb_lines_ref = &fb_end_lines;
                break;

            default:
                wxLogMessage("record_kb : undefined");
                return false;
        }

        for(auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] : *fb_info_ref)
        {
            auto fb_str_sub = fb_str_line.substr(offset, length);


            

            if(fb_str_sub.find_first_not_of(char_includes) != std::string_view::npos)
            {
                if(initial_value == nullptr || fb_str_sub != std::string_view(initial_value)){
                    wxLogMessage("fb_str_sub.find_first_not_of(char_includes) != std::string_view::npos");
                    return false;
                }
            }
        }

        fb_lines_ref->emplace_back(fb_str_line);
    }

    return true;
}

bool FBDataParser::save_fb_file(const std::string &path)
{
    std::ofstream fb_ofs(path, std::ios::binary);
    if(!fb_ofs)
    {
        wxLogMessage("fb_ofs : false");
        return false;
    }
    
    for(const auto & line : fb_header_lines )fb_ofs << line << m_newline_code;
    for(const auto & line : fb_data_lines   )fb_ofs << line << m_newline_code;
    for(const auto & line : fb_trailer_lines)fb_ofs << line << m_newline_code;
    for(const auto & line : fb_end_lines    )fb_ofs << line << m_newline_code;

    return true;
};

bool FBDataParser::set_fb_newline_code(std::string_view newline_code)
{
    m_newline_code = newline_code;
    return true;
}

std::string_view FBDataParser::get_fb_newline_code()
{
    return m_newline_code;
}


std::size_t FBDataParser::get_fb_row_size(FBLines& fb_lines, FBInfo& fb_info)
{
    return fb_lines.size();
}

bool FBDataParser::assign_fb_line(std::size_t num, FBLines& fb_lines, FBInfo& fb_info)
{
    fb_lines.assign(num, std::string(FB_WIDTH, ' '));
    return true;
}


std::string_view FBDataParser::get_fb_value(std::size_t row, std::size_t col, FBLines& fb_lines, FBInfo& fb_info)
{
    if(row >= fb_lines.size() || col >= fb_info.size())
    {
        wxLogMessage("row >= fb_lines.size() || col >= fb_info.size()");
        return m_null_str;
    }
    
    auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] = fb_info.at(col);

    std::string_view line = fb_lines.at(row);
    auto value = line.substr(offset, length);
    
    return value;
 
}

bool FBDataParser::set_fb_value(std::size_t row, std::size_t col, std::string_view value, FBLines& fb_lines, FBInfo& fb_info)
{
    if(row >= fb_lines.size() || col >= fb_info.size())
    {
        wxLogMessage("row >= fb_lines.size() || col >= fb_info.size()");
        return false;
    }

    auto &[num, label, offset, length, char_includes, pad_info, initial_value, description] = fb_info.at(col);
    if(value.size() != length)
    {
        wxLogMessage("value.size() != length");
        return false;
    }


    if(value.find_first_not_of(char_includes) != std::string_view::npos)
    {
        if(initial_value == nullptr || value != std::string_view(initial_value))
        {
            wxLogMessage("value.find_first_not_of(char_includes) != std::string_view::npos");
            return false;
        }
    }

    auto &line = fb_lines.at(row);
    std::copy(value.begin(), value.end(), line.begin() + offset);

    return true;
}