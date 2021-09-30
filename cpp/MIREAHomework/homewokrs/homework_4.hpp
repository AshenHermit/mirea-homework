#pragma once
#include "../core.h"
#include <fstream>
#include <ctime>


/// <summary>
/// File
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
		/*switch (choice) {
		case 'c':
		}*/
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
		float scaleX = 0.2f;
		float scaleY = 10.0f;

		int height = ceilf(scaleY * 2.0f);
		int width = 100;

		std::vector<float> sinValues;
		for (int x = 0; x < width; ++x) {
			sinValues.push_back(-sinf(x*scaleX) * ((float)height/2.0f-1.0f));
		}

		std::string canvas = "";
		for (int y = 0; y < height; ++y) {
			canvas += "    ";
			for (int x = 0; x < width; ++x) {
				float centeredY = (float)y - (float)height / 2.0f;
				float roundValue = roundf(sinValues[x]);
				if (roundValue == roundf(centeredY)) {
					canvas += "#";
					continue;
				}
				if (
					(sinValues[x] >= 0.0f && centeredY < roundValue && centeredY>=0.0f) ||
					(sinValues[x] <=  0.0f && centeredY > roundValue && centeredY<=0.0f)) canvas += ":";
				else canvas += " ";
			}
			canvas += "\n";
		}

		Prints("\n\n" + canvas);
	}
};

/// <summary>
/// Roman nums
/// </summary>
class Task6 : public Task {
public:
	Task6() : Task("Roman nums", "conversion") {}
	int RomanCharValue(char c)
	{
		switch (std::toupper(c)){
			case 'I':  return 1;   case 'V':  return 5;
			case 'X':  return 10;  case 'L':  return 50;
			case 'C':  return 100; case 'D':  return 500; 
			case 'M':  return 1000;
			default: return 0;
		}
	}
	int RomanToInt(const std::string& strNumber)
	{
		int result = 0, prev = 0;
		for (auto pc = strNumber.rbegin(); pc != strNumber.rend(); ++pc)
		{
			int value = RomanCharValue(*pc);
			result += value < prev ? -value : value;
			prev = value;
		}
		return result;
	}
	void RunLogic() override {
		std::string romanNumber = "";
		EnterString("enter roman number", romanNumber);
		int convertedValue = RomanToInt(romanNumber);
		Prints(romanNumber + " = %i", convertedValue);
	}
};

/// <summary>
/// pseudorandom number generator
/// </summary>
class Task7 : public Task {
public:
	Task7() : Task("Pseudorandom number generator", "random numbers") {}
	int PRand(int m = 0, int i = 0, int c = 0) {
		int prev = 0;
		for (int iter = 1; iter < i; ++iter) {
			prev = (m * prev + iter) % c;
		}
		return prev;
	}
	void RunLogic() override {
		Prints("variant I  = %i\n", PRand(37, 3, 64));
		Prints("variant II = %i\n", PRand(25173, 13849, 65537));
	}
};

/// <summary>
/// matrices multiplication
/// </summary>
class Task8 : public Task {
public:
	Task8() : Task("Matrices multiplication", "and some another calculations") {}
	class Matrix {
	public:
		int sizeX;
		int sizeY;
		float** data;
		Matrix(int _sizeY, int _sizeX, float** _data) {
			sizeX = _sizeX;
			sizeY = _sizeY;
			data = _data;
		}
		Matrix(int _sizeY, int _sizeX) {
			sizeX = _sizeX;
			sizeY = _sizeY;
			data = MakeMatrixData(sizeX, sizeY, 0.0f);
		}
		Matrix(int _sizeY, int _sizeX, std::initializer_list<float> list) {
			sizeX = _sizeX;
			sizeY = _sizeY;
			data = MakeMatrixData(sizeX, sizeY, 0.0f);
			int i = 0;
			for (std::initializer_list<float>::iterator iter = list.begin(); iter != list.end(); ++iter) {
				Set(i % sizeX, i / sizeX, *iter);
				i += 1;
			}
		}
		static float** MakeMatrixData(int sizeX, int sizeY, float fillValue) {
			float** data = new float* [sizeY];
			for (int y = 0; y < sizeY; ++y) {
				data[y] = new float[sizeX];
				for (int x = 0; x < sizeX; ++x) {
					data[y][x] = fillValue;
				}
			}
			return data;
		}
		~Matrix() {
			/*for (int y = 0; y < GetSizeY(); ++y) {
				delete[] data[y];
			}
			delete[] data;*/
		}

		float Get(int x, int y) const {
			return data[y][x];
		}
		int GetSizeX() const { return sizeX; }
		int GetSizeY() const { return sizeY; }
		void Set(int x, int y, float value) {
			data[y][x] = value;
		}

		Matrix operator*(const Matrix& B) {
			Matrix result = Matrix(GetSizeY(), B.GetSizeX());
			for (int i = 0; i < GetSizeY(); ++i) {
				for (int j = 0; j < GetSizeX(); ++j) {
					result.Set(j, i, 0.0f);
					for (int k = 0; k < B.GetSizeY(); k++)
					{
						result.Set(j, i, result.Get(j, i) + (Get(k, i) * B.Get(j, k)));
					}
				}
			}
			return result;
		}
		std::string ToString() {
			std::string str;
			int fieldLength = 10;
			for (int y = 0; y < GetSizeY(); ++y) {
				str += "(";
				for (int x = 0; x < GetSizeX(); ++x) {
					std::string field = std::to_string(Get(x, y));
					if (x < GetSizeX() - 1) {
						while (field.size() < fieldLength) field += " ";
						field += " ";
					};
					str += field;
				}
				str += ")\n";
			}
			return str;
		}
	};
	void RunLogic() override {
		Matrix sellers = Matrix(3, 4, { 5,2,0,10,
										3,5,2,5,
										20,0,0,0, });

		Matrix prices = Matrix(4, 2, {	1.20f, 0.50f,
										2.80f, 0.40f,
										5.00f, 1.00f,
										2.00f, 1.50f, });

		Matrix C = sellers * prices;
		Prints("\n" + C.ToString());
		Prints("\n");

		int personMinMoney = 0;
		int personMaxMoney = 0;
		int personMinCommission = 0;
		int personMaxCommission = 0;
		float allCommissions = 0.0f;
		float allPrimaryMoney = 0.0f;

		for (int y = 0; y < C.GetSizeY(); ++y) {
			if (C.Get(0, y)- C.Get(1, y) < C.Get(0, personMinMoney) - C.Get(1, personMinMoney)) personMinMoney = y;
			if (C.Get(0, y)- C.Get(1, y) > C.Get(0, personMaxMoney) - C.Get(1, personMaxMoney)) personMaxMoney = y;

			if (C.Get(1, y) < C.Get(1, personMinCommission)) personMinCommission = y;
			if (C.Get(1, y) < C.Get(1, personMaxCommission)) personMaxCommission = y;

			allCommissions += C.Get(1, y);
			allPrimaryMoney += C.Get(0, y);
		}

		Print("seller %i got min money\n", personMinMoney+1);
		Print("seller %i got max money\n", personMaxMoney+1);
		Print("seller %i got min commissions\n", personMinCommission+1);
		Print("seller %i got max commissions\n", personMaxCommission+1);
		Print("all commisions: %f \n", allCommissions);
		Print("all primary money: %f \n", allPrimaryMoney);
	}
};

/// <summary>
/// numeral system conversion
/// </summary>
class Task9 : public Task {
public:
	Task9() : Task("Numeral system conversion", "") {}
	std::string	vocab = std::string("0123456789ABCDEF");

	std::string Convert(std::string value, int fromBase, int toBase) {
		std::string result;
		int decimal = 0;
		int i = value.size()-1;
		for (std::string::iterator c = value.begin(); c != value.end(); ++c) {
			int v = vocab.find(std::toupper(*c));
			decimal += v * powf((float)fromBase, i);
			i -= 1;
		}
		while (decimal > 0) {
			result += vocab[decimal%toBase];
			decimal = decimal / toBase;
		}
		std::string reversed = "";
		for (i = result.length() - 1; i >= 0; i--)
			reversed += result[i];
		return reversed;
	}
	void RunLogic() override {
		std::string value;
		int fromBase;
		int toBase;


		EnterString("enter value", value);
		EnterInt("enter value base", fromBase);
		if(CheckNumberGreatherThanZero("old base", fromBase)) return;
		EnterInt("enter base to convert value in", toBase);
		if(CheckNumberGreatherThanZero("new base", toBase)) return;
		std::string result = Convert(value, fromBase, toBase);

		Prints(value+" (%i) = "+result+" (%i)", fromBase, toBase);
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
		taskRunner.AddTask(new Task6());
		taskRunner.AddTask(new Task7());
		taskRunner.AddTask(new Task8());
		taskRunner.AddTask(new Task9());

		return taskRunner;
	};
};