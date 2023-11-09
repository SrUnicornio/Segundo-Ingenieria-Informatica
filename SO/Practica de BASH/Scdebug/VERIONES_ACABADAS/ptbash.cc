#include <unistd.h>
#include <sys/prctl.h>
#include <vector>
#include <iostream>
extern char ** environ;
const char *command = "/bin/bash";
int main(int argc, char*argv[]){
    std::vector<const char*> exec_data; 
    prctl(PR_SET_PTRACER,PR_SET_PTRACER_ANY);
    exec_data.emplace_back(command);
    for(auto i=1; i<(argc); i++){
        exec_data.emplace_back(argv[i]);
    }
    exec_data.emplace_back(nullptr);
    execve(command,const_cast<char*const *>(exec_data.data()),environ);
    std::cerr<<"Execve failed: "<<errno<<std::endl;
    exit(1);
    
}
