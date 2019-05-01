#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "argument error\n");
		return 1;
	}

	char *p = argv[1];

	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");
	// strtolで左から数値を読み込む。数値でない値が来たら
	// アドレスを&pに格納する。今回は文字列を10進数に変換
	printf("	mov rax, %ld\n", strtol(p, &p, 10));

	while (*p) {
		// pに+が格納されていたら
		if (*p == '+') {
			// インクリメントで先頭アドレスが一つ増える(文字が右にシフト)
			// hoge -> oge -> ge -> e
			// +記号を飛ばす
			p++;
			// addアセンブリを記述し、次の数字読み取り
			printf("	add rax, %ld\n", strtol(p, &p, 10));
			// 以降の処理を行わない
			continue;
		}


		if (*p == '-') {
			// -記号を飛ばす
			p++;
			printf("	sub rax, %ld\n", strtol(p, &p, 10));
			continue;
		}
		
		// ストリーム(stderr)に出力
		fprintf(stderr, "予期しない文字です: '%c'\n", *p);
		return 1;
	}

	printf("	ret\n");
	return 0;
}
