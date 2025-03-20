#include "conflictprompt.h"

#include <QApplication>
#include <QCheckBox>
#include <QScreen>
#include <QStyle>

#include "utils.h"

ConflictPrompt::ConflictPrompt(const QString& p1nameR, const QString& p1assetR, const QString& p1imageR,
                               const QString& p2nameR, const QString& p2assetR, const QString& p2imageR)
{

    this->p1name  =   p1nameR;
    this->p1asset =  p1assetR;
    this->p1image = new QImage(p1imageR);
    this->p2name  =   p2nameR;
    this->p2asset =  p2assetR;
    this->p2image = new QImage(p2imageR);

    // QIcon icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);
    // QLabel *iconWrapper = new QLabel();
    // iconWrapper->setPixmap(icon.pixmap(64,64));

    // int screenH = QGuiApplication::primaryScreen()->geometry().size().height();
    // int screenW = QGuiApplication::primaryScreen()->geometry().size().width();

    if(p1image->height()<128&&p1image->width()<128){
        //resize to 128x128px
        *p1image = Utils::upscaleImage(*p1image, 128, 128);
    }
    else if(p1image->height()>400/*screenH * 0.5*/){
        *p1image = Utils::downscaleImage(*p1image, 400, 400);//p1image->scaledToWidth(/*p1image->width(),*/ 400, /*Qt::KeepAspectRatio,*/ Qt::SmoothTransformation);
        // qDebug()<<p1image->width();
    }

    if(p2image->height()<128&&p2image->width()<128){
        //resize to 128x128px
        //*p2image = p2image->scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        *p2image = Utils::upscaleImage(*p2image, 128, 128);
    }
    else if(p2image->height()>400/*screenH * 0.5*/){
        *p2image = Utils::downscaleImage(*p2image, 400, 400);//p2image->scaled(400/*p2image->width()*/, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // qDebug()<<p2image->width();
    }

    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *masterLayout = new QVBoxLayout();

    QFrame *lf =           new QFrame();
    lf-> setFrameShape    (QFrame::Box);
    if(Utils::getSystemTheme() != Qt::ColorScheme::Dark)
        lf-> setFrameShape  (QFrame::Panel);
    lf-> setFrameShadow(QFrame::Sunken);
    lf-> setLineWidth               (2);
    lf-> setMidLineWidth            (3);

    QVBoxLayout* layL = new QVBoxLayout(lf);

    QLabel* imageWrapperL = new QLabel;
    imageWrapperL->setPixmap(QPixmap::fromImage(*p1image));
    imageWrapperL->setAlignment(Qt::AlignHCenter);

    selL = new QCheckBox();
    QWidget *selLC = new QWidget();
    QHBoxLayout *selLL = new QHBoxLayout();
    selLL->addWidget(selL);
    selLL->setAlignment(Qt::AlignHCenter);
    selLC->setLayout(selLL);

    QLabel *nameL = new QLabel("Pack: "+p1name);
    nameL->setAlignment(Qt::AlignHCenter);
    nameL->setFont(*new QFont("Bahnschrift",12,1,false));

    QLabel *assetL = new QLabel("File: "+p1asset);
    assetL->setAlignment(Qt::AlignHCenter);
    assetL->setFont(*new QFont("Bahnschrift",12,1,false));

    layL->addWidget(nameL);
    layL->addWidget(imageWrapperL);
    layL->addWidget(assetL);
    layL->addWidget(selLC);

    lf->setLayout(layL);

    mainLayout->addWidget(lf);

    QFrame *rf =           new QFrame();
    rf-> setFrameShape    (QFrame::Box);
    if(Utils::getSystemTheme() != Qt::ColorScheme::Dark)
        rf-> setFrameShape  (QFrame::Panel);
    rf-> setFrameShadow(QFrame::Sunken);
    rf-> setLineWidth               (2);
    rf-> setMidLineWidth            (3);

    QVBoxLayout* layR = new QVBoxLayout(rf);

    QLabel* imageWrapperR = new QLabel();
    imageWrapperR->setPixmap(QPixmap::fromImage(QImage(*p2image)));
    imageWrapperR->setAlignment(Qt::AlignHCenter);

    selR = new QCheckBox();
    QWidget *selRC = new QWidget();
    QHBoxLayout *selRL = new QHBoxLayout();
    selRL->addWidget(selR);
    selRL->setAlignment(Qt::AlignHCenter);
    selRC->setLayout(selRL);

    QLabel *nameR = new QLabel("Pack: "+p2name);
    nameR->setAlignment(Qt::AlignHCenter);
    nameR->setFont(*new QFont("Bahnschrift",12,1,false));

    QLabel *assetR = new QLabel("File: "+p2asset);
    assetR->setAlignment(Qt::AlignHCenter);
    assetR->setFont(*new QFont("Bahnschrift",12,1,false));

    layR->addWidget(nameR);
    layR->addWidget(imageWrapperR);
    layR->addWidget(assetR);
    layR->addWidget(selRC);

    rf->setLayout(layR);

    mainLayout->addWidget(rf);

    // int minWidth = Utils::maxValue(lf->sizeHint().width(), rf->sizeHint().width());
    // int maxWidth = screenW * 0.4;

    // lf->setMaximumWidth(1000/*Utils::minValue(minWidth, maxWidth)*/);
    // rf->setMaximumWidth(1000/*Utils::minValue(minWidth, maxWidth)*/);

    // int minHeight = Utils::maxValue(lf->sizeHint().height(), rf->sizeHint().height());
    // int maxHeight = screenH * 0.75;

    // lf->setMaximumHeight(800/*Utils::minValue(minHeight, maxHeight)*/);
    // rf->setMaximumHeight(800/*Utils::minValue(minHeight, maxHeight)*/);

    QLabel *title = new QLabel("Conflict Detected: Choose one asset.");
    title-> setFont             (*new QFont("Bahnschrift",16,1,false));
    title-> setAlignment                            (Qt::AlignHCenter);

    setLayout(masterLayout);

    layout()->addWidget(title);

    QWidget *mainContainerWidget = new QWidget();
    mainContainerWidget->setLayout(mainLayout);
    layout()->addWidget(mainContainerWidget);

    QPushButton* ok = new QPushButton("Apply");
    QWidget *okW = new QWidget();
    QHBoxLayout *okWL = new QHBoxLayout();
    okWL->addWidget(ok);
    okWL->setAlignment(Qt::AlignHCenter);
    okW->setLayout(okWL);

    layout()->addWidget(okW);

    connect(ok, SIGNAL (clicked()), this, SLOT (accept()));

    connect(selR, &QCheckBox::clicked, this, [this]() {
        updateR(selR->isChecked());
    });

    connect(selL, &QCheckBox::clicked, this, [this]() {
        updateL(selL->isChecked());
    });

    selL->setChecked(true);//default

}

int ConflictPrompt::result(){return this->choice;}

void ConflictPrompt::updateR(bool checked){
    // qDebug()<<"Update Right!"<<checked;
    if(checked && selL->isChecked())
        selL->setChecked(false);
}

void ConflictPrompt::updateL(bool checked){
    // qDebug()<<"Update Left!"<<checked;
    if(checked && selR->isChecked())
        selR->setChecked(false);
}

void ConflictPrompt::accept(){
    if(selL->isChecked())
        choice = 1;
    else if(selR->isChecked())
        choice = 2;
    else
        choice = 0;

    // qDebug()<<choice;

    QDialog::accept();
}
