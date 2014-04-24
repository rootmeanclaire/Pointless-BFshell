#include <iostream>
#include <string.h>
#include "parser.h"
#include "color.h"
#include "settings.h"

using namespace std;

unsigned char memory[30/*000*/];
unsigned int ptr = 0;

//Matches an opening or closing bracket to a corresponding opening or closing bracket
int getMatchingBraceIndex(unsigned int braceIndex, string str) {
	int level = 0;
	int returnLvl = -1;

	if (str.at(braceIndex) == '[') {
		for (int i = 0; i < str.length(); i++) {
			if (str.at(i) == '[') {
				level++;
				if (i == braceIndex) {
					returnLvl = level;
				}
			} else if (str.at(i) == ']') {
				if (level == returnLvl) {
					return i;
				} else {
					level--;
				}
			}
		}
	} else if (str.at(braceIndex) == ']') {
		for (int i = str.length() - 1; i >= 0; i--) {
			if (str.at(i) == ']') {
				level++;
				if (i == braceIndex) {
					returnLvl = level;
				}
			} else if (str.at(i) == '[') {
				if (level == returnLvl) {
					return i;
				} else {
					level--;
				}
			}
		}
	} else {
		//Error
		setColor(RED, BLACK);
		printf("Unable to match brace at index %i\n", braceIndex);
		setColor(DEFAULT, BLACK);
		return -1;
	}

	return -1;
}

void reset() {
	memset(memory, 0, sizeof(memory));
}

void exec(string code) {
	//Check for syntax errors
	{
		int openbrs = 0;
		int closebrs = 0;

		for (int i = 0; i < code.length(); i++) {
			if (code.at(i) == '[') {
				openbrs++;
			} else if (code.at(i) == ']') {
				closebrs++;
			}
		}

		if (openbrs != closebrs) {
			setColor(RED, BLACK);
			if (openbrs > closebrs) {
				printf("Syntax error: missing ']'\n");
			} else {
				printf("Syntax error: missing '['\n");
			}
			setColor(DEFAULT, BLACK);

			return;
		}
	}

	//Run Code
	bool hasOutput = false;

	for (int i = 0; i < code.length(); i++) {
		switch (code.at(i)) {
			case '+':
				memory[ptr]++;
				break;
			case '-':
				memory[ptr]--;
				break;
			case '>':
				if (ptr >= sizeof(memory) - 1) {
					if (wrap) {
						ptr = 0;
					} else {
						setColor(RED, BLACK);
						cout << "Runtime Error: Unable to move pointer, reached last cell\n";
						setColor(DEFAULT, BLACK);
					}
				} else {
					ptr++;
				}
				break;
			case '<':
				if (ptr <= 0) {
					if (wrap) {
						ptr = sizeof(memory) - 1;
					} else {
						setColor(RED, BLACK);
						cout << "Runtime Error: Unable to move pointer, reached cell 0\n";
						setColor(DEFAULT, BLACK);
					}
				} else {
					ptr--;
				}
				break;
			case '.':
				putchar((char) memory[ptr]);
				if (printASCIIval) {
					printf("(%d)", (int) memory[ptr]);
				}
				hasOutput = true;
				break;
			case ',':
				memory[ptr] = (unsigned char) getchar();
				break;
			case '[':
				if (memory[ptr] == 0) {
					i = getMatchingBraceIndex(i, code);
				}
				break;
			case ']':
				if (memory[ptr] > 0) {
					i = getMatchingBraceIndex(i, code);
				}
				break;
			default:
				break;
		}
	}
	if (hasOutput) {
		putchar('\n');
	}
}
