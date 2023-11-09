#pragma once

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

//���� ���� ����
int cnt = 0;
int score = 0;


int now_block[BLOCK_ROW][BLOCK_COL];
int fast_block[BLOCK_ROW][BLOCK_COL];

int hard_y = 0, hard_x = 0;

int r;
struct rank
{
    char name[20];
    int sco;
}typedef rank;
rank List[11];

void rotate_block();      //���� ȸ��
int is_crash(int d, int s);           //������ �� �浹����
int is_FullLine();     //���ٿϼ��Ǻ�
int block_down(int r, int d, int s);        //���ϳ�����
int input();       //������Է�
int create_block();     //�������� ���ϻ���
void print_area();      //ȭ�����
void area_init(int r, int d, int s);//ȭ�� �ʱ�ȭ
void area_change();     //���� ���̸� ���Ÿ� ����ιٲٱ�
void r_area_change();      //�߸��� ���̸� ���縦 �������� ���ŷ� �ٲٱ�
int block_side(int r, int d, int s, int lr);    //���� �������̵�
int is_crash2(int d, int s);    //������ �̵��Ҷ� �浹����
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