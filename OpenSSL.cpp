#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>
#include "OpenSSL.h"
#include <iostream>

#define MAX_URL_SIZE 100

char *str2char(int integer){
    char *str = (char*) malloc(MAX_URL_SIZE);
    int i = 0;
    while(integer > 0){
        
        str[i] = (integer % 10) + '0';
        integer /= 10;
        i++;
    }
    str[i] = '\0';

    char *p1, *p2;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }

    return str;
}

char *parseURL(std::string url, bool isHTTPS, int port, bool portInLink){
    char charURL[MAX_URL_SIZE];
    strcpy(charURL, url.c_str());

    char *newURL = (char*) malloc(MAX_URL_SIZE);;
    int start;
    char end = '/';

    if(isHTTPS){
        start = 8;
    }else{
        start = 7;
    }
    size_t i;
    for(i = 0; i < strlen(charURL); i++, start++){
        if(charURL[start] == end){
            newURL[i] = ':';
            i++;
            break;
        }
        newURL[i] = charURL[start];
    }

    if(portInLink){
        char *strPort = str2char(port);

        for(size_t j = 0; j < strlen(strPort); j++, i++){
            newURL[i] = strPort[j];
        }
        newURL[i] = '\0';
    }
    else{
        newURL[--i] = '\0';
    }

    printf("%s\n", newURL);
    return newURL;
}

bool OpenSSL::processFeeds(std::vector <std::string> urls, Arguments *arguments){
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();

    int i = 0, urlCount = urls.size();
    for (const auto &url : urls){
        i++;
        if (i != 1 && i <= urlCount){
            printf("\n");
        }
        BIO *bio = nullptr;
        SSL_CTX *ssl_ctx = nullptr;

        if(!strstr(url.c_str(), "https:")){
            bio = BIO_new_connect(parseURL(url, false, arguments->getPort(), arguments->portInLink));
        }
        else{
            bio = BIO_new_connect(parseURL(url, true, arguments->getPort(), arguments->portInLink));
        }
    }return false;
}