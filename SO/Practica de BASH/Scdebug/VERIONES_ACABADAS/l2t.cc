#include <unistd.h>
#include <sys/prctl.h>
#include <vector>
#include <iostream>
#include <string>

extern char ** environ;
std::vector<const char*> exec_data;
std::string command;

int main(int argc, char *argv[]){


	if(argc > 1)
	  command=argv[1];
	else{
	  std::cerr<<"Usage: l2t prog [arg ...]"<<std::endl;
	  exit(1);
	}
	prctl(PR_SET_PTRACER,PR_SET_PTRACER_ANY);
	for(auto i=1; i<argc; i++){
		exec_data.push_back(argv[i]);
	}
	
	exec_data.push_back(nullptr);
	execvpe(command.c_str(),const_cast<char* const *>(exec_data.data()),environ);
	std::cerr<<"Execve failed: "<<errno<<std::endl;
	exit(errno);
}
	