#include "FBAttrs.h"

const char CHARS_NUM[]   = "0123456789";
const char PAD_NUM[]     = "L0";

const char CHARS_KANA[]  = " ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜｦﾝﾞﾟ";
const char PAD_KANA[]    = "R ";

const char CHARS_DUMMY[] = " ";
const char PAD_DUMMY[]   = "R ";

//SOHFURI
const FBAttrsArray ATTRS_ARRAY_SOHFURI =
{
    FBAttrs{//HEADER
        { 0, "データ区分",     0,  1, "1",         "L1",      nullptr,   "文字属性：Ｎ（１）　\r\n必須項目\r\n「１」：ヘッダーレコード"},
        { 1, "種別コード",     1,  2, CHARS_NUM,   PAD_NUM,   nullptr,   "文字属性：Ｎ（２）　\r\n必須項目\r\n「２１」：総合振込"},
        { 2, "コード区分",     3,  1, CHARS_NUM,   PAD_NUM,   nullptr,   "文字属性：Ｎ（１）　\r\n必須項目\r\n「０」：ＳＪＩＳ\r\n「１」：ＥＢＣＤＩＣ"},
        { 3, "依頼人コード",   4, 10, CHARS_NUM,   PAD_NUM,   nullptr,   "文字属性：Ｎ（１０）\r\n必須項目\r\n振込依頼人識別のため銀行が採番したコード"},
        { 4, "依頼人名",      14, 40, CHARS_KANA,  PAD_KANA,  nullptr,   "文字属性：Ｃ（４０）\r\n必須項目"},
        { 5, "取組日",        54,  4, CHARS_NUM,   PAD_NUM,   nullptr,   "文字属性：Ｎ（４）　\r\n必須項目\r\nＭＭＤＤ（ＭＭ月ＤＤ日）"},
        { 6, "仕向銀行番号",  58,  4, CHARS_NUM,   PAD_NUM,   nullptr,   "文字属性：Ｎ（４）　\r\n必須項目\r\n統一金融機関番号"},
        { 7, "仕向銀行名",    62, 15, CHARS_KANA,  PAD_KANA,  nullptr,   "文字属性：Ｃ（１５）\r\n任意項目"},
        { 8, "仕向支店番号",  77,  3, CHARS_NUM,   PAD_NUM,   nullptr,   "文字属性：Ｎ（３）　\r\n必須項目\r\n統一店番号"},
        { 9, "仕向支店名",    80, 15, CHARS_KANA,  PAD_KANA,  nullptr,   "文字属性：Ｃ（１５）\r\n任意項目"},
        {10, "預金種目",      95,  1, CHARS_NUM,   PAD_NUM,   " ",       "文字属性：Ｎ（１）　\r\n任意項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「９」：その他"},
        {11, "口座番号",      96,  7, CHARS_NUM,   PAD_NUM,   "       ", "文字属性：Ｎ（７）　\r\n任意項目"},
        {12, "ダミー",       103, 17, CHARS_DUMMY, PAD_DUMMY, nullptr,   "文字属性：Ｃ（１７）\r\n必須項目\r\nスペースとする"}
    }//HEADER
    ,

    FBAttrs{//DATA
        { 0, "データ区分",      0,  1, "2",         "L2",      nullptr, "文字属性：Ｎ（１）　\r\n必須項目\r\n「２」：データーレコード"},
        { 1, "銀行番号",        1,  4, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（４）　\r\n必須項目\r\n統一金融機関番号"},
        { 2, "銀行名",          5, 15, CHARS_KANA,  PAD_KANA,  nullptr, "文字属性：Ｃ（１５）\r\n任意項目"},
        { 3, "支店番号",       20,  3, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（３）　\r\n必須項目\r\n統一店番号"},
        { 4, "支店名",         23, 15, CHARS_KANA,  PAD_KANA,  nullptr, "文字属性：Ｃ（１５）\r\n任意項目"},
        { 5, "手形交換所番号", 38,  4, CHARS_NUM,   PAD_NUM,   "    ",  "文字属性：Ｎ（４）　\r\n任意項目\r\n統一手形交換所番号"},
        { 6, "預金種目",       42,  1, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１）　\r\n必須項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「４」：貯蓄預金\r\n「９」：その他"},
        { 7, "口座番号",       43,  7, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（７）　\r\n必須項目"},
        { 8, "受取人名",       50, 30, CHARS_KANA,  PAD_KANA,  nullptr, "文字属性：Ｃ（３０）\r\n必須項目"},
        { 9, "振込金額",       80, 10, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１０）\r\n必須項目"},
        {10, "新規コード",     90,  1, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１）　\r\n必須項目\r\n「１」：第１回振込分\r\n「２」：変更分（銀行・支店、預金種目・口座番号）\r\n「０」：その他"},
        {11, "顧客コード1",    91, 10, CHARS_KANA,  PAD_KANA,  nullptr, "文字属性：Ｎ（１０）\r\n任意項目\r\n依頼人が定めた受取人識別のための顧客コード"},
        {12, "顧客コード2",   101, 10, CHARS_KANA,  PAD_KANA,  nullptr, "文字属性：Ｎ（１０）\r\n任意項目\r\n依頼人が定めた受取人識別のための顧客コード"},
        {13, "振込区分",      111,  1, CHARS_NUM,   PAD_NUM,   " ",     "文字属性：Ｎ（１）　\r\n任意項目\r\n「７」：テレ振込\r\n「８」：文書振込"},
        {14, "識別表示",      112,  1, CHARS_KANA,  PAD_KANA,  nullptr, "文字属性：Ｃ（１）　\r\n任意項目\r\n「Ｙ」またはスペース\r\n「Ｙ」の場合、「顧客コード１」「顧客コード２」は「EDI情報」となる"},
        {15, "ダミー",        113,  7, CHARS_DUMMY, PAD_DUMMY, nullptr, "文字属性：Ｃ（７）　\r\n必須項目\r\nスペースとする"}
    }//DATA
    ,

    FBAttrs{//TRAILER
        { 0, "データ区分",  0,   1, "8",         "L8",      nullptr, "文字属性：Ｎ（１）　　\r\n必須項目\r\n「８」：トレーラーレコード"},
        { 1, "合計件数",    1,   6, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（６）　　\r\n必須項目"},
        { 2, "合計金額",    7,  12, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１２）　\r\n必須項目"},
        { 3, "ダミー",     19, 101, CHARS_DUMMY, PAD_DUMMY, nullptr, "文字属性：Ｃ（１０１）\r\n必須項目\r\nスペースとする"}
    }//TRAILER
    ,

    FBAttrs{//END
        { 0, "データ区分", 0,   1, "9", "L9", nullptr, "文字属性：Ｎ（１）\r\n必須項目\r\n「９」：エンドレコード"},
        { 1, "ダミー",     1, 119, CHARS_DUMMY, PAD_DUMMY, nullptr, "文字属性：Ｃ（１１９）\r\n必須項目\r\nスペースとする"}
    }//END
};

//KYUYO_SHOYO

const FBAttrsArray ATTRS_ARRAY_KYUYO_SHOYO =
{
    FBAttrs{//HEADER
        { 0, "データ区分",          0,  1, "1",         "L1",      nullptr,   "文字属性：Ｎ（１）　\r\n必須項目\r\n「１」：ヘッダーレコード"},
        { 1, "種別コード",          1,  2, CHARS_NUM,   PAD_NUM,   nullptr,   "文字属性：Ｎ（２）　\r\n必須項目\r\n「１１」：給与振込（民間）\r\n「１２」：給与振込（民間）\r\n「７１」：給与振込（地方公務員）\r\n「７２」：賞与振込（地方公務員）"},
        { 2, "コード区分",          3,  1, CHARS_NUM,   PAD_NUM,   nullptr,   "文字属性：Ｎ（１）　\r\n必須項目\r\n「０」：ＳＪＩＳ\r\n「１」：ＥＢＣＤＩＣ"},
        { 3, "会社コード",          4, 10, CHARS_NUM,   PAD_NUM,   nullptr,   "文字属性：Ｎ（１０）\r\n必須項目\r\n銀行が採番した取引先の会社コード"},
        { 4, "会社名",             14, 40, CHARS_KANA,  PAD_KANA,  nullptr,   "文字属性：Ｃ（４０）\r\n必須項目"},
        { 5, "振込指定日",         54,  4, CHARS_NUM,   PAD_NUM,   nullptr,   "文字属性：Ｎ（４）　\r\n必須項目\r\nＭＭＤＤ（ＭＭ月ＤＤ日）"},
        { 6, "仕向銀行番号",       58,  4, CHARS_NUM,   PAD_NUM,   nullptr,   "文字属性：Ｎ（４）　\r\n必須項目\r\n統一金融機関番号"},
        { 7, "仕向銀行名",         62, 15, CHARS_KANA,  PAD_KANA,  nullptr,   "文字属性：Ｃ（１５）\r\n任意項目"},
        { 8, "仕向支店番号",       77,  3, CHARS_NUM,   PAD_NUM,   nullptr,   "文字属性：Ｎ（３）　\r\n必須項目\r\n統一店番号"},
        { 9, "仕向支店名",         80, 15, CHARS_KANA,  PAD_KANA,  nullptr,   "文字属性：Ｃ（１５）\r\n任意項目"},
        {10, "預金種目（企業等）", 95,  1, CHARS_NUM,   PAD_NUM,   " ",       "文字属性：Ｎ（１）　\r\n任意項目\r\n「１」：普通預金\r\n「２」：当座預金"},
        {11, "口座番号（企業等）", 96,  7, CHARS_NUM,   PAD_NUM,   "       ", "文字属性：Ｎ（７）　\r\n任意項目"},
        {12, "ダミー",            103, 17, CHARS_DUMMY, PAD_DUMMY, nullptr,   "文字属性：Ｃ（１７）\r\n必須項目\r\nスペースとする"}
    }//HEADER
    ,

    FBAttrs{//DATA
        { 0, "データ区分",      0,  1, "2",         "L2",      nullptr,      "文字属性：Ｎ（１）　\r\n必須項目\r\n「２」：データーレコード"},
        { 1, "銀行番号",        1,  4, CHARS_NUM,   PAD_NUM,   nullptr,      "文字属性：Ｎ（４）　\r\n必須項目\r\n統一金融機関番号"},
        { 2, "銀行名",          5, 15, CHARS_KANA,  PAD_KANA,  nullptr,      "文字属性：Ｃ（１５）\r\n必須項目"},
        { 3, "支店番号",       20,  3, CHARS_NUM,   PAD_NUM,   nullptr,      "文字属性：Ｎ（３）　\r\n必須項目\r\n統一店番号"},
        { 4, "支店名",         23, 15, CHARS_KANA,  PAD_KANA,  nullptr,      "文字属性：Ｃ（１５）\r\n必須項目"},
        { 5, "手形交換所番号", 38,  4, CHARS_NUM,   PAD_NUM,   "    ",       "文字属性：Ｎ（４）　\r\n任意項目\r\n統一手形交換所番号"},
        { 6, "預金種目",       42,  1, CHARS_NUM,   PAD_NUM,   nullptr,      "文字属性：Ｎ（１）　\r\n必須項目\r\n「１」：普通預金\r\n「２」：当座預金"},
        { 7, "口座番号",       43,  7, CHARS_NUM,   PAD_NUM,   nullptr,      "文字属性：Ｎ（７）　\r\n必須項目"},
        { 8, "預金者名",       50, 30, CHARS_KANA,  PAD_KANA,  nullptr,      "文字属性：Ｃ（３０）\r\n必須項目"},
        { 9, "振込金額",       80, 10, CHARS_NUM,   PAD_NUM,   nullptr,      "文字属性：Ｎ（１０）\r\n必須項目"},
        {10, "新規コード",     90,  1, CHARS_NUM,   PAD_NUM,   nullptr,      "文字属性：Ｎ（１）　\r\n必須項目\r\n「１」：第１回振込分\r\n「２」：変更分（銀行・支店、預金種目・口座番号）\r\n「０」：その他"},
        {11, "社員番号",       91, 10, CHARS_NUM,   PAD_NUM,   "          ", "文字属性：Ｎ（１０）\r\n任意項目\r\n企業等での社員番号"},
        {12, "所属コード",    101, 10, CHARS_NUM,   PAD_NUM,   "          ", "文字属性：Ｎ（１０）\r\n任意項目\r\n企業等での所属コード"},
        {13, "ダミー",        111,  9, CHARS_DUMMY, PAD_DUMMY, nullptr,      "文字属性：Ｃ（９）　\r\n必須項目\r\nスペースとする"}
    }//DATA
    ,

    FBAttrs{//TRAILER
        { 0, "データ区分",  0,   1, "8",         "L8",      nullptr, "文字属性：Ｎ（１）　　\r\n必須項目\r\n「８」：トレーラーレコード"},
        { 1, "合計件数",    1,   6, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（６）　　\r\n必須項目"},
        { 2, "合計金額",    7,  12, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１２）　\r\n必須項目"},
        { 3, "ダミー",     19, 101, CHARS_DUMMY, PAD_DUMMY, nullptr, "文字属性：Ｃ（１０１）\r\n必須項目\r\nスペースとする"}
    }//TRAILER
    ,

    FBAttrs{//END
        { 0, "データ区分", 0,   1, "9", "L9", nullptr, "文字属性：Ｎ（１）\r\n必須項目\r\n「９」：エンドレコード"},
        { 1, "ダミー",     1, 119, CHARS_DUMMY, PAD_DUMMY, nullptr, "文字属性：Ｃ（１１９）\r\n必須項目\r\nスペースとする"}
    }//END
};

//FURIKAE
const FBAttrsArray ATTRS_ARRAY_FURIKAE =
{
    FBAttrs{//HEADER
        { 0, "データ区分",           0,  1, "1",         "L1",      nullptr, "文字属性：Ｎ（１）　\r\n必須項目\r\n「１」：ヘッダーレコード"},
        { 1, "種別コード",           1,  2, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（２）　\r\n必須項目\r\n「９１」：預金口座振替"},
        { 2, "コード区分",           3,  1, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１）　\r\n必須項目\r\n「０」：ＳＪＩＳ\r\n「１」：ＥＢＣＤＩＣ\r\nその他のコードについては全銀協基準による"},
        { 3, "委託者コード",         4, 10, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１０）\r\n必須項目\r\n銀行が定めた委託者のコード"},
        { 4, "委託者名",            14, 40, CHARS_KANA,  PAD_KANA,  nullptr, "文字属性：Ｃ（４０）\r\n必須項目"},
        { 5, "引落日",              54,  4, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（４）　\r\n必須項目\r\nＭＭＤＤ（ＭＭ月ＤＤ日）"},
        { 6, "取引銀行番号",        58,  4, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（４）　\r\n必須項目\r\n統一金融機関番号"},
        { 7, "取引銀行名",          62, 15, CHARS_KANA,  PAD_KANA,  nullptr, "文字属性：Ｃ（１５）\r\n任意項目"},
        { 8, "取引支店番号",        77,  3, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（３）　\r\n必須項目\r\n統一店番号"},
        { 9, "取引支店名",          80, 15, CHARS_KANA,  PAD_KANA,  nullptr, "文字属性：Ｃ（１５）\r\n任意項目"},
        {10, "預金種目（委託者）",  95,  1, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１）　\r\n必須項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「９」：その他"},
        {11, "口座番号（委託者）",  96,  7, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（７）　\r\n必須項目"},
        {12, "ダミー",             103, 17, CHARS_DUMMY, PAD_DUMMY, nullptr, "文字属性：Ｃ（１７）\r\n必須項目\r\nスペースとする"}
    }//HEADER
    ,

    FBAttrs{//DATA
        { 0, "データ区分",       0,  1, "2",         "L2",      nullptr, "文字属性：Ｎ（１）　\r\n必須項目\r\n「２」：データーレコード"},
        { 1, "銀行番号",         1,  4, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（４）　\r\n必須項目\r\n統一金融機関番号"},
        { 2, "銀行名",           5, 15, CHARS_KANA,  PAD_KANA,  nullptr, "文字属性：Ｃ（１５）\r\n任意項目"},
        { 3, "支店番号",        20,  3, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（３）　\r\n必須項目\r\n統一店番号"},
        { 4, "支店名",          23, 15, CHARS_KANA,  PAD_KANA,  nullptr, "文字属性：Ｃ（１５）\r\n任意項目"},
        { 5, "ダミー",          38,  4, CHARS_DUMMY, PAD_DUMMY, nullptr, "文字属性：Ｃ（４）　\r\n必須項目\r\nスペースとする"},
        { 6, "預金種目",        42,  1, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１）　\r\n必須項目\r\n「１」：普通預金\r\n「２」：当座預金\r\n「３」：納税準備預金\r\n「９」：その他"},
        { 7, "口座番号",        43,  7, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（７）　\r\n必須項目"},
        { 8, "預金者名",        50, 30, CHARS_KANA,  PAD_KANA,  nullptr, "文字属性：Ｃ（３０）\r\n必須項目"},
        { 9, "引落金額",        80, 10, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１０）\r\n必須項目"},
        {10, "新規コード",      90,  1, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１）　\r\n必須項目\r\n「１」：第１回引落分\r\n「２」：変更分（銀行・支店・口座番号）\r\n「０」：その他"},
        {11, "顧客番号",        91, 20, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（２０）\r\n必須項目\r\n委託者が定めた顧客番号"},
        {12, "振替結果コード", 111,  1, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１）　\r\n必須項目\r\n振替処理を行った結果\r\n依頼明細では「０」とする"},
        {13, "ダミー",         112,  8, CHARS_DUMMY, PAD_DUMMY, nullptr, "文字属性：Ｃ（８）　\r\n必須項目\r\nスペースとする"}
    }//DATA
    ,

    FBAttrs{//TRAILER
        { 0, "データ区分",    0,   1, "8",         "L8",      nullptr, "文字属性：Ｎ（１）　\r\n必須項目\r\n「８」：トレーラーレコード"},
        { 1, "合計件数",      1,   6, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（６）　\r\n必須項目"},
        { 2, "合計金額",      7,  12, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１２）\r\n必須項目"},
        { 3, "振替済件数",   19,   6, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（６）　\r\n必須項目\r\n依頼明細では全て「０」とする"},
        { 4, "振替済金額",   25,  12, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１２）\r\n必須項目\r\n依頼明細では全て「０」とする"},
        { 5, "振替不能件数", 37,   6, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（６）　\r\n必須項目\r\n依頼明細では全て「０」とする"},
        { 6, "振替不能金額", 43,  12, CHARS_NUM,   PAD_NUM,   nullptr, "文字属性：Ｎ（１２）\r\n必須項目\r\n依頼明細では全て「０」とする"},
        { 7, "ダミー",       55,  65, CHARS_DUMMY, PAD_DUMMY, nullptr, "文字属性：Ｃ（５５）\r\n必須項目\r\nスペースとする"}
    }//TRAILER
    ,

    FBAttrs{//END
        { 0, "データ区分", 0,   1, "9", "L9", nullptr, "文字属性：Ｎ（１）\r\n必須項目\r\n「９」：エンドレコード"},
        { 1, "ダミー",     1, 119, CHARS_DUMMY, PAD_DUMMY, nullptr, "文字属性：Ｃ（１１９）\r\n必須項目\r\nスペースとする"}
    }//END
};

