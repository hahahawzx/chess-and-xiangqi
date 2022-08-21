#include <graphics.h>
#include <conio.h>
#include<stdio.h>
#include<string.h>
#include <memory.h>
#include<fstream>
#include<iostream>
using namespace std;
class chessboard;
class chess {
private:
	int id;//代表棋子的身份，正数为红，负数为黑
public:
	int grade = 0;
	chess(int i, int j) :id(i), grade(j) {};
	int get() { return id; }
    int getgrade() { return grade; }
    virtual void upgrade() { grade++;  }
	virtual bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) = 0;//纯虚函数
	virtual ~chess() {};//虚析构
};
class chessboard {
private:
	chess* c[10][9];//用指针调用各个棋子，实现多态//继续用
	
public:
	chessboard() { memset(c, NULL, sizeof(c)); };//把指针初始化为零指针
	void before();//完成初始棋子摆盘
	chess* get(int x, int y) { return c[x][y]; } //输出x，y处的棋子信息
	int getid(int x, int y) { if (c[x][y] != NULL) return c[x][y]->get(); return 0; } //输出x，y处的棋子的id号码
    int getgrade(int x,int y) { if (c[x][y] != NULL) return c[x][y]->getgrade(); return 0; }//输出x，y处的棋子的grade等级
    void showword(int x, int y) const;
    void upgrade(int x, int y) { c[x][y]->upgrade(); }
	void show()const;//输出棋盘的展示//修改好了
	void play();//开始执行游戏
    void txt();
	bool move(int startx, int starty, int aimx, int aimy);//进行一步动子//继续用
	~chessboard();
	static bool end;//判断是否结束
	static int player;//判断哪方落子用的参数
};
bool chessboard::end = true;
int chessboard::player = -1;
//鼠标交互功能实现
void getstart(int& a, int& b)
{
    MOUSEMSG m;
    while (1) {
        m = GetMouseMsg();
        if ((m.x > 70 && m.x < 410 && m.y>60 && m.y < 422))// && ((m.x - 80) % 40 >= 30 || (m.x - 80) % 40 <= 20) && ((m.y - 70) % 38 >= 28 && (m.y - 70) % 38 <= 10))
        {
            int mx = (m.x - 80 + 10) / 40;
            int my = (m.y - 70 + 10) / 38;
            setlinecolor(BLUE);
            rectangle(90 + 40 * mx, 60 + 38 * my, 70 + 40 * mx, 80 + 38 * my);
            if (m.uMsg == WM_LBUTTONDOWN)
            {		//如果按下鼠标左键实现相应功能.
                a = mx;
                b = my;
                IMAGE qd;
                loadimage(&qd, L"起点.jpg", 40, 40);
                putimage(500, 100, &qd);
                break;
            }

            else {	//当鼠标不在目标位置就覆盖之前的蓝色边框.
                setlinecolor(WHITE);
                rectangle(90 + 40 * mx, 60 + 38 * my, 70 + 40 * mx, 80 + 38 * my);
            }
        }
    }

}
void getaim(int& a, int& b)
{
    MOUSEMSG m;
    while (1) {
        m = GetMouseMsg();
        if ((m.x > 70 && m.x < 410 && m.y>60 && m.y < 422))// && ((m.x - 80) % 40 >= 30 || (m.x - 80) % 40 <= 20) && ((m.y - 70) % 38 >= 28 && (m.y - 70) % 38 <= 10))
        {
            int mx = (m.x - 80 + 10) / 40;
            int my = (m.y - 70 + 10) / 38;
            setlinecolor(BLUE);
            rectangle(90 + 40 * mx, 60 + 38 * my, 70 + 40 * mx, 80 + 38 * my);
            if (m.uMsg == WM_LBUTTONDOWN)
            {		//如果按下鼠标左键实现相应功能.
                a = mx;
                b = my;
                IMAGE zd;
                loadimage(&zd, L"终点.jpg", 40, 40);
                putimage(500, 100, &zd);
                break;
            }

            else {	//当鼠标不在目标位置就覆盖之前的蓝色边框.
                setlinecolor(WHITE);
                rectangle(90 + 40 * mx, 60 + 38 * my, 70 + 40 * mx, 80 + 38 * my);
            }
        }
    }
}
//走棋功能的实现
bool chessboard::move(int startx, int starty, int aimx, int aimy) {
    if (startx == aimx && starty == aimy && getgrade(startx, starty) == 0 && getid(startx, starty) * player > 0 
        &&(abs(getid(startx,starty))==1|| getid(startx, starty) ==-5 || getid(startx, starty) ==-2 || getid(startx, starty) == 3 || 
             abs(getid(startx, starty)) == 6 || abs(getid(startx, starty)) == 4))
    {
        IMAGE NQDM;
        loadimage(&NQDM, L"你确定吗.jpg", 120, 30);
        putimage(480, 0, &NQDM);
        IMAGE QD;
        loadimage(&QD, L"确定.jpg", 40, 25);
        putimage(480, 40, &QD);
        IMAGE BQD;
        loadimage(&BQD, L"不确定.jpg", 50, 20);
        putimage(480, 70, &BQD);
        MOUSEMSG m;
        while (1)
        {
            m = GetMouseMsg();
            if ((m.x > 480 && m.x < 520 && m.y>40 && m.y < 65))
            {
                setfillcolor(BLUE);
                fillrectangle(480, 40, 520, 65);
                if (m.uMsg == WM_LBUTTONDOWN)
                {		//如果按下鼠标左键实现相应功能.
                    upgrade(startx, starty);
                    player *= -1;//交换动子权
                    return true;
                }
            }
            else if (m.x > 480 && m.x < 530 && m.y>70 && m.y < 90)
            {
                setfillcolor(BLUE);
                fillrectangle(480, 70, 530, 90);
                if (m.uMsg == WM_LBUTTONDOWN)
                {		//如果按下鼠标左键实现相应功能. 
                    return false;
                }
            }
        }
    }
    if (startx >= 0 && startx < 10 && starty >= 0 && starty < 9//初步判断传入的点是否符合规则
        && aimx >= 0 && aimx < 10 && aimy >= 0 && aimy < 9//判断希望落子的点是否符合规则
        && getid(startx, starty)//判断这个位置是否有棋子
        && getid(startx, starty) * player>0//判断是否是这一方落子
        && c[startx][starty]->judge_move(*this, startx, starty, aimx, aimy))//判断是否符合棋子的规则，即判断这一步能不能走
    {
        if (c[aimx][aimy] != NULL)
            delete c[aimx][aimy];//吃子
        c[aimx][aimy] = c[startx][starty];//这个才是代表了动子，棋子中的那个只是代表了判断是否可以走
        c[startx][starty] = NULL;
        player *= -1;//交换动子权
        this->txt();
        return true;
    }
    return false;
}
//棋子的实现
//中国象棋一方。
class horse : public chess //马的具体实现
{
public:
    horse() : chess(-2, 0) {};//给对象马设定编号
    virtual bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override//这个函数就是用来判断是否可以走的函数
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
        if (grade == 0)
        {
            if (sid * aid <= 0 && (tempx * tempx + tempy * tempy == 5) && !cb.get(startx + tempx / 2, starty + tempy / 2))
                return true;
            else
                return false;
        }
        else
        {
            if (sid * aid <= 0 && (tempx * tempx + tempy * tempy == 5) )
                return true;
            else
                return false;
        }
    }
};

class soldier : public chess //兵（卒）的实现
{
public:
    soldier() : chess(-1, 0) {};//给卒定好编号
    virtual bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
        if (grade == 0)
        {
            if (sid * aid <= 0 && sid * tempx <= 0)
            {
                if (abs(tempx) == 1 && tempy == 0)
                    return true;
                if (abs(tempy) == 1 && tempx == 0)
                    if ((startx / 5 == 0 && sid > 0) || (startx / 5 == 1 && sid < 0)) //判断是否过河
                        return true;
                return false;
            }
            return false;
        }
        else
        {
            if (sid * aid <= 0 && sid * tempx <= 0)
            {
                if (((abs(tempx) == 1|| abs(tempx) == 2) && tempy == 0))
                    return true;
                if ((abs(tempy) == 1 && tempx == 0)||(abs(tempy) == 2 && tempx == 0))
                    if (startx / 5 == 1 && sid < 0)//判断是否过河
                        return true;
                return false;
            }
            return false;
        }
                
    }
};

class general : public chess //将
{
public:
    general() : chess(-7, 0) {};
    virtual  bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
    
            if (sid * aid <= 0 && tempy * tempy + tempx * tempx == 1 && aimx % 7 >= 0 && aimx % 7 <= 2 && aimy >= 3 && aimy <= 5)
                return true;
            return false;
    }
    ~general() {
        ofstream fout("recordwinner.txt");
        int a = -1;
        fout.clear();
        fout << a;
        fout.close();
        chessboard::end = false; }
};

class minister : public chess//相
{
public:
    minister() : chess(-6, 0) {}
    virtual bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
        if (grade == 0)
        {
            if (sid * aid <= 0 && tempy * tempy + tempx * tempx == 8 && startx / 5 == aimx / 5 && !cb.get(startx + tempx / 2, starty + tempy / 2))
                return true;
            return false;
        }
        else
        {
            if (sid * aid <= 0 && tempy * tempy + tempx * tempx == 8 && startx / 5 == aimx / 5 )//不绊脚
                return true;
            return false;
        }

    }
};

class cannon : public chess//炮
{
public:
    cannon() : chess(-5, 0) {}
    virtual bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
        if (grade == 0)
        {
            if (sid * aid <= 0 && !(tempx && tempy) && (tempx + tempy))
            {
                int tot = 0;
                if (tempx != 0)
                {
                    int sign = tempx > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempx); i++)
                        if (cb.get(startx + sign * i, starty)) tot++;
                }
                else
                {
                    int sign = tempy > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempy); i++)
                        if (cb.get(startx, starty + sign * i)) tot++;
                }
               
                    if (tot == 1&&aid!=0) return true;//路上有一个子
                
            }
            return false;
        }
        else
        {
            if (sid * aid <= 0 && !(tempx && tempy) && (tempx + tempy))
            {
                int tot = 0;
                if (tempx != 0)
                {
                    int sign = tempx > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempx); i++)
                        if (cb.get(startx + sign * i, starty)) tot++;
                }
                else
                {
                    int sign = tempy > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempy); i++)
                        if (cb.get(startx, starty + sign * i)) tot++;
                }
                //判断路上有没有东西
                if (!aid)//路上啥也没有，终点也啥也没有
                {
                    if (!tot) return true;
                }
                else
                {
                    if (tot == 1) return true;//路上有一个子且终点有子
                }
            }
            return false;
        }
        return false;
    }
};

class guard : public chess //仕
{
public:
    guard() : chess(-4, 0) {}
    virtual bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
        if (grade == 0)
        {
            if (sid * aid <= 0 && abs(tempy)+ abs(tempx) == 2 && aimx % 7 >= 0 && aimx % 7 <= 2 && aimy >= 3 && aimy <= 5)
                return true;
            return false;
        }
        else
        {
            if (sid * aid <= 0 && (tempy * tempy + tempx * tempx == 2 || tempy * tempy + tempx * tempx == 8)&& aimx % 7 >= 0 && aimx % 7 <= 2 && aimy >= 3 && aimy <= 5)
                return true;
            return false;
        }

    }
};

class rook : public chess //车
{
public:
    rook() : chess(-3, 0) {}
    virtual bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
       
            if (sid * aid <= 0 && !(tempx && tempy) && (tempx + tempy))
            {
                if (tempx != 0) {
                    int sign = tempx > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempx); i++)  //abs是取绝对值
                        if (cb.get(startx + sign * i, starty)) return false;//路径上有东西就报错
                }
                else {
                    int sign = tempy > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempy); i++)
                        if (cb.get(startx, starty + sign * i)) return false;
                }
                return true;
            }
            return false;
      
    
    }
};
//国际象棋一方。
class horse1 :public chess
{
public:
    horse1() :chess(2, 0) {};
    virtual  bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override//这个函数就是用来判断是否可以走的函数
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
       
            if (sid * aid <= 0 && (tempx * tempx + tempy * tempy == 5))//不别马脚
                return true;
            return false;
    }
};

class king :public chess
{
public:
    king() :chess(7, 0) {};
    virtual  bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
            if (sid * aid <= 0)
            {
                if (abs(tempx)* abs(tempx) + abs(tempy)* abs(tempy) == 2)
                    return true;
                if (abs(tempx)==1 && tempy == 0)
                    return true;
                if (abs(tempy) == 1 && tempx == 0)
                    return true;
                return false;
            }
            return false;
    }
    ~king() { 
        ofstream fout("recordwinner.txt");
        int a = 1;
        fout.clear();
        fout << a;
        fout.close();
        chessboard::end = false; }
};

class queen : public chess //后
{
public:
    queen() : chess(4, 0) {}
    virtual   bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
        if (grade == 0)
        {
            if (sid * aid <= 0)
            {
                if (abs(tempx) == 0 && abs(tempy) > 0&&abs(tempy)<4)
                {
                        int sign = tempy > 0 ? 1 : -1;
                        for (int i = 1; i < abs(tempy); i++)  //abs是取绝对值
                        {
                            if (cb.get(startx, starty + sign * i))
                                return false;//路径上有东西就报错
                        }
                        return true;
                }
                if (abs(tempy) == 0 && abs(tempx) > 0&&abs(tempx)<4)
                {
                    int sign = tempx > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempx); i++)  //abs是取绝对值
                    {
                        if (cb.get(startx + sign * i, starty ))
                            return false;//路径上有东西就报错
                    }
                    return true;
                }
                if (abs(abs(tempy)-abs(tempy)) == 0 && abs(tempx) > 0&&abs(tempx)<4)
                {
                    int sign1 = tempx > 0 ? 1 : -1;
                    int sign2 = tempy > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempx); i++)  //abs是取绝对值
                    {
                        if (cb.get(startx + sign1 * i, starty + sign2* i))
                            return false;//路径上有东西就报错
                    }
                    return true;
                }
                return false;
           }
            return false;
        }
        else
        {
            if (sid * aid <= 0)
            {
                if (abs(tempx) == 0 && abs(tempy) > 0 && abs(tempy) < 7)
                {
                    int sign = tempy > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempy); i++)  //abs是取绝对值
                    {
                        if (cb.get(startx, starty + sign * i))
                            return false;//路径上有东西就报错
                    }
                    return true;
                }
                if (abs(tempy) == 0 && abs(tempx) > 0 && abs(tempx) < 7)
                {
                    int sign = tempx > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempx); i++)  //abs是取绝对值
                    {
                        if (cb.get(startx + sign * i, starty))
                            return false;//路径上有东西就报错
                    }
                    return true;
                }
                if (abs(abs(tempy) - abs(tempy)) == 0 && abs(tempx) > 0 && abs(tempx) < 7)
                {
                    int sign1 = tempx > 0 ? 1 : -1;
                    int sign2 = tempy > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempx); i++)  //abs是取绝对值
                    {
                        if (cb.get(startx + sign1 * i, starty + sign2 * i))
                            return false;//路径上有东西就报错
                    }
                    return true;
                }
                return false;
            }
            return false;
        }

    }
};

class elephant : public chess
{
public:
    elephant() :chess(6, 0) {};
    virtual  bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
        if (grade == 0)
        {
            if (sid * aid <= 0)
            {
                if (abs(abs(tempx) - abs(tempy)) == 0 && abs(tempx) > 0&&abs(tempx)<4)
                {
                    int sign1 = tempx > 0 ? 1 : -1;
                    int sign2 = tempy > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempx); i++)  //abs是取绝对值
                    {
                        if (cb.get(startx + sign1 * i, starty + sign2 * i))
                            return false;//路径上有东西就报错
                    }
                    return true;
                }
            }
            return false;
        }
        else
        {
            if (sid * aid <= 0)
            {
                if (abs(abs(tempx) - abs(tempy)) == 0 && abs(tempx) > 0)
                {
                    int sign1 = tempx > 0 ? 1 : -1;
                    int sign2 = tempy > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempx); i++)  //abs是取绝对值
                    {
                        if (cb.get(startx + sign1 * i, starty + sign2 * i))
                            return false;//路径上有东西就报错
                    }
                    return true;
                }
            }
            return false;
        }
    }
};

class soldier1 : public chess //
{
public:
    soldier1() : chess(1, 0) {};//给卒定好编号
    virtual bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
        if (grade == 0)
        {
            if (startx == 8)
            {
                if (aid == 0 && (abs(tempx) == 1 || abs(tempx) == 2)&& abs(tempy)==0)
                    return true;
                if (aid < 0 && abs(tempx) == 1 && abs(tempy) == 1)
                    return true;
                return false;
            }
            if (startx < 8)
            {
                if ((aid == 0 && tempx == -1&&tempy==0)|| (aid < 0 && tempx == -1&&abs(tempy)==1))
                    return true;
                return false;
            }
            return false;
        }
        else
        {
            if (startx == 8)
            {
                if (aid == 0 && (abs(tempx) == 1 || abs(tempx) == 2) && abs(tempy) == 0)
                    return true;
                if (aid < 0 &&abs(tempx) == 1 && abs(tempy) == 1)
                    return true;
                return false;
            }
            if (startx < 8)
            {
                if ((aid == 0 && tempx == -1 && tempy == 0) || (aid < 0 && tempx == -1 && abs(tempy) == 1)|| (aid < 0 && tempx == -1 && abs(tempy) == 0))
                    return true;
                return false;
            }
            return false;
        }
    }
};

class rook1 : public chess //车
{
public:
    rook1() : chess(3, 0) {}
    virtual  bool judge_move(chessboard& cb, int startx, int starty, int aimx, int aimy) override
    {
        int tempx = aimx - startx, tempy = aimy - starty;
        int sid = cb.getid(startx, starty), aid = cb.getid(aimx, aimy);
        if (grade == 0)
        {
            if (sid * aid <= 0 && !(tempx && tempy) && (tempx + tempy)&&abs((tempx + tempy))<=5) {
                if (tempx != 0) {
                    int sign = tempx > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempx); i++)
                        if (cb.get(startx + sign * i, starty)) return false;
                }
                else {
                    int sign = tempy > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempy); i++)
                        if (cb.get(startx, starty + sign * i)) return false;
                }
                return true;
            }
            return false;
        }
        else
        {
            if (sid * aid <= 0 && !(tempx && tempy) && (tempx + tempy)) {
                if (tempx != 0) {
                    int sign = tempx > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempx); i++)
                        if (cb.get(startx + sign * i, starty)) return false;
                }
                else {
                    int sign = tempy > 0 ? 1 : -1;
                    for (int i = 1; i < abs(tempy); i++)
                        if (cb.get(startx, starty + sign * i)) return false;
                }
                return true;
            }
            return false;
        }
    }
};


void chessboard::before()
{
    c[0][0] = new rook();        c[0][8] = new rook();
    c[0][1] = new horse();       c[0][7] = new horse();
    c[0][2] = new minister();    c[0][6] = new minister();
    c[0][3] = new guard();       c[0][5] = new guard();
    c[0][4] = new general();     c[9][4] = new king();
    c[2][1] = new cannon();      c[2][7] = new cannon();
    c[3][0] = new soldier();     c[3][2] = new soldier();
    c[3][4] = new soldier();     c[3][6] = new soldier();
    c[3][8] = new soldier();     c[8][0] = new soldier1();
    c[8][1] = new soldier1();     c[8][2] = new soldier1();
    c[8][3] = new soldier1();     c[8][4] = new soldier1();
    c[8][5] = new soldier1();     c[8][6] = new soldier1();
    c[8][7] = new soldier1();     c[8][8] = new soldier1();
    c[9][0] = new rook1();        c[9][8] = new rook1();
    c[9][1] = new horse1();       c[9][7] = new horse1();
    c[9][2] = new elephant();    c[9][6] = new elephant();
    c[9][3] = new queen();       c[9][5] = new queen();
}//棋子的初始摆盘。
ofstream os("record.txt", ios_base::out | ios_base::binary);
//下面两个是一起实现记录到txt文档的功能
void chessboard::showword(int x, int y) const {
    int a = c[x][y]->get();
    switch (a) {
    case -7: os.write("帅", sizeof("帅"));break;
    case -6:os.write("象", sizeof("象")); break;
    case -5: os.write("炮", sizeof("炮")); break;
    case -4: os.write("仕", sizeof("仕"));break;
    case -3:os.write("車", sizeof("車"));break;
    case -2: os.write("馬", sizeof("馬")); break;
    case -1:os.write("卒", sizeof("卒"));break;
    case 1:os.write("兵", sizeof("兵"));break;
    case 2:os.write("马", sizeof("马"));break;
    case 3:os.write("车", sizeof("车"));break;
    case 4:os.write("后", sizeof("后"));break; 
    case 6:os.write("相", sizeof("相"));break; 
    case 7:os.write("王", sizeof("王"));break;
    }
}
void chessboard::txt()
{
    
    
    for (int i = 0; i < 10; i++)
    {
        if (i == 5)
            os.write("    ——楚 河   汉 界———\n", sizeof("    ——楚 河   汉 界———\n"));
        for (int j = 0; j < 9; j++)
        {
            if (c[i][j] != NULL)
                showword(i, j);
                //os.write("子", sizeof("子"));
            else if ((i == 1 && j == 4) || (i == 8 && j == 4))
                os.write("米", sizeof("米"));
            else
                os.write("十", sizeof("十"));
        }
        os.write("\n", sizeof("\n"));
    }
    os.write("\n", sizeof("\n"));
    os.write("\n", sizeof("\n"));
}//用来记录（主要是为了使用输出功能）

//下面两个是为了一起实现画面的输出功能
void drawchess(int x, int y, int id)
{
    int mbx = 60 + 40 * x;
    int mby = 50 + 38 * y;

    IMAGE* k;

    IMAGE _7;loadimage(&_7, L"-7.jpg", 40, 40);
    IMAGE _6;loadimage(&_6, L"-6.jpg", 40, 40);
    IMAGE _26; loadimage(&_26, L"-26.jpg", 40, 40);
    IMAGE _5;loadimage(&_5, L"-5.jpg", 40, 40);
    IMAGE _25; loadimage(&_25, L"-25.jpg", 40, 40);
    IMAGE _4;loadimage(&_4, L"-4.jpg", 40, 40);
    IMAGE _24; loadimage(&_24, L"-24.jpg", 40, 40);
    IMAGE _3;loadimage(&_3, L"-3.jpg", 40, 40);
    IMAGE _2;loadimage(&_2, L"-2.jpg", 40, 40);
    IMAGE _22; loadimage(&_22, L"-22.jpg", 40, 40);
    IMAGE _1;loadimage(&_1, L"-1.jpg", 40, 40);
    IMAGE _21; loadimage(&_21, L"-21.jpg", 40, 40);
    IMAGE z7;loadimage(&z7, L"7.jpg", 40, 40);
    IMAGE z6;loadimage(&z6, L"6.jpg", 40, 40);
    IMAGE z26; loadimage(&z26, L"26.jpg", 40, 40);
    IMAGE z4;loadimage(&z4, L"4.jpg", 40, 40);
    IMAGE z24; loadimage(&z24, L"24.jpg", 40, 40);
    IMAGE z3;loadimage(&z3, L"3.jpg", 40, 40);
    IMAGE z23; loadimage(&z23, L"23.jpg", 40, 40);
    IMAGE z2;loadimage(&z2, L"2.jpg", 40, 40);
    IMAGE z1;loadimage(&z1, L"1.jpg", 40, 40);
    IMAGE z21; loadimage(&z21, L"21.jpg", 40, 40);
    switch (id)
    {
    case 15:
        k = &_25;
        break;
    case 14:
        k = &_26;
        break;
    case 19:
        k = &_21;
        break;
    case 16:
        k = &_24;
        break;
    case 18:
        k = &_22;
        break;
    case 23:
        k = &z23;
        break;
    case 24:
        k = &z24;
        break;
    case 26:
        k = &z26;
        break;
    case 21:
        k = &z21;
        break;
    case -7:
        k = &_7;
        break;
    case -6:
        k = &_6;
        break;
    case -5:
        k = &_5;
        break;
    case -4:
        k = &_4;
        break;
    case -3:
        k = &_3;
        break;
    case -2:
        k = &_2;
        break;
    case -1:
        k = &_1;
        break;
    case 7:
        k = &z7;
        break;
    case 6:
        k = &z6;
        break;
    case 4:
        k = &z4;
        break;
    case 3:
        k = &z3;
        break;
    case 2:
        k = &z2;
        break;
    case 1:
        k = &z1;
        break;
    default:
        k = &_7;
        break;
    }
    putimage(mbx, mby, k);
}
void chessboard::show() const//棋盘的展示
{
    IMAGE chessboard;
    loadimage(&chessboard, L"chessboard.jpg", 480, 480);
    putimage(0, 0, &chessboard);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (c[i][j] != NULL)
                drawchess(j, i, c[i][j]->get()+20* c[i][j]->getgrade());
        }
    }
}

//不断运行的play程序
void chessboard::play()//开玩
{
    this->before();
    this->show();
    do {
        int startx, starty, aimx, aimy;
        int sid, aid;
        do {
            sid = aid = 0;//传给move中要用
            getstart(starty, startx);
            getaim(aimy, aimx);
        } while (!this->move(startx, starty, aimx, aimy));//直到输出是ture为止
        cleardevice();
        this->show();
       
    } while (chessboard::end);
   
}

//最后写个析构函数
chessboard :: ~chessboard() {
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
            if (c[i][j] != NULL) {
                delete c[i][j];
                c[i][j] = NULL;
            }
}//逐一删除，释放空间。

int main()
{
    ofstream os("test2.txt", ios_base::out | ios_base::binary);
	initgraph(640, 480, NULL);
        chessboard C;
        ifstream fin("recordwinner.txt");
        int record;
        fin >> record;
        C.player = record;
        C.play();

	closegraph();
}
