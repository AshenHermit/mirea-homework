#pragma once
#include "../core.h"
#include <fstream>


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
	Task2() : Task("Text file encryption", "") {}
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
	}
	void RunLogic() override {
		int limit;
		EnterInt("simple numbers limit", limit);
	}
};

/// <summary>
/// File
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