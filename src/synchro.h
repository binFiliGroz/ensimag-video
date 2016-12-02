#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <stdbool.h>
#include "ensitheora.h"

/* Les extern des variables pour la synchro ici */

extern pthread_t theora2sdlthread;

extern bool fini;
extern pthread_mutex_t acces_hashmap;

/* Fonctions de synchro à implanter */
void initSynchro();

void envoiTailleFenetre(th_ycbcr_buffer buffer);
void attendreTailleFenetre();

void attendreFenetreTexture();
void signalerFenetreEtTexturePrete();

void debutConsommerTexture();
void finConsommerTexture();

void debutDeposerTexture();
void finDeposerTexture();

#endif
