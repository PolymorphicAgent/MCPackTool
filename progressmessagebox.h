#ifndef PROGRESSMESSAGEBOX_H
#define PROGRESSMESSAGEBOX_H

#include <QProgressBar>
#include <QHBoxLayout>
#include <QDialog>
#include <QWidget>
#include <QLabel>

class ProgressMessageBox : public QDialog
{
public:
    ProgressMessageBox(const QString&, const QString&);
    QProgressBar *progressBar();
    void setProgress(int);
    int getProgress(void);
private:
    QLabel *statement;
    QProgressBar *bar;
};

#endif // PROGRESSMESSAGEBOX_H
