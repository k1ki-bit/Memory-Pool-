#define _CRT_SECURE_NO_WARNINGS //这一句必须放在第一行
#include <stdio.h> //标准输入输出文件
#include <stdlib.h>

//不带头节点的首次拟合法
typedef struct WORD {//字
    union {
        WORD* llink; //头部域,指向前驱结点
        WORD* uplink;//底部域,指向本结点头部
    };
    int tag;//标识,0空闲,1占用,头部和尾部都有
    int size;//头部域,块大小,以WORD为单位
    WORD* rlink;//头部域,指向后继结点
}WORD, * Space;//Space:可利用空间指针类型

#define SIZE 10000  //内存池的大小(WORD)
#define e 10 //碎片临界值,当剩余的空闲块小于e时,整个空闲块都分配出去

static Space FootLoc(Space p)//通过p返回p的尾
{
    return p + p->size - 1;
}

Space InitMem()//初始化内存池
{
    Space pav = (Space)malloc(SIZE * sizeof(WORD));//内存池
    //处理pav的头
    pav->llink = pav;
    pav->rlink = pav;
    pav->tag = 0;
    pav->size = SIZE;

    //处理p的尾
    Space p = FootLoc(pav);//p指向尾
    p->uplink = pav;
    p->tag = 0;

    return pav;
}

//向内存池pav,申请n个WORD,成功返回申请内存的地址,失败返回NULL
//利用首次拟合法
WORD* MyMalloc(Space* pav, int n)
{
    if (*pav == NULL)//内存池已经空了
        return NULL;
    
    Space p = *pav;
    do //首次拟合法,找第一个符合条件的空闲块
    {
        if (p->size >= n)//找到了
            break;
    } while (p != *pav);

    if (p->size < n)//没有满足条件的空闲块
        return NULL;

    if (p->size - n < e)//整个空闲块都分配
    {
        WORD* q = p;
        //q->llink,q->rlink,占用块不需要处理
        q->tag = 1;//占用
        //q->size 不改变
        p = FootLoc(p);//尾部
        //p->uplink不改变
        p->tag = 1;//占用

        //把q从链表中剔除
        if (q->rlink == q)//空闲链表中只有q这一个结点
            *pav = NULL;
        else//有多个结点
        {
            *pav = q->rlink;//头指针指向下一个结点

            q->llink->rlink = q->rlink;
            q->rlink->llink = q->llink;
        }

        return q;
    }
    else //空闲块一部分分配出去,把下面(高地址)分出去
    {
        *pav = p->rlink;//头指针指向下一个结点
        p->size -= n; //p分割后的新大小
        WORD *p2 = FootLoc(p);//新结点的尾
        p2->uplink = p;
        p2->tag = 0;

        //处理占用块
        WORD* q = p2+1;//指向需要返回的地址
        q->tag = 1;//占用
        q->size = n;//申请的大小
        p2 = FootLoc(q);//q的尾巴
        p2->uplink = q;
        p2->tag = 1;

        return q;
    }
}
void Show(WORD* p)//测试函数,输出p的数据
{
    printf("p的地址:%d,p->tag:%d,p->size:%d(WORD),p的尾巴的tag:%d\n",
        p,p->tag,p->size,FootLoc(p)->tag);
}

int main()
{
    Space pav = InitMem();//创建并初始化好的内存池
    WORD* p1 = MyMalloc(&pav, 1000);
    WORD* p2 = MyMalloc(&pav, 1500);
    WORD* p3 = MyMalloc(&pav, 500);
    WORD* p4 = MyMalloc(&pav, 1000);
    WORD* p5 = MyMalloc(&pav, 1000);

    Show(p1);
    Show(p2);
    Show(p3);
    Show(p4);
    Show(p5);

	return 0;
}




