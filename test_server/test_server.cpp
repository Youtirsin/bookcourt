#include <iostream>
#include "httplib.h"

int main() {
  httplib::Server server;

  server.Post("/book", [](const httplib::Request &req, httplib::Response &res) {
    std::cout << "request body: \n";
    std::cout << req.body << std::endl;
    res.set_content("This is book!", "text/plain");
  });

  server.Post("/getPriLogs", [](const httplib::Request &req, httplib::Response &res) {
    std::cout << "request body: \n";
    std::cout << req.body << std::endl;
    res.set_content("This is info!", "text/plain");
  });

  server.Get("/book", [](const httplib::Request &req, httplib::Response &res) {
    std::cout << "request body: \n";
    std::cout << req.body << "\n\n";
    res.set_content("This is book!", "text/plain");
  });

  server.Get("/getPriLogs", [](const httplib::Request &req, httplib::Response &res) {
    std::cout << "request body: \n";
    std::cout << req.body << std::endl;
    res.set_content("This is info!", "text/plain");
  });
  std::cout << "listening at port 8888.\n";
  server.listen("0.0.0.0", 8888);
}