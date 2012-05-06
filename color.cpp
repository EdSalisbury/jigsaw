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

#include "color.h"

Color::Color()
{
    mRed = 0;
    mBlue = 0;
    mGreen = 0;
    mAlpha = 0;
}

Color::Color(UINT red, UINT green, UINT blue, UINT alpha)
{
    mRed = red;
    mGreen = green;
    mBlue = blue;
    mAlpha = alpha;
}

UINT Color::Red() const
{
    return mRed;
}

UINT Color::Green() const
{
    return mGreen;
}

UINT Color::Blue() const
{
    return mBlue;
}

UINT Color::Alpha() const
{
    return mAlpha;
}

void Color::Red(UINT color)
{
    mRed = color;
}

void Color::Green(UINT color)
{
    mGreen = color;
}

void Color::Blue(UINT color)
{
    mBlue = color;
}

void Color::Alpha(UINT alpha)
{
    mAlpha = alpha;
}

bool Color::operator==(const Color& other) const
{
    if (mRed == other.Red() && mGreen == other.Green() && mBlue == other.Blue() && mAlpha == other.Alpha())
    {
        return true;
    }
    return false;
}

bool Color::operator!=(const Color& other) const
{
    return !(*this == other);
}

Color& Color::operator=(const Color& other)
{
    if (this == &other)
        return *this;

    mRed = other.Red();
    mGreen = other.Green();
    mBlue = other.Blue();
    mAlpha = other.Alpha();

    return *this;
}

std::ostream& operator << (std::ostream& out, const Color& color)
{
    out << color.Red() << ", ";
    out << color.Green() << ", ";
    out << color.Blue() << ", ";
    out << color.Alpha();

    return out;
}
