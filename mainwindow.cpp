#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

MainWindow::MainWindow(QuickMerge* const qOther, MainMenu* const mOther)
    : QMainWindow()
    , ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    q = qOther;
    m = mOther;
    connect(qOther, SIGNAL (toMain())    , this, SLOT (toMain())    );
    connect(mOther, SIGNAL (quickmerge()), this, SLOT (quickmerge()));
    Utils::mkdir(".mctmp");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete q;
    delete m;
    // Utils::rm(".mctmp");
}

void MainWindow::quickmerge(){
    this->takeCentralWidget();
    this->setFixedSize(800,550);
    this->setCentralWidget(q);
}

void MainWindow::toMain(){
    this->takeCentralWidget();
    this->setFixedSize(800,600);
    this->setCentralWidget(m);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Escape)
        toMain();

    else
        // Let the base class handle the event
        QWidget::keyPressEvent(event);

}
