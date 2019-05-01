##!/bin/bash
try() {
	# 変数定義
	expected="$1"            # 期待(予想)値(引数1 -> $1)
	input="$2"               # 入力値(引数2 -> $2)

	# コンパイルした9ccにinput(引数)を与えて実行
	# 実行した結果をtmp.sに出力
	./9cc "$input" > tmp.s
	gcc -o tmp tmp.s         # tmp.sのコンパイル
	./tmp                    # コンパイル結果を実行
	actual="$?"              # returnの中身(結果)をactualに代入

	# 期待値と結果が同じでない場合、エラーを返す
	if [ "$actual" = "$expected" ]; then
		echo "$input => $actual"
	else
		echo "$expected expected, but got $actual"
		exit 1
	fi
}

# try関数を実行 期待値と入力値を引数にする(0, 0) and (42, 42)
try 0 0
try 42 42
try 21 '5+20-4'

echo OK
