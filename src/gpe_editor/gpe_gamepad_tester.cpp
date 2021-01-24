/*
gpe_gamepad_tester.cpp
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
#include "gpe_gamepad_tester.h"

gamePencilgamepadTesterResource * main_gamepad_tester = NULL;


gamePencilgamepadTesterResource::gamePencilgamepadTesterResource()
{
    playerRadiusMin = 32;
    playerRadiusMax = 1024;
    for( int iController = 0; iController < gp_max_devices; iController++)
    {
        playerDeadZone[iController] = 0.25;
        playerRadius[iController] = 64;
        playerX[iController] = playerRadius[iController];
        playerY[iController] = playerRadius[iController];
        playerTestColor[iController]  = new pawgui::gpe_widget_color_picker("Player Color","Change to your favorite color",228,32,32 );
        playerFontColor[iController]  = new pawgui::gpe_widget_color_picker("Player Font Color","Change to your favorite color",228,228,228 );
        playerDeadZoneField[iController]  = new pawgui::widget_input_number("Joysticks DeadZone(From 0.0 to 1.0)",false,0, 1);
        playerDeadZoneField[iController] ->set_number( playerDeadZone[iController] );
        playerSizeRadius[iController]  = new pawgui::widget_input_number("Radius(in pixels)",true,playerRadiusMin, playerRadiusMax);
        playerSizeRadius[iController] ->set_number( playerRadius[iController] );

    }
    resourceFileName = "";
    resource_name = "gamepad Tester";
    resourceType = -8;
    subResouceId = -8;
    controllerInView = 0;
    detectControllersButton = new pawgui::widget_button_push("","Detect Controllers","Click to rediscover all controllers");
    vibrateControllerButton = new pawgui::widget_button_push("","Vibrate Controller","Click to vibrate current controller");
    previousControllerButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/arrow-left.png","Previous Controller");
    nextControllerButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/arrow-right.png","Next Controller");
    conrollerIdField  = new pawgui::widget_input_number("",true,0, gp_max_devices );
    conrollerIdField->set_string("0");
    conrollerIdField->set_width(64);

    controllerViewTitleLabel = new pawgui::widget_label_title("Current Controller:","Current Controller:");
    controllerNameLabel = new pawgui::widget_label_text ("...","XYZ Controller....");
    requireInputToConnect = new pawgui::widget_checkbox("Input required for connection","Delay controller connection state til a button is pressed(Useful for adapters)", gpe::input->gamepad_requires_input );
    showControllerInfo = new pawgui::widget_checkbox("Show Debugger","Check to see controller info overlay",true );
    playerStats = new pawgui::widget_label_title("Player Stats","Player Stats");

}

gamePencilgamepadTesterResource::~gamePencilgamepadTesterResource()
{
    if( detectControllersButton != NULL)
    {
        delete detectControllersButton;
        detectControllersButton = NULL;
    }

    if( vibrateControllerButton != NULL)
    {
        delete vibrateControllerButton;
        vibrateControllerButton = NULL;
    }

    if( previousControllerButton != NULL)
    {
        delete previousControllerButton;
        previousControllerButton = NULL;
    }

    if( nextControllerButton != NULL)
    {
        delete nextControllerButton;
        nextControllerButton = NULL;
    }

    if( controllerViewTitleLabel != NULL)
    {
        delete controllerViewTitleLabel;
        controllerViewTitleLabel = NULL;
    }
    if( controllerNameLabel != NULL)
    {
        delete controllerNameLabel;
        controllerNameLabel = NULL;
    }
}

bool gamePencilgamepadTesterResource::include_local_files( std::string pBuildDir , int buildType )
{

}

void gamePencilgamepadTesterResource::prerender_self( )
{

}

void gamePencilgamepadTesterResource::load_resource(std::string file_path)
{

}

void gamePencilgamepadTesterResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    view_space = gpe::camera_find( view_space);
    cam = gpe::camera_find( cam );

    controllerNameLabel->set_name( gpe::input->gamepad_detected_name( controllerInView ) );
    if( panel_main_area!=NULL )
    {
        panel_main_area->clear_panel();

        panel_main_area->add_gui_element(showControllerInfo,true);
        panel_main_area->add_gui_element(detectControllersButton,true);
        panel_main_area->add_gui_element(requireInputToConnect,true);
        panel_main_area->add_gui_element(controllerViewTitleLabel,true);
        panel_main_area->add_gui_element(controllerNameLabel,true);
        panel_main_area->add_gui_element(previousControllerButton,false);
        panel_main_area->add_gui_element(conrollerIdField,false);
        panel_main_area->add_gui_element(nextControllerButton,true);
        panel_main_area->add_gui_element(playerStats,true);
        panel_main_area->add_gui_element(playerTestColor[controllerInView],true);
        panel_main_area->add_gui_element(playerFontColor[controllerInView],true);
        panel_main_area->add_gui_element(playerSizeRadius[controllerInView],true);
        panel_main_area->add_gui_element(playerDeadZoneField[controllerInView],true);
        panel_main_area->process_self();

        gpe::input->gamepad_requires_input = requireInputToConnect->is_clicked();
        if( detectControllersButton->is_clicked() )
        {
            gpe::input->gamepad_detect_all( );
        }
        if( previousControllerButton->is_clicked() )
        {
            controllerInView--;
            if( controllerInView < 0 )
            {
                controllerInView = 0;
            }
            conrollerIdField->set_number( controllerInView );
        }
        else if( nextControllerButton->is_clicked() )
        {
            controllerInView++;
            if( controllerInView >= gp_max_devices )
            {
                controllerInView = gp_max_devices - 1;
            }
            conrollerIdField->set_number( controllerInView );
        }
        controllerInView = conrollerIdField->get_held_int();
    }
    playerRadiusMax = std::min( view_space->w , view_space->h )/2;



    for( int iController = 0; iController < gp_max_devices; iController++)
    {
        playerRadius[iController] = std::max( playerRadiusMin, playerSizeRadius[iController]->get_held_int() );
        playerRadius[iController] = std::min( playerRadiusMax, playerRadius[iController] );

        playerDeadZone[iController] = playerDeadZoneField[iController]->get_held_number() ;
        if(playerDeadZone[iController] > 1.f )
        {
            playerDeadZone[iController] = 1.f;
        }

        if(playerDeadZone[iController] < 0.f )
        {
            playerDeadZone[iController] = 0.f;
        }

        if( gpe::input->gamepad_get_axis_value( iController, 0) < -playerDeadZone[iController] )
        {
            playerX[iController] -= 2;
        }
        else if( gpe::input->gamepad_get_axis_value( iController, 0) > playerDeadZone[iController]  )
        {
            playerX[iController] += 2;
        }

        if( gpe::input->gamepad_get_axis_value( iController, 1) < -playerDeadZone[iController] )
        {
            playerY[iController] -= 2;
        }
        else if( gpe::input->gamepad_get_axis_value( iController, 1) > playerDeadZone[iController] )
        {
            playerY[iController] += 2;
        }

        if( playerX[iController] > view_space->w - playerRadius[iController])
        {
            playerX[iController] = view_space->w - playerRadius[iController];
        }

        if( playerY[iController] > view_space->h - playerRadius[iController])
        {
            playerY[iController] = view_space->h - playerRadius[iController];
        }

        if( playerX[iController] < playerRadius[iController])
        {
            playerX[iController] = playerRadius[iController];
        }
        if( playerY[iController] < playerRadius[iController])
        {
            playerY[iController] = playerRadius[iController];
        }
    }
}

void gamePencilgamepadTesterResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{

    view_space = gpe::camera_find( view_space);
    cam = gpe::camera_find( cam );

    gpe::gcanvas->render_rectangle( 0,0,view_space->w, view_space->h, pawgui::theme_main->program_color, false, 255 );
    for( int iController = gp_max_devices - 1; iController >=0 ; iController-- )
    {
        if( gpe::input->gamepad_connected( iController ) )
        {
            gpe::gcanvas->render_circle_color( playerX[iController], playerY[iController], playerRadius[iController],playerTestColor[iController]->get_color(), 255, false );
            gpe::gfs->render_text( playerX[ iController], playerY[iController], +"[ Player "+ stg_ex::int_to_string(iController)+" ]",playerFontColor[iController]->get_color(), gpe::font_default, gpe::fa_center, gpe::fa_middle, 255);
        }
    }

    if( showControllerInfo!=NULL && showControllerInfo->is_clicked() )
    {
        std::string controllerOutputString = "Controller ["+ stg_ex::int_to_string( controllerInView)+"]";

        if( gpe::input->gamepad_connected( controllerInView ) )
        {
            controllerOutputString+= " is connected";
            gpe::gfs->render_text( 32, 32, controllerOutputString, gpe::c_lime,gpe::font_default, gpe::fa_left, gpe::fa_bottom, 255);

            controllerOutputString = "Buttons: ";
            for(int iCButton = 0; iCButton < gp_button_count; iCButton++)
            {
                controllerOutputString+= "["+ stg_ex::int_to_string( gpe::input->gamepad_check_button_down( controllerInView, iCButton) ) +"], ";
            }
            gpe::gfs->render_text( 32, 64, controllerOutputString,pawgui::theme_main->main_box_font_color,gpe::font_default, gpe::fa_left, gpe::fa_bottom, 255);

            controllerOutputString = "Axis: ";
            for(int iCAxis = 0; iCAxis < gp_axes_count; iCAxis++)
            {
                controllerOutputString+= "["+ stg_ex::float_to_string( gpe::input->gamepad_get_axis_value( controllerInView, iCAxis) ) +"], ";
            }
            gpe::gfs->render_text( 32, 96, controllerOutputString,pawgui::theme_main->main_box_font_color,gpe::font_default, gpe::fa_left, gpe::fa_bottom, 255);

        }
        else
        {
            controllerOutputString+= " is NOT connected";
            gpe::gfs->render_text( 32, 32, controllerOutputString,gpe::c_red,gpe::font_default, gpe::fa_left, gpe::fa_bottom, 255);

        }
    }


    if( gpe::input->gamepad_count() > 0 )
    {
        gpe::gfs->render_text( view_space->w/2, view_space->h-64, "Viewing Controller ["+ stg_ex::int_to_string(controllerInView+1)+"] out of ["+ stg_ex::int_to_string(gpe::input->gamepad_count() )+"]",pawgui::theme_main->main_box_font_color,gpe::font_default, gpe::fa_center, gpe::fa_bottom, 255);
        gpe::gfs->render_text( view_space->w/2, view_space->h-32, gpe::input->game_pads[controllerInView]->get_name(),pawgui::theme_main->main_box_font_color,gpe::font_default, gpe::fa_center, gpe::fa_bottom, 255);
    }
    else
    {
        gpe::gfs->render_text( view_space->w/2, view_space->h-64, "No game controllers detected!",pawgui::theme_main->main_box_font_color,gpe::font_default, gpe::fa_center, gpe::fa_bottom, 255);
        gpe::gfs->render_text( view_space->w/2, view_space->h-32, "Experimental gamepad Tester...",pawgui::theme_main->main_box_font_color,gpe::font_default, gpe::fa_center, gpe::fa_bottom, 255);

    }
}

void gamePencilgamepadTesterResource::save_resource(std::string file_path, int backupId )
{

}

bool gamePencilgamepadTesterResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{

}

