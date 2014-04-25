#include <iostream>
#include <string.h>
#include "parser.h"
#include "color.h"
#include "settings.h"

using namespace std;

unsigned char memory[30000];
unsigned int ptr = 0;

//Matches an opening or closing bracket to a corresponding opening or closing bracket
int getMatchingBraceIndex(unsigned int braceIndex, string str) {
	//Current nesting level
	int level = 0;
	//Matching brace's nesting level
	//(Matching braces have to be at the same level)
	int returnLvl = -1;

	/*******************************************************************************\
	 * Note:                                                                       *
	 * If brace given is open brace, iterate through string from beginning to end  *
	 * If brace given is close brace, iterate through string from end to beginning *
	\*******************************************************************************/
	//If brace given is open brace
	if (str.at(braceIndex) == '[') {
		//Iterate through given string
		for (int i = 0; i < str.length(); i++) {
			//If current character of string is open brace
			if (str.at(i) == '[') {
				//Increment nesting level
				level++;
				//If current character is given brace, assume that matching brace is on the same nesting level
				if (i == braceIndex) {
					returnLvl = level;
				}
			//If current character of string is close brace
			} else if (str.at(i) == ']') {
				//If currnet nesting level is nesting level of matching brace, return current index
				if (level == returnLvl) {
					return i;
				//Otherwise, decrement nesting level and keep going through the string
				} else {
					level--;
				}
			}
		}
	//If brace given is close brace
	} else if (str.at(braceIndex) == ']') {
		//Iterate through given string
		for (int i = str.length() - 1; i >= 0; i--) {
			//If current character of string is close brace
			if (str.at(i) == ']') {
				//Increment nesting level
				level++;
				//If current character is given brace, assume that matching brace is on the same nesting level
				if (i == braceIndex) {
					returnLvl = level;
				}
			//If current character of string is open brace
			} else if (str.at(i) == '[') {
				//If nesting level is nesting level of matching brace, return current index
				if (level == returnLvl) {
					return i;
				//Otherwise, decrement nesting level and keep going through the string
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
	{
		//If program has output anything
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
		//Put a newline after the program has run if the program output anything
		//(Prevents the program's output from being on the same line as the shell's input prompt)
		if (hasOutput) {
			putchar('\n');
		}
	}
}
