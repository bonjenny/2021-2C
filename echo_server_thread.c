#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#define PORT 8000
void *do_echo(void *);

main()
{
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len, sts;
	pthread_t tid;
	s_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

	if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == 1) { ... }
	if(listen(s_socket, 5) == -1) { ... }
	while(1)
	{
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);
		sts = pthread_create(&tid, NULL, do_echo, (void *)&c_socket);
	}
	close(s_socket);
}
void *do_echo(void *data) {
	char rBuffer[BUFSIZ];
	char *temp;
	int length, n;
	int c_socket = * (int *)data;

	while(1) {
		length = 0;
		temp = rBuffer;
		while((n = read(c_socket, temp, 1)) > 0) {
			if (*temp == ‘\r’) continue;
			if (*temp == ‘\n’) break;	
			if (*temp == ‘\0’) break;	
			if (length == BUFSIZ) break;
			length++; temp++;
		}
		rBuffer[length] = ‘\0’;
		if(!strcmp(rBuffer, “quit”)) { close(c_socket); return; }
		else { n = strlen(rBuffer); write(c_socket, rBuffer, n); }
	}
}