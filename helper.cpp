//
// Created by ra_watt on 4/21/20.
//
#include "helper.h"


void fput(std::string filename){

}

void fget(std::string filename){

}

int check_file_exist(std::string filename,struct stat *statbuf){
    int value=stat(filename.c_str(),statbuf);
    if(value==0)return 1;
    std::cout<<"error occured  "<<strerror(errno)<<std::endl;
    exit(errno);
}

int check_read_permission(std::string filename){
    int permission=access(filename.c_str(),R_OK);
    if(permission==0)return 1;
    std::cout<<"error occured  "<<strerror(errno)<<std::endl;
    exit(errno);
}

int check_write_permission(std::string filename){
    int permission=access(filename.c_str(),W_OK);
    if(permission==0)return 1;
    std::cout<<"error occured  "<<strerror(errno)<<std::endl;
    exit(errno);
}
