#include "synchro.h"
#include "ensitheora.h"
#include <pthread.h>


bool fini;

/* les variables pour la synchro, ici */

pthread_mutex_t acces_hashmap;


pthread_mutex_t TailleFenetre_m, ConsommerTexture_m;

/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    windowsx = buffer->height;
    windowsy = buffer->width;
    pthread_mutex_unlock(&TailleFenetre_m);
}

void attendreTailleFenetre() {
    pthread_mutex_lock(&TailleFenetre_m);
}

void signalerFenetreEtTexturePrete() {

}

void attendreFenetreTexture() {
}

void debutConsommerTexture() {
    pthread_mutex_lock(&ConsommerTexture_m);
}

void finConsommerTexture() {
    pthread_mutex_unlock(&ConsommerTexture_m);
}


void debutDeposerTexture() {
}

void finDeposerTexture() {
}
