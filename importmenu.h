#ifndef IMPORTMENU_H
#define IMPORTMENU_H

#include <QWidget>
#include <QFont>
#include <QPushButton>

class ImportMenu : public QWidget
{
    Q_OBJECT
public:
    explicit ImportMenu(QWidget *parent = nullptr);
private slots:
    void back();
private:
    QPushButton *workspaceDirButton, *workspaceMCPTButton, *resourcepackButton,*backButton;
signals:
    void workspaceDir();
    void workspaceMCPT();
    void resourcepack();
};

#endif // IMPORTMENU_H
