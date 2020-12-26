#include <analyzer.hpp>

int main() {

  filesystem::path path_ftp = "../misc/ftp";
  analyzer analyzing(path_ftp);
  analyzing.main_analyzer(path_ftp);
  std::cout << analyzing << std::endl;

}