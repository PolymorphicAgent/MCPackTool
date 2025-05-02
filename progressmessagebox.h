#ifndef PROGRESSMESSAGEBOX_H
#define PROGRESSMESSAGEBOX_H

#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QProgressBar>
#include <QQuickWidget>
#include <QSvgRenderer>
#include <QStyleHints>
#include <QHBoxLayout>
#include <QQmlEngine>
#include <QPainter>
#include <QPixmap>
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QMovie>

class ProgressMessageBox : public QDialog
{
public:
    ProgressMessageBox(const QString& title, const QStringList& barLabels);
    // Sets the progress (0–100) of bar at index i
    void setProgress(int index, int value);
    static QPixmap loadSvgIcon(const QString&);

private:
    QVector<QProgressBar*> m_bars;
    QVector<QQuickWidget*> m_spinners;
    QVector<QLabel*> m_checks;

    QPixmap m_checkPixmap;

    QVBoxLayout*           m_layout;
};

#endif // PROGRESSMESSAGEBOX_H
