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

    char *newURL = (char*) malloc(MAX_URL_SIZE);
    int start;
    char end = '/';

    if(isHTTPS){
        start = 8;
    }else{
        start = 7;
    }
    size_t i;
    for(i = 0; i < strlen(charURL); i++, start++){
        if(charURL[start] == end || charURL[start] == '\0'){
            newURL[i] = ':';
            i++;
            break;
        }
        newURL[i] = charURL[start];
    }

    if(!portInLink){
        char *strPort = str2char(port);

        for(size_t j = 0; j < strlen(strPort); j++, i++){
            newURL[i] = strPort[j];
        }
        newURL[i] = '\0';
    }
    else{
        newURL[--i] = '\0';
    }
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
        SSL *ssl = nullptr;
        char *host;

        if(!strstr(url.c_str(), "https:")){
            if(arguments->getPort() != 0){
                host = parseURL(url, false, arguments->ports[i-1], arguments->portInLink);
            } else {
                host = parseURL(url, false, arguments->getPort(), arguments->portInLink);
            }
            bio = BIO_new_connect(host);
            ssl_ctx = SSL_CTX_new(SSLv23_client_method());
        }
        else{
            if(arguments->getPort() != 0){
                host = parseURL(url, true, arguments->ports[i-1], arguments->portInLink);
            } else {
                host = parseURL(url, true, arguments->getPort(), arguments->portInLink);
            }
            bio = BIO_new_connect(host);
            ssl_ctx = SSL_CTX_new(SSLv23_client_method());

            int verify = 0;
            if(arguments->getCertificate() == "" && arguments->getCertificateAddr() == "")
            {
                verify = SSL_CTX_set_default_verify_paths(ssl_ctx);
            } else if(arguments->getCertificate() != "" && arguments->getCertificateAddr() != ""){
                verify = SSL_CTX_load_verify_locations(ssl_ctx,arguments->getCertificate().c_str(),arguments->getCertificateAddr().c_str());
            } else if(arguments->getCertificate() != ""){
                verify = SSL_CTX_load_verify_locations(ssl_ctx, arguments->getCertificate().c_str(), nullptr);
            } else if(arguments->getCertificateAddr() != ""){
                verify = SSL_CTX_load_verify_locations(ssl_ctx, nullptr, arguments->getCertificateAddr().c_str());
            }

            if (!verify)
            {
                fprintf(stderr,"Verification of certificates on '%s' failed.", url.c_str());
                if(bio)
                    BIO_free_all(bio);
                if(ssl_ctx)
                    SSL_CTX_free(ssl_ctx);
                return false;
            }

        }

        if(bio == NULL)
        {
            fprintf(stderr, "Error: %s\n", ERR_reason_error_string(ERR_get_error()));
            if(ssl_ctx)
                SSL_CTX_free(ssl_ctx);
            return false;
        }

        if(BIO_do_connect(bio) <= 0)
        {
            fprintf(stderr, "Error: %s\n", ERR_reason_error_string(ERR_get_error()));
            return false;
        }

        if (strstr(url.c_str(), "https:"))
        {
            BIO_get_ssl(bio, &ssl);
            SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
            BIO_set_conn_hostname(bio, host);
        }

        //cout << url;
    }
    return true;
}