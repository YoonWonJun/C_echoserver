#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <Windows.h>

#define ECHO_SERVER "127.0.0.1"	// 접속할 서버의 아이피를 127.0.0.1로 정의
#define ECHO_PORT 3000			// 포트번호를 3000으로 정의

void PrintData(char *data);
void ErrorHandling(char *message);

int main(void)
{
	WSADATA wsaData;
	int sock;
	struct sockaddr_in server;
	char buf[256];				// 서버에 전송할 데이터와 수신할 데이터를 저장할 공간 할당

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)	// 윈도우 소켓 초기화
		ErrorHandling("WSAStartup() errer!");

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)	// 소켓 생성
		ErrorHandling("hSocketet() error!");

	server.sin_family = AF_INET;						// IPv4 인터넷 프로토콜
	server.sin_addr.s_addr = inet_addr(ECHO_SERVER);	// 접속할 32bit IPv4 주소 지정
	server.sin_port = htons(ECHO_PORT);					// 접속할 서버의 포트 지정

	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1)	// 서버에 연결을 요청
		ErrorHandling("connect() error!");

	printf("Input Data >>> ");
	fgets(buf, sizeof(buf), stdin);				// 키보드를통해 문자열을 입력

	send(sock, buf, strlen(buf) + 1, 0);		// 입력받은 문자열을 서버에 전송

	if (recv(sock, buf, sizeof(buf), 0) == -1)	// 서버로부터 데이터를받을때까지 대기
		ErrorHandling("read() error!");

	PrintData(buf);								// 데이터를 출력해주는 함수 호출

	closesocket(sock);							// 서버와 연결된 소켓 자원 반납
	WSACleanup();								// 윈도우 소켓 해제
	return 0;
}

void PrintData(char *data)						// 데이터 출력 함수
{
	char c_data[256];
	char time_buf[30];
	char message_buf[256];
	char *strToken;

	strcpy(c_data, data);

	strToken = strtok(c_data, "\n");
	strcpy(time_buf, strToken);
	strToken = strtok(NULL, "\n");
	strcpy(message_buf, strToken);

	printf("(Received message time: %s)\n\"%s\"\n", time_buf, message_buf);
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}