#include "helper.h"

int main(int argc ,char*argv[]){
    if(argc==1){
        std::cout<<"file name is required"<<std::endl;
        return(-1);
    }
    fsign(std::string(argv[1]));
}