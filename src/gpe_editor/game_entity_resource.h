/*
game_entity_resource.h
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

#ifndef GAME_ENTITY_RESOURCE_H
#define GAME_ENTITY_RESOURCE_H

#include "gpe_basic_resource_page.h"
#include "gpe_editor_settings.h"
#include "gpe_js_compiler_settings.h"

const int objectFunctionTypes = 5; //normal, timed, collisions, user added, special functions

class objectResParentChainLink
{
public:
    int chainObjectType;
    objectResParentChainLink( int origObjectType);
    ~objectResParentChainLink();
    std::vector< int> heldChains;
    void grab_chain( objectResParentChainLink * chainIn = NULL);
    bool is_inchain(int chainId);
};


const int OBJ_MODE_COMPONENTS = 0;
const int OBJ_MODE_HEADER = 1;
const int OBJ_MODE_SOURCE = 2;

class gameEntityResource: public standardEditableGameResource
{
public:
    pawgui::widget_label_title * resource_nameLabel;

    pawgui::widget_selectbox * objModeSelector;
    pawgui::widget_checkbox * checkBoxNeedsCamera;
    pawgui::widget_checkbox * checkBoxIsMoveable;
    pawgui::widget_checkbox * checkBoxIsVisible;
    pawgui::widget_checkbox * checkBoxIsContinuous;
    pawgui::widget_text_editor * headerCodeArea;
    pawgui::widget_text_editor * sourceCodeArea;
    int animationIndex;
    int imageIndex;
    int selectedMode;
    int parentObjectId;
    gpe::animaton2d * animInEditor;
    pawgui::widget_drop_down_resource_menu * animationField;
    pawgui::widget_drop_down_resource_menu * parentObjectField;

    //Component Related Variables
    pawgui::widget_panel_list * customComponentsGuiList;
    pawgui::widget_label_text  * componentsMainGuiLabel;
    pawgui::widget_label_text  * componentsGuiLabel;
    pawgui::widget_button_push * inheritParentComponentButton;
    pawgui::widget_button_push * resetComponentsButton;
    pawgui::widget_dropdown_menu * addNewComponentDropDown;
    pawgui::widget_button_icon * removeComponentButton;
    pawgui::widget_button_icon * editCompnentButton;
    pawgui::widget_button_icon * componentSettingsButton;

    gameEntityResource(pawgui::widget_resource_container * pFolder = NULL);
    ~gameEntityResource();
    void add_component(std::string newComponentData);
    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    void compile_cpp();
    pawgui::widget_resource_container * get_parent_resource();
    gpe::animaton2d * get_resource_animation();
    bool include_local_files( std::string pBuildDir , int buildType );
    bool is_build_ready();
    void integrate_into_syntax();
    void manage_components( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    void open_code(int lineNumb, int colNumb, std::string codeTitle = "" );
    void prerender_self( );
    void load_resource(std::string file_path = "");
    void process_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);

    void save_resource(std::string file_path = "", int backupId = -1);
    int search_for_string(std::string needle);
    int search_and_replace_string(std::string needle, std::string newStr = "");
    void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

void setup_object_components( pawgui::widget_dropdown_menu * componentMenu);

#endif //GAME_ENTITY_RESOURCE_H
