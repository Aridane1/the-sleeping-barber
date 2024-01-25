#include "barber.h"
#include <iostream>
#include <ostream>

Barber::Barber(QObject *parent)
    : QThread{parent}
{}

Barber::Barber(const int contador,
               const int chairsFree,
               const int ClientSize,
               QSemaphore *clients,
               QSemaphore *chairAccessible,
               QSemaphore *barberReady,
               QSemaphore *consoleAvailable,
               QList<int> bufferEnEspera)
{
    this->contador=contador;
    this->chairsFree=chairsFree;
    this->clients=clients;
    this->chairAccessible=chairAccessible;
    this->barberReady=barberReady;
    this->consoleAvailable=consoleAvailable;
    this->bufferEnEspera=bufferEnEspera;
}

void Barber::run()
{
    while (contador < ClientSize) {
        if (clients->available() == 0) {
            consoleAvailable->acquire();
            std::cout << "El barbero se duerme" << std::endl;
            consoleAvailable->release();
        }
        clients->acquire();
        chairAccessible->acquire();
        chairsFree += 1;
        consoleAvailable->acquire();
        int clientId = bufferEnEspera.first();
        std::cout << "Corta el pelo al cliente: " << clientId << std::endl;
        bufferEnEspera.removeOne(clientId);
        contador++;
        consoleAvailable->release();
        barberReady->release();
        chairAccessible->release();

    }
}
