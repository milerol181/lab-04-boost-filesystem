//
// Created by michael on 22.12.2020.
//

#include "analyzer.hpp"
analyzer::analyzer(const filesystem::path& pathToFtp)
    : path_to_ftp(pathToFtp) {

}
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
  out << ac.broker_name << std::endl;
  out << ac.filenames.size() << std::endl;
  out << ac.number_account << std::endl;
  return out;
}
void analyzer::parse_dir_info(const filesystem::path& pathToDir,
                              const std::string &broker) {
  for (const auto &i : filesystem::directory_iterator(pathToDir)){
    if (filesystem::is_directory(i)) {
      parse_dir_info(i.path(),broker);
      continue;
    } else {
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
   // out << i.path().filename().string() << std::endl;
    std::string broker = i.path().filename().string();
    parse_dir_info(i.path(),broker);
}
  }
std::string analyzer::get_number_account(const std::string &filename) {
  if (filename.find('_') == std::string::npos) return "error";
  std::string tmp = filename.substr(filename.find('_') + 1,8);
  return tmp;
}
