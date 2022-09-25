#include "bookcourt_util.h"

#include <cryptopp/aes.h>
#include <cryptopp/base64.h>   // Base64Encoder
#include <cryptopp/filters.h>  // StringSource
#include <cryptopp/modes.h>    // CBC_Mode

#include <boost/date_time/gregorian/gregorian.hpp>  // gregorian::date
#include <boost/format.hpp>                         // boost::format
#include <boost/json.hpp>                           // json
#include <ctime>                                    // time
#include <fstream>                                  // ifstream
#include <iostream>

std::string read_file(const std::string &path) {
  std::ifstream reader(path);
  std::string res, line;
  while (std::getline(reader, line)) {
    res += line + '\n';
  }
  return res;
}

std::string create_timestamp() {
  boost::gregorian::date today = boost::gregorian::day_clock::local_day();
  auto timestamp = boost::format("%1%-%2%-%3%") % today.year() %
                   today.month().as_number() % today.day();
  return timestamp.str();
}

boost::json::object read_json_obj_from_file(const std::string &path) {
  return boost::json::parse(read_file(path)).as_object();
}

TimeSignaturer::TimeSignaturer(const std::string &key_, const std::string &iv_)
    : key(key_), iv(iv_) {}

TimeSignaturer &TimeSignaturer::operator=(TimeSignaturer &&other) {
  key = std::move(other.key);
  iv = std::move(other.iv);
  return *this;
}

TimeSignaturer::TimeSignaturer(TimeSignaturer &&other) {
  *this = std::move(other);
}

std::pair<std::string, std::string> TimeSignaturer::sign() {
  auto now = std::to_string(time(nullptr) * 1000);
  return {now, encrypt(key, iv, now)};
}

std::string TimeSignaturer::encrypt(const std::string &KEY,
                                    const std::string &IV,
                                    const std::string &text) {
  using namespace CryptoPP;

  std::string res, cipher;
  Base64Encoder encoder(new StringSink(res));
  SecByteBlock key((const byte *)(KEY.c_str()), KEY.size());
  SecByteBlock iv((const byte *)(IV.c_str()), IV.size());

  try {
    CBC_Mode<AES>::Encryption e;
    e.SetKeyWithIV(key, key.size(), iv);

    StringSource s(text, true,
                   new StreamTransformationFilter(
                       e,
                       new StringSink(cipher))  // StreamTransformationFilter
    );                                          // StringSource
  } catch (const Exception &e) {
    std::cerr << e.what() << std::endl;
    return "";
  }

  encoder.Put((const byte *)&cipher[0], cipher.size());

  return res;
}

TimeSignaturer init_signaturer(boost::json::object &config) {
  TimeSignaturer res(config["KEY"].as_string().c_str(),
                     config["IV"].as_string().c_str());
  return res;
}