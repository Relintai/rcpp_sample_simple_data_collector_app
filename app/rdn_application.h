#ifndef RDN_APPLICATION_H
#define RDN_APPLICATION_H

#include "core/application.h"
#include "core/object.h"

#include "modules/message_page/message_page.h"
#include "modules/list_page/list_page.h"
#include "modules/paged_article/paged_article.h"
#include "paged_list.h"

class RDNApplication : public Application {
public:
    enum Menuentries {
        MENUENTRY_HOME = 0,
        MENUENTRY_PROJECTS,
        MENUENTRY_VIDEOS,
        MENUENTRY_CLASSES,
        MENUENTRY_PAPERS,
        MENUENTRY_RESEARCH,

        MENUENTRY_MAX,
    };

public:
    static void index(Object *instance, Request *request);

    static void session_middleware_func(Object* instance, Request *request);

    static void add_menu(Request *request, const Menuentries index);

    static void projects_page_func(Object *instance, Request *request);
    static void classes_page_func(Object *instance, Request *request);
    static void videos_page_func(Object *instance, Request *request);
    static void papers_page_func(Object *instance, Request *request);
    static void research_page_func(Object *instance, Request *request);

    virtual void setup_routes();
    virtual void setup_middleware();

    virtual void migrate();

    void compile_menu();

    RDNApplication();
    ~RDNApplication();

    ListPage *index_page;
    ListPage *projects_page;
    PagedList *classes_page;
    ListPage *videos_page;
    ListPage *papers_page;
    PagedList *research_page;

    static std::vector<std::string> menu_strings;
    static std::string menu_head;
    static std::string footer;
};

#endif