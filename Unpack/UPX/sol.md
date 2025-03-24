经典pushad：

![image-20241122160514444](./sol/images/image-20241122160514444.png)



pushad压栈寄存器后，在-0x20的ESP往高地址偏一点的内存，下硬件断点，F9。

然后断到这里：![image-20241122160854015](./sol/images/image-20241122160854015.png)



禁用硬件断点，到010154B8处的JMP就跳转到OEP了~

![image-20241122160938693](./sol/images/image-20241122160938693.png)



对比原来notepad.exe的OEP：

![image-20241122161031010](./sol/images/image-20241122161031010.png)



RIGHT。



但IAT的恢复好像还是不大对。



---

所以要考察最基本的脱壳技巧的话，直接把UPX改了，使得正常脱壳器脱不出来。