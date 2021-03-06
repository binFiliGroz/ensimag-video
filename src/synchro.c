#include "synchro.h"
#include "ensitheora.h"
#include <pthread.h>

pthread_t theora2sdlthread;

bool fini;
pthread_mutex_t acces_hashmap;

/* les variables pour la synchro, ici */

bool tailleFenetreEnvoyee;
pthread_mutex_t TailleFenetre_m;
pthread_cond_t TailleFenetre_cond;

bool fenetrePrete;
pthread_mutex_t FenetreTexture_m;
pthread_cond_t FenetreTexture_cond;
pthread_cond_t FenetreTextureReception_cond;

unsigned int nTextures;
pthread_mutex_t Info_Texture_m;
pthread_cond_t plein_cond, vide_cond;

/* l'implantation des fonctions de synchro ici */

void initSynchro() {
    fini = false;

    pthread_mutex_init(&acces_hashmap, 0);

    tailleFenetreEnvoyee = false;
    pthread_mutex_init(&TailleFenetre_m, 0);
    pthread_cond_init(&TailleFenetre_cond, 0);

    fenetrePrete = 0;
    pthread_mutex_init(&FenetreTexture_m, 0);
    pthread_cond_init(&FenetreTexture_cond, 0);

    nTextures = 0;
    pthread_mutex_init(&Info_Texture_m, 0);
    pthread_cond_init(&plein_cond, 0);
    pthread_cond_init(&vide_cond, 0);
}

void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    pthread_mutex_lock(&TailleFenetre_m);

    windowsy = buffer->height;
    windowsx = buffer->width;
    tailleFenetreEnvoyee = true;
    pthread_cond_signal(&TailleFenetre_cond);

    pthread_mutex_unlock(&TailleFenetre_m);
}

void attendreTailleFenetre() {
    pthread_mutex_lock(&TailleFenetre_m);

    if (!tailleFenetreEnvoyee)
        pthread_cond_wait(&TailleFenetre_cond, &TailleFenetre_m);

    tailleFenetreEnvoyee = false;

    pthread_mutex_unlock(&TailleFenetre_m);
}

void signalerFenetreEtTexturePrete() {
    pthread_mutex_lock(&FenetreTexture_m);

    fenetrePrete = true;
    pthread_cond_signal(&FenetreTexture_cond);

    pthread_mutex_unlock(&FenetreTexture_m);
}

void attendreFenetreTexture() {
    pthread_mutex_lock(&FenetreTexture_m);

    if (!fenetrePrete)
        pthread_cond_wait(&FenetreTexture_cond, &FenetreTexture_m);
    fenetrePrete = false;

    pthread_mutex_unlock(&FenetreTexture_m);
}

/** 
  Producteur - Consommateur

  */

void debutConsommerTexture() {
    pthread_mutex_lock(&Info_Texture_m);

    if (nTextures == 0)
	    pthread_cond_wait(&vide_cond, &Info_Texture_m);

    pthread_mutex_unlock(&Info_Texture_m);
}

void finConsommerTexture() {
    pthread_mutex_lock(&Info_Texture_m);

    nTextures--;
    pthread_cond_signal(&plein_cond);

    signalerFenetreEtTexturePrete();

    pthread_mutex_unlock(&Info_Texture_m);
}

void debutDeposerTexture() {
    pthread_mutex_lock(&Info_Texture_m);

    if (nTextures == NBTEX)
        pthread_cond_wait(&plein_cond, &Info_Texture_m);

    pthread_mutex_unlock(&Info_Texture_m);
}

void finDeposerTexture() {
    pthread_mutex_lock(&Info_Texture_m);

    nTextures++;

    pthread_mutex_unlock(&Info_Texture_m);
}
