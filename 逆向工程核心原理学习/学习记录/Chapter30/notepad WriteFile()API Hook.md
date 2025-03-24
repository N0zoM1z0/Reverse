

用作者给的exe和notepad.exe测试：

（x86编译，用作者的notepad.exe）

![image-20240905115713395](./notepad WriteFile()API Hook/images/image-20240905115713395.png)

![image-20240905115738064](./notepad WriteFile()API Hook/images/image-20240905115738064.png)



但迁移到64位的话，改完寄存器后好像还是不行，读取的字符串为空？