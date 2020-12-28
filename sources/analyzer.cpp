//
// Created by michael on 22.12.2020.
//

#include "analyzer.hpp"
analyzer::analyzer(const filesystem::path& pathToFtp)
    : path_to_ftp(pathToFtp) {}
std::ostream& operator<<(std::ostream& out, analyzer& a) {
  for (const auto & current_account : a.accounts){
    for (size_t i = 0; i < current_account->filenames.size(); ++i){
      out << current_account->broker_name << " " <<
          current_account->filenames[i] << std::endl;
    }
  }
for (const auto & current_account : a.accounts){
  out << *current_account << std::endl;
}
  return out;
}
std::ostream& operator<<(std::ostream& out, account& ac) {
  out << " broker: "  << ac.broker_name
      << " account: " << ac.number_account
      << " files: "   << ac.filenames.size()
      << " lastdate: " << ac.lastdate;
  return out;
}
void analyzer::parse_dir_info(const filesystem::path& pathToDir,
                              const std::string &broker) {
  filesystem::path current_path;
  if (filesystem::is_symlink(pathToDir)){
    current_path = filesystem::read_symlink(pathToDir);
  } else {
    current_path = pathToDir;
  }
  for (const auto &i : filesystem::directory_iterator(current_path)){
    if (filesystem::is_directory(i)) {
      parse_dir_info(i.path(),broker);
      continue;
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
void analyzer::main_analyzer(const filesystem::path& pathToFtp) {
  path_to_ftp = pathToFtp;
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
  if (filename.find(under_line) == std::string::npos) return "error";
  std::string tmp = filename.substr(filename.find('_') + 1,
                                    number_length);
  return tmp;
}
bool analyzer::check_filename(const filesystem::path& pathToFile) {
  if (pathToFile.extension() != extension_txt) return false;
  else if (pathToFile.stem().has_extension())
    return false;
  std::string filename = pathToFile.stem().filename().string();
  size_t underscore_pos = filename.find(under_line);
  std::string tmp;
  if (underscore_pos) tmp = filename.substr(0, underscore_pos);
  else return false;
  if (tmp != balance) return false;
  filename = filename.substr(underscore_pos + 1);
  underscore_pos = filename.find(under_line);
  if (!underscore_pos) return false;
  else {
    tmp = filename.substr(0, underscore_pos);
    if (tmp.size() != number_length) return false;
    if (tmp.find_first_not_of(digits) != std::string::npos) return false;
    tmp = filename.substr( underscore_pos + 1);
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
