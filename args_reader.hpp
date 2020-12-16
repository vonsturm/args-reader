// The MIT License (MIT)
//
// Copyright (c) 2020 Katharina von Sturm
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//
// ============================================================================
//    ___   ___   __    __      ___   ____   ___   ___   ____  ___
//   / /_\ | |_) / /`_ ( (` __ | |_) | |_   / /_\ | | \ | |_  | |_)
//  /_/   \|_| \ \_\_/ _)_)    |_| \ |_|__ /_/   \|_|_/ |_|__ |_| \
//
// Author  : Katharina von Sturm
// Date    : 29.09.2020
// Usage   : fetch_arg(<arguments>, <identifier>, <var>...)
//           fetch_arg(<arguments>, <identifier>, std::vector<T> <var>)
//           fetch_arg(<argc>, <argv>, <identifier>, <var>...)
//           fetch_arg(<argc>, <argv>, <identifier>, std::vector<T> <var>)
//
//    <arguments>  : std::vector<std::string>
//    <identifier> : std::string - expected to start with '-' in case of
//                   zero size vector or single boolean <var> and no option
//    <var>        : std::vector<T> or arbitrary number of variables of
//                   arbitrary type
//
//    Searches <arguments> for <identifier> and reads the next
//    element(s) into arbitrary number of <var> of arbitrary type. If
//    <var> is a vector the number of elements read is equal to the
//    vector size. If the vector size is zero all elements after
//    <identifier> until next <identifier> are read into the vector. In
//    the vector case only same_type arguments can be stored. Single
//    boolean type variables are set to true in case no option is
//    provided after <identifier>.
//
//    returns `true` if <identifier> was found, `false` otherwise
//
// ============================================================================

#include <iostream>
#include <algorithm>
#include <limits>
#include <vector>
#include <stdexcept>

// global variables
std::vector<std::string> vpar(0); uint16_t vit = 0;

template <class T>
void ignore_unused(T&) {} 

template<typename T>
void check_convert_and_assign(T & vv) {

  auto exit_on_conversion_error = [] (std::string s) {
    std::cerr << "\33[1;31m[ ARGS-READER::CONVERSION_ERROR ]\33[m expected: " << s << "; found : " << vpar[vit] << "\n";
    exit(EXIT_FAILURE);
  };
  auto exit_on_number_error = [] (size_t s) {
    std::cerr << "\33[1;31m[ ARGS-READER::NUMBER_ERROR ]\33[m insufficient given:" << s << "\n";
    exit(EXIT_FAILURE);
  };
  ignore_unused(exit_on_conversion_error);
  ignore_unused(exit_on_number_error);

  if (vpar.size() > vit) {
         if constexpr (std::is_same<T, bool>       ::value) vv = vpar[vit] == "true" or (vpar[vit])[0] == '-' ? true : false;
    else if ((vpar[vit])[0] == '-') exit_on_number_error(vpar.size());
    else if constexpr (std::is_same<T, int16_t>    ::value) { try { vv = stoi (vpar[vit]); } catch(...) { exit_on_conversion_error("int16_t") ; } }
    else if constexpr (std::is_same<T, int32_t>    ::value) { try { vv = stoi (vpar[vit]); } catch(...) { exit_on_conversion_error("int32_t") ; } }
    else if constexpr (std::is_same<T, int64_t>    ::value) { try { vv = stol (vpar[vit]); } catch(...) { exit_on_conversion_error("int64_t") ; } }
    else if constexpr (std::is_same<T, uint16_t>   ::value) { try { vv = stoul(vpar[vit]); } catch(...) { exit_on_conversion_error("uint16_t"); } }
    else if constexpr (std::is_same<T, uint32_t>   ::value) { try { vv = stoul(vpar[vit]); } catch(...) { exit_on_conversion_error("uint32_t"); } }
    else if constexpr (std::is_same<T, uint64_t>   ::value) { try { vv = stoul(vpar[vit]); } catch(...) { exit_on_conversion_error("uint64_t"); } }
    else if constexpr (std::is_same<T, float>      ::value) { try { vv = stof (vpar[vit]); } catch(...) { exit_on_conversion_error("float")   ; } }
    else if constexpr (std::is_same<T, double>     ::value) { try { vv = stod (vpar[vit]); } catch(...) { exit_on_conversion_error("double")  ; } }
    else if constexpr (std::is_same<T, std::string>::value) { try { vv = vpar[vit];        } catch(...) { exit_on_conversion_error("string")  ; } }
    else if constexpr (std::is_same<T, bool>       ::value) { try { vv = vpar[vit] == "true" or (vpar[vit])[0] == '-' ? true : false; }
                                                                                             catch(...) { exit_on_conversion_error("bool or void"); } }
    else exit_on_number_error(vpar.size());
  }
  else if constexpr (std::is_same<T, bool>::value) vv = "true";
  else exit_on_number_error(vpar.size());
  vit++;
}

template<typename... T>
bool fetch_arg(const std::vector<std::string> & args, std::string identifier, T&... var) {
  auto result = find(args.begin(), args.end(), identifier);

  if (result != args.end()) { // argument not found
    vpar.resize(0); vit = 0;
    if ((uint64_t)abs(args.end()-result) > sizeof...(var)) { // only copy necessary stuff and only if there is stuff to copy
      std::copy(result + 1, result + sizeof...(var) + 1, std::back_inserter(vpar));
    }
    (check_convert_and_assign(var),...);
    return true;
  }
  return false;
}

template<typename T>
bool fetch_arg(const std::vector<std::string> & args, std::string identifier, std::vector<T> & var) {
  auto result = find(args.begin(), args.end(), identifier);
  
  if (result != args.end()) {
    vpar.resize(0); vit = 0;
    if (var.size() == 0) {
      auto result1 = std::find_if(result + 1, args.end(), [](const std::string &s) { return s[0] == '-'; });
      var.resize(result1-result-1);
    }
    if ((uint64_t)abs(args.end()-result) > var.size()) {
      std::copy(result + 1, result + var.size() + 1, std::back_inserter(vpar));
    }
    for (auto & v : var) check_convert_and_assign(v);
    return true;
  }
  return false;
}

template<typename... T>
bool fetch_arg(int const argc, char * const argv[], std::string identifier, T&... var) {
  std::vector<std::string> args(argc);
  std::copy(argv, argv+argc, args.begin());

  return fetch_arg(args, identifier, var...);
}

template<typename T>
bool fetch_arg(int const argc, char * const argv[], std::string identifier, std::vector<T> & var) {
  std::vector<std::string> args(argc);
  std::copy(argv, argv+argc, args.begin());

  return fetch_arg(args, identifier, var);
}
