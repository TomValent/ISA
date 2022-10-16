#include <string>
#include <cstring>
#include "Arguments.h"
#include "Parser.h"

bool isAtom = false;
bool isRss = false;

size_t strpos(const std::string &haystack, const std::string &needle)
{
    int sleng = haystack.length();
    int nleng = needle.length();

    if (sleng==0 || nleng==0)
        return std::string::npos;

    for(int i=0, j=0; i<sleng; j=0, i++ )
    {
        while (i+j<sleng && j<nleng && haystack[i+j]==needle[j])
            j++;
        if (j==nleng)
            return i;
    }
    return std::string::npos;
}

bool Parser::parseHttpResponse(std::string response, std::string *responseText){
    if(strstr(response.c_str(), "HTTP/1.0 200 OK") || strstr(response.c_str(), "HTTP/1.1 200 OK")){
        if(strstr(response.c_str(), "<rss")) {
            size_t beginning = strpos(response.c_str(), "<rss");
            *responseText = response.substr(beginning, response.length());
            isRss = true;
        } else if(strstr(response.c_str(), "<feed")) {
            size_t beginning = strpos(response.c_str(), "<feed");
            *responseText = response.substr(beginning, response.length());
            isAtom = true;
        }
    } else{
        return false;
    }

    return true;
}

bool Parser::parseXML(std::string feeds, Arguments arguments){
    printf("%d, %d\n", isAtom, isRss);
    isAtom = false;
    isRss = false;
    return true;
}