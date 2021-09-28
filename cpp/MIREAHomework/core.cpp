#include "core.h"

// Task

Task::Task(std::string _name, std::string _description) {
	name = _name;
	description = _description;
}
bool Task::IsNumber(std::string str) {
	for (char const& c : str) {
		if (std::isdigit(c) == 0) return false;
	}
	return true;
}
void Task::PrintTaskGreetings() {
	Print("\n");
	Print("running task: \"%s\"\n", name.c_str());
	Print("- \"%s\"\n", description.c_str());
}
void Task::Print(char const* _Format, ...) {
	va_list ap;
	va_start(ap, _Format);
	std::vprintf((Task::padding + std::string(_Format)).c_str(), ap);
	va_end(ap);
}
void Task::Prints(std::string _Format, ...)
{
	va_list ap;
	va_start(ap, _Format);
	std::vprintf((Task::padding + _Format).c_str(), ap);
	va_end(ap);
}
void Task::EnterFloat(std::string variableName, float& varRef) {
	Print("%s = ", variableName.c_str());
	std::cin >> varRef;
}
void Task::EnterBool(std::string question, bool& varRef) {
	Print("%s (y/n) your answer:", question.c_str());
	std::string answer;
	std::cin >> answer;
	varRef = answer == "y";
}
void Task::EnterString(std::string statement, std::string& varRef) {
	Print("%s : ", statement.c_str());
	std::cin >> varRef;
}
std::string Task::NumberToString(float number)
{
	std::string sNum = std::to_string(number);
	if (sNum == "-nan(ind)" || sNum == "inf") return "---";
	return sNum;
}
void Task::Run() {
	PrintTaskGreetings();
	RunLogic();
	std::cout << std::endl;
}
bool Task::CheckDivisionByZero(float quotient) {
	if (quotient == 0.0f) {
		Print("cant divide by zero");
		return true;
	}
	return false;
}
bool Task::CheckNumberGreatherThanZero(std::string variableName, float quotient) {
	if (quotient <= 0.0f) {
		Print("%s must be greaher than zero\n", variableName.c_str());
		return true;
	}
	return false;
}
std::string Task::ReadTextFile(std::string filepath)
{
	std::ifstream ifile;
	ifile.open(filepath);
	if (!ifile.is_open()) {
		Print("cant read file\n");
		return "";
	}

	ifile.seekg(0, std::ios::end);
	size_t size = ifile.tellg();
	ifile.seekg(0, std::ios::beg);

	std::stringstream contentsStream;
	contentsStream << ifile.rdbuf();

	ifile.close();
	return contentsStream.str();
}
void Task::RunLogic() {

}
const std::string Task::padding = "    ";



// TaskRunner

TaskRunner::TaskRunner() {}
void TaskRunner::RunTask(int taskIndex) {
	taskIndex = fmaxf(0, fminf(taskIndex, tasks.size() - 1));
	while (true) {
		tasks[taskIndex]->Run();
		system("PAUSE");
	}
}
void TaskRunner::RunAllTasks() {
	for (int i = 0; i < tasks.size(); ++i)
	{
		tasks[i]->Run();
	}
}
void TaskRunner::AddTask(Task* taskPtr) {
	tasks.push_back(std::shared_ptr<Task>(taskPtr));
}
int TaskRunner::GetTasksCount() {
	return tasks.size();
}


// utilities
std::string GenerateNumbersList(int start, int count) {
	std::ostringstream numbersList;
	numbersList << "(";
	for (int i = 0; i < count; ++i)
	{
		int num = i + 1;
		numbersList << num;
		if (i < count - 1) numbersList << ", ";
	}
	numbersList << ")";
	return numbersList.str();
}

void UserLaunchTaskRunner(TaskRunner& taskRunner) {
	std::string list = GenerateNumbersList(1, taskRunner.GetTasksCount());
	Task::Print((std::string("enter task number ") + list + " or \"all\": ").c_str());
	std::string choice; std::cin >> choice;
	if (Task::IsNumber(choice)) {
		int taskIndex = std::stoi(choice) - 1;
		taskIndex = fmax(0, fmin(taskIndex, taskRunner.GetTasksCount() - 1));
		taskRunner.RunTask(taskIndex);
	}
	else {
		taskRunner.RunAllTasks();
	}
}