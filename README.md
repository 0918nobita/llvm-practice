# LLVM IRを気軽に勉強してみる

## 全力で要約すると

## 想定する読者

このC言語プログラムを理解できる人

```c
#include<stdio.h>

int main() {
  printf("Hello, world");
  return 0;
}
```

## 想定する開発環境

- llvm, clang がインストール済みの Linux

## 単純なプログラムをLLVM IRに変換してみる

```c:test.c
int main() {
  return 334;
}
```

↑これを、``clang -S -emit-llvm -O0 test.c`` コマンドで LLVM IR に変換すると…

↓こんなファイルが出力されます。要らなそうな部分は消しました。

```text:test.ll
define i32 @main() #0 {
  ret i32 334
}
```

↓容易に想像できることをまとめてみました。

| LLVM IR | なにそれ |
| ---- | ---- |
| ``define`` | 関数定義 |
| ``i32`` | 32bit 整数型 (この関数の返り値の型) |
| ``@main`` | 関数名「main」 |
| ``#0`` | **謎(1)** なんか Attribute ってやつらしい |
| ``{`` ... ``}`` | 関数定義の内容を囲むブロック的なあれ |
| ``ret i32 810`` | たぶん ``return 810;`` と対応してる |

わからないことには↑の「**謎(1)**」のようにチェックをつけていきます。
まぁ、そのうちわかるんでしょう、たぶん。

## 変数を使ったプログラムを変換してみる

```c:test2.c
int main() {
  int a = 2;
  return a;
}
```

```text:test2.ll
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 2, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  ret i32 %3
}
```

``%1``, ``%2``, ``%3`` のような、``%`` から始まるものは**レジスタ**を示します。

以下で紹介する ``alloca``, ``store``, ``load`` は**メモリアクセス演算子**です。

| LLVM IR | なにそれ |
| ---- | ---- |
| ``%1 = alloca i32, align 4`` | メモリ確保 ( アドレスが ``%1`` に格納される )  |
| ``store i32 0, i32* %1, align 4`` | 第 2 引数に渡したポインタ ``%1`` が指すメモリアドレスに、<br>第 1 引数で指定した値 ``0`` を格納する |
| ``%3 = load i32, i32* %2, align 4`` | 第 2 引数に渡したポインタ ``%2`` が指すメモリアドレスから、<br>値を取り出して ``%3`` に格納する |

やっとわかってきました。``main`` 関数の処理の流れを整理します。

1. 2 つの i32 型データのために 2 箇所メモリ確保して、それぞれのアドレスを ``%1`` , ``%2`` に格納する
2. ``%1`` に値 ``0`` を、``%2`` に値 ``2`` を格納する
3. ``%2`` の値を ``%3`` に格納する
4. ``%3`` の値を終了コードとして返す

要は『メモリに ``0`` と ``2`` を書き込んで、``2`` の方を終了コードとする』ってことです。  
ん？ ``0`` をメモリに書き込ん消してみますだのはなぜ？その処理、いらなくね？

思い切って、要らなそうな行を消してみます。

```echo
define i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 2, i32* %1, align 4
  %2 = load i32, i32* %1, align 4
  ret i32 %2
}
```

↓実行してみました

```
$ lli test_re.ll; echo $?
2
```

やっぱりね！初めて自分が書いたLLVM IRを動かすことができました！はい拍手！！

```test3.c
int main() {
  int a = 2;
  int b = 3;
  return a + b;
}
```

```text:test3.ll
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 2, i32* %2, align 4
  store i32 3, i32* %3, align 4
  %4 = load i32, i32* %2, align 4
  %5 = load i32, i32* %3, align 4
  %6 = add nsw i32 %4, %5
  ret i32 %6
}
```

```c:test4.c
int main() {
  int a = 2;
  int b = 3;
  return a + b;
}
```

```text:test4.ll
```

## 関数定義を含むプログラムを変換してみる

```c:test5.c
int main() {
  return add(1, 2);
}

int add(int x, int y) {
  return x + y;
}
```

```text:test5.ll
```
