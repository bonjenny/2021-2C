#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#define PORT 7000

void do_echo(int);

main()
{
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len, id;		// fork() 함수의 반환값을 저장할 변수 id
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
		id = fork();	// 부모 프로세스 복제, 자식 프로세스 생성
		if(id > 0) { close(c_socket); continue; } // id > 0, 부모 프로세스
		else if(id == 0) { 			  // id = 0, 자식 프로세스
			do_echo(c_socket); close(s_socket); return; }
		else { printf("fork() error\n"); return; } // fork 호출 실패
	}
	close(s_socket);
}
void do_echo(int c_socket) {
	char rBuffer[BUFSIZ]; char *temp; int length, n;
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