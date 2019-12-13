#ifndef DATALABEL_H
#define DATALABEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class DataLabel
{
public:
    DataLabel(QPixmap Small, QPixmap Large, QString Name,
              QString url, QString id, QString directory,
              QString duration, QString outdir, int Length); //Set up of class
    QPixmap LogoSmall; //Small thumbnail
    QPixmap LogoLarge; //LargeThumbnail
    QString Title; //Video title
    QString Url; //Link to video
    QString ID; //Video ID
    QString Directory; //Location of folder in which JSON and thumbails are stored
    QString Duration; //Formatted time (m):(s)
    QString Outdir;
    int length; //Time in seconds
};

class DataWidget : public QWidget
{
    // Widget to show the current inputted videos awaiting download
    Q_OBJECT

public:
    DataWidget(QWidget *parent, DataLabel* data);
    DataLabel* VidData;

public slots:
    void NextPressed();
    void DeletePressed();

private:
    QLabel* PictureLabel;
    QLabel* titleLabel;
    QLabel* timeLabel;
    QLabel* urlLabel;
    QPushButton* DelButton;
    QPushButton* NextButton;

signals:
    void DeleteThis(QString ID);
    void NextData(QString ID);
};

#endif // DATALABEL_H
