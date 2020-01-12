#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include "datalabel.h"
#include "json.hpp"
#include <QtCore>
#include <QtWidgets>
#include <QApplication>

//Constants
const QRegularExpression idReg("((?<=(v|V)/)|(?<=be/)|(?<=(\\?|\\&)v=)|(?<=embed/))([\\w-]++)"); //Regex to extract youtube id from any url
const QRegularExpression playlistIDReg("((?<=(<)/)|(?<=(\\?|\\&)list=)|(?<=</))([\\w-]++)"); //Regex to extract playlist id from url
//Flags to pass to GetUrl function
const int SUBMIT_VIDEO = 1; 
const int SUBMIT_PLAYLIST = 2;

class Downloads : public QObject
{
    //Worker class to download the files.
    Q_OBJECT

public:
    Downloads(QVector<DataWidget*> Files, QString OutputLocation);
    void DownloadFiles();

private:
    QVector<DataWidget*> DLFiles;
    QString outputLocation;

signals:
    void finished();
    void finishedSong(QString title);
};

class BaseWindow : public QWidget
{

    Q_OBJECT

public:
    BaseWindow(QWidget *parent = 0);
    QString Directory = qApp->applicationDirPath()+"/"; //Main app directory
    nlohmann::json ParseJson(std::fstream& DataFile); //Returns the parsed json from the given file
    void SubmitUrl(QString ytID, int outFlag, QString outdir); //Add file to download list and create widget
    void CheckSettings(); //Ensure that all settings exist
    QString getBgRGBString();
    ~BaseWindow();

private:
    QMenuBar *WindowMenu;
    //Download actions
    QMenu *MainMenu;
    QAction *InputUrl;
    QAction *PlaylistInputUrl;
    QAction *DownloadAction;
    QAction *QuitAction;
    //Settings actions
    QMenu *SettingsMenu;
    QAction *BgColor;
    QAction *KeepBin;
    QAction *OutputDirectory;
    QSettings settings {"Youtube2MP3", "Y2MP3"}; //Application settings
    void CreateActions(); //Create all MainWindow menu actions
    void LoadWidgets(); //Load all created widgets for MainWindow
    void createNewData(QString ID, int outFlag, QString PlaylistID); //Create new data label and data widget for submitted video
    void GetUrl(int Flag); //Open QDialog for inputting an url and begin procedure of adding url to list
    void showLastWidget();
    QString CreateJSONUrl(QString ytID); //Create video JSON url using youtube api v3
    QString CreatePlaylistUrl(QString plID); //Create playlist JSON url using youtube api v3
    QDialog *UrlBox;
    QPushButton *SubmitButton;
    QLineEdit *UrlInput;
    QVector<DataWidget*> DataWidgets; //Vector containing the data widgets
    QVector<QString> IDs; //List of ids to ensure no duplicates
    void DownloadMP3(); //Downloads the MP3 file using cmd line youtube-dl
    const QString apiUrl = "https://www.googleapis.com/youtube/v3/videos?part=snippet,contentDetails&id=%1&key=%2";
    const QString apiPLUrl = "https://www.googleapis.com/youtube/v3/playlistItems?part=contentDetails&maxResults=50&playlistId=%1&key=%2";
    const QString apiKey = "";
    const QString baseOutDir = Directory+"Bin/Data/";

public slots:
    void CallGetUrlForVideo();
    void CallGetUrlForPlaylist();
    void GetIDFromUrlBox();
    void SubmitPlaylistUrl();
    void setColor(); //Set app settings background color
    void setBin(bool checked); //Set whether to auto delete bin files after download
    void setOutputdir(); //Set the download directory for the mp3 folders
    void DeleteData(QString MatchID); //Deletes the DataWidget and DataLabel of the given ID
    void ShowNext(QString ID); //Shows the next DataWidget in list
    void WarnProcessHang(); //Dialog to begin downloads
    void CleanUpAll(); //Delete all DataWidgets
};


//Utility functions (definitely not taken from stack overflow at all)
QString getLength(std::string ISOString);
void execCmd(const char* command); //Run command line programs
void SetSize(QWidget* window, int x,int y); //Sets exact size of windows, disallows resizing
void createErrorMessage(QString Error);
QString getYoutubeID(QString Url); //Use the constant regex to extract yt video ID
QString getPlaylistID(QString Url); //Use the constant regex to extract the playlist ID
QFont scaleFont(QFont Font, QString text, int width, int height); //Scale font to properly fit a labels size

#endif // BASEWINDOW_H
