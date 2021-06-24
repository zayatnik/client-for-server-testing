#include <WinSock2.h>
#include <iostream>
#include <Windows.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <istream>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <ctime>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996) 

using namespace std;

const int N = 6;

const double p = 180 / pow(2, 23);

class coord { //класс координат в градусах (десяичн.)
private:
    double s, d; //широта и долгота
public:
    void getcoord() {
        gets();
        getd();
    }

    void gets() {
        cout << "enter the latitude in degrees\n";
        cin >> s;
    }

    void getd() {
        cout << "enter the longitude in degrees\n";
        cin >> d;
    }

    unsigned char* bin() {
        int s1 = int(abs(s / p));
        int d1 = int(d / p);
		unsigned char* RES = new unsigned char[6];
		memset(RES, 0, 6);
        for (int i = 0; i < 3; i++) {
            int a = s1 >> 8 * (2 - i);
            unsigned char tmp = a;
            RES[i] = tmp;
            s1 = s1 - (a << (8 * (2 - i)));
        }
        if (s < 0)
            RES[0] = RES[0] + 128;
        for (int i = 3; i < 6; i++) {
            int a = d1 >> 8 * (5 - i);
            unsigned char tmp = a;
            RES[i] = tmp;
            d1 = d1 - (a << (8 * (5 - i)));
        }
        return RES;
    }
};

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData)) { //инициализация Winsock
        WSACleanup();
    }
    char* MES = new char(N);
	int port = 1234;
    struct sockaddr_in ad; //переменная для хранения адреса
    SOCKET s = socket(PF_INET, SOCK_DGRAM, 0); //создание сокета на отправление
	cout << "Enter port \n";
	cout << "\n";
    ad.sin_family = AF_INET; //тип адреса
    ad.sin_port = htons(port); //порт
    ad.sin_addr.s_addr = inet_addr("127.0.0.1"); //адрес сервер
    srand(time(NULL));
        for (int i = 0; i < N; i++)
            MES[i] = char(i) + 200;
        int b1 = sendto(s, MES, N, 0, (sockaddr*)&ad, sizeof(ad)); //отправка сообщения
        if (b1 < 0)
            cout << "sending error\n";
        else
            cout << b1 << " bytes of data were sent\n";
        closesocket(s); //закрытие сокета
	return 0;
}