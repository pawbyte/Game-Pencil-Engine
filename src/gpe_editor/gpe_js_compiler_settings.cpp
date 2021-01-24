/*
gpe_js_compiler_settings.cpp
This file is part of:
GAME PENCIL ENGINE
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

#include "gpe_js_compiler_settings.h"

gameJSCompilerSettingsResource * GPE_JS_COMPILER_SETTINGS = NULL;

gameJSCompilerSettingsResource::gameJSCompilerSettingsResource(pawgui::widget_resource_container * pFolder)
{
    isFullScreenResource = true;
    resourceFileName = "";
    resource_name = "JS Compiler Settings";
    parentProjectName = "";
    parentProjectDirectory = "";

    sideAreaPanel = new pawgui::widget_selectbox("Mode");
    sideAreaPanel->set_width(160);
    sideAreaPanel->set_option_height(64);
    sideAreaPanel->add_option("General",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/wrench.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Obfuscation",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/crosshairs.png"),NULL,2, false, false);

    sidePanelRect = new gpe::shape_rect();

    editorPageList = new pawgui::widget_panel_list();
    editorPageList->barXPadding = pawgui::padding_default;
    editorPageList->panelAlignType = pawgui::panel_align_left;
    editorPageList->barXMargin = 0;

    editorPageTabBar = new pawgui::widget_tabbar();
    editorPageTabBar->set_coords(16, 16);
    editorPageTabBar->add_new_tab("General");
    editorPageTabBar->add_new_tab("Obfuscation");
    //editorPageTabBar->add_new_tab("Themes");
    //editorPageTabBar->add_new_tab("Other Settings");
    editorPageTabBar->open_tab(0);

    subViewedSpace.x = 0;
    subViewedSpace.y = 0;
    subViewedSpace.w = gpe::screen_width;
    subViewedSpace.h = gpe::screen_height;

    stopCompileOnError = new pawgui::widget_checkbox("Stop Compiling on first detected error","Exit build phase as soon as error is found", false);

    //Added as of 1.13 [ BEGIN ]
    minifyCode = new pawgui::widget_checkbox("Minify Code","Use to eliminate many linebreaks and un-needed spaces in code base", true);
    pluginConstantValues = new pawgui::widget_checkbox("Plugin Constant Values","Removes constants in codebase and replaces it with their values", true);
    obfuscateCode = new pawgui::widget_checkbox("Obfuscate Code","Obfuscate your games code as a layer of protection",true);
    obfuscatorDirectoryField = new pawgui::widget_input_text("","");
    obfuscatorDirectoryField->set_label("Obfuscator Directory");
    obfuscatorDirectoryLoadButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/binoculars.png","Search Directory..",-1,32);

    googleClosureCompilerFile = new pawgui::widget_input_text("","");
    googleClosureCompilerFile->set_label("Google Closure File Location");
    googleClosureCompilerLoadButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/binoculars.png","Find Editor..",-1,32);

    //Added as of 1.13 [ END ]
}

gameJSCompilerSettingsResource::~gameJSCompilerSettingsResource()
{
    if( editorPageList!=NULL)
    {
        editorPageList->clear_list();
        delete editorPageList;
        editorPageList = NULL;
    }

    if( editorPageTabBar!=NULL)
    {
        delete editorPageTabBar;
        editorPageTabBar = NULL;
    }


    if( minifyCode!=NULL)
    {
        delete minifyCode;
        minifyCode = NULL;
    }
    if( pluginConstantValues!=NULL)
    {
        delete pluginConstantValues;
        pluginConstantValues = NULL;
    }
    if( obfuscateCode!=NULL)
    {
        delete obfuscateCode;
        obfuscateCode = NULL;
    }

    if( obfuscatorDirectoryField!=NULL)
    {
        delete obfuscatorDirectoryField;
        obfuscatorDirectoryField = NULL;
    }

    if( obfuscatorDirectoryLoadButton!=NULL)
    {
        delete obfuscatorDirectoryLoadButton;
        obfuscatorDirectoryLoadButton = NULL;
    }

    if( googleClosureCompilerFile!=NULL)
    {
        delete googleClosureCompilerFile;
        googleClosureCompilerFile = NULL;
    }

    if( googleClosureCompilerLoadButton!=NULL)
    {
        delete googleClosureCompilerLoadButton;
        googleClosureCompilerLoadButton = NULL;
    }
}

bool gameJSCompilerSettingsResource::include_local_files( std::string pBuildDir , int buildType )
{

}

void gameJSCompilerSettingsResource::prerender_self( )
{

}

void gameJSCompilerSettingsResource::load_resource(std::string file_path)
{
    //showStatupTipsBox->set_clicked( editor_gui_main->showTipsAtStartUp );
    if( pawgui::main_loader_display != NULL )
    {
        pawgui::main_loader_display->update_submessages( "Loading JS Compiler Settings","Please Wait..." );
    }

    std::string otherColContainerName = "";

    std::string newFileIn =  gpe::get_user_settings_folder()+"gpe_js_compiler_settings.txt";
    std::ifstream gameResourceFileIn( newFileIn.c_str() );

    gpe::error_log->report("Loading Local settings - "+newFileIn);
    //If the level file could be loaded
    if( !gameResourceFileIn.fail() )
    {
        //makes sure the file is open
        if (gameResourceFileIn.is_open())
        {
            int equalPos = 0;
            std::string firstChar="";
            std::string keyString="";
            std::string valString="";
            std::string subValString="";
            std::string currLine="";
            std::string currLineToBeProcessed;
            std::string colortheme_name;
            float foundFileVersion = 0;
            int textDelTime = 0;
            int foundFPSValue = gpe::fps_cap;
            while ( gameResourceFileIn.good() )
            {
                getline (gameResourceFileIn,currLine); //gets the next line of the file
                currLineToBeProcessed = stg_ex::trim_left_inplace(currLine);
                currLineToBeProcessed = stg_ex::trim_right_inplace(currLineToBeProcessed);

                if( foundFileVersion <=0)
                {
                    //Empty Line skipping is only allowed at the top of the file
                    if(!currLineToBeProcessed.empty() )
                    {
                        //Comment skipping is only allowed at the top of the file
                        if( currLineToBeProcessed[0]!= '#' && currLineToBeProcessed[0]!='/'  )
                        {
                            //searches for an equal character and parses through the variable
                            equalPos=currLineToBeProcessed.find_first_of("=");
                            if(equalPos!=(int)std::string::npos)
                            {
                                //if the equalPos is present, then parse on through and carryon
                                keyString = currLineToBeProcessed.substr(0,equalPos);
                                valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                                if( keyString=="Version")
                                {
                                    foundFileVersion = stg_ex::string_to_float(valString);
                                }
                            }
                        }
                    }
                }
                else if( foundFileVersion <= 2)
                {
                    //Begin processing the file.
                    if(!currLineToBeProcessed.empty() )
                    {
                        equalPos=currLineToBeProcessed.find_first_of("=");
                        if(equalPos!=(int)std::string::npos)
                        {
                            //if the equalPos is present, then parse on through and carryon
                            keyString = currLineToBeProcessed.substr(0,equalPos);
                            valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());
                            if( keyString=="ShowCompileAtFirstError")
                            {
                                stopCompileOnError->set_clicked( stg_ex::string_to_bool( valString ) );
                            }
                        }
                    }
                }
            }
        }
    }
}

void gameJSCompilerSettingsResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    cam = gpe::camera_find(cam);
    view_space = gpe::camera_find(view_space);
    if( cam!=NULL && editorPageList!=NULL && editorPageTabBar!=NULL && view_space!=NULL)
    {
        int prevTab = sideAreaPanel->get_selection();
        if( panel_main_area!=NULL )
        {
            subViewedSpace.x = 0;
            subViewedSpace.y = 0;
            subViewedSpace.w = view_space->w;
            subViewedSpace.h = view_space->h;
            panel_main_area->add_gui_element_fullsize( sideAreaPanel );
            panel_main_area->process_self();

        }
        else
        {
            sideAreaPanel->set_coords(0, 0 );
            sideAreaPanel->set_height( view_space->h );
            sideAreaPanel->process_self( view_space, cam );
            subViewedSpace.x = sideAreaPanel->get_x2();
            subViewedSpace.y = 0;
            subViewedSpace.w = view_space->w - sideAreaPanel->get_width();
            subViewedSpace.h = view_space->h;
        }

        if( prevTab!=sideAreaPanel->get_selection() )
        {
            editorPageList->reset_self();
        }
        editorPageList->clear_list();
        editorPageList->set_coords( subViewedSpace.x, subViewedSpace.y );
        editorPageList->set_width(subViewedSpace.w);
        editorPageList->set_height(subViewedSpace.h );
        editorPageList->barXPadding = pawgui::padding_default;
        editorPageList->barXMargin = pawgui::padding_default;
        int i = 0;
        if(sideAreaPanel->get_selected_name()=="General")
        {
            editorPageList->add_gui_element(stopCompileOnError,true);

            editorPageList->add_gui_element(confirmResourceButton,false);
            editorPageList->add_gui_element(cancelResourceButton,true);
        }
        else if(sideAreaPanel->get_selected_name()=="Obfuscation")
        {
            // To be used in future Versions hopefully...
            editorPageList->add_gui_element(obfuscatorDirectoryField,false);
            editorPageList->add_gui_element(obfuscatorDirectoryLoadButton,true);
            editorPageList->add_gui_element(googleClosureCompilerFile,false);
            editorPageList->add_gui_element(googleClosureCompilerLoadButton,true);
            editorPageList->add_gui_element(minifyCode,true);
            editorPageList->add_gui_element(pluginConstantValues,true);
            editorPageList->add_gui_element(obfuscateCode,true);
            editorPageList->add_gui_element(confirmResourceButton,false);
            editorPageList->add_gui_element(cancelResourceButton,true);
        }
        editorPageList->process_self( view_space,cam);
        if( confirmResourceButton->is_clicked() )
        {
            save_resource();
        }
        if( cancelResourceButton->is_clicked() )
        {
            load_resource();
        }
    }
}

void gameJSCompilerSettingsResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    cam = gpe::camera_find(cam);
    view_space = gpe::camera_find(view_space);
    if( cam!=NULL && view_space!=NULL)
    {
        if( sideAreaPanel!=NULL && panel_main_area==NULL )
        {
            sideAreaPanel->render_self( view_space,cam);
        }
        if( editorPageList!=NULL )
        {
            editorPageList->render_self( view_space,cam);
        }
    }
}

void gameJSCompilerSettingsResource::save_resource(std::string file_path, int backupId )
{
    bool usingAltSaveSource = false;
    isModified = false;
    std::string newSaveDataFilename =  gpe::get_user_settings_folder()+"gpe_js_compiler_settings.txt";
    std::ofstream newSaveDataFile( newSaveDataFilename.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            write_header_on_file(&newSaveDataFile);
            newSaveDataFile << "#     \n";
            newSaveDataFile << "ShowCompileAtFirstError=" << stopCompileOnError->is_clicked() << "\n";
        }
    }
}
bool gameJSCompilerSettingsResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{

}
