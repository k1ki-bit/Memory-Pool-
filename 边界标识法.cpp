#define _CRT_SECURE_NO_WARNINGS //��һ�������ڵ�һ��
#include <stdio.h> //��׼��������ļ�
#include <stdlib.h>

//����ͷ�ڵ���״���Ϸ�
typedef struct WORD {//��
    union {
        WORD* llink; //ͷ����,ָ��ǰ�����
        WORD* uplink;//�ײ���,ָ�򱾽��ͷ��
    };
    int tag;//��ʶ,0����,1ռ��,ͷ����β������
    int size;//ͷ����,���С,��WORDΪ��λ
    WORD* rlink;//ͷ����,ָ���̽��
}WORD, * Space;//Space:�����ÿռ�ָ������

#define SIZE 10000  //�ڴ�صĴ�С(WORD)
#define e 10 //��Ƭ�ٽ�ֵ,��ʣ��Ŀ��п�С��eʱ,�������п鶼�����ȥ

static Space FootLoc(Space p)//ͨ��p����p��β
{
    return p + p->size - 1;
}

Space InitMem()//��ʼ���ڴ��
{
    Space pav = (Space)malloc(SIZE * sizeof(WORD));//�ڴ��
    //����pav��ͷ
    pav->llink = pav;
    pav->rlink = pav;
    pav->tag = 0;
    pav->size = SIZE;

    //����p��β
    Space p = FootLoc(pav);//pָ��β
    p->uplink = pav;
    p->tag = 0;

    return pav;
}

//���ڴ��pav,����n��WORD,�ɹ����������ڴ�ĵ�ַ,ʧ�ܷ���NULL
//�����״���Ϸ�
WORD* MyMalloc(Space* pav, int n)
{
    if (*pav == NULL)//�ڴ���Ѿ�����
        return NULL;
    
    Space p = *pav;
    do //�״���Ϸ�,�ҵ�һ�����������Ŀ��п�
    {
        if (p->size >= n)//�ҵ���
            break;
    } while (p != *pav);

    if (p->size < n)//û�����������Ŀ��п�
        return NULL;

    if (p->size - n < e)//�������п鶼����
    {
        WORD* q = p;
        //q->llink,q->rlink,ռ�ÿ鲻��Ҫ����
        q->tag = 1;//ռ��
        //q->size ���ı�
        p = FootLoc(p);//β��
        //p->uplink���ı�
        p->tag = 1;//ռ��

        //��q���������޳�
        if (q->rlink == q)//����������ֻ��q��һ�����
            *pav = NULL;
        else//�ж�����
        {
            *pav = q->rlink;//ͷָ��ָ����һ�����

            q->llink->rlink = q->rlink;
            q->rlink->llink = q->llink;
        }

        return q;
    }
    else //���п�һ���ַ����ȥ,������(�ߵ�ַ)�ֳ�ȥ
    {
        *pav = p->rlink;//ͷָ��ָ����һ�����
        p->size -= n; //p�ָ����´�С
        WORD *p2 = FootLoc(p);//�½���β
        p2->uplink = p;
        p2->tag = 0;

        //����ռ�ÿ�
        WORD* q = p2+1;//ָ����Ҫ���صĵ�ַ
        q->tag = 1;//ռ��
        q->size = n;//����Ĵ�С
        p2 = FootLoc(q);//q��β��
        p2->uplink = q;
        p2->tag = 1;

        return q;
    }
}
void Show(WORD* p)//���Ժ���,���p������
{
    printf("p�ĵ�ַ:%d,p->tag:%d,p->size:%d(WORD),p��β�͵�tag:%d\n",
        p,p->tag,p->size,FootLoc(p)->tag);
}

int main()
{
    Space pav = InitMem();//��������ʼ���õ��ڴ��
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




