#include "helper.h"


void fput_encrypt(std::string filename) {
    /* A 256 bit key */
    unsigned char key[33];

    /* A 128 bit IV */
    unsigned char iv[17] ;
    get_key_iv(key,iv);
    std::cout<<key<<"    "<<iv<<"\n";

    struct stat statbuf;
    std::fstream myfile;
    std::string temp;
    unsigned char ciphertext[200];
    int ciphertext_len;
    if (check_file_exist(filename, &statbuf) == 1) { //check whether file exist or not

        check_write_permission(filename);// exits the program if the file does not have write permission for the user

        myfile.open(filename.c_str(), std::ios::app);

        std::getline(std::cin, temp);
        while (temp.compare("//end") != 0) {
            ciphertext_len = encrypt((unsigned char *)temp.c_str(), strlen(temp.c_str()), key, iv, ciphertext);
            myfile << ciphertext << "\n";
            std::getline(std::cin, temp);
        }
        myfile.close();
    }
    else { //if file does not exist create one
        myfile.open(filename.c_str(), std::ios::out);
        if (!myfile) {
            std::cout << "Error in creating file!!!" << std::endl;
            exit(-1);
        }
        std::getline(std::cin, temp);
        while (temp.compare("//end") != 0) {
            ciphertext_len = encrypt((unsigned char *)temp.c_str(), strlen(temp.c_str()), key, iv, ciphertext);
            ciphertext[ciphertext_len]='\0';
            myfile << ciphertext << "\n";
            std::getline(std::cin, temp);
        }
        myfile.close();


    }
    fsign(filename);
}


int main(int argc,char* argv[]){
    if(argc==1){
        std::cout<<"file name is required"<<std::endl;
        return(-1);
    }
    fput_encrypt(std::string(argv[1]));
}