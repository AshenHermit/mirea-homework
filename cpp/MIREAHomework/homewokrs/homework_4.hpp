#pragma once
#include "../core.h"
#include <fstream>
#include <ctime>


/// <summary>
/// Loan
/// </summary>
class Task1 : public Task {
public:
	Task1() : Task("File", "Open, write numbers, read, find sum") {}
	void WriteRandomNumbersInFile(std::string filename, int numbersCount) {
		srand(time(0));
		std::ofstream file;
		file.open(filename, std::ios::binary);
		file.write(reinterpret_cast<const char*>(&numbersCount), sizeof(int));
		for (int i = 0; i < numbersCount; i++)
		{
			int number = rand() % 20;
			file.write(reinterpret_cast<const char*>(&number), sizeof(int));
		}
		file.close();
	}
	std::vector<int> ReadNumbersVectorFromFile(std::string filename) {
		std::vector<int> numbers;
		std::ifstream file;
		file.open(filename, std::ios::binary);
		if (!file.is_open()) {
			Print("cant read file");
			return numbers;
		}

		file.seekg(0, std::ios::beg);

		int count = 0;
		file.read(reinterpret_cast<char*>(&count), sizeof(int));
		for (int i = 0; i < count; i++)
		{
			int number = 0;
			file.read(reinterpret_cast<char*>(&number), sizeof(int));
			numbers.push_back(number);
		}
		file.close();
		return numbers;
	}
	void RunLogic() override {
		std::string filename = "./numbers.dat";

		// write
		WriteRandomNumbersInFile(filename, 10);

		// read
		std::vector<int> numbers = ReadNumbersVectorFromFile(filename);
		Print("numbers: \n");
		int sum = 0;
		for (int i = 0; i < numbers.size(); i++)
		{
			Print("%i\n", numbers[i]);
			sum += numbers[i];
		}
		Print("sum of numbers: %i", sum);
		Print("\n\n");
	}
};

/// <summary>
/// Sign
/// </summary>
class Task2 : public Task {
public:
	Task2() : Task("Sign", "") {}
	void RunLogic() override {
		float x;
		EnterFloat("x", x);
		if (x > 0.0f) {
			Print("%f > 0 | x > 0", x);
		}
		else if(x==0.0f){
			Print("%f = 0 | x = 0", x);
		}
		else {
			Print("%f < 0 | x < 0", x);
		}
	}
};

/// <summary>
/// Geometry shapes
/// </summary>
class Task3 : public Task {
public:
	Task3() : Task("Geometry shapes", "and their areas") {}
	float GetCircleArea(float r) {
		return PI * (r * r);
	}
	float GetRectArea(float a, float b) {
		return a * b;
	}
	float GetTriangleArea(float h, float b) {
		return h * b / 2.0f;
	}
	void RunLogic() override {
		Print("enter type of geometry shape: rectangle(r), circle(c), or triangle(t): ");
		std::string answer;
		std::cin >> answer;
		std::cout << std::endl;
		char choice = answer[0];
		if (choice == 'c') {
			float R;
			EnterFloat("radius R", R);
			float area = GetCircleArea(R);
			Print("area of circle with radius %f is %f", R, area);
		}
		else if (choice == 'r') {
			float a, b;
			EnterFloat("side a", a);
			EnterFloat("side b", b);
			float area = GetRectArea(a, b);
			Print("area of rect with side a = %f and b = %f is %f", a, b, area);
		}
		else if (choice == 't') {
			float h, b;
			EnterFloat("height h", h);
			EnterFloat("base b", b);
			float area = GetTriangleArea(h, b);
			Print("area of triangle with height h = %f and base b = %f is %f", h, b, area);
		}
	}
};

/// <summary>
/// USA flag
/// </summary>
class Task4 : public Task {
public:
	Task4() : Task("USA flag", "draws flag") {}
	void RunLogic() override {
		int numberOfRows = 13;
		int numberOfStars = 48;

		int height = numberOfRows;
		int width = 64;
		int starsRectWidth = 16;

		int remainingStars = numberOfStars;

		std::string canvas = "";
		for (int y = 0; y < height; ++y) {
			canvas += "    ";
			for (int x = 0; x < width; ++x) {
				// drawing stars
				if (x < starsRectWidth && remainingStars>0) {
					// a checkmate grid
					if ((x + y) % 2 == 0) {
						canvas += "*";
						remainingStars -= 1;
					}
					else {
						canvas += " ";
					}
				}
				// drawing rows
				else {
					if (y % 2 == 0) canvas += "#";
					else canvas += "-";
				}
			}
			canvas += "\n";
		}

		Prints("\n\n" + canvas);
	}
};

/// <summary>
/// Sin grapgh
/// </summary>
class Task5 : public Task {
public:
	Task5() : Task("Sin grapgh", "sin graph") {}
	void RunLogic() override {
		float scaleX = 1.0f;
		float scaleY = 16.0f;

		int height = ceilf(scaleY * 2.0f);
		int width = 100;

		int remainingStars = numberOfStars;

		std::vector<float> sinValues;
		for (int x = 0; x < width; ++x) {
			sinValues.push_back(sinf(x*scaleX));
		}

		std::string canvas = "";
		for (int y = 0; y < height; ++y) {
			canvas += "    ";
			for (int x = 0; x < width; ++x) {
				//TODO: here. continue...
				if (roundf(sinValues[x]) canvas += "-";
				else canvas += "-";
			}
			canvas += "\n";
		}

		Prints("\n\n" + canvas);
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