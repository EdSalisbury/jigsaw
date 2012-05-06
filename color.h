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

#ifndef COLOR_H
#define COLOR_H

#include <iostream>

typedef unsigned int UINT;

class Color
{
    public:
        Color();
        Color(UINT red, UINT green, UINT blue, UINT alpha);
        UINT Red() const;
        UINT Green() const;
        UINT Blue() const;
        UINT Alpha() const;
        void Red(UINT color);
        void Green(UINT color);
        void Blue(UINT color);
        void Alpha(UINT alpha);
        bool operator== (const Color& other) const; 
        bool operator!= (const Color& other) const; 
        Color& operator= (const Color& other); 
        friend std::ostream& operator << (std::ostream& out, const Color& color);

    private:
        UINT mRed;
        UINT mGreen;
        UINT mBlue;
        UINT mAlpha;
};

#endif
