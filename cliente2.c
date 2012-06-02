/* recibe
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>

void* hiloUDPServer(void *arg){
    //int sd = (int)arg; //socket
    // primer llamado a socket
    int sdudp = socket(AF_INET, SOCK_DGRAM, 0);
    if (sdudp < 0) {
        perror("ERROR abriendo socket");
        exit(1); }
    /* Inicializa estructura */
    //bzero((char *) &serv_addr, sizeof(serv_addr));
    struct sockaddr_in serv_udp, serv_cliente1;
    int portno = 5002;
    serv_udp.sin_family = AF_INET;
    serv_udp.sin_addr.s_addr = INADDR_ANY;
    serv_udp.sin_port = htons(portno);
     
    if (bind(sdudp, (struct sockaddr *) &serv_udp,
                          sizeof(serv_udp)) < 0) {
         perror("ERROR bind udp\n"); exit(1); }

    /* Espera conecciones    */
    //listen(sdudp,5); //5 es canidad máxima
    char buffer[1024]; //puede ser (char *)&estructura
    int long_cliente=sizeof(serv_cliente1);
    //get nombre archivo y size
    struct Encabezado{
          char nombre[255];
          off_t size;
      }encabezado;  
    int e=recvfrom(sdudp,&encabezado,sizeof(struct Encabezado),0,
            (struct sockaddr *) &serv_cliente1,&long_cliente);
        if (e<0){ perror("error en recvfrom\n");exit(1);}
        //crear archivo
    strcat(encabezado.nombre,".copy");
    int fd = open(encabezado.nombre,O_CREAT | O_WRONLY, 0777); // NOTA: si hago un read va a dar error porque estamos abiendolo como O_WRONLY
    if( fd<0 ) { perror("no se pudo abrir el archivo.\n");exit(1); }
    printf("comienzo de transferencia\n");
    int acumulador=0;
    printf("encabezado.size: %d\n",encabezado.size );
    while(acumulador < encabezado.size){
        e=recvfrom(sdudp,buffer,sizeof(buffer),0,
            (struct sockaddr *) &serv_cliente1,&long_cliente);
        if (e<0){ perror("error en recvfrom\n");exit(1);}
        write(fd,buffer,strlen(buffer));
        acumulador = acumulador + e;
        printf("%d\n",acumulador);
        
    } 
    close(fd);   
    printf("fin de transferencia\n");
    return NULL;
}

int crearHiloUDP(){
    pthread_t id;
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0){
        perror("error init hilo"); exit(1); }
    if(pthread_create(&id, &attr,hiloUDPServer,NULL) != 0){
        perror("ERROR create hilo");exit(1); }
        return id;
}

//recibe cant de conecciones por parametro
int main( int argc, char *argv[] )
{
    
    //lo de abajo ava en un hilo
    int hudp = crearHiloUDP();

    //aquí va como sigue, con TCP/IP
    
    //espero finalizacion hilos
    pthread_join(hudp,NULL);
    

    return 0; 
}
