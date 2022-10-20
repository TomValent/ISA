#include <string>
#include <vector>

#ifndef ISA_ARGUMENTS_H
#define ISA_ARGUMENTS_H

class Arguments {
    protected:
        std::string feedfile = "";
        std::string feedURL = "";
        std::string certif = "";
        std::string certifAddr = "";
        bool showTime = false;
        bool showAuthor = false;
        bool showURL = false;
        int port;

    public:
        bool portInLink = true;
        int ports[100] = {0};

        void setFeedfile(std::string feedfile);
        std::string getFeedfile();
        void setFeedURL(std::string feedURL);
        std::string getFeedURL();
        void setCertificate(std::string certificate);
        std::string getCertificate();
        void setCertificateAddr(std::string certificateAddr);
        std::string getCertificateAddr();
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

