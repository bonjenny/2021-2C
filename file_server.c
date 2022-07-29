#include <stdio.h>				// BUFSIZ, printf()
#include <netinet/in.h>				// struct sockaddr_in
#include <sys/socket.h>				// socket(), bind(), listen(), accept()
#include <dirent.h>				// DIR, struct dirent
#include <string.h>				// strcmp(), strlen()
#include <stdlib.h>				// exit()
#include <fcntl.h>				// O_RDONLY, open()

#define PORT 5000		// file 서버는 5000번 포트로 서비스를 제공
char errMsg[] = “directory open error\n”;	 // opendir()이 실패할 때, 출력할 문자열
char rBuffer[BUFSIZ];		// 클라이언트가 보낸 문자열을 저장하기 위한 버퍼
char hbuf[] = “Hello, world\n”;	// print가 전달되었을 때, 클라이언트에 전송할 문자열

main()
{
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len, n, length, fd;
	char *temp;
	char buf[BUFSIZ];

	DIR *dp;
	struct dirent *dir;
	s_socket = socket(PF_INET, SOCK_STREAM, 0);	// 소켓 생성

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// IP 주소 정보 저장
	s_addr.sin_family = AF_INET;			// IPv4 정보 저장
	s_addr.sin_port = htons(PORT);			// PORT 번호 정보 저장

	if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == 1)
	{ // 소켓과 위 주소 정보를 연결
		printf(“Cannot Bind\n”);
		return –1;
	}

	if(listen(s_socket, 5) == -1)
	{ // 운영체제에 소켓을 활성화하도록 요청
		printf(“Listen Fail\n”);
		return –1;
	}

	while(1)
	{
		len = sizeof(c_addr);			// 연결 요청 수락
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);
		length = 0;
		temp = rBuffer;

		// 출력 요청 명령 대기하다가 명령 오면 동작
		while((n = read(c_socket, temp, 1)) > 0) {
			if (*temp == ‘\r’) continue;
			if (*temp == ‘\n’) break;	
			if (*temp == ‘\0’) break;	
			if (length == BUFSIZ) break;
			length++; temp++;
		}
		rBuffer[length] = ‘\0’;		// 클라이언트가 보낸 문자열 저장
		if(!strcmp(rBuffer, “ls”)) {		// 명령이 “ls“인 경우
			if((dp = opendir(“.”)) == NULL) {	// 클라이언트로 오류문구 전송
				write(c_socket errMsg, strlen(errMsg)); 
				exit(1);			// 비정상 종료, 함수 강제 종료
			}
			else {
				while((dir = readdir(dp)) != NULL) {
					if(dir->d_ino == 0) continue;
					write(c_socket, dir->d_name, strlen(dir->d_name));
					wirte(c_socket, “\n”, 1); // 가독성 위해 줄 넘김
				}
				closedir(dp);
			}
		}
		else if(!strcmp(rBuffer, “print”)) {	// 명령이 “print“인 경우
			n = strlen(buffer);
			write(c_socket, hbuf, n);
		}
		else if(!strcmp(rBuffer, “get”, 3)) {	// 명령이 “get“인 경우
			if((fd = open(rBuffer + 4, O_RDONLY)) < 0) {
				printf(“%s file open error\n”, rBuffer + 4);
				exit(1);
			}
			// fd의 내용을 buf로 sizeof(buf)만큼 읽어옴
			// 읽기에 실패할 경우 –1을 반환
			while((n = read(fd, buf, sizeof(buf))) > 0) {
				write(c_socket, buf, n);
			}
		}
		close(c_socket);
	}
	close(s_socket);
}