#include "datalabel.h"
#include "basewindow.h"
#include <QtCore>
#include <QtWidgets>
#include <QApplication>

DataLabel::DataLabel(QPixmap Small, QPixmap Large, QString Name, QString url, QString id, QString directory, QString duration, int Length)
{
    LogoSmall = Small;
    LogoLarge = Large;
    Title = Name;
    Url = url;
    ID = id;
    Directory = directory;
    Duration = duration;
    length = Length;
}

DataWidget::DataWidget(QWidget *parent, DataLabel* data)
    : QWidget(parent)
{
    //Initialising the widget and all components
    VidData = data;
    setStyleSheet("QWidget {background-color: rgb(230, 230, 240);}");
    move(0, 24);
    setFont(QFont("Arial",25));
    SetSize(this, 730, 408);
    PictureLabel = new QLabel(this);
    PictureLabel->setPixmap(VidData->LogoLarge);
    PictureLabel->setMask(VidData->LogoLarge.mask());
    PictureLabel->setGeometry(205,0, 320,180);
    PictureLabel->show();
    titleLabel = new QLabel(VidData->Title, this);
    titleLabel->setGeometry(0,190, 730,45);
    titleLabel->setFont(scaleFont(QFont("Arial", 20), VidData->Title, 730, 45));
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->show();
    timeLabel = new QLabel(VidData->Duration,this);
    timeLabel->setFont(QFont("Arial",20));
    timeLabel->setGeometry(0,235, 730,40);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->show();
    urlLabel = new QLabel("Url: "+VidData->Url, this);
    urlLabel->setFont(scaleFont(QFont("Arial", 20), VidData->Url, 730, 45));
    urlLabel->setGeometry(0,280, 730,45);
    urlLabel->setAlignment(Qt::AlignCenter);
    urlLabel->show();
    DelButton = new QPushButton("Delete", this);
    DelButton->setFont(QFont("Arial",20));
    DelButton->setStyleSheet("QPushButton {"
                             "background-color: rgb(130, 130, 130);"
                             "color: rgb(200, 200, 200);}"
                             "QPushButton::pressed {"
                             "background-color: rgb(110, 110, 110);}");
    DelButton->setGeometry(110,330, 200,60);
    DelButton->show();
    NextButton = new QPushButton("Next", this);
    NextButton->setFont(QFont("Arial", 20));
    NextButton->setStyleSheet("QPushButton {"
                             "background-color: rgb(130, 130, 130);"
                             "color: rgb(200, 200, 200);}"
                             "QPushButton::pressed {"
                             "background-color: rgb(110, 110, 110);}");
    NextButton->setGeometry(420,330, 200,60);
    NextButton->show();
    connect(DelButton, &QPushButton::released, this, &DataWidget::DeletePressed);
    connect(NextButton, &QPushButton::released, this, &DataWidget::NextPressed);
}

void DataWidget::NextPressed()
{
    emit NextData(VidData->ID);
}

void DataWidget::DeletePressed()
{
    emit DeleteThis(VidData->ID);
}
