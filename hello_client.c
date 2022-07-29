#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

main(int argc, char *argv[])
{
	int c_socket;
	struct sockaddr_in s_addr;
	char buffer[BUFSIZ];
	int str_len;

	if(argc != 3) { ... }
	c_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(c_socket == -1) printf("socket() error\n");

	memset(&s_addr, 0, sizeof(s_addr));		// 메모리의 시작 주소 초기화
	s_addr.sin_addr.s_addr = inet_addr(argv[1]);	// 사용 가능한 IP주소 할당
	s_addr.sin_family = AF_INET;			// IPv4 프로토콜 사용
	s_addr.sin_port = htons(atoi(argv[2]);		// PORT 내선번호 할당

	if(connect(c_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
		printf("connect() error\n");
	} else  printf("Connected...\n");

	str_len = read(c_socket, buffer, sizeof(buffer) -1);
	if(str_len == -1) printf("read() error\n");
	printf("Message from server: %s", buffer);

	close(s_socket);
}