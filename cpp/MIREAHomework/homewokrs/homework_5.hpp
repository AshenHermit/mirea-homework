#pragma once
#include "../core.h"
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <math.h>

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
/// Text file manipulations
///		my variants are 16 and 32
/// </summary>
class Task2 : public Task {
public:
	Task2() : Task("Text file manipulations", "") {}
	std::string filepath = "text.txt";
	
	char ShiftCharInArea(char c, char areaStart, char areaEnd, int amount) {
		if (c >= areaStart && c <= areaEnd) {
			c = areaStart + (char)Mod((c - areaStart + amount), (areaEnd - areaStart));
		}
		return c;
	}
	std::string EncryptWithShift(std::string text, int amount) {
		char max_value = std::numeric_limits<char>::max();
		for (size_t i = 0; i < text.size(); i++)
		{
			text[i] = ShiftCharInArea(text[i], 'A', 'Z', amount);
			text[i] = ShiftCharInArea(text[i], 'a', 'z', amount);
		}
		return text;
	}
	bool IsAlphabetLetter(char c) {
		c = std::tolower(c);
		return (c >= 'a' && c <= 'z');
	}
	// 16. variant
	void FindOperation() {
		std::string pattern;
		EnterString("enter pattern", pattern, true);
		Prints("\n");

		std::string text = ReadTextFile(filepath);

		size_t prevIndex = 0;
		int resultsCount = 0;
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
				resultsCount += 1;
			}
			prevIndex = index+1;
		}
		Prints("Found %i matches\n", resultsCount);
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
	// 32. variant
	void ShiftEncryptionOperation() {
		int amount = 0;
		EnterInt("amount", amount);
		std::string text = ReadTextFile(filepath);
		text = EncryptWithShift(text, amount);
		bool success = WriteTextInFile(filepath, text);
		if (success) {
			Prints("file successfully updated");
		}
		else {
			Prints("cant update file");
		}
	}
	void RunLogic() override {
		typedef void(Task2::*operationFunction)();

		std::string choice;
		Prints("what do you want to do with contents of file \""+filepath+"\"?\n");
		//TODO: operations list must be generated, with map approach of course
		EnterString("\n(f) find word sequence (my, variant 16) \n(e) encrypt with shift (my, variant 32) \n(s) sort words \nyour choice", choice);

		// switch case
		switch (choice[0])
		{
		case 'f': FindOperation(); break;
		case 'e': ShiftEncryptionOperation(); break;
		case 's': WordsSortOperation(); break;
		default:
			break;
		}

		// map approach
		/*
		std::unordered_map<std::string, operationFunction> operationsMap = std::unordered_map<std::string, operationFunction>
			{ {"f", &Task2::FindOperation}, {"s", &Task2::WordsSortOperation}, {"e", &Task2::ShiftEncryptionOperation} };

		if (operationsMap[choice]) {
			operationFunction func = operationsMap[choice];
			(*this.*func)();
		}
		*/
	}
};

/// <summary>
/// Expressions
/// </summary>
class Task3 : public Task {
public:
	Task3() : Task("Expressions", "solve some expressions") {}
	// 16. variant
	double FirstExpression(double epsilon) {
		double summ = 0.0f;
		double n = 1.0f;
		while (true) {
			double division = 1.0f / std::powf(3.0f, n);
			if (division < epsilon) break;
			double term = division * std::powf(std::cosf(std::powf(3.0f, n - 1.0f)), 3.0f);
			summ += term;
			n += 1.0f;
		}
		return summ;
	}
	void FirstExpressionProgram() {
		double epsilon = 0;
		EnterDouble("epsilon", epsilon);
		double result = FirstExpression(epsilon);
		Prints("y = %e\n", result);
		Prints("  = %f\n", result);
	}

	// 32. variant
	void SecondExpressionProgram() {
		double x = 0;
		EnterDouble("x", x);

		std::string xString = std::to_string((long)x);
		std::string resultString = "";
		for (std::string::iterator ci = xString.begin(); ci != xString.end(); ++ci) {
			char c = *ci;
			if (c != '0' && c != '5'     && c!='.') {
				resultString += c;
			}
		}
		if (resultString == "") {
			Prints("ended up with empty string\n");
			return;
		}

		long result = std::atof(resultString.c_str());
		Prints("result: %i\n", result);
	}

	void RunLogic() override {
		std::string choice;
		Prints("choose expression");
		//TODO: operations list must be generated, with map approach of course
		EnterString("\n(1) variant 16 \n(2) variant 32\nyour choice", choice);
		switch (choice[0])
		{
		case '1': FirstExpressionProgram(); break;
		case '2': SecondExpressionProgram(); break;
		default:
			break;
		}
	}
};

/// <summary>
/// Another operations with text files
/// </summary>
class Task4 : public Task {
public:
	Task4() : Task("Another operations with text files", "some task with files") {}

	//TODO: dont reuse code. it is from 4th homework
	std::string	vocab = std::string("0123456789ABCDEF");
	std::string Convert(std::string value, int fromBase, int toBase) {
		std::string result;
		int decimal = 0;
		int i = value.size() - 1;
		//converting to a decimal
		for (std::string::iterator c = value.begin(); c != value.end(); ++c) {
			int v = vocab.find(std::toupper(*c));
			decimal += v * powf((float)fromBase, i);
			i -= 1;
		}
		//converting to final base
		while (decimal > 0) {
			result += vocab[decimal % toBase];
			decimal = decimal / toBase;
		}
		std::string reversed = "";
		for (i = result.length() - 1; i >= 0; i--)
			reversed += result[i];
		return reversed;
	}
	bool CanConvert(std::string value, int fromBase) {
		for (std::string::iterator c = value.begin(); c != value.end(); ++c) {
			int v = vocab.find(std::toupper(*c));
			if (v >= fromBase) {
				return false;
			}
		}
		return true;
	}

	void NumberBaseConversionProgram() {
		std::string filepath = "hex_numbers.txt";
		int fromBase = 16;
		Prints("File \""+filepath+"\" contains numbers with a radix %i, \n", fromBase);
		Prints("In which radix from 2 to 10 do you want to translate those numbers?\n");
		int toBase = 0;
		EnterInt("radix", toBase);
		if (toBase < 2 || toBase>10) {
			Print("cant convert");
		}

		std::string text = ReadTextFile(filepath);
		std::string resultText = "";
		
	}

	void RunLogic() override {
		NumberBaseConversionProgram();
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

		return taskRunner;
	};
};