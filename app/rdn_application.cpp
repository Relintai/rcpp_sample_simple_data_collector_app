#include "rdn_application.h"

#include <iostream>
#include <string>

#include <tinydir/tinydir.h>

#include "core/database_manager.h"
#include "core/file_cache.h"
#include "core/handler_instance.h"
#include "core/html_builder.h"
#include "core/query_result.h"
#include "core/request.h"
#include "core/utils.h"

void RDNApplication::index(Object *instance, Request *request) {
	if (FileCache::get_singleton()->wwwroot_has_file("/index.html")) {
		std::string fp = FileCache::get_singleton()->wwwroot + "/index.html";

		request->send_file(fp);

		return;
	}

	request->send_error(404);
}

void RDNApplication::get_sensor_data(Object *instance, Request *request) {
	std::string sql = "SELECT * FROM sensor_data;";

	QueryResult *res = DatabaseManager::get_singleton()->ddb->query(sql);

	std::string json;

	json += "[";

	bool first = true;
	while (res->next_row()) {
		if (first) {
			first = false;
		} else {
			json += ",";
		}

		json += "{";

		json += "\"id\":" + std::string(res->get_cell(0)) + ",";
		json += "\"client_id\":\"" + std::string(res->get_cell(1)) + "\",";
		json += "\"measurement\":" + std::string(res->get_cell(2));

		json += "}";
	}

	json += "]";

	request->response->setContentType("application/json");
	request->response->setBody(json);
	request->send();
}

void RDNApplication::app_docs_page(Object *instance, Request *request) {
	request->response->setBody(app_docs);
	request->send();
}

void RDNApplication::engine_docs_page(Object *instance, Request *request) {
	request->response->setBody(engine_docs);
	request->send();
}

void RDNApplication::setup_routes() {
	Application::setup_routes();

	index_func = HandlerInstance(index);

	main_route_map["sensor_data"] = HandlerInstance(get_sensor_data);
	main_route_map["app_docs"] = HandlerInstance(app_docs_page);
	main_route_map["engine_docs"] = HandlerInstance(engine_docs_page);
}

void RDNApplication::setup_middleware() {
	Application::setup_middleware();

	//middlewares.push_back(RDNApplication::session_middleware_func);
}

void RDNApplication::migrate() {
	std::string sql = "CREATE TABLE sensor_data("
					  "id  INTEGER  PRIMARY KEY  AUTOINCREMENT,"
					  "client_id  TEXT  NOT NULL,"
					  "measurement  REAL);";

	DatabaseManager::get_singleton()->ddb->query_run(sql);
}

void RDNApplication::mqtt_sensor_callback(const std::string &client_id, const std::vector<uint8_t> &data) {
	if (client_id != "1") {
		return;
	}

	if (data.size() == 0) {
		return;
	}

	std::string d;
	d.reserve(data.size());

	for (int i = 0; i < data.size(); ++i) {
		d += data[i];
	}

	//IMPORTANT: SQL INJECTION WILL WORK ON THIS, IF YOU DON"T FILTER THE CLINET IDS!!!! No prepared statement support yet! :(
	std::string sql = "INSERT INTO sensor_data (client_id, measurement)"
					  "VALUES ('" +
					  client_id + "'," + d + ");";

	DatabaseManager::get_singleton()->ddb->query_run(sql);
}

void RDNApplication::load_md(const std::string &file_name, std::string *str) {
	FILE *f = fopen(file_name.c_str(), "r");

	if (!f) {
		printf("RDNApplication::load_md: Error opening file!\n");
		return;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); /* same as rewind(f); */

	str->resize(fsize);

	fread(&(*str)[0], 1, fsize, f);
	fclose(f);

	Utils::markdown_to_html(str);
}

RDNApplication::RDNApplication() :
		Application() {

	load_md("./engine/Readme.md", &engine_docs);
	load_md("./Readme.md", &app_docs);
}

RDNApplication::~RDNApplication() {
}

std::string RDNApplication::engine_docs;
std::string RDNApplication::app_docs;