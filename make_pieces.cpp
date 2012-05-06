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

#include <iostream>
#include "image.h"
#include "color.h"
#include "puzzle.h"

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cout << "Usage: make_pieces <filename> <num_pieces> <variance>" << std::endl;
        return(1);
    }
    
    // Get directory name
    std::stringstream filename;
    filename << argv[1];
    int idx = filename.str().rfind('.');
    std::string dir = filename.str().substr(0, idx);

    // Load the image
    Image img;
    if (!img.Load(argv[1]))
    {
        std::cout << "Error: Unable to load " << argv[1] << std::endl;
    }

    // Make all pixels opaque (remove transparency)
    img.Opacify();

    Color black = Color(0, 0, 0, 255);
    Color gray = Color(1, 1, 1, 255);
    Color blue = Color(0, 0, 255, 255);
    Color lt_blue = Color(0, 0, 254, 255);

    // Replace colors in case they're used
    img.ColorReplace(black, gray);
    img.ColorReplace(blue, lt_blue);

    // Put lines into image
    Puzzle puzzle;
    puzzle.MakePieces(img, atoi(argv[2]), atoi(argv[3]), black, blue);
    
    // Make data files
    puzzle.MakeFiles(dir, img, black, blue, 1);

    // Save the image
    std::stringstream basename;
    basename << dir << "/0.png";
    img.Save(basename.str());

    return 0;
}
