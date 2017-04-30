/* Urbain Nathan & Quentin Wendlig
 * L3S6 CMI Image
 * Projet 2 IHM
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractItemModel>

class Client : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = 0);

private:
};

#endif // CLIENT_H
