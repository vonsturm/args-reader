// usage example args_reader

#include "args_reader.hpp"

//using namespace args_reader;

int main (int argc, char * argv[]) {

  auto usage = [](){ 
    std::cout << "\nUsage: ./example\n\n";
    std::cout << "Options:\n";
    std::cout << "\t-h (--help) <bool/void>\n";
    std::cout << "\t-n (--name) <string>\n";
    std::cout << "\t--nums <4x-double>\n";
    std::cout << "\t--arb <int>...\n";
    std::cout << "\t--mix <double double string>\n\n";
    std::cout << "\t-c <char>\n\n";
    return 1;
  };

  std::vector<std::string> args(argc);
  std::copy(argv, argv+argc, args.begin());

  bool help = false;
  fetch_arg(args, "-h",     help);
  fetch_arg(args, "--help", help);

  if (help) usage();

  // you can use multiple identifiers for the same option and use the
  // return value to check if the argument was found and read
  std::string name = "";
  bool found = fetch_arg(args, "-n",     name) or
               fetch_arg(args, "--name", name);
  if (found) std::cout << "Hello " << name << std::endl;
  
  // read into vector with fixed number of elements
  std::vector<double> a(4,0);
  found = fetch_arg(args, "--nums", a);
  if (found) std::cout << "Numbers: " << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << std::endl;

  // read arbitrary number of variables into zero-size vector
  std::vector<int> v;
  found = fetch_arg(args, "--arb", v);
  if (found) {
    std::cout << "Arbitrary: ";
    for (auto e : v) std::cout << e << " ";
    std::cout << std::endl;
  }

  // read into mixed types of variables
  double b=0,c=0;
  std::string s = "default";
  found = fetch_arg(args, "--mix", b,c,s);
  if (found) std::cout << "Mix: " << b << " " << c << " " << s << std::endl;

  found = fetch_arg(argc, argv, "-d", b);
  if (found) std::cout << "direct: " << b << std::endl;

  char cc;
  found = fetch_arg(argc, argv, "-c", cc);
  if (found) std::cout << "Char: " << cc << std::endl;

  return 0;
}