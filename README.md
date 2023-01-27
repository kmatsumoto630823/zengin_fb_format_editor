# Zengin FB Format Editor
全銀協が定めるFBデータ（固定長）を編集するエディタ

総振、給与振込、賞与振込、口座振替のフォーマット対応

ゼロからFBデータを作成するというよりは出力済みのFBデータを微修正する用途を想定

# Description

■FBデータの読込
- CR/LF/CRLF/改行なしに対応（実際はCRとLFを読み飛ばしているだけ）
- SJISのみ対応(EBCDICは非対応)

■ヘッダレコード・データレコードのみの読込・出力
- ヘッダレコードの場合は上書き、データレコードの場合は尾また選択した位置に挿入となる
- ヘッダレコードの場合 取組日は上書きしない
- よく使う振込元や振込先のデータを事前作成しておき入力の負担を減らす用途を想定

■編集
- データレコードの行削・行追
- クリップボードからデータレコード・ヘッダレコードの読込
  - ヘッダレコードは既存レコードに上書、データレコードは末尾また選択した位置に行追
  - クリップボードテキストのCRとLFとTabを読み飛ばした結果がFBデータに適合していればOKとする
  - エクセル等の連携にうまく使ってください
- トレーラーレコードの合計件数と合計金額の再計算
- 入力制御
  - 項目ごとの最大桁数を超えて入力不可
  - 文字(C)の項目
    - 使用可能文字：" ()-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜｦﾝﾞﾟ"
    - ※使用可能文字はconfig.iniで変更可能
    - 確定時最大桁数に満たない場合は左詰め右スペース埋め
  - 数値(N)の項目
    - 使用可能文字："0123456789"
    - ※使用可能文字はconfig.iniで変更可能
    - 確定時最大桁数に満たない場合は右詰め左0埋め
    - 任意項目の場合確定時空文字で、０埋めを行わずブランク（スペース埋め）とする
 
■検索
- データレコードの次検索・前検索

■FBデータの出力
- 改行はCR/LF/CRLF/改行なしから選択可能
- SJISのみ(EBCDICは非対応)
- ０円のレコードが存在・データレコードの合計件数合計金額がトレーラーレコードと不一致・必須項目がスペースのみで埋められている場合は警告

# Dependencies
下記の環境でビルド
- WxWidgets-3.2.1
- Windows 10
- gcc/MinGW 12.2.0/10.0.0

# Author
kmatsumoto630823

k.matsumoto.s630823@gmail.com

何かありましたらこのメール宛てにでも

# License
MIT License

詳細はLicenseファイルをお読みください
