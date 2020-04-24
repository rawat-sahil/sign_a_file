//
// Created by ra_watt on 4/21/20.
//
#include "helper.h"



void fsign(std::string filename){
    /* A 256 bit key */
    unsigned char key[33];

    /* A 128 bit IV */
    unsigned char iv[17] ;
    unsigned char *result;
    int result_len = 32;
    static char res_hexstring[32];
    struct stat statbuf;
    check_file_exist(filename, &statbuf);
    get_key_iv(key,iv,statbuf.st_uid);

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
    int fd=creat(signatureFile.c_str(),0664);
    fchown(fd,statbuf.st_uid,statbuf.st_gid);
    close(fd);
    myfile.open(signatureFile.c_str(),std::ios::out|std::ios::binary);

    result = HMAC(EVP_sha256(), key, strlen((char *)key), (unsigned char *)(wholefile.c_str()), strlen((char *)(wholefile.c_str())), NULL, NULL);
//    for (int i = 0; i < result_len; i++) {
//        sprintf(&(res_hexstring[i * 2]), "%02x", result[i]);
//    }
//    myfile<<res_hexstring<<"\n";
    myfile<<result<<"\n";
//    std::cout<<result<<"\n";
    myfile.close();


}
void fverify(std::string filename){
    /* A 256 bit key */
    unsigned char key[33];

    /* A 128 bit IV */
    unsigned char iv[17] ;
    unsigned char *result;
    std::string expected;
    int result_len = 32;
    static char res_hexstring[32];

    struct stat statbuf;
    check_file_exist(filename, &statbuf);
    get_key_iv(key,iv,statbuf.st_uid);

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

    result = HMAC(EVP_sha256(), key, strlen((char *)key), (unsigned char *)(wholefile.c_str()), strlen((char *)(wholefile.c_str())), NULL, NULL);
//    for (int i = 0; i < result_len; i++) {
//        sprintf(&(res_hexstring[i * 2]), "%02x", result[i]);
//    }


    myfile.open(signatureFile.c_str(),std::ios::in | std::ios::binary);
    getline(myfile,expected);

//    std::cout<<expected<<"\n";
//    std::cout<<res_hexstring<<"\n";

    for(int i=0;i<32;++i){
//        if(expected[i]!=res_hexstring[i]){
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