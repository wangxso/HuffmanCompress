/*
 * @author wangxinsheng
 * created 2018-12-17
 *
 */
#include <iostream>
#include <ctime>
#include <cstring>
#include <unistd.h>
#define MAXN 512
#define OK 1
#define ERROR 0
typedef int Status;
typedef struct  huffmanode{
    long weight;
    //权值
    unsigned char data;
    //字符数据
    int parent;
    //双亲结点
    int lchild;
    //左孩子
    int rchild;
    //右孩子
    char code[MAXN];
    //编码的位数
    int codeLength;
}HuffmanNode;
/*
 * itoa函数补全linux下缺失的
 * itoa
 * itoa的原型
 */
char *itoa(int num,char*str,int radix)
{
    /*索引表*/
    char index[]="0123456789ABCDEF";
    unsigned unum;/*中间变量*/
    int i=0,j,k;
    /*确定unum的值*/
    if(radix==10&&num<0)/*十进制负数*/
    {
        unum=(unsigned)-num;
        str[i++]='-';
    }
    else
        unum=(unsigned)num;/*其他情况*/
    /*转换*/
    do
    {
        str[i++]=index[unum%(unsigned)radix];
        unum/=radix;
    }
    while(unum);
    str[i]='\0';
    /*逆序*/
    if(str[0]=='-')
        k=1;/*+进制-数*/
    else
        k=0;
    char temp;
    for(j=k; j<=(i-1)/2; j++)
    {
        temp=str[j];
        str[j]=str[i-1+k-j];
        str[i-1+k-j]=temp;
    }
    return str;
}
/**
 * 一个假的进度条
 */
void toolBar()
{
    char bar[102] = {0};
    std::string lab = "-\\|/";
    int i=0;
    while(i<=100){
        printf("[%-101s][%d%%][%c]\r",bar,i,lab[i%4]);
        fflush(stdout);
        usleep(50000);
        bar[i++] = '#';
        bar[i] = '\0';
    }
    printf("\n");
}
HuffmanNode huffmanNodes[MAXN];
/**
 * 选择权值最小的结点的下标
 * @param node 具有权值的哈夫曼树
 * @param pointNumber 当前的下标
 * @return 权值最小节点的下标
 */
int selectMin(huffmanode *node,int pointNumber)
{
    int i;
    int index;
    long min = 6666666666;//赋值一个极大的数字
    for(i=0; i<=pointNumber; i++)
    {
        //有父节点则不进行比较,已经在树中
        if(node[i].parent != -1)
        {
            continue;
        }
        if(node[i].weight < min)
        {
            min = node[i].weight;
            index = i;
        }
    }
    return index;
}
/**
 * 编码程序
 * @param node 构造成的haffuman树
 * @param nodeNumber 结点数量
 * @return 程序运行完成
 */
Status enCode(HuffmanNode *node,int nodeNumber) {
    int start;//标记编码起始点
    int i;//循环变量
    int j;
    int parent;//临时保存父结点下标
    char codes[MAXN];//临时保存编码
    codes[nodeNumber - 1] = '\0';//编码结束标识符
    for (i = 0; i < nodeNumber; i++) {//逐个字符的求哈夫曼码
        start = nodeNumber - 1;//逆序求编码
        parent = node[i].parent;
        for (j = i; parent != -1; j = parent, parent = node[parent].parent) {
            start--;
            if (node[parent].lchild == j) {
                codes[start] = '0';
            } else {
                codes[start] = '1';
            }
        }
        strcpy(node[i].code, &codes[start]);//保存编辑好的编码
        node[i].codeLength = (int)strlen(node[i].code) ;//保存编码长度
    }
    return OK;
}
/**
 * 选择是否显示详细内容
 * @param choice 选择项
 * @param n 叶子节点数
 * @param m 节点数
 */
void choosing(char choice,int n,int m){
    getchar();
    getchar();

    printf("编码完成,是否查看具体信息(Y/N)\n");
    scanf("%c",choice);
    printf("\n叶子节点个数:%d,节点数为%d.\n",n,m);
    for(int i=0; i<n; i++)
    {
        printf("%d号叶子结点的权值为：%ld,双亲为：%d,左孩子：%d,右孩子：%d,编码为：%s 数据为:%c\n",
               i,huffmanNodes[i].weight,huffmanNodes[i].parent,
               huffmanNodes[i].lchild,huffmanNodes[i].rchild,huffmanNodes[i].code,
               huffmanNodes[i].data);
    }
}
/**
 *创建树
 */
Status createHuffmanNode(int &n){
    //叶子节点初值
    n=0;
    //将ASCII码转换为字符存入到结点的data成员里,同时给双亲和孩子赋初值-1
    for(int i=0; i<MAXN; i++)
    {
        if(huffmanNodes[i].weight!=0)
        {
            huffmanNodes[i].data = (unsigned char)i;
            //存在该节点 叶子节点数加一
            n++;
            huffmanNodes[i].lchild=-1;
            huffmanNodes[i].rchild=-1;
            huffmanNodes[i].parent=-1;
        }
    }
    //哈夫曼树的结点总数
    int m = 2*n-1;
    int position=0;
    //去掉权值为0的结点
    for(int i=0; i<MAXN; i++)
    {
        if(huffmanNodes[i].weight!=0)
        {
            huffmanNodes[position] = huffmanNodes[i];
            position++;
        }
    }
    //初始化根节点
    for(int i=n; i<m; i++)// 1 --> n 保存叶子结点数据，没必要重复计算
    {
        huffmanNodes[i].lchild=-1;
        huffmanNodes[i].rchild=-1;
        huffmanNodes[i].parent=-1;
    }
    //建立哈夫曼树
    for(int i=n; i<m; i++)
    {
        int index_1 = selectMin(huffmanNodes,i-1);
        huffmanNodes[i].lchild=index_1;
        huffmanNodes[index_1].parent=i;

        int index_2 = selectMin(huffmanNodes,i-1);
        huffmanNodes[i].rchild=index_2;
        huffmanNodes[index_2].parent=i;

        huffmanNodes[i].weight = huffmanNodes[index_1].weight + huffmanNodes[index_2].weight;
    }
    return OK;
}
void comPress()
{
    int i;
    int j;
    char filePath[100];
    char destPath[100];
    FILE * file_fp;
    FILE * dest_fp;
    unsigned char ch;
    char choice;
    //文件字节数
    long FileLength;
    long fileLength;
    //叶子数
    int n;
    //节点数
    int m;
    //存放哈夫曼编码
    char codes[MAXN];
    long sumLength=0;
    //下面是记录程序运行效果的变量
    float rate;//压缩率
    float speed;//压缩速率
    int count = 0;
    clock_t start_1;//记录时间计算速率等
    clock_t end_1;
    clock_t start_2;
    clock_t end_2;
    double interval_1;
    double interval_2;

    ///////////////////////////////////////////////////////////////

    printf("请输入要压缩的文件名称：");
    scanf("%s",filePath);
    file_fp = fopen(filePath,"rb");
    if(file_fp==NULL)
    {
        printf("文件名输入错误,文件不存在！\n");
        return;
    }
    printf("请输入压缩后的文件名称：");
    scanf("%s",destPath);
    dest_fp = fopen(destPath,"wb");
    if(dest_fp==NULL)
    {
        printf("文件名输入错误,文件不存在！\n");
        return;
    }

    /////////////////////////////////////////////////////////////
    //文件操作
    /////////////////////////////////////////////////////////////
    start_1 = clock();
    FileLength = 0;
    //统计文件中各类字符的个数,用字符的ASCII码统计
    //优化二叉排序树可达到算法复杂度O(logn)-O(n)，降低查找复杂度()
    while(!feof(file_fp))
    {
        fread(&ch,1,1,file_fp);
        huffmanNodes[ch].weight++;
        FileLength++;
    }
    //文件末尾存在\n,文件长度减一,最后一个统计的字符权值减一
    FileLength--;
    huffmanNodes[ch].weight--;
    ////////////////////////////////////////////////////////////////
    createHuffmanNode(n);//创建哈弗曼树
    enCode(huffmanNodes,n);//进行编码
    ////////////////////////////////////////////////////////////////
    end_1 = clock();
    interval_1 = (double)(end_1 - start_1) / CLOCKS_PER_SEC;//计算时间

    choosing(choice,n,2*n-1);//选择是否显示信息

    start_2 = clock();
    fseek(file_fp,0,SEEK_SET);//设置文件指针file_fp 在文件 0个字节 的位置
    fwrite(&FileLength,4,1,dest_fp);//写入文件的长度四位
    fseek(dest_fp,8,SEEK_SET);//设置文件指针dest_fp 在文件 8个字节 的位置 SEEK_SET从开头计算
    codes[0]=0;//初始化
    fileLength=0;
    while(!feof(file_fp))
    {
        fread(&ch,1,1,file_fp);//读入一项一个字节到ch
        //printf("%c",data);
        fileLength++;//目标文件计数++
        for(i=0; i<n; i++)
        {
            if(ch==huffmanNodes[i].data)//找到他的位置
            {
                break;
            }
        }
        strcat(codes,huffmanNodes[i].code);
        while(strlen(codes)>=8)//重新编码为八位
        {
            for(i=0; i<8; i++)
            {
                if(codes[i]=='1')
                {
                    ch = (ch<<1)|1;//ch*2+1
                }
                else
                {
                    ch = ch<<1;//ch*2
                }
            }//将八位转换成一位ch
            fwrite(&ch,1,1,dest_fp);
            sumLength++;
            strcpy(codes,codes+8);
        }
        if(fileLength==FileLength)
        {
            break;
        }
    }
    //剩余不足8位的数字补全
    if(strlen(codes)>0)
    {
        strcat(codes,"00000000");
        for(i=0; i<8; i++)
        {
            if(codes[i]=='1')
            {
                ch=(ch<<1)|1;
            }
            else
            {
                ch=ch<<1;
            }
        }
        fwrite(&ch,1,1,dest_fp);
        sumLength++;
    }
    sumLength+=8;
    /***********
     * 写入节点信息
     *************/
    printf(" 编码区总长为：%ld 个字节 \n",sumLength-8);
    fseek(dest_fp,4,SEEK_SET);
    fwrite(&sumLength,4,1,dest_fp);
    fseek(dest_fp,sumLength,SEEK_SET);
    fwrite(&n,4,1,dest_fp);
    for(i=0; i<n; i++)
    {
        //写入字符
        fwrite(&(huffmanNodes[i].data),1,1,dest_fp);
        ch=huffmanNodes[i].codeLength;
        //写入编码长度
        fwrite(&ch,1,1,dest_fp);
        //写入编码
        if(huffmanNodes[i].codeLength%8!=0)//不足八位补全
        {
            for(j=huffmanNodes[i].codeLength%8; j<8; j++)
            {
                strcat(huffmanNodes[i].code,"0");
            }
        }
        while(huffmanNodes[i].code[0]!=0)
        {
            ch = 0;
            for(j=0; j<8; j++)
            {
                if(huffmanNodes[i].code[j]=='1')
                {
                    ch=(ch<<1)|1;
                }
                else
                {
                    ch=ch<<1;
                }
            }
            strcpy(huffmanNodes[i].code,huffmanNodes[i].code+8);
            count++;
            fwrite(&ch,1,1,dest_fp);
        }
    }
    printf("压缩中...........\n");
    toolBar();//假进度条
    printf("\n");
    end_2=clock();
    interval_2=(double)(end_2-start_2)/CLOCKS_PER_SEC;
    printf( "压缩用时为：%.2f seconds\n", interval_1+interval_2);
    speed=(float)FileLength/(interval_1+interval_2)/1000;
    printf("\n 压缩速率为：%.2f KB/S\n",speed);
    printf("\n");
    printf(" 源文件长度为：%ld 个字节 \n",FileLength);
    sumLength=sumLength+4+n*2+count;
    printf(" 压缩后文件长度为：%ld 个字节 \n",sumLength);
    rate=(float)sumLength/(float)FileLength;
    printf(" 压缩率 (百分比 )为：%.2f%%\n",rate*100);
    fclose(file_fp);
    fclose(dest_fp);
    return;
}
void unCompress()
{
    clock_t start;
    clock_t end;
    FILE * file_fp;
    FILE * dest_fp;
    char filePath[100];
    char destPath[100];
    long FileLength;
    long sumLength;
    long fileLength;
    int n;
    //编码所占的字节数
    int m;
    int i;
    int j;
    int k;
    char buffer[MAXN];
    char codes[MAXN];
    unsigned char ch;
    int maxLength;
    double interval;
    float speed;
    //////////////////////////////

    printf("请输入要压缩的文件名称：");
    scanf("%s",filePath);
    file_fp = fopen(filePath,"rb");
    if(file_fp==NULL)
    {
        printf("文件名输入错误,文件不存在！\n");
        return;
    }
    printf("请输入压缩后的文件名称：");
    scanf("%s",destPath);
    dest_fp = fopen(destPath,"wb");
    if(dest_fp==NULL)
    {
        printf("文件名输入错误,文件不存在！\n");
        return;
    }

    //////////////////////////////
    start = clock();
    //从压缩文件中读出FileLength和sumLength
    fread(&FileLength,4,1,file_fp);
    fread(&sumLength,4,1,file_fp);
    //读取叶子结点数
    fseek(file_fp,sumLength,SEEK_SET);
    fread(&n,4,1,file_fp);
    printf("\n解码信息：源文件长度为%d个字节\n",FileLength);
    //读取节点信息
    for(i=0; i<n; i++)
    {
        //读入字符
        fread(&(huffmanNodes[i].data),1,1,file_fp);
        //编码长度
        fread(&ch,1,1,file_fp);
        huffmanNodes[i].codeLength=ch;
        huffmanNodes[i].code[0]=0;
        if(huffmanNodes[i].codeLength%8>0)//长度不是八位的
        {
            m = huffmanNodes[i].codeLength/8+1;
        }
        else
        {
            m = huffmanNodes[i].codeLength/8;
        }
        //根据字节长度m读出编码
        for(j=0; j<m; j++)
        {
            //此时的ch为01编码转换成的字符
            fread(&ch,1,1,file_fp);
            //字符型编码转换成二进制型(首位为1)
            itoa(ch,buffer,2);
            //如果编码不够8位，则缺少了8-k位 在前面补0
            for(k=8; k>strlen(buffer); k--)
            {
                strcat(huffmanNodes[i].code,"0");
            }
            //把二进制代码存入huffmannode.code中
            strcat(huffmanNodes[i].code,buffer);
        }
        //去掉编码中多余的0
        huffmanNodes[i].code[huffmanNodes[i].codeLength]=0;
    }
    //找出编码长度的最大值
    maxLength=0;
    for(i=0; i<n; i++)
    {
        if(huffmanNodes[i].codeLength > maxLength)
        {
            maxLength = huffmanNodes[i].codeLength;
        }
    }
    //写入解压后的文件
    fseek(file_fp,8,SEEK_SET);
    fileLength=0;
    codes[0]=0;
    buffer[0]=0;
    while(1)
    {
        while(strlen(codes)<maxLength)
        {
            fread(&ch,1,1,file_fp);
            itoa(ch,buffer,2);
            for(k=8; k>strlen(buffer); k--)
            {
                strcat(codes,"0");
            }
            strcat(codes,buffer);
        }
        for(i=0; i<n; i++)
        {
            if(memcmp(huffmanNodes[i].code,codes,(unsigned int)huffmanNodes[i].codeLength)==0)//找到那个长度相同的code在Huffman节点中
            {
                break;
            }
        }
        strcpy(codes,codes+huffmanNodes[i].codeLength);
        ch = huffmanNodes[i].data;//获得该字符
        fwrite(&ch,1,1,dest_fp);//写入字符
        fileLength++;
        if(fileLength==FileLength)
        {
            break;
        }
    }
    printf("解压中...........\n");
    toolBar();//假进度条
    end = clock();
    interval = (double)(end-start)/ CLOCKS_PER_SEC;
    printf( "\n解压完成,解压用时为：%f seconds\n",interval);
    fseek(file_fp,0,SEEK_SET);
    FileLength=0;
    while(!feof(file_fp))
    {
        fread(&ch,1,1,file_fp);
        FileLength++;
    }
    FileLength--;
    speed = (float)FileLength/interval/1000;
    printf("\n解压速度为：%.2f KB/S\n",speed);
    fclose(file_fp);
    fclose(dest_fp);
    return;
}
int main(){

    int n;
    printf("1--压缩               2--解压\n");
    scanf("%d",&n);
    switch (n){
        case 1:
            comPress();
            break;
        case 2:
            unCompress();
            break;
        default:
            break;
    }
}