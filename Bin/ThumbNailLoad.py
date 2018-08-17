"""
Python file to download the thumbails
Saves to local file in the ID given
"""

import sys,os
import urllib.request as getUrl

smallUrl = sys.argv[1]
highUrl = sys.argv[2]
ID = sys.argv[3]
#print(sys.argv)

#Download the jpg thumbnails
getUrl.urlretrieve(smallUrl, "Bin/Data/{}/smallThumbnail.jpg".format(ID))
getUrl.urlretrieve(highUrl, "Bin/Data/{}/bigThumbnail.jpg".format(ID))
sys.exit()
