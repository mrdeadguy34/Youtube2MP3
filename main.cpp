#include "basewindow.h"
#include "filedownload.h"
#include "json.hpp"
#include <QtCore>
#include <QtWidgets>
#include <QApplication>
#include <Windows.h>

QString getYoutubeID(QString Url)
{
    QRegularExpressionMatch IDMatch = idReg.match(Url);
    return IDMatch.captured(0);
}

QString getPlaylistID(QString Url) 
{
    QRegularExpressionMatch IDMatch = playlistIDReg.match(Url);
    return IDMatch.captured(0);
}

QString getLength(std::string ISOString)
{
    //Get the QString in format "(m):(s)" from ISO 8601 duration string. Does not accept hours
    QString ISOQString = QString::fromStdString(ISOString);
    ISOQString.remove("PT").remove("S");
    QStringList Times = ISOQString.split("M");
    QString Minutes = Times.value(0);
    QString Seconds = Times.value(1);
    if (Seconds.toInt() < 10){
        Seconds = "0"+Seconds;
    }
    QString formatTime = Minutes + ":" + Seconds;
    return formatTime;
}

void execCmd(const char* command)
{
    //Command to open up cmd without causing a window pop up
    //Used to run youtube-dl and pass arguments
    wchar_t *path = (wchar_t*) malloc(sizeof(wchar_t) * 500);
    for (int i = 0; i < strlen(command) + 1; i++)
    {
        path[i] = command[i];
    }

    PROCESS_INFORMATION pi;
    STARTUPINFO si;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL,
        path,
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi))
    {
        qDebug() << "Could not start command "<< QString::fromLocal8Bit(command) << GetLastError();
        return;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void createErrorMessage(QString Error) 
{
    QMessageBox* errorBox = new QMessageBox();
    errorBox->setIcon(QMessageBox::Warning);
    errorBox->setWindowTitle("ERROR");
    errorBox->setText("The program has encountered an error:");
    errorBox->setInformativeText(Error);
    errorBox->setStandardButtons(QMessageBox::Abort);
    QObject::connect(errorBox, &QMessageBox::buttonClicked, errorBox, &QMessageBox::close);
    errorBox->exec();
}

QFont scaleFont(QFont Font, QString text, int width, int height)
{
    //Iterate through to find appropriate font to represent the text in the label
    QLabel Label;
    Label.setFont(Font);
    Label.resize(width, height);
    QRect bound = QFontMetrics(Font).boundingRect(
                0, 0, 320, 180, Qt::AlignCenter, text);
    bool fit = (bound.width() <= Label.width() &&
                bound.height() <= Label.height());
    while (!fit) {
        Font.setPointSize(Font.pointSize() - 1);
        QRect bound = QFontMetrics(Font).boundingRect(
                    0, 0, 320, 180, Qt::AlignCenter, text);
        if ((bound.width() <= Label.width() &&
             bound.height() <= Label.height())) {
            return Font;
        }
    }
    return Font;

}

void SetSize(QWidget *window, int x,int y)
{
    //Force QWidget size
    window->resize(x,y);
    window->setMaximumSize(x,y);
    window->setMinimumSize(x,y);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Youtube2MP3");
    qDebug() << "Version 0.3.2";

    BaseWindow MainWindow;
    MainWindow.show();
    MainWindow.setAttribute(Qt::WA_QuitOnClose);

    return app.exec();
}


