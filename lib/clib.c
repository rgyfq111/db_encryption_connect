#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
    int buffer_len = 128;


	char* server_name = get_server_name();
	char* db_encrytp_connect_string, *encrypted;
	//char key[]=ENCRYPT_KEY;
	


	char file_name[] = "db_connect.enc";
	char *decrypted;
	
	int db_str_len = 0;

	char** split;

	int split_num, i;

	char * db_connect_file_path ;

	
	char * tns_file = ".enc";
	if((db_connect_file_path = malloc(strlen(connect_file_path)+strlen("/")+strlen(server_name)+strlen("/")+strlen(tns_name)+strlen("_")+strlen(user_name)+strlen(tns_file)+1)) != NULL)
	{
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

	} 
	else 
	{
exit(1);
	}

	//讀取檔案
	buffer = ReadFile(db_connect_file_path);

	//去空白
	TrimSpaceReturn(buffer);

    decrypt_aes(buffer, buffer_len, IV, key, keysize);



	split = str_split( buffer, ',', &split_num );

    if ( split == NULL )
    {
        puts( "str_split returned NULL" );
    }
    else
    {
        // printf( "%i Results: \n", split_num );

        tns.user_name = malloc(strlen(split[1]));
        strcpy(tns.user_name,split[1]);
// printf("split1=> %d", strlen(split[1]));
        tns.password = malloc(strlen(split[2]));
        strcpy(tns.password,split[2]);

        tns.hostname = malloc(strlen(split[0]));
        strcpy(tns.hostname,split[0]);

        tns.char_set = malloc(strlen(split[3]));
        strcpy(tns.char_set,split[3]);

free(split[0]);
free(split[1]);
free(split[2]);
free(split[3]);
        //strncpy(tns.char_set, split[3], 4);
// i=0;
// while(1)
// {
//     printf("testestest0**==> %c \n", split[3][i]);
//     i++;

//     if(strlen(split[3]) == i)
//         break;    
// }

// printf("testestest0==> %s \n", split[3]);
// printf("testestest1==> %d \n", strlen(split[3]));
// printf("testestest2==> %s \n", tns.char_set);

 //printf("tns.char_set=>%s<=",tns.char_set);

        // for ( i = 0; i < split_num; i++ )
        // {
        //     puts( split[i] );
        // }
    }



    // free( split );
    // free( decrypted );
    // free(db_connect_file_path);

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
//printf("\n***************check test=>%s<<=", str);
    if ( ( str == NULL ) || ( delim == '\0' ) )
    {
        /* Either of those will cause problems */
        ret = NULL;
        retLen = -1;
    }
    else
    {
        retLen = 0;
        c = str;

        /* Pre-calculate number of elements */
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
//printf("array size: %d \n", retLen);
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
  //printf("test=>>??%s??<<==", ret[0]);
            }


            c++;
        } while ( *c != '\0' );

        //strlen(retLen)-1
        ret[retLen-1][strlen(ret[(retLen-1)])] = 0;
        // printf("test11=>>%s", ret[retLen] );

    }

    if ( numSplits != NULL )
    {
        *numSplits = retLen;
    }

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
	char *result;

	char *evn_string;
	char *hostname = get_host_name();
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


//printf("evn string => %s <===", evn_string);

	reti = regcomp(&regex, regex_march_string, REG_EXTENDED);

//printf("test=>%d\n" , REG_EXTENDED);
	if (reti) 
	{
	    fprintf(stderr, "Could not compile regex\n");
	    exit(1);
	}
//char * match_string = "pos-api01.dev1.ux";
	/* Execute regular expression */
	reti = regexec(&regex, hostname, nmatch, pmatch, 0);
	if (!reti) 
	{
		result = (char*)malloc(pmatch[1].rm_eo - pmatch[1].rm_so);
		strncpy(result, &hostname[pmatch[1].rm_so] , pmatch[1].rm_eo - pmatch[1].rm_so);

	//	printf("get string=>>\"%s\" <<<===", result );

		/* Free compiled regular expression if you want to use the regex_t again */
		regfree(&regex);
 	   	//puts("Match");
	}
	else if (reti == REG_NOMATCH) 
	{
    		puts("No match");
	}
	else 
	{
		regerror(reti, &regex, msgbuf, sizeof(msgbuf));
	    	fprintf(stderr, "Regex match failed: %s\n", msgbuf);
	    	exit(1);
	}

		
	return	result;
}

/*
	取得server host name
*/
char * get_host_name()
{
        char *hostname = malloc(1024);
        //char hostname[1024];
        hostname[1023] = '\0';
//exit(1);
        //get host name
        gethostname(hostname, 1023);
        // replace stage- to ""
        hostname = replace(hostname, "stage-", "");
	
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

  return 0;
}



int WriteFile(char* file_name, char* connect, int buffer_size)
{
	FILE *fp = fopen(file_name, "w");
	if (fp == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	//fprintf(fp, "%s", connect);
    if ((fp == NULL) || (fwrite(connect, buffer_size, 1, fp) != 1)) {
        fprintf(stderr, "Failed to write data\n");
        exit(EXIT_FAILURE);
    }

	fclose(fp);
	
	return 0;
}

char *ReadFile(char* file_path)
{
	FILE* fp;
	char ch, * buffer;
	long file_length;
	fp = fopen(file_path, "r");
//exit(1);	
	if( fp == NULL)
	{
		exit(EXIT_FAILURE);
	}

	
	fseek(fp, 0, SEEK_END);
	file_length = ftell(fp);
	rewind(fp);	
	buffer = (char*) malloc (sizeof(char) *  file_length) + 1;
	if(buffer != NULL)
	{
		fread(buffer, 1, file_length, fp);
	}
	else
	{
		printf("builed mem fail.");
	//	exit(EXIT_FAILURE);
	}
	
	fclose(fp);
	return buffer;
}



void TrimSpaceReturn(char *strStr)
{

    if(strlen(strStr)<1)
    {
        //printf("trim space stop******");
        return;
    }
 //printf("trim space stop***963**");    

    int i=0;
    int len=strlen(strStr);

//printf("strlen ***** %d ** \n", len);
    for(i=len-1;i>=0;i--)
    {
       // printf(" \"%d<->%c\" ",strStr[i],strStr[i]);
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

    //printf("trim ******=> %s <==*******", strStr); 
}
