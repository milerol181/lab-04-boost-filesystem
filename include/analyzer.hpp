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
  friend std::ostream &operator<<(std::ostream &out, analyzer &a);
  void parse_dir_info(const filesystem::path&path_dir,
                      const std::string &broker);
  void main_analyzer(const filesystem::path&path_ftp);
  std::string get_number_account(const std::string &filename);
  bool check_filename(const filesystem::path&path_file);
  std::string get_date(const std::string& filename) const;
  void set_lastdates();
  ~analyzer();
 private:
  filesystem::path path_to_ftp;
  std::vector<struct account*> accounts;

};

#endif  // TEMPLATE_ANALYZER_HPP
