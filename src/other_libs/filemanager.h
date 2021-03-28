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

#include <string>
#include <vector>
#include <cstddef>

namespace tkg {

  class filemanager {

    private:

      std::string retained_string;
      std::size_t retained_length;
      std::uintmax_t szsource = 0;

      bool directory_copy_retained(std::string dname, std::string newname);

      std::string string_replace_all(std::string str, std::string sstr, std::string nstr);
      std::vector<std::string> string_split(std::string str, std::string delim);

    public:

      std::wstring widen(std::string str);
      std::string narrow(std::wstring wstr);

      std::string get_working_directory();
      bool set_working_directory(std::string dname);
      std::string get_executable_path();
      std::string get_temp_directory();

      std::string environment_get_variable(std::string name);
      bool environment_set_variable(std::string name, std::string value);
      std::string environment_expand_variables(std::string str);

      std::string filename_canonical(std::string fname);
      std::string filename_weakly_canonical(std::string fname);
      std::string filename_remove_slash(std::string dname, bool canonical = false);
      std::string filename_add_slash(std::string dname, bool canonical = false);
      std::string filename_path(std::string fname);
      std::string filename_name(std::string fname);
      std::string filename_ext(std::string fname);

      bool file_exists(std::string fname);
      bool file_delete(std::string fname);
      bool file_rename(std::string oldname, std::string newname);
      bool file_copy(std::string fname, std::string newname);
      std::uintmax_t file_size(std::string fname);

      bool directory_exists(std::string dname);
      bool directory_create(std::string dname);
      bool directory_destroy(std::string dname);
      bool directory_rename(std::string oldname, std::string newname);
      bool directory_copy(std::string dname, std::string newname);
      std::uintmax_t directory_size(std::string dname);
      std::vector<std::string> directory_contents(std::string dname, std::string pattern, bool incdirs);

  }; // class filemanager

} // namespace tkg
