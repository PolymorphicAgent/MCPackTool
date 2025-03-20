#ifndef NEWWORKSPACE_H
#define NEWWORKSPACE_H

#include <QLabel>
#include <QWidget>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QLineEdit>
#include <QApplication>
#include <QPushButton>

class NewWorkspace : public QWidget
{
    Q_OBJECT
public:
    explicit NewWorkspace(QWidget *parent = nullptr);
private slots:
    void backClicked();
    void nextClicked();
    void spaceRejected();
private:
    QPushButton *next, *back;
    QLineEdit *nameinp;
    QRegularExpression *noSpace;
    QLabel *prompt;
    QString name;
    int *slide;
signals:
    void newWorkspace(QString,QString,QString,QString);
};

#endif // NEWWORKSPACE_H
