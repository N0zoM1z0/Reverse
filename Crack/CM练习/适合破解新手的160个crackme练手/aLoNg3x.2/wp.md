尝试2※CM

逻辑确实就复杂起来了 不是简单的cmp check
测试发现Codice长度为<11位
又是这种形式的布局
![img](wp/images/image-1.png)

ida 找check 
![img](wp/images/image.png)

但下断点发现报错? access denied???
好吧是Codice输长了...
![img](wp/images/image-2.png)

ida的check长这样
![img](wp/images/image-3.png)
倒也不难 逆算法
但是动调关键值为什么是0???
?????
