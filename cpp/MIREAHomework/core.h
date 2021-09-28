#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <stdarg.h>
#include <vector>
#include <memory>
#include <fstream>

#define PI 3.14159265f


/// <summary>
/// Base class for Tasks. When run, prints its name, description and runs logic.
/// </summary>
class Task {
public:
	static const std::string padding;
	std::string name = "";
	std::string description = "";

	Task(std::string _name = "Task", std::string _description = "Description");

	static bool IsNumber(std::string str);
	void PrintTaskGreetings();
	static void Print(char const* _Format, ...);
	static void Prints(std::string _Format, ...);
	static void EnterFloat(std::string variableName, float& varRef);
	static void EnterBool(std::string question, bool& varRef);
	static void EnterString(std::string statement, std::string& varRef);
	static std::string NumberToString(float number);
	static bool CheckDivisionByZero(float quotient);
	static bool CheckNumberGreatherThanZero(std::string variableName, float quotient);
	static std::string ReadTextFile(std::string filepath);
	void Run();
	virtual void RunLogic();
};


class TaskRunner {
private:
	std::vector<std::shared_ptr<Task>> tasks;

public:
	TaskRunner();

	void RunTask(int taskIndex);
	void RunAllTasks();
	void AddTask(Task* taskPtr);
	int GetTasksCount();
};

std::string GenerateNumbersList(int start, int count);
void UserLaunchTaskRunner(TaskRunner& taskRunner);