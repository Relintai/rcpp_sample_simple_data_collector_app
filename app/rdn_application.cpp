#include "rdn_application.h"

#include "core/request.h"

#include <iostream>

#include "core/file_cache.h"

#include "core/handler_instance.h"

#include "core/database_manager.h"

#include "core/html_builder.h"

void RDNApplication::index(Object *instance, Request *request) {
	add_menu(request, MENUENTRY_HOME);

	dynamic_cast<ListPage *>(instance)->index(request);
}

void RDNApplication::session_middleware_func(Object *instance, Request *request) {
	std::cout << "test: session_middleware_func called" << std::endl;

	//if fail
	//request->send(); in middleware

	request->next_stage();
}

void RDNApplication::add_menu(Request *request, const Menuentries index) {
	request->head += menu_head;
	request->body += menu_strings[index];
	request->footer = footer;
}

void RDNApplication::projects_page_func(Object *instance, Request *request) {
	add_menu(request, MENUENTRY_PROJECTS);

	dynamic_cast<ListPage *>(instance)->index(request);
}

void RDNApplication::classes_page_func(Object *instance, Request *request) {
	add_menu(request, MENUENTRY_CLASSES);

	dynamic_cast<PagedList *>(instance)->index(request);
}

void RDNApplication::videos_page_func(Object *instance, Request *request) {
	add_menu(request, MENUENTRY_VIDEOS);

	dynamic_cast<ListPage *>(instance)->index(request);
}

void RDNApplication::papers_page_func(Object *instance, Request *request) {
	add_menu(request, MENUENTRY_PAPERS);

	dynamic_cast<ListPage *>(instance)->index(request);
}

void RDNApplication::research_page_func(Object *instance, Request *request) {
	add_menu(request, MENUENTRY_RESEARCH);

	dynamic_cast<PagedList *>(instance)->index(request);
}

void RDNApplication::setup_routes() {
	Application::setup_routes();

	index_func = HandlerInstance(index, index_page);
	main_route_map["projects"] = HandlerInstance(projects_page_func, projects_page);
	main_route_map["classes"] = HandlerInstance(classes_page_func, classes_page);
	//main_route_map["videos"] = HandlerInstance(videos_page_func, videos_page);
	main_route_map["papers"] = HandlerInstance(papers_page_func, papers_page);
	//main_route_map["research"] = HandlerInstance(research_page_func, research_page);
}

void RDNApplication::setup_middleware() {
	Application::setup_middleware();

	//middlewares.push_back(RDNApplication::session_middleware_func);
}

void RDNApplication::migrate() {
}

void RDNApplication::compile_menu() {

	for (int i = 0; i < MENUENTRY_MAX; ++i) {
		HTMLBuilder b;

		HTMLTag *t;

		b.div()->cls("content");

		b.header();
		b.w("&#60;relintai::net&#62;");
		b.span()->cls("header_link");
		b.w(" [ ");
		b.a()->href("https://github.com/Relintai");
		b.w("Github");
		b.ca();
		b.w(" ]");
		b.ca();

		b.cheader();

		b.ul()->cls("menu");

		//b.li();
		//b.w("::Relintai.net::");
		//b.cli();

		b.li();
		t = b.a()->href("/");

		if (i == MENUENTRY_HOME) {
			t->cls("menu_active");
		}

		b.w("Home");
		b.ca();
		b.cli();

		b.li();
		t = b.a()->href("/projects");

		if (i == MENUENTRY_PROJECTS) {
			t->cls("menu_active");
		}

		b.w("Projects");
		b.ca();
		b.cli();

		/*
		b.li();
		t = b.a()->href("/videos");

		if (i == MENUENTRY_VIDEOS) {
			t->cls("menu_active");
		}

		b.w("Videos");
		b.ca();
		b.cli();
*/

		b.li();
		t = b.a()->href("/classes");

		if (i == MENUENTRY_CLASSES) {
			t->cls("menu_active");
		}

		b.w("Classes");
		b.ca();
		b.cli();

		b.li();
		t = b.a()->href("/papers");

		if (i == MENUENTRY_PAPERS) {
			t->cls("menu_active");
		}

		b.w("Papers");
		b.ca();
		b.cli();

		/*
		b.li();
		t = b.a()->href("/research");

		if (i == MENUENTRY_RESEARCH) {
			t->cls("menu_active");
		}

		b.w("Research");
		b.ca();
		b.cli();
*/

		b.cul();

		//b.div()->cls("content");
		//b.write_tag();

		b.div()->cls("inner_content");
		b.write_tag();

		menu_strings.push_back(b.result);
	}

	HTMLBuilder bh;

	bh.meta()->charset_utf_8();

	bh.link()->rel_stylesheet()->href("/css/main.css");
	bh.write_tag();

	menu_head = bh.result;

	HTMLBuilder bf;

	bf.cdiv();
	bf.footer();
	bf.w("Powered by ");
	bf.a()->href("https://github.com/Relintai/rcpp_cms");
	bf.w("rcpp cms");
	bf.ca();
	bf.w(".");
	bf.cfooter();

	bf.cdiv();

	footer = bf.result;
}

RDNApplication::RDNApplication() :
		Application() {

	index_page = new ListPage();
	index_page->folder = "./content/home/";
	index_page->load();

	projects_page = new ListPage();
	projects_page->folder = "./content/projects/";
	projects_page->load();

	classes_page = new PagedList();
	classes_page->base_path = "/classes";
	classes_page->folder = "./content/classes/";
	classes_page->load();

	videos_page = new ListPage();
	videos_page->folder = "./content/videos/";
	videos_page->load();

	papers_page = new ListPage();
	papers_page->folder = "./content/papers/";
	papers_page->load();

	research_page = nullptr;

	//research_page = new PagedList();
	//research_page->base_path = "/research";
	//research_page->folder = "./content/research/";
	//research_page->load();

	compile_menu();
}

RDNApplication::~RDNApplication() {
	delete index_page;
	delete projects_page;
	delete classes_page;
	delete videos_page;
	delete papers_page;
//	delete research_page;
}

std::vector<std::string> RDNApplication::menu_strings;
std::string RDNApplication::menu_head = "";
std::string RDNApplication::footer = "";
