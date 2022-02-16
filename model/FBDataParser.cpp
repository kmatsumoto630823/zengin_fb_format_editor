#include "FBDataParser.h"

#include <fstream>
#include <algorithm>
#include <wx/log.h>

constexpr std::size_t FB_WIDTH = 120;
constexpr std::size_t FB_HEADER_KB  = 1;
constexpr std::size_t FB_DATA_KB    = 2;
constexpr std::size_t FB_TRAILER_KB = 8;
constexpr std::size_t FB_END_KB     = 9;

constexpr char char_num []  = "0123456789";
constexpr char pad_num []   = "L0";

constexpr char char_kana []  = " ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ�����������������������������������������������";
constexpr char pad_kana []   = "R ";

constexpr char char_dummy []  = " ";
constexpr char pad_dummy []   = "R ";

const FBAttrs FBDataParser::fb_header_attrs_sohfuri =
{
    { 0, "�f�[�^�敪",     0,  1, "1",        "L1",      nullptr,   "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�P�v�F�w�b�_�[���R�[�h"},
    { 1, "��ʃR�[�h",     1,  2, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�Q�j\r\n�K�{����\r\n�u�Q�P�v�F�����U��"},
    { 2, "�R�[�h�敪",     3,  1, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�O�v�F�r�i�h�r\r\n�u�P�v�F�d�a�b�c�h�b"},
    { 3, "�˗��l�R�[�h",   4, 10, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�P�O�j\r\n�K�{����\r\n�U���˗��l���ʂ̂��ߋ�s���̔Ԃ����R�[�h"},
    { 4, "�˗��l��",      14, 40, char_kana,  pad_kana,  nullptr,   "������i���j�F�b�i�S�O�j\r\n�K�{����"},
    { 5, "��g��",        54,  4, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�S�j\r\n�K�{����\r\n�l�l�c�c�i�l�l���c�c���j"},
    { 6, "�d����s�ԍ�",  58,  4, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�S�j\r\n�K�{����\r\n������Z�@�֔ԍ�"},
    { 7, "�d����s��",    62, 15, char_kana,  pad_kana,  nullptr,   "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    { 8, "�d���x�X�ԍ�",  77,  3, char_num,   pad_num,   nullptr,   "������i���j�F�m�i�R�j\r\n�K�{����\r\n����X�ԍ�"},
    { 9, "�d���x�X��",    80, 15, char_kana,  pad_kana,  nullptr,   "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    {10, "�a�����",      95,  1, char_num,   pad_num,   " ",       "������i���j�F�m�i�P�j\r\n�C�Ӎ���\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�X�v�F���̑�"},
    {11, "�����ԍ�",      96,  7, char_num,   pad_num,   "       ", "������i���j�F�m�i�V�j\r\n�C�Ӎ���"},
    {12, "�_�~�[",       103, 17, char_dummy, pad_dummy, nullptr,   "������i���j�F�b�i�P�V�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

const FBAttrs FBDataParser::fb_data_attrs_sohfuri =
{
    { 0, "�f�[�^�敪",      0,  1, "2",        "L2",      nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�Q�v�F�f�[�^�[���R�[�h"},
    { 1, "��s�ԍ�",        1,  4, char_num,   pad_num,   nullptr, "������i���j�F�m�i�S�j\r\n�K�{����\r\n������Z�@�֔ԍ�"},
    { 2, "��s��",          5, 15, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    { 3, "�x�X�ԍ�",       20,  3, char_num,   pad_num,   nullptr, "������i���j�F�m�i�R�j\r\n�K�{����\r\n����X�ԍ�"},
    { 4, "�x�X��",         23, 15, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�P�T�j\r\n�C�Ӎ���"},
    { 5, "��`�������ԍ�", 38,  4, char_num,   pad_num,   "    ",  "������i���j�F�m�i�S�j\r\n �C�Ӎ���\r\n�����`�������ԍ�"},
    { 6, "�a�����",       42,  1, char_num,   pad_num,   nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�P�v�F���ʗa��\r\n�u�Q�v�F�����a��\r\n�u�S�v�F���~�a��\r\n�u�X�v�F���̑�"},
    { 7, "�����ԍ�",       43,  7, char_num,   pad_num,   nullptr, "������i���j�F�m�i�V�j\r\n�K�{����"},
    { 8, "���l��",       50, 30, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�R�O�j\r\n�K�{����"},
    { 9, "�U�����z",       80, 10, char_num,   pad_num,   nullptr, "������i���j�F�m�i�P�O�j\r\n�K�{����"},
    {10, "�V�K�R�[�h",     90,  1, char_num,   pad_num,   " ",     "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�P�v�F��P��U����\r\n�u�Q�v�F�ύX���i��s�E�x�X�A�a����ځE�����ԍ��j\r\n�u�O�v�F���̑�"},
    {11, "�ڋq�R�[�h1",    91, 10, char_kana,  pad_kana,  nullptr, "������i���j�F�m�i�P0�j\r\n�C�Ӎ���\r\n�˗��l����߂����l���ʂ̂��߂̌ڋq�R�[�h"},
    {12, "�ڋq�R�[�h2",   101, 10, char_kana,  pad_kana,  nullptr, "������i���j�F�m�i�P0�j\r\n�C�Ӎ���\r\n�˗��l����߂����l���ʂ̂��߂̌ڋq�R�[�h"},
    {13, "�U���敪",      111,  1, char_num,   pad_num,   " ",     "������i���j�F�m�i�P�j\r\n�C�Ӎ���\r\n�u�V�v�F�e���U��\r\n�u�W�v�F�����U��"},
    {14, "���ʕ\\��",     112,  1, char_kana,  pad_kana,  nullptr, "������i���j�F�b�i�P�j\r\n�C�Ӎ���\r\n�u�x�v�܂��̓X�y�[�X\r\n�u�x�v�̏ꍇ�A�u�ڋq�R�[�h�P�v�u�ڋq�R�[�h�Q�v�́uEDI���v�ƂȂ�"},
    {15, "�_�~�[",        113,  7, char_dummy, pad_dummy, nullptr, "������i���j�F�b�i�V�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

const FBAttrs FBDataParser::fb_trailer_attrs_sohfuri =
{
    { 0, "�f�[�^�敪",  0,   1, "8",        "L8",      nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�W�v�F�g���[���[���R�[�h"},
    { 1, "���v����",    1,   6, char_num,   pad_num,   nullptr, "������i���j�F�m�i�U�j\r\n�K�{����"},
    { 2, "���v���z",    7,  12, char_num,   pad_num,   nullptr, "������i���j�F�m�i�P�Q�j\r\n�K�{����"},
    { 3, "�_�~�[",     19, 101, char_dummy, pad_dummy, nullptr, "������i���j�F�b�i�P�O�P�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

const FBAttrs FBDataParser::fb_end_attrs_sohfuri =
{
    { 0, "�f�[�^�敪", 0,   1, "9", "L9", nullptr, "������i���j�F�m�i�P�j\r\n�K�{����\r\n�u�X�v�F�G���h���R�[�h"},
    { 1, "�_�~�[",     1, 119, char_dummy, pad_dummy, nullptr, "������i���j�F�b�i�P�P�X�j\r\n�K�{����\r\n�X�y�[�X�Ƃ���"}
};

FBDataParser::FBDataParser() : m_null_str("")
{
    m_newline_code = "\r\n";

    for(const auto &attr : FBDataParser::fb_header_attrs){
        if(attr.initial_value != nullptr){
            if(attr.length != std::string_view(attr.initial_value).length()){
                wxLogMessage("length != std::string_view(initial_value).length()");
            }
        }
    }

    for(const auto &attr : FBDataParser::fb_data_attrs){
        if(attr.initial_value != nullptr){
            if(attr.length != std::string_view(attr.initial_value).length()){
                wxLogMessage("length != std::string_view(initial_value).length()");
            }
        }
    }

    for(const auto &attr : FBDataParser::fb_trailer_attrs){
        if(attr.initial_value != nullptr){
            if(attr.length != std::string_view(attr.initial_value).length()){
                wxLogMessage("length != std::string_view(initial_value).length()");
            }
        }
    }

    for(const auto &attr : FBDataParser::fb_end_attrs){
        if(attr.initial_value != nullptr){
            if(attr.length != std::string_view(attr.initial_value).length()){
                wxLogMessage("length != std::string_view(initial_value).length()");
            }
        }
    }

    fb_header_attrs = fb_header_attrs_sohfuri;
    fb_data_attrs = fb_data_attrs_sohfuri;
    fb_trailer_attrs = fb_trailer_attrs_sohfuri;
    fb_end_attrs = fb_end_attrs_sohfuri;
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

        FBAttrs *fb_attrs_ref;
        FBLines *fb_lines_ref;

        switch(record_kb)
        {
            case FB_HEADER_KB:
                fb_attrs_ref = &fb_header_attrs;
                fb_lines_ref = &fb_header_lines;
                break;

            case FB_DATA_KB:
                fb_attrs_ref = &fb_data_attrs;
                fb_lines_ref = &fb_data_lines;
                break;
            
            case FB_TRAILER_KB:
                fb_attrs_ref = &fb_trailer_attrs;
                fb_lines_ref = &fb_trailer_lines;
                break;

            case FB_END_KB:
                fb_attrs_ref = &fb_end_attrs;
                fb_lines_ref = &fb_end_lines;
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


std::size_t FBDataParser::get_fb_row_size(FBLines& fb_lines, FBAttrs& fb_attrs)
{
    return fb_lines.size();
}

bool FBDataParser::assign_fb_line(std::size_t num, FBLines& fb_lines, FBAttrs& fb_attrs)
{
    fb_lines.assign(num, std::string(FB_WIDTH, ' '));
    return true;
}


std::string_view FBDataParser::get_fb_value(std::size_t row, std::size_t col, FBLines& fb_lines, FBAttrs& fb_attrs)
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

bool FBDataParser::set_fb_value(std::size_t row, std::size_t col, std::string_view value, FBLines& fb_lines, FBAttrs& fb_attrs)
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