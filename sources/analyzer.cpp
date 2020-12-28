//
// Created by michael on 22.12.2020.
//

#include "analyzer.hpp"
std::ostream& operator<<(std::ostream& out, analyzer& a) {
  for (const auto & current_account : a.accounts){
    for (size_t i = 0; i < current_account->filenames.size(); ++i){
      out << current_account->broker_name << " "
          << current_account->filenames[i] << std::endl;
    }
  }
for (const auto & current_account : a.accounts){
  out << *current_account << std::endl;
}
  return out;
}
std::ostream& operator<<(std::ostream& out, account& ac) {
  out << "broker:"    << ac.broker_name
      << " account:"  << ac.number_account
      << " files:"    << ac.filenames.size()
      << " lastdate:" << ac.lastdate;
  return out;
}
void analyzer::parse_dir_info(const filesystem::path& path_dir,
                              const std::string &broker) {
  filesystem::path current_path;
  if (filesystem::is_symlink(path_dir)){
    current_path = filesystem::read_symlink(path_dir);
  } else {
    current_path = path_dir;
  }
  for (const auto &i : filesystem::directory_iterator(current_path)){
    if (filesystem::is_directory(i)) {
      parse_dir_info(i.path(),broker);
    } else {
      if (!check_filename(i.path())) continue;
      std::string ac_number = get_number_account(i.path().filename().string());
      account* acc = nullptr;
      for (const auto& current_account : accounts) {
        if ((current_account->broker_name == broker) &&
            (current_account->number_account == ac_number)) {
          acc = current_account;
          break;
        }
      }
      if (!acc) {
      acc = new account;
      acc->filenames.push_back(i.path().filename().string());
      acc->broker_name = broker;
      acc->number_account = ac_number;
      accounts.push_back(acc);
      } else {
        acc->filenames.push_back(i.path().filename().string());
      }
    }
  }
}
void analyzer::main_analyzer(const filesystem::path& path_ftp) {
  path_to_ftp = path_ftp;
  if (!filesystem::exists(path_ftp))
    throw (std::string("Path is wrong\n"));
  if (!filesystem::is_directory(path_ftp))
    throw (std::string("Dir is wrong\n"));
 // out << path_to_ftp << std::endl;
  for (const auto &i : filesystem::directory_iterator(path_to_ftp)){
    if (!filesystem::is_directory(i)) continue;
    if (i.path().filename().string() == "docs") continue;
   // out << i.path().filename().string() << std::endl;
    std::string broker = i.path().filename().string();
    parse_dir_info(i.path(),broker);
  }
  set_lastdates();
}
const std::string extension_txt = ".txt";
const std::string balance = "balance";
const std::string digits = "0123456789";
const char under_line = '_';
const int number_length = 8;
const int date_length = 8;
const char dot = '.';
std::string analyzer::get_number_account(const std::string &filename) {
  std::string tmp = filename.substr(filename.find(under_line) + 1,
                                    number_length);
  return tmp;
}
bool analyzer::check_filename(const filesystem::path& path_file) {
  if (path_file.extension() != extension_txt) return false;
  else if (path_file.stem().has_extension())
    return false;
  std::string filename = path_file.stem().filename().string();
  size_t underline_pos = filename.find(under_line);
  std::string tmp;
  if (underline_pos) tmp = filename.substr(0, underline_pos);
  else return false;
  if (tmp != balance) return false;
  filename = filename.substr(underline_pos + 1);
  underline_pos = filename.find(under_line);
  if (!underline_pos) return false;
  else {
    tmp = filename.substr(0, underline_pos);
    if (tmp.size() != number_length) return false;
    if (tmp.find_first_not_of(digits) != std::string::npos) return false;
    tmp = filename.substr(underline_pos + 1);
    if (tmp.size() != date_length) return false;
    if (tmp.find_first_not_of(digits) != std::string::npos) return false;
  }
  return true;
}
void analyzer::set_lastdates() {
  for (auto & current_account : accounts) {
    current_account->lastdate = get_date(current_account->filenames[0]);
    for (auto & current_filename : current_account->filenames){
      std::string tmp = get_date(current_filename);
    if (tmp > current_account->lastdate) current_account->lastdate = tmp;
    }
  }
}
std::string analyzer::get_date(const std::string& filename) const {
  std::string date = filename.substr(filename.find(under_line) + 1,
                                     filename.find(dot) -
                                     filename.find(under_line) - 1);
  date = date.substr(date.find(under_line) + 1);
  return date;
}
analyzer::~analyzer() {
  for (const auto & current_account : accounts) {
    delete current_account;
  }
}
