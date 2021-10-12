#pragma once
#include "../core.h"
#include <fstream>
#include <unordered_map>
#include <algorithm>

/// <summary>
/// Max common divider
/// </summary>
class Task1 : public Task {
public:
	Task1() : Task("Max common divider", "") {}
	int FindMCDWithDivision(int a, int b) {
		while (a != 0 && b != 0) {
			if (a > b) {
				a = a % b;
			}
			else {
				b = b % a;
			}
		}
		return a + b;
	}
	int FindMCDWithSubstraction(int a, int b) {
		while (a != b) {
			if (a > b) {
				a = a - b;
			}
			else {
				b = b - a;
			}
		}
		return a;
	}
	void RunLogic() override {
		int a, b;
		EnterInt("a", a);
		EnterInt("b", b);
		Print("MCD of %i and %i calculated with division: %i\n", a, b, FindMCDWithDivision(a, b));
		Print("MCD of %i and %i calculated with substraction: %i\n", a, b, FindMCDWithSubstraction(a, b));
	}
};

/// <summary>
/// Text file encryption
/// </summary>
class Task2 : public Task {
public:
	Task2() : Task("Text file manipulations", "") {}

	std::string filepath = "text.txt";

	// actually, my variants are 

	// 32. variant
	std::string EncryptWithReplacement(std::string text) {
		for (size_t i = 0; i < text.size(); i++)
		{

		}
	}
	// 31. variant
	std::string EncryptWithShift(std::string text, int amount) {
		char max_value = std::numeric_limits<char>::max();
		for (size_t i = 0; i < text.size(); i++)
		{
			if(text[i] >= 'A' && text[i] <= 'z')
			text[i] = (text[i]+amount) % max_value;
		}
		return text;
	}
	bool IsAlphabetLetter(char c) {
		c = std::tolower(c);
		return (c >= 'a' && c <= 'z');
	}
	void FindOperation() {
		std::string pattern;
		EnterString("enter pattern", pattern, true);
		Prints("\n");

		std::string text = ReadTextFile(filepath);

		size_t prevIndex = 0;
		while (true) {
			size_t index = text.find(pattern, prevIndex);
			if (index == std::string::npos) break;
			int busWidth = 20;
			std::string pice = text.substr(
				std::max(0, (int)index - busWidth), 
				std::min((int)text.size()-1 - (int)index, (int)pattern.size() + busWidth*2));

			char prev = text[std::max(0, (int)index-1)];
			char next = text[std::min((int)text.size()-1, (int)index+(int)pattern.size())];

			if (!IsAlphabetLetter(prev) && !IsAlphabetLetter(next)) {
				Prints(pice + "\n");
			}
			prevIndex = index+1;
		}
	}
	void SortInAlphabetOrder(std::vector<std::string> &words) {
		for (int i = 0; i < words.size(); ++i) {
			for (int j = i; j < words.size(); ++j) {
				if (std::tolower(words[j][0]) < std::tolower(words[i][0])) {
					std::string tmp = words[i];
					words[i] = words[j];
					words[j] = tmp;
				}
			}
		}
	}
	int GetIndexToCompare(size_t index) {
		if (index == std::string::npos) return std::numeric_limits<int>::max();
		return (int)index;
	}
	std::vector<std::string> SplitWords(std::string text){
		std::vector<std::string> words = std::vector<std::string>();
		int preIndex = 0;
		while (true) {
			int findResult = std::min({
				GetIndexToCompare(text.find(" ", preIndex)),
				GetIndexToCompare(text.find("	", preIndex)),
				GetIndexToCompare(text.find("\n", preIndex)),
				GetIndexToCompare(text.find("\r", preIndex)),
			});
			if (findResult == std::numeric_limits<int>::max()){
				break;
			}
			words.push_back(text.substr(preIndex, findResult-preIndex));
			preIndex = findResult + 1;
		}
		return words;
	}
	void WordsSortOperation(){
		std::string text = ReadTextFile(filepath);
		std::vector<std::string> words = SplitWords(text);
		SortInAlphabetOrder(words);
		for (int i = 0; i < words.size(); ++i) {
			Prints(words[i]);
		}
	}
	void ShiftEncryptionOperation() {

	}
	void CharReplacementEncryptionOperation() {

	}
	void RunLogic() override {
		typedef void(Task2::*operationFunction)();
		std::string choice;
		Prints("what do you want to do with file \""+filepath+"\"?\n");
		EnterString("\n(f) find word sequence, \n(s) sort words, \n(er) encrypt with char replacement, \n(es) encrypt with shift \nyour choice", choice);

		std::unordered_map<std::string, operationFunction> operationsMap = std::unordered_map<std::string, operationFunction>
			{ {"f", &Task2::FindOperation}, {"s", &Task2::WordsSortOperation}, {"er", &Task2::CharReplacementEncryptionOperation}, {"es", &Task2::ShiftEncryptionOperation} };

		if (operationsMap[choice]) {
			operationFunction func = operationsMap[choice];
			(*this.*func)();
		}
	}
};

/// <summary>
/// Expressions
/// </summary>
class Task3 : public Task {
public:
	Task3() : Task("File", "Printing a file contents") {}
	void RunLogic() override {
		std::string filepath = "./dummy_file.txt";

		// write
		std::ofstream ofile;
		ofile.open(filepath);
		ofile << "i'm a file \"" << filepath << "\"" << "\n";
		ofile << std::to_string(32.5f);
		ofile.close();

		// read
		std::string contents = ReadTextFile(filepath);
		if (contents == "") return;

		Print("characters count: %i\n", contents.size());
		Prints("file contents:\n\n"+ contents + "\n\n");
		Print("\n");
	}
};

/// <summary>
/// Filter
/// </summary>
class Task4 : public Task {
public:
	void GenerateMessyFile(std::string filepath) {
		srand(time(0));
		int length = 64;

		std::ofstream ofile;
		ofile.open(filepath);
		for (int i = 0; i < length; ++i) {
			char character = 0;
			if (rand() % 4 > 1) {
				character = 'a' + rand() % 26;
			}
			else {
				character = '0' + rand() % 10;
			}
			ofile << character;
		}
		ofile.close();
	}
	Task4() : Task("Filter", "Display only numbers from a text file you created on disk containing letters and numbers.") {}
	void RunLogic() override {
		std::string filepath = "./messy_fil.txt";
		//GenerateMessyFile(filepath);
		std::string text = ReadTextFile(filepath);
		if (text == "") return;

		Print("found numbers: \n\n");

		std::string numberSequence = "";
		for (std::string::iterator it = text.begin(); it != text.end(); ++it) {
			char c = *it;
			if (std::isdigit(c)) {
				numberSequence.push_back(c);
			}
			else {
				if (numberSequence != "") {
					Print("%i\n", std::atoi(numberSequence.c_str()));
					numberSequence = "";
				}
			}
		}
		Print("\n\n");
	}
};

/// <summary>
/// Characters Sort
/// </summary>
class Task5 : public Task {
public:
	Task5() : Task("Characters Sort", "sort chars in string") {}
	std::string SortChars(std::string str) {
		for (int i = 0; i < str.size()-1; ++i) {
			for (int j = i+1; j < str.size(); ++j) {
				if (str[j] < str[i]) {
					char tmpChar = str[i];
					str[i] = str[j];
					str[j] = tmpChar;
				}
			}
		}
		return str;
	}
	void RunLogic() override {
		std::string letters = "axoiqwlkgboauiweadc";
		std::string sorted = SortChars(letters);
		Prints(sorted+"\n");
	}
};

class Homework {
public:
	Homework() {}
	TaskRunner MakeTaskRunner() {
		TaskRunner taskRunner = TaskRunner();
		taskRunner.AddTask(new Task1());
		taskRunner.AddTask(new Task2());
		taskRunner.AddTask(new Task3());
		taskRunner.AddTask(new Task4());
		taskRunner.AddTask(new Task5());

		return taskRunner;
	};
};