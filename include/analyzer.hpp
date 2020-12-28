//
// Created by michael on 22.12.2020.
//

#ifndef TEMPLATE_ANALYZER_HPP
#define TEMPLATE_ANALYZER_HPP
#include <boost/filesystem.hpp>
#include <ostream>
#include <iostream>
using namespace boost;
struct account{
  std::string broker_name;
  std::vector<std::string> filenames;
  std::string number_account;
  std::string lastdate;
};
std::ostream &operator<<(std::ostream &out, account &ac);
class analyzer {
 public:
  analyzer(const filesystem::path& pathToFtp);
  friend std::ostream &operator<<(std::ostream &out, analyzer &a);
  void parse_dir_info(const filesystem::path& pathToDir,
                      const std::string &broker);
  void main_analyzer(const filesystem::path& pathToFtp);
  std::string get_number_account(const std::string &filename);
  bool check_filename(const filesystem::path& pathToFile);
  std::string get_date(const std::string& filename) const;
  void set_lastdates();
 private:
  filesystem::path path_to_ftp;
  std::vector<struct account*> accounts;

};

#endif  // TEMPLATE_ANALYZER_HPP
