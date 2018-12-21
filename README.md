# ReplaceWordsCommand
文字列の一括置換をコマンドで処理するCプログラム．改良予定．

### 仕様
- 現行ではトークンレベルでの一括置換を行う．
- 置換リストファイルの名称は「ReplaceList.txt」で固定．
    - 書き方にルールがあり，「置換前->置換後」の形式で行を追加していく．
    - 1行目はアトリビュートを置く．
- 上限は256で一括置換可能．
- 置換する文書ファイルは標準入力で指定．
    - 動作確認できている対応拡張子は「.txt」，「.md」．
- 置換後の文書ファイルは，「(文書ファイル名)_replaced.(文書ファイル拡張子)」で出力される．

### 実行例
- Doc.txt(置換前の文書ファイル)
```
In human robot conversation in a real environment, low 
speech recognition and unnatural response generation 
are critical issues. Most of autonomous conversational 
robotic systems avoid these issues by restricting user 
input and robot responses. However, such restrictions 
often make the interaction be boring because they are 
easy to predict a plot for a conversation.This paper 
proposes using multiple robots as a solution of this 
problem.
```
- ReplaceList.txt
```
置換前->置換後
system->SYSTEM
responses->RESPONSES
robot->MACHINE
```
- コマンド実行
```
$ ./replacetokens
Input FileName of Document: Doc.txt
Outputted: Doc_replaced.txt
$ 
```
- Doc_replaced.txt
```
In human MACHINE conversation in a real environment, low 
speech recognition and unnatural response generation 
are critical issues. Most of autonomous conversational 
robotic systems avoid these issues by restricting user 
input and MACHINE RESPONSES. However, such restrictions 
often make the interaction be boring because they are 
easy to predict a plot for a conversation.This paper 
proposes using multiple robots as a solution of this 
problem.
```

### 今後の改良
- トークンレベルではなく，文字列中であっても置換できるように実装し直す．