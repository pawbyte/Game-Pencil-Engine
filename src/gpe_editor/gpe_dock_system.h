/*
gpe_dock_system.h
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


#ifndef GPE_DOCK_SYSTEM_H
#define GPE_DOCK_SYSTEM_H

#include "../pawgui/pawgui.h"
#include "../pawgui/pawgui_resource_dropdown.h"
#include "../pawgui/pawgui_general_resource.h"
#include "gpe_log_manager.h"

class gpeEditorDockPanel: public pawgui::widget_basic
{
private:
    pawgui::widget_tabbar * panelBar;
    gpe::shape_rect * panelRect;
public:
    pawgui::widget_basic * specialPanelElement;
    pawgui::widget_button_icon * dockSettingsButton;
    bool justResized;
    bool beingResized;
    bool menuResized;
    bool wasProcessed;
    pawgui::widget_panel_list * panelGuiList;
    gpe::shape_rect * containerRect;
    gpeEditorDockPanel();
    ~gpeEditorDockPanel();
    bool add_container( std::string name, bool openNew = false );
    void add_gui_element(pawgui::widget_basic * element, bool isNLElement = true );
    void add_gui_auto(pawgui::widget_basic * element);
    void add_gui_element_fullsize(pawgui::widget_basic * element);
    void clear_panel();
    bool container_in_view( std::string name );
    int get_panel_height();
    std::string get_selected_container();
    bool has_content();
    void process_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL );
    bool save_panel_data_to_file(std::ofstream * fileTarget);
    void setup_panel( bool addSpacing = true, bool clearList = true );
    bool remove_container( std::string name );
    void render_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL );
    void reset_panel();
};

extern gpeEditorDockPanel * panel_resource_tree;
extern gpeEditorDockPanel * panel_main_area;
extern gpeEditorDockPanel * panel_inspector;
extern gpeEditorDockPanel * panel_meta;

const int DOCK_TOP_LEFT = 0;
const int DOCK_TOP_LEFT_CENTER = 1;
const int DOCK_TOP_MIDDLE = 2;
const int DOCK_TOP_RIGHT_CENTER = 3;
const int DOCK_TOP_RIGHT = 4;

const int DOCK_BOTTOM_LEFT = 5;
const int DOCK_BOTTOM_LEFT_CENTER = 6;
const int DOCK_BOTTOM_MIDDLE = 7;
const int DOCK_BOTTOM_RIGHT_CENTER = 8;
const int DOCK_BOTTOM_RIGHT = 9;

const int DOCK_MAX_COLUMN_COUNT = 5;
const int DOCK_MAX_ROW_COUNT = 2;
const int DOCK_MAX_PANEL_COUNT = DOCK_MAX_COLUMN_COUNT * DOCK_MAX_ROW_COUNT;

class gpeEditorDock: public pawgui::widget_basic
{
private:
    bool isHidden;
    std::vector< gpe::key_pair * > defaultPanels;
    int panelColumnWidth[ DOCK_MAX_COLUMN_COUNT ];
    float panelWidthPercentages[ DOCK_MAX_COLUMN_COUNT ];
    float panelHeightPercentages[ DOCK_MAX_PANEL_COUNT ];
public:
    int dockVerticalPadding;
    int dockHorizontalPadding;
    float dockMinimumColumnPercentage;
    int currentColumnCount;
    float dockWidthMinusColumnPadding;
    pawgui::popup_menu_option * toolbarOptonsHolder;
    bool justResized;
    bool beingResized;
    int horiResizeStartX;
    int resizePanelX1, resizePanelX2;

    bool horizontalResize;
    bool verticalResize;



    //Reside related Data [ panelHeightPercentagesSTART ]
    int columnBeingResizedId;

    gpeEditorDockPanel * panels[DOCK_MAX_PANEL_COUNT ];
    std::string defaultPanelNames[DOCK_MAX_PANEL_COUNT ];

    gpeEditorDock();
    ~gpeEditorDock();
    void add_to_panel(  std::string name, int panelId = 0,  bool open = true, bool saveToSettings = false );
    void add_default_panel(  std::string name, int panelId = 0,  bool open = true , bool saveToSettings = false );
    void clear_all_panels();
    void clear_defaults();
    bool container_in_view( std::string name );
    gpeEditorDockPanel * find_panel( std::string name );

    int get_column(int panelId);
    int get_dock_left_width();
    int get_dock_right_width();

    int handle_resizing();
    void hide_dock();
    bool is_hidden();
    bool load_dock_setings( std::string filename );
    void process_dock( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL );
    void process_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL );
    void remove_panel( std::string name );
    void remove_default_panel( std::string name );
    void render_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL );
    void reset_dock();
    bool save_dock_setings( std::string filename );
    void setup_dock();
    void toggle_default_pane( std::string name );
    void toggle_panel(  std::string name, int panelId = 0,  bool open = true );
    void unhide_dock();
    void update_toolbar();
};

extern gpeEditorDock * GPE_DOCK;

#endif
