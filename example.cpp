// usage example args_reader

#include "args_reader.hpp"

int main (int argc, char * argv[]) {

  auto usage = [](){ 
    std::cout << "\nUsage: ./example -h (--help) <opt> -n (--name) <opt> -i (--index) <opt>\n\n";
    return 1;
  };

  std::vector<std::string> args(argc);
  std::copy(argv, argv+argc, args.begin());

  bool help = false;
  fetch_arg(args, "-h",     help);
  fetch_arg(args, "--help", help);

  if (help) usage();

  // you can use multiple identifiers for the same option
  uint64_t index = 0;
  fetch_arg(args, "-i",      index);
  fetch_arg(args, "--index", index);

  // and the return value to check if the argument was found and read
  std::string name = "";
  bool found = fetch_arg(args, "-n",     name) or
               fetch_arg(args, "--name", name);

  if (found) {
    std::cout << "Hello " << name << " No." << index << std::endl;
  }
  else {
    std::cout << "I don't know your name No." << index << std::endl;
    usage();
  };

  return 0;
}