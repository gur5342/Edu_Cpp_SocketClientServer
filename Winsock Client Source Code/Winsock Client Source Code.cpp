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
	// ���� ���̺귯�� �ʱ�ȭ
	WSADATA wsaData; // ������ �ʱ�ȭ ������ �����ϱ� ���� ����ü
	//WSAStartup(WINSOCK_VERSION, &wsaData); // ���� ���� ȣ��Ǵ� �Լ�
	WSAStartup(MAKEWORD(2,2), &wsaData);
	//���� ����(2,2), 2.2�� �Ǽ� �̹Ƿ� MAKEWORD�� ���
	char PORT[5], IP[15];

	// ���� ����
	SOCKET socket_client = socket(AF_INET, SOCK_STREAM, 0);
	// ?

	cout << "����� IP �ּҸ� �Է��ϼ��� : ";
	cin >> IP;
	cout << "����� ��Ʈ ��ȣ�� �Է��ϼ��� : ";
	cin >> PORT;


	// ���� �ּ� ����
	SOCKADDR_IN tListenAddr = { }; // �ּ������� ��Ƶδ� ����ü
	tListenAddr.sin_family = AF_INET; // sin_family�� �ݵ�� AF_INET���� �Ѵ�.
	tListenAddr.sin_addr.s_addr = inet_addr(IP);
	tListenAddr.sin_port = htons(atoi(PORT)); // ��Ʈ

	// ���� ����
	if (connect(socket_client, (struct sockaddr*) & tListenAddr, sizeof(tListenAddr)) == SOCKET_ERROR)
	{
		closesocket(socket_client);
		cout << "������ ���� �� �� �����ϴ�.";
		return SOCKET_ERROR;
	}
	

	CreateThread(NULL, 0, SendThread, (LPVOID)socket_client, 0, NULL);
	// ������ Ŀ�� ������Ʈ ���� Ư�� �⺻ NULL
	// ������ ���� ũ�� 0
	// SendThread ���� 
	// ����Ǵ� �Լ��� �Ѱ����� �Ű����� socket_clinet -> ipdata
	// ������ �����ϴ� flag = 0

	// ���� ���
	DWORD dwTmp;
	char text[1024] = "������ ���ӵǾ����ϴ�.\n";
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), text, strlen(text), &dwTmp, NULL);

	int clinet = 1; // �ȵǸ� ���� ��


	// ���� & ���� ��Ǫ
	while (1)
	{
		
		char buffer[1024] = { 0 };
	
		int len = recv(socket_client, buffer, sizeof(buffer), 0);
		if (len <= 0)
		{
			cout << "���� ����" << endl;
			break;
		}
	
		// �޽��� ���
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, strlen(buffer), &dwTmp, NULL);
	
	}

	//���� ����
	closesocket(socket_client);
	
	WSACleanup(); 
	// ������ Ȱ���ϴ°��� WSAStartup �Լ��� WSACleanup �Լ� ���̿� �ۼ�
	// �Ҹ��� ���� �����̴�. ������ ������ �����ش�.
    
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
		//	cout << "������ ���� : ";
		//
		//}
		
		//getline(cin, a);
		
		/*
		if (a == "") {	
			cout << "������ ���� : ";
			//system("cls");
		}
		*/
		cout << "������ ���� : ";

		DWORD dwTmp;
		char text[1024] = { };
		ReadFile(GetStdHandle(STD_INPUT_HANDLE), text, 1024, &dwTmp, NULL);
		// 1024 -> �о���� �������� ����
		send(socket_client, text, strlen(text), 0);
	}
	return 0;
}
