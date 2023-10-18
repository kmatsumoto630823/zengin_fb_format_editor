#include "FBAttrs.h"
#include <unordered_map>

const std::string _KN_ = "###KN_PLACE_HOLDER###";
const std::string _NM_ = "###NM_PLACE_HOLDER###";
const std::string _DM_ = "###DM_PLACE_HOLDER###";

const std::string chars_kana_default   = " ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜｦﾝﾞﾟ";
const std::string padding_kana_default = "R ";

const std::string chars_num_default    = "0123456789";
const std::string padding_num_default  = "L0";

const std::string chars_dummy_default    = " ";
const std::string padding_dummy_default  = "R ";

// SOHFURI
const FBAttrsArray ATTRS_ARRAY_SOHFURI =
{
    //HEADER RECORD
    FBAttrs
    {
        { 0,   0,  1, 'N', "1",  'M', "L1", "データ区分",   "「１」：ヘッダーレコード"},
        { 1,   1,  2, 'N', _NM_, 'M', _NM_, "種別コード",   "「２１」：総合振込"},
        { 2,   3,  1, 'N', _NM_, 'M', _NM_, "コード区分",   "「０」：ＳＪＩＳ\r\n「１」：ＥＢＣＤＩＣ"},
        { 3,   4, 10, 'N', _NM_, 'M', _NM_, "依頼人コード", "振込依頼人識別のため銀行が採番したコード"},
        { 4,  14, 40, 'C', _KN_, 'M', _KN_, "依頼人名",     ""},
        { 5,  54,  4, 'N', _NM_, 'M', _NM_, "取組日",       "ＭＭＤＤ（ＭＭ月ＤＤ日）"},
        { 6,  58,  4, 'N', _NM_, 'M', _NM_, "仕向銀行番号", "統一金融機関番号"},
        { 7,  62, 15, 'C', _KN_, 'O', _KN_, "仕向銀行名",   ""},
        { 8,  77,  3, 'N', _NM_, 'M', _NM_, "仕向支店番号", "統一店番号"},
        { 9,  80, 15, 'C', _KN_, 'O', _KN_, "仕向支店名",   ""},
        {10,  95,  1, 'N', _NM_, 'O', _NM_, "預金種目",     "「１」：普通預金\r\n「２」：当座預金\r\n「９」：その他"},
        {11,  96,  7, 'N', _NM_, 'O', _NM_, "口座番号",     ""},
        {12, 103, 17, 'C', _DM_, '-', _DM_, "ダミー",       "スペースとする"}
    }
    ,

    //DATA RECORD
    FBAttrs
    {
        { 0,   0,  1, 'N', "2",  'M', "L2", "データ区分",     "「２」：データーレコード"},
        { 1,   1,  4, 'N', _NM_, 'M', _NM_, "銀行番号",       "統一金融機関番号"},
        { 2,   5, 15, 'C', _KN_, 'O', _KN_, "銀行名",         ""},
        { 3,  20,  3, 'N', _NM_, 'M', _NM_, "支店番号",       "統一店番号"},
        { 4,  23, 15, 'C', _KN_, 'O', _KN_, "支店名",         ""},
        { 5,  38,  4, 'N', _NM_, 'O', _NM_, "手形交換所番号", "統一手形交換所番号"},
        { 6,  42,  1, 'N', _NM_, 'M', _NM_, "預金種目",       "「１」：普通預金\r\n「２」：当座預金\r\n「４」：貯蓄預金\r\n「９」：その他"},
        { 7,  43,  7, 'N', _NM_, 'M', _NM_, "口座番号",       ""},
        { 8,  50, 30, 'C', _KN_, 'M', _KN_, "受取人名",       ""},
        { 9,  80, 10, 'N', _NM_, 'M', _NM_, "振込金額",       ""},
        {10,  90,  1, 'N', _NM_, 'M', _NM_, "新規コード",     "「１」：第１回振込分\r\n「２」：変更分（銀行・支店、預金種目・口座番号）\r\n「０」：その他"},
        {11,  91, 10, 'N', _KN_, 'O', _KN_, "顧客コード1",    "依頼人が定めた受取人識別のための顧客コード"},
        {12, 101, 10, 'N', _KN_, 'O', _KN_, "顧客コード2",    "依頼人が定めた受取人識別のための顧客コード"},
        {13, 111,  1, 'N', _NM_, 'O', _NM_, "振込区分",       "「７」：テレ振込\r\n「８」：文書振込"},
        {14, 112,  1, 'C', _KN_, 'O', _KN_, "識別表示",       "「Ｙ」またはスペース\r\n「Ｙ」の場合、「顧客コード１」「顧客コード２」は「EDI情報」となる"},
        {15, 113,  7, 'C', _DM_, '-', _DM_, "ダミー",         "スペースとする"}
    }
    ,

    //TRAILER RECORD
    FBAttrs
    {
        { 0,  0,   1, 'N', "8",  'M', "L8", "データ区分", "「８」：トレーラーレコード"},
        { 1,  1,   6, 'N', _NM_, 'M', _NM_, "合計件数",   ""},
        { 2,  7,  12, 'N', _NM_, 'M', _NM_, "合計金額",   ""},
        { 3, 19, 101, 'C', _DM_, '-', _DM_, "ダミー",     "スペースとする"}
    }
    ,

    //END RECORD
    FBAttrs
    {
        { 0, 0,   1, 'N', "9",  'M', "L9", "データ区分", "「９」：エンドレコード"},
        { 1, 1, 119, 'C', _DM_, '-', _DM_, "ダミー",     "スペースとする"}
    }
};

//KYUYO_SHOYO
const FBAttrsArray ATTRS_ARRAY_KYUYO_SHOYO =
{
    //HEADER RECORD
    FBAttrs
    {
        { 0,   0,  1, 'N', "1",  'M', "L1", "データ区分",         "「１」：ヘッダーレコード"},
        { 1,   1,  2, 'N', _NM_, 'M', _NM_, "種別コード",         "「１１」：給与振込（民間）\r\n「１２」：給与振込（民間）\r\n「７１」：給与振込（地方公務員）\r\n「７２」：賞与振込（地方公務員）"},
        { 2,   3,  1, 'N', _NM_, 'M', _NM_, "コード区分",         "「０」：ＳＪＩＳ\r\n「１」：ＥＢＣＤＩＣ"},
        { 3,   4, 10, 'N', _NM_, 'M', _NM_, "会社コード",         "銀行が採番した取引先の会社コード"},
        { 4,  14, 40, 'C', _KN_, 'M', _KN_, "会社名",             ""},
        { 5,  54,  4, 'N', _NM_, 'M', _NM_, "振込指定日",         "ＭＭＤＤ（ＭＭ月ＤＤ日）"},
        { 6,  58,  4, 'N', _NM_, 'M', _NM_, "仕向銀行番号",       "統一金融機関番号"},
        { 7,  62, 15, 'C', _KN_, 'O', _KN_, "仕向銀行名",         ""},
        { 8,  77,  3, 'N', _NM_, 'M', _NM_, "仕向支店番号",       "統一店番号"},
        { 9,  80, 15, 'C', _KN_, 'O', _KN_, "仕向支店名",         ""},
        {10,  95,  1, 'N', _NM_, 'O', _NM_, "預金種目（企業等）", "「１」：普通預金\r\n「２」：当座預金"},
        {11,  96,  7, 'N', _NM_, 'O', _NM_, "口座番号（企業等）", ""},
        {12, 103, 17, 'C', _DM_, '-', _DM_, "ダミー",             "スペースとする"}
    }
    ,

    //DATA RECORD
    FBAttrs
    {
        { 0,   0,  1, 'N', "2",  'M', "L2", "データ区分",     "「２」：データーレコード"},
        { 1,   1,  4, 'N', _NM_, 'M', _NM_, "銀行番号",       "統一金融機関番号"},
        { 2,   5, 15, 'C', _KN_, 'M', _KN_, "銀行名",         ""},
        { 3,  20,  3, 'N', _NM_, 'M', _NM_, "支店番号",       "統一店番号"},
        { 4,  23, 15, 'C', _KN_, 'M', _KN_, "支店名",         ""},
        { 5,  38,  4, 'N', _NM_, 'O', _NM_, "手形交換所番号", "統一手形交換所番号"},
        { 6,  42,  1, 'N', _NM_, 'M', _NM_, "預金種目",       "「１」：普通預金\r\n「２」：当座預金"},
        { 7,  43,  7, 'N', _NM_, 'M', _NM_, "口座番号",       ""},
        { 8,  50, 30, 'C', _KN_, 'M', _KN_, "預金者名",       ""},
        { 9,  80, 10, 'N', _NM_, 'M', _NM_, "振込金額",       ""},
        {10,  90,  1, 'N', _NM_, 'M', _NM_, "新規コード",     "「１」：第１回振込分\r\n「２」：変更分（銀行・支店、預金種目・口座番号）\r\n「０」：その他"},
        {11,  91, 10, 'N', _NM_, 'O', _NM_, "社員番号",       "企業等での社員番号"},
        {12, 101, 10, 'N', _NM_, 'O', _NM_, "所属コード",     "企業等での所属コード"},
        {13, 111,  9, 'C', _DM_, '-', _DM_, "ダミー",         "スペースとする"}
    }
    ,

    //TRAILER RECORD
    FBAttrs
    {
        { 0,  0,   1, 'N', "8",  'M', "L8", "データ区分", "「８」：トレーラーレコード"},
        { 1,  1,   6, 'N', _NM_, 'M', _NM_, "合計件数",   ""},
        { 2,  7,  12, 'N', _NM_, 'M', _NM_, "合計金額",   ""},
        { 3, 19, 101, 'C', _DM_, '-', _DM_, "ダミー",     "スペースとする"}
    }
    ,

    //END RECORD
    FBAttrs
    {
        { 0, 0,   1, 'N', "9",  'M', "L9", "データ区分", "「９」：エンドレコード"},
        { 1, 1, 119, 'C', _DM_, '-', _DM_, "ダミー",     "スペースとする"}
    }
};

//FURIKAE
const FBAttrsArray ATTRS_ARRAY_FURIKAE =
{
    //HEADER RECORD
    FBAttrs
    {
        { 0,   0,  1, 'N', "1",  'M', "L1", "データ区分",         "「１」：ヘッダーレコード"},
        { 1,   1,  2, 'N', _NM_, 'M', _NM_, "種別コード",         "「９１」：預金口座振替"},
        { 2,   3,  1, 'N', _NM_, 'M', _NM_, "コード区分",         "「０」：ＳＪＩＳ\r\n「１」：ＥＢＣＤＩＣ\r\nその他のコードについては全銀協基準による"},
        { 3,   4, 10, 'N', _NM_, 'M', _NM_, "委託者コード",       "銀行が定めた委託者のコード"},
        { 4,  14, 40, 'C', _KN_, 'M', _KN_, "委託者名",           ""},
        { 5,  54,  4, 'N', _NM_, 'M', _NM_, "引落日",             "ＭＭＤＤ（ＭＭ月ＤＤ日）"},
        { 6,  58,  4, 'N', _NM_, 'M', _NM_, "取引銀行番号",       "統一金融機関番号"},
        { 7,  62, 15, 'C', _KN_, 'O', _KN_, "取引銀行名",         ""},
        { 8,  77,  3, 'N', _NM_, 'M', _NM_, "取引支店番号",       "統一店番号"},
        { 9,  80, 15, 'C', _KN_, 'O', _KN_, "取引支店名",         ""},
        {10,  95,  1, 'N', _NM_, 'M', _NM_, "預金種目（委託者）", "「１」：普通預金\r\n「２」：当座預金\r\n「９」：その他"},
        {11,  96,  7, 'N', _NM_, 'M', _NM_, "口座番号（委託者）", ""},
        {12, 103, 17, 'C', _DM_, '-', _DM_, "ダミー",             "スペースとする"}
    }
    ,

    //DATA RECORD
    FBAttrs
    {
        { 0,   0,  1, 'N', "2",  'M', "L2", "データ区分",     "「２」：データーレコード"},
        { 1,   1,  4, 'N', _NM_, 'M', _NM_, "銀行番号",       "統一金融機関番号"},
        { 2,   5, 15, 'C', _KN_, 'O', _KN_, "銀行名",         ""},
        { 3,  20,  3, 'N', _NM_, 'M', _NM_, "支店番号",       "統一店番号"},
        { 4,  23, 15, 'C', _KN_, 'O', _KN_, "支店名",         ""},
        { 5,  38,  4, 'C', _DM_, '-', _DM_, "ダミー",         "スペースとする"},
        { 6,  42,  1, 'N', _NM_, 'M', _NM_, "預金種目",       "「１」：普通預金\r\n「２」：当座預金\r\n「３」：納税準備預金\r\n「９」：その他"},
        { 7,  43,  7, 'N', _NM_, 'M', _NM_, "口座番号",       ""},
        { 8,  50, 30, 'C', _KN_, 'M', _KN_, "預金者名",       ""},
        { 9,  80, 10, 'N', _NM_, 'M', _NM_, "引落金額",       ""},
        {10,  90,  1, 'N', _NM_, 'M', _NM_, "新規コード",     "「１」：第１回引落分\r\n「２」：変更分（銀行・支店・口座番号）\r\n「０」：その他"},
        {11,  91, 20, 'N', _NM_, 'M', _NM_, "顧客番号",       "委託者が定めた顧客番号"},
        {12, 111,  1, 'N', _NM_, 'M', _NM_, "振替結果コード", "振替処理を行った結果\r\n依頼明細では「０」とする"},
        {13, 112,  8, 'C', _DM_, '-', _DM_, "ダミー",         "スペースとする"}
    }
    ,

    //TRAILER RECORD
    FBAttrs
    {
        { 0,  0,   1, 'N', "8",  'M', "L8", "データ区分",   "「８」：トレーラーレコード"},
        { 1,  1,   6, 'N', _NM_, 'M', _NM_, "合計件数",     ""},
        { 2,  7,  12, 'N', _NM_, 'M', _NM_, "合計金額",     ""},
        { 3, 19,   6, 'N', _NM_, 'M', _NM_, "振替済件数",   "依頼明細では全て「０」とする"},
        { 4, 25,  12, 'N', _NM_, 'M', _NM_, "振替済金額",   "依頼明細では全て「０」とする"},
        { 5, 37,   6, 'N', _NM_, 'M', _NM_, "振替不能件数", "依頼明細では全て「０」とする"},
        { 6, 43,  12, 'N', _NM_, 'M', _NM_, "振替不能金額", "依頼明細では全て「０」とする"},
        { 7, 55,  65, 'C', _DM_, '-', _DM_, "ダミー",       "スペースとする"}
    }
    ,

    //END RECORD
    FBAttrs
    {
        { 0, 0,   1, 'N', "9",  'M', "L9", "データ区分", "「９」：エンドレコード"},
        { 1, 1, 119, 'C', _DM_, '-', _DM_, "ダミー",     "スペースとする"}
    }
};

FBAttrsArray make_attrs_array
(
    FBType type,
    std::string chars_kana,
    std::string padding_kana,
    std::string chars_num,
    std::string padding_num,
    std::string chars_dummy,
    std::string padding_dummy 
)
{
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
            throw;          
            break;
    }
    

    if(chars_kana.empty()   ){ chars_kana     = chars_kana_default;    }
    if(padding_kana.empty() ){ padding_kana   = padding_kana_default;  }
    if(chars_num.empty()    ){ chars_num      = chars_num_default;     }
    if(padding_num.empty()  ){ padding_num    = padding_num_default;   }
    if(chars_dummy.empty()  ){ chars_dummy    = chars_dummy_default;   }
    if(padding_dummy.empty()){ padding_dummy  = padding_dummy_default; }

    for(auto& attrs : attrs_array)
    {
        for(auto& attr : attrs)
        {
            if(attr.char_includes == _KN_){ attr.char_includes = chars_kana;    }
            if(attr.padding_conf  == _KN_){ attr.padding_conf  = padding_kana;  }
            if(attr.char_includes == _NM_){ attr.char_includes = chars_num;     }
            if(attr.padding_conf  == _NM_){ attr.padding_conf  = padding_num;   }
            if(attr.char_includes == _DM_){ attr.char_includes = chars_dummy;   }
            if(attr.padding_conf  == _DM_){ attr.padding_conf  = padding_dummy; }

            std::string  descript;
            descript =
                "項目：" + attr.label + 
                "\r\n" +
                "属性：" + attr.char_kind + "（" + std::to_string(attr.length) + "）" +
                "\r\n" +
                "入力：" + (attr.optionality == 'M' ? "必須" : (attr.optionality == 'O' ? "任意" : (attr.optionality == '-' ? "なし" : ""))) +
                "\r\n" +
                "\r\n" +
                (!attr.descript.empty() ? "説明：" : "") + 
                "\r\n" +
                attr.descript;


            attr.descript = descript;

        }
        
    }

    return attrs_array;
}