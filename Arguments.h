#include <string>

#ifndef ISA_ARGUMENTS_H
#define ISA_ARGUMENTS_H

class Arguments {
    protected:
        std::string feedfile = "";
        std::string feedURL = "";
        char *certif = nullptr;
        char *certifAddr = nullptr;
        bool showTime = false;
        bool showAuthor = false;
        bool showURL = false;
        int port;

    public:
        void setFeedfile(std::string feedfile);
        std::string getFeedfile();
        void setFeedURL(std::string feedURL);
        std::string getFeedURL();
        void setCertificate(char *certificate);
        char *getCertificate();
        void setCertificateAddr(char *certificateAddr);
        char *getCertificateAddr();
        void setShowTime(bool showTime);
        bool ShowTime();
        void setShowAuthor(bool showAuthor);
        bool ShowAuthor();
        void setShowURL(bool showURL);
        bool ShowURL();
        void setPort(int port);
        int getPort();
        int findPort(char *link);
        static std::vector<std::string> getUrlsFromFile(std::string filename);
};
#endif //ISA_ARGUMENTS_H
