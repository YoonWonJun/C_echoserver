#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <time.h>

#define ECHO_PORT 3000					// 포트번호를 3000으로 정의

void PrintData(char *data);
void ErrorHandling(char *message);

main() {
	WSADATA wsaData;
	int sock, sock2;
	struct sockaddr_in server, client;
	int len;
	char read_buf[256];					// 클라이언트로부터 받는 데이터를 저장할 공간 할당
	char send_buf[256];					// 클라이언트에게 전송할 데이터를 저장할 공간 할당
	time_t today;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)			// 윈도우 소켓 초기화
		ErrorHandling("WSAStartup() errer!");

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)		// 서버 소켓 생성
		ErrorHandling("hSocketet() error!");

	server.sin_family = AF_INET;							// IPv4 인터넷 프로토콜
	server.sin_addr.s_addr = htonl(INADDR_ANY);				// 32bit IPv4 주소 지정
	server.sin_port = htons(ECHO_PORT);						// 사용할 포트번호 지정

	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)	// socket에 필요한 정보를 할당
		ErrorHandling("bind() error!");

	if (listen(sock, 5) == SOCKET_ERROR)										// 클라이언트 접속요청 확인
		ErrorHandling("listen() error");

	while (1) {
		len = sizeof(client);

		if ((sock2 = accept(sock, (struct sockaddr *)&client, &len)) == -1)	// 클라이언트에서 응답요청을 보낼때까지 대기
			ErrorHandling("accept() error!!");

		if (recv(sock2, read_buf, sizeof(read_buf), 0) == -1)				// 클라이언트에서 전송한 데이터가 도착할때까지 대기
			ErrorHandling("read() error!");

		time(&today);
		strcpy(send_buf, ctime(&today));									// 현재시간을 send_buf 배열에 복사
		strcat(send_buf, read_buf);											// 현재시간과 클라이언트로부터 받은 데이터를 연결

		PrintData(send_buf);												// 데이터를 출력해주는 함수 호출

		send(sock2, send_buf, strlen(send_buf) + 1, 0);						// 클라이언트에서 전송한 데이터와 현재 서버시간을 합쳐 전송

		closesocket(sock2);													// 클라이언트와 연결된 소켓 자원 반납
	}
	WSACleanup();															// 윈도우 소켓 해제
	return 0;
}

void PrintData(char *data)													// 데이터 출력 함수
{
	char c_data[256];
	char time_buf[30];
	char message_buf[256];
	char *strToken;

	strcpy(c_data, data);													// 매개변수로 받은 전송할 데이터를 c_data로 복사

	strToken = strtok(c_data, "\n");										// \n을 기준으로 문자열 분리
	strcpy(time_buf, strToken);
	strToken = strtok(NULL, "\n");
	strcpy(message_buf, strToken);

	printf("(Received message time: %s)\n\"%s\"\n", time_buf, message_buf);	// 양식에 맞게 분리된 데이터 출력
}

void ErrorHandling(char *message)											// 에러 메시지 출력 함수
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}