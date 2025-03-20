#include "newworkspace.h"

NewWorkspace::NewWorkspace(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(750,450);

    setWindowModality(Qt::ApplicationModal);//block input to other windows

    slide=0;

    name = *new QString;

    back    =  new QPushButton                         ("Back",this);
    next    =  new QPushButton                         ("Next",this);
    nameinp =  new QLineEdit                      ("Workspace",this);
    prompt  =  new QLabel("Enter a name for the new workspace",this);

    back    -> setToolTip            ("Go Back");
    next    -> setToolTip("Go to the next step");
    nameinp -> setToolTip ("Enter a valid name");
    nameinp -> setAlignment    (Qt::AlignCenter);

    back    -> setFont(*new QFont("Bahnschrift SemiBold",12,2,false));
    next    -> setFont(*new QFont("Bahnschrift SemiBold",12,2,false));
    nameinp -> setFont(*new QFont("Bahnschrift SemiBold",18,2,false));
    prompt  -> setFont(*new QFont("Bahnschrift SemiBold",22,2,false));

    back    -> setGeometry(10 ,410,50 ,30);
    next    -> setGeometry(690,410,50 ,30);
    nameinp -> setGeometry(225,220,325,30);
    prompt  -> setGeometry(135,150,600,50);

    connect(back,    SIGNAL (clicked()),       this, SLOT (backClicked()));
    connect(next,    SIGNAL (clicked()),       this, SLOT (nextClicked()));
    connect(nameinp, SIGNAL (returnPressed()), this, SLOT (nextClicked()));

    noSpace  =  new QRegularExpression("[^\\s\\n\\r]*");
    nameinp  -> setValidator(new QRegularExpressionValidator(*noSpace,0));

    connect(nameinp, SIGNAL (inputRejected()), this, SLOT (spaceRejected()));

}

void NewWorkspace::backClicked(){
    if(slide==0)
        this->close();
    else {
        slide--;
    }
}

//steps:
//1. name
//2. version
//3. description
//4. pack image

void NewWorkspace::nextClicked(){
    if(slide==0){
        name = nameinp->text();

        prompt -> setText(name);

        slide++;
    }
//    if(slide==5){
//        emit newWorkspace(name,version,description,packpng);
//    }
}

void NewWorkspace::spaceRejected(){
    this->close();
}
