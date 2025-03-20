#ifndef MAINMENU_H
#define MAINMENU_H

#include "newworkspace.h"
#include "importmenu.h"
#include <QKeyEvent>
#include <QWidget>
#include <QPushButton>

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = nullptr);
private slots:
    void importButtonClick();
    void importWorkspaceDir();
    void importWorkspaceMCPT();
    void importResourcepack();
    void newWorkspace();
private:
    QPushButton *quit_button,*import_button,*quickmerge_button,*newWorkspace_button;
    ImportMenu *w;
    NewWorkspace *n;
signals:
    void quickmerge();
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINMENU_H
