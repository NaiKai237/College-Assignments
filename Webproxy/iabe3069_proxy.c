/*
 * Ian Bell
 * PA3
 */

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

#include <signal.h>
#include <curl/curl.h>
#include <sys/mman.h>


#define BUFSIZE 1024

//https://stackoverflow.com/questions/4217037/catch-ctrl-c-in-c/4217052
int running=1;
void intHandler(int dummy){
	signal(dummy,SIG_IGN);
	printf("\nShuting down proxy...");
	running=0;
	exit(0);
}

void error(char *msg) {
  perror(msg);
  exit(1);
}
int black_list(char _name[100],char *list);
void curl_request(FILE *cfp, char name[100],CURLM *easyhandle);
int generate_id(char _name[]);
int ceck_cache(struct tm _cache[],int _ttl,int _id);
void save_to_cache(struct tm _cache[],int _id,char data[]);
int main(int argc, char const *argv[])
{

    if(argc<2){
        fprintf(stderr,"ussage %s <portnumber> <timeout>\n",argv[0]);
        return -1;
    }

    int sockfd; /* socket */
    int socknew; //child socket
    int portno; /* port to listen on */
    struct sockaddr_in proxyaddr; /* proxy's addr */
    int proxylen=sizeof(proxyaddr); /* byte size of servers address */
    int optval; /* flag value for setsockopt */
    int n; /* message byte size */
    char buf[BUFSIZE];
    struct tm *cache;
    //https://stackoverflow.com/questions/13274786/how-to-share-memory-between-process-fork
    cache = mmap(NULL, sizeof(cache), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    struct in_addr ip_addr;

    CURLM *easyhandle; /* handle for curl api */
    FILE *cfp;

    char *sub;
    char buff[BUFSIZE];
    char method[1024];
    char URL[1024];
    char name[1024];

    char filename[100];
    long length = 0;

    char *temp;
    char *temp2;
    char header[]="HTTP/1.0 200 OK\r\nContent Type: html\r\n";
    printf("header: %s\n", header);
    

    int TTL;
    if(argc==3){
        TTL=atoi(argv[2]);
    } else {
        TTL=60;
    }

    curl_global_init(CURL_GLOBAL_ALL);

    /*
     *create blacklist array
     */

    if(NULL==(cfp=fopen("blacklist.txt","r"))){
        error("file failed to open");
    }

    fseek(cfp,0,SEEK_END);
    long l = ftell(cfp)+1;
    fseek(cfp,0,SEEK_SET);
    
    char blacklist[l];
    bzero(blacklist,l);

    fread(blacklist,1,l,cfp);
    printf("blacklist \n%s\n",blacklist);
    fclose(cfp);


    /*
     *establish TCP soccket
     */

    portno=atoi(argv[1]);

    sockfd=socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
    if (sockfd==-1){
        error("Error opening socket");
    }

    optval =1;
    setsockopt(sockfd,SOL_SOCKET, (SO_REUSEADDR & SO_REUSEPORT),&optval,sizeof(optval));

    proxyaddr.sin_family = AF_INET;
    proxyaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    proxyaddr.sin_port = htons((unsigned short)portno);

    if(bind(sockfd, (struct sockaddr *)&proxyaddr, sizeof(proxyaddr))<0){
        error("ERROR on binding");
    }

    printf("proxy running\nlistening for connections...\n");

    signal(SIGINT,intHandler);
    while(running){
        length=0;
        bzero(buf,BUFSIZE);
        bzero(method,1024);
        bzero(URL,1024);
        bzero(name,1024);
        //listen on socket
        printf("...\n");
        fflush(stdout);
        if(listen(sockfd,1)<0){
            error("listen error");
        }


    //accept connection and fork child
        //parent process goes to the end then loops
        //child handles the rest of program
        if((socknew = accept(sockfd,(struct sockaddr *)&proxyaddr,(socklen_t*)&proxylen))<0){
            error("accept error");
        }
        if(fork()==0){
            //child proccess
        
            printf("connection accepted\n");
            close(sockfd);
            sockfd = -1;
            bzero(buf,BUFSIZE);
            if((n=recv(socknew,buf,BUFSIZE,0))<0){
                error("recv error");
            }
            printf("recieved:\n%s\n",buf);

        //check if valid connection
            //only supports GET
            //check if the url exists gethostbyname()
            bzero(buff,BUFSIZE);
            strcpy(buff,buf);
            //find method in buff
            sub=strtok(buff," ");
            
            bzero(method,1024);
            strcpy(method,sub);
            printf("Method: %s\n",method);
            if(strstr(method,"GET")==NULL){
                bzero(buf,BUFSIZE);
                strcpy(buf,"405 Method Not Allowed\r\n\r\nProxy only handles GET commands\r\n");
                printf("sending...\n");
                if(-1==send(socknew,buf,sizeof(buf),0)){
                    error("sending error");
                }
                exit(EXIT_SUCCESS);
            }

            //if valid request parse for:
            //URL is next token in buff
            sub=strtok(NULL," ");
            bzero(URL,1024);
            strcpy(URL,sub);
            bzero(name,1024);
            
            char _url[1024];
            strcpy(_url,URL);
            sub=strtok(_url,":");
            //check if http or https
            if(0==strcmp("http",_url)){
                //name starts at 7
                bzero(_url,1024);
                strcpy(_url,URL+7);
                sub=strtok(_url,".");
                //check if www was included in the URL
                if(0==strcmp(_url,"www")){
                    bzero(_url,1024);
                    strcpy(_url,URL+7);
                    sub=strtok(_url,"/");
                    strcpy(name,sub);
                } else {
                    bzero(_url,1024);
                    strcpy(_url,URL+7);
                    sub=strtok(_url,"/");
                    strcpy(name,"www.");
                    strcat(name,sub);
                    strcpy(URL,"http://");
                    strcat(URL,name);
                }
            } else {
                //name starts at 8
                //https
                bzero(_url,1024);
                strcpy(_url,URL+8);
                sub=strtok(_url,".");
                //check if www was included in the URL
                if(0==strcmp(_url,"www")){
                    bzero(_url,1024);
                    strcpy(_url,URL+8);
                    sub=strtok(_url,"/");
                    strcpy(name,sub);
                } else {
                    bzero(_url,1024);
                    strcpy(_url,URL+8);
                    sub=strtok(_url,"/");
                    strcpy(name,"www.");
                    strcat(name,sub);
                    bzero(URL,1024);
                    strcpy(URL,"https://");
                    strcat(URL,name);
                }
            }

            printf("URL: %s\n",URL);
            printf("Name: %s\n",name);

            struct hostent *serveraddr;
            serveraddr= gethostbyname(name);
            if(serveraddr==NULL){
                //send error message back to client
                /*
                 *remove error below
                 *send 400 bad request message to client
                 */
                bzero(buf,BUFSIZE);
                strcpy(buf,"400 bad request\r\n\r\n");
                printf("sending1...\n");
                if(-1==send(socknew,buf,sizeof(buf),0)){
                    error("sending error");
                }
                close(socknew);
                error("gethostbyname error");

            }

            int b=black_list(name,blacklist);
            if(b==1){
            	printf("blacklisted site\n");
            	bzero(buf,BUFSIZE);
            	strcpy(buf,"ERROR 403 Forbidden\r\n\r\n");
                printf("sending2...\n");
            	if(-1==send(socknew,buf,sizeof(buf),0)){
                    error("sending error");
                }
            	close(socknew);
            	exit(EXIT_SUCCESS);
            }
            ip_addr= *(struct in_addr *) (serveraddr->h_addr);
            printf("server addr: %s\n",inet_ntoa(ip_addr));


        /*
         * Caching stuff
         */
        
            //check if page is in cache
            int hash = generate_id(name);
            int res;
            res= ceck_cache(cache,TTL,hash);
            //file is in cache
            if(res==1){
            	printf("using file from cache\n");
                
                bzero(filename,100);
                sprintf(filename,"./cache/%d",hash);
                //reading from the file to a string buffer
                if(NULL==(cfp=fopen(filename,"r"))){
                    printf("file failed to open\n");
                    error("file failed to open");
                }
                printf("cache file opened\n");
                fseek(cfp,0,SEEK_END);
                length = ftell(cfp)+1;
                fseek(cfp,0,SEEK_SET);
    
                temp = (char *)malloc(length*sizeof(char));
                fread(temp,1,length,cfp);
                temp2 = (char *)malloc(strlen(temp)+strlen(header));
                strcpy(temp2,header);
                strcat(temp2,temp);
                //send to client
                printf("sending3...\n%s\n",temp2);
                if(-1==send(socknew,temp2,strlen(temp2),0)){
                    error("sending error");
                }
                close(socknew);
                fclose(cfp);
                free(temp);
                free(temp2);
                exit(EXIT_SUCCESS);
                
                //close socknew
                //exit child process
            }
    
        //forward request to server
            //construct new header
            //libcurl stuff
            //http://www.curl.haxx.se/libcurl/c/sepheaders.html
            curl_request(cfp,URL,easyhandle);

            //reading from the file to a string buffer
            if(NULL==(cfp=fopen("body.out","r"))){
            error("file failed to open");
            }

            fseek(cfp,0,SEEK_END);
            length = ftell(cfp)+1;
            fseek(cfp,0,SEEK_SET);
            printf("body file length: %ld\n\n",length);     
            temp = (char *)malloc(length*sizeof(char));
            fread(temp,1,length,cfp);
            
            save_to_cache(cache,hash,temp);

            //attach 200 ok header to front of webpage to send
            temp2 = (char *)malloc(strlen(temp)+strlen(header));
            strcpy(temp2,header);
            strcat(temp2,temp);
            //forward resonse to client
            printf("sending4...\n%s\n",temp2);
            send(socknew,temp2,strlen(temp2),0);

            fclose(cfp);
            close(socknew);
            socknew = -1;
            free(temp);
            free(temp2);
            exit(EXIT_SUCCESS);
            

        } else {
            //parrent process
            close(socknew);
            socknew = -1;
        }
    }
    close(sockfd);
    munmap(cache,sizeof(cache));
    return 0;
}

int black_list(char _name[100],char *list){
	char *sub;//used to iterate through blacklist buffer
	int i = 0;//iterate through req list
	struct hostent *req_addr; //struct for requested page
	struct in_addr ip_addr;//struct for the ipv ipaddr
	
	char _name1[strlen(_name)];

	strcpy(_name1,_name);
	req_addr= gethostbyname(_name1);
	//iterate through possible requested server ip addresses
	while((req_addr->h_addr_list[i])!=NULL){
		char _list[strlen(list)]; //local blacklist
		bzero(_list,strlen(list));
		strcpy(_list,list);
		
		

		ip_addr= *(struct in_addr *) (req_addr->h_addr_list[i]);
		
		//break blacklist buffer by \n delimeter
		sub=strtok(_list,"\n");
		while(sub!=NULL){
			struct hostent *black_addr;//struct for blacklist page
			int j=0;//iterate through possible blacklist ip addresses
			struct in_addr b_addr;//struct for black list ipv addr
			char name2[strlen(sub)];//char buffer for blacklist names
			bzero(name2,sizeof(name2));
			strcpy(name2,sub);

			black_addr=NULL;
			black_addr=gethostbyname(name2);

			if(black_addr==NULL){
				printf("gethostbyname error\n");
				break;
			}

			//iterate through possible blacklist ip addresses
			while((black_addr->h_addr_list[j])!=NULL){
				b_addr=*(struct in_addr *) (black_addr->h_addr_list[j]);

				char ip1[strlen(inet_ntoa(ip_addr))];//char buffer for request ip
				char ip2[strlen(inet_ntoa(b_addr))];//char buffer for blacklist ip

				strcpy(ip1,inet_ntoa(ip_addr));
				strcpy(ip2,inet_ntoa(b_addr));
				
				//blacklist entry was in name form
				if((strcmp(ip2,name2))!=0){
					//compare the two names
					if((strcmp(_name1,name2))==0){
						printf("\nreq name: %s\nblacklist name: %s\n",_name1,name2 );
						printf("req ip: %s\nblacklist ip: %s\n",ip1,ip2 );
						printf("match through name\n");
						return 1;
					}
				}
				//compare req addr with blacklist addr
				if (strcmp(ip1,ip2)==0){
					printf("\nreq name: %s\nblacklist name: %s\n",_name1,name2 );
					printf("req ip: %s\nblacklist ip: %s\n",ip1,ip2 );
					printf("match through ip\n");
					return 1;
				}
				j=j+1;
			}
			//move to next line in blacklist
			sub=strtok(NULL,"\n");
		}
		i=i+1;
	}
	return 0;
}


//basic hash function
//not efficient, fairly high collision
int generate_id(char _name[100]){
    int total=0;
    int size=strlen(_name);
    for(int i=0;i<=size;i++){
        total=total+_name[i];
    }
    return total%100;
}

//write the data buffer to a file in the cache directory
//file is named by the hash value from generate_id
//updates cache array with current time values
void save_to_cache(struct tm _cache[],int _id,char data[]){
	time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info=gmtime(&rawtime);
	
	FILE *fp;
	char filename[100];
    sprintf(filename,"./cache/%d",_id);
    
    if(NULL==(fp=fopen(filename,"w"))){
        error("file failed to open");
    }
    fwrite(data,1,strlen(data),fp);

    fclose(fp);

    _cache[_id]=*info;
}

//sends curl request to requested url
void curl_request(FILE *_cfp, char _name[100],CURLM *_easyhandle)
{
    if(NULL==(_cfp=fopen("body.out","wb"))){
        error("file failed to open");
    }


    _easyhandle = curl_easy_init();
    curl_easy_setopt(_easyhandle,CURLOPT_URL,_name);
    curl_easy_setopt(_easyhandle,CURLOPT_WRITEDATA,_cfp);
    CURLcode res;
    res=curl_easy_perform(_easyhandle);

    curl_easy_cleanup(_easyhandle);
    fclose(_cfp);
}




int ceck_cache(struct tm _cache[],int _ttl,int _id){
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info=gmtime(&rawtime);

    struct tm page;
    page=_cache[_id];
    
    int current=0;
    int pt=0;
    //converts time to seconds
    //compares the difference to the ttl
    //errors when a file's TTL goes over the new week
    pt=(page.tm_hour*60*60)+(page.tm_min * 60) + page.tm_sec;
    current=(info->tm_hour*60*60)+(info->tm_min*60) + info->tm_sec;
    if(current-pt > _ttl){
    	printf("file not in cache\n");
        return 0;
    }
    return 1;
}
