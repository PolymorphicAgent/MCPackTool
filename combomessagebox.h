#ifndef COMBOMESSAGEBOX_H
#define COMBOMESSAGEBOX_H

#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QComboBox>
#include <QDialog>
#include <QLabel>

class ComboMessageBox : public QDialog
{
public:
    ComboMessageBox(QStringList, const QString&);
    QComboBox* comboBox();
private:
    QComboBox* box;
};

#endif // COMBOMESSAGEBOX_H
