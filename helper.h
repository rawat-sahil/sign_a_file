//
// Created by ra_watt on 4/21/20.
//

#ifndef HELPER_H
#define HELPER_H
#include<iostream>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<fcntl.h>
#include<errno.h>
#include <fstream>
void fput(std::string filename);
void fget(std::string filename);
int check_file_exist(std::string filename,struct stat *statbuf);
int check_read_permission(std::string filename);
int check_write_permission(std::string filename);
#endif //SE_ASSIGNMENT_4_HELPER_H
