#include "guithreadslotter.h"

GUIThreadSlotter::GUIThreadSlotter(QWidget *parent)
    : QWidget{parent}
{}

void GUIThreadSlotter::conflictPrompter(const QString& p1nameR, const QString& p2nameR, MCResourcePackElement* p1, MCResourcePackElement* p2)
{
    pro = new ConflictPrompt(p1nameR, p2nameR, p1, p2);
    pro->exec();
    emit conflictResult(pro->result());
}

void GUIThreadSlotter::resetDialog(){
    pro->flush();
}
