#include "quickmergesettings.h"
#include "utils.h"

QuickMergeSettings::QuickMergeSettings(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(570,620);

    setWindowModality(Qt::ApplicationModal);//block input to other windows

    fileDialog = new QFileDialog(this);
#ifdef Q_OS_WIN
    mcDir = new QDir("C:\\Users\\"+Utils::getUser()+"\\AppData\\Roaming\\.minecraft");
    dirSlash = "\\";
#elif defined(Q_OS_MAC)
    mcDir = new QDir(QDir::homePath()+"/Library/Application Support/minecraft");
    dirSlash = "/";
#elif defined(Q_OS_LINUX)
    mcDir = new QDir(QDir::homePath()+"/.minecraft");\
    dirSlash = "/";
#endif
    noSpace  =  new QRegularExpression("[^\\s\\n\\r]*");

    info   = new QIcon  (":/icons/info.ico");
    folder = new QIcon(":/icons/folder.ico");
    check  = new QIcon (":/icons/check.ico");

    back =  new QPushButton                         ("Back", this);
    back -> setFont(*new QFont("Bahnschrift SemiBold",10,1,false));
    back -> setGeometry                         (15 ,575, 50 , 30);

    apply =  new QPushButton                        ("Apply", this);
    apply -> setFont(*new QFont("Bahnschrift SemiBold",10,1,false));
    apply -> setGeometry                         (500,575, 50 , 30);

    modeF  = new QFrame              (this);
    modeF -> setFrameShape    (QFrame::Box);
    if(Utils::getSystemTheme() != Qt::ColorScheme::Dark)
        modeF -> setFrameShape  (QFrame::Panel);
    modeF -> setFrameShadow(QFrame::Sunken);
    modeF -> setLineWidth               (2);
    modeF -> setMidLineWidth            (3);
    modeF -> setGeometry  (40 ,47 ,490, 80);

    modeL =  new QLabel("Merge Mode", this);
    modeL -> setFont(*new QFont("Bahnschrift SemiBold",22,1,false));
    modeL -> setFrameShadow(QFrame::Raised);
    modeL -> setFrameShape    (QFrame::Box);
    if(Utils::getSystemTheme() != Qt::ColorScheme::Dark)
        modeL -> setFrameShape  (QFrame::Panel);
    modeL -> setLineWidth               (2);
    modeL -> setMidLineWidth            (3);
    modeL -> setGeometry(45 ,5  , 180, 35 );

    modeI = new QPushButton(*info,"", this);
    modeI -> setToolTip  ("Click for info");
    modeI -> setGeometry(230 , 12 , 20, 20);
    modeI -> setToolTipDuration      (3000);

    modeD =  new QLabel("When nodes from packs conflict,", this);
    modeD -> setFont      (*new QFont("Bahnschrift",16,1,false));
    modeD -> setGeometry                     (50 , 45 , 400, 50);

    modeC =  new QComboBox                             (this);
    modeC -> setFont   (*new QFont("Bahnschrift",12,1,false));
    modeC -> addItem ("Prioritize pack 1 nodes in all cases");
    modeC -> addItem ("Prioritize pack 2 nodes in all cases");
    modeC -> addItem    ("Leave out both nodes in all cases");
    modeC -> addItem("Prompt me at every conflict (advaced)");
    modeC -> setGeometry                 (50 , 90 , 310, 25 );

    infoF  = new QFrame              (this);
    infoF -> setFrameShape    (QFrame::Box);
    if(Utils::getSystemTheme() != Qt::ColorScheme::Dark)
        infoF -> setFrameShape  (QFrame::Panel);
    infoF -> setFrameShadow(QFrame::Sunken);
    infoF -> setLineWidth               (2);
    infoF -> setMidLineWidth            (3);
    infoF -> setGeometry   (40,180,490,200);

    infoI = new QPushButton(*info,"", this);
    infoI -> setToolTip  ("Click for info");
    infoI -> setToolTipDuration      (3000);
    infoI -> setGeometry(190 ,142 , 20, 20);

    infoL =  new QLabel("Pack Info", this);
    infoL -> setFont(*new QFont("Bahnschrift SemiBold",22,1,false));
    infoL -> setFrameShadow(QFrame::Raised);
    infoL -> setFrameShape    (QFrame::Box);
    if(Utils::getSystemTheme() != Qt::ColorScheme::Dark)
        infoL -> setFrameShape  (QFrame::Panel);
    infoL -> setLineWidth               (2);
    infoL -> setMidLineWidth            (3);
    infoL -> setGeometry(45 ,135, 140, 35 );

    infoD =  new QLabel("The output Resourcepack's description should be:",this);
    infoD -> setFont                      (*new QFont("Bahnschrift",16,1,false));
    infoD -> setGeometry                                     (50 , 177, 465, 50);

    infoCD =  new QComboBox                          (this);
    infoCD -> setFont(*new QFont("Bahnschrift",12,1,false));
    infoCD -> addItem                ("Pack 1 description");
    infoCD -> addItem                ("Pack 2 description");
    infoCD -> addItem      ("Pack 1 + Pack 2 descriptions");
    infoCD -> addItem                ("Custom Description");
    infoCD -> setGeometry              (50 , 220, 310, 25 );

    infoN =  new QLabel("The file name of the output file should be:", this);
    infoN -> setFont                  (*new QFont("Bahnschrift",16,1,false));
    infoN -> setGeometry                                (50 , 240, 455, 50 );

    infoCN =  new QComboBox                          (this);
    infoCN -> setFont(*new QFont("Bahnschrift",12,1,false));
    infoCN -> addItem        ("Pack 1 + Pack 2 file names");
    infoCN -> addItem                  ("Pack 1 file name");
    infoCN -> addItem                  ("Pack 2 file name");
    infoCN -> addItem                       ("Custom_name");
    infoCN -> setGeometry              (50 , 280, 310, 25 );

    infoPI =  new QLabel("The pack image of the output pack should be:", this);
    infoPI -> setFont                   (*new QFont("Bahnschrift",16,1,false));
    infoPI -> setGeometry                                 (50 , 300, 455, 50 );

    infoPII = new QPushButton(*info,"", this);
    infoPII -> setToolTip  ("Click for info");
    infoPII -> setToolTipDuration      (3000);
    infoPII -> setGeometry(475 ,315 , 20, 20);

    infoCPI =  new QComboBox                          (this);
    infoCPI -> setFont(*new QFont("Bahnschrift",12,1,false));
    infoCPI -> addItem                 ("Pack 1 pack image");
    infoCPI -> addItem                 ("Pack 2 pack image");
    infoCPI -> addItem                      ("Custom image");
    infoCPI -> setGeometry              (50 , 340, 310, 25 );

    infoPIS =  new QPushButton (*folder,"Select image",this);
    infoPIS -> setFont(*new QFont("Bahnschrift",12,1,false));
    infoPIS -> setCheckable                          (false);
    infoPIS -> setGeometry               (370, 340, 120, 25);

    exportF  = new QFrame              (this);
    exportF -> setFrameShape    (QFrame::Box);
    if(Utils::getSystemTheme() != Qt::ColorScheme::Dark)
        exportF -> setFrameShape  (QFrame::Panel);
    exportF -> setFrameShadow(QFrame::Sunken);
    exportF -> setLineWidth               (2);
    exportF -> setMidLineWidth            (3);
    exportF -> setGeometry  (40, 435,490,130);

    exportL =  new QLabel    ("Export", this);
    exportL -> setFont(*new QFont("Bahnschrift SemiBold",22,1,false));
    exportL -> setFrameShadow(QFrame::Raised);
    exportL -> setFrameShape    (QFrame::Box);
    if(Utils::getSystemTheme() != Qt::ColorScheme::Dark)
        exportL -> setFrameShape  (QFrame::Panel);
    exportL -> setLineWidth               (2);
    exportL -> setMidLineWidth            (3);
    exportL -> setGeometry(45 ,390, 110, 35 );

    exportI = new QPushButton(*info,"", this);
    exportI -> setToolTip  ("Click for info");
    exportI -> setToolTipDuration      (3000);
    exportI -> setGeometry(160 ,398, 20 , 20);

    exportD =  new QLabel               ("Export to: ",this);
    exportD -> setFont(*new QFont("Bahnschrift",16,1,false));
    exportD -> setGeometry               (50 , 430, 100, 50);

    exportC =  new QComboBox                          (this);
    exportC -> setFont(*new QFont("Bahnschrift",12,1,false));
    exportC -> addItem                   (*info,"Minecraft");
    exportC -> addItem                       ("Custom path");
    exportC -> setGeometry              (50 , 470, 310, 25 );

    exportIM = new QPushButton(*info,"", this);
    exportIM -> setToolTip  ("Click for info");
    exportIM -> setToolTipDuration      (3000);
    exportIM -> setGeometry(365, 473, 20 , 20);

    exportM =  new QPushButton(*folder,"Select folder",this);
    exportM -> setFont(*new QFont("Bahnschrift",12,1,false));
    exportM -> setCheckable                          (false);
    exportM -> setGeometry               (390, 470, 120, 25);

    exportT =  new QLabel               ("Export as: ",this);
    exportT -> setFont(*new QFont("Bahnschrift",16,1,false));
    exportT -> setGeometry               (50 , 490, 100, 50);

    exportCT =  new QComboBox                          (this);
    exportCT -> setFont(*new QFont("Bahnschrift",12,1,false));
    exportCT -> addItem                               ("Zip");
    exportCT -> addItem                            ("Folder");
    exportCT -> setGeometry              (50 , 530, 310, 25 );

    exportIT = new QPushButton(*info,"", this);
    exportIT -> setToolTip  ("Click for info");
    exportIT -> setToolTipDuration      (3000);
    exportIT -> setGeometry(365, 533, 20 , 20);

    connect(modeI ,  SIGNAL (               clicked()), this, SLOT (          modeIclicked()));
    connect(infoI ,  SIGNAL (               clicked()), this, SLOT (          infoIclicked()));
    connect(infoPII, SIGNAL (               clicked()), this, SLOT (        infoPIIclicked()));
    connect(infoPIS, SIGNAL (               clicked()), this, SLOT (        infoPISclicked()));
    connect(infoCD , SIGNAL (currentIndexChanged(int)), this, SLOT ( infoCDindexChanged(int)));
    connect(infoCN , SIGNAL (currentIndexChanged(int)), this, SLOT ( infoCNindexChanged(int)));
    //connect(infoCPI, SIGNAL (currentIndexChanged(int)), this, SLOT (infoCPIindexChanged(int)));
    connect(exportI, SIGNAL (               clicked()), this, SLOT (        exportIclicked()));
    connect(exportIM,SIGNAL (               clicked()), this, SLOT (       exportIMclicked()));
    connect(exportIT,SIGNAL (               clicked()), this, SLOT (       exportITclicked()));
    connect(exportM, SIGNAL (               clicked()), this, SLOT (        exportMclicked()));
    connect(exportC, SIGNAL (currentIndexChanged(int)), this, SLOT (exportCindexChanged(int)));
    connect(back,    SIGNAL (               clicked()), this, SLOT (           backClicked()));
    connect(apply,   SIGNAL (               clicked()), this, SLOT (          applyClicked()));

    mode              = modeC    -> currentIndex();
    description       = infoCD   -> currentIndex();
    customDescription = infoCD   ->  currentText();
    name              = infoCN   -> currentIndex();
    customName        = infoCN   ->  currentText();
    image             = infoCPI  -> currentIndex();
    customImage       = infoCPI  ->  currentText();
    toMinecraft       = exportC  -> currentIndex();
    customPath        = exportC  ->  currentText();
    exportAs          = exportCT -> currentIndex();
    applyEvent = false; backEvent = false;
    piis = false; exs = false;

}

void QuickMergeSettings::reset(){
    modeC    -> setCurrentIndex(0);
    infoCD   -> setCurrentIndex(0);
    infoCN   -> setCurrentIndex(0);
    infoCPI  -> setCurrentIndex(0);
    exportC  -> setCurrentIndex(0);
    exportCT -> setCurrentIndex(0);
    infoPIS  -> setIcon(*folder);
    exportM  -> setIcon(*folder);
}

void QuickMergeSettings::modeIclicked(){
    QToolTip::showText(this->mapToGlobal(modeI->pos()), "What to do when both Resourcepacks modify the same texture.", this, {}, 10000);
}

void QuickMergeSettings::infoIclicked(){
    QToolTip::showText(this->mapToGlobal(infoI->pos()), "Select info for the output pack", this, {}, 10000);
}

void QuickMergeSettings::infoPIIclicked(){
    QToolTip::showText(this->mapToGlobal(infoPII->pos()), "Pack image must be square (length = height)", this, {}, 10000);
}

void QuickMergeSettings::infoPISclicked(){
    infoCPI -> setCurrentIndex(2);
    fileDialog -> setFileMode(QFileDialog::ExistingFile);
    fileDialog -> setNameFilter(tr("Images (*.png)"));
    fileDialog -> setDirectory("C:\\Users\\"+Utils::getUser()+"\\Pictures");
    QStringList filesSelected;
    if(fileDialog -> exec()){
        filesSelected = fileDialog->selectedFiles();
        infoCPI -> removeItem(2);
        infoCPI -> addItem(filesSelected[0]);
        infoCPI -> setCurrentIndex(2);
        infoPIS -> setIcon(*check);
    }
}

void QuickMergeSettings::infoCDindexChanged(int index){
    if (index >= 3)
        infoCD->setEditable(true);
    else
        infoCD->setEditable(false);
}

void QuickMergeSettings::infoCNindexChanged(int index){
    if (index >= 3){
        infoCN->setEditable(true);
        infoCN -> setValidator(new QRegularExpressionValidator(*noSpace,0));
    }
    else
        infoCN->setEditable(false);
}

//void QuickMergeSettings::infoCPIindexChanged(int index){
//    if(index == 2) infoPIS -> click();
//}

void QuickMergeSettings::exportIclicked(){
    QToolTip::showText(this->mapToGlobal(exportI->pos()), "Set options for exporting the merged resourcepack", this, {}, 10000);
}

void QuickMergeSettings::exportIMclicked(){
    if(exportC -> currentIndex() == 0)
        QToolTip::showText(this->mapToGlobal(exportIM->pos()), "Minecraft must be installed for this option", this, {}, 10000);
    else
        QToolTip::showText(this->mapToGlobal(exportIM->pos()), "Select a custom output path ↑", this, {}, 10000);
}

void QuickMergeSettings::exportITclicked(){
    if(exportCT -> currentIndex() == 0)
        QToolTip::showText(this->mapToGlobal(exportIT->pos()), "Zipped archive for easy sharing", this, {}, 10000);
    else
        QToolTip::showText(this->mapToGlobal(exportIT->pos()), "Unzipped directory for easy content viewing", this, {}, 10000);
}

void QuickMergeSettings::exportMclicked(){
    exportC -> setCurrentIndex(1);
    fileDialog -> setFileMode(QFileDialog::Directory);
    fileDialog -> setDirectory("C:\\Users\\"+Utils::getUser()+"\\Documents");
    QStringList filesSelected;
    if(fileDialog -> exec()){
        filesSelected = fileDialog->selectedFiles();
        exportC -> removeItem(1);
        exportC -> addItem(filesSelected[0]);
        exportC -> setCurrentIndex(1);
        exportM -> setIcon(*check);
    }

}

void QuickMergeSettings::exportCindexChanged(int index){
    if(index == 0)
        exportCT -> setCurrentIndex(0);
}

void QuickMergeSettings::applyClicked(){
    if(exportC -> currentIndex() == 0)
        exportCT -> setCurrentIndex(0);
    if(infoCPI -> currentIndex() == 2&&Utils::compareQIcon(infoPIS->icon(),*folder)){
        QMessageBox::critical(this, "MCPackTool", "Please select a pack PNG.", QMessageBox::Ok);
    }else if(exportC -> currentIndex() == 1&&Utils::compareQIcon(exportM->icon(),*folder)){
        QMessageBox::critical(this, "MCPackTool", "Please select an output directory.", QMessageBox::Ok);
    }else if(infoCPI->currentIndex()==2&&!Utils::isSquare(infoCPI->currentText())){
        QMessageBox::critical(this, "MCPackTool", "Selected pack PNG is either invalid or not square.", QMessageBox::Ok);
    }else if(exportC -> currentIndex() == 0 && !mcDir->exists()){
        QMessageBox::StandardButton choice = QMessageBox::warning(this, "MCPackTool", "Minecraft not found. \nSelect .minecraft directory? (advanced)", QMessageBox::Yes | QMessageBox::No);
        if(choice == QMessageBox::Yes){
            fileDialog -> setFileMode(QFileDialog::Directory);
            fileDialog -> setDirectory("C:\\Users\\"+Utils::getUser()+"\\AppData\\Roaming");
            QStringList filesSelected;
            if(fileDialog -> exec()){
                filesSelected = fileDialog->selectedFiles();
                if((new QDir(filesSelected[0]+dirSlash+"resourcepacks"))->exists()){
                    exportC -> removeItem(1);
                    exportC -> addItem(filesSelected[0]+dirSlash+"resourcepacks");
                    exportC -> setCurrentIndex(1);
                    mode              = modeC    -> currentIndex();
                    description       = infoCD   -> currentIndex();
                    customDescription = infoCD   ->  currentText();
                    name              = infoCN   -> currentIndex();
                    customName        = infoCN   ->  currentText();
                    image             = infoCPI  -> currentIndex();
                    customImage       = infoCPI  ->  currentText();
                    toMinecraft       = exportC  -> currentIndex();
                    customPath        = exportC  ->  currentText();
                    exportAs          = exportCT -> currentIndex();
                    if(Utils::compareQIcon(infoPIS->icon(), *check))
                        piis = true;
                    if(Utils::compareQIcon(exportM->icon(), *check))
                        exs = true;
                    applyEvent = true;
                    emit quickMergeSettngsChanged(mode,
                                                  description, customDescription,
                                                  name,        customName,
                                                  image,       customImage,
                                                  toMinecraft, customPath,
                                                  exportAs);
                    this -> close();
                }else{
                    QMessageBox::critical(this, "MCPackTool", "Invalid directory.", QMessageBox::Ok);
                }
            }
        }
    }else{
        mode              = modeC    -> currentIndex();
        description       = infoCD   -> currentIndex();
        customDescription = infoCD   ->  currentText();
        name              = infoCN   -> currentIndex();
        customName        = infoCN   ->  currentText();
        image             = infoCPI  -> currentIndex();
        customImage       = infoCPI  ->  currentText();
        toMinecraft       = exportC  -> currentIndex();
        customPath        = exportC  ->  currentText();
        exportAs          = exportCT -> currentIndex();
        if(Utils::compareQIcon(infoPIS->icon(), *check))
            piis = true;
        if(Utils::compareQIcon(exportM->icon(), *check))
            exs = true;
        applyEvent = true;
        emit quickMergeSettngsChanged(mode,
                                      description, customDescription,
                                      name,        customName,
                                      image,       customImage,
                                      toMinecraft, customPath,
                                      exportAs);
        this -> close();
    }
}

void QuickMergeSettings::backClicked(){
    modeC    ->             setCurrentIndex(mode);
    infoCD   ->      setCurrentIndex(description);
    infoCD   -> setCurrentText(customDescription);
    infoCN   ->             setCurrentIndex(name);
    infoCN   ->        setCurrentText(customName);
    infoCPI  ->            setCurrentIndex(image);
    infoCPI  ->       setCurrentText(customImage);
    exportC  ->      setCurrentIndex(toMinecraft);
    exportC  ->        setCurrentText(customPath);
    exportCT ->         setCurrentIndex(exportAs);
    if(piis)
        infoPIS -> setIcon(*check);
    else
        infoPIS -> setIcon(*folder);
    if(exs)
        exportM -> setIcon(*check);
    else
        exportM -> setIcon(*folder);
    backEvent = true;
    this -> close();
}

void QuickMergeSettings::closeEvent (QCloseEvent *event){
    if(!applyEvent&&!backEvent){
        QMessageBox::StandardButton resBtn =

            QMessageBox::question( this, "MCPackTool", tr("Save changes?\n"),
                                  QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

        if (resBtn == QMessageBox::Yes){
            if(exportC -> currentIndex() == 0)
                exportCT -> setCurrentIndex(0);
            if(infoCPI -> currentIndex() == 2&&Utils::compareQIcon(infoPIS->icon(),*folder)){
                QMessageBox::critical(this, "MCPackTool", "Please select a pack PNG.", QMessageBox::Ok);
                event->ignore();
            }else if(exportC -> currentIndex() == 1&&Utils::compareQIcon(exportM->icon(),*folder)){
                QMessageBox::critical(this, "MCPackTool", "Please select an output directory.", QMessageBox::Ok);
                event->ignore();
            }else if(infoCPI->currentIndex()==2&&!Utils::isSquare(infoCPI->currentText())){
                QMessageBox::critical(this, "MCPackTool", "Selected pack PNG is either invalid or not square.", QMessageBox::Ok);
                event->ignore();
            }else if(exportC -> currentIndex() == 0 && !mcDir->exists()){
                QMessageBox::StandardButton choice = QMessageBox::warning(this, "MCPackTool", "Minecraft not found. \nSelect .minecraft directory? (advanced)", QMessageBox::Yes | QMessageBox::No);
                if(choice == QMessageBox::Yes){
                    fileDialog -> setFileMode(QFileDialog::Directory);
                    fileDialog -> setDirectory("C:\\Users\\"+Utils::getUser()+"\\AppData\\Roaming");
                    QStringList filesSelected;
                    if(fileDialog -> exec()){
                        filesSelected = fileDialog->selectedFiles();
                        if((new QDir(filesSelected[0]+dirSlash+"resourcepacks"))->exists()){
                            exportC -> removeItem(1);
                            exportC -> addItem(filesSelected[0]+dirSlash+"resourcepacks");
                            exportC -> setCurrentIndex(1);
                            mode              = modeC    -> currentIndex();
                            description       = infoCD   -> currentIndex();
                            customDescription = infoCD   ->  currentText();
                            name              = infoCN   -> currentIndex();
                            customName        = infoCN   ->  currentText();
                            image             = infoCPI  -> currentIndex();
                            customImage       = infoCPI  ->  currentText();
                            toMinecraft       = exportC  -> currentIndex();
                            customPath        = exportC  ->  currentText();
                            exportAs          = exportCT -> currentIndex();
                            if(Utils::compareQIcon(infoPIS->icon(), *check))
                                piis = true;
                            if(Utils::compareQIcon(exportM->icon(), *check))
                                exs = true;
                            applyEvent = true;
                            emit quickMergeSettngsChanged(mode,
                                                          description, customDescription,
                                                          name,        customName,
                                                          image,       customImage,
                                                          toMinecraft, customPath,
                                                          exportAs);
                            this -> close();
                        }else{
                            QMessageBox::critical(this, "MCPackTool", "Invalid directory.", QMessageBox::Ok);
                        }
                    }
                }
            }else{
                mode              = modeC    -> currentIndex();
                description       = infoCD   -> currentIndex();
                customDescription = infoCD   ->  currentText();
                name              = infoCN   -> currentIndex();
                customName        = infoCN   ->  currentText();
                image             = infoCPI  -> currentIndex();
                customImage       = infoCPI  ->  currentText();
                toMinecraft       = exportC  -> currentIndex();
                customPath        = exportC  ->  currentText();
                exportAs          = exportCT -> currentIndex();
                if(Utils::compareQIcon(infoPIS->icon(), *check))
                    piis = true;
                if(Utils::compareQIcon(exportM->icon(), *check))
                    exs = true;
                emit quickMergeSettngsChanged(mode,
                                              description, customDescription,
                                              name,        customName,
                                              image,       customImage,
                                              toMinecraft, customPath,
                                              exportAs);
                event->accept();
            }
        }else if(resBtn == QMessageBox::No){
            modeC    ->             setCurrentIndex(mode);
            infoCD   ->      setCurrentIndex(description);
            infoCD   -> setCurrentText(customDescription);
            infoCN   ->             setCurrentIndex(name);
            infoCN   ->        setCurrentText(customName);
            infoCPI  ->            setCurrentIndex(image);
            infoCPI  ->       setCurrentText(customImage);
            exportC  ->      setCurrentIndex(toMinecraft);
            exportC  ->        setCurrentText(customPath);
            exportCT ->         setCurrentIndex(exportAs);
            if(piis)
                infoPIS ->  setIcon(*check);
            else
                infoPIS -> setIcon(*folder);
            if(exs)
                exportM ->  setIcon(*check);
            else
                exportM -> setIcon(*folder);
            event->accept();
        }else {
            event->ignore();
        }
    }else{
        applyEvent = false;
        backEvent = false;
    }
}

//handle if the escape key is pressed
void QuickMergeSettings::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Escape)
        backClicked();
    else
        // Let the base class handle the event
        QWidget::keyPressEvent(event);

}
