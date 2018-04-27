#include "caballos.h"

typedef struct _Mensaje{
	long id; /*Campo obligatorio a long que identifica el tipo de mensaje*/
	int tirada; /*Informacion a transmitir en el mensaje*/
}Mensaje;

int aleat_num(int inf, int sup){
  if (inf>sup) return -1;
  int randm,rango,grupo,limite;
  rango=sup-inf+1;
  grupo=RAND_MAX/rango;
  limite=grupo*rango;
  do{
    randm=rand();
  }while(randm>=limite);
  return (randm/grupo)+inf;
}

int dado(int modo){
  if (modo == GANADORA){
    return aleat_num(1,7);
  }else if (modo == REMONTADORA){
    return aleat_num(1,6) + aleat_num(1,6);
  }
  return aleat_num(1,6);

  int avanzar_caballo(int posicion){
    if(posicion == PRIMERO){
      return dado(GANADORA);
    }else if(posicion == ULTIMO){
      return dado(REMONTADORA);
    }
    return dado(NORMAL);
  }

  void manejador(int sig){
    return;
  }
  void terminar(int sig){
    exit(EXIT_SUCCESS);
  }
  void caballo(int pipe[2]){
    char buffer[MAXBUFFER];
    int posicion, tirada, clave, msqid;
    Mensaje mensaje;
    if(signal(SIGUSR1,manejador) == SIG_ERR){
      printf("Error en la señal\n");
    }
    if (signal(SIGUSR2,terminar) == SIG_ERR){
      printf("Error segundo manejador\n");
    }
    
    clave = ftok (FILEKEY, KEY);
    if (clave==(key_t) -1){
      printf("Error al coger el key de la cola de mensajes");
    }
    msqid = msgget(clave,0600);
    if (msqid == -1){
      printf("Error al coger el key de la cola de mensajes");
    }
    while(1){
      pause();
      close(pipe[1]);
      read(pipe[0],buffer,sizeof(buffer));
      posicion = atoi(buffer);
      tirada = avanzar_caballo(posicion);
      mensaje.id=0;
      mensaje.tirada=tirada;
      msgsnd(msqid,(struct msgbuf *) &mensaje,sizeof(Mensaje) - sizeof(long),IPC_NOWAIT);
    }
  }
}