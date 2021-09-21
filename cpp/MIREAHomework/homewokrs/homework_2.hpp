#pragma once
#include "../core.h"

/// <summary>
/// Task one, calculates the volume and total surface of the truncated cone
/// </summary>
class Task1 : public Task {
public:
	Task1() : Task("Cone Volume", "calculates the volume and total surface of the truncated cone") {}
	void RunLogic() override {
		float R, r, h;
		EnterFloat("R (r1)", R);
		if (CheckNumberGreatherThanZero("R", R)) return;
		EnterFloat("r (r2)", r);
		if (CheckNumberGreatherThanZero("r", r)) return;
		EnterFloat("h", h);
		if (CheckNumberGreatherThanZero("h", h)) return;

		float L = sqrtf(h * h + powf(R - r, 2.0f));
		float volume = 1.0f / 3.0f * PI * h * (R * R + R * r + r * r);
		float surface = PI * (R * R + (R + r) * L + r * r);

		std::printf("the volume is : %f \n", volume);
		std::printf("the surface is: %f \n", surface);
	}
};

/// <summary>
/// Task two, solves equation bx+c=0
/// </summary>
class Task2 : public Task {
public:
	Task2() : Task("Branching", "solves something") {}
	void RunLogic() override {
		float x, a;
		EnterFloat("x", x);
		EnterFloat("a", a);

		float w = 0.0f;

		if (fabsf(x) < 1.0f) {
			w = a * logf(fabsf(x));
			if (x == 0.0f) {
				Print("w = --");
				return;
			}
		}
		else {
			float inner = a - x * x;
			if (inner <= 0.0f) {
				Print("w = --");
				return;
			}
			w = sqrtf(inner);
		}

		Print("w = %f\n", w);
	}
};

/// <summary>
/// Task three, finds z
/// </summary>
class Task3 : public Task {
public:
	Task3() : Task("Function", "finds z") {}
	void RunLogic() override {
		float x, y, b;
		EnterFloat("b", b);
		EnterFloat("x", x);
		EnterFloat("y", y);
		if (b - y <= 0.0f && b - x <= 0.0f) { Print("--"); return; }
		float z = logf(b - y) * sqrtf(b - x);
		Print("z = %f \n", z);
	}
};

/// <summary>
/// Task four, prints 10 consecutive natural numbers in ascending order, starting at arbitrary number N
/// </summary>
class Task4 : public Task {
public:
	Task4() : Task("Dark Detector", "prints 10 consecutive natural numbers in ascending order, starting at arbitrary number N") {}
	void RunLogic() override {
		float N;
		EnterFloat("N", N);
		if (N < 0.0f || N != round(N)) {
			Print("--\n");
			return;
		}
		int count = 10;
		for (int i = 0; i < count; ++i) {
			int num = (int)roundf(N) + i;
			Print((std::to_string(num) + " ").c_str());
		}
		Print("\n");
	}
};

/// <summary>
/// Task five, calculates function call results
/// </summary>
class Task5 : public Task {
public:
	Task5() : Task("Dark Detector", "calculates function call results") {}
	float Function(float x) {
		if (x - 1.0f != 0.0f) return 0.0f;
		return (x * x + 2.0f * x + 2.0f) / (x - 1.0f);
	}
	void RunLogic() override {
		for (float x = -4.0f; x <= 4.0f; x = x + 0.5f)
		{
			Print("x = %f; F(x) = %f\n", x, Function(x));
		};
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