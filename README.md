# Projet-Se2
Projet Systéme d'informations 02
Groupe : Laoudi Houda G4, Mebrouk Maria G4, Belfegroune Maroua Hibetellah G6 , Zaidi Hocine G2
Langage : C
Multitâche : pthread
Synchronisation : semaphore (sem_t)
Sommeil : usleep() pour simuler le temps de trajet


- Détails de l'implémentation:
*Chaque bus est modélisé par un thread.
*Le tunnel est géré par un sémaphore mutex (sem_mutex) assurant l’exclusion mutuelle sur les ressources partagées :
*nb_bus_tunnel : nombre de bus dans le tunnel
*sens_tunnel : direction actuelle du tunnel (-1, 0 ou 1)
*Deux files d’attente :file_attente_X et file_attente_Y permettent de :
  *Donner la priorité à l'autre sens si une file est vide.
  *Assurer l'équité .
*Le passage dans le tunnel est géré par acces_tunnel() et sortie_tunnel().
