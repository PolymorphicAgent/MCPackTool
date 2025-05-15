#include "quickmerge.h"
#include "utils.h"

QuickMerge::QuickMerge(QWidget *parent)
    : QWidget{parent}
{

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

    fileDialog = new QFileDialog();

    check  = new QIcon (":/icons/check.ico");
    folder = new QIcon(":/icons/folder.ico");
    info   = new QIcon  (":/icons/info.ico");

    Ldiv = new QFrame              (this);
    Ldiv-> setFrameShape    (QFrame::Box);
    if(Utils::getSystemTheme() != Qt::ColorScheme::Dark)
        Ldiv-> setFrameShape  (QFrame::Panel);
    Ldiv-> setFrameShadow(QFrame::Raised);
    Ldiv-> setLineWidth               (2);
    Ldiv-> setMidLineWidth            (3);
    Ldiv-> setGeometry   (47,115,350,200);

    Rdiv = new QFrame              (this);
    Rdiv-> setFrameShape    (QFrame::Box);
    if(Utils::getSystemTheme() != Qt::ColorScheme::Dark)
        Rdiv-> setFrameShape  (QFrame::Panel);
    Rdiv-> setFrameShadow(QFrame::Raised);
    Rdiv-> setLineWidth               (2);
    Rdiv-> setMidLineWidth            (3);
    Rdiv-> setGeometry  (400,115,350,200);

    back  =  new QPushButton("Back"            ,this);
    pack1 =  new QPushButton(*folder, "  No file selected",this);
    pack2 =  new QPushButton(*folder, "  No file selected",this);
    merge =  new QPushButton("Merge!"          ,this);
    settings=new QPushButton("Merge Settings  ",this);

    pack1 -> setIconSize(QSize(32,32));
    pack2 -> setIconSize(QSize(32,32));

    pack1ButtonLabel = new QLabel("...", this);
    pack2ButtonLabel = new QLabel("...", this);

    pack1ButtonLabel -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    pack2ButtonLabel -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    pack1ButtonLabel  -> setFont(*new QFont("Bahnschrift",24,2,false));
    pack2ButtonLabel  -> setFont(*new QFont("Bahnschrift",24,2,false));

    pack1ButtonLabel  -> setGeometry(68 ,195,300,50);
    pack2ButtonLabel  -> setGeometry(424,195,300,50);

    back  ->  setCheckable(false);
    merge ->  setCheckable(false);
    settings->setCheckable(false);

    back  ->  setToolTip        ("Go back to main menu");
    pack1 ->  setToolTip      ("Click to select a file");
    pack2 ->  setToolTip      ("Click to select a file");
    merge ->  setToolTip   ("Begin Quick Merge process");
    settings->setToolTip("Set merge settings and rules");

    pack1L = new QLabel("Resourcepack 1"                   ,this);
    pack2L = new QLabel("Resourcepack 2"                   ,this);
    title  = new QLabel("Select two resourcepacks to merge",this);

    back   -> setFont(*new QFont("Bahnschrift SemiBold",10,1,false));
    pack1  -> setFont(*new QFont("Bahnschrift SemiBold",22,2,false));
    pack2  -> setFont(*new QFont("Bahnschrift SemiBold",22,2,false));
    merge  -> setFont(*new QFont("Bahnschrift SemiBold",32,2,false));
    settings->setFont(*new QFont("Bahnschrift SemiBold",30,2,false));
    pack1L -> setFont(*new QFont("Bahnschrift SemiBold",26,2,false));
    pack2L -> setFont(*new QFont("Bahnschrift SemiBold",26,2,false));
    title  -> setFont(*new QFont("Bahnschrift SemiBold",32,2,false));

    back   -> setGeometry(10 ,10 ,50 ,30);
    pack1  -> setGeometry(66 ,250,300,50);
    pack2  -> setGeometry(434,250,300,50);
    merge  -> setGeometry(50 ,440,700,75);
    settings->setGeometry(50 ,340,700,75);
    pack1L -> setGeometry(100,150,250,50);
    pack2L -> setGeometry(450,150,250,50);
    title  -> setGeometry(150, 50,500,50);

    connect(back,  SIGNAL (clicked()), this, SIGNAL (toMain()));
    connect(pack1, SIGNAL (clicked()), this, SLOT  (selectL()));
    connect(pack2, SIGNAL (clicked()), this, SLOT  (selectR()));
    connect(merge, SIGNAL (clicked()), this, SLOT     (init()));

    qs = new QuickMergeSettings;

    connect(settings, SIGNAL (clicked()), this, SLOT (settingss()));
    connect(qs, SIGNAL (quickMergeSettngsChanged(int,int,QString,int,QString,int,QString,int,QString,int)),
            this, SLOT (recievedSettingsUpdate(int,int,QString,int,QString,int,QString,int,QString,int)));

}

void QuickMerge::reset(){
    p1Path = "";
    p2Path = "";
    pack1ButtonLabel->setText("...");
    pack2ButtonLabel->setText("...");
    Ldiv -> setFrameShadow(QFrame::Raised);
    Rdiv -> setFrameShadow(QFrame::Raised);
    pack1 -> setIcon(*folder);
    pack1->setText("  No file selected");
    pack2->setText("  No file selected");
    pack2 -> setIcon(*folder);
    qs -> reset();
}

void QuickMerge::selectL(){
    fileDialog -> setNameFilter(tr("Zipped Archive (*.zip)"));
    fileDialog -> setFileMode(QFileDialog::ExistingFile);
    fileDialog -> setDirectory("C:\\Users\\"+Utils::getUser()+"\\Documents");
    QStringList filesSelected;
    if(fileDialog -> exec()){
        filesSelected = fileDialog->selectedFiles();
        if(!Utils::isResourcepack(filesSelected[0])){
            QMessageBox::critical(this, "MCPackTool", "Invalid pack.", QMessageBox::Ok);
            Ldiv-> setFrameShadow(QFrame::Raised);
            pack1-> setIcon(*folder);
        }else {
            Ldiv-> setFrameShadow(QFrame::Sunken);
            pack1-> setIcon(*check);
            p1Path = filesSelected[0];
            pack1->setText("  File Selected");
            pack1ButtonLabel->setText(Utils::limitedLength(Utils::extractName(filesSelected[0]), 18));//cant be more than 18 chars long
        }
    }
}

void QuickMerge::selectR(){
    fileDialog -> setNameFilter(tr("Zipped Archive (*.zip)"));
    fileDialog -> setFileMode(QFileDialog::ExistingFile);
    fileDialog -> setDirectory("C:\\Users\\"+Utils::getUser()+"\\Documents");
    QStringList filesSelected;
    if(fileDialog -> exec()){
        filesSelected = fileDialog->selectedFiles();
        if(!Utils::isResourcepack(filesSelected[0])){
            QMessageBox::critical(this, "MCPackTool", "Invalid pack.", QMessageBox::Ok);
            Rdiv-> setFrameShadow(QFrame::Raised);
            pack2-> setIcon(*folder);
        }else {
            Rdiv-> setFrameShadow(QFrame::Sunken);
            pack2-> setIcon(*check);
            p2Path = filesSelected[0];
            pack2->setText("  File Selected");
            pack2ButtonLabel->setText(Utils::limitedLength(Utils::extractName(filesSelected[0]), 18));
        }
    }
}

void QuickMerge::settingss(){
    qs->setWindowTitle("Quick Merge Settings");
    qs->show();
    qs->raise();
}

void QuickMerge::recievedSettingsUpdate(int modeR,
    int descriptionR, QString customDescriptionR,
    int nameR,        QString customNameR,
    int imageR,       QString customImageR,
    int toMinecraftR, QString customPathR,
    int exportAsR)
{
    mode        = modeR;
    description = descriptionR;
    name        = nameR;
    image       = imageR;
    toMinecraft = toMinecraftR;
    exportAs    = exportAsR;

    customDescription = customDescriptionR;
    customName        = customNameR;
    customImage       = customImageR;
    customPath        = customPathR;

}

void QuickMerge::init(){
    if(!(Utils::compareQIcon(pack1->icon(),*check)&&Utils::compareQIcon(pack2->icon(),*check)))
        QMessageBox::critical(this, "MCPackTool", "Please select both resourcepacks.", QMessageBox::Ok);
    else if(p1Path == p2Path)
        QMessageBox::critical(this, "MCPackTool", "Please select a different resourcepack for each node.", QMessageBox::Ok);
    else{
        //need to check toMinecraft if settings not set
        if(!mcDir->exists()){
            QMessageBox::StandardButton choice = QMessageBox::warning(this, "MCPackTool", "Minecraft not found. (configure in settings) \nSelect .minecraft directory? (advanced)", QMessageBox::Yes | QMessageBox::No);
            if(choice == QMessageBox::Yes){
                fileDialog -> setFileMode(QFileDialog::Directory);
                fileDialog -> setDirectory("C:\\Users\\"+Utils::getUser()+"\\AppData\\Roaming");
                QStringList filesSelected;
                if(fileDialog -> exec()){
                    filesSelected = fileDialog->selectedFiles();
                    if((new QDir(filesSelected[0]+dirSlash+"resourcepacks"))->exists()){
                        mcDir -> setPath(filesSelected[0]);
                        Utils::MC_PATH = mcDir->path();
                        //begin merging & pass all them args
                        if(Utils::merge(p1Path,p2Path,mode,
                                                   description,customDescription,
                                                   name,customName,
                                                   image,customImage,
                                                   toMinecraft,customPath,
                                                   exportAs))
                        {
                            QMessageBox::StandardButton post = QMessageBox::question(this, "MCPackTool", "Success! Do you want to return to the Main Menu?", QMessageBox::Yes | QMessageBox::No);
                            if(post == QMessageBox::Yes){
                                //reset to defaults
                                this->reset();
                                emit toMain();
                            }
                        }
                        //else
                        //TODO: error catching

                    }else{
                        QMessageBox::critical(this, "MCPackTool", "Invalid directory.", QMessageBox::Ok);
                    }
                }
            }
        }else{
            Utils::MC_PATH = mcDir->path();
            //begin merging & pass all them args
            if(Utils::merge(p1Path,p2Path,mode,
                                       description,customDescription,
                                       name,customName,
                                       image,customImage,
                                       toMinecraft,customPath,
                                       exportAs))
            {
                QMessageBox::StandardButton post = QMessageBox::question(this, "MCPackTool", "Success! Do you want to return to the Main Menu?", QMessageBox::Yes | QMessageBox::No);
                if(post == QMessageBox::Yes){
                    //reset to defaults
                    this->reset();
                    emit toMain();
                }
            }
            //else
            //TODO: error catching

        }
    }
}

//handle 'esc' key pressed
void QuickMerge::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Escape)
        back->click();
    else
        // Let the base class handle the event
        QWidget::keyPressEvent(event);
}
