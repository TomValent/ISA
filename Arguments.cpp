#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include "Arguments.h"
#include "OpenSSL.h"

using namespace std;

#define DEFAULT_HTTP_PORT 80
#define DEFAULT_HTTPS_PORT 443
#define ERROR 1


void Arguments::setFeedfile(char *feedfile){
    this->feedfile = feedfile;
}
char* Arguments::getFeedfile(){
    return this->feedfile;
}

void Arguments::setFeedURL(char *feedURL){
    this->feedURL = feedURL;
}
char* Arguments::getFeedURL(){
    return this->feedURL;
}

void Arguments::setCertificate(char *certificate){
    this->certif = certificate;
}
char* Arguments::getCertificate(){
    return this->certif;
}

void Arguments::setCertificateAddr(char *certificateAddr){
    this->certifAddr = certificateAddr;
}
char* Arguments::getCertificateAddr(){
    return this->certifAddr;
}

void Arguments::setShowTime(bool showTime){
    this->showTime = showTime;
}
bool Arguments::ShowTime(){
    return this->showTime;
}

void Arguments::setShowAuthor(bool showAuthor){
    this->showAuthor = showAuthor;
}
bool Arguments::ShowAuthor(){
    return this->showAuthor;
}

void Arguments::setShowURL(bool showURL){
    this->showURL = showURL;
}
bool Arguments::ShowURL(){
    return this->showURL;
}

void Arguments::setPort(int port){
    this->port = port;
}
int Arguments::getPort(){
    return this->port;
}

int Arguments::findPort(char *link){
    char port[10];
    for(unsigned i = 0; i < strlen(link); i++){
        if(link[i] == ':' && link[i+1] >= '0' && link[i+1] <= '9'){
            int j = 0;
            i++;
            while(link[i] >= '0' && link[i] <= '9'){
                port[j] = link[i];
                j++;
                i++;
            }
            port[j] = '\0';
            return atoi(port);
        }
    }
    return 0; // == false
}

Arguments parseArguments(int argc, char **argv){
    bool file = false;
    Arguments args;

    for(int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            if(i+1 < argc){
                args.setFeedfile(argv[i+1]);
                file = true;
            }
        }
        else if(strstr(argv[i], "http://www.") || strstr(argv[i], "https://www.")){
            args.setFeedURL(argv[i]);
            file = true;
            args.setPort(args.findPort(argv[i]));
            if(!args.getPort()){
                if(strstr(argv[i], "http:")){
                    args.setPort(DEFAULT_HTTP_PORT);
                } else {
                    args.setPort(DEFAULT_HTTPS_PORT);
                }
            }
        }
        else if(strcmp(argv[i], "-c") == 0) {
            if(i+1 < argc){
                args.setCertificate(argv[i+1]);
            }
        }
        else if(strcmp(argv[i], "-C") == 0) {
            if(i+1 < argc){
                args.setCertificateAddr(argv[i+1]);
            }
        }
        else if(strcmp(argv[i], "-T") == 0){
            args.setShowTime(true);
        }
        else if(strcmp(argv[i], "-a") == 0){
            args.setShowAuthor(true);
        }
        else if(strcmp(argv[i], "-u") == 0){
            args.setShowURL(true);
        }
    }

    if(!file){
        fprintf(stderr, "Missing feedfile or URL.\n");
        exit(ERROR);
    }

    return args;
}

int main(int argc, char **argv) {

    Arguments args = parseArguments(argc, argv);
    std::vector<std::string> urls;

    if(!OpenSSL::processFeeds(urls, &args)){
        return ERROR;
    }

    return 0;
}