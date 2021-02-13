/*
gpe_font_sdl.cpp
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


#include "gpe_font_sdl.h"

namespace gpe
{
    font_pair_sdl::font_pair_sdl(TTF_Font * fontIn,std::string strIn)
    {
        lastAlphaRendered = 255;
        str = strIn;
        strTexture = NULL;
        text_width = 0;
        text_height = 0;
        if( fontIn!=NULL && (int)strIn.size() > 0)
        {
            //Render text surface
            SDL_Color sdl_color_white = {255, 255, 255,255};
            SDL_Surface * textSurface = textSurface = TTF_RenderUTF8_Blended( fontIn, strIn.c_str(),sdl_color_white );
            if( textSurface == NULL )
            {
                std::string ttfError = TTF_GetError();
                gpe::error_log->report( "Unable to render text surface! Error:  " + ttfError );
            }
            else
            {
                //Create texture from surface pixels
                text_width = textSurface->w;
                text_height = textSurface->h;
                strTexture = sdl_surface_ex::create_texture_from_surface( renderer_main_sdl->get_sdl_renderer(), textSurface);
                if( strTexture != NULL )
                {
                    //Get image dimensions
                    SDL_QueryTexture(strTexture,NULL, NULL,&text_width, &text_height);
                    //SDL_SetTextureBlendMode(strTexture, SDL_BLENDMODE_ADD );
                }
                else
                {
                    std::string sdlError = SDL_GetError();
                    gpe::error_log->report( "Unable to create texture from rendered text! SDL Error: "+ sdlError  );
                }
            }
        }
        else
        {
            gpe::error_log->report("Unable to pre-render text [" + strIn +"]");
        }
    }

    font_pair_sdl::~font_pair_sdl()
    {
        if( strTexture!=NULL)
        {
            SDL_DestroyTexture(strTexture);
            strTexture = NULL;
        }
    }

    SDL_Texture * font_pair_sdl::get_texture()
    {
        return strTexture;
    }

    font_sdl_tff::font_sdl_tff(std::string fFileLocation, int fSize, bool isMonospaced, std::string fNickName,  int fontIdNumb )
    {
        font_system_type = "sdl_ttf";
        last_used_halign = fa_left;
        last_used_valign = fa_top;
        customFontId = fontIdNumb;
        monoWidth = 0;
        monoHeight = 0;
        numberWidth = 0;
        numberHeight = 0;
        fontIsMonoSpaced = isMonospaced;
        heldSDLFont = NULL;
        fontFamilyName = "";
        fontNickName = fNickName;

        if( (int)fFileLocation.size() > 0 )
        {
            if( !sff_ex::file_exists( fFileLocation) )
            {
                gpe::error_log->report("Font-Error: Unable to located [" + fFileLocation + "]!" );
            }
            else
            {
                gpe::error_log->report("Font-Update: Attempting to load font from ["+fFileLocation +"] file!" );
            }
        }
        heldSDLFont = TTF_OpenFontIndex(fFileLocation.c_str(), fSize,0);

        int i = 0;
        if( heldSDLFont!=NULL)
        {
            fontFamilyName = TTF_FontFaceFamilyName(heldSDLFont);
            fontFamilyName+=" "+ stg_ex::int_to_string(fSize)+"pt";
            std::string longCharString = "";
            for( i = 0; i < 256; i++)
            {
                longCharString+=i;
            }

            if( isMonospaced)
            {
                TTF_SetFontHinting(heldSDLFont,TTF_HINTING_MONO);
            }
            else
            {
                TTF_SetFontHinting(heldSDLFont,TTF_HINTING_LIGHT );
            }

            font_pair_sdl * monoSpacedPair = find_texture_sdl("W");
            if( monoSpacedPair!=NULL)
            {
                numberWidth = monoWidth = monoSpacedPair->get_width();
                numberHeight = monoHeight = monoSpacedPair->get_height();
            }

            for( i = 0; i < 10; i++)
            {
                characterPairs[ stg_ex::int_to_string(i)] =  new font_pair_sdl(heldSDLFont, stg_ex::int_to_string(i) );
            }
            characterPairs["." ] = new font_pair_sdl(heldSDLFont,"." );
            characterPairs["," ] =  new font_pair_sdl(heldSDLFont,"," );
            characterPairs["-" ] =  new font_pair_sdl(heldSDLFont,"-" );
            characterPairs["(" ] =  new font_pair_sdl(heldSDLFont,"(" );
            characterPairs[")" ] = new font_pair_sdl(heldSDLFont,")" );
            if( isMonospaced == false)
            {
                std::map<const std::string,font_pair_sdl *>::iterator foundIterator = characterPairs.find("9");
                if( foundIterator!= characterPairs.end() )
                {
                    font_pair_sdl * tempfont_pair_sdl = foundIterator->second;
                    if( tempfont_pair_sdl!=NULL )
                    {
                        if( tempfont_pair_sdl->get_width() > numberWidth)
                        {
                            numberWidth = tempfont_pair_sdl->get_width();
                        }
                        if( tempfont_pair_sdl->get_height() > numberHeight)
                        {
                            numberHeight = tempfont_pair_sdl->get_height();
                        }
                    }
                }
            }
        }
        else if( (int)fFileLocation.size() > 0 )
        {
            gpe::error_log->report("Font-Error: Unable to load [" + fFileLocation + "]!" );
        }
    }

    font_sdl_tff::~font_sdl_tff()
    {
        if( heldSDLFont!=NULL)
        {
            TTF_CloseFont(heldSDLFont);
            heldSDLFont = NULL;
        }
        clear_cache();
        font_pair_sdl * tempfont_pair_sdl = NULL;
        for (std::map<const std::string,font_pair_sdl *>::iterator it=characterPairs.begin(); it!=characterPairs.end(); ++it)
        {
            tempfont_pair_sdl = it->second;
            if( tempfont_pair_sdl!=NULL)
            {
                delete tempfont_pair_sdl;
                tempfont_pair_sdl = NULL;
            }
        }
        characterPairs.clear();

    }

    void font_sdl_tff::clear_cache()
    {
        font_pair_sdl * tempfont_pair_sdl = NULL;
        for (std::map<const std::string,font_pair_sdl *>::iterator it=textPairs.begin(); it!=textPairs.end(); ++it)
        {
            tempfont_pair_sdl = it->second;
            if( tempfont_pair_sdl!=NULL)
            {
                delete tempfont_pair_sdl;
                tempfont_pair_sdl = NULL;
            }
        }
        textPairs.clear();
    }

    font_base * font_sdl_tff::create_new(std::string fFileLocation, int fSize, bool isMonospaced , const std::string fNickName , int fontIdNumb )
    {
        return new font_sdl_tff( fFileLocation, fSize, isMonospaced, fNickName, fontIdNumb );
    }

    int font_sdl_tff::get_cache_count()
    {
        return (int)textPairs.size();
    }

    int font_sdl_tff::get_font_id()
    {
        return customFontId;
    }

    void font_sdl_tff::get_metrics(std::string textToRender, int * wVal, int * hVal)
    {
        int tSize = (int)textToRender.size();
        if( tSize > 0)
        {
            if( fontIsMonoSpaced)
            {
                *wVal = monoWidth*tSize;
                *hVal = monoHeight;
            }
            else if(heldSDLFont!=NULL)
            {
                font_pair_sdl *  newFoundPair = find_texture_sdl( textToRender);
                if( newFoundPair!=NULL)
                {
                    *wVal = newFoundPair->get_width();
                    *hVal = newFoundPair->get_height();
                }

            }
        }
        else
        {
            *wVal = 0;
            *hVal = 0;
        }
    }

    void font_sdl_tff::get_numbered_metrics(std::string textToRender, int * wVal, int * hVal)
    {
        int tSize = (int)textToRender.size();
        if( tSize > 0)
        {
            *wVal = numberWidth*tSize;
            *hVal = numberHeight;
        }
        else
        {
            *wVal = 0;
            *hVal = 0;
        }
    }

    void font_sdl_tff::get_wrapped_string_metrics( const std::string strIn, int lineWidth, int linePadding, int * wVal, int *hVal )
    {
        if( (int)strIn.size() == 0 || lineWidth <=0 )
        {
            return;
        }
        std::vector < std::string > wrappedTextLine;
        stg_ex::wrap_string( strIn, wrappedTextLine, lineWidth);
        if( (int)wrappedTextLine.size() == 0  )
        {
            *wVal = 0;
            *hVal = 0;
        }
        else if( (int)wrappedTextLine.size() == 1 )
        {

        }
        else
        {

        }
    }

    TTF_Font * font_sdl_tff::get_sdl_font()
    {
        return heldSDLFont;
    }

    font_pair_base * font_sdl_tff::find_character_texture( const std::string numbId )
    {
        return find_character_texture_sdl( numbId );
    }

    font_pair_sdl * font_sdl_tff::find_character_texture_sdl( const std::string numbId )
    {
        font_pair_sdl * fPair = NULL;
        SDL_Texture * fSDLTexture = NULL;
        std::map<const std::string,font_pair_sdl *>::iterator foundVal = characterPairs.find( numbId );
        if( foundVal !=  characterPairs.end() )
        {
            fPair = foundVal->second;
            if( fPair!=NULL && fPair->texture_matches(numbId) )
            {
                return fPair;
            }
        }
        else
        {
            if( numbId!=" ")
            {
                fPair = new font_pair_sdl(heldSDLFont,numbId);
                characterPairs[numbId] = fPair;
                return fPair;
            }
        }
        return NULL;
    }

    font_pair_base * font_sdl_tff::find_texture( const std::string textToRender)
    {
        return find_texture_sdl( textToRender );
    }

    font_pair_sdl * font_sdl_tff::find_texture_sdl( const std::string textToRender)
    {
        font_pair_sdl * fPair = NULL;
        SDL_Texture * fSDLTexture = NULL;
        std::map< const std::string,font_pair_sdl *>::iterator foundVal = textPairs.find( textToRender );
        if( foundVal !=  textPairs.end() )
        {
            return foundVal->second;
        }
        else if( textToRender!=" ")
        {
            fPair = new font_pair_sdl(heldSDLFont,textToRender);
            textPairs[textToRender] = fPair;
            return fPair;
        }
        return NULL;
    }

    void font_sdl_tff::render_bitmapped_text( int xPos, int yPos, std::string numberToRender, color * textColor, int hAlign,int vAlign, int renderAlpha )
    {
        int numberSize = (int)numberToRender.size();
        if( renderAlpha > 0 && numberSize >0 )
        {
            switch(hAlign)
            {
            case gpe::fa_center:
                xPos=xPos-(numberWidth*numberSize)/2;
                //last_used_halign=gpe::fa_center;
                break;

            case gpe::fa_right:
                xPos=xPos-(numberWidth*numberSize);
                // last_used_halign=gpe::fa_right;
                break;

            //rendering left will be the default
            default:
                // last_used_halign=gpe::fa_left;
                break;
            }
            switch(vAlign)
            {
            case gpe::fa_middle:
                yPos=yPos-numberHeight/2;
                // last_used_valign=gpe::fa_middle;
                break;

            case gpe::fa_bottom:
                yPos=yPos-numberHeight;
                // last_used_valign=gpe::fa_middle;
                break;

            //rendering left will be the default
            default:
                //last_used_valign=gpe::fa_top;
                break;
            }
            font_pair_sdl * strTex = NULL;
            SDL_Texture * fPairTex = NULL;
            int i = 0;
            std::string currCharacter = "";
            for( i = 0; i < numberSize; i++)
            {
                currCharacter = stg_ex::get_substring(numberToRender,i,1);
                strTex = find_character_texture_sdl( currCharacter );
                if( strTex!=NULL)
                {
                    fPairTex = strTex->get_texture();
                    if( renderAlpha > 255)
                    {
                        renderAlpha = 255;
                    }
                    if( fPairTex!=NULL && renderAlpha >0)
                    {
                        int texWid = 0;
                        int texHeight =0;
                        SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                        if( strTex->lastAlphaRendered!=renderAlpha )
                        {
                            SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                            strTex->lastAlphaRendered = renderAlpha;
                        }
                        //Get image dimensions
                        texWid = strTex->get_width();
                        texHeight = strTex->get_height();

                        if(hAlign<gpe::fa_left || hAlign<0)
                        {
                            hAlign=last_used_halign;
                        }
                        if(vAlign <gpe::fa_top || vAlign<0)
                        {
                            vAlign=last_used_valign;
                        }
                        SDL_Rect clip = {xPos, yPos, texWid,texHeight};
                        SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),fPairTex, NULL, &clip);
                        SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                    }
                }
                xPos+=numberWidth;
            }
        }
    }

    void font_sdl_tff::render_text( int xPos, int yPos, std::string textureText, color * textColor, int hAlign,int vAlign,int renderAlpha )
    {
        if( renderAlpha > 0 && textColor!=NULL)
        {
            //SDL_SetRenderDrawColor( renderer_main_sdl->get_sdl_renderer(), textColor->get_r(), textColor->get_g(), textColor->get_b(), 255 );
            font_pair_sdl * strTex = find_texture_sdl(textureText);
            SDL_Texture * fPairTex=  NULL;
            if( strTex!=NULL)
            {
                fPairTex = strTex->get_texture();

                if( fPairTex!=NULL && renderAlpha >0)
                {
                    if( renderAlpha >= 255)
                    {
                        renderAlpha = 255;
                    }
                    int texWid = 0;
                    int texHeight =0;
                    SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                    if( strTex->lastAlphaRendered!=renderAlpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                        strTex->lastAlphaRendered = renderAlpha;
                    }
                    //Get image dimensions
                    texWid = strTex->get_width();
                    texHeight = strTex->get_height();

                    if(hAlign<gpe::fa_left || hAlign > gpe::fa_right )
                    {
                        hAlign=last_used_halign;
                    }

                    if(vAlign < gpe::fa_top || vAlign > gpe::fa_bottom )
                    {
                        vAlign=last_used_valign;
                    }
                    switch(hAlign)
                    {
                    case gpe::fa_center:
                        xPos=xPos-texWid/2;
                        //last_used_halign=gpe::fa_center;
                        break;

                    case gpe::fa_right:
                        xPos=xPos-texWid;
                        // last_used_halign=gpe::fa_right;
                        break;

                    //rendering left will be the default
                    default:
                        // last_used_halign=gpe::fa_left;
                        break;
                    }
                    switch(vAlign)
                    {
                        case gpe::fa_middle:
                            yPos=yPos-texHeight/2;
                            // last_used_valign=gpe::fa_middle;
                        break;

                        case gpe::fa_bottom:
                            yPos=yPos-texHeight;
                            // last_used_valign=gpe::fa_middle;
                        break;

                        //rendering left will be the default
                        default:
                            //last_used_valign=gpe::fa_top;
                        break;
                        }
                    SDL_Rect clip = {xPos, yPos, texWid,texHeight};
                    SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),fPairTex, NULL, &clip);
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                }
                else
                {
                    gpe::error_log->report("Unable to find sdl_text for ["+textureText+"]...");
                }
            }
            else
            {
                gpe::error_log->report("Unable to render ["+textureText+"]...");
            }
        }
        else
        {
            gpe::error_log->report("Unable to render text  ["+textureText+"]...");
        }
    }

    void font_sdl_tff::render_text_scaled( int xPos, int yPos, std::string textureText, color * textColor, float textScale, int hAlign,int vAlign, int renderAlpha )
    {

    }

    void font_sdl_tff::render_text_resized( int xPos, int yPos, std::string textureText, color * textColor, int hAlign,int vAlign, int rendWid, int rendHeight, int renderAlpha )
    {
        if( renderAlpha > 0)
        {
            font_pair_sdl * strTex = find_texture_sdl(textureText);
            if( strTex!=NULL)
            {
                SDL_Texture * fPairTex = strTex->get_texture();
                if( fPairTex!=NULL)
                {
                    int texWid = 0;
                    int texHeight =0;
                    SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );
                    if( strTex->lastAlphaRendered!=renderAlpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                        strTex->lastAlphaRendered = renderAlpha;
                    }
                    //Get image dimensions
                    texWid = strTex->get_width();
                    texHeight = strTex->get_height();

                    //Get image dimensions
                    if( rendWid<=0 || rendWid > strTex->get_width())
                    {
                        texWid = strTex->get_width();
                    }
                    else
                    {
                        texWid = rendWid;
                    }

                    if( rendHeight<=0 || rendHeight > strTex->get_height() )
                    {
                        texHeight = strTex->get_height();
                    }
                    else
                    {
                        texHeight = rendHeight;
                    }

                    if(hAlign<gpe::fa_left || hAlign > gpe::fa_right )
                    {
                        hAlign=last_used_halign;
                    }

                    if(vAlign < gpe::fa_top || vAlign > gpe::fa_bottom )
                    {
                        vAlign=last_used_valign;
                    }
                    switch(hAlign)
                    {
                    case gpe::fa_center:
                        xPos=xPos-texWid/2;
                        //last_used_halign=gpe::fa_center;
                        break;

                    case gpe::fa_right:
                        xPos=xPos-texWid;
                        // last_used_halign=gpe::fa_right;
                        break;

                    //rendering left will be the default
                    default:
                        // last_used_halign=gpe::fa_left;
                        break;
                    }
                    switch(vAlign)
                    {
                    case gpe::fa_middle:
                        yPos=yPos-texHeight/2;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    case gpe::fa_bottom:
                        yPos=yPos-texHeight;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    //rendering left will be the default
                    default:
                        //last_used_valign=gpe::fa_top;
                        break;
                    }

                    SDL_Rect clip = {xPos, yPos, texWid,texHeight};
                    SDL_Rect renderRect = {0, 0, texWid, texHeight};
                    SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),fPairTex, &renderRect, &clip);
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                }
            }
        }
    }

    void font_sdl_tff::render_text_boxed( int xPos, int yPos, std::string textureText, color * textColor,color * boxColor,int hAlign,int vAlign, int renderAlpha )
    {
        if( renderAlpha > 0)
        {
            font_pair_sdl * strTex = find_texture_sdl(textureText);
            SDL_Texture * fPairTex=  NULL;
            if( strTex!=NULL)
            {
                fPairTex = strTex->get_texture();
                if( renderAlpha > 255)
                {
                    renderAlpha = 255;
                }
                if( fPairTex!=NULL && renderAlpha >0)
                {
                    int texWid = 0;
                    int texHeight =0;
                    SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                    if( strTex->lastAlphaRendered!=renderAlpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                        strTex->lastAlphaRendered = renderAlpha;
                    }
                    //Get image dimensions
                    texWid = strTex->get_width();
                    texHeight = strTex->get_height();

                    if(hAlign<gpe::fa_left || hAlign > gpe::fa_right )
                    {
                        hAlign=last_used_halign;
                    }

                    if(vAlign < gpe::fa_top || vAlign > gpe::fa_bottom )
                    {
                        vAlign=last_used_valign;
                    }
                    switch(hAlign)
                    {
                    case gpe::fa_center:
                        xPos=xPos-texWid/2;
                        //last_used_halign=gpe::fa_center;
                        break;

                    case gpe::fa_right:
                        xPos=xPos-texWid;
                        // last_used_halign=gpe::fa_right;
                        break;

                    //rendering left will be the default
                    default:
                        // last_used_halign=gpe::fa_left;
                        break;
                    }
                    switch(vAlign)
                    {
                    case gpe::fa_middle:
                        yPos=yPos-texHeight/2;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    case gpe::fa_bottom:
                        yPos=yPos-texHeight;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    //rendering left will be the default
                    default:
                        //last_used_valign=gpe::fa_top;
                        break;
                    }
                    gpe::shape_rect gpeClip;
                    gpeClip.x = xPos;
                    gpeClip.y = yPos;
                    gpeClip.w = texWid;
                    gpeClip.h = texHeight;
                    SDL_Rect sdlClip = {xPos, yPos, texWid,texHeight};
                    gpe::gcanvas->render_rect(  &gpeClip,boxColor,false,renderAlpha );
                    SDL_RenderCopy( renderer_main_sdl->get_sdl_renderer(),fPairTex, NULL, &sdlClip);
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                }
            }
        }
    }

    void font_sdl_tff::render_text_rotated( int xPos, int yPos, std::string textureText, color * textColor, float textAngle, int renderAlpha )
    {
        if( renderAlpha > 0)
        {
            font_pair_sdl * strTex = find_texture_sdl(textureText);
            SDL_Texture * fPairTex=  NULL;
            if( strTex!=NULL)
            {
                fPairTex = strTex->get_texture();
                if( renderAlpha > 255)
                {
                    renderAlpha = 255;
                }
                if( fPairTex!=NULL && renderAlpha >0)
                {
                    int texWid = 0;
                    int texHeight =0;
                    SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );

                    if( strTex->lastAlphaRendered!=renderAlpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                        strTex->lastAlphaRendered = renderAlpha;
                    }
                    //Get image dimensions
                    texWid = strTex->get_width();
                    texHeight = strTex->get_height();

                    SDL_Rect sdlDstrect = {xPos-texWid, yPos-texHeight, texWid,texHeight};
                    SDL_Point sdlCenterPoint = {texWid, texHeight};
                    SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),fPairTex, NULL,&sdlDstrect,textAngle,&sdlCenterPoint, SDL_FLIP_NONE );
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                }
            }
        }
    }

    bool font_sdl_tff::render_text_special( int xPos, int yPos, std::string textureText, color * textColor, int hAlign,int vAlign, float renderAngle , float renderScale, int renderAlpha )
    {
        if( renderAlpha > 0)
        {
            font_pair_sdl * strTex = find_texture_sdl(textureText);
            if( strTex!=NULL)
            {
                SDL_Texture * fPairTex = strTex->get_texture();
                if( fPairTex!=NULL)
                {
                    int texWid = 0;
                    int texHeight =0;
                    SDL_SetTextureColorMod( fPairTex, textColor->get_r(), textColor->get_g(), textColor->get_b() );
                    if( strTex->lastAlphaRendered!=renderAlpha )
                    {
                        SDL_SetTextureAlphaMod(fPairTex,renderAlpha);
                        strTex->lastAlphaRendered = renderAlpha;
                    }
                    //Get image dimensions
                    texWid = (float)strTex->get_width() * renderScale;
                    texHeight = (float)strTex->get_height() * renderScale;

                    //Get image dimensions
                    if( texWid < 0)
                    {
                        texWid *=-1;
                        renderAngle *=-1;
                    }

                    if( texHeight<0 )
                    {
                        texHeight *=-1;
                        renderAngle *=-1;
                    }

                    if(hAlign<gpe::fa_left || hAlign > gpe::fa_right )
                    {
                        hAlign=last_used_halign;
                    }

                    if(vAlign < gpe::fa_top || vAlign > gpe::fa_bottom )
                    {
                        vAlign=last_used_valign;
                    }
                    switch(hAlign)
                    {
                    case gpe::fa_center:
                        xPos=xPos-texWid/2;
                        //last_used_halign=gpe::fa_center;
                        break;

                    case gpe::fa_right:
                        xPos=xPos-texWid;
                        // last_used_halign=gpe::fa_right;
                        break;

                    //rendering left will be the default
                    default:
                        // last_used_halign=gpe::fa_left;
                        break;
                    }
                    switch(vAlign)
                    {
                    case gpe::fa_middle:
                        yPos=yPos-texHeight/2;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    case gpe::fa_bottom:
                        yPos=yPos-texHeight;
                        // last_used_valign=gpe::fa_middle;
                        break;

                    //rendering left will be the default
                    default:
                        //last_used_valign=gpe::fa_top;
                        break;
                    }

                    SDL_Rect renderRect = {xPos, yPos, texWid,texHeight};
                    SDL_RenderCopyEx( renderer_main_sdl->get_sdl_renderer(),fPairTex, NULL,  &renderRect, -renderAngle,  NULL, SDL_FLIP_NONE );
                    SDL_SetTextureColorMod( fPairTex, c_white->get_r(), c_white->get_g(), c_white->get_b() );
                    return true;
                }
            }
        }
        return false;
    }

    bool init_sdl_font_system()
    {
        //If we already started the font system do nothing.
        if( gfs!=NULL )
        {
            delete gfs;
            gfs = NULL;
        }
        //Initialize SDL_ttf
        gpe::error_log->report("Starting SDL_TFF font system...");
        if( TTF_Init() == -1 )
        {
            gpe::error_log->report("        Error loading SDL_TTF");
            return false;
        }

        gfs = new font_system_controller();
        gfs->font_template = new font_sdl_tff("",0,false,"",-1 );
        gpe::error_log->report("SDL_TFF font system started...");
        return true;
    }

    void quit_sdl_font_system()
    {
        if( gfs!=NULL )
        {
            delete gfs;
            gfs = NULL;
        }
        gpe::error_log->report("Quitting SDL_TFF....");
        gpe::error_log->report("font_System Quit...");
        TTF_Quit();
    }
}
