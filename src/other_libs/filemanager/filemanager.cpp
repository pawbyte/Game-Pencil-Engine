/*

 MIT License

 Copyright © 2021 Samuel Venable

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*/

#include <filesystem>
#include <sstream>
#include <set>

#include <cwchar>
#include <cstddef>

#include "../filemanager.h"

#if defined(_WIN32)
#include <windows.h>
#endif

namespace fs = std::filesystem;

using std::string;
using std::wstring;

using std::size_t;
using std::uintmax_t;

using std::vector;
using std::stringstream;

namespace tkg {

  bool filemanager::directory_copy_retained(string dname, string newname) {
    std::error_code ec;
    bool result = false;
    const fs::path path1 = fs::u8path(dname);
    const fs::path path2 = fs::u8path(newname);
    const fs::path path3 = fs::u8path(path2.u8string().substr(0, path1.u8string().length()));
    if (retained_string.empty() && retained_length == 0) {
      retained_length = path1.u8string().length();
      retained_string = path2.u8string().substr(retained_length);
    }
    if (directory_exists(dname)) {
      if ((filename_name(path1.u8string()) != filename_name(path2.u8string()) &&
        filename_path(path1.u8string()) == filename_path(path2.u8string())) ||
        path1.u8string() != path3.u8string()) {
        fs::copy(path1, path2, fs::copy_options::recursive, ec);
        result = (ec.value() == 0);
      } else if (path1.u8string() == path3.u8string()) {
        vector<string> itemvec = directory_contents(dname, "*.*", true);
        if (!directory_exists(newname)) {
          directory_create(newname);
          for (const string &item : itemvec) {
            if (directory_exists(filename_remove_slash(item)) &&
              filename_remove_slash(item).substr(retained_length) != retained_string) {
              directory_copy_retained(filename_remove_slash(item), filename_add_slash(path2.u8string()) +
              filename_name(filename_remove_slash(item)));
            } else if (file_exists(item)) {
              fs::copy(item, filename_add_slash(path2.u8string()) + filename_name(item), ec);
            }
          }
          result = (directory_exists(newname) && szsource == directory_size(newname));
        }
      }
    }
    return result;
  }

  string filemanager::string_replace_all(string str, string sstr, string nstr) {
    size_t pos = 0;
    while ((pos = str.find(sstr, pos)) != string::npos) {
      str.replace(pos, sstr.length(), nstr);
      pos += nstr.length();
    }
    return str;
  }

  vector<string> filemanager::string_split(string str, string delim) {
    vector<string> vec;
    stringstream sstr(str);
    string tmp;
    while (std::getline(sstr, tmp, delim[0]))
      vec.push_back(tmp);
    return vec;
  }

  wstring filemanager::widen(string str) {
    #if defined(_WIN32)
    size_t wchar_count = str.size() + 1;
    vector<wchar_t> buf(wchar_count);
    return wstring { buf.data(), (size_t)MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf.data(), (int)wchar_count) };
    #else
    const fs::path path = fs::u8path(str);
    return path.wstring();
    #endif
  }

  string filemanager::narrow(wstring wstr) {
    #if defined(_WIN32)
    int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
    vector<char> buf(nbytes);
    return string { buf.data(), (size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, NULL, NULL) };
    #else
    const fs::path path = wstr;
    return path.u8string();
    #endif
  }

  string filemanager::get_working_directory() {
    std::error_code ec;
    string result = filename_add_slash(fs::current_path(ec).u8string());
    return (ec.value() == 0) ? result : "";
  }

  bool filemanager::set_working_directory(string dname) {
    std::error_code ec;
    const fs::path path = fs::u8path(dname);
    fs::current_path(path, ec);
    return (ec.value() == 0);
  }

  string filemanager::get_executable_path() {
    string path;
    #if defined(_WIN32)
    wchar_t buffer[MAX_PATH];
    if (GetModuleFileNameW(NULL, buffer, MAX_PATH) != 0) {
      path = narrow(buffer);
    }
    #elif defined(__APPLE__) && defined(__MACH__)
    char buffer[PROC_PIDPATHINFO_MAXSIZE];
    if (proc_pidpath(getpid(), buffer, sizeof(buffer)) > 0) {
      path = string(buffer) + "\0";
    }
    #elif defined(__linux__) && !defined(__ANDROID__)
    char *buffer = realpath("/proc/self/exe", NULL);
    path = buffer ? : "";
    free(buffer);
    #elif defined(__FreeBSD__)
    size_t length;
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
    if (sysctl(mib, 4, NULL, &length, NULL, 0) == 0) {
      path.resize(length, '\0');
      char *buffer = path.data();
      if (sysctl(mib, 4, buffer, &length, NULL, 0) == 0) {
        path = string(buffer) + "\0";
      }
    }
    #endif
    return path;
  }

  string filemanager::get_temp_directory() {
    std::error_code ec;
    string result = filename_add_slash(fs::temp_directory_path(ec).u8string());
    return (ec.value() == 0) ? result : "";
  }

  string filemanager::environment_get_variable(string name) {
    #if defined(_WIN32)
    string value;
    wchar_t buffer[32767];
    wstring u8name = widen(name);
    if (GetEnvironmentVariableW(u8name.c_str(), buffer, 32767) != 0) {
      value = narrow(buffer);
    }
    return value;
    #elif defined(__unix__)
    char *value = getenv(name.c_str());
    return value ? value : "";
    #endif
    return "";
  }

  bool filemanager::environment_set_variable(string name, string value) {
    #if defined(_WIN32)
    wstring u8name = widen(name);
    wstring u8value = widen(value);
    if (value == "") return (SetEnvironmentVariableW(u8name.c_str(), NULL) != 0);
    return (SetEnvironmentVariableW(u8name.c_str(), u8value.c_str()) != 0);
    #elif defined(__unix__)
    if (value == "") return (unsetenv(name.c_str()) == 0);
    return (setenv(name.c_str(), value.c_str(), 1) == 0);
    #endif
    return false;
  }

  string filemanager::environment_expand_variables(string str) {
    if (str.find("${") == string::npos) return str;
    string pre = str.substr(0, str.find("${"));
    string post = str.substr(str.find("${") + 2);
    if (post.find('}') == string::npos) return str;
    string variable = post.substr(0, post.find('}'));
    post = post.substr(post.find('}') + 1);
    string value = environment_get_variable(variable);
    return environment_expand_variables(pre + value + post);
  }

  string filemanager::filename_canonical(string fname) {
    string result = "";
    if (directory_exists(fname)) {
      result = filename_add_slash(fname, true);
    } else if (file_exists(fname)) {
      result = filename_weakly_canonical(fname);
    }
    return result;
  }

  string filemanager::filename_weakly_canonical(string fname) {
    std::error_code ec;
    const fs::path path = fs::u8path(fname);
    string result = fs::weakly_canonical(path, ec).u8string();
    #ifdef _WIN32
    result = string_replace_all(result, "/", "\\");
    #endif
    if (ec.value() == 0 && directory_exists(result)) {
      return filename_add_slash(result);
    }
    return (ec.value() == 0) ? result : "";
  }

  string filemanager::filename_remove_slash(string dname, bool canonical) {
    if (canonical) dname = filename_weakly_canonical(dname);
    #ifdef _WIN32
    while (!dname.empty() && (dname.back() == '\\' || dname.back() == '/')) dname.pop_back();
    #else
    while (!dname.empty() && dname.back() == '/') dname.pop_back();
    #endif
    return dname;
  }

  string filemanager::filename_add_slash(string dname, bool canonical) {
    #ifdef _WIN32
    filename_remove_slash(dname, canonical);
    dname += "\\";
    #else
    if (canonical) dname = filename_weakly_canonical(dname);
    if (dname.back() != '/') dname += "/";
    #endif
    return dname;
  }

  string filemanager::filename_path(string fname) {
    size_t fpos = fname.find_last_of("/\\");
    return fname.substr(0,fpos + 1);
  }

  string filemanager::filename_name(string fname) {
    size_t fpos = fname.find_last_of("/\\");
    return fname.substr(fpos + 1);
  }

  string filemanager::filename_ext(string fname) {
    fname = filename_name(fname);
    size_t fpos = fname.find_last_of(".");
    if (fpos == string::npos)
      return "";
    return fname.substr(fpos);
  }

  uintmax_t filemanager::file_size(string fname) {
    std::error_code ec;
    if (!file_exists(fname)) return 0;
    const fs::path path = fs::u8path(fname);
    uintmax_t result = fs::file_size(path, ec);
    return (ec.value() == 0) ? result : 0;
  }

  bool filemanager::file_exists(string fname) {
    std::error_code ec;
    const fs::path path = fs::u8path(fname);
    return (fs::exists(path, ec) && ec.value() == 0 &&
      (!fs::is_directory(path, ec)) && ec.value() == 0);
  }

  bool filemanager::file_delete(string fname) {
    std::error_code ec;
    if (!file_exists(fname)) return false;
    const fs::path path = fs::u8path(fname);
    return (fs::remove(path, ec) && ec.value() == 0);
  }

  bool filemanager::file_rename(string oldname, string newname) {
    std::error_code ec;
    if (!file_exists(oldname)) return false;
    if (!directory_exists(filename_path(newname)))
      directory_create(filename_path(newname));
    const fs::path path1 = fs::u8path(oldname);
    const fs::path path2 = fs::u8path(newname);
    fs::rename(path1, path2, ec);
    return (ec.value() == 0);
  }

  bool filemanager::file_copy(string fname, string newname) {
    std::error_code ec;
    if (!file_exists(fname)) return false;
    if (!directory_exists(filename_path(newname)))
      directory_create(filename_path(newname));
    const fs::path path1 = fs::u8path(fname);
    const fs::path path2 = fs::u8path(newname);
    fs::copy(path1, path2, ec);
    return (ec.value() == 0);
  }

  uintmax_t filemanager::directory_size(string dname) {
    uintmax_t result = 0;
    if (!directory_exists(dname)) return 0;
    const fs::path path = fs::u8path(filename_remove_slash(dname, true));
    if (fs::exists(path)) {
      fs::directory_iterator end_itr;
      for (fs::directory_iterator dir_ite(path); dir_ite != end_itr; dir_ite++) {
        fs::path file_path = fs::u8path(filename_canonical(dir_ite->path().u8string()));
        if (!fs::is_directory(dir_ite->status())) {
          result += file_size(file_path.u8string());
        } else {
          result += directory_size(file_path.u8string());
        }
      }
    }
    return result;
  }

  bool filemanager::directory_exists(string dname) {
    std::error_code ec;
    dname = filename_remove_slash(dname, false);
    const fs::path path = fs::u8path(dname);
    return (fs::exists(path, ec) && ec.value() == 0 &&
      fs::is_directory(path, ec) && ec.value() == 0);
  }

  bool filemanager::directory_create(string dname) {
    std::error_code ec;
    dname = filename_remove_slash(dname, true);
    const fs::path path = fs::u8path(dname);
    return (fs::create_directories(path, ec) && ec.value() == 0);
  }

  bool filemanager::directory_destroy(string dname) {
    std::error_code ec;
    if (!directory_exists(dname)) return false;
    dname = filename_remove_slash(dname, true);
    const fs::path path = fs::u8path(dname);
    return (fs::remove(path, ec) && ec.value() == 0);
  }

  bool filemanager::directory_rename(string oldname, string newname) {
    std::error_code ec;
    if (!directory_exists(oldname)) return false;
    if (!directory_exists(newname)) directory_create(newname);
    oldname = filename_remove_slash(oldname, true);
    newname = filename_remove_slash(newname, true);
    bool result = false;
    const fs::path path1 = fs::u8path(oldname);
    const fs::path path2 = fs::u8path(newname);
    const fs::path path3 = fs::u8path(path2.u8string().substr(0, path1.u8string().length()));
    if (directory_exists(oldname)) {
      if ((filename_name(path1.u8string()) != filename_name(path2.u8string()) &&
        filename_path(path1.u8string()) == filename_path(path2.u8string())) ||
        path1.u8string() != path3.u8string()) {
        fs::rename(path1, path2, ec);
        result = (ec.value() == 0);
      }
    }
    return result;
  }

  bool filemanager::directory_copy(string dname, string newname) {
    if (!directory_exists(dname)) return false;
    dname = filename_remove_slash(dname, true);
    newname = filename_remove_slash(newname, true);
    retained_string = "";
    retained_length = 0;
    szsource = directory_size(dname);
    return directory_copy_retained(dname, newname);
  }

  vector<string> filemanager::directory_contents(string dname, string pattern, bool incdirs) {
    std::error_code ec;
    vector<string> itemvec, result;
    if (pattern.empty()) pattern = "*.*";
    if (!directory_exists(dname)) return result;
    dname = filename_remove_slash(dname, true);
    const fs::path path = fs::u8path(dname);
    if (directory_exists(dname)) {
      fs::directory_iterator end_itr;
      for (fs::directory_iterator dir_ite(path, ec); dir_ite != end_itr; dir_ite++) {
        if (ec.value() != 0) { break; }
        fs::path file_path = fs::u8path(filename_canonical(dir_ite->path().u8string()));
        if (!fs::is_directory(dir_ite->status(ec)) && ec.value() == 0) {
          itemvec.push_back(file_path.u8string());
        } else if (ec.value() == 0 && incdirs == true) {
          itemvec.push_back(filename_add_slash(file_path.u8string()));
        }
      }
    }
    pattern = string_replace_all(pattern, " ", "");
    pattern = string_replace_all(pattern, "*", "");
    vector<string> extvec = string_split(pattern, ";");
    std::set<string> filtered_items;
    for (string item : itemvec) {
      for (string ext : extvec) {
        if (ext == "." || ext == filename_ext(item) || directory_exists(item)) {
          filtered_items.insert(item);
          break;
        }
      }
    }
    if (filtered_items.empty()) return result;
    for (string filtered_name : filtered_items) {
      result.push_back(filtered_name);
    }
    return result;
  }

} // namespace tkg
