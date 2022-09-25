#include <boost/filesystem.hpp>
#include <iostream>

#include "config/bookcourt_config.h"
#include "httplib.h"
#include "util/bookcourt_util.h"
#include "request_handler/request_handler.h"

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cerr << "wrong usage\n"
                 "usage: bookcourt [config_dir]\n"
                 "default: config\n";
    return 1;
  }
  if (!boost::filesystem::exists(argv[1])) {
    std::cerr << "wrong path of config files\n"
                 "usage: bookcourt [config_dir]\n"
                 "default: config\n";
    return 1;
  }

  if (!get_config().init(argv[1])) {
    std::cerr << "init config failed\n";
    return 1;
  }

  httplib::Server svr;

  const std::string host = "0.0.0.0";
  const int port = 8080;

  svr.Post("/book", [](const httplib::Request &req, httplib::Response &res) {
    std::cout << "post request:\n"
                 "req body: "
              << req.body << "\n\n";
    auto data = handle_book(req.body);
    res.set_content(data, "text/plain");
  });

  svr.Post("/info", [](const httplib::Request &req, httplib::Response &res) {
    std::cout << "post request:\n"
                 "req body: "
              << req.body << "\n\n";
    auto data = handle_info(req.body);
    res.set_content(data, "application/json");
  });

  svr.Get("/test", [](const httplib::Request &req, httplib::Response &res) {
    std::cout << req.body << std::endl;
    res.set_content("Hello world", "text/plain");
  });

  auto ret = svr.set_mount_point("/", "./frontend");
  if (!ret) {
    std::cerr << "error mounting...\n";
  }

  std::cout << "listening at " << host << ":" << port << "...\n";
  svr.listen(host, port);

  return 0;
}
