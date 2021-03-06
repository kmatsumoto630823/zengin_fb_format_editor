#include "FBAttrs.h"

const char CHARS_NUM[]   = "0123456789";
const char PAD_NUM[]     = "L0";

const char CHARS_KANA[]  = " ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜ¦ÝÞß";
const char PAD_KANA[]    = "R ";

const char CHARS_DUMMY[] = " ";
const char PAD_DUMMY[]   = "R ";

//SOHFURI
const FBAttrsArray ATTRS_ARRAY_SOHFURI =
{
    FBAttrs{//HEADER
        { 0, "f[^æª",     0,  1, "1",         "L1",      nullptr,   "¶®«FmiPj@\r\nK{Ú\r\nuPvFwb_[R[h"},
        { 1, "íÊR[h",     1,  2, CHARS_NUM,   PAD_NUM,   nullptr,   "¶®«FmiQj@\r\nK{Ú\r\nuQPvFU"},
        { 2, "R[hæª",     3,  1, CHARS_NUM,   PAD_NUM,   nullptr,   "¶®«FmiPj@\r\nK{Ú\r\nuOvFrihr\r\nuPvFdabchb"},
        { 3, "ËlR[h",   4, 10, CHARS_NUM,   PAD_NUM,   nullptr,   "¶®«FmiPOj\r\nK{Ú\r\nUËl¯ÊÌ½ßâsªÌÔµ½R[h"},
        { 4, "Ël¼",      14, 40, CHARS_KANA,  PAD_KANA,  nullptr,   "¶®«FbiSOj\r\nK{Ú"},
        { 5, "ægú",        54,  4, CHARS_NUM,   PAD_NUM,   nullptr,   "¶®«FmiSj@\r\nK{Ú\r\nllccillccúj"},
        { 6, "düâsÔ",  58,  4, CHARS_NUM,   PAD_NUM,   nullptr,   "¶®«FmiSj@\r\nK{Ú\r\nêàZ@ÖÔ"},
        { 7, "düâs¼",    62, 15, CHARS_KANA,  PAD_KANA,  nullptr,   "¶®«FbiPTj\r\nCÓÚ"},
        { 8, "düxXÔ",  77,  3, CHARS_NUM,   PAD_NUM,   nullptr,   "¶®«FmiRj@\r\nK{Ú\r\nêXÔ"},
        { 9, "düxX¼",    80, 15, CHARS_KANA,  PAD_KANA,  nullptr,   "¶®«FbiPTj\r\nCÓÚ"},
        {10, "aàíÚ",      95,  1, CHARS_NUM,   PAD_NUM,   " ",       "¶®«FmiPj@\r\nCÓÚ\r\nuPvFÊaà\r\nuQvFÀaà\r\nuXvF»Ì¼"},
        {11, "ûÀÔ",      96,  7, CHARS_NUM,   PAD_NUM,   "       ", "¶®«FmiVj@\r\nCÓÚ"},
        {12, "_~[",       103, 17, CHARS_DUMMY, PAD_DUMMY, nullptr,   "¶®«FbiPVj\r\nK{Ú\r\nXy[XÆ·é"}
    }//HEADER
    ,

    FBAttrs{//DATA
        { 0, "f[^æª",      0,  1, "2",         "L2",      nullptr, "¶®«FmiPj@\r\nK{Ú\r\nuQvFf[^[R[h"},
        { 1, "âsÔ",        1,  4, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiSj@\r\nK{Ú\r\nêàZ@ÖÔ"},
        { 2, "âs¼",          5, 15, CHARS_KANA,  PAD_KANA,  nullptr, "¶®«FbiPTj\r\nCÓÚ"},
        { 3, "xXÔ",       20,  3, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiRj@\r\nK{Ú\r\nêXÔ"},
        { 4, "xX¼",         23, 15, CHARS_KANA,  PAD_KANA,  nullptr, "¶®«FbiPTj\r\nCÓÚ"},
        { 5, "è`ð·Ô", 38,  4, CHARS_NUM,   PAD_NUM,   "    ",  "¶®«FmiSj@\r\nCÓÚ\r\nêè`ð·Ô"},
        { 6, "aàíÚ",       42,  1, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPj@\r\nK{Ú\r\nuPvFÊaà\r\nuQvFÀaà\r\nuSvF~aà\r\nuXvF»Ì¼"},
        { 7, "ûÀÔ",       43,  7, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiVj@\r\nK{Ú"},
        { 8, "óæl¼",       50, 30, CHARS_KANA,  PAD_KANA,  nullptr, "¶®«FbiROj\r\nK{Ú"},
        { 9, "Uàz",       80, 10, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPOj\r\nK{Ú"},
        {10, "VKR[h",     90,  1, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPj@\r\nK{Ú\r\nuPvFæPñUª\r\nuQvFÏXªiâsExXAaàíÚEûÀÔj\r\nuOvF»Ì¼"},
        {11, "ÚqR[h1",    91, 10, CHARS_KANA,  PAD_KANA,  nullptr, "¶®«FmiPOj\r\nCÓÚ\r\nËlªèß½óæl¯ÊÌ½ßÌÚqR[h"},
        {12, "ÚqR[h2",   101, 10, CHARS_KANA,  PAD_KANA,  nullptr, "¶®«FmiPOj\r\nCÓÚ\r\nËlªèß½óæl¯ÊÌ½ßÌÚqR[h"},
        {13, "Uæª",      111,  1, CHARS_NUM,   PAD_NUM,   " ",     "¶®«FmiPj@\r\nCÓÚ\r\nuVvFeU\r\nuWvF¶U"},
        {14, "¯Ê\¦",      112,  1, CHARS_KANA,  PAD_KANA,  nullptr, "¶®«FbiPj@\r\nCÓÚ\r\nuxvÜ½ÍXy[X\r\nuxvÌêAuÚqR[hPvuÚqR[hQvÍuEDIîñvÆÈé"},
        {15, "_~[",        113,  7, CHARS_DUMMY, PAD_DUMMY, nullptr, "¶®«FbiVj@\r\nK{Ú\r\nXy[XÆ·é"}
    }//DATA
    ,

    FBAttrs{//TRAILER
        { 0, "f[^æª",  0,   1, "8",         "L8",      nullptr, "¶®«FmiPj@@\r\nK{Ú\r\nuWvFg[[R[h"},
        { 1, "v",    1,   6, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiUj@@\r\nK{Ú"},
        { 2, "vàz",    7,  12, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPQj@\r\nK{Ú"},
        { 3, "_~[",     19, 101, CHARS_DUMMY, PAD_DUMMY, nullptr, "¶®«FbiPOPj\r\nK{Ú\r\nXy[XÆ·é"}
    }//TRAILER
    ,

    FBAttrs{//END
        { 0, "f[^æª", 0,   1, "9", "L9", nullptr, "¶®«FmiPj\r\nK{Ú\r\nuXvFGhR[h"},
        { 1, "_~[",     1, 119, CHARS_DUMMY, PAD_DUMMY, nullptr, "¶®«FbiPPXj\r\nK{Ú\r\nXy[XÆ·é"}
    }//END
};

//KYUYO_SHOYO

const FBAttrsArray ATTRS_ARRAY_KYUYO_SHOYO =
{
    FBAttrs{//HEADER
        { 0, "f[^æª",          0,  1, "1",         "L1",      nullptr,   "¶®«FmiPj@\r\nK{Ú\r\nuPvFwb_[R[h"},
        { 1, "íÊR[h",          1,  2, CHARS_NUM,   PAD_NUM,   nullptr,   "¶®«FmiQj@\r\nK{Ú\r\nuPPvF^Ui¯Ôj\r\nuPQvF^Ui¯Ôj\r\nuVPvF^Uinûö±õj\r\nuVQvFÜ^Uinûö±õj"},
        { 2, "R[hæª",          3,  1, CHARS_NUM,   PAD_NUM,   nullptr,   "¶®«FmiPj@\r\nK{Ú\r\nuOvFrihr\r\nuPvFdabchb"},
        { 3, "ïÐR[h",          4, 10, CHARS_NUM,   PAD_NUM,   nullptr,   "¶®«FmiPOj\r\nK{Ú\r\nâsªÌÔµ½æøæÌïÐR[h"},
        { 4, "ïÐ¼",             14, 40, CHARS_KANA,  PAD_KANA,  nullptr,   "¶®«FbiSOj\r\nK{Ú"},
        { 5, "Uwèú",         54,  4, CHARS_NUM,   PAD_NUM,   nullptr,   "¶®«FmiSj@\r\nK{Ú\r\nllccillccúj"},
        { 6, "düâsÔ",       58,  4, CHARS_NUM,   PAD_NUM,   nullptr,   "¶®«FmiSj@\r\nK{Ú\r\nêàZ@ÖÔ"},
        { 7, "düâs¼",         62, 15, CHARS_KANA,  PAD_KANA,  nullptr,   "¶®«FbiPTj\r\nCÓÚ"},
        { 8, "düxXÔ",       77,  3, CHARS_NUM,   PAD_NUM,   nullptr,   "¶®«FmiRj@\r\nK{Ú\r\nêXÔ"},
        { 9, "düxX¼",         80, 15, CHARS_KANA,  PAD_KANA,  nullptr,   "¶®«FbiPTj\r\nCÓÚ"},
        {10, "aàíÚiéÆj", 95,  1, CHARS_NUM,   PAD_NUM,   " ",       "¶®«FmiPj@\r\nCÓÚ\r\nuPvFÊaà\r\nuQvFÀaà"},
        {11, "ûÀÔiéÆj", 96,  7, CHARS_NUM,   PAD_NUM,   "       ", "¶®«FmiVj@\r\nCÓÚ"},
        {12, "_~[",            103, 17, CHARS_DUMMY, PAD_DUMMY, nullptr,   "¶®«FbiPVj\r\nK{Ú\r\nXy[XÆ·é"}
    }//HEADER
    ,

    FBAttrs{//DATA
        { 0, "f[^æª",      0,  1, "2",         "L2",      nullptr,      "¶®«FmiPj@\r\nK{Ú\r\nuQvFf[^[R[h"},
        { 1, "âsÔ",        1,  4, CHARS_NUM,   PAD_NUM,   nullptr,      "¶®«FmiSj@\r\nK{Ú\r\nêàZ@ÖÔ"},
        { 2, "âs¼",          5, 15, CHARS_KANA,  PAD_KANA,  nullptr,      "¶®«FbiPTj\r\nK{Ú"},
        { 3, "xXÔ",       20,  3, CHARS_NUM,   PAD_NUM,   nullptr,      "¶®«FmiRj@\r\nK{Ú\r\nêXÔ"},
        { 4, "xX¼",         23, 15, CHARS_KANA,  PAD_KANA,  nullptr,      "¶®«FbiPTj\r\nK{Ú"},
        { 5, "è`ð·Ô", 38,  4, CHARS_NUM,   PAD_NUM,   "    ",       "¶®«FmiSj@\r\nCÓÚ\r\nêè`ð·Ô"},
        { 6, "aàíÚ",       42,  1, CHARS_NUM,   PAD_NUM,   nullptr,      "¶®«FmiPj@\r\nK{Ú\r\nuPvFÊaà\r\nuQvFÀaà"},
        { 7, "ûÀÔ",       43,  7, CHARS_NUM,   PAD_NUM,   nullptr,      "¶®«FmiVj@\r\nK{Ú"},
        { 8, "aàÒ¼",       50, 30, CHARS_KANA,  PAD_KANA,  nullptr,      "¶®«FbiROj\r\nK{Ú"},
        { 9, "Uàz",       80, 10, CHARS_NUM,   PAD_NUM,   nullptr,      "¶®«FmiPOj\r\nK{Ú"},
        {10, "VKR[h",     90,  1, CHARS_NUM,   PAD_NUM,   nullptr,      "¶®«FmiPj@\r\nK{Ú\r\nuPvFæPñUª\r\nuQvFÏXªiâsExXAaàíÚEûÀÔj\r\nuOvF»Ì¼"},
        {11, "ÐõÔ",       91, 10, CHARS_NUM,   PAD_NUM,   "          ", "¶®«FmiPOj\r\nCÓÚ\r\néÆÅÌÐõÔ"},
        {12, "®R[h",    101, 10, CHARS_NUM,   PAD_NUM,   "          ", "¶®«FmiPOj\r\nCÓÚ\r\néÆÅÌ®R[h"},
        {13, "_~[",        111,  9, CHARS_DUMMY, PAD_DUMMY, nullptr,      "¶®«FbiXj@\r\nK{Ú\r\nXy[XÆ·é"}
    }//DATA
    ,

    FBAttrs{//TRAILER
        { 0, "f[^æª",  0,   1, "8",         "L8",      nullptr, "¶®«FmiPj@@\r\nK{Ú\r\nuWvFg[[R[h"},
        { 1, "v",    1,   6, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiUj@@\r\nK{Ú"},
        { 2, "vàz",    7,  12, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPQj@\r\nK{Ú"},
        { 3, "_~[",     19, 101, CHARS_DUMMY, PAD_DUMMY, nullptr, "¶®«FbiPOPj\r\nK{Ú\r\nXy[XÆ·é"}
    }//TRAILER
    ,

    FBAttrs{//END
        { 0, "f[^æª", 0,   1, "9", "L9", nullptr, "¶®«FmiPj\r\nK{Ú\r\nuXvFGhR[h"},
        { 1, "_~[",     1, 119, CHARS_DUMMY, PAD_DUMMY, nullptr, "¶®«FbiPPXj\r\nK{Ú\r\nXy[XÆ·é"}
    }//END
};

//FURIKAE
const FBAttrsArray ATTRS_ARRAY_FURIKAE =
{
    FBAttrs{//HEADER
        { 0, "f[^æª",           0,  1, "1",         "L1",      nullptr, "¶®«FmiPj@\r\nK{Ú\r\nuPvFwb_[R[h"},
        { 1, "íÊR[h",           1,  2, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiQj@\r\nK{Ú\r\nuXPvFaàûÀUÖ"},
        { 2, "R[hæª",           3,  1, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPj@\r\nK{Ú\r\nuOvFrihr\r\nuPvFdabchb\r\n»Ì¼ÌR[hÉÂ¢ÄÍSâ¦îÉæé"},
        { 3, "ÏõÒR[h",         4, 10, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPOj\r\nK{Ú\r\nâsªèß½ÏõÒÌR[h"},
        { 4, "ÏõÒ¼",            14, 40, CHARS_KANA,  PAD_KANA,  nullptr, "¶®«FbiSOj\r\nK{Ú"},
        { 5, "øú",              54,  4, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiSj@\r\nK{Ú\r\nllccillccúj"},
        { 6, "æøâsÔ",        58,  4, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiSj@\r\nK{Ú\r\nêàZ@ÖÔ"},
        { 7, "æøâs¼",          62, 15, CHARS_KANA,  PAD_KANA,  nullptr, "¶®«FbiPTj\r\nCÓÚ"},
        { 8, "æøxXÔ",        77,  3, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiRj@\r\nK{Ú\r\nêXÔ"},
        { 9, "æøxX¼",          80, 15, CHARS_KANA,  PAD_KANA,  nullptr, "¶®«FbiPTj\r\nCÓÚ"},
        {10, "aàíÚiÏõÒj",  95,  1, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPj@\r\nK{Ú\r\nuPvFÊaà\r\nuQvFÀaà\r\nuXvF»Ì¼"},
        {11, "ûÀÔiÏõÒj",  96,  7, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiVj@\r\nK{Ú"},
        {12, "_~[",             103, 17, CHARS_DUMMY, PAD_DUMMY, nullptr, "¶®«FbiPVj\r\nK{Ú\r\nXy[XÆ·é"}
    }//HEADER
    ,

    FBAttrs{//DATA
        { 0, "f[^æª",       0,  1, "2",         "L2",      nullptr, "¶®«FmiPj@\r\nK{Ú\r\nuQvFf[^[R[h"},
        { 1, "âsÔ",         1,  4, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiSj@\r\nK{Ú\r\nêàZ@ÖÔ"},
        { 2, "âs¼",           5, 15, CHARS_KANA,  PAD_KANA,  nullptr, "¶®«FbiPTj\r\nCÓÚ"},
        { 3, "xXÔ",        20,  3, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiRj@\r\nK{Ú\r\nêXÔ"},
        { 4, "xX¼",          23, 15, CHARS_KANA,  PAD_KANA,  nullptr, "¶®«FbiPTj\r\nCÓÚ"},
        { 5, "_~[",          38,  4, CHARS_DUMMY, PAD_DUMMY, nullptr, "¶®«FbiSj@\r\nK{Ú\r\nXy[XÆ·é"},
        { 6, "aàíÚ",        42,  1, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPj@\r\nK{Ú\r\nuPvFÊaà\r\nuQvFÀaà\r\nuRvF[Åõaà\r\nuXvF»Ì¼"},
        { 7, "ûÀÔ",        43,  7, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiVj@\r\nK{Ú"},
        { 8, "aàÒ¼",        50, 30, CHARS_KANA,  PAD_KANA,  nullptr, "¶®«FbiROj\r\nK{Ú"},
        { 9, "øàz",        80, 10, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPOj\r\nK{Ú"},
        {10, "VKR[h",      90,  1, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPj@\r\nK{Ú\r\nuPvFæPñøª\r\nuQvFÏXªiâsExXEûÀÔj\r\nuOvF»Ì¼"},
        {11, "ÚqÔ",        91, 20, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiQOj\r\nK{Ú\r\nÏõÒªèß½ÚqÔ"},
        {12, "UÖÊR[h", 111,  1, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPj@\r\nK{Ú\r\nUÖðsÁ½Ê\r\nË¾×ÅÍuOvÆ·é"},
        {13, "_~[",         112,  8, CHARS_DUMMY, PAD_DUMMY, nullptr, "¶®«FbiWj@\r\nK{Ú\r\nXy[XÆ·é"}
    }//DATA
    ,

    FBAttrs{//TRAILER
        { 0, "f[^æª",    0,   1, "8",         "L8",      nullptr, "¶®«FmiPj@\r\nK{Ú\r\nuWvFg[[R[h"},
        { 1, "v",      1,   6, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiUj@\r\nK{Ú"},
        { 2, "vàz",      7,  12, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPQj\r\nK{Ú"},
        { 3, "UÖÏ",   19,   6, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiUj@\r\nK{Ú\r\nË¾×ÅÍSÄuOvÆ·é"},
        { 4, "UÖÏàz",   25,  12, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPQj\r\nK{Ú\r\nË¾×ÅÍSÄuOvÆ·é"},
        { 5, "UÖs\", 37,   6, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiUj@\r\nK{Ú\r\nË¾×ÅÍSÄuOvÆ·é"},
        { 6, "UÖs\àz", 43,  12, CHARS_NUM,   PAD_NUM,   nullptr, "¶®«FmiPQj\r\nK{Ú\r\nË¾×ÅÍSÄuOvÆ·é"},
        { 7, "_~[",       55,  65, CHARS_DUMMY, PAD_DUMMY, nullptr, "¶®«FbiTTj\r\nK{Ú\r\nXy[XÆ·é"}
    }//TRAILER
    ,

    FBAttrs{//END
        { 0, "f[^æª", 0,   1, "9", "L9", nullptr, "¶®«FmiPj\r\nK{Ú\r\nuXvFGhR[h"},
        { 1, "_~[",     1, 119, CHARS_DUMMY, PAD_DUMMY, nullptr, "¶®«FbiPPXj\r\nK{Ú\r\nXy[XÆ·é"}
    }//END
};

