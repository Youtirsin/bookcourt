#ifndef FUNC_H
#define FUNC_H

#include <boost/json.hpp>
#include <iostream>

std::string create_timestamp();

boost::json::object read_json_obj_from_file(const std::string &path);

class TimeSignaturer {
 public:
  TimeSignaturer(const std::string &key_, const std::string &iv_);

  TimeSignaturer(TimeSignaturer &&other);

  TimeSignaturer& operator=(TimeSignaturer &&other);

  std::pair<std::string, std::string> sign();

 private:
  std::string key, iv;

  static std::string encrypt(const std::string &KEY, const std::string &IV,
                             const std::string &text);
};


TimeSignaturer init_signaturer(boost::json::object &config);

#endif // FUNC_H