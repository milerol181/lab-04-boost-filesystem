#include <analyzer.hpp>

int main(int argc, char* argv[]) {
  filesystem::path path_ftp;
  if (argc == 1) path_ftp = filesystem::current_path();
  else if (argc == 2) path_ftp = argv[1];
  else {
    std::cout << "Too many arguments" << std::endl;
    return -1;
  }
  try {
    analyzer analyzing(path_ftp);
    std::cout << analyzing << std::endl;
  } catch (std::string error) {
    std::cout << error << std::endl;
    return -1;
  }
  return 0;
}