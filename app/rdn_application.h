#ifndef RDN_APPLICATION_H
#define RDN_APPLICATION_H

#include <vector>

#include "core/application.h"
#include "core/object.h"

#include "modules/message_page/message_page.h"
#include "modules/list_page/list_page.h"
#include "modules/paged_article/paged_article.h"

class RDNApplication : public Application {
public:
    static void index(Object *instance, Request *request);
    static void get_sensor_data(Object *instance, Request *request);

    virtual void setup_routes();
    virtual void setup_middleware();

    virtual void migrate();

    void mqtt_sensor_callback(const std::string &client_id, const std::vector<uint8_t> &data);

    RDNApplication();
    ~RDNApplication();

};

#endif