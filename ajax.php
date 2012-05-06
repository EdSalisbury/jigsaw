<?php
//**********************************************************
//*                         Jigsaw                         *
//*           A jigsaw puzzle creator/solver/game          *
//*          by Ed Salisbury (ed@edsalisbury.net),         *
//*    Matt Goralczyk (matthew.goralczyk@ucdenver.edu),    *
//*      and James Brayton (james.brayton@gmail.com)       *
//*              (c)2012, Some Rights Reserved             *
//**********************************************************/
//
// License:
// Except where otherwise noted, this work is licensed under Creative Commons
//  Attribution ShareAlike 3.0.
//
// You are free:
//  ~ to Share -- to copy, distribute and transmit the work
//  ~ to Remix -- to adapt the work
//
// Under the following conditions:
//  ~ Attribution. You must attribute the work in the manner specified by the
//    author or licensor (but not in any way that suggests that they endorse
//    you or your use of the work).
//  ~ Share Alike. If you alter, transform, or build upon this work, you may
//    distribute the resulting work only under the same, similar or a
//    compatible license.
//  ~ For any reuse or distribution, you must make clear to others the license
//    terms of this work. The best way to do this is with a link to the
//    license's web page (http://creativecommons.org/licenses/by-sa/3.0/)
//  ~ Any of the above conditions can be waived if you get permission from the
//    copyright holder.
//  ~ Nothing in this license impairs or restricts the author's moral rights.

$cmd = $_GET['cmd'];
$arg1 = $_GET['arg1'];
$arg2 = $_GET['arg2'];
$arg3 = $_GET['arg3'];
$arg4 = $_GET['arg4'];
$arg5 = $_GET['arg5'];
$arg6 = $_GET['arg6'];

if ($arg1 == 'undefined')   $arg1 = '';
if ($arg2 == 'undefined')   $arg2 = '';
if ($arg3 == 'undefined')   $arg3 = '';
if ($arg4 == 'undefined')   $arg4 = '';
if ($arg5 == 'undefined')   $arg5 = '';
if ($arg6 == 'undefined')   $arg6 = '';

// Sanitize arguments
$arg1 = sanitize_filename($arg1);
$arg2 = intval($arg2);
$arg3 = intval($arg3);
$arg4 = intval($arg4);
$arg5 = intval($arg5);
$arg6 = intval($arg6);

if ($cmd == "match_pieces" || $cmd == "join_pieces")
{
    system("./$cmd $arg1 $arg2 $arg3 $arg4 $arg5 $arg6");
}
else if ($cmd == "rm_piece")
{
    unlink("$arg1/$arg2.png");
    unlink("$arg1/$arg2.rot");
    unlink("$arg1/$arg2.xy");
}

function sanitize_filename($filename)
{
    $special_chars = array("?", "[", "]", "/", "\\", "=", "<", ">", ":", ";", ",", "'", "\"", "&", "$", "#", "*", "(", ")", "|", "~", "`", "!", "{", "}", chr(0));

    $filename = str_replace($special_chars, '', $filename);
    $filename = preg_replace('/[\s-]+/', '-', $filename);
    $filename = trim($filename, '.-_');
    return ($filename);
}
