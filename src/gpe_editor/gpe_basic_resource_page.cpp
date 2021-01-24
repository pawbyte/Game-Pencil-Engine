/*
gpe_basic_resource_page.cpp
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

#include "gpe_basic_resource_page.h"

gpe::texture_base * texture_transparent_bg = NULL;
gpe::texture_base * texture_gpe_logo = NULL;

GPE_ObjectComponent::GPE_ObjectComponent()
{
    component = NULL;
    settingsGear = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cogs.png","settings",-1,16 );
}

GPE_ObjectComponent::~GPE_ObjectComponent()
{
    if( component!=NULL )
    {
        delete component;
        component = NULL;
    }

    if( settingsGear!=NULL)
    {
        delete settingsGear;
        settingsGear = NULL;
    }
}

void GPE_ObjectComponent::load_data(std::string dataString)
{
    if( component!=NULL)
    {
        component->load_data(dataString);
    }
}

void GPE_ObjectComponent::remove_data(std::string dataString)
{
    if( component!=NULL)
    {
        component->remove_data(dataString);
    }
}

void GPE_ObjectComponent::rename_component(std::string new_name)
{
    if( component!=NULL)
    {
        component->set_name(new_name);
    }

    if( settingsGear!=NULL)
    {
        settingsGear->set_name(new_name);
    }
}

pawgui::widget_basic * GPE_ObjectComponent::get_component()
{
    return component;
}

pawgui::widget_button_icon * GPE_ObjectComponent::get_gear()
{
    return settingsGear;
}

std::string GPE_ObjectComponent::get_data()
{
    if( component!=NULL)
    {
        return component->get_data();
    }
    return "";
}

std::string GPE_ObjectComponent::get_name()
{
    if( component!=NULL)
    {
        return component->get_name();
    }

    if( settingsGear!=NULL)
    {
        return settingsGear->get_name();
    }
    return "";

}

std::string GPE_ObjectComponent::get_plain_string()
{
    if( component!=NULL)
    {
        return component->get_plain_string();
    }
    return "";
}

std::string GPE_ObjectComponent::get_type()
{
    if( component!=NULL)
    {
        return component->get_element_type();
    }

    return "";
}

void GPE_ObjectComponent::set_component(pawgui::widget_basic * newComponent)
{
    if( newComponent!=NULL)
    {
        if( component!=NULL)
        {
            delete component;
        }
        component = newComponent;
        component->hasLineBreak = false;
        if( settingsGear!=NULL)
        {
            settingsGear->set_name( component->get_name() );
        }
    }
}

standardEditableGameResource::standardEditableGameResource(pawgui::widget_resource_container * ppFolder)
{
    resource_name = "";
    globalResouceIdNumber = -1;
    commonButtonAlignment = 0;
    resourcePostProcessed = false;
    justOpenedThisFrame = false;
    parentProjectName = "";
    projectParentFolder = ppFolder;
    if( projectParentFolder!=NULL)
    {
        parentProjectName = projectParentFolder->get_name();
    }
    renameBox = new pawgui::widget_input_text("","Name");
    renameBox->set_label("Name");

    loadResourceButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/folder.png","Load Resource",-1);
    exportResourceButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/plane.png","Export Resource TO File",-1);
    saveResourceButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/save.png","Save Resource to File",-1);

    confirmResourceButton = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/check.png","Confirm Changes","Confirm and Save Changes");
    cancelResourceButton =  new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/times.png","Cancel Changes","Cancel Changes and Revert to previous settings");

    int maxSize = std::max(confirmResourceButton->get_width(),cancelResourceButton->get_width() );
    //confirmResourceButton->set_width(maxSize);
    //cancelResourceButton->set_width(maxSize);
    //makes it so the main button options are the same width along with the rename string box too for design purposes
}


standardEditableGameResource::~standardEditableGameResource()
{
    if( confirmResourceButton!=NULL)
    {
        delete confirmResourceButton;
        confirmResourceButton = NULL;
    }
    if( cancelResourceButton!=NULL)
    {
        delete cancelResourceButton;
        cancelResourceButton = NULL;
    }
    if( loadResourceButton!=NULL)
    {
        delete loadResourceButton;
        loadResourceButton = NULL;
    }
    if( exportResourceButton!=NULL)
    {
        delete exportResourceButton;
        exportResourceButton = NULL;
    }
    if( saveResourceButton!=NULL)
    {
        delete saveResourceButton;
        saveResourceButton = NULL;
    }
    if( renameBox!=NULL)
    {
        delete renameBox;
        renameBox = NULL;
    }
}

std::string standardEditableGameResource::get_current_name()
{
    if( renameBox!=NULL)
    {
        if( stg_ex::is_alnum(renameBox->get_string(),false,true)==true)
        {
            return renameBox->get_string();
        }
        else if( main_editor_log!=NULL && resourceType!=gpe::resource_type_project_settings && globalResouceIdNumber >=1000 )
        {
            main_editor_log->log_general_comment("Non-alphabetical resource ["+renameBox->get_string()+"] being renamed to "+resource_name+"." );
        }
    }
    return get_name();
}

pawgui::widget_basic * standardEditableGameResource::add_gui_component(std::string componentData)
{
    if( (int)componentData.size() > 0)
    {
        int colonPos = componentData.find(":");
        int equalPos = componentData.find("==|||==");

        if( colonPos!=(int)std::string::npos && equalPos!=(int)std::string::npos && (equalPos>colonPos) )
        {
            std::string newComponentType = stg_ex::split_first_string(componentData,":");
            std::string newComponentName = stg_ex::split_first_string(componentData,"==|||==");

            pawgui::widget_basic * newComponentField = NULL;
            if( newComponentType=="checkbox")
            {
                newComponentField = new pawgui::widget_checkbox(newComponentName);
            }
            else if( newComponentType=="colorpicker")
            {
                pawgui::gpe_widget_color_picker * newColorField = new pawgui::gpe_widget_color_picker( newComponentName);
                newColorField->set_label(newComponentName);
                newComponentField = newColorField;
            }
            else if( newComponentType == "contentselector-int")
            {
                pawgui::widget_content_selector * newContentSelector = new pawgui::widget_content_selector( newComponentName, "");
                newContentSelector->selectorType = pawgui::content_selector_type::cs_type_int;
                newComponentField = newContentSelector;
            }
            else if( newComponentType == "contentselector-float")
            {
                pawgui::widget_content_selector * newContentSelector = new pawgui::widget_content_selector( newComponentName, "");
                newContentSelector->selectorType = pawgui::content_selector_type::cs_type_float;
                newComponentField = newContentSelector;
            }
            else if( newComponentType == "contentselector-string")
            {
                pawgui::widget_content_selector * newContentSelector = new pawgui::widget_content_selector( newComponentName, "");
                newContentSelector->selectorType = pawgui::content_selector_type::cs_type_string;
                newComponentField = newContentSelector;
            }
            else if( newComponentType == "contentselector-color")
            {
                pawgui::widget_content_selector * newContentSelector = new pawgui::widget_content_selector( newComponentName, "");
                newContentSelector->selectorType = pawgui::content_selector_type::cs_type_color;
                newComponentField = newContentSelector;
            }
            else if( newComponentType=="dropdown")
            {
                newComponentField = new pawgui::widget_dropdown_menu( newComponentName,false);
            }
            else if(newComponentType=="inputtext")
            {
                pawgui::widget_input_text * tempNewField = new pawgui::widget_input_text("");
                tempNewField->set_name(newComponentName);
                tempNewField->set_label(newComponentName);
                tempNewField->set_width(256);
                newComponentField = tempNewField;
            }
            else if(newComponentType=="inputnumber")
            {
                pawgui::widget_input_text * tempNewField = new pawgui::widget_input_number("");
                tempNewField->set_name(newComponentName);
                tempNewField->set_label(newComponentName);
                tempNewField->set_width(256);
                newComponentField = tempNewField;
            }
            else if(newComponentType=="radio")
            {
                pawgui::widget_radio_button_controller * tempRadioButton = new pawgui::widget_radio_button_controller();
                tempRadioButton->set_name(newComponentName);
                newComponentField = tempRadioButton;
            }
            else if(newComponentType=="labeltext")
            {
                newComponentField = new pawgui::widget_label_text (newComponentName,newComponentName);
            }
            else if(newComponentType=="texturl")
            {
                newComponentField = new pawgui::widget_text_url(newComponentName,newComponentName);
            }
            else if( newComponentType=="resourcedropdown")
            {
                std::string dropdownType = stg_ex::split_first_string(componentData,"=");
                newComponentField = new pawgui::widget_drop_down_resource_menu( newComponentName, projectParentFolder );
            }
            if( newComponentField!=NULL)
            {
                newComponentField->load_data(componentData);
            }
            //error_log->report("Returning ["+newComponentType+"] gui component named ["+newComponentName+"] using ["+componentData+"] data.");
            return newComponentField;
        }
    }
    gpe::error_log->report("Returning NULL gui component using ["+componentData+"] data.");
    return NULL;
}

bool standardEditableGameResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount )
{
    return true;
}

bool standardEditableGameResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount )
{
    return true;
}

void standardEditableGameResource::compile_cpp( )
{

}

bool standardEditableGameResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}

void standardEditableGameResource::prerender_self( )
{
    if( loadResourceButton!=NULL)
    {
        loadResourceButton->prerender_self( );
    }
    if( exportResourceButton!=NULL)
    {
        exportResourceButton->prerender_self( );
    }
    if( saveResourceButton!=NULL)
    {
        saveResourceButton->prerender_self( );
    }
    if( confirmResourceButton!=NULL)
    {
        confirmResourceButton->prerender_self( );
    }
    if( cancelResourceButton!=NULL)
    {
        cancelResourceButton->prerender_self( );
    }
}

void standardEditableGameResource::open_code( int lineNumb, int colNumb, std::string codeTitle )
{

}

void standardEditableGameResource::process_resource( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    isModified = false;
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    //Unclicks buttons
    if( loadResourceButton!=NULL )
        loadResourceButton->set_clicked( false );
    if( saveResourceButton!=NULL )
        saveResourceButton->set_clicked( false );
    if( exportResourceButton!=NULL )
        exportResourceButton->set_clicked( false );
    if( confirmResourceButton!=NULL )
        confirmResourceButton->set_clicked( false );
    if( cancelResourceButton!=NULL )
        cancelResourceButton->set_clicked( false );

    process_self(view_space,cam);
    process_export();
    /*
    if( confirmResourceButton!=NULL)
    {
        if( commonButtonAlignment==1)
        {
            cancelResourceButton->set_coords(view_space->w-cancelResourceButton->get_width()-pawgui::padding_default,view_space->h-cancelResourceButton->get_height()-pawgui::default_line_height);
            cancelResourceButton->process_self(view_space,cam);
            if( confirmResourceButton!=NULL)
            {
                if( confirmResourceButton->is_enabled() )
                {
                    confirmResourceButton->set_coords(cancelResourceButton->get_xpos()-confirmResourceButton->get_width()-pawgui::padding_default,cancelResourceButton->get_ypos());
                }
                else
                {
                    confirmResourceButton->set_coords(cancelResourceButton->get_xpos()-confirmResourceButton->get_width()-pawgui::padding_default,cancelResourceButton->get_ypos());
                }
                confirmResourceButton->process_self(view_space,cam);
                if( confirmResourceButton->is_clicked() && editorMode==0 )
                {
                    if( renameBox!=NULL)
                    {
                        set_name( renameBox->get_string() );
                    }
                }
            }
        }
        else
        {
            cancelResourceButton->set_coords(-1,view_space->h-cancelResourceButton->get_height()-pawgui::default_line_height);
            cancelResourceButton->process_self(view_space,cam);
            if( confirmResourceButton!=NULL)
            {
                if( cancelResourceButton->is_enabled() )
                {
                    confirmResourceButton->set_coords(-1,cancelResourceButton->get_ypos()-confirmResourceButton->get_height()-pawgui::default_line_height);
                }
                else
                {
                    confirmResourceButton->set_coords(-1,view_space->h-confirmResourceButton->get_height()-pawgui::default_line_height);
                }
                confirmResourceButton->process_self(view_space,cam);
                if( confirmResourceButton->is_clicked() && editorMode==0 )
                {
                    if( renameBox!=NULL)
                    {
                        set_name( renameBox->get_string() );
                    }
                }
            }
        }
    }
    if( renameBox!=NULL)
    {
        renameBox->process_self(view_space,cam);
        //set_name( renameBox->get_string() );
        if(resource_name!=renameBox->get_string() )
        {
            isModified = true;
        }
    }
    */
}

void standardEditableGameResource::process_export()
{
    bool exportCalled = false;
    if( exportResourceButton!=NULL && exportResourceButton->is_clicked() )
    {
        exportCalled = true;
    }
    else if( gpe::input->check_kb_down(kb_ctrl) && gpe::input->check_kb_pressed(kb_e) )
    {
        exportCalled = true;
        gpe::input->reset_all_input();
    }

    if( exportCalled )
    {
        if( resourceType >=0 && resourceType < gpe::resource_type_max )
        {
            std::string fileToExportName = pawgui::get_filename_save_from_popup( "Export "+gpe::resource_type_names[resourceType]+" to file",".gpf", pawgui::main_settings->fileSaveExportFileDir );
            if( (int)fileToExportName.size() > 0)
            {
                if(  stg_ex::get_file_ext(fileToExportName) !="gpf" )
                {
                    fileToExportName = stg_ex::get_file_noext(fileToExportName)+".gpf";
                }
                main_editor_log->log_general_line("Exporting "+gpe::resource_type_names[resourceType]+" resource to "+fileToExportName+".");
                save_resource(fileToExportName);
            }
            else
            {
                main_editor_log->log_general_comment("Export of ["+resource_name+"] canceled.");
            }
        }
        exportResourceButton->set_clicked( false );
    }
}

void standardEditableGameResource::integrate_into_syntax()
{
    if( current_project!=NULL)
    {
        if( resourceType >=0 && resourceType < gpe::resource_type_max)
        {
            current_project->add_project_keyword(resource_name,"Project "+gpe::resource_type_names[resourceType]+" Resource",-1,"");
        }
    }
}

bool standardEditableGameResource::is_build_ready()
{
    recentErrorMessage = "";
    return true;
}

void standardEditableGameResource::load_resource(std::string file_path)
{

}

void standardEditableGameResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{

}

void standardEditableGameResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect *cam  )
{

}

void standardEditableGameResource::seek_parent_name()
{
    if( projectParentFolder!=NULL)
    {
        parentProjectName = projectParentFolder->get_name();
    }
}

void standardEditableGameResource::set_name(std::string new_name)
{
    resource_name = new_name;
    if(renameBox!=NULL)
    {
        renameBox->set_string(new_name);
    }
}

void standardEditableGameResource::set_parent_name(std::string new_name)
{
    parentProjectName = new_name;
}

int standardEditableGameResource::search_for_string(std::string needle)
{
    return 0;
}

int standardEditableGameResource::search_and_replace_string(std::string needle, std::string newStr )
{
    return 0;
}

