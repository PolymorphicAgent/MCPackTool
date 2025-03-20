#include "mainmenu.h"

#include <QApplication>

MainMenu::MainMenu(QWidget *parent)
    : QWidget{parent}
{

    quit_button         = new QPushButton                 ("Quit",this);
    import_button       = new QPushButton               ("Import",this);
    quickmerge_button   = new QPushButton          ("Quick Merge",this);
    newWorkspace_button = new QPushButton ("Create New Workspace",this);

    newWorkspace_button ->       setToolTip("Create a new workspace");
    import_button       -> setToolTip("Import an existing workspace");
    quickmerge_button   ->      setToolTip("Merge two Resourcepacks");
    quit_button         ->             setToolTip("Exit the program");

    newWorkspace_button -> setFont(*new QFont("Bahnschrift SemiBold",22,2,false));
    import_button       -> setFont(*new QFont("Bahnschrift SemiBold",22,2,false));
    quickmerge_button   -> setFont(*new QFont("Bahnschrift SemiBold",26,2,false));
    quit_button         -> setFont(*new QFont("Bahnschrift SemiBold",36,2,false));

    newWorkspace_button -> setGeometry(80,60 ,640,100);
    import_button       -> setGeometry(80,190,640,100);
    quickmerge_button   -> setGeometry(80,320,640,100);
    quit_button         -> setGeometry(80,450,640,100);

    connect(import_button,       SIGNAL (clicked()),     this,                 SLOT (importButtonClick()));
    connect(newWorkspace_button, SIGNAL (clicked()),     this,                 SLOT      (newWorkspace()));
    connect(quit_button,         SIGNAL (clicked()), QApplication::instance(), SLOT              (quit()));
    connect(quickmerge_button,   SIGNAL (clicked()),     this,                 SIGNAL      (quickmerge()));

    w = new ImportMenu;
    connect(w, SIGNAL (workspaceDir()), this, SLOT (importWorkspaceDir()));
    connect(w, SIGNAL (workspaceMCPT()),this, SLOT(importWorkspaceMCPT()));
    connect(w, SIGNAL (resourcepack()), this, SLOT (importResourcepack()));

    n = new NewWorkspace;

}

void MainMenu::newWorkspace(){
    n->setWindowTitle("Create New Workspace");
    n->show();
    n->raise();
}

void MainMenu::importButtonClick(){
    w->setWindowTitle("Import");
    w->show();
    w->raise();
}

void MainMenu::importWorkspaceDir(){
    import_button->setText("Importing workspace directory...");
    w->hide();
}

void MainMenu::importWorkspaceMCPT(){
    import_button->setText("Importing workspace MCPT...");
    w->hide();
}

void MainMenu::importResourcepack(){
    import_button->setText("Importing resourcepack...");
    w->hide();
}

void MainMenu::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Escape)
        quit_button->click();
    else
        // Let the base class handle the event
        QWidget::keyPressEvent(event);

}
