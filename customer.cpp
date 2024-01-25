#include "customer.h"
#include <iostream>
#include <ostream>

Customer::Customer(QObject *parent)
    : QThread{parent}
{}

Customer::Customer(const int id,
                   const int contador,
                   const int chairsFree,
                   QSemaphore *clients,
                   QSemaphore *chairAccessible,
                   QSemaphore *barberReady,
                   QSemaphore *consoleAvailable,
                   QList<int> bufferEnEspera)
{
    this->id=id;
    this->contador=contador;
    this->chairsFree=chairsFree;
    this->clients=clients;
    this->chairAccessible=chairAccessible;
    this->barberReady=barberReady;
    this->consoleAvailable=consoleAvailable;
    this->bufferEnEspera=bufferEnEspera;
}


void Customer::run()
{
    chairAccessible->acquire();
    if (chairsFree > 0) {
        chairsFree -= 1;
        consoleAvailable->acquire();
        bufferEnEspera.append(id);
        std::string str;
        for (int var = 0; var < bufferEnEspera.size(); ++var) {
            str += std::to_string(static_cast<unsigned char>(bufferEnEspera[var]));
            str += ' ';
        }
        std::cout << "Se sienta el cliente " << id << " En espera: " << str << std::endl;
        consoleAvailable->release();
        clients->release();
        chairAccessible->release();
        barberReady->acquire();
        consoleAvailable->release();
    } else {
        chairAccessible->release();
        consoleAvailable->acquire();
        std::cout << "El establecimiento está lleno" << std::endl;
        contador++;
        consoleAvailable->release();
    }
}
