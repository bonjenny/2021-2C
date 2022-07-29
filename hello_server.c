#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
char buffer[BUFSIZ] = “Hello, world!\n”;

main()
{
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len, n;
	s_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&s_addr, 0, sizeof(s_addr));		// 메모리의 시작 주소 초기화
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// 사용 가능한 IP주소 할당
	s_addr.sin_family = AF_INET;			// IPv4 프로토콜 사용
	s_addr.sin_port = htons(PORT);			// PORT 내선번호 할당

	if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == 1)
	{
		printf(“Cannot Bind\n”);
		return –1;				// bind에 실패하면 –1 반환
	} // s_socket(서버의 소켓), s_addr(서버의 주소), sizeof(s_addr)(서버의 주소 길이)

	if(listen(s_socket, 5) == -1)
	{
		printf(“Listen Fail\n”);
		return –1;				// listen 실패하면 –1 반환
	} // s_socket(서버의 소켓), 5(연결 요청 대기실, Queue)
	 // 서버 세팅 끝

	while(1)
	{ // 무한 반복을 하면서 서비스 수행
		len = sizeof(c_addr);
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);
		n = strlen(buffer);			 // 보낼 데이터의 크기
		write(c_socket, buffer, n);			 // 보낼 데이터 저장된 버퍼
		close(c_socket);				 // 클라이언트 소켓
	} // s_socket(서버의 소켓), c_addr(client의 주소), sizeof(c_addr)(client의 주소 길이)
	close(s_socket);
}