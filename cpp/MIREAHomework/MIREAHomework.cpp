//#include "homewokrs/homework_2.hpp" 
//#include "homewokrs/homework_3.hpp"
#include "homewokrs/homework_4.hpp"

int main(int argc, char* argv[]) {
	Homework homework = Homework();
	TaskRunner taskRunner = homework.MakeTaskRunner();
	UserLaunchTaskRunner(taskRunner);
}