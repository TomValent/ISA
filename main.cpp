#include <iostream>
#include <cstring>

using namespace std;

#define DEFAULT_HTTP_PORT 80
#define DEFAULT_HTTPS_PORT 443

class Arguments{
    protected:
        char *feedfile;
        char *feedURL;
        char *certif;
        char *certifAddr;
        bool showTime = false;
        bool showAuthor = false;
        bool showURL = false;
        int port;

    public:                                         //getters and setters for given arguments
        void setFeedfile(char *feedfile){
            this->feedfile = feedfile;
        }
        char *getFeedfile(){
            return this->feedfile;
        }

        void setFeedURL(char *feedURL){
            this->feedURL = feedURL;
        }
        char *getFeedURL(){
            return this->feedURL;
        }

        void setCertificate(char *certificate){
            this->certif = certificate;
        }
        char *getCertificate(){
            return this->certif;
        }

        void setCertificateAddr(char *certificateAddr){
            this->certifAddr = certificateAddr;
        }
        char *getCertificateAddr(){
            return this->certifAddr;
        }

        void setShowTime(bool showTime){
            this->showTime = showTime;
        }
        bool ShowTime(){
            return this->showTime;
        }

        void setShowAuthor(bool showAuthor){
            this->showAuthor = showAuthor;
        }
        bool ShowAuthor(){
            return this->showAuthor;
        }

        void setShowURL(bool showURL){
            this->showURL = showURL;
        }
        bool ShowURL(){
            return this->showURL;
        }

        void setPort(int port){
            this->port = port;
        }
        int getPort(){
            return this->port;
        }

        int findPort(char *link){
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
};

Arguments parseArguments(int argc, char **argv){
    bool file = false;
    bool certificate = false;
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
                certificate = true;
            }
        }
        else if(strcmp(argv[i], "-C") == 0) {
            if(i+1 < argc){
                args.setCertificateAddr(argv[i+1]);
                certificate = true;
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
        exit(1);
    }

    if(!certificate){
        //args.setCertificate(SSL_CTX_set_default_verify_paths()); //TODO zistit jak toto funguje
    }

    return args;
}

int main(int argc, char **argv) {

    Arguments src = parseArguments(argc, argv);

//    if(src.getFeedURL()){
//
//    }else if(src.getFeedfile()){
//
//    }

    return 0;
}