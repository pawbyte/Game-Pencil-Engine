/*
gpe_file_system.cpp
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

#include <cstring>

#include "gpe_file_system.h"
#include "gpe_globals.h"

namespace gpe
{
    std::string parse_file_types(std::string allowedFileTypes, std::vector <std::string> &fileTypeVector)
    {
        fileTypeVector.clear();
        std::string returnFileFilterString = "All Files (*.*)|*.*";
        std::vector<GPE_FileFilter *> filters;
        GPE_FileFilter * tempNewFilter = NULL;
        if( (int)allowedFileTypes.size() > 0)
        {
            if( allowedFileTypes=="All Files (*.*)" ||  allowedFileTypes=="All Files" || allowedFileTypes=="None" ||  allowedFileTypes=="")
            {
                fileTypeVector.clear();
            }
            else if( allowedFileTypes=="Image Only" || allowedFileTypes=="Image" || allowedFileTypes=="Images" || allowedFileTypes=="Photos")
            {
                tempNewFilter = new GPE_FileFilter("Image Files (*.bmp *.png *.jpg)", "*.bmp;*.png;*.jpg");
                filters.push_back(tempNewFilter );
                fileTypeVector.push_back("bmp");
                fileTypeVector.push_back("BMP");
                fileTypeVector.push_back("gif");
                fileTypeVector.push_back("GIF");
                fileTypeVector.push_back("jpg");
                fileTypeVector.push_back("JPG");
                fileTypeVector.push_back("jpeg");
                fileTypeVector.push_back("JPEG");
                fileTypeVector.push_back("png");
                fileTypeVector.push_back("PNG");
                returnFileFilterString = "Image Files (*.bmp *.png *.jpg)|*.bmp;*.BMP;*.gif;*.GIF;*.jpg;*.JPG;*.jpeg;*.JPEG;*.png;*.PNG|";
            }
            else if( allowedFileTypes=="Audio Only" || allowedFileTypes=="Audio")
            {
                tempNewFilter = new GPE_FileFilter("Audio (*.aac *.mp3 *.ogg *.wav)", "*.aac;*.mp3;*.ogg;*.wav");
                filters.push_back(tempNewFilter );
                fileTypeVector.push_back("aac");
                fileTypeVector.push_back("AAC");
                fileTypeVector.push_back("mp3");
                fileTypeVector.push_back("MP3");
                fileTypeVector.push_back("ogg");
                fileTypeVector.push_back("OGG");
                fileTypeVector.push_back("wav");
                fileTypeVector.push_back("WAV");
                returnFileFilterString = "Audio Files (*.aac *.mp3 *.ogg *.wav)|*.aac;*.AAC;*.mp3;*.MP3;*.ogg;*.OGG;*.wav;*.WAV|";
            }
            else if( allowedFileTypes=="Video Only" || allowedFileTypes=="Video" || allowedFileTypes=="Videos")
            {
                tempNewFilter = new GPE_FileFilter("Video (*.mp4 *.ogg *.webm)", "*.mp4;*.ogg;*.webm");
                filters.push_back(tempNewFilter );
                fileTypeVector.push_back("mp4");
                fileTypeVector.push_back("MP4");
                fileTypeVector.push_back("ogg");
                fileTypeVector.push_back("OGG");
                fileTypeVector.push_back("webm");
                fileTypeVector.push_back("WEBM");
                returnFileFilterString = "Video Files (*.mp4 *.ogg *.webm)|*.mp4;*.MP4;*.ogg;*.OGG;*.webm;*.WEBM|";
            }
            else if( allowedFileTypes=="Fonts Only" || allowedFileTypes=="Font" || allowedFileTypes=="Fonts")
            {
                tempNewFilter = new GPE_FileFilter("Fonts (*.eot *.svg *.ttf)", "*.eot;*.svg;*.ttf");
                filters.push_back(tempNewFilter );
                fileTypeVector.push_back("eot");
                fileTypeVector.push_back("EOT");
                fileTypeVector.push_back("svg");
                fileTypeVector.push_back("SVG");
                fileTypeVector.push_back("ttf");
                fileTypeVector.push_back("TTF");
                returnFileFilterString = "Font Files (*.eot *.svg *.ttf)|eot;EOT;svg;SVG;ttf;TTF|";
            }
            else if( allowedFileTypes=="Game Pencil Projects" || allowedFileTypes=="GPE Project")
            {
                tempNewFilter = new GPE_FileFilter("Game Pencil Projects (*.gppf)", "*.gppf");
                filters.push_back(tempNewFilter );
                fileTypeVector.push_back("gppf");
                fileTypeVector.push_back("GPPF");
                returnFileFilterString = "Game Pencil Projects (*.gppf)|*.gppf;*.GPPF|";
            }
            else if( allowedFileTypes.size() > 2)
            {
                allowedFileTypes = stg_ex::string_replace_all(allowedFileTypes," ","");
                int asterikPosition = allowedFileTypes.find("*");
                int maxIterations = 10;
                int otherTypesIterations = 0;
                if( asterikPosition!=(int)std::string::npos)
                {
                    while( asterikPosition!=(int)std::string::npos && otherTypesIterations < maxIterations )
                    {
                        fileTypeVector.push_back( stg_ex::split_first_string(allowedFileTypes,"*") );
                        otherTypesIterations++;
                    }
                }
                else
                {
                    fileTypeVector.push_back(allowedFileTypes);
                }
            }
        }
        tempNewFilter = new GPE_FileFilter("All Files (*.*)", "*.*");
        filters.push_back(tempNewFilter );
        for( int iDelete = (int)filters.size()-1; iDelete>=0; iDelete--)
        {
            tempNewFilter = filters.at(iDelete);
            if( tempNewFilter!=NULL)
            {
                delete tempNewFilter;
                tempNewFilter = NULL;
            }
        }
        filters.clear();
        returnFileFilterString += "All Files (*.*)|*.*";
        return returnFileFilterString;
    }

    bool file_passes_filter(std::string fileTypeIn, std::vector <std::string> &fileTypeVector)
    {
        if( (int)fileTypeVector.size() > 0 )
        {
            if( (int)fileTypeIn.size() > 0)
            {
                std::string tempStr = "";
                for( int i = (int)fileTypeVector.size()-1; i >=0; i--)
                {
                    tempStr = fileTypeVector[i];
                    if(fileTypeIn==tempStr)
                    {
                        return true;
                    }
                }
            }
        }
        else
        {
            return true;
        }
        return false;
    }

    GPE_FileFilter::GPE_FileFilter(std::string fDesc, std::string fExts )
    {
        desc = fDesc;
        exts = fExts;
    }

    GPE_FileFilter::~GPE_FileFilter()
    {

    }

    GPE_File::GPE_File(std::string fName,bool fileIsDirectory)
    {
        isDirectory = fileIsDirectory;
        if( (int)fName.size() > 0)
        {
            fileName = fName;
            if( isDirectory)
            {
                fileType = "";
            }
            else
            {
                fileType = stg_ex::get_file_ext(fName);
            }
        }
        else
        {
            fileName = "";
            fileType = "";
        }
        fileThumbnail = NULL;
        fileSizeInfo = 0;
        dataCreatedInfo = "";
        dataModifiedInfo = "";
    }

    GPE_File::~GPE_File()
    {
        if( fileThumbnail!=NULL)
        {
            delete fileThumbnail;
            fileThumbnail = NULL;
        }
    }

    bool GPE_File::is_directory()
    {
        return isDirectory;
    }

    std::string GPE_File::get_name()
    {
        return fileName;
    }

    std::string GPE_File::get_type()
    {
        return fileType;
    }

    GPE_FileDirectory::GPE_FileDirectory()
    {
        fdPosition= 0;
        fileCount= 0;
        subdirectoryCount= 0;
    }

    GPE_FileDirectory::~GPE_FileDirectory()
    {
        close_directory();
    }


    bool GPE_FileDirectory::open_directory(std::string directoryIn)
    {
        return open_directory_sorted(directoryIn);
    }

    void GPE_FileDirectory::close_directory()
    {
        GPE_File * tFile = NULL;
        for( int i = (int)myFiles.size()-1; i>=0; i--)
        {
            tFile = myFiles[i];
            if( tFile!=NULL)
            {
                delete tFile;
                tFile = NULL;
            }
        }
        myFiles.clear();
        fdPosition= 0;
        fileCount= 0;
        subdirectoryCount= 0;
    }

    void GPE_FileDirectory::filter_directory(bool onlyFolders, std::vector <std::string> &fTypesToUse)
    {
        GPE_File * tempFile = NULL;
        fdPosition = 0;
        fileCount = 0;
        subdirectoryCount = 0;
        for( int i = (int)myFiles.size() - 1; i >=0; i--)
        {
            tempFile = myFiles[i];
            if( tempFile!=NULL)
            {
                if( onlyFolders)
                {
                    if( tempFile->is_directory() )
                    {
                        subdirectoryCount++;
                    }
                }
                else if( tempFile->is_directory() )
                {
                    subdirectoryCount++;
                }
                else if( file_passes_filter (tempFile->get_type(), fTypesToUse) )
                {
                    fileCount++;
                }
                else
                {
                    delete tempFile;
                    tempFile = NULL;
                    myFiles.erase(myFiles.begin()+i);
                }
            }
            else
            {
                myFiles.erase(myFiles.begin()+i);
            }
        }
    }

    bool GPE_FileDirectory::open_directory_sorted(std::string directoryIn)
    {
        if( sff_ex::path_exists(directoryIn) )
        {
            close_directory();
            std::string foundErrorMessage;
            DIR *dir;
            struct dirent *ent;
            dir = opendir( directoryIn.c_str() );
            if( dir==NULL)
            {
                foundErrorMessage = strerror(errno);

                error_log->report("Unable to open directory["+directoryIn+"]");
                error_log->report("Error: "+foundErrorMessage);

                //tinydir_close(&dir);
                return false;
            }
            else
            {
                GPE_File * newFile = NULL;
                GPE_File * tFile = NULL;
                int i = 0;
                int j = 0;
                bool foundNewPosition = false;
                bool newFileIsDirectory = false;
                std::string new_file_name = "";
                while ( (ent = readdir(dir)) != NULL )
                {
                    std::string entry( ent->d_name );
                    //std::string lcEntry( std::strToLower(entry) );
                    /*
                    if( tinydir_readfile(&dir, &file)==-1)
                    {
                        foundErrorMessage = strerror(errno) ;
                        error_log->report("Unable to open directory's file #"+ stg_ex::int_to_string(i)+"]");
                        error_log->report("Error: "+foundErrorMessage);
                        tinydir_close(&dir);
                        return false;
                    }
                    else*/
                    {
                        new_file_name = ent->d_name;
                        if(new_file_name!=".." && new_file_name!=".")
                        {
                            newFileIsDirectory = sff_ex::path_exists(directoryIn+"/"+ent->d_name);
                            //if( ( main_editor_settings!=NULL && main_editor_settings->showHiddenFilesInBrowser->is_clicked() ) || system_found_os!=GPE_IDE_LINUX || (system_found_os==GPE_IDE_LINUX && new_file_name.find_first_of(".",0)!=0) )
                            {
                                newFile = new GPE_File(new_file_name, newFileIsDirectory);
                                if( newFile!=NULL)
                                {
                                    newFile->fileLocationDirectory = directoryIn;
                                    foundNewPosition = false;

                                    for( j = (int)myFiles.size()-1; j >=0 && foundNewPosition==false; j--)
                                    {
                                        tFile = myFiles[j];
                                        if( tFile!=NULL)
                                        {
                                            if( tFile->get_name() >= new_file_name )
                                            {
                                                myFiles.insert(myFiles.begin()+j,newFile);
                                                foundNewPosition = true;
                                            }
                                        }
                                    }
                                    if( !foundNewPosition)
                                    {
                                        myFiles.push_back(newFile);
                                    }
                                    if( newFileIsDirectory )
                                    {
                                        subdirectoryCount++;
                                    }
                                    else
                                    {
                                        fileCount++;
                                    }
                                }
                            }
                        }
                        //tinydir_next(&dir);
                    }
                    i++;
                }
                //tinydir_close(&dir);

            }

            return true;
        }
        return false;
    }

    GPE_File * GPE_FileDirectory::get_file(int position)
    {
        if( position >=0 && position < (int)myFiles.size() )
        {
            return myFiles.at(position);
        }
        return NULL;
    }

    int GPE_FileDirectory::get_count()
    {
        return (int)myFiles.size();
    }

    int GPE_FileDirectory::get_subdirectory_count()
    {
        return subdirectoryCount;
    }

    int GPE_FileDirectory::get_file_count()
    {
        return fileCount;
    }

    GPE_File * GPE_FileDirectory::get_next_file()
    {
        if( has_next_file() )
        {
            fdPosition++;
            return get_file(fdPosition);
        }
        return NULL;
    }

    bool GPE_FileDirectory::has_next_file()
    {
        if( fdPosition>=0 && fdPosition < (int)myFiles.size()-1 )
        {
            return true;
        }
        return false;
    }

    bool GPE_FileDirectory::set_position(int position)
    {
        if( position >=0 && position < (int)myFiles.size() )
        {
            fdPosition = position;
        }
        return false;
    }



    void external_open_program(std::string programLocation, std::string programInfo, bool openProgrmaInfoOnFail )
    {
        error_log->report("Attempting to open ["+programLocation+"] with ["+programInfo+"] data...");
        if( sff_ex::file_exists(programLocation) || sff_ex::path_exists(programLocation) )
        {
    #ifdef _WIN32
            // win implementation
            //programLocation = "start "+urlIn;
            programInfo = stg_ex::string_replace_all(programInfo,"/","\\");
            ShellExecute(NULL,NULL,programLocation.c_str(),programInfo.c_str(),NULL,SW_SHOW );
    #elif __APPLE__
            // apple implementation
            programLocation = "open "+programLocation;

            system(programLocation.c_str() );
    #else
            // Linux and others implementation
            programLocation = "xdg-open \""+programLocation+"\"";
            system(programLocation.c_str() );
    #endif
        }
        else if( openProgrmaInfoOnFail )
        {
            external_open_url(programInfo);
        }
    }

    void external_open_url(std::string urlIn)
    {
        //urlIn+="&";
        /**/
    #ifdef _WIN32
        // win implementation
        ShellExecute(NULL, "open", urlIn.c_str(), NULL, NULL, SW_SHOWNORMAL);
    #elif __APPLE__
        // apple implementation
        urlIn = "open "+urlIn;
        system(urlIn.c_str() );
    #else
        // Linux and others implementation
        urlIn = "xdg-open \""+urlIn+"\"";
        system(urlIn.c_str() );
    #endif
    }

    int getFileSize(const std::string &fileName)
    {
        std::ifstream file(fileName.c_str(), std::ifstream::in | std::ifstream::binary);

        if(!file.is_open())
        {
            return -1;
        }

        file.seekg(0, std::ios::end);
        int fileSize = file.tellg();
        file.close();

        return fileSize;
    }

    std::string getFileSizeString(const std::string &fileName)
    {
        float fSizeInBytes = getFileSize( fileName );
        if( fSizeInBytes <  0 )
        {
            return "Error: File Size not found...";
        }
        if( fSizeInBytes >=BSIZE_TB )
        {
            return stg_ex::float_to_string( fSizeInBytes/BSIZE_TB )+"GB";
        }
        else if( fSizeInBytes >=BSIZE_GB )
        {
            return stg_ex::float_to_string( fSizeInBytes/BSIZE_GB )+"GB";
        }
        else if( fSizeInBytes >=BSIZE_MB )
        {
            return stg_ex::float_to_string( fSizeInBytes/BSIZE_MB )+"MB";
        }
        else if( fSizeInBytes >=BSIZE_KB )
        {
            return stg_ex::float_to_string( fSizeInBytes/BSIZE_KB )+"KB";
        }
        return stg_ex::float_to_string( fSizeInBytes )+" bytes";
    }

    int clean_folder(std::string folderName)
    {
        GPE_FileDirectory * dir = new GPE_FileDirectory();
        GPE_File * file = NULL;
        int iFile = 0;
        int iDirectory = 0;

        std::string fileToClick = "";
        std::vector< std::string > foldersToDelete;
        foldersToDelete.push_back(folderName);
        std::string currentFolderToClear = folderName;
        int filesDeletedCount = 0;
        if( dir!=NULL)
        {
            while( (int)foldersToDelete.size() > 0 )
            {
                currentFolderToClear = foldersToDelete[0];
                dir->open_directory(currentFolderToClear);
                for (iFile = (int)dir->get_count()-1; iFile>=0; iFile--)
                {
                    file = dir->get_file(iFile);
                    if( file!=NULL)
                    {
                        fileToClick = file->get_name();
                        if( fileToClick!="." && fileToClick!="..")
                        {
                            fileToClick = currentFolderToClear+"/"+fileToClick;
                            if( file->is_directory() )
                            {
                                foldersToDelete.push_back(fileToClick );
                            }
                            else
                            {
                                remove(fileToClick.c_str() );
                                filesDeletedCount++;
                            }
                        }
                    }
                }
                for( iDirectory = (int)foldersToDelete.size()-1; iDirectory >=0; iDirectory--)
                {
                    if( currentFolderToClear==foldersToDelete.at(iDirectory) )
                    {
                        foldersToDelete.erase( foldersToDelete.begin()+iDirectory);
                    }
                }
            }
            delete dir;
            dir = NULL;
            return filesDeletedCount;
        }
        return 0;
    }

    int copy_folder(std::string folderName, std::string targetFolder, bool copySubFolders, bool overwriteExistingFiles )
    {
        if( sff_ex::path_exists( folderName ) == false )
        {
            return -1;
        }
        if( sff_ex::path_exists(targetFolder) == false )
        {
            return -2;
        }

        int filesCopiedCount = 0;
        GPE_FileDirectory * dir = new GPE_FileDirectory();
        GPE_File * file = NULL;
        std::string currentFileName = "";
        int iFile = 0;
        int iDirectory = 0;
        int addedFolderFileCount = 0;
        if( dir!=NULL )
        {
            dir->open_directory(folderName);
            for (iFile = 0; iFile < (int)dir->get_count(); iFile++)
            {
                file = dir->get_file(iFile);
                if( file!=NULL)
                {
                    currentFileName = file->get_name();
                    if( currentFileName!="." && currentFileName!="..")
                    {
                        //displayMessageString = "Copying "+currentFileName;
                        currentFileName = folderName+"/"+currentFileName;
                        if( file->is_directory() )
                        {
                            if( copySubFolders)
                            {
                                sff_ex::create_directory(targetFolder+"/"+file->get_name() );
                                addedFolderFileCount= copy_folder( currentFileName, targetFolder+"/"+ file->get_name(), true, overwriteExistingFiles );

                                if( addedFolderFileCount > 0 )
                                {
                                    filesCopiedCount += addedFolderFileCount;
                                }
                            }
                        }
                        else
                        {
                            if( sff_ex::file_copy(currentFileName,targetFolder+"/"+ file->get_name(), overwriteExistingFiles ) )
                            {
                                filesCopiedCount++;
                            }
                        }
                    }
                }
            }
            delete dir;
            dir = NULL;
            return filesCopiedCount;
        }
        return 0;
    }


    bool seek_settings_folder()
    {
        //Sets the folder used in all  get_user_settings_folder() calls based on the 2 parameters above
        settings_app_folder = app_directory_name;
        if( (int)settings->programPublisher.size() ==0 )
        {
            return false;
        }
        //if a publisher is said to be to this program
        //Here is where he fun happens...
        bool useProgramFolder = false;
        bool homeDirFound  = true;

        //Going thru all of the possible getenv paramters that i know of...
        char* homeDir = getenv("%UserProfile%");
        std::string foundPath = "";
        //Attempt 1...
        if( homeDir!=NULL)
        {
            //Attempt 1...
            foundPath = homeDir;
        }
        else
        {
            //Attempt 2...
            homeDir = getenv("home");
            if( homeDir!=NULL )
            {
                foundPath = homeDir;
            }
            else
            {
                //Attempt 3...
                homeDir = getenv("HOME");
                if( homeDir!=NULL)
                {
                    foundPath = homeDir;
                }
                else
                {
                    //final shot...
                    homeDir = getenv("homepath");
                    if( homeDir!=NULL )
                    {
                        foundPath = homeDir;
                    }
                    else
                    {
                        //All attempts failed defaults to program folder...
                        return false;
                    }
                }
            }
        }

        if( homeDirFound && sff_ex::path_exists(foundPath) )
        {
            std::string appDataPath = foundPath;
            if( system_found_os== system_os_windows)
            {
                //First Layer of Windows unlocked :-)
                if( sff_ex::create_directory(appDataPath+"/AppData")!=-1)
                {
                    foundPath = appDataPath = appDataPath+"/AppData";

                    //See if we can get into the Roaming folder now
                    if( sff_ex::create_directory(appDataPath+"/Roaming")!=-1)
                    {
                        foundPath = appDataPath = appDataPath+"/Roaming";
                    }
                    else if( sff_ex::create_directory(appDataPath+"/Local")!=-1 )
                    {
                        //If failed, lets see if we can get into the Local folder now
                        foundPath = appDataPath = appDataPath+"/Local";
                    }
                    else if( sff_ex::create_directory(appDataPath+"/LocalLow")!=-1 )
                    {
                        foundPath = appDataPath = appDataPath+"/LocalLow";
                    }
                    else
                    {
                        //Windows failed to get into settings...
                        return false;
                    }
                }
                else
                {
                    useProgramFolder = true;
                    appDataPath = foundPath = "";
                }
            }
            else if( system_found_os== system_os_mac)
            {
                appDataPath = foundPath+"/~/Library/Preferences";
                if( sff_ex::create_directory(appDataPath)==-1)
                {
                    useProgramFolder = true;
                    appDataPath = foundPath = "";
                }
                else
                {
                    return false;
                }
            }

            if( useProgramFolder )
            {
                //Something failed above so now we just use the program folder...
                return false;
            }

            //If we have some sort of file path...
            if( system_found_os== system_os_linux )
            {
                appDataPath = appDataPath+"/."+settings->programPublisher;
            }
            else
            {
                //Windows, Mac, etc afaik...
                appDataPath = appDataPath+"/"+settings->programPublisher;
            }

            //If we are able to get into the publisher folder
            if( sff_ex::create_directory(appDataPath)!=-1)
            {
                foundPath = appDataPath;
                settings_app_folder = foundPath;
                if( (int)settings->programTitle.size() > 0 )
                {
                    //If we are able to get into the titled program's folder
                    if( sff_ex::create_directory( foundPath+"/"+settings->programTitle )!=-1 )
                    {
                        foundPath = foundPath+"/"+settings->programTitle;
                        settings_app_folder = foundPath;
                    }
                    settings_app_folder = settings_app_folder+"/";
                    //If this fails we will just default to the publisher and call it a day...
                }
                return true;
            }
            else
            {
                return false;
            }
        }


        //save the settings and local directory to the application directory
        if( useProgramFolder )
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    std::string  get_user_settings_folder()
    {
        return settings_app_folder;
    }

    std::string get_user_temp_folder()
    {
        std::string foundPath =  get_user_settings_folder();
        std::string tempFolderString = foundPath;
        foundPath = foundPath+"/temp_files";
        if( sff_ex::create_directory(foundPath)!=-1)
        {
            tempFolderString = foundPath+"/";
        }
        return tempFolderString;
    }

    std::string get_user_screenshot_folder()
    {
        std::string foundPath =  get_user_settings_folder();
        std::string tempScreenshotString = foundPath;
        foundPath = foundPath+"/screenshots";
        if( sff_ex::create_directory(foundPath)!=-1)
        {
            tempScreenshotString = foundPath+"/";
        }
        return tempScreenshotString;
    }

    bool delete_file( std::string fName)
    {
        //Function not yet created
        return false;
    }
}
