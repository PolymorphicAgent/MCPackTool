#ifndef MCRESOURCEPACK_H
#define MCRESOURCEPACK_H

#include <QObject>

class MCResourcePack : public QObject
{
    Q_OBJECT
public:
    explicit MCResourcePack(QObject *parent = nullptr);

signals:
};

#endif // MCRESOURCEPACK_H
