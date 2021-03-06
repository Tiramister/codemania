CodeMania
===

CodeManiaは、[Beatmania IIDX](http://dic.nicovideo.jp/a/beatmaniaiidx)という音楽ゲームに着想を得たプログラミング言語です。


## 特徴

* コードは1文字の「制御用スクラッチ」と8文字の「コマンド用鍵盤」の積み重ねからなります。

    * 「制御用スクラッチ」は各行1文字目のことです。この文字を一定区間`|`(パイプ)にすることで、繰り返し実行される範囲を指定します。

    * 「コマンド用鍵盤」は各行2~9文字目のことです。`_`(アンダースコア)をオン、` `(ホワイトスペース)をオフとみなして2進数のように値を入力することで、特定のコマンドを実行したりリテラルを指定することができます。

* なお、本プログラミング言語は元ネタであるBeatmania IIDXのリスペクトとして、上から下に鍵盤が落ちてくるようにコードが実行されます。言い換えれば、コードは**下から上へ**実行されます。

* 製作者の技術力的事情により、本言語のデータ型は「数値型(64bit符号付き整数)」、「文字型」、「文字列型」の3つのみからなり、各データ型に対して「変数1つ」と「stack」のみが用意されています。

* ソースコードを見れば脆弱性が散見されると思いますが、どうか攻めないでください。


## コードの解釈について

* 先にも述べた通り、本言語は9文字からなるコマンドを積み重ねることでコードを記述します。

    * 1行の文字数が9を超過した場合、10文字目以降は**無視されます**。つまりコメントとして好きなことを記述することが可能です。

    * 逆に1行の文字数が9に満たない場合、9文字になるまで自動的に**ホワイトスペースが補完されます**。

* 未定義のコマンドからなる行は無視されます。空行を挟んで譜面密度を調整することも可能です。

* コードは下から上へ実行され、1行目のコマンドを実行し終えるとプログラムは正常に終了します。


## コマンド一欄

コマンドは以下のような割り振りを基調としていますが、一部例外もあるので確認するのが一番確実です。

<img src="https://i.imgur.com/5OXmBZX.png" width="320px">


### 標準入出力

| コマンド | 名前 | 機能 |
| :---: | :---: | :---: |
| 10 10 00 00 | 文字列入力 | 文字列型を自由変数に標準入力 |
| 10 01 00 00 | 文字列出力 | 文字列型の自由変数を標準出力(改行なし) |
| 10 01 00 01 | 〃 | 〃(改行あり) |
| | | |
| 01 10 00 00 | 数値入力 | 整数型の自由変数に標準入力 |
| 01 01 00 00 | 数値出力 | 整数型の自由変数を標準出力(改行なし) |
| 01 01 00 01 | 〃 | 〃(改行あり) |

### スタック操作

本言語における各データ型の変数は、「自由変数」と「スタック」の2種類のみです。基本的に演算は自由変数のみに対して行えるため、これらの間でデータを上手くやりとりすることが鍵となります。

| コマンド | 名前 | 機能 |
| :---: | :---: | :---: |
| 10 00 01 00 | 文字列push | 文字列スタックに自由変数をpushする |
| 10 00 10 00 | 文字列pop | 文字列スタックの先頭を削除する |
| 10 00 10 01 | 文字列top | 文字列スタックの先頭を自由変数にcopyする |
| 10 00 11 00 | 文字列swap | 文字列スタックの先頭と自由変数を交換する |
| | | |
| 01 00 01 00 | 数値push | 数値スタックに自由変数をpushする |
| 01 00 10 00 | 数値pop | 数値スタックの先頭を削除する |
| 01 00 10 01 | 数値top | 数値スタックの先頭を自由変数にcopyする |
| 01 00 11 00 | 数値swap | 数値スタックの先頭と自由変数を交換する |

特に、先頭をコピーすることでスタックの先頭が削除されるわけではない点に要注意です。


### 文字型と文字列型

文字型は他2つのデータ型と比べて若干弱く、スタックが存在しません。これは、文字型は文字列型の自由変数をより操作しやすくするために導入したという経緯から来ています。ということで、文字型の操作は文字列型の自由変数とのやり取りが主となります。

| コマンド | 名前 | 機能 |
| :---: | :---: | :---: |
| 11 00 01 00 | 文字push | 文字変数を文字列変数の末尾に挿入する | 11 00 10 00 | 文字pop | 文字列変数の末尾を削除する |
| 11 00 10 01 | 文字back | 文字列変数の末尾を文字変数にcopyする |

文字列変数を文字のスタックと見れば、概ね上とコマンドは同じです。


### リテラル処理

本言語には数値やアルファベットを記述することができません。そこで、コマンド用鍵盤をbit列と見なすことでその役割を代理させています。

| コマンド | 名前 | 機能 |
| :---: | :---: | :---: |
| 11 00 00 00 | 文字リテラル | 挿入モードに移動。入力されたbit列のASCIIコードに対応する文字を文字変数に代入する |
| 01 00 00 00 | 整数リテラル | 挿入モードに移動。まず入力したい数値のbyte長を入力する。その後byte長回だけ挿入モードになり、入力された数値が数値変数に代入される |

例えば、文字変数に`a`を格納したい場合は

```
   __   _  aのASCIIコード、97=00110001を入力
 __        文字挿入モードへ移動
```

となり、また数値変数に`142857 = 10 0010 1110 0000 1001`を格納したい場合は

```
     _  _  0000 1001
   _ ___   0010 1110
       _   0000 0010
       __  3byte必要なので3を入力
  _        数値挿入モードへ移動
```

となります。

また、文字変数と数値変数はこれらで初期化できるのだが、文字列変数はそう簡単には初期化できない。そこで、文字列変数を初期化するコマンドも実装してあります。

| コマンド | 名前 | 機能 |
| :---: | :---: | :---: |
| 10 00 00 00 | 文字列初期化 | 文字列変数を初期化する |


### 数値演算

基本的な数値演算5種も実装してあります。これらは数値変数に対して数値スタックの先頭要素を作用させるようにしました。

| コマンド | 名前 | 機能 |
| :---: | :---: | :---: |
| 01 11 00 00 | 加算 | 数値変数に数値スタックの先頭要素を加える |
| 01 11 00 01 | 減算 | 数値変数から数値スタックの先頭要素を引く |
| 01 11 00 10 | 乗算 | 数値変数に数値スタックの先頭要素を掛ける |
| 01 11 00 11 | 除算 | 数値変数を数値スタックの先頭要素で割る(切り捨て) |
| 01 11 00 11 | 剰余算 | 数値変数を数値スタックで割ったときの余りを出す |


### 文字列演算

一応文字列にもわずかながら演算を用意しました。

| コマンド | 名前 | 機能 |
| :---: | :---: | :---: |
| 10 11 00 00 | 文字列長 | 文字列変数の長さを数値変数に代入する |
| 10 11 00 11 | 文字列反転 | 文字列変数を反転させる |


### 文字演算

文字と数値間の演算です。

| コマンド | 名前 | 機能 |
| :---: | :---: | :---: |
| 11 11 00 01 | キャスト(ASCII) | 文字列変数のASCIIコードを数値変数に代入 |
| 11 11 00 10 | キャスト(digit) | 文字列変数を数値と見たときの値を数値変数に代入 |
| 11 11 00 11 | 文字加算 | 文字列変数に数値変数を加算 |


コマンドは以上です。

## 制御構文

最後に制御構文についてです。本言語には非常に致命的なことに、if文が存在しません。そのかわりfor文とwhile文は存在します。

どちらの構文も基本は同じで、ループしたい部分全体の1文字目を`|`にするだけです。

for文は初めて`|`になったときのコマンドが0でないときに実行され、ループを**開始時点での数値変数の値の回数**だけ実行します。なお、最初の行のコマンドはループの頭で毎回実行されます。

一方while文は初めて`|`になったときのコマンドが0であったときに実行され、ループ終了時点での**数値変数の値が0の場合**だけループを抜けます。


## コード例

以上で構文の説明は終わりです。最後に、この言語での実装例を参考程度に載せておきます。

* `Hello, World!`を出力する

```
 _  _   _
 __   _  
   _    _
 __      
 __   _  
  __  _  
 __      
 __   _  
  __ __  
 __      
 __   _  
  ___  _
 __      
 __   _  
  __ ____
 __      
 __   _  
  _ _ ___
 __      
 __   _  
   _
 __      
 __   _  
   _ __  
 __      
 __   _  
  __ ____
 __      
 __   _  
  __ __  
 __      
 __   _  
  __ __  
 __      
 __   _  
  __  _ _
 __      
 __   _  
  _  _   
 __      
```

* 1 ~ N(標準入力)を足す

```
  _ _   _  output
  _  __    swap
| _  __    swap
| _  _     pop
| ___      add
|
  _  __    swap
           0
        _  1
  _        num
| _   _    push
| ___      add
|       _  1
|       _  1
| _        num
  __       input
  _   _    push
           0
        _  1
  _        num
```
