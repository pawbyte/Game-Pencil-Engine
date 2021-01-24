/*
gpe_artist_sdl.h
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

#ifndef gpe_artist_sdl_h
#define gpe_artist_sdl_h

#include "../gpe/gpe_artist_base.h"
#include "../gpe/gpe_common_includes.h"
#include "../gpe/gpe_globals.h"
#include "../gpe/gpe_camera.h"
#include "../gpe/gpe_shared_resources.h"
#include "../other_libs/semath.h"

#include "gpe_renderer_sdl.h"
#include "sdl_surface_ex.h"
#include "gpe_texture_sdl.h"
#include "gpe_texture_target_sdl.h"

namespace gpe
{
    const int render_points_giant_size = 65536;
    //The Master class to render things to textures/the screen
    class artist_sdl: public artist_base
    {
        protected:
            SDL_Renderer * sdlRenderer;
            renderer_system_sdl * gpeSDLRenderer;
            texture_sdl * prerenderedSquare;
            std::vector<texture_sdl * >prerenderedCircles;
            std::vector<texture_sdl * >prerenderedCirclesOutlines;
            SDL_Texture * geometry_texture;
            shape_point2d triangle_midpoint;
            SDL_Point line_render_points[ render_points_giant_size];
            SDL_FRect rect_render_points[ render_points_giant_size];
            int line_render_point_position;
            int render_triangle_flatbottom( float x1, float y1, float x2, float y2, float x3, float y3 );
            int render_triangle_flattop( float x1, float y1, float x2, float y2, float x3, float y3 );
        public:
            artist_sdl(renderer_system_sdl * aRenderer);
            ~artist_sdl();

             void clear_render_points();
            //Circle and SemiCircles and Ovals Rendering Functions
             bool render_circle( int x, int y, int rad, bool renderOutLine = false);
             bool render_circle_color( int x, int y, int rad, color * rendColor = NULL, int alphaChannel = 255,bool renderOutLine = false);

             bool render_oval( int x, int y, int w, int h, bool renderOutLine = false);

             bool render_oval_color( int x, int y, int w, int h, color *rendColor, int alphaChannel = 255, bool renderOutLine = false);
             bool render_semi_circle( int x, int y, int rad, int direction = 0, bool renderOutLine = false);
             bool render_semi_circle_color( int x, int y, int rad, int direction = 0, color *rendColor = NULL, int alphaChannel = 255, bool renderOutLine = false);


            //Gradients Rendering [ BEGIN ]
             void render_gradient_circle(  int radius, shape_rect * rendRect = NULL, color * rendColor = NULL);
             void render_gradient_horizontal(  shape_rect * rendRect = NULL, color * rendColor = NULL);
             void render_gradient_vertical(  shape_rect * rendRect = NULL, color * rendColor = NULL);
            //Gradients Rendering [ END ]

            //Line Rendering [ BEGIN ]
             void render_horizontal_line(int y, int x1, int x2);
             void render_horizontal_line_color( int y, int x1, int x2, color *rendColor, int alphaChannel = 255);

             void render_line(  int x1, int y1, int x2, int y2 );
             void render_line_color(  int x1, int y1, int x2, int y2,  color *rendColor, int alphaChannel = 255);
             void render_line_capped(  int x1, int y1, int x2, int y2, int lineWidth );
             void render_line_capped_color(  int x1, int y1, int x2, int y2, int lineWidth, color *rendColor, int alphaChannel = 255);
             void render_line_width(  int x1, int y1, int x2, int y2, int lineWidth);
             void render_line_width_color(  int x1, int y1, int x2, int y2, int lineWidth, color *rendColor, int alphaChannel = 255);

             void render_vertical_line( int x, int y1, int y2);
             void render_vertical_line_color( int x, int y1, int y2, color *rendColor, int alphaChannel = 255);
            //Line Rendering [ END ]

            //Points Rendering [ BEGIN ]
             void render_point( int x, int y);
             void render_point_color( int x, int y, color *rendColor, int alphaChannel = 255);
             //Points Rendering [ END ]

            //Rectangles Rendering [ BEGIN ]
             void render_rect(  shape_rect * rendRect,color * rendColor,bool outline = false, int alphaChannel = 255);
             void render_rectangle( int x1, int y1, int x2, int y2,  color *rendColor,bool outline = false, int alphaChannel = 255);
             void render_square( int x, int y, int squareSize,  color *rendColor,bool outline = false, int alphaChannel = 255);
             void render_rotated_rectangle(int get_center(), int yCenter, int w, int h, int angle, color * rendColor = NULL, int alphaChannel = 255);
             void render_roundrect(int x1, int y1, int x2, int y2, int rad, bool outline = false);
             void render_roundrect_color( int x1, int y1,int x2, int y2, int rad, bool outline = false,color * rendColor = NULL, int alphaChannel  = 255);
            //Rectangles Rendering [ END ]

             //Triangle Rendering [ BEGIN ]
             void render_triangle( shape_triangle2d * tri );
             void render_triangle_color( shape_triangle2d * tri, color * rendColor = NULL, int alphaChannel = 255 );

             void render_triangle_coords( int x1, int y1, int x2, int y2, int x3, int y3);
             void render_triangle_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * rendColor = NULL, int alphaChannel = 255 );

             void render_triangle_outline( shape_triangle2d * tri, int lineWidth = 1);
             void render_triangle_outline_color( shape_triangle2d * tri, color * rendColor = NULL, int alphaChannel = 255,int lineWidth = 1);

             void render_triangle_outline_coords( int x1, int y1, int x2, int y2, int x3, int y3, int lineWidth = 1);
             void render_triangle_outline_color_coords( int x1, int y1, int x2, int y2, int x3, int y3, color * rendColor = NULL, int alphaChannel = 255,int lineWidth = 1);

             //Triangle Rendering [ END ]

            //Color / blend related functions
             void set_color( color * color);
             void set_color( int r, int g, int b );
             void set_render_alpha( int newAlpha = 255);
             void set_line_width( int newWidth = 1);
             void set_artist_blend_mode( int newBlendMode );
            //Render Mode Related Functions [ END ]

            //Lighting Overlay [ BEGIN ]
             void render_ligting_overlay( int x, int y);
             void render_ligting_overlay_scaled( int x, int y, float scaleSize);
             void resize_ligting_overlay( int w, int h);
             void switch_ligting_overlay(  bool on  = true );
            //Lighting Overlay [ END ]
    };
}

#endif //gpe_artist_sdl_h
