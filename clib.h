#ifndef __CLIB_H__
#define __CLIB_H__

typedef struct Oracle_TNS
{
  char *user_name;
  char *password;
  char *hostname;
  char *char_set;
  char *file_path;
}Oracle_TNS;

Oracle_TNS  get_ora_tns(char* tns_name, char* user_name, char* encrytp_key, char* connect_file_path);
char** str_split( char* str, char delim, int* numSplits );
char* get_server_name();
char * get_host_name();
char * get_server_environment();
char * replace( char const * const original, char const * const pattern, char const * const replacement);
//char * Encrypt( char *Key, char *Msg, int size);
//char * Decrypt( char *Key, char *Msg, int size);

int encrypt_aes( void* buffer, int buffer_len, char* IV, char* key, int key_len);
int decrypt_aes( void* buffer, int buffer_len, char* IV, char* key, int key_len);

int WriteFile(char* file_name, char* connect, int buffer_size);

char *ReadFile(char* file_path);

void TrimSpaceReturn(char *strStr);

#endif
