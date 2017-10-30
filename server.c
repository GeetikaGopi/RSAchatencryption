#include<stdio.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define MAX 99
#define PORT 43454
#define SA struct sockaddr
//RSA Algorithm
long long int p,q,n,t,flag,e[100],d[100],temp[100],temp1[100],j,m[100],en[100],en1[100],i,key[2],key1[2];
char sk='m',msg[100],cmp[100];
int check_prime(long long int);
24
void cal_e();
long long int cal_d(long long int);
void encrypt();
void decrypt();
long long int isprime(int n) {
 int i;
 if(n % 2 == 0 || n == 1) { return 0; }
 for(i = 3; i*i <= n; i += 2) {
 if(n % i == 0) { return 0; }

}
 return 1; }
long long int randPrime(void) {
 int r,up=9000,lo=100;
 while(!isprime((r = rand()%(up+1
-lo)+lo)));
 return r; }
int check_prime(long long int pr) {
 int i;
 j=sqrt(pr);
 for(i=2;i<=j;i++)

{
 if(pr%i==0)
 return 0;

}
 return 1; }
25
void cal_e() {
 int k;
 k=0;
 for(i=2;i<t;i++)

{
 if(t%i==0)
 continue;
 flag=check_prime(i);
 if(flag==1&&i!=p&&i!=q)

{
 e[k]=i; flag=cal_d(e[k]);
 if(flag>0)

{
 d[k]=flag;
 k++;

}
 if(k==99)
 break;

}

}
}
long long int cal_d(long long int a) {
 long long int f=1;
 for(;;)

{
 f=f+t;
 if(f%a==0)
 return(f/a);

}
}
26
void encrypt()
{
long long int pt,ct,ke=key1[1],k,len;
 i=0;
 len=strlen(msg);
 while(i!=len
-1)

{
 pt=m[i];
 pt=pt
-96;
 k=1;
for(j=0;j<ke;j++)

{
 k=k*pt;
 k=k%key1[0];

}
 temp[i]=htonl(abs(k));
 ct=k+96;
 if(ct<0)
{
en[i]=abs(ct);
cmp[i]='y';
}
else {
en[i]=ct;
cmp[i]='n';
}
 i++;

}
 en[i]=1;
temp[i]=1;
cmp[i]='t';
 printf("
\nTHE ENCRYPTED MESSAGE IS: ");
 for(i=0;en[i]!=1;i++)
27

{
en1[i]=htonl(en[i]);
printf("%c",en[i]);
}
printf("
\n");
}
void decrypt() {
 long long int pt,ct,ke=d[0],k;
 i=0;
 while(cmp[i]!='t')

{
if(cmp[i]=='y') {
en[i]=0
-en[i];
temp[i]=0
-temp[i];
}
ct=temp[i];
 k=1;
 for(j=0;j<ke;j++)

{
 k=k*ct;
 k=k%n;

}
 pt=k+96;
 m[i]=pt;
 i++;

}
 m[i]=
-1;
 printf("
\nFrom Sender: ");
 for(i=0;m[i]!=
-1;i++)

{
printf("%c",m[i]);
28
}
printf("\n");
}
void func(int sockfd)
{
 int it,clen;
 struct sockaddr_in cli;
 clen=sizeof(cli);
 for(;;)
 {
int aa=0;
recvfrom(sockfd,key,sizeof(key),0,(SA *)&cli,&clen);
 for(it=0;it<2;it++)
{
key1[it]=ntohl(key[it]);
}
printf("Enter message: ");
memset(msg, 0, sizeof(msg));
while((msg[aa++]=getchar())!='\n');
for(i=0;msg[i]!=NULL;i++)
 {
m[i]=msg[i];
}
encrypt();
 sendto(sockfd,en1,sizeof(en1),0,(SA *)&cli,clen); //I AM HERE
sendto(sockfd,temp,sizeof(temp),0,(SA *)&cli,clen);
sendto(sockfd,cmp,sizeof(cmp),0,(SA *)&cli,clen);
 p=randPrime();
 q=randPrime();
 printf("%d %d\n",p,q);
 flag=check_prime(q);
 n=p*q;
 t=(p-1)*(q-1);
29
 cal_e();
key[0]=htonl(n);
key[1]=htonl(e[0]);
sendto(sockfd,key,sizeof(key),0,(SA *)&cli,clen);
recvfrom(sockfd,en1,sizeof(en1),0,(SA *)&cli,&clen);
recvfrom(sockfd,temp1,sizeof(temp1),0,(SA *)&cli,&clen);
recvfrom(sockfd,cmp,sizeof(cmp),0,(SA *)&cli,&clen);
for(it=0;;it++)
{
en[it]=ntohl(en1[it]);
temp[it]=ntohl(temp1[it]);
if(cmp[it]=='t')
break;
}
decrypt();
//printf("Enter q to quit: ");
//scanf("%c",&sk);
 }
}
int main(int argc, char *argv[])
{
 int sockfd;
 struct sockaddr_in servaddr;
 sockfd=socket(AF_INET,SOCK_DGRAM,0);
 if(sockfd==-1)
 {
 printf("socket creation failed...\n");
 exit(0);
 }
 else
 printf("Socket successfully created..\n");
 bzero(&servaddr,sizeof(servaddr));
 servaddr.sin_family=AF_INET;
30
 servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
 servaddr.sin_port=htons(PORT);
 if((bind(sockfd,(SA *)&servaddr,sizeof(servaddr)))!=0)
 {
 printf("Socket bind failed...\n");
 exit(0);
 }
 else
 printf("Socket successfully binded..\n");
 func(sockfd);
 close(sockfd);
}