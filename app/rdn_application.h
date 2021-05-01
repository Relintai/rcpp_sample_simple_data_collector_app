#ifndef RDN_APPLICATION_H
#define RDN_APPLICATION_H

#include "core/application.h"
#include "core/object.h"

#include "modules/message_page/message_page.h"
#include "modules/list_page/list_page.h"
#include "modules/paged_article/paged_article.h"

class RDNApplication : public Application {
public:
    static void index(Object *instance, Request *request);

    virtual void setup_routes();
    virtual void setup_middleware();

    virtual void migrate();

    RDNApplication();
    ~RDNApplication();

};

#endif