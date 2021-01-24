/*
texture_base.cpp
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

#include "gpe_texture_base.h"

namespace gpe
{
    texture_base::texture_base()
    {
        texId=-1;
        texWid = 0;
        texHeight = 0;
        currentR = 255;
        currentG = 255;
        currentB = 255;

        lastAlphaRendered = 255;
        currentBlendMode = -1;
        fileLocation = "";
        isTransparent = false;
        textureType = "base";
    }

    texture_base::~texture_base()
    {

    }

    void texture_base::change_alpha( uint8_t alpha )
    {

    }

    void texture_base::change_color( color * newColor)
    {

    }

    void texture_base::change_color( uint8_t red, uint8_t green, uint8_t blue )
    {

    }

    bool texture_base::copy_image_source(std::string outDirectoryName)
    {
        return false;
    }

    int texture_base::get_id()
    {
        return texId;
    }

    std::string texture_base::get_filename()
    {
        return fileLocation;
    }

    int texture_base::get_height()
    {
        return texHeight;
    }

    std::string texture_base::get_name()
    {
        return name;
    }

    int texture_base::get_width()
    {
        return texWid;
    }

    std::string texture_base::get_texture_type()
    {
        return textureType;
    }

    void texture_base::load_new_texture( renderer_base * renderer, std::string fileName, int id , bool transparent , bool useLinearScaling )
    {

    }

    void texture_base::prerender_circle( renderer_base * renderer,int rad, color * circleColor,  uint8_t alpha ,int id, bool transparent, bool useLinearScaling, bool isOutline )
    {

    }

    void texture_base::prerender_triangle( renderer_base * renderer, shape_triangle2d,  color * circleColor,  uint8_t alpha )
    {

    }

    void texture_base::prerender_rectangle( renderer_base * renderer,int w, int h, color * newColor, int id , bool transparent, bool useLinearScaling, bool isOutline  )
    {

    }

    texture_base * texture_base::create_new()
    {
        return new texture_base();
    }


    void texture_base::render_align(  int x, int y, int hAlign, int vAlign, shape_rect* clip, color * rendColor, int alpha )
    {

    }

    void texture_base::render_align_resized(  int x, int y,int newWidth, int newHeight, int hAlign, int vAlign, shape_rect* clip , color * rendColor , int alpha)
    {

    }

    void texture_base::texture_base::render_tex(  int x, int y, shape_rect* clip, int alpha  )
    {

    }

    void texture_base::render_tex_colored(  int x, int y, color * rendColor, int alpha, shape_rect* clip )
    {

    }

    void texture_base::render_tex_resized(  int x, int y,float newWidth, float newHeight, shape_rect* clip, color * rendColor, int alpha)
    {

    }

    void texture_base::render_tex_scaled(  int x, int y,float xScale, float yScale, shape_rect* clip, color * rendColor, int alpha )
    {

    }

    void texture_base::render_tex_rotated(  int x, int y, float renderAngle,  color * renderColor, shape_rect* clip , int alpha )
    {

    }

    void texture_base::render_tex_rotated_at_point(  int x, int y, float renderAngle, int pointX, int pointY, color * renderColor , shape_rect* clip, int alpha )
    {

    }

    void texture_base::render_tex_special(  int x, int y, float renderAngle, int newWidth , int newHeight, color * renderColor, shape_rect* clip , int alpha )
    {

    }

    void texture_base::render_tex_special_at_point(  int x, int y, float renderAngle, int pointX, int pointY,int newWidth, int newHeight, color * renderColor, shape_rect* clip, int alpha )
    {

    }


    void texture_base::set_alpha( int alpha)
    {

    }


    void texture_base::set_blend_mode( int newBlendMode)
    {

    }
}
