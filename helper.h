//
// Created by ra_watt on 4/21/20.
//

#ifndef HELPER_H
#define HELPER_H
#include<iostream>
#include <stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<fcntl.h>
#include<errno.h>
#include <fstream>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include<shadow.h>
#include <pwd.h>



void fput(std::string filename);
void fget(std::string filename);
void fput_encrypt(std::string filename);
void fget_decrypt(std::string filename);
void handleErrors();
void get_key_iv(unsigned char * key,unsigned char *iv);
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv, unsigned char *plaintext);
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,unsigned char *iv, unsigned char *ciphertext);
int check_file_exist(std::string filename,struct stat *statbuf);
int check_read_permission(std::string filename);
int check_write_permission(std::string filename);
#endif