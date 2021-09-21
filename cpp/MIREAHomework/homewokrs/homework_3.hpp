#pragma once
#include "../core.h"
#include <fstream>
#include <ctime>


/// <summary>
/// Loan
/// </summary>
class Task1 : public Task {
public:
	Task1() : Task("Loan", "Monthly payment m on a loan of S rubles for n years at percent p") {}
	void RunLogic() override {
		float S, p, n;
		EnterFloat("S", S);
		if (CheckNumberGreatherThanZero("S", S)) return;
		EnterFloat("p", p);
		if (CheckNumberGreatherThanZero("p", p)) return;
		EnterFloat("n", n);
		if (CheckNumberGreatherThanZero("n", n)) return;

		float r = p / 100.0f;
		float m = (S * r * powf((1 + r), n)) / (12.0f * (powf(1.0f + r, n) - 1.0f));

		Prints(std::string("m = \n")+NumberToString(m));
	}
};

/// <summary>
/// Second Loan
/// </summary>
class Task2 : public Task {
public:
	Task2() : Task("Second Loan", "At what percentage p a loan of S rubles was issued, which is repaid by monthly payments of size m over n years.") {}
	float CountM(float S, float p, float n) {
		float r = p / 100.0f;
		float m = (S * r * powf((1 + r), n)) / (12.0f * (powf(1.0f + r, n) - 1.0f));
		return m;
	}
	void RunLogic() override {
		float S, m, n;
		EnterFloat("S", S);
		if (CheckNumberGreatherThanZero("S", S)) return;
		EnterFloat("m", m);
		if (CheckNumberGreatherThanZero("m", m)) return;
		EnterFloat("n", n);
		if (CheckNumberGreatherThanZero("n", n)) return;

		float offset = 0.0f;
		float width = 400.0f;
		float resultM = 0.0f;
		float p = 0.0f;

		while (fabsf(resultM - m) > 0.01f && width>0.0f) {
			p = offset + width/2.0f;
			resultM = CountM(S, p, n);

			Print("offset = %f  width = %f | ", offset, width);
			std::string message = std::string("p = %f  m = ") + NumberToString(resultM);
			Prints(message + "\n", p);

			if (m < resultM) {
				width /= 2.0f;
			}
			else {
				offset += width / 2.0f;
				width /= 2.0f;
			}
		}
		Prints("p = %f\n", p);
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