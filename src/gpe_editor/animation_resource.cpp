/*
animation_resource.cpp
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

#include "animation_resource.h"
#include "gpe_editor_settings.h"

std::string animaton2d_LABELS[ANIMATION_DATA_FIELD_COUNT];

animationResource::animationResource(pawgui::widget_resource_container * pFolder)
{
    autoAnimationPos = 0;
    animationTrackBar = new pawgui::widget_slide_xaxis();

    animatonPreviewRect = new gpe::shape_rect(0,0,0,0);
    animCameraRect.x = animRect.x = 0;
    animCameraRect.y = animRect.y = 0;
    animCameraRect.w = animRect.w = 0;
    animCameraRect.h = animRect.h = 0;
    areaIsScrollable = false;
    animationPreviewCam = new gpe::shape_rect();
    animationSpeed = 1;
    editorZoomValue = 1;
    minZoomValue = 0.0625;
    maxZoomValue = 16;

    projectParentFolder = pFolder;
    animInEditor = new gpe::animaton2d( gpe::rph->get_default_render_package() );
    animationSpeedLabel = new pawgui::widget_label_text ("Preview Speed","Preview Speed");
    labelanimationDimensions = new pawgui::widget_label_text ("animation Dimensions");
    previewZoomLevel = new pawgui::widget_checkbox("Preview Zoom Level","Preview Zoom Level", true);
    previewSubImageNumbers = new pawgui::widget_checkbox("Sub-images","Sub-images", true);

    labelFrameInfo = new pawgui::widget_label_text ("Frame Info");
    labelanimationMessage = new pawgui::widget_label_text ("animation Message");
    //animation Collision Area
    preloadCheckBox = new pawgui::widget_checkbox("Preload animation","Check to load animation at game open", true);
    showCollisionShapeCheckBox = new pawgui::widget_checkbox("Preview Collision Shape","", true);
    showAnimationCheckBox = new pawgui::widget_checkbox("Preview animation Animation","", true);
    animationCollisionShapeMenu = new pawgui::widget_dropdown_menu( "Collision Shape",true);
    animationCollisionShapeMenu->add_menu_option("Rectangle","rectangle",0,true);
    animationCollisionShapeMenu->add_menu_option("Circle","circle",1,false);
    //animationCollisionShapeMenu->add_menu_option("Triangle [ Coming Soon]");
    //animationCollisionShapeMenu->add_menu_option("Diamond [ Coming Soon]");
    //animationCollisionShapeMenu->add_menu_option("Hexagon [ Coming Soon]");

    animationCenterCollisionButton = new pawgui::widget_button_label( "Center Collision Shape","Centers the collision box");
    animationCollisionRectX = new pawgui::widget_input_number("0",true,0);
    animationCollisionRectX->set_label("X-Pos");
    animationCollisionRectX->set_string("0");
    animationCollisionRectX->scale_width(0.5);

    animationCollisionRectY = new pawgui::widget_input_number("0",true,0);
    animationCollisionRectY->set_label("Y-Pos");
    animationCollisionRectY->set_string("0");
    animationCollisionRectY->scale_width(0.5);

    animationCollisionRectW = new pawgui::widget_input_number("0",true,0);
    animationCollisionRectW->set_label("Width");
    animationCollisionRectW->set_string("0");
    animationCollisionRectW->scale_width(0.5);

    animationCollisionRectH = new pawgui::widget_input_number("0",true,0);
    animationCollisionRectH->set_label("Height");
    animationCollisionRectH->set_string("0");
    animationCollisionRectH->scale_width(0.5);

    animationCollisionCircleX = new pawgui::widget_input_number("0",true,0);
    animationCollisionCircleX->set_label("X-Pos");
    animationCollisionCircleX->set_string("0");

    animationCollisionCircleY = new pawgui::widget_input_number("0",true,0);
    animationCollisionCircleY->set_label("Y-Pos");
    animationCollisionCircleY->set_string("0");

    animationCollisionCircleR = new pawgui::widget_input_number("0",true,0);
    animationCollisionCircleR->set_label("Radius");
    animationCollisionCircleR->set_string("0");
    int i = 0;

    animationDataFields[i] = new pawgui::widget_input_number("");
    //pawgui::padding_default*2+animationDataLabelWidth,pawgui::padding_default+pawgui::default_line_height_padded*(i+1),64,pawgui::default_line_height,"0",false,INT_MIN,INT_MAX);
    animationDataFields[i]->set_string("0");
    animationDataFields[i]->set_label(animaton2d_LABELS[i]);

    for( i =1; i < ANIMATION_DATA_FIELD_COUNT; i++)
    {
        animationDataFields[i] = new pawgui::widget_input_number("");
        //pawgui::padding_default*2+animationDataLabelWidth,pawgui::padding_default+pawgui::default_line_height_padded*(i+1),64,pawgui::default_line_height,"0",true,0,512);
        animationDataFields[i]->set_string("0");
        animationDataFields[i]->set_label(animaton2d_LABELS[i]);
    }
    editResourceButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/edit.png","Edit animation Data",-1);
    transformResourceButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/magic.png","Transform the Image",-1);
    playPauseResourceButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png","Stop",-1);
    subImagePreviewNumber = 0;
    subImageMiniAnimationNumber = 0;
    resourcePostProcessed = false;

    //Subimage
    previousSubImageButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/arrow-left.png","Previous");
    //previousSubImageButton->set_animation(mainMenuanimation, 36 );
    nextSubImageButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/arrow-right.png","Next");
    //nextSubImageButton->set_animation(mainMenuanimation, 38 );
    subImageNumberField = new pawgui::widget_input_number("");
    subImageNumberField->set_string("0");
    subImageNumberField->set_width(64);
    subImageEditorPreviewId = new pawgui::widget_input_number("");
    subImageEditorPreviewId->set_string("0");
    subImageEditorPreviewId->set_label("Editor Preview Frame");
    openExternalEditorButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/rocket.png","Opens animation Image In External Editor");
    refreshResourceDataButton = new pawgui::widget_button_icon( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/refresh.png","Refreshes the loaded animation image");
    labelInfoMaxTextureSize = new pawgui::widget_label_text ("Max Image Size: 8192 X 8192px","Max Image Size: 8192 X 8192px");

    animXScroll = new pawgui::widget_scrollbar_xaxis();
    animYScroll = new pawgui::widget_scrollbar_yaxis();
}

animationResource::~animationResource()
{
    if( animatonPreviewRect!=NULL )
    {
        delete animatonPreviewRect;
        animatonPreviewRect = NULL;
    }
    if( animationTrackBar!=NULL )
    {
        delete animationTrackBar;
        animationTrackBar = NULL;
    }
    if( previewZoomLevel!=NULL )
    {
        delete previewZoomLevel;
        previewZoomLevel = NULL;
    }
    if( previewSubImageNumbers!=NULL )
    {
        delete previewSubImageNumbers;
        previewSubImageNumbers = NULL;
    }
    if( animationPreviewCam!=NULL )
    {
        delete animationPreviewCam;
        animationPreviewCam = NULL;
    }
    if( animationSpeedLabel!=NULL )
    {
        delete animationSpeedLabel;
        animationSpeedLabel = NULL;
    }
    if( labelInfoMaxTextureSize!=NULL )
    {
        delete labelInfoMaxTextureSize;
        labelInfoMaxTextureSize = NULL;
    }
    if( openExternalEditorButton!=NULL )
    {
        delete openExternalEditorButton;
        openExternalEditorButton = NULL;
    }
    if( refreshResourceDataButton!=NULL )
    {
        delete refreshResourceDataButton;
        refreshResourceDataButton = NULL;
    }
    if( animInEditor!=NULL )
    {
        delete animInEditor;
        animInEditor = NULL;
    }
    pawgui::widget_input_number *  tempDataField = NULL;
    for( int i =ANIMATION_DATA_FIELD_COUNT-1; i >=0; i--)
    {
        tempDataField = animationDataFields[i];
        if( tempDataField!=NULL )
        {
            delete tempDataField;
            tempDataField = NULL;
        }
        animationDataFields[i] = NULL;
    }

    if( editResourceButton!=NULL)
    {
        delete editResourceButton;
        editResourceButton = NULL;
    }
    if( playPauseResourceButton!=NULL)
    {
        delete playPauseResourceButton;
        playPauseResourceButton = NULL;
    }
    if( transformResourceButton!=NULL)
    {
        delete transformResourceButton;
        transformResourceButton = NULL;
    }
    if( preloadCheckBox!=NULL)
    {
        delete preloadCheckBox;
        preloadCheckBox = NULL;
    }
    if( showAnimationCheckBox!=NULL)
    {
        delete showAnimationCheckBox;
        showAnimationCheckBox = NULL;
    }
    if( showCollisionShapeCheckBox!=NULL)
    {
        delete showCollisionShapeCheckBox;
        showCollisionShapeCheckBox = NULL;
    }
    if( animationCollisionShapeMenu!=NULL)
    {
        delete animationCollisionShapeMenu;
        animationCollisionShapeMenu = NULL;
    }
    if( animationCenterCollisionButton!=NULL)
    {
        delete animationCenterCollisionButton;
        animationCenterCollisionButton = NULL;
    }
    if( animationCollisionRectX!=NULL)
    {
        delete animationCollisionRectX;
        animationCollisionRectX = NULL;
    }
    if( animationCollisionRectY!=NULL)
    {
        delete animationCollisionRectY;
        animationCollisionRectY = NULL;
    }
    if( animationCollisionRectW!=NULL)
    {
        delete animationCollisionRectW;
        animationCollisionRectW = NULL;
    }
    if( animationCollisionRectH!=NULL)
    {
        delete animationCollisionRectH;
        animationCollisionRectH = NULL;
    }
    if( animationCollisionCircleX!=NULL)
    {
        delete animationCollisionCircleX;
        animationCollisionCircleX = NULL;
    }
    if( animationCollisionCircleY!=NULL)
    {
        delete animationCollisionCircleY;
        animationCollisionCircleY = NULL;
    }
    if( animationCollisionCircleR!=NULL)
    {
        delete animationCollisionCircleR;
        animationCollisionCircleR = NULL;
    }

    if( previousSubImageButton!=NULL)
    {
        delete previousSubImageButton;
        previousSubImageButton = NULL;
    }

    if( nextSubImageButton!=NULL)
    {
        delete nextSubImageButton;
        nextSubImageButton = NULL;
    }

    if( subImageNumberField!=NULL)
    {
        delete subImageNumberField;
        subImageNumberField = NULL;
    }
    if( subImageEditorPreviewId!=NULL)
    {
        delete subImageEditorPreviewId;
        subImageEditorPreviewId = NULL;
    }

    if( labelanimationDimensions!=NULL)
    {
        delete labelanimationDimensions;
        labelanimationDimensions = NULL;
    }

    if( labelFrameInfo!=NULL)
    {
        delete labelFrameInfo;
        labelFrameInfo = NULL;
    }
    if( labelanimationMessage!=NULL)
    {
        delete labelanimationMessage;
        labelanimationMessage = NULL;
    }

}

bool animationResource::build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount)
{
    if( fileTarget!=NULL && fileTarget->is_open() )
    {
        std::string nestedTabsStr = pawgui::generate_tabs( leftTabAmount  );
        std::string html5SpShName = get_name();
        if( is_build_ready() )
        {
            std::string nestedTabsStrObjFunc = pawgui::generate_tabs( leftTabAmount +1 );

            *fileTarget << nestedTabsStr << "var " << html5SpShName << " =  GPE.rsm.add_animation(";
            *fileTarget << stg_ex::int_to_string (exportBuildGlobalId ) +",";
            *fileTarget << stg_ex::int_to_string ( (int)animInEditor->get_frame_count() )  +",";
            *fileTarget << "'resources/animations/"+ stg_ex::get_short_filename (animInEditor->get_file_name(),true )+"',";
            *fileTarget << stg_ex::int_to_string (animInEditor->get_frame_xoffset() ) +",";
            *fileTarget << stg_ex::int_to_string (animInEditor->get_frame_yoffset() ) +",";
            *fileTarget << stg_ex::int_to_string (animInEditor->get_width() ) +",";
            *fileTarget << stg_ex::int_to_string (animInEditor->get_height() ) +",";
            *fileTarget << stg_ex::int_to_string (animationCollisionShapeMenu->get_selected_id() ) +",";
            if( animationCollisionShapeMenu->get_selected_value()==1)
            {
                *fileTarget << stg_ex::int_to_string (animationCollisionCircleX->get_held_number() ) +",";
                *fileTarget << stg_ex::int_to_string (animationCollisionCircleY->get_held_number() ) +",";
                *fileTarget << stg_ex::int_to_string (animationCollisionCircleR->get_held_number() );
            }
            else
            {

                *fileTarget << stg_ex::int_to_string (animationCollisionRectX->get_held_number() ) +",";
                *fileTarget << stg_ex::int_to_string (animationCollisionRectY->get_held_number() ) +",";
                *fileTarget << stg_ex::int_to_string (animationCollisionRectW->get_held_number() ) +",";
                *fileTarget << stg_ex::int_to_string (animationCollisionRectH->get_held_number() );
            }
            *fileTarget << ");\n";
            return true;
        }
        *fileTarget << nestedTabsStr << "var " << html5SpShName << " = GPE.rsm.add_animation_empty();\n";
        return true;
    }
    return false;
}

bool animationResource::build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount  )
{
   if( fileTarget!=NULL && fileTarget->is_open() )
   {
        std::string nestedTabsStr = pawgui::generate_tabs( leftTabAmount  );
        std::string nestedTabsStrObjFunc = pawgui::generate_tabs( leftTabAmount +1 );
        std::string cppAnimationName = get_name();

        if( is_build_ready() )
        {
            *fileTarget << nestedTabsStr << "settings->animationsInstructions->add_keypair( new  key_pair( " + stg_ex::int_to_string (exportBuildGlobalId )+ "," << cppAnimationName+"-"+ stg_ex::get_short_filename (animInEditor->get_file_name(),true ) << ", \"";
            *fileTarget << stg_ex::int_to_string ( (int)animInEditor->get_frame_count() )  +",";
            *fileTarget << stg_ex::int_to_string (animInEditor->get_frame_xoffset() ) +",";
            *fileTarget << stg_ex::int_to_string (animInEditor->get_frame_yoffset() ) +",";
            *fileTarget << stg_ex::int_to_string (animInEditor->get_width() ) +",";
            *fileTarget << stg_ex::int_to_string (animInEditor->get_height() ) +",";
            *fileTarget << stg_ex::int_to_string (animationCollisionShapeMenu->get_selected_id() ) +",";
            if( animationCollisionShapeMenu->get_selected_value()==1)
            {
                *fileTarget << stg_ex::int_to_string (animationCollisionCircleX->get_held_number() ) +",";
                *fileTarget << stg_ex::int_to_string (animationCollisionCircleY->get_held_number() ) +",";
                *fileTarget << stg_ex::int_to_string (animationCollisionCircleR->get_held_number() );
            }
            else
            {
                *fileTarget << stg_ex::int_to_string (animationCollisionRectX->get_held_number() ) +",";
                *fileTarget << stg_ex::int_to_string (animationCollisionRectY->get_held_number() ) +",";
                *fileTarget << stg_ex::int_to_string (animationCollisionRectW->get_held_number() ) +",";
                *fileTarget << stg_ex::int_to_string (animationCollisionRectH->get_held_number() );
            }
            *fileTarget << "\");\n";
            return true;
        }
    }
    return false;
}

void animationResource::compile_cpp()
{

}

bool animationResource::get_mouse_coords( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    /*areaMouseXPos = 0;
    areaMouseYPos = 0;*/
    if( view_space!=NULL)
    {
        if( gpe::point_within( gpe::input->mouse_position_x, gpe::input->mouse_position_y,
                         animationPreviewCam->x,
                         animationPreviewCam->y,
                         animationPreviewCam->x+animationPreviewCam->w,
                         animationPreviewCam->y+animationPreviewCam->h ) )
        {
            /*
            areaMouseXPos = (gpe::input->mouse_position_x-animationPreviewCam->x )/zoomValue +animCameraRect.x - cam->x;
            areaMouseYPos = (gpe::input->mouse_position_y-animationPreviewCam->y )/zoomValue +animCameraRect.y - cam->y;
            */
            return true;
        }
    }
    return false;
}

int animationResource::get_preview_frame()
{
    autoAnimationPos = 0;
    if( subImageEditorPreviewId!=NULL && animInEditor!=NULL)
    {
        autoAnimationPos = subImageEditorPreviewId->get_held_number();
        if( (int)animInEditor->get_frame_count() <= autoAnimationPos )
        {
            autoAnimationPos = animInEditor->get_frame_count() - 1;
        }
        if( autoAnimationPos < 0)
        {
            autoAnimationPos = 0;
        }
        //subImageEditorPreviewId->set_string( int_to_string(previewFrameId) );
    }
    return autoAnimationPos;
}

gpe::animaton2d * animationResource::get_resource_animation()
{
    if( animInEditor!=NULL )
    {
        return animInEditor;
    }
    return NULL;
}

void animationResource::handle_scrolling()
{
    bool xScrollHappened = false;
    bool yScrollHappened = false;
    if( areaIsScrollable )
    {
        if( gpe::input->check_kb_down(kb_ctrl) )
        {
            //Zoom Out
            if( gpe::input->mouse_scrolling_down && editorZoomValue >= minZoomValue*2 )
            {
                editorZoomValue-=minZoomValue;
            }
            else if( gpe::input->mouse_scrolling_up && editorZoomValue < maxZoomValue-minZoomValue)
            {
                //Zoom in
                editorZoomValue += minZoomValue;
            }
        }
        else if( gpe::input->kb_shift_pressed)
        {
            if( gpe::input->mouse_scrolling_up > 0 )
            {
                xScrollHappened = true;
                animCameraRect.x-=(animCameraRect.w/16);
            }
            else if( gpe::input->mouse_scrolling_down)
            {
                xScrollHappened = true;
                animCameraRect.x+=(animCameraRect.w/16);
            }
        }
        else
        {
            if( gpe::input->mouse_scrolling_up )
            {
                yScrollHappened = true;
                animCameraRect.y-=(animCameraRect.h/4);
            }
            else if( gpe::input->mouse_scrolling_down)
            {
                if( animCameraRect.y <animRect.h- animCameraRect.h/4 )
                {
                    yScrollHappened = true;
                    animCameraRect.y+=(animCameraRect.h/4);
                }
            }
        }
    }

    //Prevents camera from going out of bounds
    if( animCameraRect.x+animCameraRect.w > animRect.w )
    {
        xScrollHappened = true;
        animCameraRect.x = animRect.w-animCameraRect.w;
    }

    if( animCameraRect.y+animCameraRect.h > animRect.h )
    {
        yScrollHappened = true;
        animCameraRect.y = animRect.h-animCameraRect.h;
    }

    if( animCameraRect.x <= 0)
    {
        xScrollHappened = true;
        animCameraRect.x = 0;
    }

    if( animCameraRect.y <= 0)
    {
        yScrollHappened = true;
        animCameraRect.y = 0;
    }

    if( xScrollHappened)
    {
        animXScroll->contextRect.x = animCameraRect.x;
        animXScroll->process_self(NULL,NULL );
    }
    if( yScrollHappened)
    {
        animYScroll->contextRect.y = animCameraRect.y;
        animYScroll->process_self(NULL,NULL );
    }

    if( minZoomValue == 0)
    {
        minZoomValue = 0.0625;
    }

    if( editorZoomValue == 0 )
    {
        editorZoomValue  = minZoomValue;
    }
}

bool animationResource::include_local_files( std::string pBuildDir , int buildType )
{
    sff_ex::append_to_file( gpe::get_user_settings_folder()+"resources_check.txt ",  get_name() +"...");
    if( animInEditor!=NULL && animInEditor->has_texture() )
    {
        if( animInEditor->get_width() > 0 )
        {
            save_resource();
            return animInEditor->copy_image_source(pBuildDir+"/resources/animations");
        }
    }
    return true;
}

bool animationResource::is_build_ready()
{
    recentErrorMessage = "";
    bool isBuildReady = true;
    if( animInEditor==NULL )
    {
        recentErrorMessage = "Animation is blank!";
        return false;
    }

    for( int i = 0; i < ANIMATION_DATA_FIELD_COUNT; i++)
    {
        if( animationDataFields[i]!=NULL)
        {
            if( animationDataFields[i]->get_held_number() < 0)
            {
                animationDataFields[i]->set_string("0");
            }
        }
        else
        {
            recentErrorMessage += "animationDataFields["+ stg_ex::int_to_string(i)+"]=NULL;";
            isBuildReady = false;
        }
    }

    if( animInEditor->get_width() <=0 )
    {
        recentErrorMessage +="AnimationWidth=0;";
        isBuildReady = false;
    }
    if( animInEditor->get_height() <=0 )
    {
        recentErrorMessage +="AnimationHeight=0;";
        isBuildReady = false;
    }
    return isBuildReady;
}

void animationResource::load_image(std::string new_file_name, bool autoProcess)
{
    if( (int)new_file_name.size() > 3 && new_file_name!="NULL")
    {
        if( stg_ex::file_is_image(new_file_name) && animInEditor!=NULL )
        {
            animInEditor->load_image( gpe::rph->get_default_render_package(), new_file_name, true);
            if( animInEditor->get_texture_width() <1 || animInEditor->get_texture_width()>8192 || animInEditor->get_texture_height() <1 || animInEditor->get_texture_height()>8192 )
            {
                gpe::error_log->report("[Animation Error:] Unable to load image: " +new_file_name);
                sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_error_log2.txt","[Animation Error:] Unable to load image: " +new_file_name);
            }
            else if( autoProcess )
            {
                for( int i =0; i < ANIMATION_DATA_FIELD_COUNT; i++)
                {
                    animationDataFields[i]->set_number(0);
                }
                animationDataFields[ gpe::anim_frame_width ]->set_number( animInEditor->get_texture_width() );
                animationDataFields[ gpe::anim_frame_height ]->set_number( animInEditor->get_texture_height() );

                int radMin = std::min ( animInEditor->get_texture_width(),animInEditor->get_texture_height() )/2;
                animationCollisionCircleX->set_number( animInEditor->get_texture_width()/2);
                animationCollisionCircleY->set_number( animInEditor->get_texture_height()/2 );
                animationCollisionCircleR->set_number(  radMin );

                animationCollisionRectX->set_number(0);
                animationCollisionRectY->set_number(0);
                animationCollisionRectW->set_number( animInEditor->get_texture_width() );
                animationCollisionRectH->set_number( animInEditor->get_texture_height() );
            }
        }
        else
        {
            pawgui::display_user_alert("Unable to load image","File type["+  stg_ex::get_file_ext(new_file_name)+"] not supported when loading ["+new_file_name+"].");
        }
    }
}

void animationResource::load_resource(std::string file_path)
{
    if( resourcePostProcessed && sff_ex::file_exists(file_path))
    {
        return;
    }
    if( pawgui::main_loader_display != NULL )
    {
        pawgui::main_loader_display->update_submessages( "Loading Animation", resource_name );
    }

    std::string otherColContainerName = "";

    std::string newFileIn = "";
    std::string soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/animations/";
    if( sff_ex::file_exists(file_path) )
    {
        newFileIn = file_path;
        soughtDir = stg_ex::get_path_from_file(newFileIn);
    }
    else
    {

        newFileIn = soughtDir + resource_name+".gpf";
        //Fallsback to previous folder for older versions
        if( sff_ex::file_exists(newFileIn) == false )
        {
            soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/animations/";
            newFileIn = soughtDir + resource_name+".gpf";
        }
    }
    std::ifstream gameResourceFileIn( newFileIn.c_str() );

    //error_log->report("Loading animation - "+newFileIn);
    //If the level file could be loaded
    float foundFileVersion = -1;
    if( gameResourceFileIn.fail() )
    {
        gameResourceFileIn.close();
        return;
    }
    //makes sure the file is open
    if ( !gameResourceFileIn.is_open())
    {
        gameResourceFileIn.close();
        return;
    }

    int equalPos = 0;
    std::string firstChar="";
    std::string section="";
    std::string keyString="";
    std::string valString="";
    std::string subValString="";
    std::string currLine="";
    std::string currLineToBeProcessed;
    std::string sprDataStr;

    int i = 0;
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
                    keyString= stg_ex::string_lower( keyString );
                    valString = currLineToBeProcessed.substr(equalPos+1,currLineToBeProcessed.length());

                    if( keyString=="resourcename")
                    {
                        renameBox->set_string(valString);
                    }
                    else if( keyString=="imagelocation")
                    {
                        if( valString!="NULL")
                        {
                            load_image( soughtDir+valString );
                        }
                    }
                    else if( keyString=="preload")
                    {
                        preloadCheckBox->set_checked( stg_ex::string_to_bool(valString) );
                    }
                    else if( keyString=="editorpreviewFrame")
                    {
                        subImageEditorPreviewId->set_string( valString );
                    }
                    else if( keyString=="previewcollisionshape")
                    {
                        if( showCollisionShapeCheckBox!=NULL)
                        {
                            showCollisionShapeCheckBox->set_checked( stg_ex::string_to_bool(valString) );
                        }
                    }
                    else if( keyString=="collisionshape" )
                    {
                        animationCollisionShapeMenu->set_option_value( stg_ex::string_to_int(valString) );
                    }
                    else if( keyString=="circlecollisionregion")
                    {
                        animationCollisionCircleX->set_number(stg_ex::split_first_int(valString,',') );
                        animationCollisionCircleY->set_number(stg_ex::split_first_int(valString,',') );
                        animationCollisionCircleR->set_number( stg_ex::string_to_int(valString,0) );
                    }
                    else if( keyString=="rectanglecollisionbox")
                    {
                        animationCollisionRectX->set_number( stg_ex::split_first_int(valString,',') );
                        animationCollisionRectY->set_number( stg_ex::split_first_int(valString,',') );
                        animationCollisionRectW->set_number( stg_ex::split_first_int(valString,',') );
                        animationCollisionRectH->set_number( stg_ex::string_to_int(valString,0) );
                    }
                    else if( keyString=="subimageinview")
                    {
                        subImagePreviewNumber = stg_ex::string_to_int(valString);
                        subImageNumberField->set_string( stg_ex::int_to_string(subImagePreviewNumber) );
                    }
                    else
                    {
                        if( semath::compare_floats(foundFileVersion, GPE_ANIM_FCOUNT_VERSION) || foundFileVersion > GPE_ANIM_FCOUNT_VERSION )
                        {
                            //error_log->report("Proper animation system used ["+resource_name+"]");
                            for( i = 0; i <ANIMATION_DATA_FIELD_COUNT ; i++)
                            {
                                sprDataStr = "animationdata["+ stg_ex::int_to_string(i)+"]";
                                if( keyString==sprDataStr)
                                {
                                    animationDataFields[i]->set_string( valString );
                                }
                                else
                                {
                                    sprDataStr = "spritedata["+ stg_ex::int_to_string(i)+"]";
                                    if( keyString==sprDataStr)
                                    {
                                        animationDataFields[i]->set_string( valString );

                                    }
                                }
                            }
                        }
                        else
                        {
                            //error_log->report("Alternate animation system used ["+resource_name+"]");
                            animationDataFields[0]->set_string( "-1");
                            for( i = 0; i <ANIMATION_DATA_FIELD_COUNT-1 ; i++)
                            {
                                sprDataStr = "animationdata["+ stg_ex::int_to_string(i)+"]";
                                if( keyString==sprDataStr)
                                {
                                    animationDataFields[i+1]->set_string( valString);
                                }
                                else
                                {
                                    sprDataStr = "spritedata["+ stg_ex::int_to_string(i)+"]";
                                    if( keyString==sprDataStr)
                                    {
                                        animationDataFields[i+1]->set_string( valString);
                                    }
                                }
                            }
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
    if( foundFileVersion > 0 )
    {
        resourcePostProcessed= true;
    }
    process_data_fields(foundFileVersion);
}

void animationResource::process_collision_box()
{
    if( animInEditor== NULL)
    {
        return;
    }
    if( animationCollisionShapeMenu->get_selected_name()=="Circle")
    {
        int colX = animationCollisionCircleX->get_held_number();
        int colY = animationCollisionCircleY->get_held_number();
        int colR = animationCollisionCircleR->get_held_number();
        animInEditor->edit_collision_circle(colX, colY, colR );
    }
    else
    {
        int colX = animationCollisionRectX->get_held_number();
        int colW = animationCollisionRectW->get_held_number();
        int colY = animationCollisionRectY->get_held_number();
        int colH = animationCollisionRectH->get_held_number();
        animInEditor->edit_collision_box(colX, colY, colW,  colH );
    }
}

void animationResource::process_data_fields(float versionToProcess)
{
    if( animInEditor!=NULL )
    {
        bool animationIsReadyForEditing = true;

        int i = 0;
        int j = 0;
        while( i < ANIMATION_DATA_FIELD_COUNT )
        {
            if( animationDataFields[i]!=NULL)
            {
                if( animationDataFields[i]->is_inuse()==true)
                {
                    animationIsReadyForEditing = false;
                }
            }
            else
            {
                animationIsReadyForEditing = false;
            }
            i++;
        }
        if( animationIsReadyForEditing)
        {
            //error_log->report("Beginning to Process animation Data...");
            int fWidth = animationDataFields[ gpe::anim_frame_width]->make_valid_number(0);
            int fHeight = animationDataFields[ gpe::anim_frame_height]->make_valid_number(0);

            int fPixelOffsetH = animationDataFields[ gpe::anim_pixel_offset_hori]->make_valid_number(0);
            int fPixelOffsetV = animationDataFields[ gpe::anim_pixel_offset_vert]->make_valid_number(0);

            int hPadding = animationDataFields[ gpe::anim_pixel_padding_hori]->make_valid_number(0);

            int vPadding = animationDataFields[ gpe::anim_pixel_padding_vert]->make_valid_number(0);

            if( fWidth > 0 && fHeight > 0 && hPadding>=0 && vPadding >=0)
            {
                int startX = fPixelOffsetH;
                int startY = fPixelOffsetV;
                int maxanimationFrames = animationDataFields[ gpe::anim_frame_count]->make_valid_number(-1);

                if( versionToProcess > 0 && ( semath::compare_floats(versionToProcess,GPE_ANIM_FCOUNT_VERSION) || versionToProcess > GPE_ANIM_FCOUNT_VERSION  ) && maxanimationFrames > 0)
                {
                    animInEditor->setup_animation( maxanimationFrames,fWidth, fHeight, fPixelOffsetH, fPixelOffsetV, hPadding, vPadding);
                }
                else
                {
                    animInEditor->setup_fullimg_animation(fWidth, fHeight, fPixelOffsetH, fPixelOffsetV, hPadding, vPadding);
                    animationDataFields[ gpe::anim_frame_count]->set_number( animInEditor->get_frame_count() );
                }
            }
        }
        process_collision_box();
    }
}

void animationResource::prerender_self(  )
{
    standardEditableGameResource::prerender_self();
    if( editResourceButton!=NULL)
    {
        editResourceButton->prerender_self();
    }
    if( transformResourceButton!=NULL)
    {
        transformResourceButton->prerender_self();
    }
    if( preloadCheckBox!=NULL)
    {
        preloadCheckBox->prerender_self();
    }
    if( showAnimationCheckBox!=NULL)
    {
        showAnimationCheckBox->prerender_self();
    }
    if( animationCenterCollisionButton!=NULL)
    {
        animationCenterCollisionButton->prerender_self();
    }
}

void animationResource::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    int i;
    if( cam==NULL && view_space == NULL )
    {
        return;
    }
    if( openExternalEditorButton!=NULL )
        openExternalEditorButton->set_clicked( false );

    bool editorHasControl = false;

    animRect.x = 0;
    animRect.y = 0;
    animRect.w = 0;
    animRect.h = 0;

    if( animInEditor!=NULL && animInEditor->has_texture() )
    {
        animRect.w = animInEditor->get_texture_width();
        animRect.h = animInEditor->get_texture_height();
    }

    if( minZoomValue == 0)
    {
       minZoomValue = 0.0625;
    }
    if( editorZoomValue == 0)
    {
        editorZoomValue = minZoomValue;
    }
    animationPreviewCam->x = 0;
    animationPreviewCam->y = 0;
    animationPreviewCam->w = view_space->w;
    animationPreviewCam->h = view_space->h;

    animCameraRect.w = animationPreviewCam->w / editorZoomValue;
    animCameraRect.h = animationPreviewCam->h / editorZoomValue;

    if( animCameraRect.w > animRect.w)
    {
        animCameraRect.w = animRect.w;
    }

    if( animCameraRect.h > animRect.h)
    {
        animCameraRect.h = animRect.h;
    }

    //Horizontal scrolling
    animXScroll->set_coords( 0,view_space->h - animXScroll->get_height() );
    animXScroll->set_width( view_space->w);
    update_rectangle(&animXScroll->fullRect,0,0,(float)animRect.w, (float)animRect.h );
    update_rectangle(&animXScroll->contextRect,(float)animCameraRect.x,(float)animCameraRect.y, (float)animCameraRect.w, (float)animCameraRect.h );
    animXScroll->process_self(view_space,cam );
    //if( animXScroll->has_moved() )
    {
        animCameraRect.x = (float)(animXScroll->contextRect.x);
    }

    //Vertical Scrolling
    animYScroll->set_coords( view_space->w -animYScroll->get_width(),0);
    animYScroll->set_height( view_space->h );
    update_rectangle(&animYScroll->fullRect,0,0,(float)animRect.w, (float)animRect.h );
    update_rectangle(&animYScroll->contextRect,(float)animCameraRect.x,(float)animCameraRect.y, (float)animCameraRect.w, (float)animCameraRect.h );
    //animYScroll->contextRect.h = sceneEditorView.h;
    animYScroll->process_self(view_space,cam );
    //if( animYScroll->has_moved() )
    {
        animCameraRect.y = float(animYScroll->contextRect.y);
    }

    if( animInEditor!=NULL)
    {
        if( animInEditor->has_texture() )
        {
            labelanimationDimensions->set_name("Image Dimensions:"+ stg_ex::int_to_string( animInEditor->get_texture_width() )+" x "+ stg_ex::int_to_string( animInEditor->get_texture_height() )+"px");
        }
        else
        {
            labelanimationDimensions->set_name("animationsheet size unknown");
        }
    }
    else
    {
        labelanimationDimensions->set_name("animationsheet size unknown");
    }

    if( editorZoomValue == 0 )
    {
        minZoomValue = 0.0625;
    }
    if( editorZoomValue == 0 )
    {
        editorZoomValue  = minZoomValue;
    }
    if( editorZoomValue < minZoomValue)
    {
        editorZoomValue = minZoomValue;
    }
    if( editorZoomValue > maxZoomValue)
    {
        editorZoomValue = maxZoomValue;
    }


    if( panel_main_area==NULL && panel_inspector == NULL)
    {
        return;
    }

    gpeEditorDockPanel * collisionAndPreviewDataPanel = panel_main_area;
    if( panel_inspector != NULL)
    {
        panel_inspector->clear_panel();
        collisionAndPreviewDataPanel = panel_inspector;
    }

    if( panel_main_area!=NULL )
    {
        editorHasControl = panel_main_area->hasScrollControl || collisionAndPreviewDataPanel->hasScrollControl;
        panel_main_area->add_gui_element( renameBox,true);
        panel_main_area->add_gui_element( loadResourceButton,false );
        panel_main_area->add_gui_element( refreshResourceDataButton,false );
        panel_main_area->add_gui_element( editResourceButton,false);
        panel_main_area->add_gui_element( transformResourceButton,false);
        panel_main_area->add_gui_element( openExternalEditorButton,true);
        panel_main_area->add_gui_element( previewSubImageNumbers,true);

        panel_main_area->add_gui_element(labelInfoMaxTextureSize,true);
        panel_main_area->add_gui_element( labelanimationDimensions,true);
        for(  i =0; i < ANIMATION_DATA_FIELD_COUNT; i++)
        {
            if( animationDataFields[i]!=NULL)
            {
                panel_main_area->add_gui_element(animationDataFields[i],true );
                        //animationDataFields[i]->set_width(panel_main_area->get_width()-pawgui::padding_default*3);
            }
        }
    }
    else
    {
        editorHasControl = collisionAndPreviewDataPanel->hasScrollControl;
    }
    //Collision and animation preview stuff
    collisionAndPreviewDataPanel->add_gui_element( previousSubImageButton,false);
    collisionAndPreviewDataPanel->add_gui_element( subImageNumberField,false);
    collisionAndPreviewDataPanel->add_gui_element( nextSubImageButton,false);
    collisionAndPreviewDataPanel->add_gui_element(playPauseResourceButton,true);
    collisionAndPreviewDataPanel->add_gui_element( subImageEditorPreviewId,true);

    labelFrameInfo->set_name("Animation Size: "+ stg_ex::int_to_string(animInEditor->get_width() )+" x "+ stg_ex::int_to_string(animInEditor->get_height() )+"px");
    collisionAndPreviewDataPanel->add_gui_element( labelFrameInfo,true);
    collisionAndPreviewDataPanel->add_gui_element( animationCollisionShapeMenu,true);
    collisionAndPreviewDataPanel->add_gui_element( animationCenterCollisionButton,true);

    //Rectangle collision box
    if( animationCollisionShapeMenu->get_selected_value()==0 )
    {
        collisionAndPreviewDataPanel->add_gui_element( animationCollisionRectX,false);
        collisionAndPreviewDataPanel->add_gui_element( animationCollisionRectY,true);
        collisionAndPreviewDataPanel->add_gui_element( animationCollisionRectW,false);
        collisionAndPreviewDataPanel->add_gui_element( animationCollisionRectH,true);
    }
    else
    {
        //Circle collision box
        animationCollisionCircleX->set_width( (collisionAndPreviewDataPanel->get_width()-pawgui::padding_default*3 )/3 );
        animationCollisionCircleY->set_width( animationCollisionCircleX->get_width( ) );
        animationCollisionCircleR->set_width(animationCollisionCircleX->get_width( ) );

        collisionAndPreviewDataPanel->add_gui_element( animationCollisionCircleX,false);
        collisionAndPreviewDataPanel->add_gui_element( animationCollisionCircleY,false);
        collisionAndPreviewDataPanel->add_gui_element( animationCollisionCircleR,true);
        collisionAndPreviewDataPanel->add_gui_element( showCollisionShapeCheckBox,true);
    }

    if( panel_inspector != NULL)
    {
        panel_inspector->process_self( NULL, NULL );
    }

    if( panel_main_area!=NULL )
    {
        panel_main_area->add_gui_element(preloadCheckBox,true);
        panel_main_area->add_gui_element(confirmResourceButton,true);

        panel_main_area->add_gui_element(cancelResourceButton,true);
        //panel_main_area->set_maxed_out_width();
        panel_main_area->process_self( NULL, NULL );

        if( loadResourceButton!=NULL && loadResourceButton->is_clicked() )
        {
            std::string new_file_name = pawgui::get_filename_open_from_popup("Load Your animation Image","Image",pawgui::main_settings->fileOpenanimationDir);
            if( (int)new_file_name.size() > 3 && sff_ex::file_exists(new_file_name) )
            {
                bool autoProcessData = false;
                if( animationDataFields[0]!=NULL)
                {
                    if( animationDataFields[0]->get_held_number()==0)
                    {
                        autoProcessData = true;
                    }
                }
                load_image(new_file_name, autoProcessData);
                gpe::input->reset_all_input();
                return;
            }
        }
        else if( confirmResourceButton->is_clicked() )
        {
            gpe::input->reset_all_input();
            for( int i =0; i < ANIMATION_DATA_FIELD_COUNT; i++)
            {
                if( animationDataFields[i]!=NULL)
                {
                    animationDataFields[i]->switch_inuse(false);
                }
            }

            int radMin = std::min (animationDataFields[ gpe::anim_frame_width]->get_held_number(), animationDataFields[ gpe::anim_frame_height]->get_held_number() )/2;
            animationCollisionCircleX->set_number( animationDataFields[ gpe::anim_frame_width]->get_held_number()/2);
            animationCollisionCircleY->set_number( animationDataFields[ gpe::anim_frame_height]->get_held_number()/2 );
            animationCollisionCircleR->set_number(  radMin );

            animationCollisionRectX->set_number(0);
            animationCollisionRectY->set_number(0);
            animationCollisionRectW->set_number( animationDataFields[ gpe::anim_frame_width]->get_held_number() );
            animationCollisionRectH->set_number( animationDataFields[ gpe::anim_frame_height]->get_held_number() );
            requestedMode = 0;
            gpe::input->reset_all_input();
            save_resource();
            isPreloaded = preloadCheckBox->is_clicked();
        }
        else if( cancelResourceButton->is_clicked() )
        {
            if( pawgui::display_prompt_message("Are you sure you will like to reverse changes?","This will load in data from save-file!", true )== pawgui::display_query_yes )
            {
                resourcePostProcessed = false;
                gpe::input->reset_all_input();
                requestedMode = 0;
                gpe::error_log->report("Reverting to Editor Mode 0");
                revert_data_fields();
                process_data_fields( gpe::version_number_total);
                load_resource();
            }
        }

        if( editResourceButton!=NULL && editResourceButton->is_clicked() )
        {
            editResourceButton->set_clicked( false );
            gpe::error_log->report("Going to Editor Mode 1");
            gpe::input->reset_all_input();
            process_self(view_space,cam);
            return;
        }

        if( transformResourceButton!=NULL &&  transformResourceButton->is_clicked() && animInEditor!=NULL)
        {
            if( animInEditor->get_width() > 0 && animInEditor->get_height() > 0)
            {
                pawgui::context_menu_open(-1,-1,256 );
                pawgui::main_context_menu->add_menu_option("Erase BG Color",0);
                pawgui::main_context_menu->add_menu_option("Invert Colors",1);
                pawgui::main_context_menu->add_menu_option("Make GrayScale",2);
                pawgui::main_context_menu->add_menu_option("Exit",10);
                int menuSelection = pawgui::context_menu_process();
                if( menuSelection>=0 && menuSelection <=3)
                {
                    std::string tempStr = animInEditor->get_file_name();
                    SDL_Surface * oTempSurface = sdl_surface_ex::load_surface_image(  tempStr.c_str() );
                    SDL_Surface *nTempSurface = NULL;
                    if( oTempSurface!=NULL)
                    {
                        if( menuSelection==0)
                        {
                            gpe::color * foundBGColor = gpe::c_fuchsia->duplicate_color();
                            if( pawgui::get_color_from_popup("Image Background Color To Remove",foundBGColor) )
                            {
                                if( pawgui::display_prompt_message("Are you sure you want to erase this Color from this image?","This action is irreversible and will change your image's format to a .png file!")==pawgui::display_query_yes)
                                {
                                    gpe::error_log->report("Modifying image at: "+animInEditor->get_file_name() +".");
                                    nTempSurface = sdl_surface_ex::surface_remove_color_rgba( oTempSurface, foundBGColor->get_r(), foundBGColor->get_g(), foundBGColor->get_b() );
                                }
                            }
                            delete foundBGColor;
                            foundBGColor = NULL;

                        }
                        else if( menuSelection==1 )
                        {
                            if( pawgui::display_prompt_message("Are you sure you want to invert your image's colors?","This action is irreversible and will change your image's format to a .png file!")==pawgui::display_query_yes)
                            {
                                nTempSurface= sdl_surface_ex::surface_invert(oTempSurface);
                            }
                        }
                        else if( menuSelection==2 )
                        {
                            if( pawgui::display_prompt_message("Are you sure you want to grayscale your image?","This action is irreversible and will change your image's format to a .png file!")==pawgui::display_query_yes)
                            {
                                nTempSurface= sdl_surface_ex::surface_grayscale(oTempSurface);
                            }
                        }
                        if( nTempSurface!=NULL)
                        {
                            gpe::delete_file( animInEditor->get_file_name() );
                            std::string newImageName = stg_ex::get_file_noext(animInEditor->get_file_name() )+".png";
                            IMG_SavePNG(nTempSurface,newImageName.c_str() );
                            SDL_FreeSurface(nTempSurface);
                            nTempSurface = NULL;
                            animInEditor->load_image(  gpe::rph->get_default_render_package(), newImageName, true );
                        }
                        SDL_FreeSurface(oTempSurface);
                        oTempSurface = NULL;
                    }
                }
            }
        }

        if( openExternalEditorButton!=NULL && animInEditor!=NULL)
        {
            if( openExternalEditorButton->is_clicked() )
            {
                if( sff_ex::file_exists(animInEditor->get_file_name() ) )
                {
                    if( main_editor_settings!=NULL && main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]!=NULL)
                    {
                        gpe::external_open_program(main_editor_settings->pencilExternalEditorsFile[GPE_EXTERNAL_EDITOR_IMG]->get_string(),animInEditor->get_file_name(), true );
                    }
                    else
                    {
                        gpe::external_open_url(animInEditor->get_file_name() );
                    }
                    sff_ex::append_to_file( gpe::get_user_settings_folder()+"gpe_error_log2.txt","Attempting to edit ["+animInEditor->get_file_name()+"]...");
                }
            }
        }

        if( refreshResourceDataButton!=NULL  && refreshResourceDataButton->is_clicked() )
        {
            resourcePostProcessed = false;
            load_resource();
        }

        //actual saving of the animation onto the engine and files
        if( playPauseResourceButton!=NULL &&  playPauseResourceButton->is_clicked() )
        {
            if( animInEditor!=NULL)
            {
                if( playPauseResourceButton->get_name()=="Play")
                {
                    playPauseResourceButton->set_name("Stop");
                    playPauseResourceButton->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/stop.png" );
                }
                else
                {
                    playPauseResourceButton->set_name("Play");
                    playPauseResourceButton->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png" );
                }
            }
        }
    }

    if( animationCollisionShapeMenu!=NULL)
    {
        if( animationCollisionShapeMenu->get_selected_value()==0 )
        {
            if( animationCenterCollisionButton->is_clicked() && animInEditor!=NULL)
            {
                animationCollisionRectX->set_string( stg_ex::int_to_string( 0 ) );
                animationCollisionRectY->set_string( stg_ex::int_to_string( 0 ) );
                animationCollisionRectW->set_string( stg_ex::int_to_string( animInEditor->get_width() ) );
                animationCollisionRectH->set_string( stg_ex::int_to_string( animInEditor->get_height() ) );
            }
        }
        else if( animationCollisionShapeMenu->get_selected_value()==1 )
        {
            if( animationCenterCollisionButton->is_clicked() && animInEditor!=NULL)
            {
                int smallestCenter = std::min( animInEditor->get_width(), animInEditor->get_height() )/2;
                animationCollisionCircleX->set_string( stg_ex::int_to_string( animInEditor->get_width()/2 ) );
                animationCollisionCircleY->set_string( stg_ex::int_to_string( animInEditor->get_height()/2 ) );
                animationCollisionCircleR->set_string( stg_ex::int_to_string(smallestCenter ) );
            }
        }

        //Animates animation
        if( playPauseResourceButton->get_name()=="Stop" )
        {
            animationSpeed = gpe::fps_cap/4.f;
            //animationSpeed = std::max( 1, animationSpeed );
            subImageMiniAnimationNumber += ( animationSpeed * gpe::time_keeper->get_delta_performance() )/ 100.f ;
            //error_log->report("Animating animation by "+ stg_ex::float_to_string(animationSpeed)+"..."+ stg_ex::int_to_string(random(0,100) )+"...");

            if( subImageMiniAnimationNumber >= animationSpeed )
            {
                subImagePreviewNumber++;
                subImageNumberField->set_number( subImagePreviewNumber);
                subImageMiniAnimationNumber = 0;
            }
        }

        if( previousSubImageButton!=NULL && previousSubImageButton->is_clicked() && subImagePreviewNumber > 0)
        {
            subImagePreviewNumber--;
            subImageNumberField->set_number( subImagePreviewNumber);
            playPauseResourceButton->set_name("Play");
            playPauseResourceButton->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png" );
        }

        if( subImageNumberField!=NULL && subImageNumberField->is_inuse() )
        {
            subImagePreviewNumber = subImageNumberField->get_held_number();
            playPauseResourceButton->set_name("Play");
            playPauseResourceButton->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png" );
        }

        if( nextSubImageButton!=NULL &&  nextSubImageButton->is_clicked() )
        {
            subImagePreviewNumber++;
            subImageNumberField->set_number( subImagePreviewNumber);
            playPauseResourceButton->set_name("Play");
            playPauseResourceButton->change_texture( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/play.png" );
        }

        if( subImagePreviewNumber >= (int)animInEditor->get_frame_count() )
        {
            subImagePreviewNumber = 0;
            subImageNumberField->set_number( subImagePreviewNumber);
        }

        if(subImagePreviewNumber < 0 )
        {
            subImagePreviewNumber = 0;
            subImageNumberField->set_number( subImagePreviewNumber);
        }

        if( panel_main_area!=NULL)
        {
            editorHasControl = panel_main_area->hasScrollControl || collisionAndPreviewDataPanel->hasScrollControl;
        }
        else
        {
            editorHasControl = collisionAndPreviewDataPanel->hasScrollControl;
        }
        //actual saving of the animation onto the engine and files
        if( get_mouse_coords(view_space, cam) )
        {
            areaIsScrollable = true;
        }
        else
        {
            areaIsScrollable = false;
        }
        handle_scrolling();

        if( areaIsScrollable )
        {
            if( gpe::input->check_mouse_released( mb_right) )
            {
                /*
                pawgui::context_menu_open( gpe::input->mouse_position_x, gpe::input->mouse_position_y );
                    pawgui::main_context_menu->add_menu_option("Change Box Color",0);
                    pawgui::main_context_menu->add_menu_option("Change Number Color",1);
                    pawgui::main_context_menu->add_menu_option("Return to Mode 0",2);
                    int menuSelection = pawgui::context_menu_process();

                    if( menuSelection == 2)
                    {
                        gpe::input->reset_all_input();
                        requestedMode = 0;
                        error_log->report("Reverting to Editor Mode 0");
                        gpe::input->reset_all_input();
                        process_self(view_space,cam);
                        return;
                    }
                    */
            }
        }

        if( !editorHasControl )
        {
            if( gpe::input->check_kb_down(kb_ctrl) )
            {
                if( gpe::input->check_kb_released(kb_minus) || gpe::input->mouse_scrolling_down )
                {
                    editorZoomValue -= minZoomValue;
                }
                else if( gpe::input->check_kb_released(kb_plus) || gpe::input->mouse_scrolling_up )
                {
                    editorZoomValue += minZoomValue;
                }
            }
        }
    }
}

void animationResource::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam )
{
    view_space = gpe::camera_find(view_space);
    cam = gpe::camera_find(cam);
    bool animationPreviewIsRendered = false;
    if( cam==NULL || view_space==NULL )
    {
        return;
    }

    if( texture_transparent_bg!=NULL )
    {
        int iPV = 0, jPV = 0;
        for( iPV = 0; iPV< view_space->w; iPV+=texture_transparent_bg->get_width() )
        {
            for( jPV= 0; jPV< view_space->h; jPV+=texture_transparent_bg->get_height() )
            {
                texture_transparent_bg->render_tex( iPV,jPV,NULL);
            }
        }
    }
    else
    {
        gpe::gcanvas->render_rectangle( 0,0,view_space->w, view_space->h, pawgui::theme_main->program_color, false, 255 );
    }

    if( animInEditor!=NULL &&animInEditor->has_texture() )
    {
        if( subImagePreviewNumber >=0 && subImagePreviewNumber < (int)animInEditor->get_frame_count() )
        {
            animInEditor->render_scaled( subImagePreviewNumber, animationPreviewCam->x+animationPreviewCam->w/2,
            animationPreviewCam->y+animationPreviewCam->h/2, editorZoomValue,editorZoomValue,NULL );
        }
        else
        {
            animInEditor->render_scaled( 0,animationPreviewCam->x+animationPreviewCam->w/2,
            animationPreviewCam->y+animationPreviewCam->h/2, editorZoomValue,editorZoomValue,NULL );
        }

        if( showCollisionShapeCheckBox->is_clicked() && animInEditor!=NULL )
        {
            if( animationCollisionShapeMenu->get_selected_value()==0 )
            {
                int sprCX = animationPreviewCam->x+animationPreviewCam->w/2;
                sprCX+=animationCollisionRectX->get_held_number() * editorZoomValue;
                int sprCY = animationPreviewCam->y+animationPreviewCam->h/2;
                sprCY+=animationCollisionRectY->get_held_number() * editorZoomValue;
                int sprCW = animationCollisionRectW->get_held_number() * editorZoomValue;
                int sprCH = animationCollisionRectH->get_held_number() * editorZoomValue;
                sprCX-= sprCW/2;
                sprCY-= sprCH/2;

                if( sprCX >= 0 && sprCY >=0 && sprCW > 0 && sprCH > 0)
                {
                    gpe::gcanvas->render_rectangle( sprCX,sprCY,sprCX+sprCW,sprCY+sprCH, gpe::c_green,false,96);
                }
            }
            else if( animationCollisionShapeMenu->get_selected_value()==1 )
            {
                int sprCX = animationPreviewCam->x+( animationPreviewCam->w )/2;// - (animInEditor->get_width()*editor0ZoomValue)/2;
                sprCX+=animationCollisionCircleX->get_held_number() * editorZoomValue;

                int sprCY = animationPreviewCam->y+( animationPreviewCam->h )/2;// - (animInEditor->get_height()*editor0ZoomValue)/2;
                sprCY+=animationCollisionCircleY->get_held_number() * editorZoomValue;

                int sprCR = animationCollisionCircleR->get_held_number() * editorZoomValue;
                sprCX-=sprCR;
                sprCY-=sprCR;
                if( sprCX >= 0 && sprCY >=0 && sprCR > 0)
                {
                    gpe::gcanvas->render_circle_color( sprCX,sprCY,sprCR, gpe::c_lime,192);
                }
            }
        }

        process_data_fields( gpe::version_number_total);

        //renders the animation in edit
        //renders the frame boxes for the animation
        animInEditor->render_piece_resized( animationPreviewCam->x, animationPreviewCam->y, animCameraRect.w* editorZoomValue, animCameraRect.h* editorZoomValue, &animCameraRect );


        int iAnimFrame = 0, jAnimRow = 0;
        int rectanglesRendered = 0;
        int maxAnimationFrames = animationDataFields[ gpe::anim_frame_count]->get_held_number();
        animatonPreviewRect->x = animationDataFields[ gpe::anim_pixel_offset_hori]->get_held_number();
        animatonPreviewRect->y = animationDataFields[ gpe::anim_pixel_offset_hori]->get_held_number();
        animatonPreviewRect->w = animationDataFields[ gpe::anim_frame_width]->get_held_number();
        animatonPreviewRect->h = animationDataFields[ gpe::anim_frame_height]->get_held_number();

        int incrementWidth = animatonPreviewRect->w  + animationDataFields[ gpe::anim_pixel_padding_hori]->get_held_number();
        int incrementHeight = animatonPreviewRect->h + animationDataFields[ gpe::anim_pixel_padding_vert]->get_held_number();
        int iTSX = 0;
        int bx1 = 0;
        int bx2 = 0;
        int by1 = 0;
        int by2 = 0;

        //Make sure we aren't gonna be stuck in a forever loop
        if( incrementWidth > 0 && incrementHeight > 0)
        {
            for( iAnimFrame = 0; iAnimFrame < maxAnimationFrames; iAnimFrame++)
            {
                bx1 = (animationPreviewCam->x+animatonPreviewRect->x - animCameraRect.x ) *editorZoomValue;
                //bx1 = bound_number(bx1,0, widget_box.w);

                by1 = (animationPreviewCam->y+animatonPreviewRect->y - animCameraRect.y)*editorZoomValue;
                //by1 = bound_number(by1,0, widget_box.h);

                bx2 = ( animationPreviewCam->x + animatonPreviewRect->x + animatonPreviewRect->w - animCameraRect.x ) * editorZoomValue;
                //bx2 = bound_number(bx2,0, widget_box.w);

                by2 = ( animationPreviewCam->y + animatonPreviewRect->y + animatonPreviewRect->h - animCameraRect.y ) * editorZoomValue;
                //by2 = bound_number(by2,0, widget_box.h);
                gpe::gcanvas->render_rectangle( bx1-cam->x, by1-cam->y, bx2-cam->x, by2-cam->y, gpe::c_black,true,255);
                rectanglesRendered++;

                if( previewSubImageNumbers->is_clicked() && editorZoomValue > 0.40 )
                {
                    gpe::gfs->render_text( bx1-cam->x+pawgui::padding_default, by2-4-cam->y, stg_ex::int_to_string(iAnimFrame), pawgui::theme_main->main_box_font_highlight_color, gpe::font_default, gpe::fa_left, gpe::fa_bottom );
                }

                animationPreviewCam->x+= incrementWidth;
                if( animationPreviewCam->x >= animInEditor->get_texture_width() )
                {
                    animatonPreviewRect->y += incrementHeight;
                }
            }
        }
    }

    if( previewZoomLevel!=NULL && previewZoomLevel->is_clicked() )
    {
        gpe::gfs->render_text_boxed( view_space->w-32,view_space->h-32,
                          "Zoom Level: "+ stg_ex::float_to_string(editorZoomValue*100 )+"%",
                          pawgui::theme_main->text_box_font_color, gpe::c_black, gpe::font_default, gpe::fa_right, gpe::fa_bottom);
    }

    if( animXScroll!=NULL)
    {
        animXScroll->render_self( view_space,cam);
    }
    if( animYScroll!=NULL)
    {
        animYScroll->render_self( view_space,cam);
    }
}


void animationResource::revert_data_fields()
{
    for( int i = 0; i < ANIMATION_DATA_FIELD_COUNT; i++ )
    {
        if( animationDataFields[i]!=NULL)
        {
            animationDataFields[i]->switch_inuse( false );
        }
        else
        {
            gpe::error_log->report("animation Detected to have NULL field at ["+ stg_ex::int_to_string(i)+"]");
            return;
        }
    }
    if( animInEditor!=NULL )
    {
        animationDataFields[ gpe::anim_frame_count]->set_number( animInEditor->get_frame_count() );
        animationDataFields[ gpe::anim_frame_width]->set_number( animInEditor->get_width() );
        animationDataFields[ gpe::anim_frame_height]->set_number( animInEditor->get_height() );
        animationDataFields[ gpe::anim_pixel_offset_hori]->set_number( animInEditor->get_frame_xoffset() );
        animationDataFields[ gpe::anim_pixel_offset_vert]->set_number( animInEditor->get_frame_yoffset() );
        animationDataFields[ gpe::anim_pixel_padding_hori]->set_number( animInEditor->get_hpadding() );
        animationDataFields[ gpe::anim_pixel_padding_vert]->set_number( animInEditor->get_vpadding() );
    }
}

void animationResource::save_resource(std::string file_path, int backupId)
{
    if( pawgui::main_loader_display != NULL )
    {
        pawgui::main_loader_display->update_submessages( "Processing Animation", resource_name );
    }
    std::string newSaveDataFilename = "";
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
        soughtDir = stg_ex::file_to_dir(parentProjectName)+"/gpe_project/resources/animations/";
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

            if( animInEditor!=NULL)
            {
                for( int i =0; i < ANIMATION_DATA_FIELD_COUNT; i++)
                {
                    newSaveDataFile << "animationData["<< stg_ex::int_to_string(i)<< "]=";

                    if( animationDataFields[i]!=NULL)
                    {
                        newSaveDataFile << stg_ex::int_to_string( animationDataFields[i]->get_held_number() )<<"\n";
                    }
                    else
                    {
                        newSaveDataFile << "0\n";
                    }
                }
                newSaveDataFile << "SubimageInView=" << stg_ex::int_to_string( subImagePreviewNumber) << "\n";
                if( preloadCheckBox!=NULL)
                {
                    newSaveDataFile << "Preload="+ stg_ex::int_to_string(preloadCheckBox->is_clicked() )+"\n";
                }
                if( subImageEditorPreviewId!=NULL)
                {
                    newSaveDataFile << "EditorPreviewFrame="+ stg_ex::int_to_string(subImageEditorPreviewId->get_held_number() )+"\n";
                }
                if( showCollisionShapeCheckBox!=NULL)
                {
                    newSaveDataFile << "PreviewCollisionShape="+ stg_ex::int_to_string(showCollisionShapeCheckBox->is_clicked() )+"\n";
                }
                newSaveDataFile << "CollisionShape="<< stg_ex::int_to_string (animationCollisionShapeMenu->get_selected_id() ) +"\n";

                newSaveDataFile << "CircleCollisionRegion=" << stg_ex::int_to_string (animationCollisionCircleX->get_held_number() ) +",";
                newSaveDataFile << stg_ex::int_to_string (animationCollisionCircleY->get_held_number() ) +",";
                newSaveDataFile << stg_ex::int_to_string (animationCollisionCircleR->get_held_number() )+"\n";
                //Rectangle info

                newSaveDataFile << "RectangleCollisionBox=" << stg_ex::int_to_string (animationCollisionRectX->get_held_number() ) +",";
                newSaveDataFile << stg_ex::int_to_string (animationCollisionRectY->get_held_number() ) +",";
                newSaveDataFile << stg_ex::int_to_string (animationCollisionRectW->get_held_number() ) +",";
                newSaveDataFile << stg_ex::int_to_string (animationCollisionRectH->get_held_number() )+"\n";
                std::string resFileCopySrc = animInEditor->get_file_name();
                std::string resFileLocation = stg_ex::get_short_filename (resFileCopySrc,true );
                newSaveDataFile << "ImageLocation="+resFileLocation+"\n";
                if( (int)resFileLocation.size() > 0 )
                {
                    std::string resFileCopyDest = soughtDir+resFileLocation;
                    if( sff_ex::file_exists(resFileCopyDest) )
                    {
                        /*
                        if( pawgui::display_prompt_message("[WARNING]animation Image File Already exists?","Are you sure you will like to overwrite your ["+resFileLocation+"] animation Image File? This action is irreversible!")==pawgui::display_query_yes)
                        {
                            file_copy(resFileCopySrc,resFileCopyDest);
                        }
                        */
                    }
                    else
                    {
                        sff_ex::file_copy(resFileCopySrc,resFileCopyDest);
                    }
                }
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

void animationResource::update_box(int newX, int newY, int newW, int newH)
{
    if( newX!=-1)
    {
        encapBox.x = newX;
    }
    if(newY!=-1)
    {
        encapBox.y = newY;
    }

    if( newW!=-1)
    {
        encapBox.w = newW;
    }
    if(newH!=-1)
    {
        encapBox.h = newH;
    }
}

bool animationResource::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn)
{
    if( fileTarget!=NULL)
    {
        if( fileTarget->is_open() )
        {
            std::string nestedTabsStr = pawgui::generate_tabs( nestedFoldersIn );
            *fileTarget << nestedTabsStr << "animation=" << resource_name << "," << get_global_rid() << ",";
            *fileTarget << "\n";
            //save_resource();
            return true;
        }
    }
    return false;
}

