#include "synchro.h"
#include "ensitheora.h"
#include <pthread.h>


bool fini;
unsigned int nTextures;

/* les variables pour la synchro, ici */

pthread_mutex_t acces_hashmap;

pthread_mutex_t TailleFenetre_m;
pthread_cond_t TailleFenetre_cond;

pthread_mutex_t Fenetre_Texture_m;
pthread_cond_t Fenetre_Texture_cond;

pthread_mutex_t Info_Texture_m;
pthread_cond_t plein_cond, vide_cond;

/* l'implantation des fonctions de synchro ici */

void initSynchro() {
    fini = false;
    nTextures = 0;

    pthread_mutex_init(&acces_hashmap, 0);

    pthread_mutex_init(&TailleFenetre_m, 0);
    pthread_cond_init(&TailleFenetre_cond, 0);

    pthread_mutex_init(&Fenetre_Texture_m, 0);
    pthread_cond_init(&Fenetre_Texture_cond, 0);

    pthread_mutex_init(&Info_Texture_m, 0);
    pthread_cond_init(&plein_cond, 0);
    pthread_cond_init(&vide_cond, 0);
}

void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    pthread_mutex_lock(&TailleFenetre_m);

    windowsy = buffer->height;
    windowsx = buffer->width;
    pthread_cond_signal(&TailleFenetre_cond);

    pthread_mutex_unlock(&TailleFenetre_m);
}

void attendreTailleFenetre() {
    pthread_mutex_lock(&TailleFenetre_m);

    pthread_cond_wait(&TailleFenetre_cond, &TailleFenetre_m);

    pthread_mutex_unlock(&TailleFenetre_m);
}

void signalerFenetreEtTexturePrete() {
    pthread_mutex_lock(&Fenetre_Texture_m);

    pthread_cond_signal(&Fenetre_Texture_cond);

    pthread_mutex_unlock(&Fenetre_Texture_m);
}

void attendreFenetreTexture() {
    pthread_mutex_lock(&Fenetre_Texture_m);

    pthread_cond_wait(&Fenetre_Texture_cond, &Fenetre_Texture_m);

    pthread_mutex_unlock(&Fenetre_Texture_m);
}

/** 
  Producteur - Consommateur

  */

void debutConsommerTexture() {
    pthread_mutex_lock(&Info_Texture_m);

    if (nTextures == 0)
	pthread_cond_wait(&plein_cond, &Info_Texture_m);

    //pthread_mutex_unlock(&Info_Texture_m);
}

void finConsommerTexture() {
    //pthread_mutex_lock(&Info_Texture_m);

    nTextures--;
    pthread_cond_signal(&plein_cond);

    pthread_mutex_unlock(&Info_Texture_m);
}

void debutDeposerTexture() {
    pthread_mutex_lock(&Info_Texture_m);

    if (nTextures == NBTEX)
	pthread_cond_wait(&plein_cond, &Info_Texture_m);

    //pthread_mutex_unlock(&Info_Texture_m);
}

void finDeposerTexture() {
    //pthread_mutex_lock(&Info_Texture_m);

    nTextures++;
    pthread_cond_signal(&vide_cond);

    pthread_mutex_unlock(&Info_Texture_m);
}
