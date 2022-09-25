#ifndef BOOKCOURT_CONFIG_H
#define BOOKCOURT_CONFIG_H

#include <iostream>

class Config {
 public:
  bool init(const std::string &configPath_ = "./config/");

  std::string get_config_path() const {
    return configPath;
  }
  std::string get_encrpt_filepath() const {
    return encrptFilepath;
  }
  std::string get_headers_filepath() const {
    return headersFilepath;
  }
  std::string get_info_params_filepath() const {
    return info_paramsFilepath;
  }
  std::string get_url_filepath() const {
    return urlFilepath;
  }

 private:
  static const char * ENCRYPT_FILENAME;
  static const char * HEADERS_FILENAME;
  static const char * INFO_PARAMS_FILENAME;
  static const char * URL_FILENAME;

  std::string configPath;
  std::string encrptFilepath;
  std::string headersFilepath;
  std::string info_paramsFilepath;
  std::string urlFilepath;
};

Config& get_config();

#endif // BOOKCOURT_CONFIG_H
