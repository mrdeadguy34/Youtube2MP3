"""
Script used to socket the youtube API Json.
cUrl was too much effort
python is easy :)
"""
import sys,os
import urllib.request as getUrl

def GetID(url):
    #Extract the ID through basic statements from the URL
    if "youtu.be/" in url:
        ID = url.split(".be/")[1]
        #print("ID = ",ID)
        return ID
    else:
        pass

    try:
        SplitUrl = url.split("watch?v=")[1]
    except:
        ID = None
        return

    if SplitUrl == url:
        ID = None
        return

    ID = SplitUrl.split("&")[0]
    if not ID:
        ID = None
    else:
        #print("ID = ",ID)
        return ID


#print(sys.argv)
VideoUrl = sys.argv[1]
FileName = sys.argv[2].strip("\"")

key = "AIzaSyDMA2wYVUSCjlhXmR3NORSmrJpDnrOP7DU" #Api key to get the JSON

ID = GetID(VideoUrl)
#print(FileName)

if not ID:
    with open("Bin/Data/errors.txt", "w") as F:
        F.write("Error unable to get id from Url = {}".format(VideoUrl))
    sys.exit()

else:
    Url = "https://www.googleapis.com/youtube/v3/videos?part=snippet,contentDetails&id={}&key={}".format(ID,key)

JSON = getUrl.urlopen(Url)
path = "Bin/Data/{}".format(FileName) #relative path to JSON data
if not os.path.isdir(path):
    os.makedirs(path)

with open(path+"/Info.json", "w", encoding="utf-8") as TempFile:
    data = JSON.read().decode("utf-8")
    TempFile.write(data)

sys.exit()
