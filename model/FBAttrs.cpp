#include "FBParser.h"

constexpr char char_num []  = "0123456789";
constexpr char pad_num []   = "L0";

constexpr char char_kana []  = " ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝﾞﾟ";
constexpr char pad_kana []   = "R ";

constexpr char char_dummy []  = " ";
constexpr char pad_dummy []   = "R ";


//総振
const FBAttrs FBParser::fb_header_attrs_sohfuri =
{
    { 0, "データ区分",     0,  1, "1",        "L1",      nullptr,   "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「１」：ヘッダーレコード"},
    { 1, "種別コード",     1,  2, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（２）\r\n必須項目\r\n「２１」：総合振込"},
    { 2, "コード区分",     3,  1, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「０」：ＳＪＩＳ\r\n「１」：ＥＢＣＤＩＣ"},
    { 3, "依頼人コード",   4, 10, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（１０）\r\n必須項目\r\n振込依頼人識別のため銀行が採番したコード"},
    { 4, "依頼人名",      14, 40, char_kana,  pad_kana,  nullptr,   "文字種（桁）：Ｃ（４０）\r\n必須項目"},
    { 5, "取組日",        54,  4, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（４）\r\n必須項目\r\nＭＭＤＤ（ＭＭ月ＤＤ日）"},
    { 6, "仕向銀行番号",  58,  4, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（４）\r\n必須項目\r\n統一金融機関番号"},
    { 7, "仕向銀行名",    62, 15, char_kana,  pad_kana,  nullptr,   "文字種（桁）：Ｃ（１５）\r\n任意項目"},
    { 8, "仕向支店番号",  77,  3, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（３）\r\n必須項目\r\n統一店番号"},
    { 9, "仕向支店名",    80, 15, char_kana,  pad_kana,  nullptr,   "文字種（桁）：Ｃ（１５）\r\n任意項目"},
    {10, "預金種目",      95,  1, char_num,   pad_num,   " ",       "文字種（桁）：Ｎ（１）\r\n任意項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「９」：その他"},
    {11, "口座番号",      96,  7, char_num,   pad_num,   "       ", "文字種（桁）：Ｎ（７）\r\n任意項目"},
    {12, "ダミー",       103, 17, char_dummy, pad_dummy, nullptr,   "文字種（桁）：Ｃ（１７）\r\n必須項目\r\nスペースとする"}
};

const FBAttrs FBParser::fb_data_attrs_sohfuri =
{
    { 0, "データ区分",      0,  1, "2",        "L2",      nullptr, "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「２」：データーレコード"},
    { 1, "銀行番号",        1,  4, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（４）\r\n必須項目\r\n統一金融機関番号"},
    { 2, "銀行名",          5, 15, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｃ（１５）\r\n任意項目"},
    { 3, "支店番号",       20,  3, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（３）\r\n必須項目\r\n統一店番号"},
    { 4, "支店名",         23, 15, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｃ（１５）\r\n任意項目"},
    { 5, "手形交換所番号", 38,  4, char_num,   pad_num,   "    ",  "文字種（桁）：Ｎ（４）\r\n 任意項目\r\n統一手形交換所番号"},
    { 6, "預金種目",       42,  1, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「４」：貯蓄預金\r\n「９」：その他"},
    { 7, "口座番号",       43,  7, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（７）\r\n必須項目"},
    { 8, "受取人名",       50, 30, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｃ（３０）\r\n必須項目"},
    { 9, "振込金額",       80, 10, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（１０）\r\n必須項目"},
    {10, "新規コード",     90,  1, char_num,   pad_num,   " ",     "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「１」：第１回振込分\r\n「２」：変更分（銀行・支店、預金種目・口座番号）\r\n「０」：その他"},
    {11, "顧客コード1",    91, 10, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｎ（１0）\r\n任意項目\r\n依頼人が定めた受取人識別のための顧客コード"},
    {12, "顧客コード2",   101, 10, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｎ（１0）\r\n任意項目\r\n依頼人が定めた受取人識別のための顧客コード"},
    {13, "振込区分",      111,  1, char_num,   pad_num,   " ",     "文字種（桁）：Ｎ（１）\r\n任意項目\r\n「７」：テレ振込\r\n「８」：文書振込"},
    {14, "識別表\示",     112,  1, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｃ（１）\r\n任意項目\r\n「Ｙ」またはスペース\r\n「Ｙ」の場合、「顧客コード１」「顧客コード２」は「EDI情報」となる"},
    {15, "ダミー",        113,  7, char_dummy, pad_dummy, nullptr, "文字種（桁）：Ｃ（７）\r\n必須項目\r\nスペースとする"}
};

const FBAttrs FBParser::fb_trailer_attrs_sohfuri =
{
    { 0, "データ区分",  0,   1, "8",        "L8",      nullptr, "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「８」：トレーラーレコード"},
    { 1, "合計件数",    1,   6, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（６）\r\n必須項目"},
    { 2, "合計金額",    7,  12, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（１２）\r\n必須項目"},
    { 3, "ダミー",     19, 101, char_dummy, pad_dummy, nullptr, "文字種（桁）：Ｃ（１０１）\r\n必須項目\r\nスペースとする"}
};

const FBAttrs FBParser::fb_end_attrs_sohfuri =
{
    { 0, "データ区分", 0,   1, "9", "L9", nullptr, "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「９」：エンドレコード"},
    { 1, "ダミー",     1, 119, char_dummy, pad_dummy, nullptr, "文字種（桁）：Ｃ（１１９）\r\n必須項目\r\nスペースとする"}
};

//給与未実装

const FBAttrs FBParser::fb_header_attrs_kyuyo =
{
    { 0, "データ区分",     0,  1, "1",        "L1",      nullptr,   "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「１」：ヘッダーレコード"},
    { 1, "種別コード",     1,  2, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（２）\r\n必須項目\r\n「２１」：総合振込"},
    { 2, "コード区分",     3,  1, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「０」：ＳＪＩＳ\r\n「１」：ＥＢＣＤＩＣ"},
    { 3, "依頼人コード",   4, 10, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（１０）\r\n必須項目\r\n振込依頼人識別のため銀行が採番したコード"},
    { 4, "依頼人名",      14, 40, char_kana,  pad_kana,  nullptr,   "文字種（桁）：Ｃ（４０）\r\n必須項目"},
    { 5, "取組日",        54,  4, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（４）\r\n必須項目\r\nＭＭＤＤ（ＭＭ月ＤＤ日）"},
    { 6, "仕向銀行番号",  58,  4, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（４）\r\n必須項目\r\n統一金融機関番号"},
    { 7, "仕向銀行名",    62, 15, char_kana,  pad_kana,  nullptr,   "文字種（桁）：Ｃ（１５）\r\n任意項目"},
    { 8, "仕向支店番号",  77,  3, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（３）\r\n必須項目\r\n統一店番号"},
    { 9, "仕向支店名",    80, 15, char_kana,  pad_kana,  nullptr,   "文字種（桁）：Ｃ（１５）\r\n任意項目"},
    {10, "預金種目",      95,  1, char_num,   pad_num,   " ",       "文字種（桁）：Ｎ（１）\r\n任意項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「９」：その他"},
    {11, "口座番号",      96,  7, char_num,   pad_num,   "       ", "文字種（桁）：Ｎ（７）\r\n任意項目"},
    {12, "ダミー",       103, 17, char_dummy, pad_dummy, nullptr,   "文字種（桁）：Ｃ（１７）\r\n必須項目\r\nスペースとする"}
};

const FBAttrs FBParser::fb_data_attrs_kyuyo =
{
    { 0, "データ区分",      0,  1, "2",        "L2",      nullptr, "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「２」：データーレコード"},
    { 1, "銀行番号",        1,  4, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（４）\r\n必須項目\r\n統一金融機関番号"},
    { 2, "銀行名",          5, 15, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｃ（１５）\r\n任意項目"},
    { 3, "支店番号",       20,  3, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（３）\r\n必須項目\r\n統一店番号"},
    { 4, "支店名",         23, 15, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｃ（１５）\r\n任意項目"},
    { 5, "手形交換所番号", 38,  4, char_num,   pad_num,   "    ",  "文字種（桁）：Ｎ（４）\r\n 任意項目\r\n統一手形交換所番号"},
    { 6, "預金種目",       42,  1, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「４」：貯蓄預金\r\n「９」：その他"},
    { 7, "口座番号",       43,  7, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（７）\r\n必須項目"},
    { 8, "受取人名",       50, 30, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｃ（３０）\r\n必須項目"},
    { 9, "振込金額",       80, 10, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（１０）\r\n必須項目"},
    {10, "新規コード",     90,  1, char_num,   pad_num,   " ",     "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「１」：第１回振込分\r\n「２」：変更分（銀行・支店、預金種目・口座番号）\r\n「０」：その他"},
    {11, "顧客コード1",    91, 10, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｎ（１0）\r\n任意項目\r\n依頼人が定めた受取人識別のための顧客コード"},
    {12, "顧客コード2",   101, 10, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｎ（１0）\r\n任意項目\r\n依頼人が定めた受取人識別のための顧客コード"},
    {13, "振込区分",      111,  1, char_num,   pad_num,   " ",     "文字種（桁）：Ｎ（１）\r\n任意項目\r\n「７」：テレ振込\r\n「８」：文書振込"},
    {14, "識別表\示",     112,  1, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｃ（１）\r\n任意項目\r\n「Ｙ」またはスペース\r\n「Ｙ」の場合、「顧客コード１」「顧客コード２」は「EDI情報」となる"},
    {15, "ダミー",        113,  7, char_dummy, pad_dummy, nullptr, "文字種（桁）：Ｃ（７）\r\n必須項目\r\nスペースとする"}
};

const FBAttrs FBParser::fb_trailer_attrs_kyuyo =
{
    { 0, "データ区分",  0,   1, "8",        "L8",      nullptr, "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「８」：トレーラーレコード"},
    { 1, "合計件数",    1,   6, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（６）\r\n必須項目"},
    { 2, "合計金額",    7,  12, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（１２）\r\n必須項目"},
    { 3, "ダミー",     19, 101, char_dummy, pad_dummy, nullptr, "文字種（桁）：Ｃ（１０１）\r\n必須項目\r\nスペースとする"}
};

const FBAttrs FBParser::fb_end_attrs_kyuyo =
{
    { 0, "データ区分", 0,   1, "9", "L9", nullptr, "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「９」：エンドレコード"},
    { 1, "ダミー",     1, 119, char_dummy, pad_dummy, nullptr, "文字種（桁）：Ｃ（１１９）\r\n必須項目\r\nスペースとする"}
};

//賞与（未実装）

const FBAttrs FBParser::fb_header_attrs_shoyo =
{
    { 0, "データ区分",     0,  1, "1",        "L1",      nullptr,   "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「１」：ヘッダーレコード"},
    { 1, "種別コード",     1,  2, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（２）\r\n必須項目\r\n「２１」：総合振込"},
    { 2, "コード区分",     3,  1, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「０」：ＳＪＩＳ\r\n「１」：ＥＢＣＤＩＣ"},
    { 3, "依頼人コード",   4, 10, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（１０）\r\n必須項目\r\n振込依頼人識別のため銀行が採番したコード"},
    { 4, "依頼人名",      14, 40, char_kana,  pad_kana,  nullptr,   "文字種（桁）：Ｃ（４０）\r\n必須項目"},
    { 5, "取組日",        54,  4, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（４）\r\n必須項目\r\nＭＭＤＤ（ＭＭ月ＤＤ日）"},
    { 6, "仕向銀行番号",  58,  4, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（４）\r\n必須項目\r\n統一金融機関番号"},
    { 7, "仕向銀行名",    62, 15, char_kana,  pad_kana,  nullptr,   "文字種（桁）：Ｃ（１５）\r\n任意項目"},
    { 8, "仕向支店番号",  77,  3, char_num,   pad_num,   nullptr,   "文字種（桁）：Ｎ（３）\r\n必須項目\r\n統一店番号"},
    { 9, "仕向支店名",    80, 15, char_kana,  pad_kana,  nullptr,   "文字種（桁）：Ｃ（１５）\r\n任意項目"},
    {10, "預金種目",      95,  1, char_num,   pad_num,   " ",       "文字種（桁）：Ｎ（１）\r\n任意項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「９」：その他"},
    {11, "口座番号",      96,  7, char_num,   pad_num,   "       ", "文字種（桁）：Ｎ（７）\r\n任意項目"},
    {12, "ダミー",       103, 17, char_dummy, pad_dummy, nullptr,   "文字種（桁）：Ｃ（１７）\r\n必須項目\r\nスペースとする"}
};

const FBAttrs FBParser::fb_data_attrs_shoyo =
{
    { 0, "データ区分",      0,  1, "2",        "L2",      nullptr, "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「２」：データーレコード"},
    { 1, "銀行番号",        1,  4, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（４）\r\n必須項目\r\n統一金融機関番号"},
    { 2, "銀行名",          5, 15, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｃ（１５）\r\n任意項目"},
    { 3, "支店番号",       20,  3, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（３）\r\n必須項目\r\n統一店番号"},
    { 4, "支店名",         23, 15, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｃ（１５）\r\n任意項目"},
    { 5, "手形交換所番号", 38,  4, char_num,   pad_num,   "    ",  "文字種（桁）：Ｎ（４）\r\n 任意項目\r\n統一手形交換所番号"},
    { 6, "預金種目",       42,  1, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「４」：貯蓄預金\r\n「９」：その他"},
    { 7, "口座番号",       43,  7, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（７）\r\n必須項目"},
    { 8, "受取人名",       50, 30, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｃ（３０）\r\n必須項目"},
    { 9, "振込金額",       80, 10, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（１０）\r\n必須項目"},
    {10, "新規コード",     90,  1, char_num,   pad_num,   " ",     "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「１」：第１回振込分\r\n「２」：変更分（銀行・支店、預金種目・口座番号）\r\n「０」：その他"},
    {11, "顧客コード1",    91, 10, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｎ（１0）\r\n任意項目\r\n依頼人が定めた受取人識別のための顧客コード"},
    {12, "顧客コード2",   101, 10, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｎ（１0）\r\n任意項目\r\n依頼人が定めた受取人識別のための顧客コード"},
    {13, "振込区分",      111,  1, char_num,   pad_num,   " ",     "文字種（桁）：Ｎ（１）\r\n任意項目\r\n「７」：テレ振込\r\n「８」：文書振込"},
    {14, "識別表\示",     112,  1, char_kana,  pad_kana,  nullptr, "文字種（桁）：Ｃ（１）\r\n任意項目\r\n「Ｙ」またはスペース\r\n「Ｙ」の場合、「顧客コード１」「顧客コード２」は「EDI情報」となる"},
    {15, "ダミー",        113,  7, char_dummy, pad_dummy, nullptr, "文字種（桁）：Ｃ（７）\r\n必須項目\r\nスペースとする"}
};

const FBAttrs FBParser::fb_trailer_attrs_shoyo =
{
    { 0, "データ区分",  0,   1, "8",        "L8",      nullptr, "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「８」：トレーラーレコード"},
    { 1, "合計件数",    1,   6, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（６）\r\n必須項目"},
    { 2, "合計金額",    7,  12, char_num,   pad_num,   nullptr, "文字種（桁）：Ｎ（１２）\r\n必須項目"},
    { 3, "ダミー",     19, 101, char_dummy, pad_dummy, nullptr, "文字種（桁）：Ｃ（１０１）\r\n必須項目\r\nスペースとする"}
};

const FBAttrs FBParser::fb_end_attrs_shoyo =
{
    { 0, "データ区分", 0,   1, "9", "L9", nullptr, "文字種（桁）：Ｎ（１）\r\n必須項目\r\n「９」：エンドレコード"},
    { 1, "ダミー",     1, 119, char_dummy, pad_dummy, nullptr, "文字種（桁）：Ｃ（１１９）\r\n必須項目\r\nスペースとする"}
};