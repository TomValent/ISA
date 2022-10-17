#include <string>
#include "Arguments.h"

#ifndef ISA_PARSER_H
#define ISA_PARSER_H


class Parser {
    public:
        bool parseHttpResponse(std::string response, std::string *responseText);
        static bool parseXML(std::string feed, Arguments arguments);
};


#endif //ISA_PARSER_H
