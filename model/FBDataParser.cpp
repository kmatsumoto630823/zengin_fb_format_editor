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

constexpr char char_kana []  = " ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß";
constexpr char pad_kana []   = "R ";

constexpr char char_dummy []  = " ";
constexpr char pad_dummy []   = "R ";

const FBAttrs FBDataParser::fb_header_attrs_sohfuri =
{
    { 0, "ƒf[ƒ^‹æ•ª",     0,  1, "1",        "L1",      nullptr,   "•¶ŽšŽíiŒ…jF‚mi‚Pj\r\n•K{€–Ú\r\nu‚PvFƒwƒbƒ_[ƒŒƒR[ƒh"},
    { 1, "Ží•ÊƒR[ƒh",     1,  2, char_num,   pad_num,   nullptr,   "•¶ŽšŽíiŒ…jF‚mi‚Qj\r\n•K{€–Ú\r\nu‚Q‚PvF‘‡Už"},
    { 2, "ƒR[ƒh‹æ•ª",     3,  1, char_num,   pad_num,   nullptr,   "•¶ŽšŽíiŒ…jF‚mi‚Pj\r\n•K{€–Ú\r\nu‚OvF‚r‚i‚h‚r\r\nu‚PvF‚d‚a‚b‚c‚h‚b"},
    { 3, "ˆË—ŠlƒR[ƒh",   4, 10, char_num,   pad_num,   nullptr,   "•¶ŽšŽíiŒ…jF‚mi‚P‚Oj\r\n•K{€–Ú\r\nUžˆË—ŠlŽ¯•Ê‚Ì‚½‚ß‹âs‚ªÌ”Ô‚µ‚½ƒR[ƒh"},
    { 4, "ˆË—Šl–¼",      14, 40, char_kana,  pad_kana,  nullptr,   "•¶ŽšŽíiŒ…jF‚bi‚S‚Oj\r\n•K{€–Ú"},
    { 5, "Žæ‘g“ú",        54,  4, char_num,   pad_num,   nullptr,   "•¶ŽšŽíiŒ…jF‚mi‚Sj\r\n•K{€–Ú\r\n‚l‚l‚c‚ci‚l‚lŒŽ‚c‚c“új"},
    { 6, "ŽdŒü‹âs”Ô†",  58,  4, char_num,   pad_num,   nullptr,   "•¶ŽšŽíiŒ…jF‚mi‚Sj\r\n•K{€–Ú\r\n“ˆê‹à—Z‹@ŠÖ”Ô†"},
    { 7, "ŽdŒü‹âs–¼",    62, 15, char_kana,  pad_kana,  nullptr,   "•¶ŽšŽíiŒ…jF‚bi‚P‚Tj\r\n”CˆÓ€–Ú"},
    { 8, "ŽdŒüŽx“X”Ô†",  77,  3, char_num,   pad_num,   nullptr,   "•¶ŽšŽíiŒ…jF‚mi‚Rj\r\n•K{€–Ú\r\n“ˆê“X”Ô†"},
    { 9, "ŽdŒüŽx“X–¼",    80, 15, char_kana,  pad_kana,  nullptr,   "•¶ŽšŽíiŒ…jF‚bi‚P‚Tj\r\n”CˆÓ€–Ú"},
    {10, "—a‹àŽí–Ú",      95,  1, char_num,   pad_num,   " ",       "•¶ŽšŽíiŒ…jF‚mi‚Pj\r\n”CˆÓ€–Ú\r\nu‚PvF•’Ê—a‹à\r\nu‚QvF“–À—a‹à\r\nu‚XvF‚»‚Ì‘¼"},
    {11, "ŒûÀ”Ô†",      96,  7, char_num,   pad_num,   "       ", "•¶ŽšŽíiŒ…jF‚mi‚Vj\r\n”CˆÓ€–Ú"},
    {12, "ƒ_ƒ~[",       103, 17, char_dummy, pad_dummy, nullptr,   "•¶ŽšŽíiŒ…jF‚bi‚P‚Vj\r\n•K{€–Ú\r\nƒXƒy[ƒX‚Æ‚·‚é"}
};

const FBAttrs FBDataParser::fb_data_attrs_sohfuri =
{
    { 0, "ƒf[ƒ^‹æ•ª",      0,  1, "2",        "L2",      nullptr, "•¶ŽšŽíiŒ…jF‚mi‚Pj\r\n•K{€–Ú\r\nu‚QvFƒf[ƒ^[ƒŒƒR[ƒh"},
    { 1, "‹âs”Ô†",        1,  4, char_num,   pad_num,   nullptr, "•¶ŽšŽíiŒ…jF‚mi‚Sj\r\n•K{€–Ú\r\n“ˆê‹à—Z‹@ŠÖ”Ô†"},
    { 2, "‹âs–¼",          5, 15, char_kana,  pad_kana,  nullptr, "•¶ŽšŽíiŒ…jF‚bi‚P‚Tj\r\n”CˆÓ€–Ú"},
    { 3, "Žx“X”Ô†",       20,  3, char_num,   pad_num,   nullptr, "•¶ŽšŽíiŒ…jF‚mi‚Rj\r\n•K{€–Ú\r\n“ˆê“X”Ô†"},
    { 4, "Žx“X–¼",         23, 15, char_kana,  pad_kana,  nullptr, "•¶ŽšŽíiŒ…jF‚bi‚P‚Tj\r\n”CˆÓ€–Ú"},
    { 5, "ŽèŒ`ŒðŠ·Š”Ô†", 38,  4, char_num,   pad_num,   "    ",  "•¶ŽšŽíiŒ…jF‚mi‚Sj\r\n ”CˆÓ€–Ú\r\n“ˆêŽèŒ`ŒðŠ·Š”Ô†"},
    { 6, "—a‹àŽí–Ú",       42,  1, char_num,   pad_num,   nullptr, "•¶ŽšŽíiŒ…jF‚mi‚Pj\r\n•K{€–Ú\r\nu‚PvF•’Ê—a‹à\r\nu‚QvF“–À—a‹à\r\nu‚SvF’™’~—a‹à\r\nu‚XvF‚»‚Ì‘¼"},
    { 7, "ŒûÀ”Ô†",       43,  7, char_num,   pad_num,   nullptr, "•¶ŽšŽíiŒ…jF‚mi‚Vj\r\n•K{€–Ú"},
    { 8, "ŽóŽæl–¼",       50, 30, char_kana,  pad_kana,  nullptr, "•¶ŽšŽíiŒ…jF‚bi‚R‚Oj\r\n•K{€–Ú"},
    { 9, "Už‹àŠz",       80, 10, char_num,   pad_num,   nullptr, "•¶ŽšŽíiŒ…jF‚mi‚P‚Oj\r\n•K{€–Ú"},
    {10, "V‹KƒR[ƒh",     90,  1, char_num,   pad_num,   " ",     "•¶ŽšŽíiŒ…jF‚mi‚Pj\r\n•K{€–Ú\r\nu‚PvF‘æ‚P‰ñUž•ª\r\nu‚QvF•ÏX•ªi‹âsEŽx“XA—a‹àŽí–ÚEŒûÀ”Ô†j\r\nu‚OvF‚»‚Ì‘¼"},
    {11, "ŒÚ‹qƒR[ƒh1",    91, 10, char_kana,  pad_kana,  nullptr, "•¶ŽšŽíiŒ…jF‚mi‚P0j\r\n”CˆÓ€–Ú\r\nˆË—Šl‚ª’è‚ß‚½ŽóŽælŽ¯•Ê‚Ì‚½‚ß‚ÌŒÚ‹qƒR[ƒh"},
    {12, "ŒÚ‹qƒR[ƒh2",   101, 10, char_kana,  pad_kana,  nullptr, "•¶ŽšŽíiŒ…jF‚mi‚P0j\r\n”CˆÓ€–Ú\r\nˆË—Šl‚ª’è‚ß‚½ŽóŽælŽ¯•Ê‚Ì‚½‚ß‚ÌŒÚ‹qƒR[ƒh"},
    {13, "Už‹æ•ª",      111,  1, char_num,   pad_num,   " ",     "•¶ŽšŽíiŒ…jF‚mi‚Pj\r\n”CˆÓ€–Ú\r\nu‚VvFƒeƒŒUž\r\nu‚WvF•¶‘Už"},
    {14, "Ž¯•Ê•\\Ž¦",     112,  1, char_kana,  pad_kana,  nullptr, "•¶ŽšŽíiŒ…jF‚bi‚Pj\r\n”CˆÓ€–Ú\r\nu‚xv‚Ü‚½‚ÍƒXƒy[ƒX\r\nu‚xv‚Ìê‡AuŒÚ‹qƒR[ƒh‚PvuŒÚ‹qƒR[ƒh‚Qv‚ÍuEDIî•ñv‚Æ‚È‚é"},
    {15, "ƒ_ƒ~[",        113,  7, char_dummy, pad_dummy, nullptr, "•¶ŽšŽíiŒ…jF‚bi‚Vj\r\n•K{€–Ú\r\nƒXƒy[ƒX‚Æ‚·‚é"}
};

const FBAttrs FBDataParser::fb_trailer_attrs_sohfuri =
{
    { 0, "ƒf[ƒ^‹æ•ª",  0,   1, "8",        "L8",      nullptr, "•¶ŽšŽíiŒ…jF‚mi‚Pj\r\n•K{€–Ú\r\nu‚WvFƒgƒŒ[ƒ‰[ƒŒƒR[ƒh"},
    { 1, "‡ŒvŒ”",    1,   6, char_num,   pad_num,   nullptr, "•¶ŽšŽíiŒ…jF‚mi‚Uj\r\n•K{€–Ú"},
    { 2, "‡Œv‹àŠz",    7,  12, char_num,   pad_num,   nullptr, "•¶ŽšŽíiŒ…jF‚mi‚P‚Qj\r\n•K{€–Ú"},
    { 3, "ƒ_ƒ~[",     19, 101, char_dummy, pad_dummy, nullptr, "•¶ŽšŽíiŒ…jF‚bi‚P‚O‚Pj\r\n•K{€–Ú\r\nƒXƒy[ƒX‚Æ‚·‚é"}
};

const FBAttrs FBDataParser::fb_end_attrs_sohfuri =
{
    { 0, "ƒf[ƒ^‹æ•ª", 0,   1, "9", "L9", nullptr, "•¶ŽšŽíiŒ…jF‚mi‚Pj\r\n•K{€–Ú\r\nu‚XvFƒGƒ“ƒhƒŒƒR[ƒh"},
    { 1, "ƒ_ƒ~[",     1, 119, char_dummy, pad_dummy, nullptr, "•¶ŽšŽíiŒ…jF‚bi‚P‚P‚Xj\r\n•K{€–Ú\r\nƒXƒy[ƒX‚Æ‚·‚é"}
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