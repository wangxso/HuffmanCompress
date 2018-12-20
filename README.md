1、输入字符流，构造树
2、构造哈夫曼编码
3、写入树结构，便于之后解压
4、解压先将树存入内存
5、解译01字符串

文件压缩的步骤：
    --1、统计字符个数 （字符的个数对应权值）
    --2、生成Huffman树
    --3、生成Huffman编码
    --4、压缩
    --4.1：压缩之前写入字符及其出现的次数（结构体），方便解压缩时重建Huffman树。
    --4.2：进行压缩。
文件解压缩的步骤：
    --1、打开压缩文件
    --2、重建Huffman树
    --3、进行解压缩
HuffmanNode --- lchild int
            --- rchild int
            --- parent int
            --- codes char*
            --- data unsigned
            --- weight int


（压缩文件）filePath destPath//文件名和目标文件名 --->
 ---> comPress() ---> enCode() ---> Write()//写入文件

（解压文件）filePath destPath//文件名和目标文件名  ---> 文件中读取编码信息 ---> 根据编码信息恢复原来的信息 ---> 写入
