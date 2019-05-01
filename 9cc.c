#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

// トークンの型を表す値
enum {
	TK_NUM = 256, // 整数トークン
	TK_EOF,       // 入力の終了を表すトークン
};

// トークンの型
typedef struct {
	int ty;       // トークンの型
	int val;      // tyがTK_NUMの場合、その数値
	char *input;  // トークン文字列(エラーメッセージ用)
} Token;

// トークナイズ↓結果のトークン列を入れる配列
// 100個以上のトークンは来ないことを前提とする
Token tokens[100];

// エラー用の関数
void error(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

// pが指している文字列をトークンに分割し、tokensに保存する
void tokenize(char *p) {
	int i = 0;
	while (*p) {
		// 空白をスキップ
		if (isspace(*p)) {
			p++;
			continue;
		}

		if (*p == '+' || *p == '-') {
			tokens[i].ty = *p;
			tokens[i].input = p;
			i++;
			p++;
			continue;
		}

		if (isdigit(*p)) {
			tokens[i].ty = TK_NUM;
			tokens[i].input = p;
			tokens[i].val = strtol(p, &p, 10);
			i++;
			continue;
		}

		error("トークナイズできません: %s", p);
		exit(1);
	}
	tokens[i].ty = TK_EOF;
	tokens[i].input = p;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "argument error\n");
		return 1;
	}

	// トークナイズする
	tokenize(argv[1]);

	// 最初の部分を書き込む
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");


	// 引数の最初が数字であるか確認
	if (tokens[0].ty != TK_NUM)
		error("最初の項が数字ではありません。");
	printf("	mov rax, %d\n", tokens[0].val);

	// 
	int i = 1;
	while (tokens[i].ty != TK_EOF) {
		// tokens[i]に+が格納されていたら
		if (tokens[i].ty == '+') {
			// インクリメントで先頭アドレスが一つ増える(文字が右にシフト)
			// hoge -> oge -> ge -> e
			// +記号を飛ばす
			i++;
			if (tokens[i].ty != TK_NUM)
				error("予期しないトークンです: %s", tokens[i].input);
			// addアセンブリを記述し、次の数字読み取り
			printf("	add rax, %d\n", tokens[i].val);
			i++;
			// 以降の処理を行わない
			continue;
		}


		if (tokens[i].ty == '-') {
			// -記号を飛ばす
			i++;
			if (tokens[i].ty != TK_NUM)
				error("予期しないトークンです: %s", tokens[i].input);
			printf("	sub rax, %d\n", tokens[i].val);
			i++;
			continue;
		}
		
		// エラー
		error("予期しないトークンです: %s", tokens[i].input); 
	}

	printf("	ret\n");
	return 0;
}
