#include "post_request_sender.h"

using std::string;

class CurlIniter {
 public:
  CurlIniter() {
    std::cout << "[post_request_sender] curl inited.\n";
    curl_global_init(CURL_GLOBAL_ALL);
  }

  ~CurlIniter() {
    std::cout << "[post_request_sender] curl deinited.\n";
    curl_global_cleanup();
  }
};

void init_curl() {
  static bool curl_inited = false;
  if (!curl_inited) {
    static CurlIniter curlIniter;
    curl_inited = true;
  }
}

size_t PostRequestSender::cb(void *data, size_t size, size_t nmemb,
                             void *userp) {
  size_t realsize = size * nmemb;
  char *from = (char *)data;
  char *to = (char *)userp;

  std::copy(from, from + realsize, to);
  return realsize;
}

PostRequestSender::PostRequestSender(int buffer_size_)
    : buffer_size(buffer_size_) {
  init_curl();
}

PostRequestSender::~PostRequestSender() {}

string PostRequestSender::send_request(const string &url,
                                       const std::vector<std::string> &headers,
                                       const string &params) {
  CURL *easyHandle = curl_easy_init();
  char *res_data = new char[buffer_size];
  if (res_data == nullptr) {
    std::cerr << "[post_request_sender] PostRequestSender::send_request allocate memory error\n";
    return "";
  }

  curl_easy_setopt(easyHandle, CURLOPT_WRITEFUNCTION, &PostRequestSender::cb);
  curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, res_data);
  curl_easy_setopt(easyHandle, CURLOPT_URL, url.c_str());
  if (!headers.empty()) {
    curl_slist *headerList = nullptr;
    for (auto &&e : headers)
      headerList = curl_slist_append(headerList, e.c_str());
    curl_easy_setopt(easyHandle, CURLOPT_HTTPHEADER, headerList);
  }
  if (!params.empty())
    curl_easy_setopt(easyHandle, CURLOPT_POSTFIELDS, params.c_str());

  auto ret = curl_easy_perform(easyHandle);
  if (ret == CURLE_OK) {
    curl_easy_cleanup(easyHandle);
    return res_data;
  } else {
    curl_easy_cleanup(easyHandle);
    std::cerr << "[post_request_sender] PostRequestSender::send_request error\n";
    return "";
  }
}
