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
#include "puzzle.h"

int main(int argc, char** argv)
{
    // Verify arguments
    if (argc < 4)
    {
        std::cout << "Usage: match_pieces <directory> <piece1> <piece2>" << std::endl;
        return 1;
    }

    Puzzle puzzle;
    std::string dir = argv[1];
    UINT piece1 = atoi(argv[2]);
    UINT piece2 = atoi(argv[3]);

    // Load in rotation file for pieces
    std::string rot1;
    std::string rot2;
    rot1 = puzzle.LoadRotationFile(dir, piece1);
    rot2 = puzzle.LoadRotationFile(dir, piece2);

    // Error checking
    if (rot1.size() == 0)
    {
        std::cout << "Error: piece " << piece1 << " is zero-length or not found" << std::endl;
        return 0;
    }
    if (rot2.size() == 0)
    {
        std::cout << "Error: piece " << piece2 << " is zero-length or not found" << std::endl;
        return 0;
    }

    // Reverse the second rotation file
    rot2 = puzzle.ReverseRotation(rot2);

    // Set up terminators
    std::vector<char> terminators;
    terminators.push_back('A');
    terminators.push_back('B');
    rot1.append("A");
    rot2.append("B");

    // Set up suffix list and children
    std::set<std::string> suffix_list;
    std::vector<std::vector<UINT> > children;
    std::vector<std::string> suffixes;
    children.resize(1);
    suffixes.push_back("");

    // Add suffixes to the suffix list
    UINT count = 0;
    while (count < rot1.size())
    {
        suffix_list.insert(rot1.substr(count++));
    }
    count = 0;
    while (count < rot2.size())
    {
        suffix_list.insert(rot2.substr(count++));
    }

    // Insert suffixes into suffix tree 
    std::set<std::string>::iterator it;
    for (it = suffix_list.begin() ; it != suffix_list.end() ; ++it)
    {
        puzzle.SuffixTreeInsert(suffixes, children, *it, 0);
    }

    // Get longest string
    std::string longest = puzzle.LongestString(children, suffixes, terminators);

    // Get offsets for each rotation file
    int offset1 = -1;
    int offset2 = -1;
    size_t found;
    found = rot1.rfind(longest);
    if (found != std::string::npos) 
    {
        offset1 = int(found);
    }
    found = rot2.rfind(longest);
    if (found != std::string::npos) 
    {
        offset2 = int(found);
    }

    // Determine match percentage for smallest piece
    UINT length = longest.size();
    int perc = 0;
    if (rot1.size() >= rot2.size())
        perc = (length / (rot2.size() + 0.0)) * 100;
    else
        perc = (length / (rot1.size() + 0.0)) * 100;

    // Display match details
    if (offset1 != -1 && offset2 != -1 && length > 0)
        std::cout << perc << " " << offset1 << " " << offset2 << " " << length << std::endl;
    else
        std::cout << "0" << std::endl;

    return 0;
}
