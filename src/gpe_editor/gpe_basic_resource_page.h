/*
gpe_basic_resource_page.h
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


#ifndef GPE_BASIC_project_RESOURCE_H
#define GPE_BASIC_project_RESOURCE_H


#include "gpe_dock_system.h"
#include "gpe_editor_globals.h"
#include "gpe_editor_project.h"
#include "../pawgui/pawgui_general_resource.h"
#include "../pawgui/pawgui.h"
#include "../sdl_libs/gpe_sdl_converter.h"
#include "../sdl_libs/sdl_surface_ex.h"

extern gpe::texture_base * texture_transparent_bg;
extern gpe::texture_base * texture_gpe_logo;

class GPE_ObjectComponent
{
private:
    pawgui::widget_basic * component;
    pawgui::widget_button_icon * settingsGear;
public:
    GPE_ObjectComponent();
    ~GPE_ObjectComponent();

    void load_data(std::string dataString);
    void remove_data(std::string dataString);
    void rename_component(std::string new_name);
    std::string get_data();
    std::string get_name();
    std::string get_plain_string();
    std::string get_type();
    pawgui::widget_basic * get_component();
    pawgui::widget_button_icon * get_gear();
    void set_component(pawgui::widget_basic * newComponent);
};

class standardEditableGameResource : public pawgui::general_resource
{
protected:
    pawgui::widget_button_icon * loadResourceButton;
    pawgui::widget_button_icon * exportResourceButton;
    pawgui::widget_button_icon * saveResourceButton;
    pawgui::widget_button_push * confirmResourceButton;
    pawgui::widget_button_push * cancelResourceButton;
    pawgui::widget_input_text * renameBox;
public:
    bool resourcePostProcessed;
    int commonButtonAlignment;
    std::vector < GPE_ObjectComponent * > customComponentRealList;
    pawgui::widget_resource_container * projectParentFolder;
    standardEditableGameResource(pawgui::widget_resource_container * ppFolder = NULL);
    virtual ~standardEditableGameResource() = 0;
    std::string get_current_name();
    pawgui::widget_basic * add_gui_component(std::string componentData);
    virtual bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    virtual bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    virtual void compile_cpp();
    virtual bool include_local_files( std::string pBuildDir , int buildType );
    virtual void integrate_into_syntax();
    virtual bool is_build_ready();
    void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
    virtual void load_resource(std::string file_path = "" );
    virtual void prerender_self( );
    void process_export();
    virtual void process_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    virtual void process_resource( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    virtual void render_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    void seek_parent_name();
    void set_name(std::string new_name);
    void set_parent_name(std::string new_name);
    virtual int search_for_string(std::string needle);
    virtual int search_and_replace_string(std::string needle, std::string newStr = "");
};

#endif
