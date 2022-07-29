#include <dirent.h>						 // 구조체 DIR 사용
#include <stdlib.h>						 // 함수 exit() 사용
#define PORT 7000
char errMsg[] = “directory open error\n”;
char rBuffer[BUFSIZ];

main()
{
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len, n, length;
	char *temp;
	DIR *dp;            // 구조체
	struct dirent *dir;    // dirent(d_ino, d_off, d_reclen, d_name[NAME_MAX+1])
	s_socket = socket(PF_INET, SOCK_STREAM, 0);		 // create socket
	...

	while(1)
	{
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);
		length = 0;
		temp = rBuffer;
		while((n = read(c_socket, temp, 1)) > 0) { // c_socket에서 1만큼씩 읽기
			if (*temp == ‘\r’) continue;
			if (*temp == ‘\n’) break;	
			if (*temp == ‘\0’) break;	
			if (length == BUFSIZ) break;
			length++; temp++;
		}
		rBuffer[length] = ‘\0’;

		if(!strcmp(rBuffer, “ls”)) { // rBuffer==“print“
			 // opendir (=open directory)
			if((dp = opendir(“.”)) == NULL) {
				write(c_socket errMsg, strlen(errMsg)); 
				exit(1); // <stdlib.h> 필요, 0이면 정상, 1이면 비정상종료
			}
			else {
				while((dir = readdir(dp)) != NULL) { // 다 읽으면 NULL
					if(dir->d_ino == 0) continue;
					write(c_socket, dir->d_name, strlen(dir->d_name));
					wirte(c_socket, “\n”, 1);
				}
				 // closedir (=close directory)
				closedir(dp);
			}
		} // c_socket(client 소켓), dir->d_name, strlen(전송할 데이터 크기)
		close(c_socket);
	}
	close(s_socket);
}