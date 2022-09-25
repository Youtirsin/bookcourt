#ifndef REQUEST_SENDER_H
#define REQUEST_SENDER_H

#include <curl/curl.h>

#include <iostream>
#include <vector>

class PostRequestSender {
 public:
  PostRequestSender(int buffer_size_ = 1024);

  ~PostRequestSender();

  std::string send_request(const std::string &url,
                           const std::vector<std::string> &headers,
                           const std::string &params);

 private:
  int buffer_size;

  static size_t cb(void *data, size_t size, size_t nmemb, void *userp);
};

#endif  // REQUEST_SENDER_H
