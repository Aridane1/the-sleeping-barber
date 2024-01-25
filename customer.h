#ifndef CUSTOMER_H:;
#define CUSTOMER_H

#include <QThread>
#include <QSemaphore>

class Customer : public QThread
{
public:
    explicit Customer(QObject *parent = nullptr);
    explicit Customer(const int id,
                      const int contador,
                      const int chairsFree,
                      QSemaphore *clients,
                      QSemaphore *chairAccessible,
                      QSemaphore *barberReady,
                      QSemaphore *consoleAvailable,
                      QList<int> bufferEnEspera);

private:
    int id;
    int contador;
    int chairsFree;
    QSemaphore *clients;
    QSemaphore *chairAccessible;
    QSemaphore *barberReady;
    QSemaphore *consoleAvailable;
    QList<int> bufferEnEspera;

protected:
    void run() override;
};

#endif // CUSTOMER_H
