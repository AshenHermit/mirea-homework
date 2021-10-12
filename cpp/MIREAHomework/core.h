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
	static bool ProcessCInError();
	static void EnterFloat(std::string variableName, float& varRef);
	static void EnterDouble(std::string variableName, double& varRef);
	static void EnterInt(std::string variableName, int& varRef);
	static void EnterBool(std::string question, bool& varRef);
	static void EnterString(std::string statement, std::string& varRef, bool useGetLine=false);
	static bool IsNanOrInfinity(float number);
	static bool IsNanOrInfinity(double number);

	static std::string NumberToString(float number);
	static std::string NumberToString(double number);
	static bool CheckDivisionByZero(float quotient);
	static bool CheckNumberGreatherThanZero(std::string variableName, float quotient);
	static std::string ReadTextFile(std::string filepath);
	static std::string FixedLengthString(std::string str, int length);
	void Run();
	virtual void RunLogic();
};

//TODO: make also test runner
class TaskRunner {
public:
	typedef std::shared_ptr<Task> TaskPtr;
private:
	std::vector<std::shared_ptr<Task>> tasks;

public:
	TaskRunner();

	void RunTask(int taskIndex);
	void RunAllTasks();
	void AddTask(Task* taskPtr);
	int GetTasksCount();
	std::string MakeTasksTable();
};

std::string GenerateNumbersList(int start, int count);
std::string GenerateNumbersList(const std::vector<int> & list);
void UserLaunchTaskRunner(TaskRunner& taskRunner);