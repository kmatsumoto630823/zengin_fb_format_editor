# zengin_fb_format_editor
全銀協のFBデータ（固定長）を編集するエディタ

いまのところ総振にのみ対応

# Description

■FBデータの読込
-CR/LF/CRLF/改行なしに対応（実際はCRとLFを読み飛ばしているだけ）
-SJISのみ(EBCDICは非対応)

■ヘッダレコードのプリセット読込・出力
- 事前にプリセットファイルを用意しておけばヘッダレコードだけ差し替え可能
- 取組日は上書きしない
- 実際はヘッダレコード1行のファイルを読込・出力しているだけ

■編集
- 行作・行追
- 入力制御
 - 項目ごとの最大桁数を超えて入力不可
 - 文字(C)
  - 使用可能文字：" ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝﾞﾟ"
  - 確定時最大桁数に満たない場合は左詰め右スペース埋め
 - 数値(N)
   - 使用可能文字："0123456789"
 　- 確定時最大桁数に満たない場合は左詰め右スペース埋め
 　- 任意項目の場合、文字をすべて削除したうえで確定するとスペース埋め
 
■データレコードの検索

■トレーラーレコードの合計件数と合計金額の再計算

■FBデータの出力
- 改行はCR/LF/CRLF/改行なしから選択可能
- データレコードから計算した合計件数と合計金額がトレーラーレコードと不一致の場合警告

# Author
kmatsumoto630823

# License
MIT
