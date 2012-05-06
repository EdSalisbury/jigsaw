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
?>

<title>Jigsaw Puzzle</title>

<style>
.dragme {position:absolute;}
h1 {font-size: 120%}
h2 {font-size: 90%}
body {font-size: 80%}
h2 {font-size: 90%}
body {font-size: 80%}

#status
{
    background-color: #000;
    color: #f00;
}

#directory, #Response
{
    visibility: hidden;
}
</style>

<script>
var isdrag = false;
var dobj;
var zTrack = 1;
var tx, ty, x, y;
var currX, currY;

dimArr = new Array();

function getXMLHttp()
{
    var xmlHttp

    try
    {
        //Firefox, Opera 8.0+, Safari
        xmlHttp = new XMLHttpRequest();
    }
    catch(e)
    {
        //Internet Explorer
        try
        {
            xmlHttp = new ActiveXObject("Msxml2.XMLHTTP");
        }
        catch(e)
        {
            try
            {
                xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
            }
            catch(e)
            {
                alert("Your browser does not support AJAX!")
                return false;
            }
        }
    }

    return xmlHttp;
}

function saveCorners()
{
	tempDims = new Object();

	// Upper-left
	tempDims.leftUpX = parseInt(dobj.style.left);
	tempDims.leftUpY = parseInt(dobj.style.top);

	// Upper-right
	tempDims.rightUpX = parseInt(tempDims.leftUpX + dobj.width);
	tempDims.rightUpY = parseInt(tempDims.leftUpY);

	// Lower-left
	tempDims.leftDownX = parseInt(tempDims.leftUpX);
	tempDims.leftDownY = parseInt(tempDims.leftUpY + dobj.height);

	// Lower-right
	tempDims.rightDownX = parseInt(tempDims.leftUpX + dobj.width);
	tempDims.rightDownY = parseInt(tempDims.leftUpY + dobj.height);

	tempDims.description = dobj.id;
	tempDims.obj = dobj;

	dimArr[dobj.id] = tempDims;
}

function movemouse(e)
{
	if(isdrag)
    {
        dobj.style.left = tx + e.clientX - x + "px";
        dobj.style.top = ty + e.clientY - y + "px";
		dobj.style.zIndex = zTrack;
        currX = tx + e.clientX - x;
        currY = ty + e.clientY - y;
    }
}

function collision()
{
    // Upper-left
    var leftUpX = parseInt(dobj.style.left);
    var leftUpY = parseInt(dobj.style.top);

    // Upper-right
    var rightUpX = parseInt(leftUpX + dobj.width);
    var rightUpY = parseInt(leftUpY);

    // Lower-left
    var leftDownX = parseInt(leftUpX);
    var leftDownY = parseInt(leftUpY + dobj.height);

    // Lower-right
    var rightDownX = parseInt(leftUpX + dobj.width);
    var rightDownY = parseInt(leftUpY + dobj.height);

    var description = dobj.id;

    for(var index in dimArr)
    {
        if(dimArr[index].description != description)
        {
            var leftUpXComp  	= Math.abs(dimArr[index].leftUpX - leftUpX);
            var leftUpYComp  	= Math.abs(dimArr[index].leftUpY - leftUpY);
            var rightUpXComp 	= Math.abs(dimArr[index].rightUpX - rightUpX);
            var rightUpYComp 	= Math.abs(dimArr[index].rightUpY - rightUpY);
            var leftDownXComp   = Math.abs(dimArr[index].leftDownX - leftDownX);
            var leftDownYComp   = Math.abs(dimArr[index].leftDownY - leftDownY);
            var rightDownXComp  = Math.abs(dimArr[index].rightDownX - rightDownX);
            var rightDownYComp  = Math.abs(dimArr[index].rightDownY - rightDownY);

            if( (leftUpXComp <= dobj.width)    && (leftUpYComp <= dobj.height)		||
                (rightUpXComp <= dobj.width)   && (rightUpYComp <= dobj.height)		||
                (leftDownXComp <= dobj.width)  && (leftDownYComp <= dobj.height)	||
                (rightDownXComp <= dobj.width) && (rightDownYComp <= dobj.height) )
            {

				// AJAX
				var dir = document.getElementById('directory').innerHTML;
                var p1 = dobj.id.substr(3);
                var p2 = dimArr[index].obj.id.substr(3);

                var xmlHttp = getXMLHttp();
                xmlHttp.onreadystatechange = function()
                {
                    if (xmlHttp.readyState == 4)
                    {
                        match(xmlHttp.responseText, dir, p1, p2);
                    }
                }
                xmlHttp.open("GET", "ajax.php?cmd=match_pieces&arg1="+dir+"&arg2="+p1+"&arg3="+p2, true);
                xmlHttp.send(null);
           }
        }
    } 

    return false;
}

function match(response, dir, p1, p2)
{
	if(parseInt(response) != 0)
	{
        // parse match response
        var responseOutput 	= response.split(" ");
        var perc = parseInt(responseOutput[0]);
        var firstOffset 	= parseInt(responseOutput[1]);
        var secondOffset 	= parseInt(responseOutput[2]);
        var length 			= parseInt(responseOutput[3]);

        var status_obj = document.getElementById('status');
        status_obj.innerHTML = "Matched " + perc + "%";

        if (perc >= 18)
        {
            status_obj.innerHTML += " (joining)";

            var xmlHttp = getXMLHttp();
            xmlHttp.onreadystatechange = function()
            {
                if (xmlHttp.readyState == 4)
                {
                    join(xmlHttp.responseText, dir, p1, p2);
                }
            }
            xmlHttp.open("GET", "ajax.php?cmd=join_pieces&arg1="+dir+"&arg2="+p1+"&arg3="+p2+"&arg4="+firstOffset+"&arg5="+secondOffset+"&arg6="+length, true);
            xmlHttp.send(null);
        }
        else
        {
            status_obj.innerHTML += " (not joining)";
        }
	}
}

function join(response, dir, p1, p2)
{
    var status_obj = document.getElementById('status');

    response = parseInt(response);

    if (isNaN(response))
    {
        status_obj.innerHTML += " - something broke!";
        return false;
    }
	if (response != 0)
    {
        status_obj.innerHTML += " - deleting pieces " + p1 + " and " + p2;
        var object1 = dimArr["img"+p1].obj;
        var object2 = dimArr["img"+p2].obj;

        delete dimArr[p1];
        delete dimArr[p2];

        var xmlHttp = getXMLHttp();
        object1.parentNode.removeChild(object1);
        xmlHttp.open("GET", "ajax.php?cmd=rm_piece&arg1="+dir+"&arg2="+p1, true);
        xmlHttp.send(null);

        var xmlHttp = getXMLHttp();
        object2.parentNode.removeChild(object2);
        xmlHttp.open("GET", "ajax.php?cmd=rm_piece&arg1="+dir+"&arg2="+p2, true);
        xmlHttp.send(null);

        status_obj.innerHTML += " and adding piece " + response;
        // Add new image
        var img=document.createElement("img");
        img.src = dir + "/" + response + ".png";
        img.className = "dragme";

        img.style.left = currX + "px";
        img.style.top = currY + "px";

        img.id = "img" + response;
        dobj = img;
        document.body.appendChild(img); 

        saveCorners();
    }
    else
    {
        status_obj.innerHTML += " - overlap detected";
    }

    return false;
}

function selectmouse(e) 
{
    var fobj       = e.target;
    var topelement = "HTML";

    while (fobj.tagName != topelement && fobj.className != "dragme")
    {
        fobj = fobj.parentElement;
    }

    if (fobj.className=="dragme")
    {
        ++zTrack;
        isdrag = true;
        dobj = fobj;
        tx = parseInt(dobj.style.left+0);
        ty = parseInt(dobj.style.top+0);
        x = e.clientX;
        y = e.clientY;
        document.onmousemove = movemouse;

        saveCorners();

        return false;
    }
}

function saveCornersMouseUp()
{
    isdrag = false;
    saveCorners();
    collision();
}

document.onmousedown = selectmouse;
document.onmouseup  = saveCornersMouseUp;

</script>

</head>
<body>

<div id='status'>Ready</div>
<h1>Jigsaw Puzzle</h1>
<h2>James Brayton, Matt Goralczyk, Ed Salisbury</h2>
<form action="index.php" method="post" enctype="multipart/form-data">

<div id="Response">Response</div>

<label>Filename:</label>
<input type="file" name="file" id="file" />

<label>Pieces:</label>
<select name='num_pieces'>
	<option value="4">4</option>
	<option value="9">9</option>
	<option value="16">16</option>
	<option value="25">25</option>
	<option value="36">36</option>
	<option value="49">49</option>
	<option value="64">64</option>
	<option value="81">81</option>
	<option value="100">100</option>
</select>

<input type="submit" name="submit" value="Build Puzzle!" />

</form>

<?php

if ($_GET['name'])
{
	$directory = $_GET['name'];
    $directory = strtolower(sanitize_filename($directory));
}

if ($_POST)
{
	// Create directory for image pieces.
	$str = $_FILES["file"]["name"];
	$tmp_file = $_FILES["file"]["tmp_name"];

	list($width, $height, $type, $attr) = getimagesize($tmp_file);

	$size_max = 600;
	$size_min = 60;
	$variance = intval($width / 50);

    $str = strtolower(sanitize_filename($str));
    $basename = substr($str, 0, strlen($str)-4);
    $num_pieces = $_POST['num_pieces'];
    $ext = substr($str, strlen($str)-3);

	if ($variance < 2)
	{
		$variance = 2;
	}

	if (!($width >= $size_min && $width <= $size_max && $height >= $size_min && $height <= $size_max))
	{
		print "<p style='color: #f00'>Error: Required dimensions ({$size_min}x{$size_min} - {$size_max}x{$size_max})</p>";
        exit();
	}

    if ($ext != 'png' && $ext != 'jpg')
    {
        print "<p style='color: #f00'>Error: Only PNG and JPG file formats are supported</p>";
        exit();
    }

    mkdir($basename);
    system("rm $basename/*");

    // Make pieces from image. 
    $filename = "$basename.$ext";
    move_uploaded_file($_FILES["file"]["tmp_name"], $filename);

    $foo = "./make_pieces $filename $num_pieces $variance";
    $sysreturn = system($foo, $retval);
    $directory = $basename;
}

if ($directory)
{
    if (!file_exists($directory))
    {
        print "<p style='color: #f00'>Error: Unable to load puzzle $directory</p>";
        exit();
    }
    print "Puzzle: <a href='index.php?name=$directory'>$directory</a><br />";

	print "<div id='directory'>$directory</div>";
	print "<img src='$directory/0.png'>";

	list($width, $height, $type, $attr) = getimagesize("$directory/0.png");
	$images = array();

	$dir = opendir($directory);
	while (($file = readdir($dir)) !== false)
	{
		if (!preg_match("/^.", $file) && !preg_match("/^0/", $file) && preg_match("/png$/", $file))
		$images[] = $file;
	}

	closedir($dir);

	shuffle($images);

	$i = 1;
	foreach ($images as $img) 
	{
		$num = basename($img, ".png");
		$top = rand(100, 300);
		$left = rand($width + 50, 800);
		echo "<div id=$i><img src='$directory/$img' id=\"img$num\" class=\"dragme\" style='top: {$top}px; left: {$left}px'/></div>";
		++$i;
	}
}

function sanitize_filename($filename)
{
    $special_chars = array("?", "[", "]", "/", "\\", "=", "<", ">", ":", ";", ",", "'", "\"", "&", "$", "#", "*", "(", ")", "|", "~", "`", "!", "{", "}", chr(0));

    $filename = str_replace($special_chars, '', $filename);
    $filename = preg_replace('/[\s-]+/', '-', $filename);
    $filename = trim($filename, '.-_');
    return ($filename);
}
