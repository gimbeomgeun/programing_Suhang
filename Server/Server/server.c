#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include "Queue.h"


#pragma comment(lib, "ws2_32.lib")

#define AREA_ROW 20
#define AREA_COL 10
#define BLOCK_ROW 4
#define BLOCK_COL 4

#define CENTER 4

#define LEFT 75
#define RIGHT 77


enum {
    BLACK,
    DARK_BLUE,
    DARK_GREEN,
    DARK_SKYBLUE,
    DARK_RED,
    DARK_VOILET,
    DARK_YELLOW,
    GRAY,
    DARK_GRAY,
    BLUE,
    GREEN,
    SKYBLUE,
    RED,
    VIOLET,
    YELLOW,
    WHITE,
};

int area[AREA_ROW + 1][AREA_COL + 6];
int now_area[AREA_ROW + 1][AREA_COL + 6];

int block[7][BLOCK_ROW][BLOCK_COL] = {
    {
        {1,1,1,1},  //SKYBLUE
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
    } ,
    {
        {2,2,0,0},  //YELLOW
        {2,2,0,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {3,3,0,0},  //RED
        {0,3,3,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {0,4,4,0},  //GREEN
        {4,4,0,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {5,5,5,0},  //BLUE
        {0,0,5,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {0,0,6,0},  //DARK_YELLOW
        {6,6,6,0},
        {0,0,0,0},
        {0,0,0,0},
    },
    {
        {7,7,7,0},  //DARK_PURPLE
        {0,7,0,0},
        {0,0,0,0},
        {0,0,0,0},
    }
};

int isGameover = 1;

//점수 관련 변수
int cnt = 0;
int score = 0;


int now_block[BLOCK_ROW][BLOCK_COL];
int fast_block[BLOCK_ROW][BLOCK_COL];

int hard_y = 0, hard_x = 0;

int r;
int ishold = 0;
int hold=-1;
struct rank
{
    char name[20];
    int sco;
}typedef rank;
rank List[11];


int minobag1[7] = { 0,1,2,3,4,5,6 };
int minobag2[7] = { 0,1,2,3,4,5,6 };

void rotate_block();      //블록 회전
int is_crash(int d, int s);           //내려갈 때 충돌판정
int is_FullLine();     //한줄완성판별
int block_down(int r, int d, int s);        //블록내리기
int input();       //사용자입력
int create_block();     //랜덤으로 블록생성
void print_area();      //화면출력
void area_init(int r, int d, int s);//화면 초기화
void area_change();     //옳은 값이면 과거를 현재로바꾸기
void r_area_change();      //잘못된 값이면 현재를 옳은값인 과거로 바꾸기
int block_side(int r, int d, int s, int lr);    //블록 옆으로이동
int is_crash2(int d, int s);    //옆으로 이동할때 충돌판정
void block_change();
void block_change2();
void r_block_change();
int line_down(int d);
int is_GameOver();
void create_area();
int change_Ranking();
int print_Ranking();
int hard_drop(int d, int s);
int color(int code);
void createGarbageLine(int x);
void shuffle(int arr[], int size);
void minobagPush();
void isMinobagEmpty();

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//socket connect varialble
WSADATA wsa;
SOCKET serverSocket, clientSocket;
struct sockaddr_in server, client;
int clientAddrLen = sizeof(client);
char message[1000];

int receivedLine = 0;
//받은 줄 수 를 저장할 Queue
Queue LineQueue;

DWORD WINAPI ReceiveLine(LPVOID lpParam);

int main() {

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Create a socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind
    if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen to incoming connections
    listen(serverSocket, 3);
    printf("Server listening on port 8888...\n");

    // Accept and handle incoming connections
    clientSocket = accept(serverSocket, (struct sockaddr*)&client, &clientAddrLen);
    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    srand(time(NULL));

    HANDLE thread_handle;

    // 스레드 생성 및 실행
    thread_handle = CreateThread(NULL, 0, ReceiveLine, NULL, 0, NULL);
    if (thread_handle == NULL) {
        perror("Error creating thread");
        return 1;
    }
    initQueue(&LineQueue);
    create_area();
    create_block();

    /*while (1)
    {
        int receivedLine;
        recv(clientSocket, (char*)&receivedLine, sizeof(int), 0);
        printf("%d\n", receivedLine);
    }*/


    //while (1) {
    //    // Receive data from client
    //    memset(message, 0, sizeof(message));
    //    if (recv(clientSocket, message, sizeof(message), 0) == SOCKET_ERROR) {
    //        printf("recv failed: %d\n", WSAGetLastError());
    //        closesocket(clientSocket);
    //        WSACleanup();
    //        return 1;
    //    }

    //    printf("Client: %s\n", message);

    //    // Send data back to client
    //    printf("Server: ");
    //    gets_s(message, sizeof(message));
    //    send(clientSocket, message, strlen(message), 0);
    //}

    WaitForSingleObject(thread_handle, INFINITE);

    CloseHandle(thread_handle); // 스레드 핸들 닫기

    printf("Thread finished\n");

    closesocket(serverSocket);
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

//pthread function to receiveline
DWORD WINAPI ReceiveLine(LPVOID lpParam)
{
    while (1)
    {
        recv(clientSocket, (char*)&receivedLine, sizeof(int), 0);
        if (!isGameover) break;
    }
    return 0;
}

//game functions..

void shuffle(int arr[], int size) {
    // 배열의 끝부터 시작하여 랜덤한 위치의 원소와 교환
    for (int i = size - 1; i > 0; --i) {
        // 0에서 i까지의 랜덤한 인덱스 선택
        int j = rand() % (i + 1);

        // arr[i]와 arr[j]를 교환
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void minobagPush()
{
    for (int i = 0; i < 6; i++)
        minobag1[i] = minobag1[i + 1];
    minobag1[6] = minobag2[0];
    for (int i = 0; i < 6; i++)
        minobag2[i] = minobag2[i + 1];
    minobag2[6] = -1;
}

void isMinobagEmpty()
{
    for (int i = 0; i < 7; i++)
    {
        if (minobag2[i] != -1)
            return;
    }
    for (int i = 0; i < 7; i++)
    {
        minobag2[i] = i;
    }
    shuffle(minobag2, 7);
}
int color(int code)
{
    switch (code)
    {
    case 1:
        return SKYBLUE;
    case 2:
        return YELLOW;
    case 3:
        return RED;
    case 4:
        return GREEN;
    case 5:
        return DARK_BLUE;
    case 6:
        return DARK_YELLOW;
    case 7:
        return VIOLET;
    case 8:
        return GRAY;
    }
}

void area_init(int r, int d, int s)
{
    for (int i = d; i < BLOCK_ROW + d; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {

            if (area[i][j + CENTER + s] == now_block[i - d][j])
            {
                area[i][j + CENTER + s] = 0;
            }
        }
    }
}

void print_area()
{
    system("cls");

    for (int i = 0; i < AREA_ROW; i++)
    {
        for (int j = 0; j < AREA_COL + 2; j++)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
            if (now_area[i][j] == 0)
            {
                if (hard_y <= i && i <= hard_y + BLOCK_ROW - 1 && hard_x + CENTER <= j && j <= hard_x + BLOCK_COL + CENTER - 1 && now_block[i - hard_y][j - CENTER - hard_x] != 0)
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color(r + 1));
                printf("□");
            }
            else if (now_area[i][j] == -1)
            {
                printf("|");
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color(now_area[i][j]));
                printf("■");
            }
        }
        printf("\n");
    }

    for (int k = 0; k < 5; k++)
    {
        for (int i = 0; i < BLOCK_ROW; i++)
        {
            gotoxy(25, k * 4 + i);
            for (int j = 0; j < BLOCK_COL; j++)
            {
                if (block[minobag1[k]][i][j] != 0)
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color(minobag1[k] + 1));
                    printf("■");
                }
                else printf(" ");
            }
            printf("\n");
        }
    }

}

int hard_drop(int d, int s)
{
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            area[d + i][s + j + CENTER] += now_block[i][j];
        }
    }
    if (is_crash(d, s) == 1)
    {
        r_area_change();
        hard_y = d - 1, hard_x = s;
        return 0;
    }
    area_init(r, d, s);
    hard_drop(d + 1, s);
}

void create_area()
{
    for (int i = 0; i < AREA_ROW + 1; i++)
    {
        for (int j = 0; j < AREA_COL + 2; j++)
        {
            if (j == 0 || j == AREA_COL + 1)      //양옆 맨 끝 9로 설정
            {
                area[i][j] = -1, now_area[i][j] = -1;
            }
            else if (i == AREA_ROW)     //맨 밑 1로설정 -> 충돌판정으로 블록멈추기
            {
                area[i][j] = 1, now_area[i][j] = 1;
            }
            else        //나머지 0
            {
                area[i][j] = 0, now_area[i][j] = 0;
            }
        }
    }
    shuffle(minobag1, 7);
    shuffle(minobag2, 7);
    print_area();
}
int create_block()
{
    //send(clientSocket, (char*)&cnt, sizeof(int), 0);
    cnt = 0;
    ishold = 0;
    area_change();
    is_FullLine();
    createGarbageLine(dequeue(&LineQueue)); //블록 놓을 때 마다 가비지 라인 스택 하나씩 받아와서 생성
    area_change();
    r = minobag1[0];
    minobagPush();
    isMinobagEmpty();
    block_change(r);
    hard_drop(0, 0);
    return block_down(r, 0, 0);     //블록내리기
}




int block_down(int r, int d, int s)
{
    for (int i = d; i < BLOCK_ROW + d; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            area[i][j + CENTER + s] += now_block[i - d][j];
        }
    }
    if (is_crash(d, s) == 1)   //밑에 블록이있으면
    {
        r_area_change();
        if (is_GameOver() == -1)
        {
            print_area();
            change_Ranking();
            return 0;
        }
        return create_block();  //다음블록
    }

    //올바른값이면
    area_change();      //과거를 현재로 변경
    print_area();
    time_t start_time = time(NULL);
    while (1)
    {
        if (receivedLine)
        {
            printf("\n%d\n", receivedLine);
            enqueue(&LineQueue, receivedLine);

            receivedLine = 0;
        }
        int ans = input();      //사용자입력받기
        if (ans == 1 || ans == -1)
        {
            area_change();
            area_init(r, d, s);     //초기화
            s = block_side(r, d, ans + s, ans);  //s값 리턴받기
            area_init(r, d, s);
            hard_drop(d, s);
        }
        else if (ans == 2)
        {
            area_change();
            area_init(r, d, s);
            block_change2();
            rotate_block();
            for (int i = d; i < BLOCK_ROW + d; i++)
            {
                for (int j = 0; j < BLOCK_COL; j++)
                {
                    area[i][j + CENTER + s] += now_block[i - d][j];
                }
            }
            if (is_crash(d, s))
            {
                r_area_change();
                r_block_change();
            }
            //올바른값이면
            area_change();      //과거를 현재로 변경
            area_init(r, d, s);
            hard_drop(d, s);
            print_area();
        }
        else if (ans == 3)
        {
            score += 1;
            area_init(r, d, s);   //초기화
            return block_down(r, d + 1, s);     //d+1로 한칸씩 내리기
        }
        else if (ans == 4)
        {
            area_init(r, d, s);
            for (int i = 0; i < BLOCK_ROW; i++)
            {
                for (int j = 0; j < BLOCK_COL; j++)
                {
                    area[hard_y + i][j + s + CENTER] += now_block[i][j];
                }
            }
            area_change();
            print_area();
            create_block();
        }
        else if (ans == 5)
        {
            if (!ishold)
            {
                ishold = 1;
                if (hold == -1)
                {
                    hold = r;
                    area_init(r, d, s);
                    area_change();
                    r = minobag1[0];
                    minobagPush();
                    isMinobagEmpty();
                    block_change(r);
                    hard_drop(0, 0);
                    return block_down(r, 0, 0);     //블록내리기
                }
                else
                {
                    area_init(r, d, s);
                    int tmp;
                    tmp = hold, hold = r, r = tmp;
                    return block_down(r, 0, 0);
                }
            }
        }
        time_t now_time = time(NULL);
        if (now_time - start_time >= 1)     //1초동안
        {
            break;
        }
    }
    //올바른값이면
    area_change();      //과거를 현재로 변경
    area_init(r, d, s);   //초기화
    score += 1;
    return block_down(r, d + 1, s);     //d+1로 한칸씩 내리기
}

int block_side(int r, int d, int s, int lr)
{
    for (int i = d; i < BLOCK_ROW + d; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            area[i][j + CENTER + s] += now_block[i - d][j];
        }
    }

    if (is_crash(d, s))    //양옆끝 또는 블록에 닿았는지 확인
    {
        r_area_change();    //닿으면 이전값으로 변경
        print_area();
        return s + (lr * -1);       //(lr*-1) : 오른쪽 키 눌렀으면 -1 왼쪽키 눌렀으면 +1
    }

    area_change();  //아니면 과거를 현재로 바꿔주기
    print_area();   //출력
    return s;
}


int is_crash(int d, int s)
{
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            if (now_block[i][j] == 0)
                continue;
            if (area[i + d][j + CENTER + s] > now_block[i][j])
            {
                return 1;
            }
            else if (area[i + d][j + CENTER + s] < now_block[i][j])
            {
                return 2;
            }
        }
    }
    return 0;
}


int input()
{
    int nSelect = 0;
    if (kbhit()) {
        nSelect = getch();
        if (nSelect == 224 || nSelect == 0) {
            nSelect = getch();
            switch (nSelect)
            {
            case LEFT:
                return -1;
            case RIGHT:
                return 1;
            case 72:
                return 2;
            case 80:
                return 3;
            }
        }
        else if (nSelect == 32)
            return 4;
        else if (nSelect == 'c' || nSelect == 'C')
            return 5;
    }
}

void area_change()
{
    for (int i = 0; i < AREA_ROW + 1; i++)
    {
        for (int j = 0; j < AREA_COL + 2; j++)
        {
            now_area[i][j] = area[i][j];
        }
    }
}

void r_area_change()
{
    for (int i = 0; i < AREA_ROW + 1; i++)
    {
        for (int j = 0; j < AREA_COL + 2; j++)
        {
            area[i][j] = now_area[i][j];
        }
    }
}

void block_change(int r)
{
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            now_block[i][j] = 0;
            now_block[i][j] = block[r][i][j];
        }
    }
}

void block_change2()
{
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            fast_block[i][j] = now_block[i][j];
        }
    }
}
void r_block_change()
{
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            now_block[i][j] = fast_block[i][j];
        }
    }
}

void rotate_block()
{
    int tmp[BLOCK_ROW][BLOCK_COL];
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            tmp[i][j] = now_block[i][j];
        }
    }
    for (int i = 0; i < BLOCK_ROW; i++)
    {
        for (int j = 0; j < BLOCK_COL; j++)
        {
            now_block[i][j] = tmp[BLOCK_COL - 1 - j][i];
        }
    }
}

int is_FullLine()
{
    int j;
    int empty_cnt;
    for (int i = AREA_ROW - 1; i >= 0; i--)
    {
        empty_cnt = 0;
        for (j = 1; j < AREA_COL + 1; j++)
        {
            if (area[i][j] == 0)
            {
                empty_cnt++;
            }
        }
        if (empty_cnt == 0)
        {
            cnt++;
            for (j = 1; j < AREA_COL + 1; j++)
            {
                //area[i][j] = 0;
                score += 100 * cnt;
                return line_down(i);
            }
        }
    }
    send(clientSocket, (char*)&cnt, sizeof(int), 0);
}

int line_down(int d)
{
    for (int i = d; i > 0; i--)
    {
        for (int j = 1; j < AREA_COL + 1; j++)
        {
            area[i][j] = area[i - 1][j];
        }
    }
    return is_FullLine();
}


void createGarbageLine(int x)
{
    if (x == -1) return;  //큐에 있는 라인 값이 없을 때

    for (int i = x; i < AREA_ROW; i++)  //받은 칸 만큼 위로 이동
    {
        for (int j = 1; j < AREA_COL + 1; j++)
        {
            area[i - x][j] = area[i][j];
        }
    }
    int hall = rand() % 10 + 1;
    for (int i = 0; i < x; i++) //받은 칸 만큼 가비지 라인 생성
    {
        for (int j = 1; j < AREA_COL + 1; j++)
        {
            if (j == hall) area[AREA_ROW - i - 1][hall] = 0;
            else area[AREA_ROW - i - 1][j] = 8;
        }
    }
}

int is_GameOver()
{
    for (int i = 1; i < AREA_COL + 1; i++)
    {
        if (area[0][i] == 1)
        {
            printf("game over\n");
            return -1;

        }
    }
}

int change_Ranking()
{
    FILE* fp = fopen("ranking.txt", "r");
    char str[100];
    int i, acc = 0, lines = 0, Lbegin[20] = { 0 };
    while (!feof(fp))
    {
        if (fgets(str, sizeof(str), fp) != NULL)
        {
            if (lines > 0)
            {
                Lbegin[lines] = acc;
            }
            acc += strlen(str) + 1;
            //printf("%s", str);
            if (lines % 2 == 0)
            {
                strcpy(List[10 - (lines / 2)].name, str);
            }
            else
            {
                List[10 - (lines / 2)].sco = atoi(str);
            }
            lines++;
        }
    }

    List[0].sco = score;
    printf("이름을 입력하세요 : ");
    scanf(" %s", List[0].name);
    strcat(List[0].name, "\n");
    if (List[0].sco > List[1].sco)
    {
        for (int i = 0; i < 11 - 1; i++)
        {
            for (int j = 0; j < 11 - 1 - i; j++)
            {
                if (List[j].sco > List[j + 1].sco)
                {
                    int tmp;
                    tmp = List[j].sco, List[j].sco = List[j + 1].sco, List[j + 1].sco = tmp;
                    char stmp[20];
                    strcpy(stmp, List[j].name), strcpy(List[j].name, List[j + 1].name), strcpy(List[j + 1].name, stmp);
                }
            }
        }
    }
    fclose(fp);


    FILE* fw = fopen("ranking.txt", "w");
    for (int i = 10; i > 0; i--)
    {
        char ssco[100];
        sprintf(ssco, "%d", List[i].sco);
        fprintf(fw, "%s%s\n", List[i].name, ssco);
    }
    fclose(fw);
    print_Ranking();
}


int print_Ranking()
{
    printf("\n\n\n");
    FILE* fp = fopen("ranking.txt", "r");
    char str[100];
    int i, acc = 0, lines = 0, Lbegin[20] = { 0 };
    while (!feof(fp))
    {
        if (fgets(str, sizeof(str), fp) != NULL)
        {
            if (lines > 0)
            {
                Lbegin[lines] = acc;
            }
            acc += strlen(str) + 1;
            if (lines % 2 == 0)
            {
                printf("\n%d위\n", lines / 2 + 1);
            }
            printf("%s", str);
            lines++;
        }
    }
}