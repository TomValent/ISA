#include <string>
#include "Arguments.h"
#include "Parser.h"
#include <iostream>

bool Parser::parseHttpResponse(std::string response, std::string *responseText){
    std::cout << response << std::endl;
    return true;
}

bool Parser::parseXML(std::string feeds, Arguments arguments){
    return true;
}