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

#include "puzzle.h"

void Puzzle::MakePieces(Image& img, UINT num_pieces, UINT variance, Color& side_color, Color& edge_color)
{
    // Seed the random number generator
    srand(time(0));

    // Set opposite length to half the variance
    int opp = variance / 2;

    // Number of pieces needs to be a perfect square
    double d_num = sqrt(num_pieces);
    int i_num = d_num;
    if (d_num != i_num)
    {
        std::cout << "Error: number of pieces must be a perfect square!" << std::endl;
        return;
    }
    
    // Add edge lines
    img.DrawLine(0, 0, img.Width() - 1, 0, edge_color);
    img.DrawLine(0, img.Height() - 1, img.Width() - 1, img.Height() - 1, edge_color);
    img.DrawLine(0, 0, 0, img.Height() - 1, edge_color);
    img.DrawLine(img.Width() - 1, 0, img.Width() - 1, img.Height() - 1, edge_color);

    // Get number of horizontal and vertical pieces
    int num_hor_pieces = sqrt(num_pieces);
    int num_vert_pieces = sqrt(num_pieces);

    // Figure out how many lines to draw
    int piece_width = img.Width() / num_hor_pieces;
    int piece_height = img.Height() / num_vert_pieces;
    int num_hor_lines = num_hor_pieces - 1;
    int num_vert_lines = num_vert_pieces - 1;

    // Set up arrays of horizontal and vertical lines
    int vert_lines[num_vert_lines];
    int hor_lines[num_hor_lines];
    for (int i = 0 ; i <= num_vert_lines ; ++i)
        vert_lines[i] = piece_width * (i + 1);
    for (int i = 0 ; i <= num_hor_lines ; ++i)
        hor_lines[i] = piece_height * (i + 1);

    UINT x = 0;
    UINT y = 0;
    int count = 0;
    
    // Draw vertical lines
    while (count < num_vert_lines)
    {
        x = vert_lines[count];
        y = 0;

        // Draw line
        while (y < img.Height())
        {
            // Pick a random angle, and determine adjacent length
            int angle = (rand() % 15) + 10;
            double rad = angle * PI / 180;
            int adj = round((opp * 2) / tan(rad));
            adj = abs(adj);

            if (adj)
            {
                // Determine end points
                UINT endx = x + opp;
                UINT endy = y + adj;

                if (endy > img.Height())
                    endy = img.Height();

                if (endx > img.Width())
                    endx = img.Width();

                // Draw the line
                img.DrawLine(x - opp, y, x + opp, endy, side_color);
                y += adj;
                opp = -opp;
            }
        }
        count++;
    }

    count = 0;

    // Draw horizontal lines
    while (count < num_hor_lines)
    {
        x = 0;
        y = hor_lines[count];

        while (x < img.Width())
        {
            // Pick a random angle, and determine adjacent length
            int angle = (rand() % 15) + 10;
            double rad = angle * PI / 180;
            int adj = round((opp * 2) / tan(rad));

            adj = abs(adj);

            if (adj)
            {
                // Determine end points
                UINT endx = x + adj;
                UINT endy = y + opp;

                if (endy > img.Height())
                    endy = img.Height();

                if (endx > img.Width())
                    endx = img.Width();

                // Draw the line
                img.DrawLine(x, y - opp, endx, endy, side_color);
                x += adj;
                opp = -opp;
            }
        }
        count++;
    }

}

void Puzzle::MakeFiles(std::string dir, Image& img, const Color& side_color, const Color& edge_color, UINT piece_num)
{
    // Set up sets
    std::set<point> adj;
    std::set<point> sides;
    std::set<point> edges;
    std::set<point> body;

    // Set up visited matrix
    bool visited[img.Width()][img.Height()];
    for (UINT y = 0 ; y < img.Height() ; ++y)
    {
        for (UINT x = 0 ; x < img.Width() ; ++x)
        {
            visited[x][y] = false;
        }
    }

    // Start in upper left corner
    UINT x = 0;
    UINT y = 0;

    // Set max and mins
    UINT min_x = MAX_INT;
    UINT min_y = MAX_INT;
    UINT max_x = 0;
    UINT max_y = 0;

    bool done = false;

    while (!done)
    {
        if (!visited[x][y] && img.GetColor(x, y) != side_color && img.GetColor(x, y) != edge_color)
        {
            // Update visited
            visited[x][y] = true;

            // Add to body list
            body.insert(std::make_pair(x, y));

            // Update max and mins for piece image sizes
            if (x < min_x)
                min_x = x;
            if (y < min_y)
                min_y = y;
            if (x > max_x)
                max_x = x;
            if (y > max_y)
                max_y = y;

            // Check Up
            if (y > 0 && !visited[x][y - 1])
            {
                point up = std::make_pair(x, y - 1);
                if (img.GetColor(x, y - 1) == side_color)
                    sides.insert(up);
                else if (img.GetColor(x, y - 1) == edge_color)
                    edges.insert(up);
                else if (img.GetColor(x, y - 1).Alpha() == 255)
                    adj.insert(up);
            }

            // Check Down
            if (y < img.Height() - 1 && !visited[x][y + 1])
            {
                point down = std::make_pair(x, y + 1);
                if (img.GetColor(x, y + 1) == side_color)
                    sides.insert(down);
                else if (img.GetColor(x, y + 1) == edge_color)
                    edges.insert(down);
                else if (img.GetColor(x, y + 1).Alpha() == 255)
                    adj.insert(down);
            }

            // Check Left
            if (x > 0 && !visited[x - 1][y])
            {
                point left = std::make_pair(x - 1, y);
                if (img.GetColor(x - 1, y) == side_color)
                    sides.insert(left);
                else if (img.GetColor(x - 1, y) == edge_color)
                    edges.insert(left);
                else if (img.GetColor(x - 1, y).Alpha() == 255)
                    adj.insert(left);
            }

            // Check Right
            if (x < img.Width() - 1 && !visited[x + 1][y])
            {
                point right = std::make_pair(x + 1, y);
                if (img.GetColor(x + 1, y) == side_color)
                    sides.insert(right);
                else if (img.GetColor(x + 1, y) == edge_color)
                    edges.insert(right);
                else if (img.GetColor(x + 1, y).Alpha() == 255)
                    adj.insert(right);
            }

            // Adjacent pixels left to check
            if (adj.size())
            {
                x = adj.begin()->first;
                y = adj.begin()->second;
                adj.erase(adj.begin());
            }
            else
            {
                // Piece is complete (and is big enough)
                if (body.size() > 2)
                {
                    --min_x;
                    --min_y;
                    max_x += 2;
                    max_y += 2;
                    UINT width = (max_x - min_x);
                    UINT height = (max_y - min_y);

                    // Create new piece image
                    std::stringstream imgfilename;
                    imgfilename << dir << "/" << piece_num << ".png";
                    Image piece = Image(width, height);

                    // Create iterators
                    std::set<point>::iterator it;
                    
                    // Add body pixels
                    for (it = body.begin() ; it != body.end() ; ++it)
                    {
                        Color newcolor = img.GetColor(it->first, it->second);
                        UINT newx = it->first - min_x;
                        UINT newy = it->second - min_y;
                        piece.Plot(newx, newy, newcolor);
                    }

                    // Add side pixels
                    for (it = sides.begin() ; it != sides.end() ; ++it)
                    {
                        Color newcolor = img.GetColor(it->first, it->second);
                        UINT newx = it->first - min_x;
                        UINT newy = it->second - min_y;
                        piece.Plot(newx, newy, newcolor);
                    }
                    
                    // Add edge pixels
                    for (it = edges.begin() ; it != edges.end() ; ++it)
                    {
                        Color newcolor = img.GetColor(it->first, it->second);
                        UINT newx = it->first - min_x;
                        UINT newy = it->second - min_y;
                        piece.Plot(newx, newy, newcolor);
                    }

                    // Save the image
                    piece.Save(imgfilename.str());

                    SaveDataFiles(dir, piece_num, img.Width(), img.Height(), min_x, min_y, sides, edges);

                    ++piece_num;
                }

                body.clear();
                sides.clear();
                edges.clear();
                min_x = MAX_INT;
                min_y = MAX_INT;
                max_x = 0;
                max_y = 0;
            }
        }
        else
        {
            // Look for the next blank pixel
            bool blank_found = false;
            x = 0;
            y = 0;
            while (!blank_found && !done)
            {
                if (img.GetColor(x, y) != side_color && img.GetColor(x, y) != edge_color && !visited[x][y] && img.GetColor(x,y).Alpha() == 255)
                    blank_found = true;
                else
                {
                    if (x < img.Width() - 1)
                        ++x;
                    else
                    {
                        if (y < img.Height() - 1)
                        {
                            ++y;
                            x = 0;
                        }
                        else
                            done = true;
                    }
                }
            }
        }
    }
}

void Puzzle::SaveDataFiles(std::string dir, int piece_num, UINT width, UINT height, int x_offset, int y_offset, std::set<point>& sides, std::set<point>& edges)
{
    // Create a matrix of the sides
    UINT side_matrix[width][height];
    for (UINT y = 0 ; y < height ; ++y)
    {
        for (UINT x = 0 ; x < width ; ++x)
        {
            side_matrix[x][y] = 0;
        }
    }

    // Populate with the sides and edges
    std::set<point>::iterator it;
    for (it = sides.begin() ; it != sides.end() ; ++it)
        side_matrix[it->first - x_offset][it->second - y_offset] = 1;
    for (it = edges.begin() ; it != edges.end() ; ++it)
        side_matrix[it->first - x_offset][it->second - y_offset] = 2;

    // Find the starting point
    UINT x = 0;
    UINT y = 0;
    while (!side_matrix[x][y])
    {
        if (x < width - 1)
        {
            ++x;
        }
        else
        {
            if (y < height - 1)
            {
                ++y;
                x = 0;
            }
        }
    }

    point first = std::make_pair(x, y);
    point last = first;
    point next;

    int direction = 0;
    UINT jump = 1;

    std::vector<point> line;
    std::vector<UINT> angles;
    bool done = false;
    int last_angle = -1;
    UINT angle = -1;
    while (!done)
    {
        if (x < width - jump && side_matrix[x + jump][y] && last_angle != 4)
        {
            // Right or 0 degrees
            next = std::make_pair(x + jump, y);
            angle = 0;
        }
        else if (x < width - jump && y >= jump && side_matrix[x + jump][y - jump] && last_angle != 5)
        {
            // Upper-right or 45 degrees
            next = std::make_pair(x + jump, y - jump);
            angle = 1;
        }
        else if (y >= jump && side_matrix[x][y - jump] && last_angle != 6)
        {
            // Up or 90 degrees
            next = std::make_pair(x, y - jump);
            angle = 2;
        }
        else if (x >= jump && y >= jump && side_matrix[x - jump][y - jump] && last_angle != 7)
        {
            // Upper-left or 135 degrees
            next = std::make_pair(x - jump, y - jump);
            angle = 3;
        }
        else if (x >= jump && side_matrix[x - jump][y] && last_angle != 0)
        {
            // Left or 180 degrees
            next = std::make_pair(x - jump, y);
            angle = 4;
        }
        else if (x >= jump && y < height - jump && side_matrix[x - jump][y + jump] && last_angle != 1)
        {
            // Lower-left or 225 degrees
            next = std::make_pair(x - jump, y + jump);
            angle = 5;
        }
        else if (y < height - jump && side_matrix[x][y + jump] && last_angle != 2)
        {
            // Down or 270 degrees
            next = std::make_pair(x, y + jump);
            angle = 6;
        }
        else if (x < width - jump && y < height - jump && side_matrix[x + jump][y + jump] && last_angle != 3)
        {
            // Lower-right or 315 degrees
            next = std::make_pair(x + jump, y + jump);
            angle = 7;
        }

        last = std::make_pair(x, y);

        if (next == first)
        {
            // We've looped around to the beginning, time to die
            done = true;
        }
        else if (next == last)
        {
            // Couldn't find anything, so next didn't change - increase the jump to hopefully find it on the next run
            ++jump;
        }
        else
        {
            // Take determinant and keep track
            direction += (x * next.second) - (y * next.first);
            if (jump > 1)
                jump = 1;

            // Get next position
            x = next.first;
            y = next.second;
            
            // Check if it's a side, if so, record it
            if (side_matrix[x][y] == 1)
            {
                line.push_back(next);
                if (line.size() > 1)
                    angles.push_back(angle);
            }
            last_angle = angle;
        }

    }

    std::stringstream rot;
    std::vector<UINT>::iterator uit;
    for (uit = angles.begin() ; uit < angles.end() ; ++uit)
        rot << *uit;
    std::string rotations = rot.str();

    if (direction < 1)
    {
        // If direction is negative, it's counter-clockwise, reverse to make it clockwise
        reverse(line.begin(), line.end());
        reverse(angles.begin(), angles.end());
        rotations = ReverseRotation(rotations);
    }

    // Get filenames
    std::stringstream xyfilename;
    xyfilename << dir << "/" << piece_num << ".xy";
    std::stringstream rotfilename;
    rotfilename << dir << "/" << piece_num << ".rot";

    // Write XY file
    std::ofstream xyfile;
    xyfile.open(xyfilename.str().c_str(), std::ios::out | std::ios::trunc);
    std::vector<point>::iterator pit;
    for (pit = line.begin() ; pit < line.end() ; ++pit)
    {
        xyfile << pit->first << " " << pit->second << std::endl;
    }
    xyfile.close();

    // Write rot file
    std::ofstream rotfile;
    rotfile.open(rotfilename.str().c_str(), std::ios::out | std::ios::trunc);
    rotfile << rotations;
    rotfile << std::endl;
    rotfile.close();
}

int Puzzle::JoinPieces (std::string dir, int piece1_num, int piece2_num, int offset1, int offset2, int length, Color& side_color, Color& edge_color)
{
    std::vector<point> piece1;
    std::vector<point> piece2;

    std::fstream infile;
    std::string x_str;
    std::string y_str;
    UINT x;
    UINT y;
    UINT x_max = 0;
    UINT y_max = 0;

    Color blank = Color(0, 0, 0, 0);

    std::stringstream filename;
    filename << dir << "/" << piece1_num << ".xy";
    infile.open(filename.str().c_str(), std::ifstream::in);
    while (infile.good())
    {
        x_str = "";
        y_str = "";
        infile >> x_str;
        infile >> y_str;

        if (x_str != "" && y_str != "")
        {
            x = atoi(x_str.c_str());
            y = atoi(y_str.c_str());
            if (x > x_max)
                x_max = x;
            if (y > y_max)
                y_max = y;

            piece1.push_back(std::make_pair(x, y));
        }
    }
    infile.close();
    filename.str("");
    filename << dir << "/" << piece2_num << ".xy";
    infile.open(filename.str().c_str(), std::ifstream::in);
    while (infile.good())
    {
        x_str = "";
        y_str = "";
        infile >> x_str;
        infile >> y_str;
        if (x_str != "" && y_str != "")
        {
            x = atoi(x_str.c_str());
            y = atoi(y_str.c_str());
            if (x > x_max)
                x_max = x;
            if (y > y_max)
                y_max = y;

            piece2.push_back(std::make_pair(x, y));
        }
    }
    infile.close();

    // Reverse the second piece
    reverse(piece2.begin(), piece2.end());

    ++x_max;
    ++y_max;

    // Get x and y-offsets
    int x_offset = piece1[offset1].first - piece2[offset2].first; 
    int y_offset = piece1[offset1].second - piece2[offset2].second; 

    // Create grid that's big enough for both pieces
    std::vector<std::vector<Color> > grid;
    UINT new_width = x_max * 3;
    UINT new_height = y_max * 3;

    grid.resize(new_width);
    for (UINT xx = 0 ; xx < new_width ; ++xx)
    {
        grid[xx].resize(new_height);
    }

    // Copy first image into the center of the grid
    filename.str("");
    filename << dir << "/" << piece1_num << ".png";

    Image piece1_img;
    piece1_img.Load(filename.str());
    for (y = 0 ; y < piece1_img.Height() ; ++y)
    {
        for (x = 0 ; x < piece1_img.Width() ; ++x)
        {
            grid[x + x_max][y + y_max] = piece1_img.GetColor(x, y);
        }
    }
    
    int current = 0;

    // Verify that there's no overlap
    while (current < length)
    {
        x = piece2[offset2 + current].first;
        y = piece2[offset2 + current].second;

        if (grid[(x + x_max) + x_offset][(y + y_max) + y_offset] != side_color && 
            grid[(x + x_max) + x_offset][(y + y_max) + y_offset] != edge_color &&
            grid[(x + x_max) + x_offset][(y + y_max) + y_offset] != blank)
        {
            return(0);
        }
        ++current;
    }

    // Load in piece for second image
    filename.str("");
    filename << dir << "/" << piece2_num << ".png";

    Image piece2_img;
    piece2_img.Load(filename.str());

    Color gray = Color(1, 1, 1, 255);

    // Copy everything in second piece to new image
    for (y = 0 ; y < piece2_img.Height() ; ++y)
    {
        for (x = 0 ; x < piece2_img.Width() ; ++x)
        {
            if (piece2_img.GetColor(x, y).Alpha() == 255)
            {
                if (grid[x + x_offset + x_max][y + y_offset + y_max] == side_color)
                {
                    grid[x + x_offset + x_max][y + y_offset + y_max] = gray;
                }
                else
                {
                    grid[x + x_offset + x_max][y + y_offset + y_max] = piece2_img.GetColor(x, y);
                }
            }
        }
    }
    
    // Trim the edges of the grid and copy to the new image
    UINT min_x = MAX_INT;
    UINT min_y = MAX_INT;
    UINT max_x = 0;
    UINT max_y = 0;
    for (y = 0 ; y < new_height ; ++y)
    {
        for (x = 0 ; x < new_width ; ++x)
        {
            if (grid[x][y].Alpha() == 255)
            {
                if (x > max_x)
                    max_x = x;
                if (x < min_x)
                    min_x = x;
                if (y > max_y)
                    max_y = y;
                if (y < min_y)
                    min_y = y;
            }
        }
    }
    ++max_x;
    ++max_y;

    Image tmp = Image(max_x - min_x, max_y - min_y);

    for (y = 0 ; y < max_y - min_y ; ++y)
    {
        for (x = 0 ; x < max_x - min_x ; ++x)
        {
            tmp.Plot(x, y, grid[x + min_x][y + min_y]); 
        }
    }

    DIR *dp;
    struct dirent *dirp;
    std::vector<int> files;
    dp = opendir(dir.c_str());
    while ((dirp = readdir(dp)) != NULL)
    {
        std::string file = std::string(dirp->d_name);
        int idx = file.rfind('.');
        files.push_back(atoi(file.substr(0, idx).c_str()));
    }
    closedir(dp);

    sort(files.begin(), files.end());

    int piece_num = files.back() + 1;

    std::stringstream outfile;
    outfile << dir << "/" << piece_num << ".png";

    MakeFiles(dir, tmp, side_color, edge_color, piece_num);
    return piece_num;
}

std::string Puzzle::LoadRotationFile(std::string dir, UINT piece_num)
{
    std::stringstream filename;
    filename << dir << "/" << piece_num << ".rot";

    std::ifstream infile;
    infile.open(filename.str().c_str(), std::ifstream::in);

    std::string rot;
    infile >> rot;

    infile.close();

    return rot;
}

std::string Puzzle::ReverseRotation(std::string data)
{
    reverse(data.begin(), data.end());
    std::stringstream newstr;

    for (UINT ii = 0 ; ii < data.size() ; ++ii)
    {
        int num = data[ii] - '0';
        switch (num)
        {
            case 0:
                num = 4;
                break;
            case 1:
                num = 5;
                break;
            case 2:
                num = 6;
                break;
            case 3:
                num = 7;
                break;
            case 4:
                num = 0;
                break;
            case 5:
                num = 1;
                break;
            case 6:
                num = 2;
                break;
            case 7:
                num = 3;
                break;
        }

        newstr << num;
    }

    return newstr.str();
}

void Puzzle::SuffixTreeInsert(std::vector<std::string>& suffixes, std::vector<std::vector<unsigned int> >& children, std::string suffix, unsigned int start)
{
    UINT size = suffixes.size();
    for (UINT ii = 0 ; ii < children[start].size() ; ++ii)
    {
        // Look at the first character
        int child = children[start][ii];
        if (suffixes[child] == suffix.substr(0, suffixes[child].size()))
        {
            SuffixTreeInsert(suffixes, children, suffix.substr(suffixes[child].size()), child);
        }
        else
        {
            int pos = 0;
            while (suffix[pos] == suffixes[child][pos])
                ++pos;

            if (pos > 0)
            {
                // Add the chopped portion as a child
                suffixes.push_back(suffixes[child].substr(pos));
                children.resize(children.size() + 1);

                children[suffixes.size() - 1] = children[child];
                children[child].clear();

                children[child].push_back(suffixes.size() - 1);


                // Chop the original suffix
                suffixes[child] = suffixes[child].substr(0, pos);

                // Push on the new suffix, minus the chopped portion
                suffixes.push_back(suffix.substr(pos));
                children[child].push_back(suffixes.size() - 1);
                children.resize(children.size() + 1);
            }
        }
    }

    if (suffixes.size() == size)
    {
        // Add to the suffix list
        suffixes.push_back(suffix);
        children.resize(children.size() + 1);

        // Update the children list of the parent
        children[start].push_back(suffixes.size() - 1);
    }
}


std::string Puzzle::LongestString(std::vector<std::vector<UINT> >& children, std::vector<std::string>& suffixes, const std::vector<char>& terminator_list)
{
    std::string longest;
    std::vector<std::string> common;
    std::vector<UINT> parent_idx;
    parent_idx.push_back(0); // for root node
    match(children, suffixes, parent_idx, common, longest, terminator_list);

    return (longest);
}

void match(std::vector<std::vector<UINT> >& children, std::vector<std::string>& suffixes, std::vector<UINT>& parent_idx, std::vector<std::string>& common, std::string& longest, const std::vector<char>& terminator_list)
{
    // Check for any more children, if not, move back up
    if (children[parent_idx.back()].size() < 1)
    {
        return;
    }

    std::vector<UINT>& check_childs = children[parent_idx.back()];
    std::vector<UINT>::iterator check_itr = check_childs.begin();
    std::set<char> terminators;
    UINT num_terminators = 0;

    // Go through children to look for terminators
    while (check_itr != check_childs.end())
    {
        // Get last character
        std::string temp_str = suffixes[*check_itr];
        char last_char = temp_str[temp_str.size() - 1];

        // Look for terminators
        if (find(terminator_list.begin(), terminator_list.end(), last_char) != terminator_list.end())
        {
            ++num_terminators;
            terminators.insert(last_char);
        }

        ++check_itr;
    }

    // Un-terminated children exist
    if (check_childs.size() > num_terminators)
    {
        // has children so
        std::vector<UINT>& child = children[parent_idx.back()];
        std::vector<UINT>::iterator child_itr = child.begin();
        while (child_itr != child.end())
        {
            // push the node we're moving to on to the index stack
            std::string temp = suffixes[*child_itr];
            common.push_back(temp);
            parent_idx.push_back(*child_itr);

            // run match on child node
            match(children, suffixes, parent_idx, common, longest, terminator_list);

            // if we've returned, we're going to check the next node of parent, so
            // pop back off the last one checked
            common.pop_back();
            parent_idx.pop_back();

            // check next child
            ++child_itr;
        }
    }
    else if (terminators.size() == terminator_list.size())
    {
        // We found a common string to both, but will it be the longest?
        std::string test_str;
        for (UINT j = 0; j < common.size(); ++j)
            test_str.append(common[j]);

        if (test_str.size() > longest.size())
            longest = test_str;
    }
    return;
}
