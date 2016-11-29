#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <SDL.h>

#include "stream_common.h"
#include "oggstream.h"


int main(int argc, char *argv[]) {
    int res;
    void * status;
	pthread_t readertheora, readervorbis;
	

    if (argc != 2) {
	fprintf(stderr, "Usage: %s FILE", argv[0]);
	exit(EXIT_FAILURE);
    }
    assert(argc == 2);


    // Initialisation de la SDL
    res = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS);
    atexit(SDL_Quit);
    assert(res == 0);
    
    // start the two stream readers

    pthread_create(&readertheora, NULL, theoraStreamReader, argv[1]);
	pthread_create(&readervorbis, NULL, vorbisStreamReader, argv[1]);
 

    // wait audio thread
    
    pthread_join(readervorbis, &status);
    
    // 1 seconde de garde pour le son,
    sleep(1);

    // tuer les deux threads videos si ils sont bloqués

    
    // attendre les 2 threads videos
    pthread_join(readervorbis);
    pthread_join(readertheora);
    
    exit(EXIT_SUCCESS);    
}
