iam_server.c

#include <stdio.h>				// BUFSIZ, printf()
#include <netinet/in.h>				// struct sockaddr_in
#include <sys/socket.h>				// socket(), bind(), listen(), accept()
#include <dirent.h>				// DIR, struct dirent
#include <string.h>				// strcmp(), strlen()
#include <stdlib.h>				// exit()

#define PORT 6000
char errMsg[] = “directory open error\n”;
char rBuffer[BUFSIZ];
char hbuf[] = “Hello, world\n”;

main()
{
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len, n, length;
	char *temp;

	DIR *dp;
	struct dirent *dir;
	s_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

	if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == 1)
	{
		printf(“Cannot Bind\n”);
		return –1;
	}

	if(listen(s_socket, 5) == -1)
	{
		printf(“Listen Fail\n”);
		return –1;
	}

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

		if(!strcmp(rBuffer, “ls”)) {
			if((dp = opendir(“.”)) == NULL) {
				write(c_socket errMsg, strlen(errMsg)); 
				exit(1);
			}
			else {
				while((dir = readdir(dp)) != NULL) {
					if(dir->d_ino == 0) continue;
					write(c_socket, dir->d_name, strlen(dir->d_name));
					wirte(c_socket, “\n”, 1);
				}
				closedir(dp);
			}
		}
		else if(!strcmp(rBuffer, “print”)) {
			n = strlen(buffer);
			write(c_socket, buffer, n);
		}
		close(c_socket);
	}
	close(s_socket);
}