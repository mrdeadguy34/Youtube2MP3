#include "basewindow.h"
#include "datalabel.h"
#include "filedownload.h"
#include "json.hpp"
#include <QtCore>
#include <QtWidgets>
#include <QApplication>
#include <cstdlib>
#include <fstream>
#include <string>
#include <Windows.h>


using json = nlohmann::json;

Downloads::Downloads(QVector<DataWidget*> Files, std::string Directory)
{
    DLFiles = Files;
    directory = Directory;
}

void Downloads::DownloadFiles()
{
    //Loops through each element in the vector executing the download command
    int index = 0;
    while (index < DLFiles.count()){
        DataLabel* FileData = DLFiles[index]->VidData;
        QString escapedTitle = FileData->Title.replace("/", "_").replace("\\","-");
        std::string OUTFILE = "-o \""+directory+"Downloads/"+escapedTitle.toStdString()+"/"+escapedTitle.toStdString()+".%(ext)s\"";
        qDebug() << QString::fromStdString(OUTFILE);
        std::string DLCommand = "youtube-dl --extract-audio --audio-format mp3 --audio-quality 0 "+OUTFILE+" "+FileData->Url.toStdString();
        qDebug() << QString::fromStdString(DLCommand);
        execCmd(DLCommand.c_str());
        index++;
        //emit finSong(FileData->Title);
    }
    emit finished();
}

BaseWindow::BaseWindow(QWidget *parent)
    : QWidget(parent)
{
    //Main window set up
    if(!QDir(Directory+"Bin"+"/Data").exists()) {
        QDir().mkdir(Directory+"Bin"+"/Data");
    }
    setWindowTitle(QString("Youtube2MP3"));
    SetSize(this, 730, 420);
    setStyleSheet("QWidget {background-color: rgb(230, 230, 240);}");
    CreateActions();
    LoadWidgets();
}

void BaseWindow::CleanUpAll()
{
    //Function to remove all elements from the vector and hide them
    while (!(DataWidgets.isEmpty())) {
        DataWidgets[0]->hide();
        DataWidgets.remove(0);
    }
}

//void BaseWindow::ShowDLProgress(QString title)
//{
//    ProgLabel->setText("Finished downloading: " + title);
//}

void BaseWindow::DownloadMP3()
{
    //Start the download process for all files
    Downloads* AllFiles = new Downloads(DataWidgets, Directory.toStdString());
    QThread* DLThread = new QThread();
    AllFiles->moveToThread(DLThread);
    connect(AllFiles, &Downloads::finished, this, &BaseWindow::CleanUpAll);
//    connect(AllFiles, &Downloads::finSong, this, &BaseWindow::ShowDLProgress);

//    //Set up progress dialog
//    ProgressBox->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
//    SetSize(ProgressBox, 550, 150);
//    ProgressBox->setWindowTitle("Current Progress");
//    ProgressBox->setModal(true);
//    ProgressBox->setFont(QFont("Arial", 18));
//    ProgressBox->setStyleSheet("QDialog {background-color: rgb(230, 230, 240);}");
//    ProgLabel->setGeometry(0, 20, 550, 60);
//    ProgLabel->setAlignment(Qt::AlignCenter);
//    ProgLabel->show();

//    //progress dialog set up end

    DLThread->start();
    AllFiles->DownloadFiles();
    //ProgressBox->close();
}

void BaseWindow::DeleteData(QString MatchID)
{
    //Loops through to find a matching ID then hiding it and deleting it from the Vector
    for (int i = 0; i < DataWidgets.count(); i++) { //Loop through data widgets
        if (DataWidgets[i]->VidData->ID == MatchID) { //When found the matching id
            DataWidgets[i]->hide();
            DataWidgets.remove(i);
            if (DataWidgets.count() == 0){ //Return if no data left
                break;
            }
            else {
                if (i == DataWidgets.count()) { //If deleted widget was last in list
                    DataWidgets[i-1]->show(); //Show then new last element
            } else {
                    DataWidgets[i]->show();
                }
            }
        }
        else {

        }
    }
}

void BaseWindow::ShowNext(QString ID)
{
    if ( DataWidgets.count() <= 1) {return;}
    //Loops through for a matching ID then shows the widget
    qDebug() << "Data widgets count = " << DataWidgets.count();
    for (int i = 0; i < DataWidgets.count(); i++) {
        if (DataWidgets[i]->VidData->ID == ID){ //When found matching id
            qDebug() << i;
            DataWidgets[i]->hide();
            if (i == DataWidgets.count() -1){
                qDebug() << "load start";
                DataWidgets[0]->show(); //Load initial data widget
                break;
            }
            else {
                qDebug() << "load next one";
                DataWidgets[i+1]->show();
                break;
            }
        }
        else {

        }
    }
}

void BaseWindow::showLastWidget()
{
    //Used to ensure only one Data Widget is currently open.
    if (DataWidgets.count() == 0) {
        return;
    }
    for (int i = 0; i < DataWidgets.count(); i++) {
        DataWidgets[i]->hide();
    }
    DataWidgets[DataWidgets.count()-1]->show();
}

json BaseWindow::ParseJson(std::fstream& DataFile)
{
    if (DataFile.fail()){
        qDebug() << "Failed to load" << strerror(errno);
        createErrorMessage("Failed to load " + QString::fromLocal8Bit(strerror(errno)));
        return NULL;
    }
    qDebug() << "File Loaded";
    json Data;
    //Check if JSON can be parsed into data
    try{
        Data = json::parse(DataFile);
    } catch (json::parse_error &e){
        createErrorMessage("JSON File cannot be parsed - " + QString::fromLocal8Bit(e.what()));
        qDebug() << QString::fromLocal8Bit(e.what());
        return NULL;
    }
    return Data;
}

void BaseWindow::createNewData(QString ID)
{
    //Create the data for a new datalabel
    if (!dirExists(Directory.toStdString()+"/Bin/Data/"+ID.toStdString())) { //check download
        createErrorMessage("Could not find the downloaded song metadata.");
        return;
    }
    
    QString Folder = QString(Directory+"/Bin/Data/"+ID); //Save folder directory
    std::fstream DataFile(QString(Directory+"/Bin/Data/"+ID+"/Info.json").toStdString());
    json Data = ParseJson(DataFile);
    if (Data == NULL) {
        return;
    }
    QString title = QString::fromStdString(Data["items"][0]["snippet"]["title"]);
    std::string ISOLength = Data["items"][0]["contentDetails"]["duration"];
    QString url = "https://youtu.be/"+QString::fromStdString(Data["items"][0]["id"]); //Remove extra tags from url to stop youtube-dl doing extra stuff
    if (QString::fromStdString(ISOLength).contains("H")){
        createErrorMessage("Video too long. Videos must be less than one hour.");
        return;
    }
    QString FormatLength = getLength(ISOLength); //Get formattted length from JSON
    QStringList MinsSecs = FormatLength.split(":");
    QString minutes = MinsSecs.value(0);
    QString secs = MinsSecs.value(1);
    int secsDuration = minutes.toInt() * 60 + secs.toInt(); //Get length of song from formatted length
    std::string smallPic = Data["items"][0]["snippet"]["thumbnails"]["default"]["url"]; //Urls for thumbnails to be downloaded from
    std::string bigPic = Data["items"][0]["snippet"]["thumbnails"]["medium"]["url"];
    if (GetFile((baseOutDir+ID+"/smallThumbnail.jpg").toStdString().c_str(), smallPic.c_str()) == 1 
    && GetFile((baseOutDir+ID+"/bigThumbnail.jpg").toStdString().c_str(), bigPic.c_str()) == 1) {
     //Download both small and big thumbnails
    } else {
        createErrorMessage("Error downloading thumbnails.");
        return;
    }
    QPixmap logoPix(baseOutDir+ID+"/smallThumbnail.jpg");
    QPixmap highPix(baseOutDir+ID+"/bigThumbnail.jpg");
    DataLabel* NewData = new DataLabel(logoPix, highPix, title, url, ID, Folder, FormatLength, secsDuration); //Create the new data
    DataWidget* NewWidget = new DataWidget(this,NewData);
    DataWidgets.append(NewWidget); //Add the new data to the storage
    connect(NewWidget, &DataWidget::DeleteThis, this, &BaseWindow::DeleteData);
    connect(NewWidget, &DataWidget::NextData, this, &BaseWindow::ShowNext);
    showLastWidget();
}

void BaseWindow::SubmitPlaylistUrl() 
{
    QString Url = UrlInput->text();
    UrlBox->close();
    if (Url.isEmpty()) {
        createErrorMessage("No url given.");
        return;
    }
    QString playlistID = getPlaylistID(Url);
    if (playlistID.isEmpty()) {
        createErrorMessage("Could not determine playlist ID from url");
    }
    QString outFile = (baseOutDir + playlistID + "/playlistInfo.json");
    std::ofstream createFile(outFile.toStdString());
    createFile.close();
    if (!QDir(baseOutDir + playlistID).exists()) {
        QDir().mkdir(baseOutDir + playlistID);
    } 
    
    if(GetFile(outFile, CreatePlaylistUrl(playlistID)) == 1){

    } else {
        createErrorMessage("Error downloading playlist metadata.");
        return;
    }

//    if (!dirExists(outFile.toStdString())) {
//        createErrorMessage("Could not find downloaded metadata.");
//        return;
//    }
    std::fstream DataFile(outFile.toStdString());
    json Data = ParseJson(DataFile);
    if (Data == NULL) {
        return;
    }
//    QDialog ProgressBox;
//    ProgressBox.setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
//    SetSize(&ProgressBox, 550, 150);
//    ProgressBox.setWindowTitle("Current Progress");
//    ProgressBox.setModal(true);
//    ProgressBox.setFont(QFont("Arial", 18));
//    ProgressBox.setStyleSheet("QDialog {background-color: rgb(230, 230, 240);}");
//    QLabel *DlProgLabel = new QLabel("", &ProgressBox);
//    DlProgLabel->setGeometry(0, 20, 550, 60);
//    DlProgLabel->setAlignment(Qt::AlignCenter);
//    ProgressBox.show();
    //For loop to pass all given IDs in the playlist to the submiturl function
    for (int i=0; i<Data["pageInfo"]["totalResults"]; i++) {
        QString ID = QString::fromStdString(Data["items"][i]["contentDetails"]["videoId"]);
        //ProgLabel->setText("Now parsing video with ID:" + ID);
        //ProgLabel->show();
        SubmitUrl(ID);
        //ProgLabel->hide();
    }

}

void BaseWindow::SubmitUrl(QString ytID)
{
    //Download dedicated JSON and create new data widget to be downloaded.
    QString outFile = (baseOutDir + ytID + "/Info.json");
    std::ofstream createFile(outFile.toUtf8().constData());
    createFile.close();
    QString URL = CreateJSONUrl(ytID);
    //qDebug() << outFile;
    if (!QDir(baseOutDir+"/"+ytID).exists()) {
        qDebug() << "making directory";
        QDir().mkdir(baseOutDir+"/"+ytID);
    }
    //qDebug() << outFile;
    if (GetFile(outFile, URL) == 1) {
    } else {
        createErrorMessage("Error downloading video metadata.");
        return;
    }
    //qDebug() << URL;
    createNewData(ytID); //Turn JSON into datalabel
}

QString BaseWindow::CreatePlaylistUrl(QString plID)
{
    return apiPLUrl.arg(plID, apiKey).toLocal8Bit();
}
QString BaseWindow::CreateJSONUrl(QString ytID)
{
    return apiUrl.arg(ytID, apiKey);
}

void BaseWindow::GetIDFromUrlBox()
{
    //Retrieve the UrlBox submitted text and pass it to submiturl function
    QString Url = UrlInput->text();
    UrlBox->close();
    if (Url.isEmpty()){
        createErrorMessage("No url given.");
        return;
    }
    QString ID = getYoutubeID(Url);
    if (ID.isEmpty()) {
        createErrorMessage("Could not determine video ID");
        return;
    }
    SubmitUrl(ID);
}

void BaseWindow::GetUrl(int Flag)
{
    //Create window for user to submit an url
    UrlBox = new QDialog();
    UrlBox->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    SetSize(UrlBox, 550, 200);
    UrlBox->setWindowTitle("Input url");
    UrlBox->setModal(true);
    UrlBox->setFont(QFont("Arial", 18));
    UrlBox->setStyleSheet("QDialog {background-color: rgb(230, 230, 240);}");
    QLabel *UrlLabel = new QLabel("Url: ", UrlBox);
    UrlLabel->setGeometry(0,10, 550, 40);
    UrlLabel->setAlignment(Qt::AlignCenter);
    UrlLabel->show();
    UrlInput = new QLineEdit(UrlBox);
    UrlInput->setGeometry(80,60, 390,35);
    UrlInput->setAlignment(Qt::AlignCenter);
    UrlInput->show();
    SubmitButton = new QPushButton("Submit", UrlBox);
    SubmitButton->setGeometry(220,110, 110,50);
    SubmitButton->setStyleSheet("QPushButton {"
                                "background-color: rgb(130, 130, 130);"
                                "color: rgb(200, 200, 200);}"
                                "QPushButton::pressed {"
                                "background-color: rgb(110, 110, 110);}");
    SubmitButton->show();
    if (Flag == 1) {
        connect(SubmitButton, &QPushButton::released, this, &BaseWindow::GetIDFromUrlBox);
    } else if (Flag == 2) {
        connect(SubmitButton, &QPushButton::released, this, &BaseWindow::SubmitPlaylistUrl);
    } else {
        connect(SubmitButton, &QPushButton::released, UrlBox, &QDialog::close);
    } //Should never run
    UrlBox->exec();
}

void BaseWindow::CallGetUrlForVideo() {GetUrl(SUBMIT_VIDEO);}

void BaseWindow::CallGetUrlForPlaylist() {GetUrl(SUBMIT_PLAYLIST);}

void BaseWindow::CreateActions()
{
    //Create actions for the menu bars
    InputUrl = new QAction(tr("&Import url"), this);
    InputUrl->setShortcut(QKeySequence::New);
    InputUrl->setToolTip("Import video from youtube url");
    PlaylistInputUrl = new QAction(tr("&Import playlist"), this);
    PlaylistInputUrl->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_N);
    PlaylistInputUrl->setToolTip("Import videos from youtube playlist");
    DownloadAction = new QAction(tr("&Download files"), this);
    DownloadAction->setShortcut(QKeySequence::Save);
    DownloadAction->setToolTip("Download the inputted files");
    QuitAction = new QAction(tr("&Quit"), this);
    QuitAction->setShortcut(QKeySequence::Quit);
    QuitAction->setToolTip("Close the application");
    connect(InputUrl, &QAction::triggered, this, &BaseWindow::CallGetUrlForVideo);
    connect(PlaylistInputUrl, &QAction::triggered, this, &BaseWindow::CallGetUrlForPlaylist);
    connect(DownloadAction, &QAction::triggered, this, &BaseWindow::DownloadMP3);
    connect(QuitAction, &QAction::triggered, this, &QApplication::quit);
}

void BaseWindow::LoadWidgets()
{
    //Load main widgets with stylesheets
    WindowMenu = new QMenuBar(this);
    MainMenu = new QMenu("&File");
    MainMenu->addAction(InputUrl);
    MainMenu->addSeparator();
    MainMenu->addAction(PlaylistInputUrl);
    MainMenu->addSeparator();
    MainMenu->addAction(DownloadAction);
    MainMenu->addSeparator();
    MainMenu->addAction(QuitAction);
    WindowMenu->addMenu(MainMenu);
    WindowMenu->setFont(QFont("Arial", 12));
    WindowMenu->setStyleSheet("QMenuBar {"
                              "background-color: rgb(130, 130, 130);"
                              "color: rgb(200,200,200);}"
                              "QMenuBar::item {"
                              "background-color: rgb(130, 130, 130);"
                              "color: rgb(200, 200, 200);}"
                              "QMenuBar::item::selected {"
                              "background-color: rgb(90, 90, 90);}");
    MainMenu->setStyleSheet("QMenu {"
                            "background-color:rgb(130, 130, 130);"
                            "color: rgb(200,200,200);"
                            "border: 1px solid;}"
                            "QMenu::item::selected {"
                            "background-color: rgb(90, 90,90);}");
    WindowMenu->show();
}

BaseWindow::~BaseWindow()
{

}
