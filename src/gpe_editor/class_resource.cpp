/*
class_resource.cpp
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

#include "class_resource.h"

classResource::classResource(pawgui::widget_resource_container * pFolder)
{
    codeCategoryTabs = new pawgui::widget_tabbar();
    codeCategoryTabs->set_coords(16,16);
    codeCategoryTabs->add_new_tab("Header",false);
    codeCategoryTabs->add_new_tab("Source",true);
    classHeaderCode = NULL;
    classHeaderCode = new pawgui::widget_text_editor(false);
    classHeaderCode->set_placeholder("Header Code...");


    classSourceCode = new pawgui::widget_text_editor(false);
    classSourceCode->set_placeholder("Source Code...");

    projectParentFolder = pFolder;

    /*cancelResourceButton->disable_self();
    confirmResourceButton->disable_self();
    loadResourceButton->disable_self();
    saveResourceButton->disable_self();*/
    //textEditorButtonBar->set_width(256);
    renameBox->set_coords(pawgui::padding_default,-1 );
    saveButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/save.png","Save Changes",-1,24);
    classEditorList = new pawgui::widget_panel_list();
    //saveResourceButton->disable_self();
}

classResource::~classResource()
{
    if(classHeaderCode!=NULL)
    {
        delete classHeaderCode;
        classHeaderCode = NULL;
    }
    if(classSourceCode!=NULL)
    {
        delete classSourceCode;
        classSourceCode = NULL;
    }
    if(codeCategoryTabs!=NULL)
    {
        delete codeCategoryTabs;
        codeCategoryTabs = NULL;
    }

    if(renameBox!=NULL)
    {
        delete renameBox;
        renameBox = NULL;
    }

    if( saveButton!=NULL)
    {
        delete saveButton;
        saveButton = NULL;
    }

    if( classEditorList!=NULL)
    {
        delete classEditorList;
        classEditorList = NULL;
    }

}

bool classResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    return true;
}

bool classResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void classResource::compile_cpp()
{

}

bool classResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

bool classResource::is_build_ready()
{

}

void classResource::integrate_into_syntax()
{
    if( current_project!=NULL)
    {
        std::string fReturnType = "unknown_void";
        std::string functionDescription = "User defined function...";
        /*
        if( parametersField !=NULL )
        {
            current_project->add_project_function(resource_name,functionDescription,parametersField->get_string(),fReturnType,"User Defined Global Function");
        }
        */
    }
}

void classResource::open_code( int lineNumb, int colNumb,std::string codeTitle)
{
    if( classHeaderCode!=NULL && classHeaderCode->has_content() && codeTitle=="header" )
    {
        classHeaderCode->scroll_to_pos(lineNumb, colNumb);
    }
    else  if( classSourceCode!=NULL && classSourceCode->has_content() )
    {
        classSourceCode->scroll_to_pos(lineNumb, colNumb);
    }
}

void classResource::load_resource(std::string file_path)
{
    if( resourcePostProcessed ==false  || sff_ex::file_exists(file_path) )
    {
        if( pawgui::main_loader_display != NULL )
        {
            pawgui::main_loader_display->update_submessages( "Processing Class", resource_name );
        }

        bool usingAltSaveSource = false;
        std::string newFileIn ="";
        std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/source/";
        if( sff_ex::file_exists(file_path) )
        {
            newFileIn = file_path;
            soughtDir = stg_ex::get_path_from_file(newFileIn);
            usingAltSaveSource = true;
        }
        else
        {
            newFileIn = soughtDir + "class_" + resource_name+".gpf";
        }


        if( classSourceCode!=NULL)
        {
            std::string  classSrcCodeLoadLocation = "";
            //if( )
            classSrcCodeLoadLocation = soughtDir + "class_" + resource_name+".js";

            if( current_project !=NULL )
            {
                if( current_project->get_project_language_id() ==
                   pawgui::program_language_cpp )
                {
                    classSrcCodeLoadLocation = soughtDir+  "entity_" + resource_name + ".cpp";
                }
            }

            classSourceCode->import_text(classSrcCodeLoadLocation);
            classSourceCode->activate_self();
            classSourceCode->init_save_history();
        }


        if( classHeaderCode!=NULL)
        {
            std::string classHeaderCodeLoadLocation = soughtDir+ "class_" + resource_name + ".h";
            classHeaderCode->import_text(classHeaderCodeLoadLocation);
            classHeaderCode->activate_self();
            classHeaderCode->init_save_history();
        }

        std::string otherColContainerName = "";

        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        gpe::error_log->report("Loading class meta data - "+newFileIn);
        //If the level file could be loaded
        if( !gameResourceFileIn.fail() )
        {
            //makes sure the file is open
            if (gameResourceFileIn.is_open())
            {
                int equalPos = 0;
                std::string firstChar="";
                std::string section="";
                std::string keyString="";
                std::string valString="";
                std::string subValString="";
                std::string currLine="";
                std::string currLineToBeProcessed;
                float foundFileVersion = 0;
                int fCursorX = 0;
                int fCursorY = 0;
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
                                if( keyString=="ResourceName")
                                {
                                    renameBox->set_string(valString);
                                }
                                else if( keyString=="Header_Cursor" && classHeaderCode!=NULL)
                                {
                                    fCursorY = stg_ex::split_first_int(valString,',');
                                    fCursorX = stg_ex::string_to_int(valString,0);
                                    classHeaderCode->set_ycursor(fCursorY);
                                    classHeaderCode->set_xcursor(fCursorX);
                                }
                                else if( keyString=="Source_Cursor" & classSourceCode!=NULL)
                                {
                                    fCursorY = stg_ex::split_first_int(valString,',');
                                    fCursorX = stg_ex::string_to_int(valString,0);
                                    classSourceCode->set_ycursor(fCursorY);
                                    classSourceCode->set_xcursor(fCursorX);
                                }
                            }
                        }
                    }
                    else
                    {
                        gpe::error_log->report("Invalid FoundFileVersion ="+ stg_ex::float_to_string(foundFileVersion)+".");
                    }
                }
            }
        }
    }
}

void classResource::prerender_self(  )
{
    standardEditableGameResource::prerender_self();
}

void classResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    gpeEditorDockPanel * fEditorPanel = GPE_DOCK->find_panel("Editor");
    if( classEditorList!=NULL && cam!=NULL && view_space!=NULL && codeCategoryTabs!=NULL &&  saveButton!=NULL && renameBox!=NULL && classSourceCode!=NULL )
    {
        int prevTab = codeCategoryTabs->tabInUse;

        classEditorList->set_coords( 0, 0 );
        classEditorList->set_width(view_space->w );
        classEditorList->set_height(view_space->h);
        classEditorList->clear_list();

        classHeaderCode->set_width(view_space->w );
        classHeaderCode->set_height(view_space->h-64 );

        classSourceCode->set_width(view_space->w );
        classSourceCode->set_height(view_space->h-64);

        classEditorList->set_coords( 0, 0 );
        classEditorList->set_width( view_space->w );
        classEditorList->set_height( view_space->h);
        classEditorList->clear_list();

        if( panel_main_area!=NULL )
        {
            panel_main_area->add_gui_element(renameBox,true);
            panel_main_area->add_gui_element(confirmResourceButton,true);
            panel_main_area->add_gui_element(cancelResourceButton,true);
            panel_main_area->process_self();
        }
        else
        {
            classEditorList->add_gui_element(saveButton,false);
            classEditorList->add_gui_element(renameBox,true);
        }
        //CPP is the only language that does header files so...
        if( current_project->get_project_language_id() == pawgui::program_language_cpp)
        {
            if( codeCategoryTabs->get_selected_name()=="Header" && classHeaderCode!=NULL)
            {
                classEditorList->add_gui_element(classHeaderCode,true);
            }
            else
            {
                classEditorList->add_gui_element(classSourceCode,true);
            }
            classEditorList->add_gui_element(codeCategoryTabs,true);
        }
        else
        {
            classEditorList->add_gui_element(classSourceCode,true);
        }

        classEditorList->process_self( view_space,cam );
        if( saveButton->is_clicked() )
        {
            save_resource();
        }
        else if( panel_main_area!=NULL )
        {
            if( confirmResourceButton->is_clicked() )
            {
                save_resource();
            }
        }
    }
}

void classResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if(cam!=NULL && view_space!=NULL)
    {
        classEditorList->render_self( view_space,cam );
    }
}

void classResource::save_resource(std::string file_path, int backupId)
{
    if( pawgui::main_loader_display != NULL )
    {
        pawgui::main_loader_display->update_submessages( "Saving Class", resource_name );
    }
    sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt ", "Saving "+ get_name() +"...");
    bool usingAltSaveSource = false;
    std::string newFileOut ="";
    std::string soughtDir = stg_ex::get_path_from_file(file_path);
    if( sff_ex::path_exists(soughtDir) )
    {
        newFileOut = file_path;
        usingAltSaveSource= true;
    }
    else
    {
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/classes/";
        newFileOut = soughtDir + resource_name+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the scene file could be saved
    if( !newSaveDataFile.fail() )
    {
        //makes sure the file is open
        if (newSaveDataFile.is_open())
        {
            write_header_on_file(&newSaveDataFile);

            if( classSourceCode!=NULL)
            {
                //    *fileTarget << classSourceCode->get_xcursor() << "," << classSourceCode->get_ycursor() << ",";
                //std::string headerCodeSaveLocation = soughtDir+resource_name+".h"; // CPP headers
                std::string sourceCodeSaveLocation = soughtDir+resource_name+".js";
                if( usingAltSaveSource)
                {
                    if( sff_ex::file_exists(sourceCodeSaveLocation) )
                    {
                        /*
                        if( pawgui::display_prompt_message("[WARNING]Class File Already exists?","Are you sure you will like to overwrite your ["+resource_name+".js] Class file? This action is irreversible!")==pawgui::display_query_yes)
                        {
                            classSourceCode->export_text(sourceCodeSaveLocation );
                        }*/
                    }
                    else
                    {
                        classSourceCode->export_text(sourceCodeSaveLocation );
                    }
                }
                else
                {
                    classSourceCode->export_text(sourceCodeSaveLocation );
                }
                newSaveDataFile << "Cursor=" << classSourceCode->get_ycursor() << "," << classSourceCode->get_xcursor() << "\n";
            }
            else
            {
                //*fileTarget << "0,0,";
            }
            newSaveDataFile.close();
            if( !usingAltSaveSource)
            {
                isModified = false;
            }
        }
        else
        {
            main_editor_log->log_general_error("Unable to save to file ["+newFileOut+"]");
        }
    }
    else
    {
        main_editor_log->log_general_error("Unable to save file ["+newFileOut+"]");
    }
}

int classResource::search_for_string(std::string needle)
{
    int foundStrings = 0;
    main_editor_log->log_general_comment("Searching ["+resource_name+"] class..");
    if( classSourceCode!=NULL && pawgui::main_anchor_controller!=NULL && classSourceCode->has_content() )
    {
        pawgui::main_anchor_controller->searchResultProjectName = parentProjectName;
        pawgui::main_anchor_controller->searchResultResourceId = globalResouceIdNumber;
        pawgui::main_anchor_controller->searchResultResourceName = resource_name;
        foundStrings=classSourceCode->find_all_strings(needle,pawgui::main_search_controller->findMatchCase->is_clicked(),true,"class");
    }
    return foundStrings;
}

int classResource::search_and_replace_string(std::string needle, std::string newStr )
{
    int foundStrings = 0;
    if( classSourceCode!=NULL && pawgui::main_search_controller!=NULL && classSourceCode->has_content() )
    {
        if( pawgui::main_loader_display != NULL )
        {
            pawgui::main_loader_display->update_messages( "Replacing Substring", needle, "with ["+newStr+"]" );
        }

        foundStrings = classSourceCode->find_all_strings(needle,pawgui::main_search_controller->findMatchCase->is_clicked() );
        if( foundStrings > 0)
        {
            int replaceCount = classSourceCode->replace_all_found(needle, newStr );
            if( pawgui::main_loader_display != NULL )
            {
                pawgui::main_loader_display->update_messages( "Replaced", needle, stg_ex::int_to_string(replaceCount)+" times");
            }
            pawgui::main_search_controller->showFindAllResults = true;
        }
        else
        {
            pawgui::main_loader_display->update_messages( "Replaced", needle, "No matches found");
            pawgui::main_search_controller->showFindAllResults = false;
        }
        //pawgui::main_overlay_system->update_temporary_message(displayMessageTitle,displayMessageSubtitle,displayMessageString,1);
    }
    return foundStrings;
}

bool classResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Class=" << resource_name << "," << get_global_rid() << ",\n";
            save_resource();
            return true;
        }
    }
    return false;
}
