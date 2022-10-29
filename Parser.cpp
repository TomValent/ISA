#include <string>
#include <cstring>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>
#include "Arguments.h"
#include "Parser.h"

size_t strpos(const std::string &haystack, const std::string &needle){
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
    if(strstr(response.c_str(), "HTTP/1.0 200 OK") || strstr(response.c_str(), "HTTP/1.1 200 OK"))
    {
        if(strstr(response.c_str(), "<rss"))
        {
            size_t beginning = strpos(response.c_str(), "<rss");
            *responseText = response.substr(beginning, response.length());
        } else if(strstr(response.c_str(), "<feed"))
        {
            size_t beginning = strpos(response.c_str(), "<feed");
            *responseText = response.substr(beginning, response.length());
        }
        else
        {
            fprintf(stderr, "Wrong type of feed.\nProgram supports only RSS <rss> and ATOM <feed> feeds\n.");
            return false;
        }
    } else{
        return false;
    }

    return true;
}

bool Parser::parseXML(std::string feed, Arguments arguments){
    xmlKeepBlanksDefault(0);

    xmlDocPtr doc = xmlParseDoc((const xmlChar *) feed.c_str());
    if (!doc)
    {
        fprintf(stderr, "Error: Unsupported feed format.\n");
        xmlCleanupParser();
        return false;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (!root)
    {
        fprintf(stderr, "Error: Empty feed file.\n");
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return false;
    }

    if(xmlStrcasecmp(root->name, (const xmlChar *) "rss") == 0)             //rss feed
    {
        if(!arguments.ShowAuthor() && !arguments.ShowTime() && !arguments.ShowURL())        //no extra newlines between entries
        {
            for(xmlNodePtr node = root->children; node; node = node->next)
            {
                if(xmlStrcasecmp(node->name, (const xmlChar *) "channel") == 0)
                {
                    for(xmlNodePtr channel = node->children; channel; channel = channel->next)
                    {
                        if(xmlStrcasecmp(channel->name, (const xmlChar *) "title") == 0)
                        {
                            char *title = (char *) xmlNodeGetContent(channel);
                            fprintf(stdout, "*** %s ***\n", title);
                        } else if(xmlStrcasecmp(channel->name, (const xmlChar *) "item") == 0)
                        {
                            for(xmlNodePtr item = channel->children; item; item = item->next)
                            {
                                if(xmlStrcasecmp(item->name, (const xmlChar *) "title") == 0)
                                {
                                    char *articleTitle = (char *) xmlNodeGetContent(item);
                                    fprintf(stdout, "%s\n", articleTitle);
                                }
                            }
                        }
                    }
                }
            }
            fprintf(stdout, "\n");
        }
        else        //extra new line between entries
        {
            for(xmlNodePtr node = root->children; node; node = node->next)
            {
                if(xmlStrcasecmp(node->name, (const xmlChar *) "channel") == 0)
                {
                    for(xmlNodePtr channel = node->children; channel; channel = channel->next)
                    {
                        if(xmlStrcasecmp(channel->name, (const xmlChar *) "title") == 0)
                        {
                            char *title = (char *) xmlNodeGetContent(channel);
                            fprintf(stdout, "*** %s ***\n", title);
                        } else if(xmlStrcasecmp(channel->name, (const xmlChar *) "item") == 0)
                        {
                            for(xmlNodePtr item = channel->children; item; item = item->next)
                            {
                                if(xmlStrcasecmp(item->name, (const xmlChar *) "title") == 0)   //force title to be first
                                {
                                    char *articleTitle = (char *) xmlNodeGetContent(item);
                                    fprintf(stdout, "%s\n", articleTitle);
                                }
                            }
                            for(xmlNodePtr item = channel->children; item; item = item->next)
                            {
                                if(xmlStrcasecmp(item->name, (const xmlChar *) "link") == 0 && arguments.ShowURL())
                                {
                                    char *articleURL = (char *) xmlNodeGetContent(item);
                                    fprintf(stdout, "URL: %s\n", articleURL);
                                } else if((xmlStrcasecmp(item->name, (const xmlChar *) "pubDate") == 0 || xmlStrcasecmp(item->name, (const xmlChar *) "lastBuildDate") == 0) && arguments.ShowTime())
                                {
                                    if(xmlStrcasecmp(item->name, (const xmlChar *) "lastBuildDate") == 0)   //article was updated
                                    {
                                        char *timeUpdated = (char *) xmlNodeGetContent(item);
                                        fprintf(stdout, "Aktualizace: %s\n", timeUpdated);
                                    } else {                                                                //article was not updated yet (only created)
                                        char *timeCreated = (char *) xmlNodeGetContent(item);
                                        fprintf(stdout, "Aktualizace: %s\n", timeCreated);
                                    }
                                } else if(xmlStrcasecmp(item->name, (const xmlChar *) "author") == 0 && arguments.ShowAuthor())
                                {
                                    char *author = (char *) xmlNodeGetContent(item);
                                    fprintf(stdout, "Autor: %s\n", author);
                                }
                            }
                            fprintf(stdout, "\n");
                        }
                    }
                }
            }
        }
    } else if(xmlStrcasecmp(root->name, (const xmlChar *) "feed") == 0)     //atom feed
    {
        if(!arguments.ShowAuthor() && !arguments.ShowTime() && !arguments.ShowURL())        //no extra newlines between entries
        {
            for(xmlNodePtr node = root->children; node; node = node->next)
            {
                if(xmlStrcasecmp(node->name, (const xmlChar *) "title") == 0)
                {
                    char *title = (char *) xmlNodeGetContent(node);
                    fprintf(stdout, "*** %s ***\n", title);
                } else if(xmlStrcasecmp(node->name, (const xmlChar *) "entry") == 0)
                {
                    for(xmlNodePtr entries = node->children; entries; entries = entries->next)
                    {
                        if(xmlStrcasecmp(entries->name, (const xmlChar *) "title") == 0)
                        {
                            char *articleTitle = (char *) xmlNodeGetContent(entries);
                            fprintf(stdout, "%s\n", articleTitle);
                        }
                    }
                }
            }
            fprintf(stdout, "\n");
        }
        else        //extra new line between entries
        {
            for(xmlNodePtr node = root->children; node; node = node->next)
            {
                if(xmlStrcasecmp(node->name, (const xmlChar *) "title") == 0)
                {
                    char *title = (char *) xmlNodeGetContent(node);
                    fprintf(stdout, "*** %s ***\n", title);
                } else if(xmlStrcasecmp(node->name, (const xmlChar *) "entry") == 0)
                {
                    for(xmlNodePtr entries = node->children; entries; entries = entries->next){    //force title to be first
                        if(xmlStrcasecmp(entries->name, (const xmlChar *) "title") == 0)
                        {
                            char *articleTitle = (char *) xmlNodeGetContent(entries);
                            fprintf(stdout, "%s\n", articleTitle);
                        }
                    }
                    for(xmlNodePtr entries = node->children; entries; entries = entries->next)
                    {
                        if(xmlStrcasecmp(entries->name, (const xmlChar *) "link") == 0 && arguments.ShowURL())
                        {
                            char *articleURL = (char *) xmlGetProp(entries, (xmlChar *) "href");
                            fprintf(stdout, "URL: %s\n", articleURL);
                        } else if(xmlStrcasecmp(entries->name, (const xmlChar *) "updated") == 0 && arguments.ShowTime())
                        {
                            char *timeUpdated = (char *) xmlNodeGetContent(entries);
                            fprintf(stdout, "Aktualizace: %s\n", timeUpdated);
                        } else if(xmlStrcasecmp(entries->name, (const xmlChar *) "author") == 0 && arguments.ShowAuthor())
                        {
                            for(xmlNodePtr authorNode = entries->children; authorNode; authorNode = authorNode->next)
                            {
                                if(xmlStrcasecmp(authorNode->name, (const xmlChar *) "name") == 0)
                                {
                                    char *name = (char *) xmlNodeGetContent(authorNode);
                                    fprintf(stdout, "Autor: %s\n", name);
                                    break;
                                } else if(xmlStrcasecmp(authorNode->name, (const xmlChar *) "email") == 0)
                                {
                                    char *email = (char *) xmlNodeGetContent(authorNode);
                                    fprintf(stdout, "Autor: %s\n", email);
                                    break;
                                }
                            }
                        }
                    }
                    fprintf(stdout, "\n");
                }
            }
        }
    }
    return true;
}
