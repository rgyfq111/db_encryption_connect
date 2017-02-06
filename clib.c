#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <regex.h>
#include <openssl/des.h>
#include <mcrypt.h>
#include "clib.h"
/*
	取得ORACLE TNS連線資訊
*/
Oracle_TNS  get_ora_tns(char* tns_name, char* user_name, char* encrytp_key, char* connect_file_path)
{

    Oracle_TNS tns;

    char* IV  = "AAAAAAAAAAAAAAAA";
    //char *key = "0123456789abcdef";
    char * key = encrytp_key;
    int keysize = 16; /* 128 bits */
    char* buffer;
    int buffer_len = 256;

//int i;
    char* server_name = get_server_name();
	//char* server_name = "";

//return tns;
	//char* db_encrytp_connect_string, *encrypted;
	//char key[]=ENCRYPT_KEY;



	char file_name[] = "db_connect.enc";
	char *decrypted;

	int db_str_len = 0;

	char** split;

	int split_num, i;

	char * db_connect_file_path ;


	char * tns_file = ".enc";
//exit(1);
//return tns;
	if( (db_connect_file_path = (char *)malloc(strlen(connect_file_path)+strlen("/")+strlen(server_name)+strlen("/")+strlen(tns_name)+strlen("_")+strlen(user_name)+strlen(tns_file)+1)) != NULL)
	{

//printf("db_connect_file path size %d \n", sizeof(db_connect_file_path));

	    db_connect_file_path[0] = '\0';   // ensures the memory is an empty string
	    // strcat(new_str,tns_name);

	    //ex: /tmp/pos-api/db_connect.enc
	    strcat(db_connect_file_path,connect_file_path);
	    strcat(db_connect_file_path,"/");
	    strcat(db_connect_file_path,server_name);
	    strcat(db_connect_file_path,"/");
	    strcat(db_connect_file_path,tns_name);

	    strcat(db_connect_file_path,"_");

	    strcat(db_connect_file_path,user_name);

	    strcat(db_connect_file_path,tns_file);

//printf( "file path: %s \n", db_connect_file_path );
//exit(1);

	}
	else
	{
	    // fprintf(STDERR,"malloc failed!\n");
	    openlog("uitox_db", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
            syslog(LOG_ERR, "malloc failed \n");
       	    closelog();
	    exit(1);
	}

//return tns;
	//讀取檔案
	buffer = ReadFile(db_connect_file_path);
//printf("%s" , buffer);
//
//openlog("uitox_db", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
//syslog(LOG_ERR, "db_connect_file_path: %s\n", db_connect_file_path);
//closelog();


//exit(1);

	if(buffer == NULL)
	{
		tns.file_path = malloc(strlen(db_connect_file_path));
		strcpy(tns.file_path,db_connect_file_path);
		return tns;
	}

//db_connect_file_path = '\0';
free(db_connect_file_path);
db_connect_file_path = NULL;
//printf("db_connect_file path size %d \n", sizeof(db_connect_file_path));

	//去空白
//	TrimSpaceReturn(buffer);

    	decrypt_aes(buffer, buffer_len, IV, key, keysize);
//free(keysize);

//printf("buffer size %d\n", sizeof(buffer));
   // decrypt_aes(buffer, strlen(buffer), IV, key, keysize);

   // TrimSpaceReturn(buffer);
//i = 0;

//while(i <= strlen(buffer) )
//{
//        printf("\"%c<=>%d\" \n", buffer[i], buffer[i]);
//        i++;

//}



	split = str_split( buffer, ',', &split_num );

   //free(buffer);

    if ( split == NULL )
    {

        openlog("uitox_db", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
        syslog(LOG_ERR, "str_split returned NULL \n");
        closelog();
        exit(1);
        // puts( "str_split returned NULL" );
    }
    else
    {
        // printf( "%i Results: \n", split_num );

        tns.user_name = malloc(strlen(split[1]));
	tns.user_name[strlen(split[1])] = '\0';
        strcpy(tns.user_name,split[1]);
	TrimSpaceReturn(tns.user_name);

// printf("split1=> %d", strlen(split[1]));
        tns.password = malloc(strlen(split[2]));
	tns.password[strlen(split[2])] = '\0';
        strcpy(tns.password,split[2]);
	TrimSpaceReturn(tns.password);

        tns.hostname = malloc(strlen(split[0]));
	tns.hostname[strlen(split[0])] = '\0';
        strcpy(tns.hostname,split[0]);
	TrimSpaceReturn(tns.hostname);


        tns.char_set = malloc(strlen(split[3]));
	tns.char_set[strlen(split[3])] = '\0';
        //strncpy(tns.char_set,split[3],3);
        strcpy(tns.char_set,split[3]);
	TrimSpaceReturn(tns.char_set);


    }
free(split);
//split = NULL;
free(buffer);
//buffer = NULL;
free(server_name);
//server_name = NULL;
	return tns;
}


/*
	字串轉array
*/
char** str_split( char* str, char delim, int* numSplits )
{

    char** ret;
    int retLen;
    char* c;

    if ( ( str == NULL ) || ( delim == '\0' ) )
    {

        ret = NULL;
        retLen = -1;
    }
    else
    {
        retLen = 0;
        c = str;

        do
        {
            // char == ,
            if ( *c == delim )
            {
                // array ++
                retLen++;
            }

            c++;
        } while ( *c != '\0' );

        ret = malloc( ( retLen + 1 ) * sizeof( *ret ) );
        ret[retLen] = NULL;

        c = str;
        retLen = 1;
        ret[0] = str;

        do
        {
            if ( *c == delim )
            {
                ret[retLen++] = &c[1];
                *c = '\0';

            }


            c++;
        } while ( *c != '\0' );

        //strlen(retLen)-1
        //ret[retLen-1][strlen(ret[(retLen-1)])] = 0;


    }

    if ( numSplits != NULL )
    {
        *numSplits = retLen;
    }
//free(c);
    return ret;
}



/*
	取得主機名稱
*/
char* get_server_name()
{
	regex_t regex;
	int reti;
	char msgbuf[100];

	size_t     nmatch = 2;
	regmatch_t pmatch[2];
	char *regex_march_string;
	char *result, *result_tmp;

	char *evn_string;
	char *hostname = get_host_name();


//return  "01234567890123456789";

	//get evn
	evn_string = get_server_environment();

	//online
	if(strcmp("online", evn_string) == 0)
	{
		regex_march_string = "^(.*)[[:digit:]]{2}.idc1.ux";
	}
	//beta
	else if(strcmp("beta", evn_string) == 0)
	{
		regex_march_string = "^(.*)[[:digit:]]{2}.beta1.ux";
	}
	// dev
	else
	{
		regex_march_string = "^(.*)[[:digit:]]{2}.dev1.ux";
	}


	reti = regcomp(&regex, regex_march_string, REG_EXTENDED);

	if (reti)
	{
		openlog("uitox_db", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
       	        syslog(LOG_ERR, "Could not compile regex\n");
        	closelog();
	        exit(1);
	}

	/* Execute regular expression */
	reti = regexec(&regex, hostname, nmatch, pmatch, 0);
	if (!reti)
	{
		result = (char*)malloc(pmatch[1].rm_eo - pmatch[1].rm_so);
//		memset(result, '\0', sizeof(result));
//printf("malloc size =>**%d**\n", pmatch[1].rm_eo - pmatch[1].rm_so);
//printf("test => **%s**\n", &hostname[pmatch[1].rm_so]);

//printf("test2 => **%s**\n", &hostname[pmatch[1].rm_so][12]);
		strncpy(result, &hostname[pmatch[1].rm_so] , (pmatch[1].rm_eo - pmatch[1].rm_so));
		result[pmatch[1].rm_eo - pmatch[1].rm_so] = '\0';

		//strncpy(result, 1 , (pmatch[1].rm_eo - pmatch[1].rm_so));
//printf("result1 => **%s** \n", result);
		/* Free compiled regular expression if you want to use the regex_t again */
		regfree(&regex);

		//get job server name
        	regex_march_string = "^[a-z]{1}(.*-job)";
        	reti = regcomp(&regex, regex_march_string, REG_EXTENDED);

        	reti = regexec(&regex, result, nmatch, pmatch, 0);
        	if (!reti)
        	{
//printf("result1 => **%s** \n", result);
			result_tmp = (char*)malloc( strlen(result));
			strncpy(result_tmp, result + 1, strlen(result)  );

			result_tmp[strlen(result)] = '\0';

			result = result_tmp;
                	regfree(&regex);
//free(result_tmp);
//printf("result_tmp => **%s**\n", result_tmp);
//printf("result2 => **%s**\n", result);
//exit(1);
       		 }
	}
	else if (reti == REG_NOMATCH)
	{
		openlog("uitox_db", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	        syslog(LOG_ERR, "No match\n");
       		 closelog();
       		 exit(1);
    		//puts("No match");
	}
	else
	{

		regerror(reti, &regex, msgbuf, sizeof(msgbuf));

		openlog("uitox_db", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
                syslog(LOG_ERR, "Regex match failed: %s\n", msgbuf);
                closelog();
	    // fprintf(stderr, "Regex match failed: %s\n", msgbuf);
	    exit(1);
	}

//free(evn_string);
//free(hostname);
//hostname = NULL;

//evn_string = NULL;

	return	result;
}

/*
	取得server host name
*/
char * get_host_name()
{
        char *hostname = (char *)malloc(1024);
        //char hostname[1024];
        hostname[1023] = '\0';

        //get host name
        gethostname(hostname, 1023);
        // replace stage- to ""
        hostname = replace(hostname, "stage-", "");
//free(hostname);
	//TrimSpaceReturn(hostname);

	return hostname;
}

/*
	取得server環境
*/
char * get_server_environment()
{
	char * hostname = get_host_name();
	char * evn_string;
	if(strstr(hostname, ".idc1.ux") != NULL)
     	{
        	evn_string = "online";
     	}
	else if(strstr(hostname, ".beta1.ux") != NULL)
	{
		evn_string = "beta";
	}
	else if(strstr(hostname, ".dev1.ux") != NULL)
	{
		evn_string = "dev";
	}
	else
	{
		evn_string = "undefine";
	}

//free(hostname);
//hostname = NULL;
	return evn_string;

}

/*
	字串取代
*/
char * replace(
    char const * const original,
    char const * const pattern,
    char const * const replacement
) {
  size_t const replen = strlen(replacement);
  size_t const patlen = strlen(pattern);
  size_t const orilen = strlen(original);

  size_t patcnt = 0;
  const char * oriptr;
  const char * patloc;

  // find how many times the pattern occurs in the original string
  for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
  {
    patcnt++;
  }

  {
    // allocate memory for the new string
    size_t const retlen = orilen + patcnt * (replen - patlen);
    char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );

    if (returned != NULL)
    {
      // copy the original string,
      // replacing all the instances of the pattern
      char * retptr = returned;
      for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
      {
        size_t const skplen = patloc - oriptr;
        // copy the section until the occurence of the pattern
        strncpy(retptr, oriptr, skplen);
        retptr += skplen;
        // copy the replacement
        strncpy(retptr, replacement, replen);
        retptr += replen;
      }
      // copy the rest of the string.
      strcpy(retptr, oriptr);
    }
    return returned;
  }
}


/*
	AES加密
*/

int encrypt_aes(
    void* buffer,
    int buffer_len, /* Because the plaintext could include null bytes*/
    char* IV,
    char* key,
    int key_len
){
  MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}

  mcrypt_generic_init(td, key, key_len, IV);
  mcrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  return 0;
}


/*
	AES解密
*/
int decrypt_aes(
    void* buffer,
    int buffer_len,
    char* IV,
    char* key,
    int key_len
){
  MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}

  mcrypt_generic_init(td, key, key_len, IV);
  mdecrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
//free(td);
  return 0;
}


/*
	寫檔
*/
int WriteFile(char* file_name, char* connect, int buffer_size)
{
	FILE *fp = fopen(file_name, "w");
	if (fp == NULL)
	{

		openlog("uitox_db", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
        syslog(LOG_ERR, "Error opening file!\n");
        closelog();
	    // printf("Error opening file!\n");
	    exit(1);
	}

	//fprintf(fp, "%s", connect);
    if ((fp == NULL) || (fwrite(connect, buffer_size, 1, fp) != 1)) {

        openlog("uitox_db", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
        syslog(LOG_INFO, "Failed to write data\n");
        closelog();

        exit(EXIT_FAILURE);
    }

	fclose(fp);

	return 0;
}

/*
	讀檔
*/
char *ReadFile(char* file_path)
{
	FILE* fp;
	char ch, * buffer;
	long file_length;
	fp = fopen(file_path, "r");

	if( fp == NULL)
	{
		openlog("uitox_db", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	        syslog(LOG_ERR, "db connect file open fail [%s].\n", file_path);
       		 closelog();

	        return NULL;
        //return buffer;
        // return NULL;
        	exit(EXIT_FAILURE);
	}


	fseek(fp, 0, SEEK_END);
	file_length = ftell(fp);
	rewind(fp);

  //      openlog("uitox_db", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    //    syslog(LOG_ERR, "file_length[%d].\n", file_length);
      //  closelog();

	//buffer = malloc (sizeof(char) *  file_length) + 1;

	//buffer = (char*) malloc (sizeof(char) *  file_length);

	buffer = (char*) malloc(256);

	if(buffer != NULL)
	{
		fread(buffer, 1, file_length, fp);
	}
	else
	{
		openlog("uitox_db", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
        	syslog(LOG_ERR, "malloc builed fail.\n");
	        closelog();
	        exit(EXIT_FAILURE);

	}
//free(buffer);
	fclose(fp);
	return buffer;
}


/*
	去空白字串
*/
void TrimSpaceReturn(char *strStr)
{
    if(strlen(strStr)<1)
    {
        return;
    }

    int i=0;
    int len=strlen(strStr);


    for(i=len-1;i>=0;i--)
    {

           if(strStr[i]==0x20 || strStr[i]== 0x0D || strStr[i]==0x0A ||strStr[i]=='"')
                strStr[i]=0;
           else
                break;
    }

    len=i+1;
    i=0;
    for(i=0;i<len;i++){
        if(strStr[i]!=0x20 && strStr[i]!= 0x0D &&  strStr[i]!=0x0A &&  strStr[i]!='"')
            break;
    }
    if(i==len)
        strcpy(strStr,"");
    else
        strcpy(strStr,strStr+i);


}
