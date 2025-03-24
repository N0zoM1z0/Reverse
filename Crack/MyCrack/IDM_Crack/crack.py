byte_75E6D8 = "2YOPB3AQCVUXMNRS97WE0IZD4KLFGHJ8165T@"
print(len(byte_75E6D8))

# 114514@123.com
table = "2YOPB3AQCVUXMNRS97WE0IZD4KLFGHJ8165T" 
table = [x for x in table]

# 222YA-2222D-22227-222Y9
for i1 in range(36):
    for i2 in range(36):
        for i3 in range(36):
            for i4 in range(36):
                for i5 in range(36):
                    t = i1*37**4 + i2*37**3 + i3*37**2 + i4*37**1 + i5
                    # t &= 0xff
                    if t%53 == 0 and t>0:
                        s = table[i1] + table[i2] + table[i3] + table[i4] + table[i5]
                        print(s)
                        exit()
