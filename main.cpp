#include <QCoreApplication>
#include <QSemaphore>
#include <iostream>
#include <QThread>

constexpr int ClientSize = 200;
int chairsFree = 5;
int contador = 0;

QSemaphore clients;
QSemaphore chairAccessible(1);
QSemaphore barberReady(0);

QSemaphore consoleAvailable(1);
QList<int> bufferEnEspera;


class Barber : public QThread
{
public:
    void run() override
    {
    while (contador < ClientSize) {
            if (clients.available() == 0) {
              consoleAvailable.acquire();
              std::cout << "El barbero se duerme" << std::endl;
              consoleAvailable.release();
            }
            clients.acquire();
            chairAccessible.acquire();
            chairsFree += 1;
            consoleAvailable.acquire();
            int clientId = bufferEnEspera.first();
            std::cout << "Corta el pelo al cliente: " << clientId << std::endl;
            bufferEnEspera.removeOne(clientId);
            contador++;
            consoleAvailable.release();
            barberReady.release();
            chairAccessible.release();

        }
    }
};

class Customer : public QThread
{
public:
    Customer(const int id) {
        this->id=id;
    }

    void run() override
    {
        chairAccessible.acquire();
        if (chairsFree > 0) {
            chairsFree -= 1;
            consoleAvailable.acquire();
            bufferEnEspera.append(id);
            std::string str;
            for (int var = 0; var < bufferEnEspera.size(); ++var) {
                str += std::to_string(static_cast<unsigned char>(bufferEnEspera[var]));
                str += ' ';
            }
            std::cout << "Se sienta el cliente " << id << " En espera: " << str << std::endl;
            consoleAvailable.release();
            clients.release();
            chairAccessible.release();
            barberReady.acquire();
            consoleAvailable.release();
        } else {
            chairAccessible.release();
            consoleAvailable.acquire();
            std::cout << "El establecimiento está lleno" << std::endl;
            contador++;
            consoleAvailable.release();
        }
    }

private:
    int id;
};

int main(int argc, char *argv[])
{
    std::cout << "Starting..." << std::endl;
    QCoreApplication a(argc, argv);

    Barber barber;
    Customer *customer[ClientSize];

    barber.start();

    for (int i = 0; i < ClientSize; ++i) {
        customer[i] =  new Customer(i+1);
        customer[i]->start();
    }

    barber.wait();

    for (int i = 0; i < ClientSize; ++i) {
        customer[i]->wait();
    }

    return 0;
}
