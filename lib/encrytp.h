#ifndef ENCRYTP_H
#define ENCRYTP_H
char* Encrypt(char* key, char* Msg, int size);
char* Decrypt(char* key, char* Msg, int szie);


int encrypt_aes(
    void* buffer,
    int buffer_len, /* Because the plaintext could include null bytes*/
    char* IV,
    char* key,
    int key_len
);

int decrypt_aes(
    void* buffer,
    int buffer_len,
    char* IV,
    char* key,
    int key_len
);

#endif