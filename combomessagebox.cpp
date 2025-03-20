#include "combomessagebox.h"
#include "utils.h"
#include <QApplication>

ComboMessageBox::ComboMessageBox(QStringList items, const QString & message){
    setLayout(new QHBoxLayout());

    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);
    QLabel *iconWrapper = new QLabel();
    iconWrapper->setPixmap(icon.pixmap(64,64));

    layout()->addWidget(iconWrapper);
    // this->setWindowIcon(icon);

    QLabel *statement = new QLabel(message);
    layout()->addWidget(statement);

    box = new QComboBox();
    if(Utils::find("INVALID PACK FORMAT", items)!=-1){
        items.removeAt(Utils::find("INVALID PACK FORMAT",items));
    }
    items[0] += "\t";
    box->addItems(items);
    layout()->addWidget(box);

    QPushButton* ok = new QPushButton("Apply");
    layout()->addWidget(ok);
    connect(ok, SIGNAL (clicked()), this, SLOT (accept()));
}

QComboBox* ComboMessageBox::comboBox() { return box; }
