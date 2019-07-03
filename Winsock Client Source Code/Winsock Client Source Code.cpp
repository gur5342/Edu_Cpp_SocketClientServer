#include <winsock2.h>
#include <iostream>>
#include <string>
#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32")

using namespace std;

DWORD WINAPI SendThread(LPVOID lpData);

char ID[10];

int main()
{
	// 소켓 라이브러리 초기화
	WSADATA wsaData; // 소켓의 초기화 정보를 저장하기 위한 구조체
	//WSAStartup(WINSOCK_VERSION, &wsaData); // 가장 먼저 호출되는 함수
	WSAStartup(MAKEWORD(2,2), &wsaData);
	//소켓 버전(2,2), 2.2는 실수 이므로 MAKEWORD를 사용
	char PORT[5], IP[15];

	// 소켓 생성
	SOCKET socket_client = socket(AF_INET, SOCK_STREAM, 0);
	// ?

	cout << "사용할 IP 주소를 입력하세요 : ";
	cin >> IP;
	cout << "사용할 포트 번호를 입력하세요 : ";
	cin >> PORT;


	// 소켓 주소 정보
	SOCKADDR_IN tListenAddr = { }; // 주소정보를 담아두는 구조체
	tListenAddr.sin_family = AF_INET; // sin_family는 반드시 AF_INET여야 한다.
	tListenAddr.sin_addr.s_addr = inet_addr(IP);
	tListenAddr.sin_port = htons(atoi(PORT)); // 포트

	// 소켓 접속
	if (connect(socket_client, (struct sockaddr*) & tListenAddr, sizeof(tListenAddr)) == SOCKET_ERROR)
	{
		closesocket(socket_client);
		cout << "서버에 접속 할 수 없습니다.";
		return SOCKET_ERROR;
	}
	

	CreateThread(NULL, 0, SendThread, (LPVOID)socket_client, 0, NULL);
	// 스레드 커널 오브젝트 보안 특성 기본 NULL
	// 스레드 스택 크기 0
	// SendThread 실행 
	// 실행되는 함수에 넘겨지는 매개변수 socket_clinet -> ipdata
	// 스레드 제어하는 flag = 0

	// 상태 출력
	DWORD dwTmp;
	char text[1024] = "서버에 접속되었습니다.\n";
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), text, strlen(text), &dwTmp, NULL);

	int clinet = 1; // 안되면 지울 것


	// 전송 & 수신 루푸
	while (1)
	{
		
		char buffer[1024] = { 0 };
	
		int len = recv(socket_client, buffer, sizeof(buffer), 0);
		if (len <= 0)
		{
			cout << "접속 종료" << endl;
			break;
		}
	
		// 메시지 출력
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, strlen(buffer), &dwTmp, NULL);
	
	}

	//소켓 해제
	closesocket(socket_client);
	
	WSACleanup(); 
	// 소켓을 활용하는것은 WSAStartup 함수와 WSACleanup 함수 사이에 작성
	// 소멸자 같은 개념이다. 지정한 내용을 지워준다.
    
	return 0;
}


DWORD WINAPI SendThread(LPVOID lpData)
{
	SOCKET socket_client = (SOCKET)lpData;
	while (1)
	{	
		string a;
		//cin >> a;
		//if (cin.get() == '\n') {
		//	cout << "접속할 내용 : ";
		//
		//}
		
		//getline(cin, a);
		
		/*
		if (a == "") {	
			cout << "접속할 내용 : ";
			//system("cls");
		}
		*/
		cout << "접속할 내용 : ";

		DWORD dwTmp;
		char text[1024] = { };
		ReadFile(GetStdHandle(STD_INPUT_HANDLE), text, 1024, &dwTmp, NULL);
		// 1024 -> 읽어들일 데이터의 길이
		send(socket_client, text, strlen(text), 0);
	}
	return 0;
}
