#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 8000
char buffer[BUFSIZ] = “Hello, world!\n”;
char rBuffer[BUFSIZ];

main()
{
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len, n, length;
	char *temp;
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
		if(!strcmp(rBuffer, “print”)) {		// rBuffer==“print“
			n = strlen(buffer);		// string length
			write(c_socket, buffer, n);
		} // c_socket(client 소켓), buffer, n=strlen(buffer)=12(전송할 데이터 크기)
		close(c_socket);
	}
	close(s_socket);
}