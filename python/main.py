import multiprocessing as mp
import time
import random

N = 5 #Número de filósofos
MAX_SECONDS = 30

palillos = [mp.Semaphore(1) for i in range(N)] #Semaforos para los palillos
locks = [mp.Lock() for i in range(N) ] #locks para evitar bloqueos

"""
Función de los Filosofos
"""
def Filosofo(id, palillos, locks, espera):
    while True:
        #Pensar()
        print(f"Filósos {id+1} está pensando.")
        time.sleep(random.uniform(1,3))

        #Tomar el palillo izquierdo TomarPalillo()
        locks[id].acquire()
        espera[id] = True
        palillos[id].acquire()
        espera[id] = False
        print(f"Filósofo {id+1} ha tomado el palillo izquierdo.")

        #Tomar el palillo derecho TomarPalillo()
        locks[(id+1)%N].acquire()
        while(espera[(id+1)%N]):
            locks[(id+1)%N].release()
            time.sleep(0.1)
            locks[(id+1)%N].acquire()
        palillos[(id + 1) % N].acquire()
        print(f"Filósofo {id+1} ha tomado el palillo izquierdo.")

        #Comer()
        print(f"Filósofo {id+1} está comiendo")
        time.sleep(random.uniform(1,3))

        #DejarPalillo() -> Izquierdo
        palillos[id].release()
        print(f"Filósodo {id+1} ha soltado el palillo izquierdo.")
        locks[id].release()

        #DejarPalillo() -> Izquierdo
        palillos[(id+1)%N].release()
        locks[(id+1)%N].release()
        print(f"Filósodo {id+1} ha soltado el palillo derecho.")

if __name__ == '__main__':
    #Creando los procesos para los filosofos
    espera = mp.Array('b', [False]*N) #variable para espera de los palillos
    filosofos = [mp.Process(target=Filosofo, args=(i, palillos, locks, espera)) for i in range(N)]

    for f in filosofos:
        f.start()
    
    #esperando un numero maximo de segundos
    time.sleep(MAX_SECONDS)

    #deteniendo los procesos de los filosofos

    for f in filosofos:
        f.terminate()