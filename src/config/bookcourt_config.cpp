#include "bookcourt_config.h"

#include <boost/filesystem.hpp>

const char * Config::ENCRYPT_FILENAME      = "encrypt.json";
const char * Config::HEADERS_FILENAME      = "headers.json";
const char * Config::INFO_PARAMS_FILENAME  = "info_params.json";
const char * Config::URL_FILENAME          = "urls.json";

bool Config::init(const std::string &configPath_) {
  std::cout << "[bookcourt_config] checking config directory...\n";
  if (!boost::filesystem::exists(configPath_)) {
    std::cerr << "[bookcourt_config] config directory is missing.\n\n";
    return false;
  }
  std::cout << "[bookcourt_config] config directory is " << configPath << ".\n\n";
  configPath = configPath_;

  encrptFilepath = configPath + ENCRYPT_FILENAME;
  headersFilepath = configPath + HEADERS_FILENAME;
  info_paramsFilepath = configPath + INFO_PARAMS_FILENAME;
  urlFilepath = configPath + URL_FILENAME;

  auto checkFileExists = [](const std::string &path) {
    std::cout << "[bookcourt_config] checking config file: " << path << ".\n";
    if (!boost::filesystem::exists(path.c_str())) {
      std::cerr << "[bookcourt_config] config file: " << path << " doesn't exist.\n\n";
      return false;
    }
    std::cout << "[bookcourt_config] found file: " << path << ".\n\n";
    return true;
  };
  if (!checkFileExists(encrptFilepath)) return false;
  if (!checkFileExists(headersFilepath)) return false;
  if (!checkFileExists(info_paramsFilepath)) return false;
  if (!checkFileExists(urlFilepath)) return false;
  return true;
}

Config& get_config() {
  static Config *config = new Config;
  return *config;
}
