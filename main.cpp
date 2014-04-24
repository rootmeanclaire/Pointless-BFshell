#include <iostream>
#include "color.h"
#include "parser.h"

using namespace std;

/**************************\
 * Version: 1.2.3 Beta    *
 * Author: Evan Shimoniak *
\**************************/

bool isCode(string chars);
void processCommand(string line);

int main() {
	//Create a character array to store user input
	string line;

	do {
		//Print input prompt
		cout << "~ ";

		//Read user input upon entering a newline
		getline(cin, line);
		//If line contains a syntax character([]+-,.<>), then parse as syntax and execute
		if (isCode(line)) {
			exec(line);
		} else {
			//If line contains no syntax characters, then attempt processing as an interpreter command
			processCommand(line);
//				char** splitStr = splitStrBySpaces(line);
//				int i = 0;
//
//				for (i; i < sizeof(splitStr); i++) {
//					printf("%s\n", splitStr[i]);
//				}
		}
	} while (true);


	return 0;
}

bool isCode(string str) {
	for (unsigned int i = 0; i < str.length(); i++) {
		if (str.at(i) == '+' ||
			str.at(i) == '-' ||
			str.at(i) == '>' ||
			str.at(i) == '<' ||
			str.at(i) == '[' ||
			str.at(i) == ']' ||
			str.at(i) == ',' ||
			str.at(i) == '.') {
				return true;
		}
	}

	return false;
}

void processCommand(string line) {
	if (line == "help") {
		cout << "COMMANDS\n" << "\t\"version\" Print the current version of the interpreter\n" << "\t\"reset\"   Reset program memory\n";
	} else if (line == "version") {
		cout << "Beta 1.2.3";
	} else if (line == "reset") {
		reset();
		setColor(BLACK, DEFAULT);
		cout << "Memory reset\n";
		setColor(DEFAULT, BLACK);
	} else if (line == "") {
		return;
	} else {
		cout << "Unrecognized command\n";
	}
}
