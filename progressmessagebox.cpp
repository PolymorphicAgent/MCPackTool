#include "progressmessagebox.h"
#include "utils.h"

ProgressMessageBox::ProgressMessageBox(const QString& message, const QString& title) {
    setLayout(new QHBoxLayout());

    statement = new QLabel("   "+message+"   ");
    layout()->addWidget(statement);

    this -> setWindowTitle(title);

    bar = new QProgressBar();
    bar ->setFixedSize(300,35);
    layout()->addWidget(bar);
}

void ProgressMessageBox::setProgress(int p){
    int v = Utils::easedValue(bar->value(),p,0.1);
    bar->setValue(v);
}

int ProgressMessageBox::getProgress(void){
    return bar->value();
}

QProgressBar *ProgressMessageBox::progressBar(){ return bar; }
