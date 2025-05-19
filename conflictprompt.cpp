#include "conflictprompt.h"
#include "modelpreviewwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

ConflictPrompt::ConflictPrompt(const QString& p1nameR, const QString& p2nameR, MCResourcePackElement* e1R, MCResourcePackElement* e2R)
{

    this->p1name  =   p1nameR;
    this->p2name  =   p2nameR;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *title = new QLabel("Conflict Detected: Choose one asset.");
    title->setFont(QFont("Bahnschrift", 16, QFont::Bold));
    title->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(title);

    QHBoxLayout *contentLayout = new QHBoxLayout();

    // Left
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QLabel *leftLabel = new QLabel("Pack: " + p1name);
    leftLabel->setAlignment(Qt::AlignHCenter);
    selL = new QCheckBox("Choose this");
    selL->setChecked(true);
    leftPreview = new ModelPreviewWidget(e1R);
    leftLayout->addWidget(leftLabel);
    leftLayout->addWidget(leftPreview, 1);
    leftLayout->addWidget(selL);
    contentLayout->addLayout(leftLayout);

    // Right
    QVBoxLayout *rightLayout = new QVBoxLayout();
    QLabel *rightLabel = new QLabel("Pack: " + p2name);
    rightLabel->setAlignment(Qt::AlignHCenter);
    selR = new QCheckBox("Choose this");
    rightPreview = new ModelPreviewWidget(e2R);
    rightLayout->addWidget(rightLabel);
    rightLayout->addWidget(rightPreview, 1);
    rightLayout->addWidget(selR);
    contentLayout->addLayout(rightLayout);

    mainLayout->addLayout(contentLayout);

    // Buttons
    okButton = new QPushButton("Apply");
    cancelButton = new QPushButton("Cancel");
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->setAlignment(Qt::AlignHCenter);
    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, &QPushButton::clicked, this, [this](){
        this->choice = -1;
        QDialog::accept();
    });
    connect(selL, &QCheckBox::clicked, this, [this](bool checked) {
        if (checked) selR->setChecked(false);
    });
    connect(selR, &QCheckBox::clicked, this, [this](bool checked) {
        if (checked) selL->setChecked(false);
    });

    //start OpenGL
    leftPreview->update();
    rightPreview->update();

}

ConflictPrompt::~ConflictPrompt(){
    flush();
}

int ConflictPrompt::result(){return this->choice;}

void ConflictPrompt::updateR(bool checked){
    if(checked && selL->isChecked())selL->setChecked(false);
}

void ConflictPrompt::updateL(bool checked){
    if(checked && selR->isChecked())selR->setChecked(false);
}

void ConflictPrompt::accept(){
    // qDebug() << "Preview size:" << leftPreview->size();

    if(selL->isChecked())choice = 1;
    else if(selR->isChecked())choice = 2;
    else choice = -1;

    QDialog::accept();
}

void ConflictPrompt::flush(){
    rightPreview->cleanupResources();
    leftPreview->cleanupResources();
}
