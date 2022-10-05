#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "OpenSSL.h"

bool OpenSSL::processFeeds(std::vector <std::string> urls, Arguments *arguments){
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    return false;
}