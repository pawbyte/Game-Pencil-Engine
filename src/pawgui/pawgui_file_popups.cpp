/*
pawgui_file_popups.cpp
This file is part of:
PawByte Ambitious Working GUI(PAWGUI)
https://www.pawbyte.com/pawgui
Copyright (c) 2014-2020 Nathan Hurde, Chase Lee.

Copyright (c) 2014-2020 PawByte LLC.
Copyright (c) 2014-2020 PawByte Ambitious Working GUI(PAWGUI) contributors ( Contributors Page )

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

-PawByte Ambitious Working GUI(PAWGUI) <https://www.pawbyte.com/pawgui>


*/

#if defined(_WIN32) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && !defined(__ANDROID__)) || defined(__FreeBSD__)
    #include <cstring>
    #include <SDL2/SDL_syswm.h>
    #include "../other_libs/dlgmodule.h"
    #include "../sdl_libs/gpe_window_controller_sdl.h"
#endif

#include "pawgui_file_popups.h"

namespace pawgui
{
    std::string  get_filename_open_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory)
    {
        return get_filename_plain_from_popup( prompt, allowedFileTypes,previousDirectory,false);
    }

    std::string get_filename_save_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory )
    {
        return get_filename_plain_from_popup( prompt, allowedFileTypes,previousDirectory,true);
    }

    std::string  get_directory_name_from_popup( const std::string & prompt, std::string &previousDirectory, bool isSaving )
    {
        return get_filename_plain_from_popup(prompt,"",previousDirectory,isSaving, true);
    }

    std::string get_filename_plain_from_popup( const std::string & prompt, std::string allowedFileTypes, std::string &previousDirectory,bool isSaving, bool isDirectorySearch )
    {
        //resource_dragged = NULL;
        gpe::GPE_File * currentFile = NULL;
        gpe::GPE_FileDirectory * currentDirectory = new gpe::GPE_FileDirectory();

        gpe::cursor_main_controller->cursor_change("arrow");
        gpe::error_log->report("[Opening file] Plainfile Function... using <"+previousDirectory+"> as previous.");
        main_overlay_system->take_frozen_screenshot();
        std::string returnFile = "";
        std::string currentDirectoryInView = "";
        std::string nextDirectoryToView = "";
        std::string soughtReturnFilename = "";
        std::string userHomePath = "";
        std::string  osFileFilterString = "All types(*.*)|*.*";
        int filesAndDirectoryPassedFilterCount = 16;
        int maxCharsToView = 16;
        bool fileMatchesFilter = false;
        bool mouseIsInBrowserBox = false;
        float imagePreviewScaleSize = 1;
        char* homeDir;
        std::string iDirLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        homeDir = getenv("%UserProfile%");
        if( homeDir!=NULL)
        {
            userHomePath = currentDirectoryInView = homeDir;
        }
        else
        {
            homeDir = getenv("home");
            if( homeDir!=NULL)
            {
                userHomePath = currentDirectoryInView = homeDir;
            }
            else
            {
                homeDir = getenv("HOME");
                if( homeDir!=NULL)
                {
                    userHomePath = currentDirectoryInView = homeDir;
                }
                else
                {
                    homeDir = getenv("homepath");
                    if( homeDir!=NULL)
                    {
                        userHomePath = currentDirectoryInView = homeDir;
                    }
                    else
                    {
                        userHomePath = currentDirectoryInView = gpe::app_directory_name;
                    }
                }
            }
        }

        for( int iLetter = 0; iLetter < (int)iDirLetters.size(); iLetter++)
        {
            if( sff_ex::path_exists(iDirLetters.substr(iLetter,1)+":"+userHomePath) )
            {
                userHomePath = iDirLetters.substr(iLetter,1)+":"+userHomePath;
                break;
            }
            else
            {
                gpe::error_log->report(iDirLetters.substr(iLetter,1)+":"+userHomePath+" is not a directory.");
            }
        }
        userHomePath = stg_ex::string_replace_all(userHomePath,"\\","/");
        currentDirectoryInView = userHomePath;
        if( sff_ex::path_exists(previousDirectory) )
        {
            currentDirectoryInView = previousDirectory.c_str();
        }
        if( (int)currentDirectoryInView.size()>0 && currentDirectory!=NULL)
        {
            main_overlay_system->take_frozen_screenshot( );
            int fontSizeH = 12;
            int fontSizeW = 12;
            font_textinput->get_metrics("A",&fontSizeW,&fontSizeH);
            if( fontSizeW<=0)
            {
                fontSizeW = 12;
            }
            gpe::texture_base * previewedImageTexture = NULL;
            widget_panel_list * shortCutGList = new widget_panel_list();

            std::string shorterCurrentDirectoryInView = currentDirectoryInView;

            std::vector<std::string > fileFilterTypes;
            if( currentDirectoryInView.size() > 32)
            {
                shorterCurrentDirectoryInView = currentDirectoryInView.substr(0,32);
            }
            gpe::input->reset_all_input();

            int iFile = 0;
            int filesProcessedCount = 0;
            int iDirectory = 0;
            bool enterMyComputerMode = false;
            bool exitOperation = false;
            if( currentDirectory->open_directory_sorted(currentDirectoryInView )==-1)
            {
                exitOperation = true;
            }
            int TEXTBOX_FONT_SIZE_WIDTH = 12;
            int TEXTBOX_FONT_SIZE_HEIGHT = 12;

            int DRIVER_FONT_SIZE_WIDTH = 24;
            int DRIVER_FONT_SIZE_HEIGHT = 24;
            if( font_textinput!=NULL)
            {
                font_textinput->get_metrics("A",&TEXTBOX_FONT_SIZE_WIDTH,&TEXTBOX_FONT_SIZE_HEIGHT);
            }
            if( font_textinput!=NULL)
            {
                font_textinput->get_metrics("A",&DRIVER_FONT_SIZE_WIDTH,&DRIVER_FONT_SIZE_HEIGHT);
            }
            std::string returnVal = "";
            std::string fileToClick = "";
            std::string currentFileExt = "";
            std::string fileRenderName[3];
            fileRenderName[0] = "";
            fileRenderName[1] = "";
            fileRenderName[2] = "";
            std::vector <std::string> mountedDriversList;
            widget_dropdown_menu * fileTypeDropDown = new widget_dropdown_menu( "All Files (*.*)",false);
            gpe::parse_file_types(allowedFileTypes,fileFilterTypes);
            std::string tempFileType = "";
            for( int fDT = 0; fDT < (int)fileFilterTypes.size(); fDT++)
            {
                tempFileType  = fileFilterTypes.at(fDT);
                fileTypeDropDown->add_menu_option(tempFileType,tempFileType );
            }
            fileFilterTypes.clear();

            widget_button_label * yesButton = NULL;
            if( isSaving)
            {
                yesButton= new widget_button_label( "Save","");
            }
            else
            {
                yesButton = new widget_button_label( "Open","");
            }

            gpe::animaton2d * mainFilesanimation = NULL;
            gpe::animaton2d * mainMenuanimation = NULL;


            widget_button_iconbar * fileBrowserModeBar = new widget_button_iconbar( 32,true);
            fileBrowserModeBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/table.png","Thumbnailed View",1,false);
            fileBrowserModeBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/list-alt.png","Tiled View",1,false);
            fileBrowserModeBar->add_option( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/list.png","Detailed View",1,false);
            int pastFileBrowserMode = 0;
            widget_button_push * mycomputerButton = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/archive.png","Computer","My Computer");
            widget_button_push * homeButton = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/home.png","Home","View Home Disk Drives");
            widget_button_push * desktopButton = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/desktop.png","Desktop","Opens the Desktop folder");
            widget_button_push * documentsButton = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/suitcase.png","Documents","Opens the Documents folder");
            widget_button_push * downloadsButton = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/download.png","Downloads","Opens the Downloads folder");
            widget_button_push * picturesButton = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/image.png","Pictures","Opens the Pictures folder");
            widget_button_push * musicButton = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/music.png","Music","Opens the Music folder");
            widget_button_push * videoButtons = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/film.png","Videos","Opens the Videos folder");

            widget_button_push * backButton = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/backward.png","Up Dir...","Traverses up one directory");
            widget_button_push * appDirectoryButton = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/pencil.png","GPE Folder","Opens the drive to GPE IDE");
            widget_button_push * examplesDirectoryButton = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/leanpub.png","Examples","Examples of Key and simple features");
            //widget_button_push * tutorialsDirectoryButton = new widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/institution.png","Tutorials","Tutorials on using GPE");

            widget_button_label * cancelButton = new widget_button_label( "Cancel","");
            widget_input_text * newStringBox = new widget_input_text("");
            widget_input_text * directoryViewBox = new widget_input_text("Directory");
            directoryViewBox->set_string("Home");
            widget_button_icon * refreshButton = new widget_button_icon( "resources/gfx/iconpacks/fontawesome/refresh.png","Refresh");

            widget_scrollbar_yaxis * fileDirYScroll = new widget_scrollbar_yaxis();
            int maxFilesInColumn = 0;
            int maxFilesInRows = 0;
            int maxContentInView = 0;
            int iFileXPos = 0;
            int iDirectoryXPos = 0;
            int fileselectedNumber = -1;
            int fileHoverNumber = -1;
            int fileSubImageToDraw = 0;
            int colTextWidth = 128;
            int fileRowHeight = 128;
            int colTextCharacters = colTextWidth/fontSizeW;
            if( isDirectorySearch)
            {
                newStringBox->set_label("Directory Name:");
            }
            else
            {
                newStringBox->set_label("File Name:");
            }
            newStringBox->switch_inuse(true);
            int buttonsWidth = yesButton->get_width()+cancelButton->get_width()+padding_default*2;

            gpe::shape_rect widget_box;
            widget_box.x = 0;
            widget_box.y = 0;
            widget_box.w = 512;
            widget_box.h = 416;

            gpe::shape_rect fileBrowserBox;
            fileBrowserBox.x = 160;
            fileBrowserBox.y = 64;
            fileBrowserBox.w = 512-32;
            fileBrowserBox.h = widget_box.h -384;

            gpe::shape_rect filePreviewBox;
            filePreviewBox.x = gpe::screen_width-(widget_box.x+widget_box.w);
            filePreviewBox.y = 64;
            filePreviewBox.w = gpe::screen_width-filePreviewBox.x;
            filePreviewBox.h = widget_box.h -384;

            gpe::shape_rect fileShortcutTab;
            fileShortcutTab.x = 0;
            fileShortcutTab.y = 64;
            fileShortcutTab.w = 144;
            fileShortcutTab.h = widget_box.h -384;

            gpe::shape_rect currFileRect;
            currFileRect.x = 0;
            currFileRect.y = 0;
            currFileRect.w = default_icon_width;

            int iContentCol = 0;
            int jContentRow = 0;

            gpe::renderer_main->reset_viewpoint();
            main_overlay_system->render_frozen_screenshot( );
            gpe::error_log->report("Attempting to open file selection menu...");
            //The lovely file selector loop
            nextDirectoryToView = currentDirectoryInView;
        #if defined(_WIN32) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__linux__) && !defined(__ANDROID__)) || defined(__FreeBSD__)
            if (!isDirectorySearch)
            {
                std::vector<std::string> fileFilterTypes;
                std::string osFileFilterString = gpe::parse_file_types(allowedFileTypes, fileFilterTypes);
                #if defined(_WIN32) || (defined(__linux__) && !defined(__ANDROID__)) || defined(__FreeBSD__)
                    SDL_SysWMinfo wmInfo;
                    SDL_VERSION(&wmInfo.version);
                    SDL_GetWindowWMInfo(gpe::window_controller_main_sdl->get_sdl_window(), &wmInfo);
                    #if defined(_WIN32)
                        widget_set_owner((char *)std::to_string((unsigned long long)wmInfo.info.win.window).c_str());
                    #elif (defined(__linux__) && !defined(__ANDROID__)) || defined(__FreeBSD__)
                        widget_set_owner((char *)std::to_string((unsigned long long)wmInfo.info.x11.window).c_str());
                    #endif
                #endif
                widget_set_icon((char *)"icon.png");
                if (!osFileFilterString.empty() && osFileFilterString.back() == '|')
                    osFileFilterString.pop_back();
                if (!isSaving)
                {
                    returnFile = get_open_filename_ext((char *)osFileFilterString.c_str(), (char *)"", (char *)previousDirectory.c_str(), (char *)prompt.c_str());
                }
                else
                {
                    returnFile = get_save_filename_ext((char *)osFileFilterString.c_str(), (char *)"", (char *)previousDirectory.c_str(), (char *)prompt.c_str());
                }
            }
            std::string widget_system = widget_get_system();
            if (widget_system == "Zenity" && (sff_ex::file_exists("/usr/bin/zenity") || sff_ex::file_exists("/usr/local/bin/zenity")))
            {
                exitOperation = true;
            }
            else if (widget_system == "KDialog" && (sff_ex::file_exists("/usr/bin/kdialog") || sff_ex::file_exists("/usr/local/bin/kdialog")))
            {
                exitOperation = true;
            }
        #endif

            gpe::texture_base * currentFIleTexture = NULL;
            gpe::texture_base * textRepFolder = rsm_gui->texture_add("folderOpen", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder-open.png");
            gpe::texture_base * textRepHarddrive = rsm_gui->texture_add("harddriveIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/hdd-o.png");
            gpe::texture_base * textRepAudio = rsm_gui->texture_add("audioIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-audio-o.png");
            gpe::texture_base * textRepCode = rsm_gui->texture_add("codeIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-code-o.png");
            gpe::texture_base * textRepExe = rsm_gui->texture_add("paperPlane", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/paper-plane-o.png");
            gpe::texture_base * textRepFont = rsm_gui->texture_add("fontIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/font.png");
            gpe::texture_base * textRepGPE = rsm_gui->texture_add("gpeIconSquare", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/pencil-square.png");
            gpe::texture_base * textRepImage = rsm_gui->texture_add("fileBrowserImageFileIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-image-o.png");
            gpe::texture_base * textRepOther = rsm_gui->texture_add("fileBrowserFileIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file.png");
            gpe::texture_base * textRepPDF = rsm_gui->texture_add("fileBrowserPDFIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-pdf-o.png");
            gpe::texture_base * textRepText = rsm_gui->texture_add("fileBrowserTxtIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-text-o.png");
            gpe::texture_base * textRepVideo = rsm_gui->texture_add("fileBrowserVideoIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-video-o.png");
            gpe::texture_base * textRepZip = rsm_gui->texture_add("fileBrowserZipIcon", gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/file-zip-o.png");
            gpe::color * fileIconColor = NULL;
            while(exitOperation==false)
            {
                gpe::cursor_main_controller->cursor_change("arrow");
                gpe::renderer_main->reset_viewpoint();
                if( !gpe::point_within(gpe::input->mouse_position_x,gpe::input->mouse_position_y,
                                  fileBrowserBox.x,fileBrowserBox.y,
                                  fileBrowserBox.x+fileBrowserBox.w,
                                  fileBrowserBox.y+fileBrowserBox.h) )
                {
                    fileHoverNumber = -1;
                }

                ///
                if( (int)nextDirectoryToView.size() > 0 && sff_ex::path_exists(nextDirectoryToView) )
                {
                    gpe::error_log->report("Attempting to open ["+nextDirectoryToView+"] directory...");
                    if( sff_ex::path_exists(nextDirectoryToView)==false)
                    {
                        nextDirectoryToView+="\\";
                    }
                    if( sff_ex::path_exists(nextDirectoryToView) )
                    {
                        gpe::error_log->report("Opening... ["+nextDirectoryToView+"] directory...");

                        currentDirectoryInView = nextDirectoryToView;
                        directoryViewBox->set_string(currentDirectoryInView);
                        shorterCurrentDirectoryInView = currentDirectoryInView;
                        if( currentDirectoryInView.size() > 32)
                        {
                            shorterCurrentDirectoryInView = currentDirectoryInView.substr(0,32);
                        }
                        nextDirectoryToView = "";
                        fileHoverNumber = -1;
                        fileselectedNumber = -1;
                        enterMyComputerMode= false;
                        iFile = 0;
                        iDirectory = 0;
                        iFileXPos = 0;
                        iDirectoryXPos = 0;
                        fileDirYScroll->fullRect.x = fileDirYScroll->contextRect.x = 0;
                        fileDirYScroll->fullRect.y = fileDirYScroll->contextRect.y = 0;
                        fileDirYScroll->fullRect.w = fileDirYScroll->contextRect.w = 0;
                        fileDirYScroll->fullRect.h = fileDirYScroll->contextRect.h = 16;
                        fileDirYScroll->process_self();
                        //previewedImageTexture->change_texture(NULL);

                        gpe::parse_file_types(fileTypeDropDown->get_selected_name(),fileFilterTypes);
                        //Processes files to view
                        filesAndDirectoryPassedFilterCount = 0;

                        if( currentDirectory->open_directory_sorted( currentDirectoryInView ) ==-1)
                        {
                            exitOperation = true;
                        }
                        else
                        {
                            currentDirectory->filter_directory(isDirectorySearch, fileFilterTypes );
                            filesAndDirectoryPassedFilterCount = currentDirectory->get_count();
                            /*
                            for(iFile = 0; iFile < currentDirectory->get_count(); iFile++)
                            {
                                currentFile = currentDirectory->get_file(iFile);
                                if( currentFile!=NULL)
                                {
                                    fileToClick = currentFile->get_name();
                                    if( currentFile->is_directory() )
                                    {
                                        filesAndDirectoryPassedFilterCount++;
                                    }
                                    else if( !isDirectorySearch &&  sff_ex::file_passes_filter(currentFile->get_type(),fileFilterTypes) )
                                    {
                                        filesAndDirectoryPassedFilterCount++;
                                    }
                                }
                            }
                            */
                            gpe::error_log->report("<"+currentDirectoryInView+"> has ["+ stg_ex::int_to_string(filesAndDirectoryPassedFilterCount)+"/"+ stg_ex::int_to_string(currentDirectory->get_count() )+"] amount of files/directories...");
                            fileDirYScroll->fullRect.h = fileDirYScroll->contextRect.h =  filesAndDirectoryPassedFilterCount;
                        }
                    }
                    else
                    {
                        nextDirectoryToView = "";
                    }
                }

                ///
                //Start the frame timer
                gpe::game_runtime->start_loop();

                widget_box.x = 0;
                widget_box.y = 0;
                if( gpe::screen_width > 1024)
                {
                    widget_box.w = gpe::screen_width*2/3;
                }
                else
                {
                    widget_box.w = gpe::screen_width-128;
                }

                //if( gpe::screen_height >= 600)
                {
                    widget_box.h = gpe::screen_height*4/5;
                }/*
                else
                {
                    widget_box.h = gpe::screen_height;
                }*/

                fileBrowserBox.w = widget_box.w-16-fileBrowserBox.x;
                fileBrowserBox.h = widget_box.h - 192;

                filePreviewBox.x = widget_box.x+widget_box.w;
                filePreviewBox.y = fileBrowserBox.y;
                filePreviewBox.w = gpe::screen_width-filePreviewBox.x-(padding_default*2);
                filePreviewBox.h = fileBrowserBox.h;

                fileShortcutTab.h = widget_box.h - fileShortcutTab.y;
                fileBrowserModeBar->process_self();
                if( fileBrowserModeBar->is_clicked() || pastFileBrowserMode!=fileBrowserModeBar->get_tab_pos() )
                {
                    iDirectoryXPos = iFileXPos = 0;
                    pastFileBrowserMode=fileBrowserModeBar->get_tab_pos();
                }

                if( enterMyComputerMode ==true)
                {
                    maxFilesInRows = 1;
                    colTextWidth = fileBrowserBox.w-padding_default-default_thumbnal_size;
                }
                else
                {
                    if( fileBrowserBox.w >TEXTBOX_FONT_SIZE_WIDTH*60 )
                    {
                        maxFilesInRows = fileBrowserBox.w / (TEXTBOX_FONT_SIZE_WIDTH*60);
                        colTextWidth = (fileBrowserBox.w-maxFilesInColumn*(padding_default+default_icon_width) )/maxFilesInRows;
                    }
                    else
                    {
                        maxFilesInRows = 1;
                        colTextWidth = fileBrowserBox.w-padding_default;
                    }
                }

                if( enterMyComputerMode ==true)
                {
                    maxFilesInColumn = fileBrowserBox.h / (default_thumbnal_size+padding_default);
                    maxFilesInRows = 1;
                    currFileRect.w =colTextWidth = fileBrowserBox.w-32;
                    currFileRect.h = default_thumbnal_size;
                }
                else
                {
                    maxCharsToView = (fileBrowserBox.w/TEXTBOX_FONT_SIZE_WIDTH)-20;
                    fileRowHeight = default_icon_width;
                    if( fileBrowserModeBar->get_tab_pos()==0 )
                    {
                        currFileRect.w = colTextWidth = (default_thumbnal_size+padding_default*4);
                        maxFilesInRows = fileBrowserBox.w/currFileRect.w;
                        currFileRect.h = fileRowHeight = default_thumbnal_size+TEXTBOX_FONT_SIZE_HEIGHT*3+padding_default;
                        maxFilesInColumn = std::ceil( (float)fileBrowserBox.h/(float)currFileRect.h );
                        maxContentInView = (maxFilesInColumn)*maxFilesInRows;
                    }
                    else if( fileBrowserModeBar->get_tab_pos()==1 )
                    {
                        currFileRect.w = colTextWidth = 40*TEXTBOX_FONT_SIZE_WIDTH+default_icon_width;
                        maxFilesInRows = fileBrowserBox.w/currFileRect.w;
                        currFileRect.h = default_icon_width+padding_default;
                        maxFilesInColumn = fileBrowserBox.h/currFileRect.h;
                        maxContentInView = (maxFilesInColumn)*maxFilesInRows;
                    }
                    else if( fileBrowserModeBar->get_tab_pos()==2 )
                    {
                        currFileRect.w =colTextWidth = fileBrowserBox.w-32;
                        currFileRect.h = default_icon_width+padding_default;
                        maxFilesInRows = 1;
                        maxFilesInColumn = fileBrowserBox.h/currFileRect.h;
                        maxContentInView = (maxFilesInColumn)*maxFilesInRows;
                    }
                }


                colTextCharacters = colTextWidth/fontSizeW;
                shorterCurrentDirectoryInView = currentDirectoryInView;

                if( (int)currentDirectoryInView.size() > maxCharsToView)
                {
                    shorterCurrentDirectoryInView = "/..."+currentDirectoryInView.substr(currentDirectoryInView.size()-maxCharsToView);
                }

                refreshButton->set_coords(padding_default,widget_box.y+32);
                newStringBox->set_coords( fileBrowserBox.x+padding_default,fileBrowserBox.y+fileBrowserBox.h+padding_default);
                newStringBox->set_width(fileBrowserBox.w-128 );

                fileTypeDropDown->set_coords( fileBrowserBox.x+padding_default,newStringBox->get_y2()+padding_default);
                fileTypeDropDown->set_width(fileBrowserBox.w-128 );


                directoryViewBox->set_coords( widget_box.x+padding_default+32,widget_box.y+32);
                directoryViewBox->set_width(widget_box.w - 192);

                fileBrowserModeBar->set_coords(directoryViewBox->get_x2(),directoryViewBox->get_ypos() );
                fileBrowserModeBar->set_width(128);

                backButton->set_width(128);
                mycomputerButton->set_width(128);
                homeButton->set_width(128);
                desktopButton->set_width(128);
                documentsButton->set_width(128);
                downloadsButton->set_width(128);
                picturesButton->set_width(128);
                musicButton->set_width(128);
                videoButtons->set_width(128);
                appDirectoryButton->set_width(128);
                examplesDirectoryButton->set_width(128);
                //tutorialsDirectoryButton->set_width(128);

                yesButton->set_coords(  widget_box.x+widget_box.w-buttonsWidth,widget_box.h-48);
                cancelButton->set_coords( widget_box.x+yesButton->get_x2()+padding_default,widget_box.h-48);

                if( shortCutGList!=NULL)
                {
                    shortCutGList->clear_list();
                    shortCutGList->barXMargin = padding_default;
                    shortCutGList->barYMargin = 0;
                    shortCutGList->barXPadding = 0;
                    shortCutGList->barYPadding = padding_default;

                    shortCutGList->set_coords(fileShortcutTab.x,fileShortcutTab.y);
                    shortCutGList->set_width(fileShortcutTab.w);
                    shortCutGList->set_height(fileShortcutTab.h);
                    /*
                    Commmented out temporarily in 1.13
                    //if( gpe::system_found_os== gpe::system_os_linux)
                    {
                        if( main_editor_settings!=NULL)
                        {
                            shortCutGList->add_gui_element(main_editor_settings->showHiddenFilesInBrowser,true);
                        }
                    }*/
                    shortCutGList->add_gui_element(backButton,true);
                    shortCutGList->add_gui_element(mycomputerButton,true);
                    shortCutGList->add_gui_element(homeButton,true);
                    shortCutGList->add_gui_element(desktopButton,true);
                    shortCutGList->add_gui_element(documentsButton,true);
                    shortCutGList->add_gui_element(downloadsButton,true);
                    shortCutGList->add_gui_element(picturesButton,true);
                    shortCutGList->add_gui_element(musicButton,true);
                    shortCutGList->add_gui_element(videoButtons,true);
                    shortCutGList->add_gui_element(appDirectoryButton,true);
                    shortCutGList->add_gui_element(examplesDirectoryButton,true);
                    //shortCutGList->add_gui_element(tutorialsDirectoryButton,true);
                    //shortCutGList->add_gui_element(musicButton);
                    shortCutGList->process_self();
                }

                refreshButton->process_self();
                yesButton->process_self();

                cancelButton->process_self();

                newStringBox->process_self();
                if( !isDirectorySearch)
                {
                    fileTypeDropDown->process_self();
                }
                if( fileTypeDropDown->just_activated() )
                {
                    nextDirectoryToView = currentDirectoryInView;
                }
                directoryViewBox->process_self();

                if( directoryViewBox->is_inuse() ==false)
                {
                    if( directoryViewBox->get_string() !=currentDirectoryInView )
                    {
                        if( sff_ex::path_exists(directoryViewBox->get_string() ) )
                        {
                            nextDirectoryToView  = directoryViewBox->get_string();
                        }
                    }
                }

                if( backButton->is_clicked() )
                {
                    nextDirectoryToView = stg_ex::get_path_from_file(currentDirectoryInView);
                    if( nextDirectoryToView.size()<=1 )
                    {
                        nextDirectoryToView = "";
                    }
                }

                if( newStringBox->is_inuse()==false && directoryViewBox->is_inuse()==false )
                {
                    if( gpe::input->check_kb_released(kb_backspace) )
                    {
                        nextDirectoryToView = stg_ex::get_path_from_file(currentDirectoryInView);
                        if( nextDirectoryToView.size()<=1 )
                        {
                            nextDirectoryToView = "";
                        }
                    }
                }
                if( homeButton->is_clicked() )
                {
                    nextDirectoryToView = userHomePath;
                }

                if( desktopButton->is_clicked() )
                {
                    nextDirectoryToView = userHomePath+"/Desktop";
                    if( sff_ex::path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/desktop";
                        if( sff_ex::path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/MyDesktop";
                            if( sff_ex::path_exists( nextDirectoryToView)==false)
                            {
                                nextDirectoryToView = userHomePath+"/My Desktop";
                            }
                            else
                            {
                                nextDirectoryToView = "";
                            }
                        }
                    }
                }

                if( downloadsButton->is_clicked() )
                {
                    nextDirectoryToView = userHomePath+"/Downloads";
                    if( sff_ex::path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/downloads";
                        if( sff_ex::path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/MyDownloads";
                            if( sff_ex::path_exists( nextDirectoryToView)==false)
                            {
                                nextDirectoryToView = userHomePath+"/My Downloads";
                            }
                            else
                            {
                                nextDirectoryToView = "";
                            }
                        }
                    }
                }

                if( picturesButton->is_clicked() )
                {
                    nextDirectoryToView = userHomePath+"/Pictures";
                    if( sff_ex::path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/pictures";
                        if( sff_ex::path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/MyPictures";
                            if( sff_ex::path_exists( nextDirectoryToView)==false)
                            {
                                nextDirectoryToView = userHomePath+"/My Pictures";
                            }
                            else
                            {
                                nextDirectoryToView = "";
                            }
                        }
                    }
                }

                if( musicButton->is_clicked() )
                {
                    nextDirectoryToView = userHomePath+"/Music";
                    if( sff_ex::path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/music";
                        if( sff_ex::path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/MyMusic";
                            if( sff_ex::path_exists( nextDirectoryToView)==false)
                            {
                                nextDirectoryToView = userHomePath+"/My Music";
                            }
                            else
                            {
                                nextDirectoryToView = "";
                            }
                        }
                    }
                }

                if( videoButtons->is_clicked() )
                {
                    nextDirectoryToView = userHomePath+"/Videos";
                    if( sff_ex::path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/videos";
                        if( sff_ex::path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/MyVideos";
                            if( sff_ex::path_exists( nextDirectoryToView)==false)
                            {
                                nextDirectoryToView = userHomePath+"/My Videos";
                            }
                            else
                            {
                                nextDirectoryToView = "";
                            }
                        }
                    }
                }

                if( enterMyComputerMode==false && refreshButton->is_clicked() )
                {
                    nextDirectoryToView = currentDirectoryInView;
                }

                if( mycomputerButton->is_clicked() || ( enterMyComputerMode==true && refreshButton->is_clicked()) )
                {
                    mountedDriversList.clear();
                    nextDirectoryToView = "/proc/mounts";
                    if( sff_ex::file_exists( nextDirectoryToView) && gpe::system_found_os==gpe::system_os_linux )
                    {
                        enterMyComputerMode = true;
                    }
                    else if( gpe::system_found_os == gpe::system_os_windows)
                    {
                        enterMyComputerMode = true;
                    }
                    nextDirectoryToView = "";
                    directoryViewBox->set_string("My Computer");
                }

                if( documentsButton->is_clicked() )
                {
                    nextDirectoryToView = userHomePath+"/Documents";
                    if( sff_ex::path_exists( nextDirectoryToView)==false)
                    {
                        nextDirectoryToView = userHomePath+"/documents";
                        if( sff_ex::path_exists( nextDirectoryToView)==false)
                        {
                            nextDirectoryToView = userHomePath+"/MyDocuments";
                            if( sff_ex::path_exists( nextDirectoryToView)==false)
                            {
                                nextDirectoryToView = userHomePath+"/My Documents";
                            }
                        }
                    }
                }

                if( appDirectoryButton->is_clicked() )
                {
                    nextDirectoryToView = stg_ex::get_path_from_file( gpe::app_directory_name);
                }

                if( examplesDirectoryButton->is_clicked() )
                {
                    nextDirectoryToView = gpe::app_directory_name+"examples";
                }

                /*if( tutorialsDirectoryButton->is_clicked() )
                {
                    nextDirectoryToView = gpe::app_directory_name+"tutorials";
                }*/

                if( gpe::input->check_kb_released(kb_esc) || cancelButton->is_clicked() )
                {
                    exitOperation = true;
                    returnVal = "";
                }
                else if( gpe::input->check_kb_released(kb_enter) || yesButton->is_clicked() )
                {
                    if( newStringBox!=NULL)
                    {
                        soughtReturnFilename = currentDirectoryInView+"/"+newStringBox->get_string();
                        if( isDirectorySearch )
                        {
                            if( sff_ex::path_exists(soughtReturnFilename) )
                            {
                                exitOperation = true;
                                returnFile = soughtReturnFilename;
                            }
                            else
                            {
                                soughtReturnFilename = currentDirectoryInView;
                                if( sff_ex::path_exists(soughtReturnFilename) )
                                {
                                    exitOperation = true;
                                    returnFile = soughtReturnFilename;
                                }
                            }
                        }
                        else if( isSaving && (int)newStringBox->get_string().size()>0 )
                        {
                            if( sff_ex::file_exists(soughtReturnFilename) )
                            {
                                if( display_prompt_message("File already exists","Do you wish to overwrite this file?")==display_query_yes )
                                {
                                    exitOperation = true;
                                    returnFile = soughtReturnFilename;
                                }
                            }
                            else
                            {
                                exitOperation = true;
                                returnFile = soughtReturnFilename;
                            }
                        }
                        else if( sff_ex::file_exists(soughtReturnFilename) )
                        {
                            exitOperation = true;
                            returnFile = soughtReturnFilename;
                        }
                    }
                }

                if( enterMyComputerMode)
                {
                    if( (int)mountedDriversList.size() > maxContentInView)
                    {

                        fileDirYScroll->set_coords( fileBrowserBox.x+fileBrowserBox.w-fileDirYScroll->get_width(), fileBrowserBox.y );
                        fileDirYScroll->set_height( fileBrowserBox.h );

                        fileDirYScroll->fullRect.x = 0;
                        fileDirYScroll->fullRect.y = 0;
                        fileDirYScroll->fullRect.w = 0;
                        fileDirYScroll->fullRect.h = (int)mountedDriversList.size();
                        //if( directoryViewBox->is_inuse()==false && newStringBox->is_inuse()==false && shortCutGList->hasScrollControl==false)
                        //{
                        if( gpe::point_between_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&fileBrowserBox ) )
                        {
                            if( gpe::input->mouse_scrolling_up || gpe::input->check_kb_down(kb_page_up))
                            {
                                iDirectoryXPos-=maxContentInView/2;
                            }
                            else if( gpe::input->mouse_scrolling_down || gpe::input->check_kb_down(kb_page_down) )
                            {
                                iDirectoryXPos+=maxContentInView/2;
                            }
                        }
                        fileDirYScroll->contextRect.x = 0;
                        fileDirYScroll->contextRect.y = iDirectoryXPos;
                        fileDirYScroll->contextRect.w = 0;
                        fileDirYScroll->contextRect.h = maxContentInView;

                        fileDirYScroll->process_self();
                        if( fileDirYScroll->has_moved() )
                        {
                            iDirectoryXPos = ceil( ( (float)mountedDriversList.size() ) * ( (float)fileDirYScroll->scrollYPos/(float)fileDirYScroll->get_height() ) );
                        }
                    }
                    else
                    {
                        fileDirYScroll->contextRect.x =  fileDirYScroll->fullRect.x = 0;
                        fileDirYScroll->contextRect.y =  fileDirYScroll->fullRect.y = 0;
                        fileDirYScroll->contextRect.w =  fileDirYScroll->fullRect.w = 32;
                        fileDirYScroll->contextRect.h =  fileDirYScroll->fullRect.h = (int)mountedDriversList.size();
                        fileDirYScroll->process_self();
                    }
                }
                else
                {
                    if( filesAndDirectoryPassedFilterCount > maxContentInView)
                    {
                        fileDirYScroll->set_coords( fileBrowserBox.x+fileBrowserBox.w-fileDirYScroll->get_width(), fileBrowserBox.y );
                        fileDirYScroll->set_height( fileBrowserBox.h );

                        fileDirYScroll->fullRect.x = 0;
                        fileDirYScroll->fullRect.y = 0;
                        fileDirYScroll->fullRect.w = 0;
                        fileDirYScroll->fullRect.h = filesAndDirectoryPassedFilterCount;

                        /*if( maxContentInView >8)
                        {
                        }
                        scrollAmount = */
                        //if( directoryViewBox->is_inuse()==false && newStringBox->is_inuse()==false && shortCutGList->hasScrollControl==false)
                        if( gpe::point_between_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&fileBrowserBox ) )
                        {

                            if( gpe::input->mouse_scrolling_up)
                            {
                                iFileXPos-=maxFilesInRows;
                            }
                            else if( gpe::input->mouse_scrolling_down)
                            {
                                iFileXPos+=maxFilesInRows;
                            }
                        }
                        if( directoryViewBox->is_inuse()==false && newStringBox->is_inuse()==false && shortCutGList->hasScrollControl==false )
                        {
                            if( gpe::input->check_kb_down(kb_up) )
                            {
                                iFileXPos-=maxFilesInRows;
                            }
                            else if( gpe::input->check_kb_down(kb_down) )
                            {
                                iFileXPos+=maxFilesInRows;
                            }
                        }

                        fileDirYScroll->contextRect.x = 0;
                        fileDirYScroll->contextRect.y = iFileXPos;
                        fileDirYScroll->contextRect.w = 0;
                        fileDirYScroll->contextRect.h = maxContentInView;
                        fileDirYScroll->process_self();

                        if( fileDirYScroll->has_moved() )
                        {
                            iFileXPos = ceil( ( (float)filesAndDirectoryPassedFilterCount ) * ( (float)fileDirYScroll->scrollYPos/(float)fileDirYScroll->get_height() ) );
                        }
                    }
                    else
                    {
                        fileDirYScroll->contextRect.x =  fileDirYScroll->fullRect.x = 0;
                        fileDirYScroll->contextRect.y =  fileDirYScroll->fullRect.y = 0;
                        fileDirYScroll->contextRect.w =  fileDirYScroll->fullRect.w = 32;
                        fileDirYScroll->contextRect.h =  fileDirYScroll->fullRect.h = filesAndDirectoryPassedFilterCount;
                        fileDirYScroll->process_self();
                    }
                }


                if( !gpe::window_controller_main->is_resized()  )
                {
                    gpe::renderer_main->clear_renderer( gpe::window_controller_main->is_minimized() );
                    //if( !gpe::input->window_input_received )
                    {
                        main_overlay_system->render_frozen_screenshot( );
                        //state_current->render();
                        //gpe::renderer_main->update_renderer();
                    }
                    //Update screen
                    gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->program_color_header,false);

                    gpe::gcanvas->render_rectangle( widget_box.x,widget_box.y,widget_box.x+widget_box.w,widget_box.y+32,pawgui::theme_main->popup_box_highlight_color,false);

                    gpe::gfs->render_text( widget_box.x+widget_box.w/2,widget_box.y+16,prompt,pawgui::theme_main->popup_box_highlight_font_color,gpe::font_default,gpe::fa_center,gpe::fa_middle);
                    if( enterMyComputerMode)
                    {
                        //render_text( widget_box.x+32,widget_box.y+48,"My Computer",pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_middle);
                    }
                    else
                    {
                        //render_text( widget_box.x+32,widget_box.y+48,"Viewing "+shorterCurrentDirectoryInView,pawgui::theme_main->popup_box_font_color,gpe::font_default,gpe::fa_left,gpe::fa_middle);
                    }

                    gpe::gcanvas->render_rect( &fileShortcutTab,pawgui::theme_main->text_box_color,false);
                    gpe::gcanvas->render_rect( &fileShortcutTab,pawgui::theme_main->text_box_highlight_color,true);

                    gpe::gcanvas->render_rect( &fileBrowserBox,pawgui::theme_main->text_box_color,false);
                    gpe::gcanvas->render_rect( &fileBrowserBox,pawgui::theme_main->text_box_highlight_color,true);

                    iContentCol = 0;
                    jContentRow = 0;
                    gpe::renderer_main->set_viewpoint( &fileBrowserBox);
                    mouseIsInBrowserBox = gpe::point_between_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&fileBrowserBox);

                    if( enterMyComputerMode==true)
                    {
                        //display my computer directory
                        for (iDirectory = iDirectoryXPos; iDirectory < (int)mountedDriversList.size() && iDirectory < iDirectoryXPos+maxContentInView; iDirectory++)
                        {
                            currFileRect.x = fileBrowserBox.x+padding_default;
                            currFileRect.y = fileBrowserBox.y+jContentRow*(default_thumbnal_size+padding_default);

                            if( mouseIsInBrowserBox && gpe::point_between_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&currFileRect) )
                            {
                                fileHoverNumber = iDirectory;
                                if( gpe::input->check_mouse_button_clicked(0) )
                                {
                                    nextDirectoryToView = mountedDriversList[iDirectory];
                                    fileHoverNumber = -1;
                                    fileselectedNumber = -1;
                                }
                                else if( gpe::input->check_mouse_released( mb_left))
                                {
                                    fileselectedNumber = iDirectory;
                                    fileHoverNumber = -1;
                                }
                            }

                            currFileRect.x = padding_default;
                            currFileRect.y = jContentRow*(default_thumbnal_size+padding_default);

                            if( fileselectedNumber ==iDirectory)
                            {
                                gpe::gcanvas->render_rect( &currFileRect,pawgui::theme_main->main_box_font_highlight_color,false);
                            }
                            else if( fileHoverNumber ==iDirectory)
                            {
                                gpe::gcanvas->render_rect( &currFileRect,pawgui::theme_main->main_border_color,false);
                            }
                            if( textRepHarddrive!=NULL )
                            {
                                textRepHarddrive->render_tex_resized( currFileRect.x,currFileRect.y,default_thumbnal_size,default_thumbnal_size,NULL, pawgui::theme_main->folder_color );
                            }
                            gpe::gfs->render_text( currFileRect.x+default_thumbnal_size+padding_default,currFileRect.y+default_thumbnal_size/2,mountedDriversList[iDirectory],pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_left,gpe::fa_middle);

                            jContentRow += 1;
                        }
                    }
                    else
                    {
                        filesProcessedCount = 0;
                        for(iFile = iFileXPos; filesProcessedCount < maxContentInView && iFile < currentDirectory->get_count(); iFile++)
                        {
                            currentFile = currentDirectory->get_file(iFile);
                            if( currentFile!=NULL)
                            {
                                fileToClick = currentFile->get_name();
                                if( currentFile->is_directory() )
                                {
                                    fileMatchesFilter = true;
                                }
                                else if( !isDirectorySearch )
                                {
                                    currentFileExt = currentFile->get_type();
                                    fileMatchesFilter = gpe::file_passes_filter(currentFileExt,fileFilterTypes);
                                }
                                else
                                {
                                    fileMatchesFilter = false;
                                }
                                if( fileMatchesFilter)
                                {
                                    currFileRect.x = fileBrowserBox.x+iContentCol*(currFileRect.w);
                                    currFileRect.y = fileBrowserBox.y+jContentRow*(currFileRect.h);
                                    if( mouseIsInBrowserBox && gpe::point_between_rect(gpe::input->mouse_position_x,gpe::input->mouse_position_y,&currFileRect) )
                                    {
                                        fileHoverNumber = iFile;
                                        if( gpe::input->check_mouse_button_clicked(0) )
                                        {
                                            if( currentFile->is_directory() )
                                            {
                                                nextDirectoryToView = currentDirectoryInView+"/"+fileToClick;
                                            }
                                            else
                                            {
                                                soughtReturnFilename = currentDirectoryInView+"/"+fileToClick;
                                                fileMatchesFilter = gpe::file_passes_filter(stg_ex::get_file_ext(fileToClick),fileFilterTypes);
                                                if( fileMatchesFilter)
                                                {
                                                    if( isSaving)
                                                    {
                                                        if( sff_ex::file_exists(soughtReturnFilename) )
                                                        {
                                                            if( display_prompt_message("File already exists","Do you wish to overwrite this file?")==display_query_yes )
                                                            {
                                                                exitOperation = true;
                                                                returnFile = soughtReturnFilename;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            exitOperation = true;
                                                            returnFile = soughtReturnFilename;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if( sff_ex::file_exists(soughtReturnFilename) )
                                                        {
                                                            exitOperation = true;
                                                            returnFile = soughtReturnFilename;
                                                        }
                                                    }
                                                }
                                            }
                                            fileHoverNumber = -1;
                                            fileselectedNumber = -1;
                                        }
                                        else if( gpe::input->check_mouse_released( mb_left))
                                        {
                                            fileselectedNumber = iFile;
                                            fileHoverNumber = -1;
                                            if( !currentFile->is_directory() )
                                            {
                                                newStringBox->set_string(fileToClick);
                                                previewedImageTexture = rsm_gui->texture_add_filename( currentDirectoryInView+"/"+fileToClick );
                                                if( previewedImageTexture!=NULL )
                                                {
                                                    if( fileMatchesFilter)
                                                    {
                                                        if( currentFileExt=="bmp" || currentFileExt=="gif" || currentFileExt=="jpg" || currentFileExt=="jpeg"  || currentFileExt=="png" )
                                                        {
                                                            if( previewedImageTexture->get_width() < 1 || previewedImageTexture->get_height() < 1 || previewedImageTexture->get_width() > 4096 || previewedImageTexture->get_height() > 4096)
                                                            {
                                                                    //previewedImageTexture->change_texture(NULL);
                                                            }
                                                        }
                                                        else
                                                        {
                                                            //previewedImageTexture->change_texture(NULL);
                                                        }
                                                    }
                                                }
                                                else if(previewedImageTexture!=NULL)
                                                {
                                                    //previewedImageTexture->change_texture(NULL);
                                                }
                                            }
                                            else if( isDirectorySearch)
                                            {
                                                newStringBox->set_string(fileToClick);
                                            }
                                        }
                                    }
                                    fileRenderName[0] = "";
                                    fileRenderName[1] = "";
                                    fileRenderName[2] = "";
                                    //Renders file
                                    currFileRect.x = iContentCol*(currFileRect.w);
                                    currFileRect.y = jContentRow*(currFileRect.h);
                                    if( fileselectedNumber ==iFile)
                                    {
                                        gpe::gcanvas->render_rect( &currFileRect,pawgui::theme_main->button_box_selected_color,false);
                                    }
                                    else if( fileHoverNumber ==iFile)
                                    {
                                        gpe::gcanvas->render_rect( &currFileRect,pawgui::theme_main->button_box_highlight_color,false);
                                    }
                                    currentFIleTexture = NULL;
                                    if( currentFile->is_directory() )
                                    {
                                        fileSubImageToDraw = 1;
                                        //mainFilesanimation
                                        if( fileselectedNumber ==iFile)
                                        {
                                            fileIconColor = pawgui::theme_main->folder_color_highlight;
                                        }
                                        else
                                        {
                                            fileIconColor = pawgui::theme_main->folder_color;
                                        }
                                        currentFIleTexture = textRepFolder;
                                    }
                                    else
                                    {
                                        fileToClick = currentFile->get_name();
                                        currentFileExt =currentFile->get_type();
                                        fileMatchesFilter = gpe::file_passes_filter(currentFileExt,fileFilterTypes);

                                        fileSubImageToDraw = 0;

                                        if( currentFileExt=="txt")
                                        {
                                            currentFIleTexture = textRepText;
                                        }
                                        else if( currentFileExt=="eot" || currentFileExt=="ttf" || currentFileExt=="woff")
                                        {
                                            currentFIleTexture = textRepFont;
                                        }
                                        else if( currentFileExt=="bmp" || currentFileExt=="gif" || currentFileExt=="jpg" || currentFileExt=="jpeg"  || currentFileExt=="png" )
                                        {
                                            currentFIleTexture = textRepImage;
                                        }
                                        else if( currentFileExt=="aac" || currentFileExt=="mp3" || currentFileExt=="ogg")
                                        {
                                            currentFIleTexture = textRepAudio;
                                        }
                                        else if( currentFileExt=="avi" || currentFileExt=="mp4" || currentFileExt=="webm")
                                        {
                                            currentFIleTexture = textRepVideo;
                                        }
                                        else if( currentFileExt=="c" || currentFileExt=="css" || currentFileExt=="h" || currentFileExt=="html" || currentFileExt=="js"  || currentFileExt=="php")
                                        {
                                            currentFIleTexture = textRepCode;
                                        }
                                        else if( currentFileExt=="gppf")
                                        {
                                            currentFIleTexture = textRepGPE;
                                        }
                                        else if( currentFileExt=="exe")
                                        {
                                            currentFIleTexture = textRepExe;
                                        }
                                        else
                                        {
                                            currentFIleTexture = textRepOther;
                                        }

                                        if( fileselectedNumber ==iFile)
                                        {
                                            fileIconColor = pawgui::theme_main->main_box_font_highlight_color;
                                        }
                                        else
                                        {
                                            fileIconColor  = pawgui::theme_main->main_box_font_color;
                                        }
                                    }

                                    fileRenderName[0] = "";
                                    fileRenderName[1] = "";
                                    fileRenderName[2] = "";
                                    if( (int)fileToClick.size() >=colTextCharacters)
                                    {
                                        fileRenderName[0] = stg_ex::get_substring(fileToClick,0,colTextCharacters);
                                        fileRenderName[1] = stg_ex::get_substring(fileToClick,colTextCharacters,colTextCharacters);

                                        if( (int)fileToClick.size() >colTextCharacters*3)
                                        {
                                            fileRenderName[2] = stg_ex::get_substring(fileToClick,colTextCharacters*2,colTextCharacters-3)+"...";
                                        }
                                        else
                                        {
                                            fileRenderName[2] = stg_ex::get_substring(fileToClick,colTextCharacters*2,colTextCharacters);
                                        }
                                        fileToClick = fileRenderName[2];
                                    }
                                    else
                                    {
                                        fileRenderName[0] = fileToClick;
                                    }
                                    //Renders Thumbnails
                                    if( fileBrowserModeBar->get_tab_pos()==0 )
                                    {
                                        if( currentFile->fileThumbnail!=NULL )
                                        {
                                            currentFile->fileThumbnail->render_tex_resized(
                                                                    currFileRect.x+(currFileRect.w-default_thumbnal_size)/2,currFileRect.y,default_thumbnal_size,default_thumbnal_size);
                                        }
                                        else if( currentFIleTexture!=NULL)
                                        {
                                            currentFIleTexture->render_tex_resized( currFileRect.x+(currFileRect.w-default_thumbnal_size)/2,currFileRect.y,default_thumbnal_size,default_thumbnal_size,NULL, fileIconColor );
                                        }
                                        gpe::gfs->render_text( currFileRect.x+currFileRect.w/2,currFileRect.y+default_thumbnal_size,fileRenderName[0],pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_center,gpe::fa_top);
                                        gpe::gfs->render_text( currFileRect.x+currFileRect.w/2,currFileRect.y+default_thumbnal_size+TEXTBOX_FONT_SIZE_HEIGHT,fileRenderName[1],pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_center,gpe::fa_top);
                                        gpe::gfs->render_text( currFileRect.x+currFileRect.w/2,currFileRect.y+default_thumbnal_size+TEXTBOX_FONT_SIZE_HEIGHT*2,fileRenderName[2],pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_center,gpe::fa_top);
                                    }
                                    else
                                    {
                                        //Renders listed icon
                                        if( currentFIleTexture!=NULL)
                                        {
                                            currentFIleTexture->render_tex_resized(
                                                                    currFileRect.x,currFileRect.y+(currFileRect.h-default_icon_width)/2,
                                                                    default_icon_width,default_icon_width,NULL, fileIconColor );
                                        }
                                        gpe::gfs->render_text( currFileRect.x+default_icon_width,currFileRect.y+currFileRect.h/2,fileRenderName[0],pawgui::theme_main->main_box_font_color,font_textinput,gpe::fa_left,gpe::fa_middle );
                                    }

                                    filesProcessedCount++;
                                    iContentCol++;
                                    if( iContentCol >= maxFilesInRows || fileBrowserModeBar->get_tab_pos()==2 )
                                    {
                                        iContentCol = 0;
                                        jContentRow += 1;
                                    }
                                }
                            }
                            //tinydir_next(&dir);
                        }
                    }
                    gpe::renderer_main->reset_viewpoint();
                    if( filesAndDirectoryPassedFilterCount > maxContentInView)
                    {
                        fileDirYScroll->render_self( );
                    }
                    /*
                    appDirectoryButton->render_self( );
                    backButton->render_self( );
                    desktopButton->render_self( );
                    documentsButton->render_self( );
                    downloadsButton->render_self( );
                    homeButton->render_self( );
                    musicButton->render_self( );
                    mycomputerButton->render_self( );*/
                    shortCutGList->render_self( );

                    refreshButton->render_self( );

                    yesButton->render_self( );
                    cancelButton->render_self( );
                    directoryViewBox->render_self( NULL, NULL);
                    fileBrowserModeBar->render_self( NULL, NULL);
                    newStringBox->render_self( NULL, NULL);
                    if( !isDirectorySearch)
                    {
                        fileTypeDropDown->render_self( NULL, NULL);
                    }

                    if( previewedImageTexture!=NULL && enterMyComputerMode==false && filePreviewBox.w!=0 && filePreviewBox.h!=0)
                    {
                        if( filePreviewBox.w > previewedImageTexture->get_width()&& filePreviewBox.h> previewedImageTexture->get_height() )
                        {
                            previewedImageTexture->render_tex( filePreviewBox.x,filePreviewBox.y);
                        }
                        else
                        {
                            imagePreviewScaleSize = std::max((float)previewedImageTexture->get_width()/ (float)(filePreviewBox.w), (float)previewedImageTexture->get_height()/(float)(filePreviewBox.h) );
                            if( imagePreviewScaleSize > 1)
                            {
                                previewedImageTexture->render_tex_resized( filePreviewBox.x,filePreviewBox.y,(float)previewedImageTexture->get_width()/imagePreviewScaleSize,(float)previewedImageTexture->get_height()/imagePreviewScaleSize);
                            }
                            else
                            {
                                previewedImageTexture->render_tex_resized( filePreviewBox.x,filePreviewBox.y,previewedImageTexture->get_width()*imagePreviewScaleSize,previewedImageTexture->get_height()*imagePreviewScaleSize);
                            }
                        }
                    }
                    gpe::gcanvas->render_rect( &widget_box,pawgui::theme_main->main_border_color,true);
                    gpe::game_runtime->end_loop(  );
                }
            }

            //Delete buttons and such
            //tinydir_close(&dir);
            if( currentDirectory!=NULL)
            {
                delete currentDirectory;
                currentDirectory = NULL;
            }

            if( refreshButton!=NULL)
            {
                delete refreshButton;
                refreshButton = NULL;
            }
            if( appDirectoryButton!=NULL)
            {
                delete appDirectoryButton;
                appDirectoryButton = NULL;
            }
            if( examplesDirectoryButton!=NULL)
            {
                delete examplesDirectoryButton;
                examplesDirectoryButton = NULL;
            }
            /*if( tutorialsDirectoryButton!=NULL)
            {
                delete tutorialsDirectoryButton;
                tutorialsDirectoryButton = NULL;
            }*/
            if( backButton!=NULL)
            {
                delete backButton;
                backButton = NULL;
            }
            if( desktopButton!=NULL)
            {
                delete desktopButton;
                desktopButton = NULL;
            }
            if( documentsButton!=NULL)
            {
                delete documentsButton;
                documentsButton = NULL;
            }
            if( downloadsButton!=NULL)
            {
                delete downloadsButton;
                downloadsButton = NULL;
            }
            if( picturesButton!=NULL)
            {
                delete picturesButton;
                picturesButton = NULL;
            }
            if( musicButton!=NULL)
            {
                delete musicButton;
                musicButton = NULL;
            }
            if( homeButton!=NULL)
            {
                delete homeButton;
                homeButton = NULL;
            }
            if( mycomputerButton!=NULL)
            {
                delete mycomputerButton;
                mycomputerButton = NULL;
            }
            if( yesButton!=NULL)
            {
                delete yesButton;
                yesButton = NULL;
            }
            if( cancelButton!=NULL)
            {
                delete cancelButton;
                cancelButton = NULL;
            }
            if( newStringBox!=NULL)
            {
                delete newStringBox;
                newStringBox = NULL;
            }
            if( fileTypeDropDown!=NULL)
            {
                delete fileTypeDropDown;
                fileTypeDropDown = NULL;
            }
            if( directoryViewBox!=NULL)
            {
                delete directoryViewBox;
                directoryViewBox = NULL;
            }
            if( fileBrowserModeBar!=NULL)
            {
                delete fileBrowserModeBar;
                fileBrowserModeBar = NULL;
            }
            if( fileDirYScroll!=NULL)
            {
                delete fileDirYScroll;
                fileDirYScroll = NULL;
            }
            if( shortCutGList!=NULL)
            {
                delete shortCutGList;
                shortCutGList = NULL;
            }
            gpe::input->reset_all_input();
            previousDirectory = currentDirectoryInView;
            return returnFile;
        }
        gpe::input->reset_all_input();
        return returnFile;
    }
}
