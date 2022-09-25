#include "request_handler.h"

#include <boost/json.hpp>
#include <iostream>
#include <vector>

#include "config/bookcourt_config.h"
#include "util/bookcourt_util.h"
#include "post_request_sender/post_request_sender.h"
#include "task_timer/task_timer.h"

std::vector<std::string> construct_headers_body(boost::json::object &headers);

template<class Task, class... Args>
void rerange_task(Timer::time_point_t timepoint, Task task, Args... args);

void update_book_params(boost::json::object &dataObj,
                          boost::json::object &params);

void update_book_headers(boost::json::object &dataObj,
                          boost::json::object &headers);

Timer::time_point_t load_timepoint(boost::json::object &dataObj);

void print_request_info(const std::string &url,
                        std::vector<std::string> &headers,
                        const std::string &params);

/*
 * request handlers
 * start
*/
std::string handle_book(const std::string &data) {
  auto dataObj = boost::json::parse(data).as_object();
  boost::json::object params;
  auto headers = read_json_obj_from_file(get_config().get_headers_filepath());
  update_book_params(dataObj, params);
  update_book_headers(dataObj, headers);

  auto headersStr = construct_headers_body(headers);
  auto paramsStr = boost::json::serialize(params);
  auto urlConfig = read_json_obj_from_file(get_config().get_url_filepath());
  std::string url = urlConfig["book_url"].as_string().c_str();
  print_request_info(url, headersStr, paramsStr);

  auto timepoint = load_timepoint(dataObj);
  rerange_task(timepoint, [](std::string url, std::vector<std::string> headers, std::string params) {
    PostRequestSender sender;
    std::cout << "sending request to: " << url << '\n';
    sender.send_request(url, headers, params);
  }, url, headersStr, paramsStr);

  return "Processing...";
}

std::string handle_info(const std::string &data) {
  auto dataObj = boost::json::parse(data).as_object();
  auto params = read_json_obj_from_file(get_config().get_info_params_filepath());
  auto headers = read_json_obj_from_file(get_config().get_headers_filepath());

  headers["token"] = dataObj["token"];
  headers["Content-Type"] = "application/json";

  PostRequestSender sender;
  auto headersStr = construct_headers_body(headers);
  auto paramsStr = boost::json::serialize(params);
  auto urlConfig = read_json_obj_from_file(get_config().get_url_filepath());
  auto url = urlConfig["info_url"].as_string().c_str();
  print_request_info(url, headersStr, paramsStr);

  return sender.send_request(url, headersStr, paramsStr);
}
/*
 * request handlers
 * end
*/

std::vector<std::string> construct_headers_body(boost::json::object &headers) {
  std::vector<std::string> res;
  for (auto &&e: headers) {
    std::string key = e.key();
    std::string value = e.value().as_string().c_str();
    res.push_back(key + ": " + value);
  }
  if (!res.empty()) res.pop_back();
  return res;
}

template<class Task, class... Args>
void rerange_task(Timer::time_point_t timepoint, Task task, Args... args) {
  static Timer *timer = nullptr;
  static std::future<void> future;
  if (timer) {
    std::cout << "canceling previous task...\n";
    timer->cancel();
    std::this_thread::sleep_for(Timer::get_check_duration() * 2);
    delete timer;
  }
  timer = new Timer;
  future = timer->schedule(timepoint, task, args...);
}

void update_book_params(boost::json::object &dataObj,
                          boost::json::object &params) {
  params["periodId"] = dataObj["periodId"];
  params["date"] = create_timestamp();
  params["stadiumId"] = dataObj["stadiumId"];
}

void update_book_headers(boost::json::object &dataObj,
                          boost::json::object &headers) {
  headers["token"] = dataObj["token"];
  headers["Content-Type"] = "application/json";

  auto encryptConfig =
      read_json_obj_from_file(get_config().get_encrpt_filepath());
  std::string resultJson, resultJsonSignature;
  TimeSignaturer signaturer = init_signaturer(encryptConfig);

  std::tie(resultJson, resultJsonSignature) = signaturer.sign();
  headers["Resultjson"] = resultJson;
  headers["Resultjsonsignature"] = resultJsonSignature;
}

Timer::time_point_t load_timepoint(boost::json::object &dataObj) {
  auto booktime = dataObj["booktime"].as_object();
  auto booktime_hour = std::stoi(std::string(booktime["hour"].as_string().c_str()));
  auto booktime_min = std::stoi(std::string(booktime["min"].as_string().c_str()));
  auto booktime_sec = std::stoi(std::string(booktime["sec"].as_string().c_str()));

  std::cout << "added task at " <<
    booktime_hour << ":" <<
    booktime_min << ":" <<
    booktime_sec << "\n\n";
  return create_timepoint(booktime_hour, booktime_min, booktime_sec);
}

void print_request_info(const std::string &url,
                        std::vector<std::string> &headers,
                        const std::string &params) {
  std::cout << "sending request:\n";
  std::cout << "url:\n" << url << "\n";
  std::cout << "headers:\n";
  for (auto &&e : headers) {
    std::cout << e << "\n";
  }
  std::cout << "params:\n" << params << "\n\n";
}