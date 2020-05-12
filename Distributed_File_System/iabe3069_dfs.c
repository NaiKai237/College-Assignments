#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(char *msg) {
  perror(msg);
  exit(1);
}

int check_auth(char name[],char pass[]){
	FILE *f;
	char *tmp;
	size_t tmp_size=100;
	char *sub;
	char temp[100];
	size_t nread;
	tmp=(char*)malloc(tmp_size*sizeof(char));
	sub=strtok(name,"\n");
	strcpy(name,sub);
	sub=strtok(pass,"\n");
	strcpy(pass,sub);
	if((f=fopen("dfs.conf","r"))==0){
		return 0;
	}
	while(getline(&tmp,&tmp_size,f)>0){
		strcpy(temp,tmp);
		sub=strtok(temp," ");
		if(strcmp(name,sub)==0){
			sub=strtok(NULL,"\n");
			if(strcmp(pass,sub)==0){
				return 1;
			}
		}
	}
	return 0;
}

int main(int argc, char const *argv[])
{
	int sockfd; /* socket */
	int socknew; /* new socket fd*/
  	int portno; /* port to listen on */
  	int optval; /* flag value for setsockopt */
  	int n; /* message byte size */
  	char buf[1024]; /* message buf */
	char temp[1024];
	char *sub;

	char *f1;
	char *f2;
	char f1_s[100];
	char f2_s[100];
	char f1_name[100];
	char f2_name[100];
	char dir[100];


	char filename[100];
	char crnt[100];
	char *tmp=NULL;
	size_t tmp_size=0;
	long int size;
	int first_num;
	int second_num;

	char cmd[100];

	FILE *pp;
	FILE *fp;


	char uname[100];
	int res;

	struct sockaddr_in serveraddr; /* client's addr */
  	struct sockaddr_in clientaddr; /* client addr */
  	int serverlen=sizeof(serveraddr); /* byte size of client's address */



	//check argc for 3 arguments
	//argv[1] is server directorys
	//argv[2] is portno
	if(argc!=3){
		fprintf(stderr,"usage: %s <server dir> <portnumber>",argv[0]);
	}

	portno = atoi(argv[2]);
	strcpy(dir,".");
	strcat(dir,argv[1]);
	strcat(dir,"/");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("ERROR opening socket");

	optval=1;
	setsockopt(sockfd,SOL_SOCKET, (SO_REUSEADDR & SO_REUSEPORT),&optval,sizeof(optval));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)portno);

	if(0>(bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr))))
		error("bind failed");

	while(1){
		listen(sockfd,3);
		if(0>(socknew=accept(sockfd,(struct sockaddr *)&serveraddr,(socklen_t*)&serverlen))){
			error("accept error");
		}
		if(fork()==0){
			close(sockfd);
			bzero(buf,1024);
			n=read(socknew,buf,1024);
			strcpy(temp,buf);
			sub=strtok(temp," ");
			//put file into server
			if(0==(strcmp("put",sub))){
				//name of file 1
				sub=strtok(NULL," ");
				strcpy(f1_name,dir);
				strcat(f1_name,sub);
				//size of file 1
				sub=strtok(NULL," ");
				strcpy(f1_s,sub);
				//name of file 2
				sub=strtok(NULL," ");
				strcpy(f2_name,dir);
				strcat(f2_name,sub);
				//size of file 2
				sub=strtok(NULL," ");
				strcpy(f2_s,sub);

				f1=(char*)malloc(atoi(f1_s)*sizeof(char));
				f2=(char*)malloc(atoi(f2_s)*sizeof(char));

				n=read(socknew,f1,atoi(f1_s));
				n=read(socknew,f2,atoi(f2_s));
				if(0==(fp=fopen(f1_name,"w")))
					error("file failed");
				fprintf(fp, "%s", f1);
				fclose(fp);
				if(0==(fp=fopen(f2_name,"w")))
					error("file failed");
				fprintf(fp, "%s", f2);
				fclose(fp);

				free(f1);
				free(f2);
			}
			//get file from server
			else if(0==(strcmp("get",sub))){
				//get filename from user
				sub=strtok(NULL," ");
				strcpy(filename,sub);
				//check what files the server has in its directory
				
				/*system call needs to clarify what directory to do it in
				 *should be able to use dir variable.
				 *will need another variable for command string
				 */
				
				sprintf(cmd,"ls %s > ls_res.txt",dir);
				system(cmd);
				sprintf(cmd,"ls_res.txt");
				if(0==(fp=fopen(cmd,"r")))
					error("file failed");
				//done using temp and buf
				
				while(getline(&tmp,&tmp_size,fp)!=-1){
					bzero(temp,1024);
					bzero(buf,1024);
					size=0;
					//compare ls with requested name
					
					strcpy(crnt,tmp);
					sub=strtok(crnt,filename);
					//if names match the follwing if statement figure out which part it is
					if(0==strcmp(sub,"1\n")){
						//reconstruct the full filename and path
						
						strcpy(crnt,dir);
						strcat(crnt,filename);
						strcat(crnt,".1");
						//check the size
						if(0==(pp=fopen(crnt,"rb")))
							error("file failed");
						fseek(pp,0,SEEK_END);
						size=ftell(pp)+1;
						fseek(pp,0,SEEK_SET);

						f1=(char*)malloc(size*sizeof(char));
						if(f1==NULL){
							error("malloc failed");
						}
						int j=fread(f1,size,1,pp);
						
						

						sprintf(buf,"1 %ld ",size);
						send(socknew,buf,1024,0);
						send(socknew,f1,size,0);
						free(f1);
						fclose(pp);
					} else if(0==strcmp(sub,"2\n")){
						//reconstruct the full filename and path
						
						strcpy(crnt,dir);
						strcat(crnt,filename);
						strcat(crnt,".2");
						//check the size
						if(0==(pp=fopen(crnt,"rb")))
							error("file failed");
						fseek(pp,0,SEEK_END);
						size=ftell(pp)+1;
						fseek(pp,0,SEEK_SET);

						f1=(char*)malloc(size*sizeof(char));
						if(f1==NULL){
							error("malloc failed");
						}
						int j=fread(f1,size,1,pp);
						
						

						sprintf(buf,"2 %ld ",size);
						send(socknew,buf,1024,0);
						send(socknew,f1,size,0);
						free(f1);
						fclose(pp);
					} else if(0==strcmp(sub,"3\n")){
						//reconstruct the full filename and path
						
						strcpy(crnt,dir);
						strcat(crnt,filename);
						strcat(crnt,".3");
						//check the size
						if(0==(pp=fopen(crnt,"rb")))
							error("file failed");
						fseek(pp,0,SEEK_END);
						size=ftell(pp)+1;
						fseek(pp,0,SEEK_SET);

						f1=(char*)malloc(size*sizeof(char));
						if(f1==NULL){
							error("malloc failed");
						}
						int j=fread(f1,size,1,pp);
						
						

						sprintf(buf,"3 %ld ",size);
						send(socknew,buf,1024,0);
						send(socknew,f1,size,0);
						free(f1);
						fclose(pp);
					} else if(0==strcmp(sub,"4\n")){
						//reconstruct the full filename and path
						
						strcpy(crnt,dir);
						strcat(crnt,filename);
						strcat(crnt,".4");
						//check the size
						if(0==(pp=fopen(crnt,"rb")))
							error("file failed");
						fseek(pp,0,SEEK_END);
						size=ftell(pp)+1;
						fseek(pp,0,SEEK_SET);

						f1=(char*)malloc(size*sizeof(char));
						if(f1==NULL){
							error("malloc failed");
						}
						int j=fread(f1,size,1,pp);
						
						

						sprintf(buf,"4 %ld ",size);
						send(socknew,buf,1024,0);
						send(socknew,f1,size,0);
						free(f1);
						fclose(pp);
					}
					
					
				}
				//free resources used in getline
				fclose(fp);
				free(tmp);
				/*
				 *doesnt have .# at end
				 *strtok through temp again for the part number?
				 *scrap the whole thing and send the part after telling the client the size
				 */
			}
			else if(0==(strcmp("list",sub))){
				// //check user/pass
				// sub=strtok(NULL," ");
				// strcpy(uname,sub);
				// sub=strtok(NULL," ");
				// if(0==(res=check_auth(uname,sub))){
				// 	sprintf(buf,"Invalid Username/Password. Please try again.");
				// 	send(socknew,buf,1024,0);
				// }

				sprintf(cmd,"ls %s > %s/ls_res.txt",dir,dir);
				system(cmd);
				sprintf(cmd,"%s/ls_res.txt",dir);
				fp=fopen(cmd,"r");
				fread(buf,1024,1,fp);
				send(socknew,buf,1024,0);
			}
			//end of child process
			close(socknew);
			exit(EXIT_SUCCESS);
		}
		//parent process
		close(socknew);
	}


	return 0;
}