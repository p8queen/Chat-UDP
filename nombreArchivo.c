#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* enviarArchivo(){
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

int main(int argc, char const *argv[])
{
	char cadena[512], *p;
	p=enviarArchivo();
	printf("[%s]\n", (char*)p);
	//strcpy(cadena,p*)
		
	return 0;
}