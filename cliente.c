/* envia
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#define PORT 5001

char* getNombreArchivo(){
    char buffer[512], *p;
    strcpy(buffer,"$HOME/temp/udp/README.md");
    //lo abro y si no existe marco error
    int i;
    p=buffer;
    for (i = strlen(buffer); i >=0 ; --i){
        if(buffer[i]=='/')
            break;
        
    }
    p=p+i+1;
    return p;

}

void enviarArchivo(int socketUDP, struct sockaddr_in serv_cliente){
    char nombre[255];
    int n,e,f;
    struct Encabezado{
          char nombre[255];
          off_t size;
      }encabezado;  
    int long_cliente=sizeof(serv_cliente);
    strcpy(encabezado.nombre,"/home/alumno/gustavo/udp/README.md");
    //
    int fd = open(encabezado.nombre,O_RDONLY,0777);
     if( fd<0 ) {
    perror("no se pudo abrir el archivo.\n");
    exit(1);
  }
  encabezado.size = lseek(fd,0,SEEK_END);
  printf("tamanio archivo: %d\n", encabezado.size);
  f=sendto(socketUDP,&encabezado, sizeof(struct Encabezado),
     0, (struct sockaddr *)&serv_cliente, long_cliente);
    if (f<0){ perror("error en sendto desde servidor a cliente\n");
        exit(1); }

  lseek(fd,0,SEEK_SET);
  while( (e = read(fd,nombre,254)) != 0 ){ 
    nombre[e]='\0'; 
    f=sendto(socketUDP,nombre, sizeof(nombre),
     0, (struct sockaddr *)&serv_cliente, long_cliente);
    if (f<0){
        perror("error en sendto desde servidor a cliente\n");
        exit(1);
    }     
  }

   close(fd); // cierro el archivo O_RDONLY


}

//espera argv[1] --> 127.0.0.1
int main(int argc, char *argv[])
{
    
    /*
    Abrir un socket con socket()
    Asociar el socket a un puerto con bind()
    Enviar mensaje al servidor con sendto()
    Leer respuesta con recvfrom()
    */
    struct sockaddr_in Direccion, serv_cliente;
    int Descriptor;

    /* Se abre el socket UDP (DataGRAM) */
    Descriptor = socket (AF_INET, SOCK_DGRAM, 0);
    if (Descriptor == -1)
    {
        return -1;
    }

    /* Se rellena la estructura de datos necesaria para hacer el bind() */
    Direccion.sin_family = AF_INET;            /* Socket inet */
    Direccion.sin_addr.s_addr = INADDR_ANY;    /* Cualquier dirección IP */
    Direccion.sin_port = htons(0);                    /* Dejamos que linux eliga el servicio */

    /* Se hace el bind() */
    if (bind (
            Descriptor, 
            (struct sockaddr *)&Direccion, 
            sizeof (Direccion)) == -1)
    {
        close (Descriptor);
        return -1;
    }

    //sendto
    char buffer[1024];
    strcpy(buffer,"desde el cliente\n");
    serv_cliente.sin_family = AF_INET;
    serv_cliente.sin_port = htons(5002);// Aquí debemos dar el puerto del servidor
    // Aquí debemos dar la dirección IP del servidor, en formato de red. 
    struct hostent *Maquina;
    Maquina = gethostbyname ("127.0.0.1"); //localhost
    serv_cliente.sin_addr.s_addr = ((struct in_addr *)(Maquina->h_addr))->s_addr;
    int long_cliente=sizeof(serv_cliente);
    
    char *p;
    p=getNombreArchivo();
    printf("nombre archivo: [%s]\n", (char*)p);
    strcpy(buffer,(char*)p);

    /*int e=sendto(Descriptor,buffer, sizeof(buffer),
     0, (struct sockaddr *)&serv_cliente, long_cliente);
    if (e<0){
        printf("error en sendto desde servidor a cliente\n");
        exit(1);
    }
    */
    enviarArchivo(Descriptor, serv_cliente);
    return 0;
}
