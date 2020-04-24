#include "helper.h"

void fget(std::string filename){
    check_read_permission(filename); // exits the program either the file does not exist or does not have read permission
    fverify(filename);
    std::ifstream myfile;
    myfile.open(filename.c_str());
    std::string line;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            std::cout << line<<"\n";
        }
        myfile.close();
    }

    else std::cout << "Unable to open file";

}

int main(int argc ,char*argv[]){
    if(argc==1){
        std::cout<<"file name is required"<<std::endl;
        return(-1);
    }
    fget(std::string(argv[1]));
}