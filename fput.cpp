#include "helper.h"


int main(int argc ,char*argv[]){
    struct stat statbuf;
    if(check_file_exist(std::string(argv[1]),&statbuf)==1)std::cout<<"success";
}