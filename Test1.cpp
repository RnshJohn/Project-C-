#include<string.h>
#include <stdio.h>
#include<stdlib.h>

#ifdef WIN32
#include <window.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#define closesocket close
#endif

#include <thread.h> //create multithread
class TcpThread
{
public:
  void Main()
  {
    char buf[1024] = {0};

    for(;;)
    {
      int recvlen = recv(client, buf, sizeof(buf)-1, 0);
      if(recvlen <= 0)break;
      if(strstr(buf, "quit") != NULL)
      {
        char re[] = "quit success!\n";
        send(client,re,strlen(re)+1, 0);
        break;
      }
      buf[recvlen] = '\0';
      int sendlen = send(client,"ok\n", 4, 0); //判斷數據不會被切割


      printf("%s", buf);
      /*printf("recv ");
      for(int i = 0 ; i <= (recvlen -1) ; i++)
      {
        printf("%c",buf[i]);
      }*/
    }
    closesocket(client);
  }
}


int main(int argc, char *argv[])
{
  #ifdef WIN32
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws);
  #endif
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1)
  {
    printf("create socket failed!\n");
    return -1;
  }

  unsigned short PORT = 8080;

  if(argc > 1)
  {
    PORT = atoi(argv[1]);
  }

  sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(PORT);
  saddr.sin_addr.s_addr = htonl(0);

  if(bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0)
  {
    printf("bind port %d failed",PORT);
    return -2;
  }
  printf("bind %d port success!",PORT);
  listen(sock,10);

  for(;;)
  {
    sockaddr_in caddr;
    socklen_t len = sizeof(caddr);

    int client = accept(sock, (sockaddr*)&caddr, &len); //create other socket   3+1 = 4  連接客戶端
    printf("accept client %d\n",client);


    char *ip = inet_ntoa(caddr.sin_addr);
    unsigned short cport = ntohs(caddr.sin_port);

    printf("client ip is %s, port is %d\n",ip,cport);
  }

  //printf("[%d]",sock);
  closesocket(sock);
  getchar();
  return 0;
}
