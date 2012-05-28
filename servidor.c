#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void* hiloUDPServer(void *arg){
    //int sd = (int)arg; //socket
    // primer llamado a socket
    int sdudp = socket(AF_INET, SOCK_DGRAM, 0);
    if (sdudp < 0) {
        perror("ERROR abriendo socket");
        exit(1); }
    /* Inicializa estructura */
    //bzero((char *) &serv_addr, sizeof(serv_addr));
    struct sockaddr_in serv_udp, serv_cliente;
    int portno = 5002;
    serv_udp.sin_family = AF_INET;
    serv_udp.sin_addr.s_addr = INADDR_ANY;
    serv_udp.sin_port = htons(portno);
     
    if (bind(sdudp, (struct sockaddr *) &serv_udp,
                          sizeof(serv_udp)) < 0) {
         perror("ERROR bind udp\n"); exit(1); }

    /* Espera conecciones    */
    listen(sdudp,5); //5 es canidad máxima
    char buffer[1024]; //puede ser (char *)&estructura
    int long_cliente=sizeof(serv_cliente);
    int e=recvfrom(sdudp,buffer,sizeof(buffer),0,
        (struct sockaddr *) &serv_cliente,&long_cliente);
    if (e<0){ perror("error en recvfrom\n");exit(1);}

    printf("cliente>%s\n",buffer );    
    //enviar respuesta a cliente
    strcpy(buffer,"servidor a cliente\n"); 

    e=sendto(sdudp,buffer, sizeof(buffer),
     0, (struct sockaddr *)&serv_cliente, long_cliente);
    if (e<0){ 
        printf("error en sendto desde servidor a cliente\n");
        exit(1); }
    return NULL;
}

int crearHiloUDP(){
    pthread_t id;
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0){
        perror("error init hilo"); exit(1); }
    if(pthread_create(&id, &attr,hiloUDPServer,NULL) != 0){
        perror("ERROR create hilo");exit(1); }
        return id
}

//recibe cant de conecciones por parametro
int main( int argc, char *argv[] )
{
    
    //lo de abajo ava en un hilo
    int hudp = crearHiloUDP();

    //aquí va como sigue, con TCP/IP
    
    //espero finalizacion hilos
    pthread_join(h1,NULL);
    

    return 0; 
}
