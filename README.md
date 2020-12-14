# ARGS-READER

Simple argument reader that checks a vector for a given identifier and
the type(s) of the variable(s) passed to it. Reads the argument(s) and
does the type conversion from string. Use with `-std=c++17`. 

maintainer  

Katharina von Sturm - katharinavonsturm@gmail.com  

## Usage

Read the arguments into a vector and pass it to the `args-reader` together
with the identifier and the variable that will store the value.

```c++
#include "args_reader.hpp"

int main (int argc, char * argv[]) {

  std::vector<std::string> args(argc);
  std::copy(argv, argv+argc, std::begin(args));

  bool help = false;
  int index = 0;

  // capture single bool argument, is set to true if no argument provided
  fetch_arg(args, "-h", help);

  // you can use multiple identifiers for the same option
  // and the return value to check if the argument was found and read
  bool found = fetch_arg(args, "-i",      index) or
               fetch_arg(args, "--index", index);

  // you can read a fixed number of same-type arguments into a vector 
  vector<int> v(5,0);
  fetch_arg(args, "-n", v);

  // or an arbitrary number of arguments into a zero-size vector that captures
  // everything before the next identifier is found (expected to start with `-`)
  vector<int> vz;
  fetch_arg(args, "-a", vz);

  // also mixed types are possible
  double a = 0.; std::string s = "";
  fetch_arg(args, "-m", a, s);

  // arguments can also directly fetched using `argc` and `argv`
  // maintaining all functionality
  fetch_arg(argc, argv, "-d", s);
}
```
## Examples

See also the example program. `./example -h` prints usage message.  
Compile it with

```
g++ -std=c++17 example.cpp -o example
```

## TODO

  - use `boost::lexical_cast<T>`
