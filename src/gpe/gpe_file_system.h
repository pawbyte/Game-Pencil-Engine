/*
gpe_file_system.h
This file is part of:GAME PENCIL ENGINE
https://www.pawbyte.com/gamepencilengine
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 Game Pencil Engine contributors ( Contributors Page )

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-Game Pencil Engine <https://www.pawbyte.com/gamepencilengine>


*/

#ifndef gpe_file_system_h
#define gpe_file_system_h

#include "gpe_common_includes.h"
#include "gpe_constants.h"
#include "gpe_globals.h"
#include "gpe_settings.h"
#include "gpe_texture_base.h"
#include "../other_libs/stg_ex.h"

namespace gpe
{
    class GPE_FileFilter
    {
        public:
            std::string desc;
            std::string exts;
            GPE_FileFilter(std::string fDesc = "", std::string fExts = "");
            ~GPE_FileFilter();
    };

    std::string parse_file_types(std::string allowedFileTypes, std::vector <std::string> &fileTypeVector);
    bool file_passes_filter(std::string fileTypeIn, std::vector <std::string> &fileTypeVector);

    class GPE_File
    {
        private:
            std::string fileName;
            std::string fileType;
            bool isDirectory;
        public:
            std::string fileLocationDirectory;
            texture_base * fileThumbnail;
            int fileSizeInfo;
            std::string dataCreatedInfo;
            std::string dataModifiedInfo;
            GPE_File(std::string fName,bool fileIsDirectory);
            ~GPE_File();
            bool is_directory();
            std::string get_name();
            std::string get_type();
    };

    class GPE_FileDirectory
    {
        private:
            std::vector< GPE_File * >myFiles;
            int fdPosition;
            int fileCount;
            int subdirectoryCount;
        public:
            GPE_FileDirectory();
            ~GPE_FileDirectory();
            bool open_directory(std::string directoryIn);
            bool open_directory_sorted(std::string directoryIn);
            void close_directory();
            void filter_directory(bool onlyFolders, std::vector <std::string> &fTypesToUse);
            GPE_File * get_file(int position);
            GPE_File * get_next_file();
            bool has_next_file();
            int get_count();
            int get_subdirectory_count();
            int get_file_count();
            bool set_position(int position);
    };

    const float BSIZE_KB = 1024; //if this is wrong, change it to 1000 in your own time. :-)
    const float BSIZE_MB = BSIZE_KB*1024;
    const float BSIZE_GB = BSIZE_MB*1024;
    const float BSIZE_TB = BSIZE_GB*1024;
    const float BSIZE_PB = BSIZE_TB*1024;

    void external_open_program(std::string programLocation, std::string programInfo="", bool openProgrmaInfoOnFail = false);
    void external_open_url(std::string urlIn);

    int getFileSize(const std::string &fileName);
    std::string getFileSizeString(const std::string &fileName);

    int clean_folder(std::string folderName);
    int copy_folder(std::string folderName, std::string targetFolder, bool copySubFolders = false, bool overwriteExistingFiles = false );

    std::string  get_user_settings_folder();
    bool seek_settings_folder();
    std::string get_user_temp_folder();
    std::string get_user_screenshot_folder();
    bool delete_file( std::string fName);

}

#endif //gpe_file_system_h
