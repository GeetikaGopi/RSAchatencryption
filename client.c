#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#define MAX 99
#define PORT 43454
#define SA struct sockaddr
//RSA Algorithm part
long long int p,q,n,t,flag,e[100],d[100],temp[100],temp1[100],j,m[100],en[100],en1[100],i,key[2],key1[2];
char sk='n',msg[100],cmp[100];
int check_prime(long long int);
void cal_e();
31
long long int cal_d(long long int);
void encrypt();
void decrypt();
long long int isprime(int n)
{
 int i;
 if(n % 2 == 0 || n == 1) { return 0; }
 for(i = 3; i*i <= n; i += 2) {
 if(n % i == 0) { return 0; }
 }
 return 1;
}
long long int randPrime(void)
{
 int r,up=9900,lo=100;
 while(!isprime((r = rand()%(up+1-lo)+lo)));
 return r;
}
int check_prime(long long int pr)
{
 int i;
 j=sqrt(pr);
 for(i=2;i<=j;i++)
 {
 if(pr%i==0)
 return 0;
 }
 return 1;
}
32
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
33
void encrypt() {
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
\nFrom Sender: ");
 for(i=0;en[i]!=1;i++)
34

{
en1[i]=htonl(en[i]);
printf("%c",en[i]); }
printf("
\n");
}
void decrypt() {
 long long int pt,ct,ke=d[0],k;
 i=0;
 while(cmp[i]!='t')

{
 if(cmp[i]=='y')

{
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
\nReceived message: ");
 for(i=0;m[i]!=
-1;i++)

{
 printf("%c",m[i]);
35
}
printf("\n");
}
int main(int argc, char *argv[])
{
int sockfd,len,it;
struct sockaddr_in servaddr;
sockfd=socket(AF_INET,SOCK_DGRAM,0);
if(sockfd==-1)
{
printf("Socket creation failed...\n");
exit(0);
}
else
printf("Socket successfully created..\n");
bzero(&servaddr,sizeof(len));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
servaddr.sin_port=htons(PORT);
len=sizeof(servaddr);
for(;;)
{
int aa=0;
p=randPrime();
 q=randPrime();
 printf("%lld %lld\n",p,q);
 flag=check_prime(q);
 n=p*q;
 t=(p-1)*(q-1);
 cal_e();
printf("Sending Keys to Server for encryption\n");
key[0]=htonl(n);
key[1]=htonl(e[0]);
36
sendto(sockfd,key,sizeof(key),0,(SA *)&servaddr,len);
recvfrom(sockfd,en1,sizeof(en1),0,(SA *)&servaddr,&len);
recvfrom(sockfd,temp1,sizeof(temp1),0,(SA *)&servaddr,&len);
recvfrom(sockfd,cmp,sizeof(cmp),0,(SA *)&servaddr,&len);
for(it=0;;it++)
{
en[it]=ntohl(en1[it]);
temp[it]=ntohl(temp1[it]);
if(cmp[it]=='t')
break;
}
decrypt();
recvfrom(sockfd,key,sizeof(key),0,(SA *)&servaddr,&len);
 for(it=0;it<2;it++)
{
key1[it]=ntohl(key[it]);
printf("%lld\n",key1[it]);
}
printf("Enter message: ");
memset(msg, 0, sizeof(msg));
fflush(stdin);
while((msg[aa++]=getchar())!='\n');
for(i=0;msg[i]!=NULL;i++)
 {
 m[i]=msg[i];
}
i--;
encrypt();
 sendto(sockfd,en1,sizeof(en1),0,(SA *)&servaddr,len);
sendto(sockfd,temp,sizeof(temp),0,(SA *)&servaddr,len);
sendto(sockfd,cmp,sizeof(cmp),0,(SA *)&servaddr,len);
}
close(sockfd);
}