# Youtube2MP3
Beta version of youtube-mp3 GUI program

Requirements:

~~Python installed and added to the path~~
#
# -New release uses libcUrl to download the files
# -See latest release if you do not wish to install python
# 
youtube-dl added to the path
ffmpeg added to the path

external headers:
~~picosha2 for SHA256 IDs~~ #Now redundant after libcUrl patch
json for modern C++ (json.hpp) for parsing the youtube JSON API

#Changes for 0.3:

-Added generalised error boxes to help show where errors may lie
-Added functionality for importing entire playlists through youtube
-Added more poorly written comments
-Fixed a major bug causing QString to const char* conversions to fail...
 therefore causing the application to incorrectly download youtube json and or incorrectly store it in the wrong file
-Fixed a bug causing the DataWidgets to be initially shown in the wrong order
-Added font scaling so titles and urls will now correctly fit on the screen

#Hopeful upcoming fixes

-Add a progress box for downloading files
 (remnants of this are already in the source code. I am currently unable to get a responsive UI while downloading the songs)
-Additional options for displaying DataWidgets moire compactly

currently very early beta: more updates soon

Will be lots of errors please inform me of them so I can fix them ty <3
