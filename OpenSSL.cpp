#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>
#include "OpenSSL.h"
#include "Parser.h"

#include <iostream>

#define MAX_URL_SIZE 500
#define MAX_REQUEST_SIZE 500
#define MAX_READ_SIZE 5000

std::string getPath(std::string url){
    char charURL[MAX_URL_SIZE], *path = (char*)malloc(MAX_URL_SIZE);
    strcpy(charURL, url.c_str());
    for(size_t i = 0; i < strlen(charURL); i++){
        if(charURL[i] == '/' && charURL[i+1] != '/' && charURL[i-1] != '/'){
            int j = 0;
            while(charURL[i] != '\0'){
                path[j] = charURL[i];
                i++;
                j++;
            }
            path[j] = '\0';
            return path;
        }
    }
    return (char*)"";
}

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
            if(arguments->getFeedfile() != ""){
                host = parseURL(url, false, arguments->ports[i-1], arguments->portInLink);
            } else {
                host = parseURL(url, false, arguments->getPort(), arguments->portInLink);
            }
            bio = BIO_new_connect(host);
            ssl_ctx = SSL_CTX_new(SSLv23_client_method());
        }
        else{
            if(arguments->getFeedfile() != ""){
                host = parseURL(url, true, arguments->ports[i-1], arguments->portInLink);
            } else {
                host = parseURL(url, true, arguments->getPort(), arguments->portInLink);
            }
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

            if (!verify){
                fprintf(stderr,"Verification of certificates failed.");
                if(ssl_ctx)
                    SSL_CTX_free(ssl_ctx);
                return false;
            }
            bio = BIO_new_ssl_connect(ssl_ctx);
            bio = BIO_new_connect(host);
            BIO_set_conn_hostname(bio, host);
            BIO_do_connect(bio);
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
        }

        if (ssl && SSL_get_verify_result(ssl) != X509_V_OK)
        {
            fprintf(stderr, "Verification of certificates failed.\n");
            if(bio)
                BIO_free_all(bio);
            if(ssl_ctx)
                SSL_CTX_free(ssl_ctx);
            return false;
        }

        std::string strHost = host;

        std::string request(
                    "GET " + getPath(url) + " HTTP/1.1\r\n"
                    "Host: " + strHost + "\r\n"
                    "Connection: Close\r\n"
                    "\r\n"
                );

        auto writeDataSize = static_cast<int>(request.size());
        bool writeDone = false;

        if (BIO_write(bio, request.c_str(), writeDataSize))
        {
            writeDone = true;
        }

        if (!writeDone)
        {
            fprintf(stderr, "Error while writing to bio.\n");
            if(bio)
                BIO_free_all(bio);
            if(ssl_ctx)
                SSL_CTX_free(ssl_ctx);
            return false;
        }

        char responseBuff[MAX_READ_SIZE] = {'\0'};
        std::string response;
        int readRes = 0;
        do{
            bool firstRead = true;
            bool done = false;
            while(firstRead || BIO_should_retry(bio)){
                firstRead = false;
                readRes = BIO_read(bio, responseBuff, MAX_READ_SIZE - 1);
                std::cout << "response len: " << readRes << "\n" << responseBuff << std::endl;
                if(readRes >= 0){
                    if(readRes > 0){
                        responseBuff[readRes] = '\0';
                        response += responseBuff;
                    }
                    done = true;
                    break;
                }
            }
            if(!done){
                fprintf(stderr, "Error while reading bio.\n");
                if(bio)
                    BIO_free_all(bio);
                if(ssl_ctx)
                    SSL_CTX_free(ssl_ctx);
                return false;
            }
        }
        while(readRes != 0);

//pray to only working url
//"http://feeds.bbci.co.uk/news/world/rss.xml"

        std::string responseText;
        Parser parser;

        parser.parseHttpResponse(response, &responseText);
//        if (!Parser::parseHttpResponse(response, &responseText))
//        {
//            fprintf(stderr, "Error: Invalid HTTP response from '%s'.\n", url.c_str());
//            if(bio)
//                BIO_free_all(bio);
//            if(ssl_ctx)
//                SSL_CTX_free(ssl_ctx);
//            return false;
//        }
//
//        if (!Parser::parseXML(responseText, arguments, url))
//        {
//            fprintf(stderr, "Error: Parsing XML feed from '%s'.\n", url.c_str());
//            if(bio)
//                BIO_free_all(bio);
//            if(ssl_ctx)
//                SSL_CTX_free(ssl_ctx);
//            return false;
//        }

        if(bio)
            BIO_free_all(bio);
        if(ssl_ctx)
            SSL_CTX_free(ssl_ctx);
    }
    return true;
}