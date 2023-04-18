#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <Windows.h>

using namespace std;

const int N = 5; // número de filósofos
const int MAX_SECONDS = 30; // número máximo de segundos que se ejecutará el programa

mutex printMutex;

vector<mutex> palillos(N); // mutexes para los tenedores

void Filosofo(int id)
{
    while (true) 
    {
        // Pensar()
        {
            lock_guard<mutex> lock(printMutex);
            cout << "Filosofo " << id + 1 << " esta pensando" << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(rand() % 1000 + 500)); // simulando la acción de pensar
        
        palillos[id].lock();
        // tomando el tenedor izquierdo
        {
            lock_guard<mutex> lock(printMutex);
            cout << "Filosofo " << id + 1 << " ha tomado el tenedor izquierdo." << endl;
        }
        

        // tomando el tenedor derecho
        palillos[(id + 1) % N].lock();
        {
            lock_guard<mutex> lock(printMutex);
            cout << "Filosofo " << id + 1 << " ha tomado el tenedor derecho." << endl;
        }
        // comiendo
        {
            lock_guard<mutex> lock(printMutex);
            cout << "Filosofo " << id + 1 << " esta comiendo." << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(rand() % 1000 + 500)); // simulando la acción de comer

        // soltando el tenedor izquierdo
        palillos[id].unlock();
        {
            lock_guard<mutex> lock(printMutex);
            cout << "Filosofo " << id + 1 << " ha soltado el tenedor izquierdo." << endl;
        }

        // soltando el tenedor derecho
        palillos[(id + 1) % N].unlock();
        {
            lock_guard<mutex> lock(printMutex);
            cout << "Filosofo " << id + 1 << " ha soltado el tenedor derecho." << endl;
        }
    }
}

int main()
{
    // semilla para la generación de números aleatorios
    srand(time(NULL));

    // creando los hilos para los filósofos
    vector<thread> Filosofos(N);
    for (int i = 0; i < N; i++) {
        Filosofos[i] = thread(Filosofo, i);
    }

    // esperando un número máximo de segundos
    Sleep(MAX_SECONDS * 1000);

    // deteniendo los hilos de los filósofos
    for (int i = 0; i < N; i++) {
        Filosofos[i].detach();
    }

    return 0;
}