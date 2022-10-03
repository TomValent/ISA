#include <string>
#include <vector>
#include "Arguments.h"

#ifndef ISA_OPENSSL_H
#define ISA_OPENSSL_H


class OpenSSL {
    public:
        static bool processFeeds(std::vector<std::string> urls, Arguments *arguments);
};


#endif //ISA_OPENSSL_H
