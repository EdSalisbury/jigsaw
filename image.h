/**********************************************************
 *                         Jigsaw                         *
 *           A jigsaw puzzle creator/solver/game          *
 *          by Ed Salisbury (ed@edsalisbury.net),         *
 *    Matt Goralczyk (matthew.goralczyk@ucdenver.edu),    *
 *      and James Brayton (james.brayton@gmail.com)       *
 *              (c)2012, Some Rights Reserved             *
 **********************************************************/

/*
 * License:
 * Except where otherwise noted, this work is licensed under Creative Commons
 *  Attribution ShareAlike 3.0.
 *
 * You are free:
 *  ~ to Share -- to copy, distribute and transmit the work
 *  ~ to Remix -- to adapt the work
 *
 * Under the following conditions:
 *  ~ Attribution. You must attribute the work in the manner specified by the
 *    author or licensor (but not in any way that suggests that they endorse
 *    you or your use of the work).
 *  ~ Share Alike. If you alter, transform, or build upon this work, you may
 *    distribute the resulting work only under the same, similar or a
 *    compatible license.
 *  ~ For any reuse or distribution, you must make clear to others the license
 *    terms of this work. The best way to do this is with a link to the
 *    license's web page (http://creativecommons.org/licenses/by-sa/3.0/)
 *  ~ Any of the above conditions can be waived if you get permission from the
 *    copyright holder.
 *  ~ Nothing in this license impairs or restricts the author's moral rights.
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <IL/il.h>
#include <string>
#include <vector>
#include "color.h"

class Image
{
    public:
        Image();
        Image(UINT width, UINT height);
        ~Image();
        bool Load(std::string filename);
        bool Save(std::string filename);
        UINT Width() const;
        UINT Height() const;
        void Width(const UINT width);
        void Height(const UINT height);
        void ColorReplace(Color& color1, Color& color2);
        void Init();
        void Destroy();
        void Fill(Color color);
        void CopyRect(Image& src, UINT src_x, UINT dest_x, UINT width, UINT y, UINT dest_y, UINT height);
        void Plot(UINT x, UINT y, Color color);
        void DrawLine(UINT startx, UINT starty, UINT endx, UINT endy, Color color);
        Color GetColor(UINT x, UINT y);
        void Opacify();

    private:
        UINT mWidth;
        UINT mHeight;
        UINT mDepth;
        UINT mChannels;
        UINT mColorType;
        std::vector<std::vector<Color> > mData;
};

#endif
