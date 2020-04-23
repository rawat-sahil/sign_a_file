//
// Created by ra_watt on 4/21/20.
//
#include "helper.h"

void get_key_iv(unsigned char *key,unsigned char *iv){
    int uid=getuid();
    char * username=getpwuid(uid)->pw_name;
    char* hashed_password=getspnam(username)->sp_pwdp;
//    std::cout<<hashed_password<<"  len:"<<strlen(hashed_password)<<"\n";
    unsigned char out[200];
    int len=49;
    std::cout << PKCS5_PBKDF2_HMAC_SHA1(hashed_password, strlen(hashed_password), nullptr, 0, 1000, len, out) << std::endl;
    out[len]='\0';
//    std::cout << out <<strlen((char *)out)<< "\n";

    strncpy((char*)key,(char *)out,32);
    key[32]='\0';
    strncpy((char*)iv,(char *)(out+32),17);
    iv[17]='\0';
}


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

        myfile.open(filename.c_str(), std::fstream::app);

        std::getline(std::cin, temp);
        while (temp.compare("//end") != 0) {
            ciphertext_len = encrypt((unsigned char *)temp.c_str(), strlen(temp.c_str()), key, iv, ciphertext);
            myfile << ciphertext << "\n";
            std::getline(std::cin, temp);
        }
        myfile.close();
    }
    else { //if file does not exist create one
        myfile.open(filename.c_str(), std::fstream::out);
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
}

void fget_decrypt(std::string filename){
    /* A 256 bit key */
    unsigned char key[33];

    /* A 128 bit IV */
    unsigned char iv[17] ;
    get_key_iv(key,iv);
    std::cout<<key<<"    "<<iv<<"\n";


    unsigned char decryptedtext[200];
    int  decryptedtext_len;

    check_read_permission(filename); // exits the program either the file does not exist or does not have read permission
    std::ifstream myfile;
    myfile.open(filename.c_str());
    std::string line;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            decryptedtext_len=decrypt((unsigned char *)line.c_str(),strlen(line.c_str()),key,iv,decryptedtext);
            decryptedtext[decryptedtext_len]='\0';
            std::cout<<decryptedtext<<std::endl;
        }
        myfile.close();
    }

    else std::cout << "Unable to open file";
}


void fput(std::string filename){
    struct stat statbuf;
    std::fstream myfile;
    std::string temp;

    if(check_file_exist(filename,&statbuf)==1){ //check whether file exist or not

        check_write_permission(filename);// exits the program if the file does not have write permission for the user

        myfile.open(filename.c_str(),std::fstream::app);
        std::getline(std::cin,temp);
        while(temp.compare("//end")!=0){
            myfile<<temp<<"\n";
            std::getline(std::cin,temp);
        }
        myfile.close();
    }

    else{ //if file does not exist create one
        myfile.open(filename.c_str(),std::fstream::out);
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
}

void fget(std::string filename){
    check_read_permission(filename); // exits the program either the file does not exist or does not have read permission
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

int check_file_exist(std::string filename,struct stat *statbuf){ // return 1 if file exist
    int value=stat(filename.c_str(),statbuf);
    if(value==0)return 1;
    return 0;
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


void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext){
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext){
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}