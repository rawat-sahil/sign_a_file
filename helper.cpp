//
// Created by ra_watt on 4/21/20.
//
#include "helper.h"



void fsign(std::string filename){
    unsigned char key[33];
    unsigned char iv[17];
    unsigned char *result;
    int result_len = 32;
    struct stat statbuf;
    check_file_exist(filename, &statbuf);
    get_key_iv(key,iv,statbuf.st_uid);

    //reading contents of the file to be signed
    std::fstream myfile;
    myfile.open(filename.c_str(),std::ios::in);
    std::string line,wholefile,signatureFile;
    signatureFile=std::string(filename);
    signatureFile.append(".sign");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            wholefile.append(line);
        }
        myfile.close();
    }

    //creating a signatur3 file
    int fd=creat(signatureFile.c_str(),0664);
    fchown(fd,statbuf.st_uid,statbuf.st_gid);
    close(fd);
    myfile.open(signatureFile.c_str(),std::ios::out|std::ios::binary);

    //generating signature in result variable
    result = HMAC(EVP_sha256(), key, strlen((char *)key), (unsigned char *)(wholefile.c_str()), strlen((char *)(wholefile.c_str())), NULL, NULL);
    myfile<<result<<"\n";
//    std::cout<<result<<"\n";
    myfile.close();


}
void fverify(std::string filename){
    unsigned char key[33];
    unsigned char iv[17];
    unsigned char *result;
    std::string expected;
    int result_len = 32;

    struct stat statbuf;
    check_file_exist(filename, &statbuf);
    check_read_permission(filename);
    get_key_iv(key,iv,statbuf.st_uid);

    //reading contents of the to be verified
    std::fstream myfile;
    myfile.open(filename.c_str(),std::ios::in | std::ios::binary);
    std::string line,wholefile,signatureFile;
    signatureFile=std::string(filename);
    signatureFile.append(".sign");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            wholefile.append(line);
        }
        myfile.close();
    }

    //generating hmac for file again
    result = HMAC(EVP_sha256(), key, strlen((char *)key), (unsigned char *)(wholefile.c_str()), strlen((char *)(wholefile.c_str())), NULL, NULL);

    //reading signature file already stored
    myfile.open(signatureFile.c_str(),std::ios::in | std::ios::binary);
    getline(myfile,expected);

//    std::cout<<expected<<"\n";
//    std::cout<<res_hexstring<<"\n";



//comparing the two signatures
    for(int i=0;i<32;++i){
        if((unsigned char)expected[i]!=(unsigned char)result[i]){

            std::cout<<"signature not verified\n";
            exit(-1);
        }
    };
    std::cout<<"signature verified\n";
    myfile.close();
}



void get_key_iv(unsigned char *key,unsigned char *iv,int uid){
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

    std::cout<<key<<"\n";
    std::cout<<iv<<"\n";
}

int check_file_exist(std::string filename,struct stat *statbuf){ // return 1 if file exist
    int value=stat(filename.c_str(),statbuf);
    if(value==0)return 1;
    return 0;
}

int check_read_permission(std::string filename){
    int permission=access(filename.c_str(),R_OK);
    if(permission==0)return 1;
    std::cout<<"error occured in check read permission "<<strerror(errno)<<std::endl;
    exit(errno);
}

int check_write_permission(std::string filename){
    int permission=access(filename.c_str(),W_OK);
    if(permission==0)return 1;
    std::cout<<"error occured  in check write permission "<<strerror(errno)<<std::endl;
    exit(errno);
}



int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv, unsigned char *plaintext){
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    // Create and initialise the context
    ctx = EVP_CIPHER_CTX_new();

    //decrypt initialisation using aes 256 ecb
    EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, iv);

    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;
    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext){
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;
    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}