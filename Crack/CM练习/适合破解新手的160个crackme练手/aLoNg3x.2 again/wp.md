crack目标是让register框去掉。不让patch，所以要真算出Code。

第一个难点：

DELPHI。。。窗口文字与处理函数的定义在这种地方：

![image-20241108211127714](./wp/images/image-20241108211127714.png)





这个cancel处，只要两个框填一样的就会弹GREAT+LAMER:

![image-20241108211856275](./wp/images/image-20241108211856275.png)

![image-20241108211624229](./wp/images/image-20241108211624229.png)





然后就是register的分析，但这里这个a1始终为0？？？

![image-20241108213758738](./wp/images/image-20241108213758738.png)

![image-20241108213745463](./wp/images/image-20241108213745463.png)



嘶，这么看有一个思路：

先通过if ( v9 )这里不是valid long的地方得到一个dword_445830，然后再传valid的。



比如我输入code = 123asd，得到的结果就是：[0x445830] = 0x1603

![image-20241108215438993](./wp/images/image-20241108215438993.png)



```py
def enc(name,code):
    a1 = 0x1603 # ?
    v14 = code
    v13 = name
    v4 = 0
    v5 = len(v13)
    v12 = v5
    v6 = 1
    while True:
        v7 = len(v13)
        if v7 >= 1:
            while True:
                v7 -= 1
                v4 += a1 * (ord(v13[v7])&0xFF) * (ord(v13[v6-1])&0xFF)
                if v7<=0:
                    break
        v6 += 1
        v12 -= 1
        if v12<=0:
            break
    
    v8 = abs(v4) % 666666
    v14 = v14%80 + v14//89 + 1
    return v8,v14
    if v8 == v14:
        return 1
    else:
        return 0


print(enc("crackme",6783372))
# 228694
for v14 in range(1,0xFFFFFFFF,1):
    if(v14%80 + v14//89 + 1  == 228694):
        print(v14)
# v14 = 89*x + y
# 20353206
```

![image-20241108220431058](./wp/images/image-20241108220431058.png)



![image-20241108220513859](./wp/images/image-20241108220513859.png)





虽然过了这个cmp，但为啥还要我again啊？？？

![image-20241108220656818](./wp/images/image-20241108220656818.png)



:???? 搞不懂了。。。



哦，是还要破解again这个按钮的意思。。。不是说我前面破解错了，而是还有一关（）

![image-20241108220817279](./wp/images/image-20241108220817279.png)



一样的操作~

![image-20241108221224553](./wp/images/image-20241108221224553.png)

完整过一遍。

![crackMe](./wp/images/crackMe.gif)



还是不错~ 



但这个算法好像自己写的有点问题。。。实际是动调拿的值



结果发现是python的abs后取模和C的不一样！！！![image-20241108224212974](./wp/images/image-20241108224212974.png)

![image-20241108224222504](./wp/images/image-20241108224222504.png)





。。。逆天。。。



整体代码最好还是用C写，这里懒得重构了，python+C就行。

```py
def GenDWORD(name):
    v7 = name
    v1 = 891
    v2 = len(v7) - 1
    v3 = 1
    while True:
        v1 += (ord(v7[v3-1])&0xFF) * ((ord(v7[v3])&0xFF) % 0x11 + 1)
        v3 += 1
        v2 -= 1
        if(v2<=0):
            break
    return v1 % 29000
def enc(name,code):
    a1 = GenDWORD('123asd')
    v14 = code
    v13 = name
    v4 = 0
    v5 = len(v13)
    v12 = v5
    v6 = 1
    while True:
        v7 = len(v13)
        if v7 >= 1:
            while True:
                v7 -= 1
                v4 += a1 * (ord(v13[v7])&0xFF) * (ord(v13[v6-1])&0xFF)
                print(hex(v4),end=' ')
                v4 &= 0xFFFFFFFF
                if v7<=0:
                    break
        v6 += 1
        v12 -= 1
        if v12<=0:
            break
    
    print(v4)
    v8 = abs(v4) % 666666 
    v14 = v14%80 + v14//89 + 1
    return v8,v14
    if v8 == v14:
        return 1
    else:
        return 0

print(enc("crackme",6783372))
# 228694
# for v14 in range(1,0xFFFFFFFF,1):
#     if(v14%80 + v14//89 + 1  == 228694):
#         print(v14)
# v14 = 89*x + y
# 20353206
```

```c
#include<bits/stdc++.h>
using namespace std;

int main(){
	int n = 0xb107b8ac;
	cout<<abs(n) % 666666;
}
```



---

记得上回做是今年4月份。。。当时竟然没分析出来，😓。。。

AEWSOME REVERSE！