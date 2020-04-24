#include "helper.h"


void fput(std::string filename){
    struct stat statbuf;
    std::fstream myfile;
    std::string temp;

    if(check_file_exist(filename,&statbuf)==1){ //check whether file exist or not

        check_write_permission(filename);// exits the program if the file does not have write permission for the user

        myfile.open(filename.c_str(),std::ios::app);
        std::getline(std::cin,temp);
        while(temp.compare("//end")!=0){
            myfile<<temp<<"\n";
            std::getline(std::cin,temp);
        }
        myfile.close();
    }

    else{ //if file does not exist create one
        int fd=creat(filename.c_str(),0644);
        fchown(fd,getuid(),getgid());
        close(fd);
        myfile.open(filename.c_str(),std::ios::out);
        if(!myfile)
        {
            std::cout<<"Error in creating file!!!"<<std::endl;
            exit(-1);
        }

        std::getline(std::cin,temp);
        while(temp.compare("//end")!=0){
            myfile<<temp<<"\n";
            std::getline(std::cin,temp);

        }
        myfile.close();
    }
    fsign(filename);
}


int main(int argc ,char*argv[]){
    if(argc==1){
        std::cout<<"file name is required"<<std::endl;
        return(-1);
    }
    fput(std::string(argv[1]));
}