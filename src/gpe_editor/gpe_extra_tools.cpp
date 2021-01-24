#include "gpe_extra_tools.h"

gamePencilExtraTools * main_EXTRA_TOOLS = NULL;

atlas_data_viewer::atlas_data_viewer( gpe::asset_manager * manager )
{
    if( manager != NULL )
    {
        gpe::error_log->report("Atlas data viewer...");
        atlas_title = new pawgui::widget_label_title( manager->get_name(), manager->get_name() );
        gpe::error_log->report("Title loaded...");

        atlas_text_animation = new pawgui::widget_label_text ("Animations count("+stg_ex::int_to_string( manager->get_size_animations() )+")","Last added animation: "+manager->get_last_animation_name() );
        gpe::error_log->report("Title loaded...");

        atlas_text_music = new pawgui::widget_label_text ( "Music count("+ stg_ex::int_to_string( manager->get_size_music() )+")","Last added audio: "+manager->get_last_music_name()  );
        gpe::error_log->report("Music loaded...");

        atlas_text_sounds = new pawgui::widget_label_text ( "Sounds count("+ stg_ex::int_to_string( manager->get_size_sounds() )+")","Last added audio: "+manager->get_last_sound_name()  );
        gpe::error_log->report("Sounds loaded...");

        atlas_text_textures = new pawgui::widget_label_text ( "Textures count("+ stg_ex::int_to_string( manager->get_size_textures() )+")","Last added texture: "+manager->get_last_texture_name()  );
        gpe::error_log->report("Textures loaded...");

        atlas_text_tilesheets = new pawgui::widget_label_text ( "Tilesheets count("+ stg_ex::int_to_string( manager->get_size_tilesheets() )+")","Last added tilesheet: "+manager->get_last_tilesheet_name()  );
        gpe::error_log->report("Tilesheets loaded...");
    }
    else
    {
        atlas_title = NULL;
        atlas_text_animation = NULL;
        atlas_text_music = NULL;
        atlas_text_sounds = NULL;
        atlas_text_textures = NULL;
        atlas_text_tilesheets = NULL;
    }
}

atlas_data_viewer::~atlas_data_viewer()
{
    if( atlas_title != NULL )
    {
        delete atlas_title;
        atlas_title = NULL;
    }

    if( atlas_text_animation != NULL )
    {
        delete atlas_text_animation;
        atlas_text_animation = NULL;
    }
    if( atlas_text_music != NULL )
    {
        delete atlas_text_music;
        atlas_text_music = NULL;
    }
    if( atlas_text_sounds != NULL )
    {
        delete atlas_text_sounds;
        atlas_text_sounds = NULL;
    }

    if( atlas_text_textures != NULL )
    {
        delete atlas_text_textures;
        atlas_text_textures = NULL;
    }

    if( atlas_text_tilesheets != NULL )
    {
        delete atlas_text_tilesheets;
        atlas_text_tilesheets = NULL;
    }

}

void atlas_data_viewer::add_to_list( pawgui::widget_panel_list * element_list )
{
    if( element_list == NULL )
    {
        return;
    }
    element_list->add_gui_element( atlas_title, true );
    element_list->add_gui_element( atlas_text_animation, true );
    element_list->add_gui_element( atlas_text_music, true );
    element_list->add_gui_element( atlas_text_sounds, true );
    element_list->add_gui_element( atlas_text_textures, true );
    element_list->add_gui_element( atlas_text_tilesheets, true );
}

gamePencilExtraTools::gamePencilExtraTools()
{
    isFullScreenResource = true;
    resourceFileName = "";
    resource_name = "JS Compiler Settings";
    resource_name = "Extra Tools";
    parentProjectName = "";
    parentProjectDirectory = "";

    sideAreaPanel = new pawgui::widget_selectbox("Mode");
    sideAreaPanel->set_width(160);
    sideAreaPanel->set_option_height(64);
    //sideAreaPanel->add_option("General",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/wrench.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Image Editing",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/image.png"),NULL,2, false, false);
    sideAreaPanel->add_option("Data Info",-1,pawgui::rsm_gui->texture_add_filename( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/question-circle.png"),NULL,2, false, false);

    sidePanelRect = new gpe::shape_rect();

    editorPageList = new pawgui::widget_panel_list();
    editorPageList->barXPadding = pawgui::padding_default;
    editorPageList->panelAlignType = pawgui::panel_align_left;
    editorPageList->barXMargin = 0;

    sectionTitleExtras = new pawgui::widget_label_title("Extras","Extras");
    transformImageDirectoryButton = new pawgui::widget_button_push( gpe::app_directory_name+"resources/gfx/iconpacks/fontawesome/magic.png","Transform IMG Folder","Transforms all Images in a given folder",-1);
    refreshAtlasRefreshButton = new pawgui::widget_button_label("Refresh Atlas Info","Hit to refresh the atlas info..." );
    refresh_atlas_info();
}

gamePencilExtraTools::~gamePencilExtraTools()
{
    if( sectionTitleExtras!=NULL )
    {
        delete sectionTitleExtras;
        sectionTitleExtras = NULL;
    }

    if( transformImageDirectoryButton!=NULL )
    {
        delete transformImageDirectoryButton;
        transformImageDirectoryButton = NULL;
    }
}

bool gamePencilExtraTools::include_local_files( std::string pBuildDir , int buildType )
{

}

void gamePencilExtraTools::load_resource(std::string file_path)
{

}

int gamePencilExtraTools::modify_folder_images(std::string folderLocation, int modifcationType)
{
    int imagesModifiedCount = -1;
    if( sff_ex::path_exists(folderLocation) && modifcationType >=0 && modifcationType < 3)
    {
        gpe::GPE_FileDirectory * foundFolder = new gpe::GPE_FileDirectory();
        foundFolder->open_directory_sorted(folderLocation);

        gpe::color * colorToRemove = gpe::c_fuchsia->duplicate_color();
        bool continueWithAction = false;
        imagesModifiedCount = 0;
        if( foundFolder->get_count() > 0 )
        {
            if( modifcationType==0)
            {
                if( pawgui::get_color_from_popup("Image Background Color To Remove",colorToRemove) )
                {
                    if( pawgui::display_prompt_message("Are you sure you want to erase this Color from all images in this folder?","This action is irreversible and will change your image's format to a .png file!")==pawgui::display_query_yes)
                    {
                        continueWithAction = true;
                    }
                }
            }
            else if( modifcationType==1 )
            {
                if( pawgui::display_prompt_message("Are you sure you want to invert your all images in this folder?","This action is irreversible and will change your image's format to a .png file!")==pawgui::display_query_yes)
                {
                    continueWithAction = true;
                }
            }
            else if( modifcationType==2 )
            {
                if( pawgui::display_prompt_message("Are you sure you want to grayscale all images in this folder?","This action is irreversible and will change your image's format to a .png file!")==pawgui::display_query_yes)
                {
                    continueWithAction = true;
                }
            }
            if( continueWithAction)
            {
                std::string newImageName = "";
                gpe::GPE_File * tempFile = NULL;
                SDL_Surface * oTempSurface = NULL;
                SDL_Surface *nTempSurface = NULL;
                for( int i = 0; i < foundFolder->get_count(); i++)
                {
                    tempFile = foundFolder->get_file(i);
                    if( tempFile!=NULL)
                    {
                        if( tempFile->get_type()=="bmp" || tempFile->get_type()=="png")
                        {
                            newImageName = folderLocation+"/"+tempFile->get_name();
                            oTempSurface = sdl_surface_ex::load_surface_image( newImageName.c_str() );
                            if( oTempSurface->w > 0 && oTempSurface->h > 0)
                            {
                                nTempSurface = NULL;
                                if( oTempSurface!=NULL)
                                {
                                    gpe::error_log->report("Modifying image at: "+newImageName+".");

                                    if( pawgui::main_loader_display != NULL )
                                    {
                                        pawgui::main_loader_display->update_messages( "Modifying Image", tempFile->get_name(),"Please wait..." );
                                    }

                                    if( modifcationType==0)
                                    {
                                        if( pawgui::get_color_from_popup("Image Background Color To Remove",colorToRemove) )
                                        {
                                            if( pawgui::display_prompt_message("Are you sure you want to erase this Color from this image?","This action is irreversible and will change your image's format to a .png file!")==pawgui::display_query_yes)
                                            {
                                                nTempSurface= sdl_surface_ex::surface_remove_color_rgba(oTempSurface, colorToRemove->get_r(), colorToRemove->get_r(), colorToRemove->get_b() );

                                            }
                                        }
                                    }
                                    else if( modifcationType==1 )
                                    {
                                        nTempSurface= sdl_surface_ex::surface_invert(oTempSurface);
                                    }
                                    else if( modifcationType==2 )
                                    {
                                        nTempSurface= sdl_surface_ex::surface_grayscale(oTempSurface);
                                    }
                                    if( nTempSurface!=NULL)
                                    {
                                        if( tempFile->get_type()=="bmp" || tempFile->get_type()=="png")
                                        {
                                            SDL_SaveBMP(nTempSurface,newImageName.c_str() );
                                        }
                                        else if( tempFile->get_type()=="png")
                                        {
                                            IMG_SavePNG(nTempSurface,newImageName.c_str() );
                                        }
                                        SDL_FreeSurface(nTempSurface);
                                        nTempSurface = NULL;
                                        imagesModifiedCount++;
                                    }
                                    SDL_FreeSurface(oTempSurface);
                                    oTempSurface = NULL;
                                }
                            }
                        }
                    }
                }
            }
        }
        foundFolder->close_directory();
        delete foundFolder;
        if( colorToRemove!=NULL)
        {
            delete colorToRemove;
            colorToRemove = NULL;
        }
    }
    return imagesModifiedCount;
}

void gamePencilExtraTools::prerender_self( )
{

}

void gamePencilExtraTools::process_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
{
    cam = gpe::camera_find(cam);
    view_space = gpe::camera_find(view_space);
    if( cam!=NULL && editorPageList!=NULL && view_space!=NULL)
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
            sideAreaPanel->set_width( 192 );
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
        if( sideAreaPanel->get_selected_name()=="General")
        {

        }
        else if( sideAreaPanel->get_selected_name()=="Image Editing")
        {
            editorPageList->add_gui_element( transformImageDirectoryButton, true );
            editorPageList->process_self( view_space,cam);

            if( transformImageDirectoryButton->is_clicked()  )
            {
                pawgui::context_menu_open(-1,-1,256);
                pawgui::main_context_menu->add_menu_option("Erase BG Color On All Images",0);
                pawgui::main_context_menu->add_menu_option("Invert Colors On All Images",1);
                pawgui::main_context_menu->add_menu_option("Make GrayScale On All Images",2);
                pawgui::main_context_menu->add_menu_option("Exit Menu",10);
                int menuSelection = pawgui::context_menu_process();
                if( menuSelection>=0 && menuSelection <=3)
                {
                    std::string lastDirectory = "";
                    std::string directoryToMod = pawgui::get_directory_name_from_popup("Transform All Images in this directory", lastDirectory,false);
                    int imagesModified = modify_folder_images(directoryToMod,menuSelection);
                }
            }
        }
        else if( sideAreaPanel->get_selected_name() == "Data Info" )
        {
            atlas_data_viewer * current_atlas = NULL;
            for( int iAtlas = 0; iAtlas < (int)atlas_list.size(); iAtlas++ )
            {
                current_atlas = atlas_list[iAtlas];
                if( current_atlas!=NULL )
                {
                    current_atlas->add_to_list( editorPageList );
                }
            }
            editorPageList->add_gui_element( refreshAtlasRefreshButton, true );
            editorPageList->process_self( view_space,cam);

            if( refreshAtlasRefreshButton!=NULL && refreshAtlasRefreshButton->is_clicked() )
            {
                refresh_atlas_info();
            }
        }
    }
}

void gamePencilExtraTools::refresh_atlas_info()
{
    gpe::error_log->report("Refreshing atlas info...");
    if( editorPageList != NULL )
    {
        editorPageList->clear_list();
    }
    gpe::error_log->report("Page list cleared...");

    int iAtlasCount = (int)atlas_list.size();
    atlas_data_viewer * current_atlas = NULL;
    for( int iAtlas = iAtlasCount -1; iAtlas >=0; iAtlas-- )
    {
        current_atlas = atlas_list[iAtlas ];
        if( current_atlas != NULL )
        {
            delete current_atlas;
            current_atlas = NULL;
        }
    }
    atlas_list.clear();
    gpe::error_log->report("atlas list cleared");

    current_atlas = new atlas_data_viewer( gpe::rsm );
    atlas_list.push_back( current_atlas );
    gpe::error_log->report("gpe::rsm atlas viewer added...");

    current_atlas = new atlas_data_viewer( pawgui::rsm_gui );
    atlas_list.push_back( current_atlas );
    gpe::error_log->report("pawgui::rsm_gui atlas viewer added...");
}

void gamePencilExtraTools::render_self( gpe::shape_rect * view_space, gpe::shape_rect * cam)
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

void gamePencilExtraTools::save_resource(std::string file_path , int backupId )
{

}

bool gamePencilExtraTools::write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn )
{

}
