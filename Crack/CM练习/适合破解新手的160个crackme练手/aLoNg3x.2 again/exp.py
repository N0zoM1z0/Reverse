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