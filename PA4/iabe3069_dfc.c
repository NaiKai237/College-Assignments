#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(char *msg);

struct Node{
	char name[100];
	struct Node* next;
};

void printlist(struct Node *n){
	char rname[100];
	char t[100];
	char num[100];
	char *s;
	int p1=0;
	int p2=0;
	int p3=0;
	int p4=0;
	while(n!=NULL){
		strcpy(rname,n->name);

		s=strtok(rname,".");
		s=strtok(NULL,".");
		strcpy(t,".");
		strcat(t,s);
		s=strtok(NULL,".");
		strcpy(num,s);
		strcat(rname,t);
		if(0==strcmp("1",num)){
			p1=1;
		} 
		else if(0==strcmp("2",num)){
			p2=1;
		} 
		else if(0==strcmp("3",num)){
			p3=1;
		} 
		else if (0==strcmp("4",num)){
			p4=1;
		}
		n=n->next;
	}
	if((p1==1)&(p2==1)&(p3==1)&(p4==1))
		printf("%s\n", rname);
	else
		printf("%s[incomplete]\n", rname);
}

//should push the new node to the front of the list passed in from arr
struct Node *addlist(char *name,struct Node **n){
	struct Node * new_node = (struct Node*)malloc(sizeof(struct Node));
	strcpy(new_node->name,name);
	new_node->next=*n;
	return new_node;
}
int generate_id(char _name[100]){
    int total=0;
    int size=strlen(_name);
    for(int i=0;i<=size;i++){
        total=total+_name[i];
    }
    return total%100;
}

int main(int argc, char const *argv[])
{
	
	int sockfd; /* socket */
  	int portno; /* port to listen on */
  	int optval; /* flag value for setsockopt */
  	int n; /* message byte size */
  	char *buf; /* message buf */ //start at 1024, change after telling server what size we're gonna send
  	buf=(char*)malloc(1024*sizeof(char));

  	char *p1;
  	char *p2;
  	char *p3;
  	char *p4;

  	FILE *fp;
  	size_t nread;
  	char input[100];//input buffer
  	char *in;
  	char *sub;
  	char temp[100];//temp buffer
  	char ser[100];
  	char port[100];
  	char filename[100];
  	
  	char *tmp;//temp buffer for getline
  	size_t tmp_size=100;
  	tmp=(char*)malloc(tmp_size*sizeof(char));
  	
  	int partnum;
  	int partsize;

  	struct Node *arr[100];
  	int hash=-1;


  	if(argc!=2){
  		fprintf(stderr,"usage: %s <conifiguration file>\n",argv[0]);
  		exit(0);
  	}


  	/*
  	 *Initialize server addr and portno based on dfc.conf
  	 */

  	if(NULL==(fp=fopen(argv[1],"r")))
  		error("file failed to open");
  	if(0==(nread=getline(&tmp,&tmp_size,fp)))
  		error("getline error");

  	strcpy(temp,tmp);
  	sub=strtok(temp," ");
  	sub=strtok(NULL," ");
  	sub=strtok(NULL,":");
  	strcpy(ser,sub);
  	sub=strtok(NULL,"");
  	strcpy(port,sub);
  	portno=atoi(port);
  	
  	struct sockaddr_in serv1; 
  	serv1.sin_family = AF_INET;
	serv1.sin_port=htons(portno);
	inet_pton(AF_INET, ser,&serv1.sin_addr);


	if(0==(nread=getline(&tmp,&tmp_size,fp)))
  		error("getline error");

  	strcpy(temp,tmp);
  	sub=strtok(temp," ");
  	sub=strtok(NULL," ");
  	sub=strtok(NULL,":");
  	strcpy(ser,sub);
  	sub=strtok(NULL,"");
  	strcpy(port,sub);
  	portno=atoi(port);
  	
  	struct sockaddr_in serv2;
  	serv2.sin_family = AF_INET;
	serv2.sin_port=htons(portno);
	inet_pton(AF_INET, ser,&serv2.sin_addr);


	if(0==(nread=getline(&tmp,&tmp_size,fp)))
  		error("getline error");

  	strcpy(temp,tmp);
  	sub=strtok(temp," ");
  	sub=strtok(NULL," ");
  	sub=strtok(NULL,":");
  	strcpy(ser,sub);
  	sub=strtok(NULL,"");
  	strcpy(port,sub);
  	portno=atoi(port);

	struct sockaddr_in serv3;
  	serv3.sin_family = AF_INET;
	serv3.sin_port=htons(portno);
	inet_pton(AF_INET, ser,&serv3.sin_addr);

	if(0==(nread=getline(&tmp,&tmp_size,fp)))
  		error("getline error");

  	strcpy(temp,tmp);
  	sub=strtok(temp," ");
  	sub=strtok(NULL," ");
  	sub=strtok(NULL,":");
  	strcpy(ser,sub);
  	sub=strtok(NULL,"");
  	strcpy(port,sub);
  	portno=atoi(port);

	struct sockaddr_in serv4;
  	serv4.sin_family = AF_INET;
	serv4.sin_port=htons(portno);
	inet_pton(AF_INET, ser,&serv4.sin_addr);


	/*
	 *get username and pass from dfc.conf
	 */

	char uname[100];
	char pass[100];


	nread=getline(&tmp,&tmp_size,fp);
	sub=strtok(tmp," ");
	sub=strtok(NULL,"\n");
	strcpy(uname,sub);

	nread=getline(&tmp,&tmp_size,fp);
	sub=strtok(tmp," ");
	sub=strtok(NULL,"\n");
	strcpy(pass,sub);


	fclose(fp);
	free(tmp);
	strcat(uname,"");
	strcat(pass,"");


	//get commands from user
	while(1){
	loop:
	bzero(temp,1024);
	bzero(buf,1024);
		printf("Enter command: ");
		fgets(input,100,stdin);
		in=input;
		if(0==strcmp("exit\n",input)){
			printf("Exiting client program\n");
			break;
		}
		else if(0==strcmp("list\n",input)){
			printf("perform list\n");
			sprintf(buf,"list");
			char red[1024];
			char *lsres=red;
			char fullname[100];
			char rootname[100];
			char type[100];
			char *lssub;
			char *sub;

			for (int i=0; i < 100;++i)
  				arr[i]=NULL;

			/*
			 *contact each server for their ls results
			 *read through each name
			 *hash the root of the name
			 *add whole name to arr through Node structure
			 *printlist should check if each element of arr has 4 nodes attached
			 */

			//server1
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if(sockfd < 0)
				goto s1clean;

			optval=1;
			setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
  			if(0>(connect(sockfd, (struct sockaddr *)&serv1,sizeof(serv1))))
  				goto s1clean;
  			//list command to server
  			send(sockfd,buf,1024,0);
  			read(sockfd,red,1024);
  			while((lssub=strtok_r(lsres,"\n",&lsres))>0){
				strcpy(fullname,lssub);
				//isolate the rootname and the fullname from the given string
				//rootname determines hash value
				//fullname is needed to create the linked list node
				strcpy(rootname,fullname);
				sub=strtok(rootname,".");
				sub=strtok(NULL,".");
				strcpy(type,".");
				strcat(type,sub);
				strcat(rootname,type);
				if(strcmp(fullname,"ls_res.txt")!=0){
					//create hash of the rootname of the file
					int hash=generate_id(rootname);
					//create and add node to array of linked lists
					arr[hash]=addlist(fullname,&(arr[hash]));
				}
			}
  			
  		s1clean:
  			close(sockfd);

  			//server2
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if(sockfd < 0)
				goto s2clean;

			optval=1;
			setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
  			if(0>(connect(sockfd, (struct sockaddr *)&serv2,sizeof(serv2))))
  				goto s2clean;
  			//list command to server
  			send(sockfd,buf,1024,0);
  			
  			lsres=red;
  			while((lssub=strtok_r(lsres,"\n",&lsres))>0){
				strcpy(fullname,lssub);
				//isolate the rootname and the fullname from the given string
				//rootname determines hash value
				//fullname is needed to create the linked list node
				strcpy(rootname,fullname);
				sub=strtok(rootname,".");
				sub=strtok(NULL,".");
				strcpy(type,".");
				strcat(type,sub);
				strcat(rootname,type);
				if(strcmp(fullname,"ls_res.txt")!=0){
					//create hash of the rootname of the file
					int hash=generate_id(rootname);
					//create and add node to array of linked lists
					arr[hash]=addlist(fullname,&(arr[hash]));
				}
			}
  			
  		s2clean:
  			close(sockfd);



  			//server3
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if(sockfd < 0)
				goto s3clean;

			optval=1;
			setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
  			if(0>(connect(sockfd, (struct sockaddr *)&serv3,sizeof(serv3))))
  				goto s3clean;
  			//list command to server
  			send(sockfd,buf,1024,0);
  			read(sockfd,red,1024);
  			
  			lsres=red;
  			while((lssub=strtok_r(lsres,"\n",&lsres))>0){
				strcpy(fullname,lssub);
				//isolate the rootname and the fullname from the given string
				//rootname determines hash value
				//fullname is needed to create the linked list node
				strcpy(rootname,fullname);
				sub=strtok(rootname,".");
				sub=strtok(NULL,".");
				strcpy(type,".");
				strcat(type,sub);
				strcat(rootname,type);
				if(strcmp(fullname,"ls_res.txt")!=0){
					//create hash of the rootname of the file
					int hash=generate_id(rootname);
					//create and add node to array of linked lists
					arr[hash]=addlist(fullname,&(arr[hash]));
				}
			}
  			
  		s3clean:
  			close(sockfd);


  			//server4
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if(sockfd < 0)
				goto s4clean;

			optval=1;
			setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
  			if(0>(connect(sockfd, (struct sockaddr *)&serv4,sizeof(serv4))))
  				goto s4clean;
  			//list command to server
  			send(sockfd,buf,1024,0);
  			read(sockfd,red,1024);
  			
  			lsres=red;
  			while((lssub=strtok_r(lsres,"\n",&lsres))>0){
				strcpy(fullname,lssub);
				//isolate the rootname and the fullname from the given string
				//rootname determines hash value
				//fullname is needed to create the linked list node
				strcpy(rootname,fullname);
				sub=strtok(rootname,".");
				sub=strtok(NULL,".");
				strcpy(type,".");
				strcat(type,sub);
				strcat(rootname,type);
				if(strcmp(fullname,"ls_res.txt")!=0){
					//create hash of the rootname of the file
					int hash=generate_id(rootname);
					//create and add node to array of linked lists
					arr[hash]=addlist(fullname,&(arr[hash]));
				}
			}
  			
  		s4clean:
  			close(sockfd);

  			//print results
  			for (int i = 0; i < 100; ++i){
				if(arr[i]!=NULL)
					printlist(arr[i]);
			}

		}
		else if(sub=strtok_r(in," ",&in)){
			bzero(temp,1024);
			strcpy(temp,sub);
			//perform get
			//needs to jump after failed connect instead of ending program
			if(0==strcmp("get",temp)){
				printf("getting file\n");
				sub=strtok_r(NULL," ",&in);
				strcpy(filename,sub);
				sub=strtok(filename,"\n");
				strcpy(filename,sub);
				strcat(filename,"");
				//send request to server
				//read server response
				//assemble pieces

				sprintf(buf,"get %s",filename);


				//server 1
				sockfd = socket(AF_INET, SOCK_STREAM, 0);
				if(sockfd < 0)
					goto serv1clean;

				optval=1;
				setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
  				if(0>(connect(sockfd, (struct sockaddr *)&serv1,sizeof(serv1))))
					goto serv1clean;
				//request the filefrom the server
				send(sockfd,buf,1024,0);
				n=read(sockfd,temp,1024);
				
				//server should send part numbers and sizes of parts
				sub = strtok(temp," ");//finds first part num
				partnum=atoi(sub);
				sub = strtok(NULL," ");//finds first size
				partsize=atoi(sub);
				//allocate part size and read file part
				if(partnum==1){
					p1=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p1,partsize);
				} else if(partnum==2){
					p2=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p2,partsize);
				} else if(partnum==3){
					p3=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p3,partsize);
				} else if(partnum==4){
					p4=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p4,partsize);
				}
				//second part from server
				n=read(sockfd,temp,1024);
				//server should send part numbers and sizes of parts
				sub = strtok(temp," ");//finds first part num
				partnum=atoi(sub);
				sub = strtok(NULL," ");//finds first size
				partsize=atoi(sub);
				//allocate part size and read file part
				if(partnum==1){
					p1=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p1,partsize);
				} else if(partnum==2){
					p2=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p2,partsize);
				} else if(partnum==3){
					p3=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p3,partsize);
				} else if(partnum==4){
					p4=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p4,partsize);
				}
			serv1clean:
				close(sockfd);



				//server 2
				sockfd = socket(AF_INET, SOCK_STREAM, 0);
				if(sockfd < 0)
					goto serv2clean;

				optval=1;
				setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
  				if(0>(connect(sockfd, (struct sockaddr *)&serv2,sizeof(serv2))))
					goto serv2clean;
				//request the filefrom the server
				send(sockfd,buf,1024,0);
				n=read(sockfd,temp,1024);
				
				//server should send part numbers and sizes of parts
				sub = strtok(temp," ");//finds first part num
				partnum=atoi(sub);
				sub = strtok(NULL," ");//finds first size
				partsize=atoi(sub);
				//allocate part size and read file part
				if(partnum==1){
					p1=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p1,partsize);
				} else if(partnum==2){
					p2=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p2,partsize);
				} else if(partnum==3){
					p3=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p3,partsize);
				} else if(partnum==4){
					p4=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p4,partsize);
				}
				//second part from server
				n=read(sockfd,temp,1024);
				//server should send part numbers and sizes of parts
				sub = strtok(temp," ");//finds first part num
				partnum=atoi(sub);
				sub = strtok(NULL," ");//finds first size
				partsize=atoi(sub);
				//allocate part size and read file part
				if(partnum==1){
					p1=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p1,partsize);
				} else if(partnum==2){
					p2=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p2,partsize);
				} else if(partnum==3){
					p3=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p3,partsize);
				} else if(partnum==4){
					p4=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p4,partsize);
				}
			serv2clean:
				close(sockfd);


				//server 3
				sockfd = socket(AF_INET, SOCK_STREAM, 0);
				if(sockfd < 0)
					goto serv3clean;

				optval=1;
				setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
  				if(0>(connect(sockfd, (struct sockaddr *)&serv3,sizeof(serv3))))
					goto serv3clean;
				//request the filefrom the server
				send(sockfd,buf,1024,0);
				n=read(sockfd,temp,1024);
				
				//server should send part numbers and sizes of parts
				sub = strtok(temp," ");//finds first part num
				partnum=atoi(sub);
				sub = strtok(NULL," ");//finds first size
				partsize=atoi(sub);
				//allocate part size and read file part
				if(partnum==1){
					p1=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p1,partsize);
				} else if(partnum==2){
					p2=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p2,partsize);
				} else if(partnum==3){
					p3=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p3,partsize);
				} else if(partnum==4){
					p4=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p4,partsize);
				}
				//second part from server
				n=read(sockfd,temp,1024);
				//server should send part numbers and sizes of parts
				sub = strtok(temp," ");//finds first part num
				partnum=atoi(sub);
				sub = strtok(NULL," ");//finds first size
				partsize=atoi(sub);
				//allocate part size and read file part
				if(partnum==1){
					p1=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p1,partsize);
				} else if(partnum==2){
					p2=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p2,partsize);
				} else if(partnum==3){
					p3=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p3,partsize);
				} else if(partnum==4){
					p4=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p4,partsize);
				}
			serv3clean:
				close(sockfd);


				//server 4
				sockfd = socket(AF_INET, SOCK_STREAM, 0);
				if(sockfd < 0)
					goto serv4clean;

				optval=1;
				setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
  				if(0>(connect(sockfd, (struct sockaddr *)&serv4,sizeof(serv4))))
					goto serv4clean;	
				//request the filefrom the server
				send(sockfd,buf,1024,0);
				n=read(sockfd,temp,1024);
				
				//server should send part numbers and sizes of parts
				sub = strtok(temp," ");//finds first part num
				partnum=atoi(sub);
				sub = strtok(NULL," ");//finds first size
				partsize=atoi(sub);
				//allocate part size and read file part
				if(partnum==1){
					p1=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p1,partsize);
				} else if(partnum==2){
					p2=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p2,partsize);
				} else if(partnum==3){
					p3=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p3,partsize);
				} else if(partnum==4){
					p4=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p4,partsize);
				}
				//second part from server
				n=read(sockfd,temp,1024);
				//server should send part numbers and sizes of parts
				sub = strtok(temp," ");//finds first part num
				partnum=atoi(sub);
				sub = strtok(NULL," ");//finds first size
				partsize=atoi(sub);
				//allocate part size and read file part
				if(partnum==1){
					p1=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p1,partsize);
				} else if(partnum==2){
					p2=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p2,partsize);
				} else if(partnum==3){
					p3=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p3,partsize);
				} else if(partnum==4){
					p4=(char*)malloc(partsize*sizeof(char));
					n=read(sockfd,p4,partsize);
				}
			serv4clean:
				close(sockfd);
				//replace with filename instead of sample file
				fp=fopen("samplefile","w");
				if((p1!=NULL)&(p2!=NULL)&(p3!=NULL)&(p4!=NULL)){
					fprintf(fp,"%s%s%s%s",p1,p2,p3,p4);
					printf("file assembled as 'samplefile'\n");
					free(p1);
					free(p2);
					free(p3);
					free(p4);
				} else
					printf("File not available\n");
				fclose(fp);
				
			}
			//perform put
			else if(0==strcmp("put",temp)){
				printf("putting file\n");
				int x=-1;
				int r = -1;
				//perform hash
				//send put message to server with filenames and their sizes.
				//send files to server
				sub=strtok_r(NULL," ",&in);
				strcpy(filename,sub);
				sub=strtok(filename,"\r\n");
				//create md5sum of file
				char cmd[100];
				sprintf(cmd,"md5sum %s > hash.md5",filename);
				system(cmd);
				//read the hash from hash.md5
				tmp=(char*)malloc(tmp_size*sizeof(char));
				if(0==(fp=fopen("hash.md5","r")))
					error("file failed to open");
				if(0==(nread=getline(&tmp,&tmp_size,fp)))
  					error("getline error");
  				fclose(fp);
  				strcpy(temp,tmp);
  				sub=strtok(temp," ");

  				x=atoi(temp)%4;

  				//break file into parts
  				if(0==(fp=fopen(filename,"r")))
  					error("file failed");
  				fseek(fp,0,SEEK_END);
  				long int file_size = ftell(fp);
  				fseek(fp,0,SEEK_SET);

  				r=file_size%4;
  				//part one will be (file_size/4 + r)
  				//other part will just be file_size/4
  				long int ps = file_size/4;
  				p1=(char*)malloc((ps+r)*sizeof(char));
  				p2=(char*)malloc((ps)*sizeof(char));
  				p3=(char*)malloc((ps)*sizeof(char));
  				p4=(char*)malloc(ps*sizeof(char));
  				fread(p1,ps+r,1,fp);
  				fread(p2,ps,1,fp);
  				fread(p3,ps,1,fp);
  				fread(p4,ps,1,fp);
  				fclose(fp);
  				//send parts to appropriate servers
  				//first send message with part names and sizes
  				if(0==x){
  					//server 1
  					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto clean1;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(optval));
  					sprintf(buf,"put %s.1 %ld %s.2 %ld",filename,ps+r,filename,ps);
  					if(0>(connect(sockfd, (struct sockaddr *)&serv1,sizeof(serv1))))
						goto clean1;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p1,ps+r,0);
					send(sockfd,p2,ps,0);
				clean1:
					close(sockfd);

					//server 2
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto clean2;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
					sprintf(buf,"put %s.2 %ld %s.3 %ld",filename,ps,filename,ps);
					if(0>(connect(sockfd, (struct sockaddr *)&serv2,sizeof(serv2))))
						goto clean2;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p2,ps,0);
					send(sockfd,p3,ps,0);
				clean2:
					close(sockfd);

					//server 3
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto clean3;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
					sprintf(buf,"put %s.3 %ld %s.4 %ld",filename,ps,filename,ps);
					if(0>(connect(sockfd, (struct sockaddr *)&serv3,sizeof(serv3))))
						goto clean3;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p3,ps,0);
					send(sockfd,p4,ps,0);
				clean3:
					close(sockfd);
					//server 4
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto clean4;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
					sprintf(buf,"put %s.4 %ld %s.1 %ld",filename,ps,filename,ps+r);
					if(0>(connect(sockfd, (struct sockaddr *)&serv4,sizeof(serv4))))
						goto clean4;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p4,ps,0);
					send(sockfd,p1,ps+r,0);
				clean4:
					close(sockfd);

  				}
  				else if(1==x){
  					//server 1
  					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto x11c;
  					sprintf(buf,"put %s.4 %ld %s.1 %ld",filename,ps,filename,ps+r);
  					if(0>(connect(sockfd, (struct sockaddr *)&serv1,sizeof(serv1))))
						goto x11c;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p4,ps,0);
					send(sockfd,p1,ps+r,0);
				x11c:
					close(sockfd);

					//server 2
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto x12c;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
					sprintf(buf,"put %s.1 %ld %s.2 %ld",filename,ps+r,filename,ps);
					if(0>(connect(sockfd, (struct sockaddr *)&serv2,sizeof(serv2))))
						goto x12c;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p1,ps+r,0);
					send(sockfd,p2,ps,0);
				x12c:
					close(sockfd);
					//server 3
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto x13c;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
					sprintf(buf,"put %s.2 %ld %s.3 %ld",filename,ps,filename,ps);
					if(0>(connect(sockfd, (struct sockaddr *)&serv3,sizeof(serv3))))
						goto x13c;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p2,ps,0);
					send(sockfd,p3,ps,0);
				x13c:
					close(sockfd);
					//server 4
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto x14c;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
					sprintf(buf,"put %s.3 %ld %s.4 %ld",filename,ps,filename,ps);
					if(0>(connect(sockfd, (struct sockaddr *)&serv4,sizeof(serv4))))
						goto x14c;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p3,ps,0);
					send(sockfd,p4,ps,0);
				x14c:
					close(sockfd);
  				}
  				else if(2==x){
  					//server 1
  					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto g2x1c;
  					sprintf(buf,"put %s.3 %ld %s.4 %ld",filename,ps,filename,ps);
  					if(0>(connect(sockfd, (struct sockaddr *)&serv1,sizeof(serv1))))
						goto g2x1c;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p3,ps,0);
					send(sockfd,p4,ps,0);
				g2x1c:
					close(sockfd);

					//server 2
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto g2x2c;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
					sprintf(buf,"put %s.4 %ld %s.1 %ld",filename,ps,filename,ps+r);
					if(0>(connect(sockfd, (struct sockaddr *)&serv2,sizeof(serv2))))
						goto g2x2c;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p4,ps,0);
					send(sockfd,p1,ps+r,0);
				g2x2c:
					close(sockfd);
					//server 3
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto g2x3c;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
					sprintf(buf,"put %s.1 %ld %s.2 %ld",filename,ps+r,filename,ps);
					if(0>(connect(sockfd, (struct sockaddr *)&serv3,sizeof(serv3))))
						goto g2x3c;
					send(sockfd,buf,1024,0);
					
					
					send(sockfd,p1,ps+r,0);
					send(sockfd,p2,ps,0);
				g2x3c:
					close(sockfd);
					//server 4
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto g2x4c;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
					sprintf(buf,"put %s.2 %ld %s.3 %ld",filename,ps,filename,ps);
					if(0>(connect(sockfd, (struct sockaddr *)&serv4,sizeof(serv4))))
						goto g2x4c;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p2,ps,0);
					send(sockfd,p3,ps,0);
				g2x4c:
					close(sockfd);
  				}
  				else if(3==x){
  					//server 1
  					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto g3x1c;
  					sprintf(buf,"put %s.2 %ld %s.3 %ld",filename,ps,filename,ps);
  					if(0>(connect(sockfd, (struct sockaddr *)&serv1,sizeof(serv1))))
						goto g3x1c;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p2,ps,0);
					send(sockfd,p3,ps,0);
				g3x1c:
					close(sockfd);

					//server 2
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto g3x2c;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
					sprintf(buf,"put %s.3 %ld %s.4 %ld",filename,ps,filename,ps);
					if(0>(connect(sockfd, (struct sockaddr *)&serv2,sizeof(serv2))))
						goto g3x2c;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p3,ps,0);
					send(sockfd,p4,ps,0);
				g3x2c:
					close(sockfd);
					//server 3
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto g3x3c;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
					sprintf(buf,"put %s.4 %ld %s.1 %ld",filename,ps,filename,ps+r);
					if(0>(connect(sockfd, (struct sockaddr *)&serv3,sizeof(serv3))))
						goto g3x3c;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p4,ps,0);
					send(sockfd,p1,ps+r,0);
				g3x3c:
					close(sockfd);
					//server 4
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					if(sockfd < 0)
						goto g3x4c;

					optval=1;
					setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,sizeof(int));
					sprintf(buf,"put %s.1 %ld %s.2 %ld",filename,ps+r,filename,ps);
					if(0>(connect(sockfd, (struct sockaddr *)&serv4,sizeof(serv4))))
						goto g3x4c;
					send(sockfd,buf,1024,0);
					
					send(sockfd,p1,ps+r,0);
					send(sockfd,p2,ps,0);
				g3x4c:
					close(sockfd);
  				} else
  					printf("hash error\n");
  				free(p1);
  				free(p2);
  				free(p3);
  				free(p4);
  				free(tmp);
			}
		}
	}
	free(buf);
	return 0;
}

void error(char *msg) {
  perror(msg);
  exit(1);
}