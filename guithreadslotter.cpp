#include "guithreadslotter.h"
#include "conflictprompt.h"

GUIThreadSlotter::GUIThreadSlotter(QWidget *parent)
    : QWidget{parent}
{}

void GUIThreadSlotter::conflictPrompter(const QString& p1nameR, const QString& p1assetR, const QString& p1imageR,
                             const QString& p2nameR, const QString& p2assetR, const QString& p2imageR)
{
    ConflictPrompt *pro = new ConflictPrompt(p1nameR, p1assetR, p1imageR, p2nameR, p2assetR, p2imageR);
    pro->exec();
    emit conflictResult(pro->result());
}
