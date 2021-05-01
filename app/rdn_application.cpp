#include "rdn_application.h"

#include "core/request.h"

#include <iostream>

#include "core/file_cache.h"

#include "core/handler_instance.h"

#include "core/database_manager.h"

#include "core/html_builder.h"

void RDNApplication::index(Object *instance, Request *request) {
	if (FileCache::get_singleton()->wwwroot_has_file("/index.html")) {
		std::string fp = FileCache::get_singleton()->wwwroot + "/index.html";

		request->send_file(fp);

		return;
	}

	request->send_error(404);
}

void RDNApplication::setup_routes() {
	Application::setup_routes();

	index_func = HandlerInstance(index);
}

void RDNApplication::setup_middleware() {
	Application::setup_middleware();

	//middlewares.push_back(RDNApplication::session_middleware_func);
}

void RDNApplication::migrate() {
}

RDNApplication::RDNApplication() :
		Application() {
}

RDNApplication::~RDNApplication() {
}
