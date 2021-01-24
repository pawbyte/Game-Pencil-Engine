/*
light_resource.h
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

#ifndef GPE_LIGHT_RESOURCE_H
#define GPE_LIGHT_RESOURCE_H


#include "../gpe/gpe_lights2d.h"
#include "gpe_basic_resource_page.h"


class lightResource: public standardEditableGameResource
{
private:
    gpe::light_basic_2d * myAmbientLight;
    gpe::light_direction2d * myDirectionLight;
    gpe::light_point2d * myPointLight;
    float areaMouseXPos, areaMouseYPos;
public:
    float tempAngleExtra;
    pawgui::widget_radio_button_controller * lightType;
    pawgui::gpe_widget_color_picker * lightColorField;
    pawgui::widget_input_number * lightIntensityField;

    //Point Light Related Variables [ BEGIN ]
    pawgui::widget_input_number * lightRadiusField;
    pawgui::widget_input_number * lightSecondRadiusField;
    pawgui::widget_input_number * lightThirdRadiusField;
    pawgui::widget_input_number * lightSecondIntensityField;
    pawgui::widget_input_number * lightThirdIntensityField;
    //Point Light Related Variables [ END ]

    //Direction Light Related Variables [ END ]
    pawgui::widget_input_number * lightLengthField;
    pawgui::widget_input_number * lightWidthField;
    pawgui::widget_input_number * lightDirectionField;
     //Texture related variables
    gpe::texture_base * directionLightTexture;
    pawgui::widget_label_text  * textureLabel;
    pawgui::widget_label_image * texturePreviewImgLabel;
    pawgui::widget_input_text * textureLocationField;
    pawgui::widget_button_label * browseTextureButton;
    pawgui::widget_button_label * clearTextureButton;
    pawgui::widget_checkbox * autorotateAngle;
    //Direction Light Related Variables [ END ]


    //Light Flickering variables
    pawgui::widget_checkbox * lightUseFlicker;
    pawgui::widget_input_number * lightFlickerTimeField;
    pawgui::widget_input_number * lightFlickerAmountField;

    lightResource( pawgui::widget_resource_container * pFolder = NULL );
    ~lightResource();

    bool build_intohtml5_file(std::ofstream * fileTarget, int leftTabAmount = 0);
    bool build_intocpp_file(std::ofstream * fileTarget, int leftTabAmount = 0);

    void compile_cpp();
    bool include_local_files( std::string pBuildDir , int buildType );
    bool get_mouse_coords( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    void handle_scrolling();
    bool is_build_ready();

    void load_image(std::string new_file_name, bool autoProcess = false);
    void load_resource(std::string file_path = "");
    void prerender_self( );
    void process_data_fields(float versionToProcess = -1);
    void process_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    void render_self( gpe::shape_rect * view_space = NULL, gpe::shape_rect * cam = NULL);
    void revert_data_fields();
    void save_resource(std::string file_path = "", int backupId = -1);
    void update_box(int newX=-1, int newY=-1, int newW=-1, int newH=-1);
    bool write_data_into_projectfile(std::ofstream * fileTarget, int nestedFoldersIn = 0);
};

#endif // GPE_LIGHT_RESOURCE_H

