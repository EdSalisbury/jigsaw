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

#include "image.h"
#include <iostream>
#include <cstdlib>

Image::Image()
{
    ilInit();
    mWidth = 0;
    mHeight = 0;
}

Image::Image(UINT width, UINT height)
{
    mWidth = width;
    mHeight = height;
    this->Init();
}

Image::~Image()
{
    this->Destroy();
}

UINT Image::Width() const
{
    return this->mWidth;
}

UINT Image::Height() const
{
    return this->mHeight;
}

bool Image::Load(std::string filename)
{
    // Prepare for image loading
    ILuint image_name;
    ilGenImages(1, &image_name);
    ilBindImage(image_name);

    // Load the image
    if (!ilLoadImage(filename.c_str()))
        return false;

    // Get image data
    mWidth = ilGetInteger(IL_IMAGE_WIDTH);
    mHeight = ilGetInteger(IL_IMAGE_HEIGHT);
    ILubyte* data = ilGetData();

    // Determine if this image has alpha components
    bool alpha = false;
    if (ilGetInteger(IL_IMAGE_FORMAT) == IL_RGBA)
    {
        alpha = true;
    }

    // Initialize the data vector
    this->Init();
    
    // Load the image data into the data vector
    for (UINT y = 0 ; y < mHeight ; ++y)
    {
       for (UINT x = 0 ; x < mWidth ; ++x)
       {
            UINT pos;
            if (alpha)
            {
                pos = (y * mWidth + x) * 4;
            }
            else
            {
                pos = (y * mWidth + x) * 3;
            }
            mData[x][y].Red(data[pos + 0]);
            mData[x][y].Green(data[pos + 1]);
            mData[x][y].Blue(data[pos + 2]);
            if (alpha)
                mData[x][y].Alpha(data[pos + 3]);
            else
                mData[x][y].Alpha(255);
       }
    }

    // Cleanup
    ilDeleteImages(1, &image_name);

    return true;
}

void Image::ColorReplace(Color& color1, Color& color2)
{
    for (UINT y = 0 ; y < mHeight ; ++y)
    {
       for (UINT x = 0 ; x < mWidth ; ++x)
       {
           if (mData[x][y] == color1)
           {
               mData[x][y] = color2;
           }
       }
    }
}

void Image::Opacify()
{
    for (UINT y = 0 ; y < mHeight ; ++y)
    {
       for (UINT x = 0 ; x < mWidth ; ++x)
       {
           if (mData[x][y].Alpha() != 255)
           {
               mData[x][y].Red(255);
               mData[x][y].Green(255);
               mData[x][y].Blue(255);
               mData[x][y].Alpha(255);
           }
       }
    }
}

void Image::Fill(Color color)
{
    for (UINT y = 0 ; y < mHeight ; ++y)
    {
       for (UINT x = 0 ; x < mWidth ; ++x)
       {
           mData[x][y] = color;
       }
    }
}

void Image::CopyRect(Image& src, UINT src_x, UINT dest_x, UINT width, UINT src_y, UINT dest_y, UINT height)
{
    UINT x_offset = dest_x - src_x;
    UINT y_offset = dest_y - src_y;

    for (UINT y = src_y ; y < height ; ++y)
    {
       for (UINT x = src_x ; x < width ; ++x)
       {
           mData[x + x_offset][y + y_offset] = src.mData[x][y];
       }
    }
}

void Image::Init()
{
    mData.resize(mWidth);
    for (UINT x = 0 ; x < mWidth ; ++x)
    {
        mData[x].resize(mHeight);
    }

    for (UINT y = 0 ; y < mHeight ; ++y)
    {
       for (UINT x = 0 ; x < mWidth ; ++x)
       {
           mData[x][y].Red(0);
           mData[x][y].Green(0);
           mData[x][y].Blue(0);
           mData[x][y].Alpha(0);
       }
    }
}

void Image::Destroy()
{
}

void Image::Plot(UINT x, UINT y, Color color)
{
    if (x >= mWidth || y >= mHeight)
    {
        return;
    }

    mData[x][y] = color;
}

void Image::DrawLine(UINT startx, UINT starty, UINT endx, UINT endy, Color color)
{
    // Implement Bresenham's Line Algorithm
    int dx = abs(endx - startx);
    int dy = abs(endy - starty);

    int sx;
    int sy;

    if (startx < endx)
        sx = 1;
    else
        sx = -1;

    if (starty < endy)
        sy = 1;
    else
        sy = -1;

    int err = dx - dy;

    bool done = false;

    while (!done)
    {
        Plot(startx, starty, color);
        if (startx == endx && starty == endy)
            done = true;
        else
        {
            int e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                startx += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                starty += sy;
            }
        }
    }
}

bool Image::Save(std::string filename)
{
    // Prepare for image saving
    ILuint image_name;
    ilGenImages(1, &image_name);
    ilBindImage(image_name);

    // Copy color data to the image
    ILubyte* data = new ILubyte[mWidth * mHeight * 4];
    for (UINT y = 0 ; y < mHeight ; ++y)
    { 
       for (UINT x = 0 ; x < mWidth ; ++x)
       {
            UINT pos = (y * mWidth + x) * 4;
            data[pos + 0] = (ILubyte)mData[x][mHeight - y - 1].Red();
            data[pos + 1] = (ILubyte)mData[x][mHeight - y - 1].Green();
            data[pos + 2] = (ILubyte)mData[x][mHeight - y - 1].Blue();
            data[pos + 3] = (ILubyte)mData[x][mHeight - y - 1].Alpha();
       }
    }

    ilTexImage(mWidth, mHeight, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, data);

    delete[] data;

    ilEnable(IL_FILE_OVERWRITE);
    ilSaveImage(filename.c_str());
    
    // Cleanup
    ilDeleteImages(1, &image_name);

    return true;
}

void Image::Width(const UINT width)
{
    mWidth = width;
}

void Image::Height(const UINT height)
{
    mHeight = height;
}

Color Image::GetColor(UINT x, UINT y)
{
    return (mData[x][y]);
}
