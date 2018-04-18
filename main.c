#include <stdio.h>
#include <string.h>
#define MAX_STR 80

typedef enum {
	Lparen, Rparen, Plus, Minus, Multi, Divi, Int, End, Error
} Kind;

typedef struct {
	Kind kind;
	int val;
} Token;

char string[MAX_STR];
int pos;
char ch;
Token token;

Token expression();

void nextChar() {
	ch = string[pos++];
}

void nextToken() {
	while (ch == ' ' || ch == '\t' || ch == '\n') nextChar();
	if (ch == '(') {
		token.kind = Lparen;
		nextChar();
	}
	else if (ch == ')') {
		token.kind = Rparen;
		nextChar();
	}
	else if (ch == '+') {
		token.kind = Plus;
		nextChar();
	}
	else if (ch == '-') {
		token.kind = Minus;
		nextChar();
	}
	else if (ch == '*') {
		token.kind = Multi;
		nextChar();
	}
	else if (ch == '/') {
		token.kind = Divi;
		nextChar();
	}
	else if ('0' <= ch && ch <= '9') {
		token.kind = Int;
		token.val = ch - '0';
		nextChar();
		int len = 1;
		while ('0' <= ch && ch <= '9') {
			token.val = token.val * 10 + (ch - '0');
			nextChar();
			len++;
		}
		if (token.val < 0 || 10 < len) token.kind = Error;
	}
	else if (ch == '\0') {
		token.kind = End;
	}
	else {
		token.kind = Error;
	}
}

Token factor() {
	Token ret;
	if (token.kind == Int) {
		ret = token;
		nextToken();
	}
	else if (token.kind == Lparen) {
		nextToken();
		ret = expression();
		if (token.kind != Rparen) ret.kind = Error;
		nextToken();
	}
	else if (token.kind == Plus) {
		nextToken();
		ret = factor();
	}
	else if (token.kind == Minus) {
		nextToken();
		ret = factor();
		ret.val = -ret.val;
	}
	else {
		ret.kind = Error;
	}
	return ret;
}

Token term() {
	Token ret = factor();
	if (ret.kind == Error) return ret;
	while (token.kind == Multi || token.kind == Divi) {
		if (token.kind == Multi) {
			nextToken();
			Token f = factor();
			if (f.kind == Error) return f;
			ret.val *= f.val;
		}
		else if (token.kind == Divi) {
			nextToken();
			Token f = factor();
			if (f.kind == Error) return f;
			ret.val /= f.val;
		}
	}
	return ret;
}

Token expression() {
	Token ret = term();
	if (ret.kind == Error) return ret;
	while (token.kind == Plus || token.kind == Minus) {
		if (token.kind == Plus) {
			nextToken();
			Token t = term();
			if (t.kind == Error) return t;
			ret.val += t.val;
		}
		else if (token.kind == Minus) {
			nextToken();
			Token t = term();
			if (t.kind == Error) return t;
			ret.val -= t.val;
		}
	}
	return ret;
}

int main() {
	while (fgets(string, MAX_STR, stdin) != NULL && strcmp(string, "\n") != 0) {
		pos = 0;
		nextChar();
		nextToken();
		Token calc = expression();
		if (token.kind == End && calc.kind == Int) printf("%d\n", calc.val);
		else printf("Error\n");
	}
	return 0;
}