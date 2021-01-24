/*
path_resource.cpp
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

#include "path_resource.h"


gamePathResource::gamePathResource(pawgui::widget_resource_container * pFolder )
{
    bottomPaneList = new pawgui::widget_panel_list();
    projectParentFolder = pFolder;

    if( projectParentFolder!=NULL)
    {
        sceneToPreview = new pawgui::widget_drop_down_resource_menu( "Previewed Scene",projectParentFolder->find_resource_from_name( gpe::resource_type_names_plural[ gpe::resource_type_scene]),-1,true );
        sceneToPreview->set_width(128);
    }
    else
    {
        sceneToPreview = NULL;
    }
    sceneZoomLevel = new pawgui::widget_dropdown_menu( "",true);
    sceneZoomLevel->add_menu_option("10%","10",10,false);
    sceneZoomLevel->add_menu_option("25%","25",25,false);
    sceneZoomLevel->add_menu_option("33%","33",33,false);
    sceneZoomLevel->add_menu_option("50%","50",50,false);
    sceneZoomLevel->add_menu_option("67%","67",67,false);
    sceneZoomLevel->add_menu_option("75%","75",75,false);
    sceneZoomLevel->add_menu_option("80%","80",80,false);
    sceneZoomLevel->add_menu_option("90%","90",90,false);
    sceneZoomLevel->add_menu_option("100%","100",100,true);
    sceneZoomLevel->add_menu_option("110%","110",110,false);
    sceneZoomLevel->add_menu_option("125%","125",125,false);
    sceneZoomLevel->add_menu_option("150%","150",150,false);
    sceneZoomLevel->add_menu_option("175%","175",175,false);
    sceneZoomLevel->add_menu_option("200%","200",200,false);
    sceneZoomLevel->add_menu_option("250%","250",250,false);
    sceneZoomLevel->add_menu_option("300%","300",300,false);
    sceneZoomLevel->add_menu_option("400%","400",400,false);
    sceneZoomLevel->add_menu_option("500%","500",500,false);
    sceneZoomLevel->set_width(96);

    pathOptions = new pawgui::widget_selectbox( "Path Points");
    pathOptions->set_width( 224 );
    pathOptions->set_height(512);
    pathOptions->limit_height(512);

    pointSettingsButtton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/cogs.png","Path Settings",-1,32);
    pointRemoveButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/remove.png", "Remove Point",-1,32 );
    pointMoveUpButtton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/arrow-up.png", "Move Point Up",-1,32);
    pointMoveDownButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/arrow-down.png","Move Point Down",-1,32 );

    pathTypeIsClosed = new pawgui::widget_checkbox("Closed Path?","Check to close the path", false );
    pathShapeType = new pawgui::widget_radio_button_controller("Path Shape");
    pathShapeType->add_menu_option("Lines","lines",0,true);
    pathShapeType->add_menu_option("Curves","curves",1,false);

    zoomValue = 1;
    editorMode = 0;
    sceneEditorView.x = 128;
    sceneEditorView.y = 32;
    sceneEditorView.w = 640;
    sceneEditorView.h = 480;

    scenePreviewRect.x = 0;
    scenePreviewRect.y = 0;
    scenePreviewRect.w = 640;
    scenePreviewRect.h = 480;

    pathLineColor = gpe::c_blue->duplicate_color();
    pathPointColor = gpe::c_orange->duplicate_color();
    sceneXScroll = new pawgui::widget_scrollbar_xaxis();
    update_rectangle(&sceneXScroll->contextRect,0,0,640,480);
    sceneYScroll = new pawgui::widget_scrollbar_yaxis();
    update_rectangle(&sceneYScroll->contextRect,0,0,640,480);
    areaIsScrollable = false;
    sceneMouseXPos = 0;
    sceneMouseYPos = 0;

    pointPos = 0;
    selectedPointPos = 0;
    selectedPathPoint = NULL;
    currentPointX = new pawgui::widget_input_number("PointX",false,0);
    currentPointY = new pawgui::widget_input_number("PointY",false,0);
    currentPointSpeed = new pawgui::widget_input_number("PointSpeed",false,0);
}

gamePathResource::~gamePathResource()
{
    if( bottomPaneList!=NULL)
    {
        delete bottomPaneList;
        bottomPaneList = NULL;
    }
    if( sceneZoomLevel!=NULL)
    {
        delete sceneZoomLevel;
        sceneZoomLevel = NULL;
    }
    if( pathOptions!=NULL)
    {
        delete pathOptions;
        pathOptions = NULL;
    }
    if( pointSettingsButtton!=NULL)
    {
        delete pointSettingsButtton;
        pointSettingsButtton = NULL;
    }

    if( pointRemoveButton!=NULL)
    {
        delete pointRemoveButton;
        pointRemoveButton = NULL;
    }
    if( pointMoveUpButtton!=NULL)
    {
        delete pointMoveUpButtton;
        pointMoveUpButtton = NULL;
    }
    if( pointMoveDownButton!=NULL)
    {
        delete pointMoveDownButton;
        pointMoveDownButton = NULL;
    }

    if( sceneToPreview!=NULL)
    {
        delete sceneToPreview;
        sceneToPreview = NULL;
    }

    if( sceneXScroll!=NULL)
    {
        delete sceneXScroll;
        sceneXScroll = NULL;
    }
    if( sceneYScroll!=NULL)
    {
        delete sceneYScroll;
        sceneYScroll = NULL;
    }
    if( pathTypeIsClosed!=NULL)
    {
        delete pathTypeIsClosed;
        pathTypeIsClosed = NULL;
    }
    clear_points();
}

gpe::game_path_point2d * gamePathResource::add_point( int pointX, int pointY, float pointSpeed )
{
    if( pathOptions !=NULL )
    {
        gpe::game_path_point2d * newPoint = new gpe::game_path_point2d(pointX, pointY, pointSpeed);
        if( pointPos >=0 && pointPos < (int)pathPoints.size() )
        {
            pathPoints.insert( pathPoints.begin()+pointPos+1,newPoint );
            pathOptions->insert_option(pointPos+1,"X:"+ stg_ex::int_to_string(pointX)+" Y:"+ stg_ex::int_to_string(pointY)+" Spd:"+ stg_ex::float_to_string(pointSpeed),NULL,NULL,-1, true );
        }
        else
        {
            pathPoints.push_back( newPoint );
            if( pathOptions!=NULL)
            {
                pathOptions->add_option("X:"+ stg_ex::int_to_string(pointX)+"Y:"+ stg_ex::int_to_string(pointY)+"Spd:"+ stg_ex::float_to_string(pointSpeed),pointPos+1,NULL,NULL,-1,true );
            }
        }
        pointPos =  pathOptions->get_selection();
    }
}

bool gamePathResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = pawgui::generate_tabs( leftTabAmount  );
        std::string html5PathName = "_path_" + get_name();

        *fileTarget << nestedTabsStr << "var " + html5PathName + " =  GPE.add_path(";
        if( pathTypeIsClosed!=NULL && pathTypeIsClosed->is_clicked() )
        {
            *fileTarget << stg_ex::int_to_string (exportBuildGlobalId ) +",true); \n";
        }
        else
        {
            *fileTarget << stg_ex::int_to_string (exportBuildGlobalId ) +",false); \n";
        }

        gpe::game_path_point2d * tempPoint = NULL;
        int pointCount = (int)pathPoints.size();
        for( int pointI = 0; pointI < pointCount; pointI++)
        {
            tempPoint = pathPoints[pointI];
            if( tempPoint!=NULL )
            {
                *fileTarget << nestedTabsStr+html5PathName+".add_original_point( ";
                *fileTarget << stg_ex::float_to_string(tempPoint->xPos)+",";
                *fileTarget << stg_ex::float_to_string(tempPoint->yPos)+",";
                *fileTarget << stg_ex::float_to_string(tempPoint->pointSpeed)+");\n";
            }
        }
        return true;
    }
    return false;
}

bool gamePathResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
    return true;
}

void gamePathResource::compile_cpp()
{

}

void gamePathResource::clear_points()
{
    selectedPointPos = -1;
    selectedPathPoint = NULL;
    gpe::game_path_point2d * tempPoint = NULL;
    for( int i = (int)pathPoints.size()-1; i >=0; i--)
    {
        tempPoint = pathPoints[i];
        if( tempPoint!=NULL)
        {
            delete tempPoint;
            tempPoint = NULL;
        }
    }
    pathPoints.clear();
    if( pathOptions!=NULL)
    {
        pathOptions->clear_list();
    }
}

bool gamePathResource::export_and_play_native( bool launchProgram )
{
    return false;
}

bool gamePathResource::get_mouse_coords( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    sceneMouseXPos = 0;
    sceneMouseYPos = 0;
    if( view_space!=NULL)
    {
        if( gpe::point_within( gpe::input->mouse_position_x, gpe::input->mouse_position_y,
                         sceneEditorView.x,
                         sceneEditorView.y,
                         sceneEditorView.x+sceneEditorView.w,
                         sceneEditorView.y+sceneEditorView.h ) )
        {
            sceneMouseXPos = ( gpe::input->mouse_position_x-sceneEditorView.x)/zoomValue +scenePreviewRect.x;
            sceneMouseYPos = ( gpe::input->mouse_position_y-sceneEditorView.y)/zoomValue +scenePreviewRect.y;
            if( spm!=NULL)
            {
                spm->mouseInScene = true;
                spm->mouseXPos = sceneMouseXPos;
                spm->mouseYPos = sceneMouseYPos;
            }
            pawgui::main_overlay_system->update_tooltip( "Mouse( "+ stg_ex::int_to_string(sceneMouseXPos )+" , "+ stg_ex::int_to_string(sceneMouseYPos)+")" );
            return true;
        }
    }
    if( spm!=NULL)
    {
        spm->mouseInScene = true;
        spm->mouseXPos = sceneMouseXPos;
        spm->mouseYPos = sceneMouseYPos;
    }
    return false;
}

void gamePathResource::handle_scrolling()
{
    bool editorPaneInUse = false;
    bool xScrollHappened = false;
    bool yScrollHappened = false;
    if( panel_main_area!=NULL )
    {
        if( panel_main_area->hasScrollControl )
        {
            return;
        }
        editorPaneInUse = panel_main_area->is_inuse();
    }
    if( areaIsScrollable  )
    {
        if( gpe::input->check_kb_down(kb_ctrl) && sceneZoomLevel!=NULL )
        {
            //Zoom In
            if( gpe::input->mouse_scrolling_down )
            {
                sceneZoomLevel->set_id(sceneZoomLevel->get_selected_id()-1 );
            }
            else if( gpe::input->mouse_scrolling_up)
            {
                //zoom out
                sceneZoomLevel->set_id(sceneZoomLevel->get_selected_id()+1 );
            }

        }
        else if( gpe::input->kb_shift_pressed)
        {
            if( gpe::input->mouse_scrolling_up > 0 )
            {
                xScrollHappened = true;
                scenePreviewRect.x-=(scenePreviewRect.w/16)*zoomValue;
            }
            else if( gpe::input->mouse_scrolling_down)
            {
                //zoom out
                xScrollHappened = true;
                scenePreviewRect.x+=(scenePreviewRect.w/16)*zoomValue;
            }
        }
        else
        {
            if( gpe::input->mouse_scrolling_up )
            {
                yScrollHappened = true;
                scenePreviewRect.y-=(scenePreviewRect.h/16)*zoomValue;
            }
            else if( gpe::input->mouse_scrolling_down)
            {
                yScrollHappened = true;
                scenePreviewRect.y+=(scenePreviewRect.h/16)*zoomValue;
            }
            else if( editorPaneInUse==false )
            {
                //arrow scrolling
                if( gpe::input->check_kb_down(kb_up) )
                {
                    yScrollHappened = true;
                    scenePreviewRect.y-=(scenePreviewRect.h/32)*zoomValue;
                }
                else if( gpe::input->check_kb_down(kb_down) )
                {
                    yScrollHappened = true;
                    scenePreviewRect.y+=(scenePreviewRect.h/32)*zoomValue;
                }
                if( gpe::input->check_kb_down(kb_left) )
                {
                    if( scenePreviewRect.x > (scenePreviewRect.w/32)*zoomValue )
                    {
                        xScrollHappened = true;
                        scenePreviewRect.x-=(scenePreviewRect.w/32)*zoomValue;
                    }
                    else
                    {
                        scenePreviewRect.x = 0;
                        xScrollHappened = true;
                    }
                }
                else if( gpe::input->check_kb_down(kb_right) )
                {
                    if( (scenePreviewRect.x +scenePreviewRect.w/32)*zoomValue < sceneRect.w*zoomValue )
                    {
                        xScrollHappened = true;
                        scenePreviewRect.x+=(scenePreviewRect.w/32)*zoomValue;
                    }
                }
            }
        }
    }

    if( scenePreviewRect.x+scenePreviewRect.w/zoomValue > sceneRect.w )
    {
        xScrollHappened = true;
        scenePreviewRect.x = sceneRect.w-scenePreviewRect.w/zoomValue;
    }

    if( scenePreviewRect.y+scenePreviewRect.h/zoomValue > sceneRect.h )
    {
        yScrollHappened = true;
        scenePreviewRect.y = sceneRect.h-scenePreviewRect.h/zoomValue;
    }

    if( scenePreviewRect.x <= 0)
    {
        xScrollHappened = true;
        scenePreviewRect.x = 0;
    }

    if( scenePreviewRect.y <= 0)
    {
        yScrollHappened = true;
        scenePreviewRect.y = 0;
    }

    if( xScrollHappened)
    {
        sceneXScroll->contextRect.x = scenePreviewRect.x;
        sceneXScroll->process_self(NULL,NULL);
    }
    if( yScrollHappened)
    {
        sceneXScroll->contextRect.y = scenePreviewRect.y;
        sceneYScroll->process_self(NULL,NULL);
    }
}

bool gamePathResource::include_local_files( std::string pBuildDir , int buildType )
{
    return true;
}


bool gamePathResource::is_build_ready()
{
    recentErrorMessage = "";
    if( (int)pathPoints.size() == 0)
    {
        recentErrorMessage = "NoPointsselected;";
        return false;
    }
    return true;
}

void gamePathResource::integrate_into_syntax()
{

}

void gamePathResource::open_code(int lineNumb, int colNumb, std::string codeTitle )
{

}

void gamePathResource::prerender_self( )
{

}

void gamePathResource::load_resource(std::string file_path )
{
    if( resourcePostProcessed ==false  || sff_ex::file_exists(file_path) )
    {
        if( pawgui::main_loader_display != NULL )
        {
            pawgui::main_loader_display->update_submessages( "Processing Path",resource_name );
        }
        std::string otherColContainerName = "";

        std::string newFileIn ="";
        std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/paths/";
        if( sff_ex::file_exists(file_path) )
        {
            newFileIn = file_path;
            soughtDir = stg_ex::get_path_from_file(newFileIn);
        }
        else
        {
            newFileIn = soughtDir + resource_name+".gpf";
        }
        std::ifstream gameResourceFileIn( newFileIn.c_str() );

        gpe::error_log->report("Loading Path - "+newFileIn);
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
                float foundFileVersion = 0;
                int foundX = 0;
                int foundY = 0;
                int foundSpeed = 0;
                int foundWaiver = 0;
                int foundR = 0, foundG = 0, foundB = 0;
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
                                equalPos = currLineToBeProcessed.find_first_of("=");
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
                                else if( keyString=="Point" || keyString=="point" )
                                {
                                    foundX = stg_ex::split_first_int(valString,',');
                                    foundY = stg_ex::split_first_int(valString,',');
                                    foundSpeed = stg_ex::split_first_int(valString,',');
                                    foundWaiver = stg_ex::split_first_int(valString,',');
                                    add_point( foundX, foundY, foundSpeed );
                                }
                                else if ( keyString == "PathIsClosed" && pathTypeIsClosed!=NULL )
                                {
                                    pathTypeIsClosed->set_checked( stg_ex::string_to_bool(valString) );
                                }
                                else if ( keyString == "pathType" && pathTypeIsClosed!=NULL )
                                {
                                    //Processes old path type from previous version
                                    if( stg_ex::split_first_string(valString, ",")=="Closed Path" )
                                    {
                                        pathTypeIsClosed->set_checked( true );
                                    }
                                    else
                                    {
                                        pathTypeIsClosed->set_checked( false);
                                    }
                                }
                                else if ( keyString == "LineColor"|| keyString=="PathLineColor")
                                {
                                    foundR = stg_ex::split_first_int(valString, ',');
                                    foundG = stg_ex::split_first_int(valString, ',');
                                    foundB = stg_ex::split_first_int(valString, ',');
                                    if( pathLineColor!=NULL)
                                    {
                                        pathLineColor->change_rgba(foundR,foundG, foundB);
                                    }
                                }

                                else if ( keyString == "PointColor"|| keyString=="PathPointColor")
                                {
                                    foundR = stg_ex::split_first_int(valString, ',');
                                    foundG = stg_ex::split_first_int(valString, ',');
                                    foundB = stg_ex::split_first_int(valString, ',');
                                    if( pathPointColor!=NULL)
                                    {
                                        pathPointColor->change_rgba(foundR,foundG, foundB);
                                    }
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

void gamePathResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    if( spm !=NULL )
    {
        spm->editMode = SCENE_MODE_NONE;
        spm->zoomValue = zoomValue;
    }
    if( projectParentFolder!=NULL)
    {
        sceneEditorView.x = view_space->x;
        sceneEditorView.y = view_space->y;
        sceneEditorView.w = scenePreviewRect.w = (int)(view_space->w - sceneYScroll->get_box_width() )-16; //xcroll = 16px height
        sceneEditorView.h = scenePreviewRect.h = view_space->h-64; //Comment pane = 64, yscroll = 16 height

        sceneRect.x = 0;
        sceneRect.y = 0;
        sceneRect.w = 8192;
        sceneRect.h = 8192;

        editorCommentPane.x =0;
        editorCommentPane.y = sceneEditorView.h+16;
        editorCommentPane.w = view_space->w;
        editorCommentPane.h = view_space->h - editorCommentPane.y;

        pawgui::widget_resource_container * sceneTypeContainer =  sceneToPreview->get_selected_container();
        if( sceneTypeContainer!=NULL && sceneTypeContainer->get_held_resource()!=NULL )
        {
            pawgui::general_resource * sceneGenObject = sceneTypeContainer->get_held_resource();
            if( sceneGenObject!=NULL)
            {
                gameSceneResource * foundSceneObject = (gameSceneResource *)sceneGenObject;
                if( foundSceneObject!=NULL)
                {
                    sceneRect.w = foundSceneObject->sceneRect.w;
                    sceneRect.h = foundSceneObject->sceneRect.h;
                }
            }
        }


        if( pathOptions !=NULL )
        {
            pathOptions->set_height( view_space->h - 160 );
            pathOptions->limit_height( view_space->h - 160 );
        }

        if(cam!=NULL && view_space!=NULL  )
        {
            //For bottom pane
            bottomPaneList->clear_list();
            bottomPaneList->set_coords( editorCommentPane.x, editorCommentPane.y );
            bottomPaneList->set_width( view_space->w );
            bottomPaneList->set_height( view_space->h - editorCommentPane.y );
            bottomPaneList->hAlign = gpe::fa_left;
            bottomPaneList->vAlign = gpe::fa_middle;
            bottomPaneList->barXPadding = pawgui::padding_default;
            bottomPaneList->barYPadding = 0;
            bottomPaneList->barXMargin  = pawgui::padding_default;
            bottomPaneList->barYMargin  = pawgui::padding_default;
            bottomPaneList->add_gui_element( sceneToPreview, false );
            bottomPaneList->add_gui_element( sceneZoomLevel, false );
            bottomPaneList->add_gui_element( pathTypeIsClosed, false );
            bottomPaneList->process_self( view_space, cam );

            if( sceneZoomLevel!=NULL)
            {
                zoomValue = sceneZoomLevel->get_selected_value();
                if( zoomValue < 0)
                {
                    zoomValue = 1;
                    sceneZoomLevel->set_option_value(100);
                }
                zoomValue/=100;
            }
            else
            {
                zoomValue = 1;
            }

            if( panel_main_area!=NULL )
            {
                //Now back to the editor pane
                panel_main_area->add_gui_element(renameBox,true);
                panel_main_area->add_gui_element(pathOptions,true);
                if( selectedPathPoint!=NULL)
                {
                    panel_main_area->add_gui_element(currentPointX,true);
                    panel_main_area->add_gui_element(currentPointY,true);
                    panel_main_area->add_gui_element(currentPointSpeed,true);
                }
                panel_main_area->add_gui_element(pointSettingsButtton,false);
                panel_main_area->add_gui_element(pointRemoveButton,false);
                panel_main_area->add_gui_element(pointMoveUpButtton,false);
                panel_main_area->add_gui_element(pointMoveDownButton,true);
                //panel_main_area->add_gui_element(pathShapeType,true);


                panel_main_area->add_gui_element(confirmResourceButton,true);
                panel_main_area->add_gui_element(cancelResourceButton,true);
                panel_main_area->process_self(NULL, NULL);

                if( pathOptions!=NULL)
                {
                    pointPos = pathOptions->get_selection();
                    if( selectedPointPos !=pointPos )
                    {
                        selectedPointPos = -1;
                        selectedPathPoint = NULL;
                    }
                }

                if( pointSettingsButtton!=NULL && pointSettingsButtton->is_clicked() )
                {
                    pawgui::context_menu_open(-1,-1,256);

                    pawgui::main_context_menu->add_menu_option("Change Points Color",1,NULL,-1,NULL,false,true);
                    pawgui::main_context_menu->add_menu_option("Change Lines Color",2,NULL,-1,NULL,false,true);
                    pawgui::main_context_menu->add_menu_option("Clear Point List",3,NULL,-1,NULL,false,true);
                    int menuSelection = pawgui::context_menu_process();
                    if( menuSelection ==1)
                    {
                        pawgui::get_color_from_popup("Change Points Color",pathPointColor);
                    }
                    else if( menuSelection ==2)
                    {
                        pawgui::get_color_from_popup("Change Points Color",pathLineColor);
                    }
                    else if( menuSelection ==3)
                    {
                        if( (int)pathPoints.size() > 0 )
                        {
                            if( (int)pathPoints.size() <=5)
                            {
                                clear_points();
                            }
                            else
                            {
                                if( pawgui::display_prompt_message("Path Clear","Are you sure you will like to clear this path?") ==pawgui::display_query_yes )
                                {
                                    clear_points();
                                }
                            }
                        }
                    }
                }
                else if( pointRemoveButton!=NULL && pointRemoveButton->is_clicked() )
                {
                    remove_point( pointPos );
                    selectedPointPos = -1;
                    selectedPathPoint = NULL;
                }
                else if( confirmResourceButton->is_clicked() )
                {
                    save_resource();
                }
                else if( cancelResourceButton->is_clicked() )
                {
                    if( pawgui::display_prompt_message("Are you sure you will like to reverse changes?","This will load in data from save-file!", true )== pawgui::display_query_yes )
                    {
                        resourcePostProcessed = false;
                        load_resource();
                    }
                }
            }
        }


        //if( panel_main_area->hasScrollControl==false && layerPaneList->hasScrollControl==false )
        {
            //Horizontal scrolling
            sceneXScroll->update_box( 0,sceneEditorView.h,sceneEditorView.w,16);
            update_rectangle(&sceneXScroll->fullRect,0,0,(float)sceneRect.w, (float)sceneRect.h );
            update_rectangle(&sceneXScroll->contextRect,(float)scenePreviewRect.x,(float)scenePreviewRect.y, (float)scenePreviewRect.w/zoomValue, (float)scenePreviewRect.h/zoomValue );
            sceneXScroll->process_self(view_space,cam );
            //if( sceneXScroll->has_moved() )
            {
                scenePreviewRect.x = (float)(sceneXScroll->contextRect.x);
            }

            //Vertical Scrolling
            sceneYScroll->update_box( sceneEditorView.w,0,16,sceneEditorView.h );
            update_rectangle(&sceneYScroll->fullRect,0,0,(float)sceneRect.w, (float)sceneRect.h );
            update_rectangle(&sceneYScroll->contextRect,(float)scenePreviewRect.x,(float)scenePreviewRect.y, (float)scenePreviewRect.w/zoomValue, (float)scenePreviewRect.h/zoomValue );
            //sceneYScroll->contextRect.h = sceneEditorView.h;
            sceneYScroll->process_self(view_space,cam );
            //if( sceneYScroll->has_moved() )
            {
                scenePreviewRect.y = float(sceneYScroll->contextRect.y);
            }
        }

        if( gpe::input->has_new_input() )
        {
            get_mouse_coords(view_space,cam);
        }
        //if( gpe::input->check_mouse_pressed(0) ||  gpe::input->check_mouse_pressed(1) || gpe::input->check_mouse_pressed(2) )
        {
            if( get_mouse_coords(view_space,cam) )
            {
                areaIsScrollable = true;
                if( panel_main_area!=NULL)
                {
                    panel_main_area->hasScrollControl = false;
                }
            }
            else
            {
                areaIsScrollable = false;
            }
        }

        int pointSize = std::max( 1, (int)( 7*zoomValue) );
        gpe::game_path_point2d * tempPoint = NULL;
        bool foundPoint  = false;
        int i = 0;
        if( gpe::input->check_mouse_down( mb_left ) )
        {
            if( get_mouse_coords(view_space,cam) )
            {
                if( sceneMouseXPos-scenePreviewRect.x < 0 )
                {
                    scenePreviewRect.x = 0;
                }

                if( sceneMouseYPos-scenePreviewRect.y < 0 )
                {
                    scenePreviewRect.y = 0;
                }
            }
            /*if(sceneYScroll->is_held() )
            {
                editorCameraRect.x+=16;
            }
            if(sceneXScroll->is_held() )
            {
                //editorCameraRect.y+=16;
            }*/
        }
        else if( gpe::input->check_mouse_released( mb_left))
        {
            //Adds, selects or moves a point
            if( get_mouse_coords(view_space,cam) )
            {
                if( selectedPathPoint!=NULL)
                {
                    selectedPathPoint->xPos = sceneMouseXPos;
                    selectedPathPoint->yPos = sceneMouseYPos;
                    if( currentPointX !=NULL)
                    {
                        currentPointX->set_number( sceneMouseXPos );
                    }
                    if( currentPointY !=NULL)
                    {
                        currentPointY->set_number( sceneMouseYPos );
                    }
                    if( pathOptions!=NULL)
                    {
                        pathOptions->rename_option( selectedPointPos, "X:"+ stg_ex::int_to_string(selectedPathPoint->xPos)+" Y:"+ stg_ex::int_to_string(selectedPathPoint->yPos)+" Spd:"+ stg_ex::float_to_string(selectedPathPoint->pointSpeed) );
                    }
                }
                else
                {
                    //searches if the mouse overlaps with a point on the screen.
                    for( i = (int)pathPoints.size()-1; i >=0 && !foundPoint; i--)
                    {
                        tempPoint = pathPoints[i];
                        if( tempPoint!=NULL)
                        {
                            // if found change its coordinates
                            if( gpe::point_between( (int)sceneMouseXPos,(int)sceneMouseYPos, tempPoint->xPos -pointSize,tempPoint->yPos - pointSize,tempPoint->xPos +pointSize,tempPoint->yPos +pointSize) )
                            {
                                foundPoint = true;
                                selectedPathPoint = tempPoint;
                                if( currentPointX !=NULL)
                                {
                                    currentPointX->set_number( selectedPathPoint->xPos );
                                }
                                if( currentPointY !=NULL)
                                {
                                    currentPointY->set_number( selectedPathPoint->yPos );
                                }
                                if( currentPointSpeed !=NULL)
                                {
                                    currentPointSpeed->set_number( selectedPathPoint->pointSpeed );
                                }
                                pointPos = i;
                                selectedPointPos = i;
                                if( pathOptions!=NULL)
                                {
                                    pathOptions->set_selection( i);
                                }
                            }
                        }
                    }

                    if( selectedPathPoint == NULL)
                    {
                        pointPos = pathOptions->get_selection();
                        gpe::game_path_point2d * newPoint = add_point(sceneMouseXPos, sceneMouseYPos, 1);
                    }
                }
            }
        }
        else if( gpe::input->check_mouse_released(1) )
        {
            //removes points
            if( get_mouse_coords(view_space,cam) )
            {
                for( i = (int)pathPoints.size()-1; i >=0 && !foundPoint; i--)
                {
                    tempPoint = pathPoints[i];
                    if( tempPoint!=NULL)
                    {
                        if( gpe::point_between( (int)sceneMouseXPos,(int)sceneMouseYPos, tempPoint->xPos -pointSize,tempPoint->yPos - pointSize,tempPoint->xPos +pointSize,tempPoint->yPos +pointSize) )
                        {
                            foundPoint = true;
                            remove_point( i );
                        }
                    }
                }
            }
            selectedPathPoint = NULL;
            selectedPointPos = -1;
        }
        else if( selectedPathPoint!=NULL)
        {
            bool foundPointChange = false;
            float foundNumber = 0;
            if( currentPointX!=NULL && currentPointX->is_valid() )
            {
                foundNumber = currentPointX->get_held_number();
                if( foundNumber >=0 && foundNumber!=selectedPathPoint->xPos)
                {
                    selectedPathPoint->xPos = foundNumber;
                    foundPointChange = true;
                }
            }

            if( currentPointX!=NULL && currentPointX->is_valid() )
            {
                foundNumber = currentPointY->get_held_number();
                if( foundNumber >=0 && foundNumber!=selectedPathPoint->yPos)
                {
                    selectedPathPoint->yPos = foundNumber;
                    foundPointChange = true;
                }
            }

            if( pathOptions!=NULL && foundPointChange)
            {
                pathOptions->rename_option( selectedPointPos, "X:"+ stg_ex::int_to_string(selectedPathPoint->xPos)+" Y:"+ stg_ex::int_to_string(selectedPathPoint->yPos)+" Spd:"+ stg_ex::float_to_string( selectedPathPoint->pointSpeed) );
            }
        }
        handle_scrolling();
    }
}

bool gamePathResource::remove_point( int pointId )
{
    if( pointId >=0 && pointId < (int)pathPoints.size() )
    {
        gpe::game_path_point2d * tempPoint = pathPoints[pointId];
        if( tempPoint!=NULL )
        {
            delete tempPoint;
            tempPoint = NULL;
        }
        pathPoints.erase(pathPoints.begin()+ pointId );
        if( pathOptions!=NULL )
        {
            pathOptions->remove_option( pointId );
        }

        if( pointPos >=pointId)
        {
            pointId-=1;
        }
        pointPos--;
        if( pointPos < 0)
        {
            pointPos = 0;
        }

        selectedPathPoint = NULL;
        selectedPointPos = -1;
        return true;

    }
    return false;
}

void gamePathResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    gpe::renderer_main->reset_viewpoint(  );
    gpe::renderer_main->set_viewpoint( &sceneEditorView );
    if( cam!=NULL && view_space!=NULL)
    {
        pawgui::widget_resource_container * sceneTypeContainer =  sceneToPreview->get_selected_container();
        if( sceneTypeContainer!=NULL && sceneTypeContainer->get_held_resource()!=NULL )
        {
            pawgui::general_resource * sceneGenObject = sceneTypeContainer->get_held_resource();
            if( sceneGenObject!=NULL)
            {
                gameSceneResource * foundSceneObject = (gameSceneResource *)sceneGenObject;
                if( foundSceneObject!=NULL)
                {

                    foundSceneObject->render_scene_layers( view_space, cam, &scenePreviewRect,zoomValue,false, false);
                }
            }
        }

        //Draws the points and lines
        gpe::game_path_point2d * tempPoint = NULL;
        int tempXPoint = 0, tempYPoint = 0;
        int tempX2Point = 0, tempY2Point = 0;
        int pointI = 0;
        int pointCount = (int)pathPoints.size();
        for( pointI = 0; pointI < pointCount; pointI++)
        {
            tempPoint = pathPoints[pointI];
            if( tempPoint!=NULL )
            {
                tempXPoint = floor( tempPoint->xPos*zoomValue - scenePreviewRect.x*zoomValue );
                tempYPoint = floor( tempPoint->yPos*zoomValue - scenePreviewRect.y*zoomValue );
                if( pointI >=1)
                {
                    gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathLineColor, 255);
                }
                tempX2Point = tempXPoint;
                tempY2Point = tempYPoint;
            }
        }

        if( pathTypeIsClosed!=NULL && pointCount >2)
        {
            tempPoint = pathPoints[0];
            if( tempPoint!=NULL && pathTypeIsClosed->is_clicked() )
            {
                tempXPoint = floor( tempPoint->xPos*zoomValue - scenePreviewRect.x*zoomValue );
                tempYPoint = floor( tempPoint->yPos*zoomValue - scenePreviewRect.y*zoomValue );
                gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathLineColor, 255);
            }
        }

        for( pointI = 0; pointI < (int)pathPoints.size(); pointI++)
        {
            tempPoint = pathPoints[pointI];
            if( tempPoint!=NULL )
            {
                tempXPoint = floor( tempPoint->xPos*zoomValue - scenePreviewRect.x*zoomValue );
                tempYPoint = floor( tempPoint->yPos*zoomValue - scenePreviewRect.y*zoomValue );
                if( selectedPointPos == pointI)
                {
                    gpe::gcanvas->render_circle_color( tempXPoint,tempYPoint, std::max( 1, (int)( 7*zoomValue) ),pathLineColor, 255 );
                }
                else
                {
                    gpe::gcanvas->render_circle_color( tempXPoint,tempYPoint, std::max( 1, (int)( 7*zoomValue) ),pathPointColor, 255 );
                }
                //gpe::gcanvas->render_rectangle( tempXPoint, tempYPoint, tempXPoint+4, tempYPoint+4,pathPointColor, false, 255);
                //gpe::error_log->report("Rendering point ["+ stg_ex::int_to_string(pointI)+"] at ("+ stg_ex::int_to_string(tempXPoint)+","+ stg_ex::int_to_string(tempYPoint)+")");
            }
        }

        if( selectedPathPoint!=NULL)
        {
            tempXPoint = floor( sceneMouseXPos*zoomValue - scenePreviewRect.x*zoomValue );
            tempYPoint = floor( sceneMouseYPos*zoomValue - scenePreviewRect.y*zoomValue );
            gpe::gcanvas->render_circle_color( tempXPoint,tempYPoint, std::max( 1, (int)( 7*zoomValue) ),pathLineColor, 128 );

            if( selectedPointPos >=0 && selectedPointPos < (int)pathPoints.size() && pointCount >=2 )
            {
                if( selectedPointPos == 0)
                {
                    tempPoint = pathPoints[1];
                    if( tempPoint!=NULL )
                    {
                        tempX2Point = floor( tempPoint->xPos*zoomValue - scenePreviewRect.x*zoomValue );
                        tempY2Point = floor( tempPoint->yPos*zoomValue - scenePreviewRect.y*zoomValue );
                        gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                    }

                    if( pathTypeIsClosed!=NULL && pathTypeIsClosed->is_clicked()  && pointCount >=3 )
                    {
                        tempPoint = pathPoints[ pointCount - 1];
                        if( tempPoint!=NULL)
                        {
                            tempX2Point = floor( tempPoint->xPos*zoomValue - scenePreviewRect.x*zoomValue );
                            tempY2Point = floor( tempPoint->yPos*zoomValue - scenePreviewRect.y*zoomValue );
                            gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                        }
                    }
                }
                else if( selectedPointPos == pointCount-1 )
                {
                    if( pointCount >2 )
                    {
                        tempPoint = pathPoints[ selectedPointPos -1];
                        if( tempPoint!=NULL )
                        {
                            tempX2Point = floor( tempPoint->xPos*zoomValue - scenePreviewRect.x*zoomValue );
                            tempY2Point = floor( tempPoint->yPos*zoomValue - scenePreviewRect.y*zoomValue );
                            gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                        }

                        if( pathTypeIsClosed!=NULL && pathTypeIsClosed->is_clicked()  && pointCount >=3 )
                        {
                            tempPoint = pathPoints[ 0];
                            if( tempPoint!=NULL)
                            {
                                tempX2Point = floor( tempPoint->xPos*zoomValue - scenePreviewRect.x*zoomValue );
                                tempY2Point = floor( tempPoint->yPos*zoomValue - scenePreviewRect.y*zoomValue );
                                gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                            }
                        }
                    }
                }
                else
                {
                    tempPoint = pathPoints[ selectedPointPos-1];
                    if( tempPoint!=NULL)
                    {
                        tempX2Point = floor( tempPoint->xPos*zoomValue - scenePreviewRect.x*zoomValue );
                        tempY2Point = floor( tempPoint->yPos*zoomValue - scenePreviewRect.y*zoomValue );
                        gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                    }

                    tempPoint = pathPoints[ selectedPointPos+1];
                    if( tempPoint!=NULL)
                    {
                        tempX2Point = floor( tempPoint->xPos*zoomValue - scenePreviewRect.x*zoomValue );
                        tempY2Point = floor( tempPoint->yPos*zoomValue - scenePreviewRect.y*zoomValue );
                        gpe::gcanvas->render_line_color(  tempXPoint, tempYPoint, tempX2Point, tempY2Point, pathPointColor, 128);
                    }
                }
            }
        }
        //"Editor View:("+ stg_ex::int_to_string(editorCameraRect.x)+","+ stg_ex::int_to_string(editorCameraRect.y)+","+ stg_ex::int_to_string(editorCameraRect.w)+","+ stg_ex::int_to_string(editorCameraRect.h)+")",

        gpe::renderer_main->reset_viewpoint(  );
        gpe::renderer_main->set_viewpoint( view_space );
        gpe::gcanvas->render_rect( &editorCommentPane,pawgui::theme_main->program_color,false);
        //gpe::gcanvas->render_rectangle( sceneYScroll->widget_box.x+sceneYScroll->widget_box.w,0,view_space->w,view_space->h,pawgui::theme_main->program_color,false);
        //gpe::gcanvas->render_rectangle( sceneXScroll->widget_box.x,sceneXScroll->widget_box.y,view_space->w,view_space->h,pawgui::theme_main->program_color,false);
        if( sceneXScroll!=NULL)
        {
            sceneXScroll->render_self( view_space,cam);
        }
        if( sceneYScroll!=NULL)
        {
            sceneYScroll->render_self( view_space,cam);
        }
        //gpe::gcanvas->render_rect( &editorPane,pawgui::theme_main->program_color,false);
    }

    if( bottomPaneList!=NULL )
    {
        bottomPaneList->render_self(  view_space, cam );
    }
}

void gamePathResource::save_resource(std::string file_path, int backupId )
{
    if( pawgui::main_loader_display != NULL )
    {
        pawgui::main_loader_display->update_submessages( "Saving Path",resource_name );
    }

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
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/paths/";
        newFileOut = soughtDir + resource_name+".gpf";
    }
    std::ofstream newSaveDataFile( newFileOut.c_str() );
    //If the font file could be saved
    if( !newSaveDataFile.fail() && newSaveDataFile.is_open() )
    {
        write_header_on_file(&newSaveDataFile);
        newSaveDataFile << "#     \n";
        if( sceneToPreview!=NULL)
        {
            newSaveDataFile << "RelevantScene=" << sceneToPreview->get_selected_name() << "\n";
        }
        else
        {
            newSaveDataFile << "RelevantScene=-1\n";
        }

        if( pathTypeIsClosed!=NULL)
        {
            newSaveDataFile << "PathIsClosed="+ stg_ex::int_to_string( pathTypeIsClosed->is_clicked() )+"\n";
        }

        if( pathLineColor!=NULL)
        {
            newSaveDataFile << "LineColor="+ stg_ex::int_to_string( (int)pathLineColor->get_r() )+","+ stg_ex::int_to_string( (int)pathLineColor->get_g() )+","+ stg_ex::int_to_string( (int)pathLineColor->get_b() )+",\n";
        }

        if( pathPointColor!=NULL)
        {
            newSaveDataFile << "PointColor="+ stg_ex::int_to_string( (int)pathPointColor->get_r() )+","+ stg_ex::int_to_string( (int)pathPointColor->get_g() )+","+ stg_ex::int_to_string( (int)pathPointColor->get_b() )+",\n";
        }
        std::string resFileLocation = "";
        std::string resFileCopySrc;
        std::string resFileCopyDest;
        gpe::game_path_point2d * tempPoint = NULL;
        for( int i = 0; i < (int)pathPoints.size(); i++)
        {
            tempPoint = pathPoints[i];
            if( tempPoint!=NULL )
            {
                newSaveDataFile << "Point="+ stg_ex::int_to_string( tempPoint->xPos )+","+ stg_ex::int_to_string( tempPoint->yPos )+","+ stg_ex::float_to_string( tempPoint->pointSpeed )+",\n";
            }
        }

        newSaveDataFile.close();
        if( !usingAltSaveSource)
        {
            isModified = false;
        }
        if( pawgui::main_loader_display != NULL )
        {
            pawgui::main_loader_display->increment_and_update( "Path Successfully Saved!",resource_name );
        }
        return;
    }

    main_editor_log->log_general_error("Unable to save to file ["+newFileOut+"]");
    if( pawgui::main_loader_display != NULL )
    {
        pawgui::main_loader_display->increment_and_update( "Unable to save to file",resource_name );
    }
}


int gamePathResource::search_for_string(std::string needle)
{
    return 0;
}

int gamePathResource::search_and_replace_string(std::string needle, std::string newStr )
{
    return 0;
}

void gamePathResource::update_project_layers()
{

}

bool gamePathResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "Path=" << resource_name << "," << get_global_rid() << ",";

            *fileTarget << "\n";
            //save_resource();
            return true;
        }
    }
    return false;
}
