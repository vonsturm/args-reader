# ARGS-READER

Simple argument reader that checks a vector for a given identifier and
the type of the variable passed to it. Reads the argument and does the
type conversion from string. Use with `-std=c++17`. 

maintainer  

Katharina von Sturm - katharinavonsturm@gmail.com  

## Usage

Read the arguments in your main into a vector and pass it to the
args-reader together with the identifier and the variable that will
store the value.

```c++
#include "args_reader.hpp"

int main (int argc, char * argv[]) {

  std::vector<std::string> args(argc);
  std::copy(argv, argv+argc, std::begin(args));

  bool help = false;
  int index = 0;

  fetch_arg(args, "-h", help);

  // you can use multiple identifiers for the same option
  // and the return value to check if the argument was found and read
  bool found = fetch_arg(args, "-i",      index) or
               fetch_arg(args, "--index", index);
}
```

See also the example program. Compile it using

```
g++ -std=c++17 example.cpp -o example
```

## TODO

  - use `boost::lexical_cast<T>`
  - allow directly passing `argc` and `argv` to `fetch_arg`
