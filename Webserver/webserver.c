//Ian Bell




#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFSIZE 8192


/*
 * error - wrapper for perror
 */
void error(char *msg) {
	printf("%s\n",msg);
  perror(msg);
  exit(1);
}

int main(int argc, char **argv){
	char temp[BUFSIZE];
	char buf[BUFSIZE];
	char *sub;
	char method[100];
	char URL[100];
	char version[100]={0};
	char header[100];
	FILE *fp;
	size_t nread;
	long int size;
	
	
	int sockfd; /* socket */
	int socknew; //child socket
	int portno; /* port to listen on */
	struct sockaddr_in serveraddr; /* server's addr */
    int servlen=sizeof(serveraddr); /* byte size of servers address */
    

    int optval; /* flag value for setsockopt */
    int n; /* message byte size */
	
	
	if(argc!=2){
		fprintf(stderr,"usage %s <portnumber>\n",argv[0]);
		return -1;
	}
		

	/*
	get info from socket
	should connect to port number from argv
	*/
	portno = atoi(argv[1]);

  /* 
   * socket: create the parent socket 
   */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
      error("ERROR opening socket");
	


  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, (SO_REUSEADDR & SO_REUSEPORT), 
	       /*(const void *)*/&optval , sizeof(optval));

  /*
   * build the server's Internet address
   */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);
	


  /* 
   * bind: associate the parent socket with a port 
   */
    if (bind(sockfd, (struct sockaddr *) &serveraddr, 
	     sizeof(serveraddr)) < 0) 
      error("ERROR on binding");
	
	printf("Waiting on request\n");    
	//isten for connections
	if(listen(sockfd,100) < 0){
		error("listen error");
	}
	//loop to keep server running
	while(1){
		printf("\r\n\r\nWaiting on request\r\n\r\n");	
		//accept connection
		if((socknew = accept(sockfd,NULL,NULL)) < 0){
			error("accept error");
		}
		
		/*
		 *child process to handle request
		 */
		if(!fork()){		
			close(sockfd);
			
			//recieve message from client
			if((n = recv(socknew,temp,sizeof(temp),0))<0){
				error("recv");
			}
			printf("\nrecieved:\n%s",temp);
			/*
			Isolate the substring
			*/
			sub=strtok(temp," ");
			strcpy(method,sub);
			
			sub=strtok(NULL," ");
			strcpy(URL,sub);
			
			sub=strtok(NULL,"\r\n");
			strcpy(version,sub);
			/*
			 *Get Method
			 */
			if(strstr(method,"GET")){
				
				/*
				 *check if file exhists
				 */
				char type[100];
				//if no page specified default to index.html
				//modify this
				if((strcmp(URL,"/"))==0){
					strcpy(type,".html");
					strcpy(URL,"./www/index.html");
				} else {
					sub=strstr(URL,".");
					strcpy(type,sub);
				
					strcpy(temp,"./www");
					strcat(temp,URL);
					strcpy(URL,temp);
				}
				
				if(access(URL,F_OK)!=-1){

					/*
					 *check filesize
					 */
					if((fp=fopen(URL,"r"))<0){
						error("fileopen");
					}
					fseek(fp,0L,SEEK_END);
					size = ftell(fp);
					fseek(fp,0L,SEEK_SET);
					
					/*
					 *create header 
					 */
				
					
					sprintf(header,"%s 200 ok\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n",version,type,size);	
					
					/*
					 *read file line by line
					 *check each line for other objects to grab
					 */	
					
					
					bzero(buf,BUFSIZE);
					fread(buf,size,1,fp);
					
					fclose(fp);
					//memcpy(temp,header,sizeof(header));
					//memcpy(temp+sizeof(header),buf,sizeof(buf));
					//memcpy(buf,temp,sizeof(temp));
					if((strcmp(type,".html"))==0){
						strcpy(temp,header);
						strcat(temp,buf);
						strcpy(buf,temp);
					} else {
						memcpy(temp,header,sizeof(header));
						memcpy(temp+sizeof(header),buf,sizeof(buf));
						memcpy(buf,temp,sizeof(temp));
					}
					
					printf("Sending:\n%s\n",buf);
					
					int prog = 0;
					n=0;
					while(prog < size){
						n=send(socknew,buf+prog,sizeof(buf),0);
						prog=prog+n;
					}
					printf("response sent\n");
				} else{
					//file not found
					strcpy(header,"");
					strcpy(header,version);
					strcat(header," 404 Not Found\r\n\r\n (my 404 error)\r\n\r\n");
					strcpy(buf,header);
					printf("Page not found\n");
					
					
					send(socknew,buf,BUFSIZE,0);
				}
			} else {
				//client tried to user POST or HEAD command
				printf("\n\n!!!!\nNot GET method\n!!!!\n\n");
				strcpy(buf,"501 Not Implemented\r\n\r\nServer only processes Get commands\r\n");
				send(socknew,buf,sizeof(buf),0);

			}
		close(socknew);
		printf("Connection closed\nTerminating Child\n");
		exit(EXIT_SUCCESS);	
		}//end of child fork 
		close(socknew);
		printf("closing socketnew\n");
	}//end of while loop
	close(sockfd);
	return 0;
}


