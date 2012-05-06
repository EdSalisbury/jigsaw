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

#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <set>
#include <utility>
#include <algorithm>
#include <dirent.h>
#include "color.h"
#include "image.h"

typedef unsigned int UINT;
typedef std::pair<UINT, UINT> point;
const double PI = 3.14159;
const UINT MAX_INT = 2147483647;

class Puzzle
{
    public:
        void MakePieces(Image& img, UINT num_pieces, UINT variance, Color& side_color, Color& edge_color);
        void MakeFiles(std::string dir, Image& img, const Color& side_color, const Color& edge_color, UINT piece_num);
        int JoinPieces (std::string dir, int piece1_num, int piece2_num, int offset1, int offset2, int length, Color& side_color, Color& edge_color);
        void SaveDataFiles(std::string dir, int piece_num, UINT width, UINT height, int x_offset, int y_offset, std::set<point>& sides, std::set<point>& edges);
        std::string LoadRotationFile(std::string dir, UINT piece_num);
        std::string ReverseRotation(std::string data);
        void SuffixTreeInsert(std::vector<std::string>& suffixes, std::vector<std::vector<unsigned int> >& children, std::string suffix, unsigned int start);
        std::string LongestString(std::vector<std::vector<UINT> >& children, std::vector<std::string>& suffixes, const std::vector<char>& terminator_list);
};

void match(std::vector<std::vector<UINT> >& children, std::vector<std::string>& suffixes, std::vector<UINT>& parent_idx, std::vector<std::string>& common, std::string& longest, const std::vector<char>& terminator_list);

#endif
