#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NB_BUS_X 5
#define NB_BUS_Y 4
#define NB_VOYAGES 10

sem_t sem_mutex;

int nb_bus_tunnel = 0;     // Nombre de bus dans le tunnel
int sens_tunnel = -1;      // -1 = libre, 0 = X->Y, 1 = Y->X

int file_attente_X = 0;
int file_attente_Y = 0;

void delai_passage() {
    usleep((rand() % 500 + 1000) * 1000);
}

void acces_tunnel(int direction) {
    sem_wait(&sem_mutex);

    if (direction == 0)
        file_attente_X++;
    else
        file_attente_Y++;

    while ((nb_bus_tunnel > 0 && sens_tunnel != direction) ||
           (direction == 0 && file_attente_Y > 0 && sens_tunnel == -1) ||
           (direction == 1 && file_attente_X > 0 && sens_tunnel == -1)) {
        sem_post(&sem_mutex);
        usleep(100000);
        sem_wait(&sem_mutex);
    }

    if (direction == 0)
        file_attente_X--;
    else
        file_attente_Y--;

    nb_bus_tunnel++;
    sens_tunnel = direction;

    sem_post(&sem_mutex);
}

void sortie_tunnel() {
    sem_wait(&sem_mutex);
    nb_bus_tunnel--;
    if (nb_bus_tunnel == 0) {
        sens_tunnel = -1;
    }
    sem_post(&sem_mutex);
}

void* comportement_bus(void* data) {
    int id = ((int*)data)[0];
    int origine = ((int*)data)[1];
    free(data);

    for (int i = 1; i <= NB_VOYAGES; i++) {
        acces_tunnel(origine == 0 ? 0 : 1);
        printf("Bus %d de Ville %c : %c → %c (Aller %d)\n",
               id, origine == 0 ? 'X' : 'Y',
               origine == 0 ? 'X' : 'Y',
               origine == 0 ? 'Y' : 'X', i);
        delai_passage();
        sortie_tunnel();

        acces_tunnel(origine == 0 ? 1 : 0);
        printf("Bus %d de Ville %c : %c → %c (Retour %d)\n",
               id, origine == 0 ? 'X' : 'Y',
               origine == 0 ? 'Y' : 'X',
               origine == 0 ? 'X' : 'Y', i);
        delai_passage();
        sortie_tunnel();
    }

    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t liste_bus[NB_BUS_X + NB_BUS_Y];

    sem_init(&sem_mutex, 0, 1);

    int index = 0;

    for (int i = 0; i < NB_BUS_X; i++) {
        int* infos = malloc(2 * sizeof(int));
        infos[0] = i + 1;
        infos[1] = 0;
        pthread_create(&liste_bus[index++], NULL, comportement_bus, infos);
    }

    for (int i = 0; i < NB_BUS_Y; i++) {
        int* infos = malloc(2 * sizeof(int));
        infos[0] = i + 1;
        infos[1] = 1;
        pthread_create(&liste_bus[index++], NULL, comportement_bus, infos);
    }

    for (int i = 0; i < NB_BUS_X + NB_BUS_Y; i++) {
        pthread_join(liste_bus[i], NULL);
    }

    sem_destroy(&sem_mutex);

    return 0;
}
