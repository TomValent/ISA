#include <cstring>
#include <vector>
#include <fstream>
#include "Arguments.h"
#include "OpenSSL.h"

using namespace std;

#define DEFAULT_HTTP_PORT 80
#define DEFAULT_HTTPS_PORT 443
#define ERROR 1
#define MAX_URL_SIZE 500

int portArray[100];

void Arguments::setFeedfile(string feedfile){
    this->feedfile = feedfile;
}
string Arguments::getFeedfile(){
    return this->feedfile;
}

void Arguments::setFeedURL(string feedURL){
    if(feedURL.length() > MAX_URL_SIZE){
        fprintf(stderr, "Error: Link is too long. Maximum length is %d.", MAX_URL_SIZE);
        exit(ERROR);
    }
    this->feedURL = feedURL;
}
string Arguments::getFeedURL(){
    return this->feedURL;
}

void Arguments::setCertificate(std::string certificate){
    this->certif = certificate;
}
std::string Arguments::getCertificate(){
    return this->certif;
}

void Arguments::setCertificateAddr(std::string certificateAddr){
    this->certifAddr = certificateAddr;
}
std::string Arguments::getCertificateAddr(){
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
            int portInt = atoi(port);
            if(portInt <= 1234 && portInt != 80 && portInt != 443){
                fprintf(stderr, "Error: Reserved port\n");
                exit(ERROR);
            }
            return portInt;
        }
    }
    return 0; // == false
}

vector<string> Arguments::getUrlsFromFile(string filename){
    fstream file;
    file.open(filename.c_str(), ios::in);
    if(!file){
        fprintf(stderr, "Error: Can't open feed file.\n");
        exit(ERROR);
    }

    string line;
    vector<string> list;
    int count = 0;

    while (getline(file, line)){
        if (line.empty() || !line.compare(0, 1, "#"))
            continue;
        list.push_back(line);
        char charURL[MAX_URL_SIZE];

        if(line.length() > MAX_URL_SIZE){
            fprintf(stderr, "Error: Link is too long. Maximum length is %d.", MAX_URL_SIZE);
            exit(ERROR);
        }

        strcpy(charURL, line.c_str());
        Arguments args;

        int port = args.findPort(charURL);
        if(!port){
            if(strstr(charURL, "http:")){
                portArray[count] = DEFAULT_HTTP_PORT;
            } else {
                portArray[count] = DEFAULT_HTTPS_PORT;
            }
        }
        else{
            portArray[count] = port;
        }
        printf("%d", portArray[count]);exit(0);
        count++;
    }

    file.close();
    return list;
}

Arguments parseArguments(int argc, char **argv){
    bool file = false;
    Arguments args;

    for(int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            if(i+1 < argc){
                if(file){
                    fprintf(stderr, "Error: You cannot combine '-f feedfile' and 'feedURL' arguments.\n");
                    exit(ERROR);
                }

                args.setFeedfile(argv[i+1]);
                file = true;
            }
            i++;
        }
        else if(strstr(argv[i], "http://") || strstr(argv[i], "https://")){
            if(file){
                fprintf(stderr, "Error: You cannot combine '-f feedfile' and 'feedURL' arguments.\n");
                exit(ERROR);
            }
            args.setFeedURL(argv[i]);
            file = true;
            args.setPort(args.findPort(argv[i]));
            if(!args.getPort()){
                args.portInLink = false;
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
            i++;
        }
        else if(strcmp(argv[i], "-C") == 0) {
            if(i+1 < argc){
                args.setCertificateAddr(argv[i+1]);
            }
            i++;
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
        else if(strcmp(argv[i], "./feedreader") != 0){
            fprintf(stderr, "Error: Parameter %s does not exist.\n", argv[i]);
            exit(ERROR);
        }
    }

    if(!file){
        fprintf(stderr, "Error: Missing feedfile or URL.\n");
        exit(ERROR);
    }

    return args;
}

int main(int argc, char **argv) {

    Arguments args = parseArguments(argc, argv);
    vector<string> urls;

    if(args.getFeedfile() != ""){
        urls = Arguments::getUrlsFromFile(args.getFeedfile());

        int len = urls.size();
        for(int i = 0; i < len; i++){       //urls
            if(portArray[i] == 0){
                args.portInLink = false;
                if(strstr(urls.at(i).c_str(), "http:")){
                    portArray[i] = DEFAULT_HTTP_PORT;
                } else {
                    portArray[i] = DEFAULT_HTTPS_PORT;
                }
            }
            args.ports[i] = portArray[i];
        }
    }
    else if(args.getFeedURL() != ""){
        urls.push_back(args.getFeedURL());
    }

    if(!OpenSSL::processFeeds(urls, &args)){
        return ERROR;
    }

    return 0;
}