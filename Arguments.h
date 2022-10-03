#ifndef ISA_ARGUMENTS_H
#define ISA_ARGUMENTS_H

class Arguments {
    protected:
        char *feedfile;
        char *feedURL;
        char *certif = nullptr;
        char *certifAddr = nullptr;
        bool showTime = false;
        bool showAuthor = false;
        bool showURL = false;
        int port;

    public:
        void setFeedfile(char *feedfile);
        char *getFeedfile();
        void setFeedURL(char *feedURL);
        char *getFeedURL();
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
};
#endif //ISA_ARGUMENTS_H
