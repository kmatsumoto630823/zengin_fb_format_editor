#include "FBAttrs.h"

const char * _KN_ = "#KN";
const char * _NM_ = "#NM";


const FBAttrsArray ATTRS_ARRAY_SOHFURI =
{
    FBAttrs{//HEADER
        { 0,   0,  1, "1",  "L1", nullptr,   "データ区分",   "属性：Ｎ（１）　  \r\n必須項目\r\n「１」：ヘッダーレコード"},
        { 1,   1,  2, _NM_, _NM_, nullptr,   "種別コード",   "属性：Ｎ（２）　  \r\n必須項目\r\n「２１」：総合振込"},
        { 2,   3,  1, _NM_, _NM_, nullptr,   "コード区分",   "属性：Ｎ（１）　  \r\n必須項目\r\n「０」：ＳＪＩＳ\r\n「１」：ＥＢＣＤＩＣ"},
        { 3,   4, 10, _NM_, _NM_, nullptr,   "依頼人コード", "属性：Ｎ（１０）  \r\n必須項目\r\n振込依頼人識別のため銀行が採番したコード"},
        { 4,  14, 40, _KN_, _KN_, nullptr,   "依頼人名",     "属性：Ｃ（４０）  \r\n必須項目\r\n"},
        { 5,  54,  4, _NM_, _NM_, nullptr,   "取組日",       "属性：Ｎ（４）　  \r\n必須項目\r\nＭＭＤＤ（ＭＭ月ＤＤ日）"},
        { 6,  58,  4, _NM_, _NM_, nullptr,   "仕向銀行番号", "属性：Ｎ（４）　  \r\n必須項目\r\n統一金融機関番号"},
        { 7,  62, 15, _KN_, _KN_, nullptr,   "仕向銀行名",   "属性：Ｃ（１５）  \r\n任意項目\r\n"},
        { 8,  77,  3, _NM_, _NM_, nullptr,   "仕向支店番号", "属性：Ｎ（３）　  \r\n必須項目\r\n統一店番号"},
        { 9,  80, 15, _KN_, _KN_, nullptr,   "仕向支店名",   "属性：Ｃ（１５）  \r\n任意項目\r\n"},
        {10,  95,  1, _NM_, _NM_, " ",       "預金種目",     "属性：Ｎ（１）　  \r\n任意項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「９」：その他"},
        {11,  96,  7, _NM_, _NM_, "       ", "口座番号",     "属性：Ｎ（７）　  \r\n任意項目\r\n"},
        {12, 103, 17, " ",  "R ", nullptr,   "ダミー",       "属性：Ｃ（１７）  \r\n必須項目\r\nスペースとする"}
    }//HEADER
    ,

    FBAttrs{//DATA
        { 0,   0,  1, "2",  "L2", nullptr, "データ区分",     "属性：Ｎ（１）　  \r\n必須項目\r\n「２」：データーレコード"},
        { 1,   1,  4, _NM_, _NM_, nullptr, "銀行番号",       "属性：Ｎ（４）　  \r\n必須項目\r\n統一金融機関番号"},
        { 2,   5, 15, _KN_, _KN_, nullptr, "銀行名",         "属性：Ｃ（１５）  \r\n任意項目\r\n"},
        { 3,  20,  3, _NM_, _NM_, nullptr, "支店番号",       "属性：Ｎ（３）　  \r\n必須項目\r\n統一店番号"},
        { 4,  23, 15, _KN_, _KN_, nullptr, "支店名",         "属性：Ｃ（１５）  \r\n任意項目\r\n"},
        { 5,  38,  4, _NM_, _NM_, "    ",  "手形交換所番号", "属性：Ｎ（４）　  \r\n任意項目\r\n統一手形交換所番号"},
        { 6,  42,  1, _NM_, _NM_, nullptr, "預金種目",       "属性：Ｎ（１）　  \r\n必須項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「４」：貯蓄預金\r\n「９」：その他"},
        { 7,  43,  7, _NM_, _NM_, nullptr, "口座番号",       "属性：Ｎ（７）　  \r\n必須項目\r\n"},
        { 8,  50, 30, _KN_, _KN_, nullptr, "受取人名",       "属性：Ｃ（３０）  \r\n必須項目\r\n"},
        { 9,  80, 10, _NM_, _NM_, nullptr, "振込金額",       "属性：Ｎ（１０）  \r\n必須項目\r\n"},
        {10,  90,  1, _NM_, _NM_, nullptr, "新規コード",     "属性：Ｎ（１）　  \r\n必須項目\r\n「１」：第１回振込分\r\n「２」：変更分（銀行・支店、預金種目・口座番号）\r\n「０」：その他"},
        {11,  91, 10, _KN_, _KN_, nullptr, "顧客コード1",    "属性：Ｎ（１０）  \r\n任意項目\r\n依頼人が定めた受取人識別のための顧客コード"},
        {12, 101, 10, _KN_, _KN_, nullptr, "顧客コード2",    "属性：Ｎ（１０）  \r\n任意項目\r\n依頼人が定めた受取人識別のための顧客コード"},
        {13, 111,  1, _NM_, _NM_, " ",     "振込区分",       "属性：Ｎ（１）　  \r\n任意項目\r\n「７」：テレ振込\r\n「８」：文書振込"},
        {14, 112,  1, _KN_, _KN_, nullptr, "識別表示",       "属性：Ｃ（１）　  \r\n任意項目\r\n「Ｙ」またはスペース\r\n「Ｙ」の場合、「顧客コード１」「顧客コード２」は「EDI情報」となる"},
        {15, 113,  7, " ",  "R ", nullptr, "ダミー",         "属性：Ｃ（７）　  \r\n必須項目\r\nスペースとする"}
    }//DATA
    ,

    FBAttrs{//TRAILER
        { 0,  0,   1, "8",  "L8", nullptr, "データ区分", "属性：Ｎ（１）　   \r\n必須項目\r\n「８」：トレーラーレコード"},
        { 1,  1,   6, _NM_, _NM_, nullptr, "合計件数",   "属性：Ｎ（６）     \r\n必須項目\r\n"},
        { 2,  7,  12, _NM_, _NM_, nullptr, "合計金額",   "属性：Ｎ（１２）　 \r\n必須項目\r\n"},
        { 3, 19, 101, " ",  "R ", nullptr, "ダミー",     "属性：Ｃ（１０１） \r\n必須項目\r\nスペースとする"}
    }//TRAILER
    ,

    FBAttrs{//END
        { 0, 0,   1, "9", "L9", nullptr, "データ区分", "属性：Ｎ（１）     \r\n必須項目\r\n「９」：エンドレコード"},
        { 1, 1, 119, " ", "R ", nullptr, "ダミー",     "属性：Ｃ（１１９） \r\n必須項目\r\nスペースとする"}
    }//END
};

//KYUYO_SHOYO

const FBAttrsArray ATTRS_ARRAY_KYUYO_SHOYO =
{
    FBAttrs{//HEADER
        { 0,   0,  1, "1",  "L1", nullptr,   "データ区分",         "属性：Ｎ（１）　  \r\n必須項目\r\n「１」：ヘッダーレコード"},
        { 1,   1,  2, _NM_, _NM_, nullptr,   "種別コード",         "属性：Ｎ（２）　  \r\n必須項目\r\n「１１」：給与振込（民間）\r\n「１２」：給与振込（民間）\r\n「７１」：給与振込（地方公務員）\r\n「７２」：賞与振込（地方公務員）"},
        { 2,   3,  1, _NM_, _NM_, nullptr,   "コード区分",         "属性：Ｎ（１）　  \r\n必須項目\r\n「０」：ＳＪＩＳ\r\n「１」：ＥＢＣＤＩＣ"},
        { 3,   4, 10, _NM_, _NM_, nullptr,   "会社コード",         "属性：Ｎ（１０）  \r\n必須項目\r\n銀行が採番した取引先の会社コード"},
        { 4,  14, 40, _KN_, _KN_, nullptr,   "会社名",             "属性：Ｃ（４０）  \r\n必須項目\r\n"},
        { 5,  54,  4, _NM_, _NM_, nullptr,   "振込指定日",         "属性：Ｎ（４）　  \r\n必須項目\r\nＭＭＤＤ（ＭＭ月ＤＤ日）"},
        { 6,  58,  4, _NM_, _NM_, nullptr,   "仕向銀行番号",       "属性：Ｎ（４）　  \r\n必須項目\r\n統一金融機関番号"},
        { 7,  62, 15, _KN_, _KN_, nullptr,   "仕向銀行名",         "属性：Ｃ（１５）  \r\n任意項目\r\n"},
        { 8,  77,  3, _NM_, _NM_, nullptr,   "仕向支店番号",       "属性：Ｎ（３）　  \r\n必須項目\r\n統一店番号"},
        { 9,  80, 15, _KN_, _KN_, nullptr,   "仕向支店名",         "属性：Ｃ（１５）  \r\n任意項目\r\n"},
        {10,  95,  1, _NM_, _NM_, " ",       "預金種目（企業等）", "属性：Ｎ（１）　  \r\n任意項目\r\n「１」：普通預金\r\n「２」：当座預金"},
        {11,  96,  7, _NM_, _NM_, "       ", "口座番号（企業等）", "属性：Ｎ（７）　  \r\n任意項目\r\n"},
        {12, 103, 17, " ",    "R ", nullptr, "ダミー",             "属性：Ｃ（１７）  \r\n必須項目\r\nスペースとする"}
    }//HEADER
    ,

    FBAttrs{//DATA
        { 0,   0,  1, "2",  "L2", nullptr,      "データ区分",     "属性：Ｎ（１）　  \r\n必須項目\r\n「２」：データーレコード"},
        { 1,   1,  4, _NM_, _NM_, nullptr,      "銀行番号",       "属性：Ｎ（４）　  \r\n必須項目\r\n統一金融機関番号"},
        { 2,   5, 15, _KN_, _KN_, nullptr,      "銀行名",         "属性：Ｃ（１５）  \r\n必須項目"},
        { 3,  20,  3, _NM_, _NM_, nullptr,      "支店番号",       "属性：Ｎ（３）　  \r\n必須項目\r\n統一店番号"},
        { 4,  23, 15, _KN_, _KN_, nullptr,      "支店名",         "属性：Ｃ（１５）  \r\n必須項目"},
        { 5,  38,  4, _NM_, _NM_, "    ",       "手形交換所番号", "属性：Ｎ（４）　  \r\n任意項目\r\n統一手形交換所番号"},
        { 6,  42,  1, _NM_, _NM_, nullptr,      "預金種目",       "属性：Ｎ（１）　  \r\n必須項目\r\n「１」：普通預金\r\n「２」：当座預金"},
        { 7,  43,  7, _NM_, _NM_, nullptr,      "口座番号",       "属性：Ｎ（７）　  \r\n必須項目"},
        { 8,  50, 30, _KN_, _KN_, nullptr,      "預金者名",       "属性：Ｃ（３０）  \r\n必須項目"},
        { 9,  80, 10, _NM_, _NM_, nullptr,      "振込金額",       "属性：Ｎ（１０）  \r\n必須項目"},
        {10,  90,  1, _NM_, _NM_, nullptr,      "新規コード",     "属性：Ｎ（１）　  \r\n必須項目\r\n「１」：第１回振込分\r\n「２」：変更分（銀行・支店、預金種目・口座番号）\r\n「０」：その他"},
        {11,  91, 10, _NM_, _NM_, "          ", "社員番号",       "属性：Ｎ（１０）  \r\n任意項目\r\n企業等での社員番号"},
        {12, 101, 10, _NM_, _NM_, "          ", "所属コード",     "属性：Ｎ（１０）  \r\n任意項目\r\n企業等での所属コード"},
        {13, 111,  9, " ",  "R ", nullptr,      "ダミー",         "属性：Ｃ（９）　  \r\n必須項目\r\nスペースとする"}
    }//DATA
    ,

    FBAttrs{//TRAILER
        { 0,  0,   1, "8",  "L8", nullptr, "データ区分", "属性：Ｎ（１）　   \r\n必須項目\r\n「８」：トレーラーレコード"},
        { 1,  1,   6, _NM_, _NM_, nullptr, "合計件数",   "属性：Ｎ（６）　   \r\n必須項目\r\n"},
        { 2,  7,  12, _NM_, _NM_, nullptr, "合計金額",   "属性：Ｎ（１２）　 \r\n必須項目\r\n"},
        { 3, 19, 101, " ",  "R ", nullptr, "ダミー",     "属性：Ｃ（１０１） \r\n必須項目\r\nスペースとする"}
    }//TRAILER
    ,

    FBAttrs{//END
        { 0, 0,   1, "9", "L9", nullptr, "データ区分", "属性：Ｎ（１）     \r\n必須項目\r\n「９」：エンドレコード"},
        { 1, 1, 119, " ", "R ", nullptr, "ダミー",     "属性：Ｃ（１１９） \r\n必須項目\r\nスペースとする"}
    }//END
};

//FURIKAE
const FBAttrsArray ATTRS_ARRAY_FURIKAE =
{
    FBAttrs{//HEADER
        { 0,   0,  1, "1",  "L1", nullptr, "データ区分",         "属性：Ｎ（１）　  \r\n必須項目\r\n「１」：ヘッダーレコード"},
        { 1,   1,  2, _NM_, _NM_, nullptr, "種別コード",         "属性：Ｎ（２）　  \r\n必須項目\r\n「９１」：預金口座振替"},
        { 2,   3,  1, _NM_, _NM_, nullptr, "コード区分",         "属性：Ｎ（１）　  \r\n必須項目\r\n「０」：ＳＪＩＳ\r\n「１」：ＥＢＣＤＩＣ\r\nその他のコードについては全銀協基準による"},
        { 3,   4, 10, _NM_, _NM_, nullptr, "委託者コード",       "属性：Ｎ（１０）  \r\n必須項目\r\n銀行が定めた委託者のコード"},
        { 4,  14, 40, _KN_, _KN_, nullptr, "委託者名",           "属性：Ｃ（４０）  \r\n必須項目\r\n"},
        { 5,  54,  4, _NM_, _NM_, nullptr, "引落日",             "属性：Ｎ（４）　  \r\n必須項目\r\nＭＭＤＤ（ＭＭ月ＤＤ日）"},
        { 6,  58,  4, _NM_, _NM_, nullptr, "取引銀行番号",       "属性：Ｎ（４）　  \r\n必須項目\r\n統一金融機関番号"},
        { 7,  62, 15, _KN_, _KN_, nullptr, "取引銀行名",         "属性：Ｃ（１５）  \r\n任意項目\r\n"},
        { 8,  77,  3, _NM_, _NM_, nullptr, "取引支店番号",       "属性：Ｎ（３）　  \r\n必須項目\r\n統一店番号"},
        { 9,  80, 15, _KN_, _KN_, nullptr, "取引支店名",         "属性：Ｃ（１５）  \r\n任意項目\r\n"},
        {10,  95,  1, _NM_, _NM_, nullptr, "預金種目（委託者）", "属性：Ｎ（１）　  \r\n必須項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「９」：その他"},
        {11,  96,  7, _NM_, _NM_, nullptr, "口座番号（委託者）", "属性：Ｎ（７）　  \r\n必須項目\r\n"},
        {12, 103, 17, " ",  "R ", nullptr, "ダミー",             "属性：Ｃ（１７）  \r\n必須項目\r\nスペースとする"}
    }//HEADER
    ,

    FBAttrs{//DATA
        { 0,   0,  1, "2",  "L2", nullptr, "データ区分",     "属性：Ｎ（１）　  \r\n必須項目\r\n「２」：データーレコード"},
        { 1,   1,  4, _NM_, _NM_, nullptr, "銀行番号",       "属性：Ｎ（４）　  \r\n必須項目\r\n統一金融機関番号"},
        { 2,   5, 15, _KN_, _KN_, nullptr, "銀行名",         "属性：Ｃ（１５）  \r\n任意項目\r\n"},
        { 3,  20,  3, _NM_, _NM_, nullptr, "支店番号",       "属性：Ｎ（３）　  \r\n必須項目\r\n統一店番号"},
        { 4,  23, 15, _KN_, _KN_, nullptr, "支店名",         "属性：Ｃ（１５）  \r\n任意項目\r\n"},
        { 5,  38,  4, " ",  "R ", nullptr, "ダミー",         "属性：Ｃ（４）　  \r\n必須項目\r\nスペースとする"},
        { 6,  42,  1, _NM_, _NM_, nullptr, "預金種目",       "属性：Ｎ（１）　  \r\n必須項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「３」：納税準備預金\r\n「９」：その他"},
        { 7,  43,  7, _NM_, _NM_, nullptr, "口座番号",       "属性：Ｎ（７）　  \r\n必須項目\r\n"},
        { 8,  50, 30, _KN_, _KN_, nullptr, "預金者名",       "属性：Ｃ（３０）  \r\n必須項目\r\n"},
        { 9,  80, 10, _NM_, _NM_, nullptr, "引落金額",       "属性：Ｎ（１０）  \r\n必須項目\r\n"},
        {10,  90,  1, _NM_, _NM_, nullptr, "新規コード",     "属性：Ｎ（１）　  \r\n必須項目\r\n「１」：第１回引落分\r\n「２」：変更分（銀行・支店・口座番号）\r\n「０」：その他"},
        {11,  91, 20, _NM_, _NM_, nullptr, "顧客番号",       "属性：Ｎ（２０）  \r\n必須項目\r\n委託者が定めた顧客番号"},
        {12, 111,  1, _NM_, _NM_, nullptr, "振替結果コード", "属性：Ｎ（１）　  \r\n必須項目\r\n振替処理を行った結果\r\n依頼明細では「０」とする"},
        {13, 112,  8, " ",  "R ", nullptr, "ダミー",         "属性：Ｃ（８）　  \r\n必須項目\r\nスペースとする"}
    }//DATA
    ,

    FBAttrs{//TRAILER
        { 0,  0,   1, "8",  "L8", nullptr, "データ区分",   "文字属性：Ｎ（１）　  \r\n必須項目\r\n「８」：トレーラーレコード"},
        { 1,  1,   6, _NM_, _NM_, nullptr, "合計件数",     "文字属性：Ｎ（６）　  \r\n必須項目\r\n"},
        { 2,  7,  12, _NM_, _NM_, nullptr, "合計金額",     "文字属性：Ｎ（１２）  \r\n必須項目\r\n"},
        { 3, 19,   6, _NM_, _NM_, nullptr, "振替済件数",   "文字属性：Ｎ（６）　  \r\n必須項目\r\n依頼明細では全て「０」とする"},
        { 4, 25,  12, _NM_, _NM_, nullptr, "振替済金額",   "文字属性：Ｎ（１２）  \r\n必須項目\r\n依頼明細では全て「０」とする"},
        { 5, 37,   6, _NM_, _NM_, nullptr, "振替不能件数", "文字属性：Ｎ（６）　  \r\n必須項目\r\n依頼明細では全て「０」とする"},
        { 6, 43,  12, _NM_, _NM_, nullptr, "振替不能金額", "文字属性：Ｎ（１２）  \r\n必須項目\r\n依頼明細では全て「０」とする"},
        { 7, 55,  65, " ",  "R ", nullptr, "ダミー",       "文字属性：Ｃ（５５）  \r\n必須項目\r\nスペースとする"}
    }//TRAILER
    ,

    FBAttrs{//END
        { 0, 0,   1, "9", "L9", nullptr, "データ区分", "文字属性：Ｎ（１）     \r\n必須項目\r\n「９」：エンドレコード"},
        { 1, 1, 119, " ", "R ", nullptr, "ダミー",     "文字属性：Ｃ（１１９） \r\n必須項目\r\nスペースとする"}
    }//END
};

bool operator==(const FBAttr& lhs, const FBAttr& rhs)
{
        if(lhs.order  != rhs.order)  return false;
        if(lhs.offset != rhs.offset) return false;
        if(lhs.length != rhs.length) return false;

        if(lhs.char_includes != rhs.char_includes) return false;
        if(lhs.pad_info      != rhs.pad_info)      return false;
        if(lhs.initial_value != rhs.initial_value) return false;
        if(lhs.label         != rhs.initial_value) return false;
        if(lhs.description   != rhs.description)   return false;

        return true;
}


constexpr auto chars_kana_default  = " ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜｦﾝﾞﾟ";
constexpr auto pad_kana_default    = "R ";

constexpr auto chars_num_default   = "0123456789";
constexpr auto pad_num_default     = "L0";

FBAttrsArray make_attrs_array
(
    FBType type,
    const char* chars_kana,
    const char* pad_kana,
    const char* chars_num,
    const char* pad_num   
){
    FBAttrsArray attrs_array;

    switch(type)
    {
        case FBType::SOHFURI:
            attrs_array = ATTRS_ARRAY_SOHFURI;          
            break;

        case FBType::KYUYO_SHOYO:
            attrs_array = ATTRS_ARRAY_KYUYO_SHOYO;  
            break;

        case FBType::FURIKAE:
            attrs_array = ATTRS_ARRAY_FURIKAE;  
            break;

        default:
            attrs_array = ATTRS_ARRAY_SOHFURI;          
            break;
    }

    if(chars_kana == nullptr) chars_kana = chars_kana_default;
    if(pad_kana   == nullptr) pad_kana   = pad_kana_default;

    if(chars_num  == nullptr) chars_num  = chars_num_default;
    if(pad_num    == nullptr) pad_num    = pad_num_default;

    using namespace std::string_view_literals;

    for(auto& attrs : attrs_array)
    {
        for(auto& attr : attrs)
        {
            if(attr.char_includes == _KN_) attr.char_includes = chars_kana;
            if(attr.pad_info      == _KN_) attr.pad_info      = pad_kana;

            if(attr.char_includes == _NM_) attr.char_includes = chars_num;
            if(attr.pad_info      == _NM_) attr.pad_info      = pad_num;        
        }
        
    }

    return attrs_array;
}