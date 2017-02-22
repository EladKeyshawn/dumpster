// test server speedtest.tele2.net

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <termios.h>


//#define SERVER_DOMAIN "speedtest.tele2.net"
#define SERVER_DOMAIN "ftp.drivehq.com"

#define FTP_CONNECT_MODE "PASV"
#define MAXDATASIZE 4096
#define MAXSZ 4096


int authenticate_user(int);

void err(char *msg)
{
    perror(msg);
}


void prline(char* msg) {
    printf("%s \n", msg);
}

void pr(char* msg) {
    printf("%s ", msg);
}


void authenticate(int sockds, char* buffer) {
    char user[20], pass[20];
    char usersend[200] = {0};
    char passsend[200] = {0};
    char* password;
    prline("_________");
    prline("| Login |");
    prline("---------");
    
    pr("user: ");
    scanf("%s", user);
    sprintf(usersend,"USER %s \r\n",user);
    printf("user command: %s", usersend);
    
    if(send(sockds, "USER eladkeyshawn \r\n", 200,0) == -1) err("failed sending user credential.");
    memset(buffer, 0, MAXDATASIZE);
    recv(sockds, buffer, MAXDATASIZE-1,0);
    printf("server: %s\n", buffer);
    
    memset(buffer, 0, MAXDATASIZE);
    
    password = getpass("Password: ");
    sprintf(passsend, "PASS %s\r\n", password);
    printf("password command: %s", passsend);
    
    if(send(sockds, passsend, strlen(passsend),0) == -1) err("failed sending password credential.");
    recv(sockds, buffer, MAXDATASIZE-1,0);
    printf("server: %s\n", buffer);
    
    
}


int main(int argc, char* argv[]) {
    // recieve arguments and validate
    
    int ctrl_sock; // holds sock
    int ftpserver_port = 21; //default ftp connection port
    struct sockaddr_in server; // socket address details of intended server
    
    int data_sock;
    int data_port;
    char main_buff[MAXDATASIZE];
    
    
    
    ctrl_sock = socket(AF_INET, SOCK_STREAM,0);
    if (ctrl_sock == -1) {
        err("failed to create socket()");
        goto fin;
    }
    
    
    struct hostent *host;
    
    
    // perform dns and convert domain to IP
    if((host=gethostbyname(SERVER_DOMAIN)) == NULL) {
        err("could not resolve domain :(");
        goto fin;
    };
    pr("resolved domain:");
    prline(host->h_name);
    server.sin_addr = *((struct in_addr *) host->h_addr);
    
    server.sin_family = AF_INET;
    server.sin_port = htons( ftpserver_port );
    
    prline("socket created.");
    
    
    if (connect(ctrl_sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        err("failed while trying to connect to server.");
        goto fin;
    }
    
    prline("established control connection with remote FTP server.");
    
    
    
    // get data port to connect to
    recv(ctrl_sock, main_buff, MAXDATASIZE,0);
    sscanf(main_buff, "%d", &data_port);
    pr("buffer:"); prline(main_buff);
    pr("data channel port recieved: "); printf("%d \n", data_port);
    
    
    int auth = authenticate_user(ctrl_sock);
    
    if (auth != 1) {
        err("problem via authentication");
        goto fin;
    }
    prline("authentication success.");
    
    
    
    
fin:
    prline("disconnecting from server");
    if(ctrl_sock > 0) {
        close(ctrl_sock);
    }
    
    return 0;
    
    
}




int authenticate_user(int sockfd){
    int temp = -1;
    char *password;
    int no_of_bytes;/* number of bytes sent or received from server */
    
    char data_from_server[MAXSZ];/* message from server*/
    char user_input[MAXSZ];/* input from user */
    char data_to_server[MAXSZ];/* message to server */
    char user[MAXSZ];/* user details sent to server */
    char pass[MAXSZ];/* password details sent to server */
    char username[MAXSZ];/* username entered by the user */
    
    prline("_________");
    prline("| Login |");
    prline("---------");
    
    
    
    printf("Name: ");
    scanf("%s",username);/* Enter name of user on server */
    
    sprintf(user,"USER %s\r\n",username);
    
    send(sockfd,user,strlen(user),0);/* Send username to server */
    
    
    
    while((no_of_bytes = recv(sockfd,data_from_server,MAXSZ,0)) > 0)
    {
        data_from_server[no_of_bytes] = '\0';
        if(strncmp(data_from_server,"331",3) == 0)
        {
            goto getpass;
        }
        
        if(strncmp(data_from_server,"230",3) == 0)
        {
            printf("%s\n",data_from_server);
            return 1;
        }
        
        
        printf("%s\n",data_from_server);
        if(strstr(data_from_server,"230 ") > 0 ||
           strstr(data_from_server,"500 ") > 0 ||
           strstr(data_from_server,"501 ") > 0 ||
           strstr(data_from_server,"421 ") > 0 ||
           strstr(data_from_server,"332 ") > 0 ||
           strstr(data_from_server,"530 ") ||
           strstr(data_from_server,"331 ") > 0)
            break;
        fflush(stdout);
    }
    
    
    
    
getpass:
    password = getpass("Password: ");/* Enter password */
    sprintf(pass,"PASS %s\r\n",password);
    
    send(sockfd,pass,strlen(pass),0);/* Send password to server */
    
    /* Receive message from server */
    while((no_of_bytes = recv(sockfd,data_from_server,MAXSZ,0)) > 0)
    {
        data_from_server[no_of_bytes] = '\0';
        
        if(strncmp(data_from_server,"230",3) == 0)
        {
            printf("%s\n",data_from_server);
            return 1;
        }
        
        if(strncmp(data_from_server,"530",3) == 0)
        {
            return 0;
        }
        
        if(strncmp(data_from_server,"501",3) == 0)
        {
            printf("%s\n",data_from_server);
            return 1;
        }
        
        printf("%s\n",data_from_server);
        fflush(stdout);
        
        if(strstr(data_from_server,"230 ") > 0 ||
           strstr(data_from_server,"500 ") > 0 || 
           strstr(data_from_server,"501 ") > 0 || 
           strstr(data_from_server,"421 ") > 0 ||
           strstr(data_from_server,"332 ") > 0 || 
           strstr(data_from_server,"530 ") ||
           strstr(data_from_server,"503 ") > 0 ||
           strstr(data_from_server,"202 ") > 0 )
        {
            break;
        }
    }
    
    return 0;
}	










