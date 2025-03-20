#include "importmenu.h"

ImportMenu::ImportMenu(QWidget *parent)
    : QWidget{parent}
{

    setFixedSize(750,450);

    setWindowModality(Qt::ApplicationModal);//block input to other windows

    workspaceDirButton = new QPushButton("Workspace Directory", this);
    workspaceMCPTButton= new QPushButton("MCPT Workspace File", this);
    resourcepackButton = new QPushButton("Resourcepack Archive",this);
    backButton         = new QPushButton("Back",                this);

    workspaceDirButton ->setFont(*new QFont("Bahnschrift SemiBold",22,2,false));
    workspaceMCPTButton->setFont(*new QFont("Bahnschrift SemiBold",22,2,false));
    resourcepackButton ->setFont(*new QFont("Bahnschrift SemiBold",22,2,false));
    backButton         ->setFont(*new QFont("Bahnschrift SemiBold",12,2,false));

    workspaceDirButton ->setToolTip("Import a workspace directory");
    workspaceMCPTButton->setToolTip("Import a *.mcpt workspace file");
    resourcepackButton ->setToolTip("Import a resourcepack to edit");
    backButton         ->setToolTip("Go back to the main menu");

    workspaceDirButton ->setGeometry(80,50 ,590,100);
    workspaceMCPTButton->setGeometry(80,180,590,100);
    resourcepackButton ->setGeometry(80,310,590,100);
    backButton         ->setGeometry(10,410,50 ,30 );

    connect(workspaceDirButton, SIGNAL (clicked()), this, SIGNAL (workspaceDir()));
    connect(workspaceMCPTButton,SIGNAL (clicked()), this, SIGNAL(workspaceMCPT()));
    connect(resourcepackButton, SIGNAL (clicked()), this, SIGNAL (resourcepack()));
    connect(backButton        , SIGNAL (clicked()), this, SLOT           (back()));

}

void ImportMenu::back(){
    this->hide();
}
