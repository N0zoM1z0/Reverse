# RC4

```c
#include<bits/stdc++.h>
using namespace std;
signed main(){
	int base64_table[] = {125, 239, 101, 151, 77, 163, 163, 110, 58, 230, 186, 206, 84, 84, 189, 193, 30, 63, 104, 178, 130, 211, 164, 94, 75, 16, 32, 33, 193, 160, 120, 47, 30, 127, 157, 66, 163, 181, 177, 47, 0, 236, 106, 107, 144, 231, 111, 16, 36, 34, 91, 9, 188, 81, 5, 241, 235, 3, 54, 150, 40, 119, 202, 150};
	string key = "genshinimpact";
	int s[256],k[256];
	int j=0;
	for (int i = 0; i < 256; i++) {
            s[i] = i;
            k[i] = key[i % key.length()];
        }
        for (int i2 = 0; i2 < 256; i2++) {
            j = (s[i2] + j + k[i2]) & 255;
            int temp = s[i2];
            s[i2] = s[j];
            s[j] = temp;
        }
	int j2 = 0;
        int i3 = 0;
        for (int i4 : base64_table) {
            i3 = (i3 + 1) & 255;
            j2 = (s[i3] + j2) & 255;
            int temp2 = s[i3];
            s[i3] = s[j2];
            s[j2] = temp2;
            int rnd = s[(s[i3] + s[j2]) & 255];
            cout<<((char) (i4 ^ rnd));
        }


}
```



# TEA

注意的点:
魔改系数
魔改轮数
魔改...
unwind异常展开两次调用
...

## tea

```c
#include<bits/stdc++.h>
using namespace std;
//解密函数
void decrypt (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=-0x61C88647*0x10, i;  /* set up */
    uint32_t delta=0x61C88647;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    for (i=0; i<0x10; i++) {                         /* basic cycle start */
    	sum += delta;
        v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
        v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;
}
void encrypt (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0, i;           /* set up */
    uint32_t delta=0x61C88647;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    for (i=0; i < 0x10; i++) {                       /* basic cycle start */
        v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
        sum -= delta;
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;
}
signed main(){
	uint32_t k[4] = {0x12345678,0x09101112,0x13141516,0x15161718};
	uint32_t enc[10] = {0xbc2b4df9,0x6213dd13,0x89fffcc9,0xfc94f7d,0x526d1d63,0xe341fd50,0x97287633,0x6bf93638,0x83143990,0x1f2ce22c};
//	uint32_t enc[10] = {0xbc2b4df9,0x6213dd13,0xc9fcff89,0x7d4fc90f,0x631d6d52,0x50fd41e3,0x33762897,0x3836f96b,0x90391483,0x2ce22c1f};
//	uint32_t enc[10] = {0xbc2b4df9,0x6213dd13,0x8d849acc,0x7ea11227,0x96e7c67,0xb5038d0a,0xea443d6c,0x27a16e7e,0x88187fc8,0x5a24eb26};
	uint32_t v[2];
	v[0] = enc[0],v[1] = enc[1];
	decrypt(v,k);
	printf("0x%x 0x%x\n",v[0],v[1]);
	for(int i=2;i<10;i+=2){
		decrypt(v,k);
		printf("0x%x 0x%x\n",v[0],v[1]);
		enc[0] = v[0],enc[1] = v[1];
	}
	/*
	0x68cb20b3,0xbdaafddd,0x6025dd45,0x10eac9d3,0x49e06b16,0xec010c2d,0xbf86d429,0xd87b98c4
	*/
	uint32_t key[8] = {0xb9b7a8fd,0x46962412,0x3d5d4e56,0x8d34bb3f,0xde770f5d,0x1f96584b,0xbc2b4df9,0x6213DD13};
	for(int i=2;i<10;i++)
		enc[i] ^= key[i-2];
	for(int i=0;i<10;i++){
		for(int j=0;j<4;j++){
			printf("%c",(enc[i]>>((j*8))&0xFF));
		}
	}
}
```



## xtea
```c
#include <stdio.h>
#include <stdint.h>
 
/* take 64 bits of data in v[0] and v[1] and 128 bits of key[0] - key[3] */
 
void encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}
 
void decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], delta=0x61C88647, sum=-delta*num_rounds;
    for (i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum += delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}
 
int main()
{
    uint32_t v[8]={0x87AAA7C1, 0x857321B6, 0x0E71D28C, 0xCADF39F2, 0x58EFCA14, 0xD7E7D9D8, 0xF29F5C5D, 0x5F5ED45E};
    uint32_t const k[4]={0x44,0x41,0x53,0x21};
    unsigned int r=36;
    decipher(r, v, k);
    decipher(r,(uint32_t*)(v+2),k);
    decipher(r,(uint32_t*)(v+4),k);
    decipher(r,(uint32_t*)(v+6),k);
    for(int i=0;i<8;i++)
    	printf("%x ",v[i]);
    return 0;
}
```
## xxtea
```c
#include <stdio.h>
#include <stdint.h>
#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))
 
void btea(uint32_t *v, int n, uint32_t const key[4])
{
    uint32_t y, z, sum;
    unsigned p, rounds, e;
    if (n > 1)            /* Coding Part */
    {
        rounds = 6 + 52/n;
        sum = 0;
        z = v[n-1];
        do
        {
            sum += DELTA;
            e = (sum >> 2) & 3;
            for (p=0; p<n-1; p++)
            {
                y = v[p+1];
                z = v[p] += MX;
            }
            y = v[0];
            z = v[n-1] += MX;
        }
        while (--rounds);
    }
    else if (n < -1)      /* Decoding Part */
    {
        n = -n;
        rounds = 6 + 52/n;
        sum = rounds*DELTA;
        y = v[0];
        do
        {
            e = (sum >> 2) & 3;
            for (p=n-1; p>0; p--)
            {
                z = v[p-1];
                y = v[p] -= MX;
            }
            z = v[n-1];
            y = v[0] -= MX;
            sum -= DELTA;
        }
        while (--rounds);
    }
}
 
 
int main()
{
    uint32_t v[8]= {0x58be7427 ,0x3394cd3a ,0x9324225f ,0x1b82dbb ,0x4b7d4b90 ,0xdeefbf8f ,0x911b6e96 ,0x38069288};
    uint32_t const k[4]={0x44,0x41,0x53,0x21};
    int n = 8;
    btea(v, -n, k);
    for(int i=0;i<8;i++){
    	int x = v[i];
    	for(int j=0;j<4;j++)
    		printf("%c",x>>(8*(j)));
	}
    return 0;
}
```




# AES

## SBOX->INVSBOX
魔改SBOX的情况
```py
new_s_box = []
new_contrary_sbox = [0]*256

for i in range(256):
    line = (new_s_box[i]&0xf0)>>4
    rol = new_s_box[i]&0xf
    new_contrary_sbox[(line*16)+rol] = i
print(new_s_box)
print()
print(new_contrary_sbox)
```

## AES C
```c
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include<bits/stdc++.h>
using namespace std;
typedef struct{
    uint32_t eK[44], dK[44];    // encKey, decKey
    int Nr; // 10 rounds
}AesKey;

#define BLOCKSIZE 16  //AES-128分组长度为16字节

// uint8_t y[4] -> uint32_t x
#define LOAD32H(x, y) \
  do { (x) = ((uint32_t)((y)[0] & 0xff)<<24) | ((uint32_t)((y)[1] & 0xff)<<16) | \
             ((uint32_t)((y)[2] & 0xff)<<8)  | ((uint32_t)((y)[3] & 0xff));} while(0)

// uint32_t x -> uint8_t y[4]
#define STORE32H(x, y) \
  do { (y)[0] = (uint8_t)(((x)>>24) & 0xff); (y)[1] = (uint8_t)(((x)>>16) & 0xff);   \
       (y)[2] = (uint8_t)(((x)>>8) & 0xff); (y)[3] = (uint8_t)((x) & 0xff); } while(0)

// 从uint32_t x中提取从低位开始的第n个字节
#define BYTE(x, n) (((x) >> (8 * (n))) & 0xff)

/* used for keyExpansion */
// 字节替换然后循环左移1位
#define MIX(x) (((S[BYTE(x, 2)] << 24) & 0xff000000) ^ ((S[BYTE(x, 1)] << 16) & 0xff0000) ^ \
                ((S[BYTE(x, 0)] << 8) & 0xff00) ^ (S[BYTE(x, 3)] & 0xff))

// uint32_t x循环左移n位
#define ROF32(x, n)  (((x) << (n)) | ((x) >> (32-(n))))
// uint32_t x循环右移n位
#define ROR32(x, n)  (((x) >> (n)) | ((x) << (32-(n))))

/* for 128-bit blocks, Rijndael never uses more than 10 rcon values */
// AES-128轮常量
static const uint32_t rcon[10] = {
        0x01000000UL, 0x02000000UL, 0x04000000UL, 0x08000000UL, 0x10000000UL,
        0x20000000UL, 0x40000000UL, 0x80000000UL, 0x1B000000UL, 0x36000000UL
};
// S盒
unsigned char S[256] = {
        41, 64, 87, 110, 133, 156, 179, 202, 225, 248, 15, 38, 61, 84, 107, 130, 153, 176, 199, 222, 245, 12, 35, 58, 81, 104, 127, 150, 173, 196, 219, 242, 9, 32, 55, 78, 101, 124, 147, 170, 193, 216, 239, 6, 29, 52, 75, 98, 121, 144, 167, 190, 213, 236, 3, 26, 49, 72, 95, 118, 141, 164, 187, 210, 233, 0, 23, 46, 69, 92, 115, 138, 161, 184, 207, 230, 253, 20, 43, 66, 89, 112, 135, 158, 181, 204, 227, 250, 17, 40, 63, 86, 109, 132, 155, 178, 201, 224, 247, 14, 37, 60, 83, 106, 129, 152, 175, 198, 221, 244, 11, 34, 57, 80, 103, 126, 149, 172, 195, 218, 241, 8, 31, 54, 77, 100, 123, 146, 169, 192, 215, 238, 5, 28, 51, 74, 97, 120, 143, 166, 189, 212, 235, 2, 25, 48, 71, 94, 117, 140, 163, 186, 209, 232, 255, 22, 45, 68, 91, 114, 137, 160, 183, 206, 229, 252, 19, 42, 65, 88, 111, 134, 157, 180, 203, 226, 249, 16, 39, 62, 85, 108, 131, 154, 177, 200, 223, 246, 13, 36, 59, 82, 105, 128, 151, 174, 197, 220, 243, 10, 33, 56, 79, 102, 125, 148, 171, 194, 217, 240, 7, 30, 53, 76, 99, 122, 145, 168, 191, 214, 237, 4, 27, 50, 73, 96, 119, 142, 165, 188, 211, 234, 1, 24, 47, 70, 93, 116, 139, 162, 185, 208, 231, 254, 21, 44, 67, 90, 113, 136, 159, 182, 205, 228, 251, 18
};

//逆S盒
unsigned char inv_S[256] = {
        65, 232, 143, 54, 221, 132, 43, 210, 121, 32, 199, 110, 21, 188, 99, 10, 177, 88, 255, 166, 77, 244, 155, 66, 233, 144, 55, 222, 133, 44, 211, 122, 33, 200, 111, 22, 189, 100, 11, 178, 89, 0, 167, 78, 245, 156, 67, 234, 145, 56, 223, 134, 45, 212, 123, 34, 201, 112, 23, 190, 101, 12, 179, 90, 1, 168, 79, 246, 157, 68, 235, 146, 57, 224, 135, 46, 213, 124, 35, 202, 113, 24, 191, 102, 13, 180, 91, 2, 169, 80, 247, 158, 69, 236, 147, 58, 225, 136, 47, 214, 125, 36, 203, 114, 25, 192, 103, 14, 181, 92, 3, 170, 81, 248, 159, 70, 237, 148, 59, 226, 137, 48, 215, 126, 37, 204, 115, 26, 193, 104, 15, 182, 93, 4, 171, 82, 249, 160, 71, 238, 149, 60, 227, 138, 49, 216, 127, 38, 205, 116, 27, 194, 105, 16, 183, 94, 5, 172, 83, 250, 161, 72, 239, 150, 61, 228, 139, 50, 217, 128, 39, 206, 117, 28, 195, 106, 17, 184, 95, 6, 173, 84, 251, 162, 73, 240, 151, 62, 229, 140, 51, 218, 129, 40, 207, 118, 29, 196, 107, 18, 185, 96, 7, 174, 85, 252, 163, 74, 241, 152, 63, 230, 141, 52, 219, 130, 41, 208, 119, 30, 197, 108, 19, 186, 97, 8, 175, 86, 253, 164, 75, 242, 153, 64, 231, 142, 53, 220, 131, 42, 209, 120, 31, 198, 109, 20, 187, 98, 9, 176, 87, 254, 165, 76, 243, 154
};

/* copy in[16] to state[4][4] */
int loadStateArray(uint8_t (*state)[4], const uint8_t *in) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[j][i] = *in++;
        }
    }
    return 0;
}

/* copy state[4][4] to out[16] */
int storeStateArray(uint8_t (*state)[4], uint8_t *out) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            *out++ = state[j][i];
        }
    }
    return 0;
}
//秘钥扩展
int keyExpansion(const uint8_t *key, uint32_t keyLen, AesKey *aesKey) {

    if (NULL == key || NULL == aesKey){
        printf("keyExpansion param is NULL\n");
        return -1;
    }

    if (keyLen != 16){
        printf("keyExpansion keyLen = %d, Not support.\n", keyLen);
        return -1;
    }

    uint32_t *w = aesKey->eK;  //加密秘钥
    uint32_t *v = aesKey->dK;  //解密秘钥

    /* keyLen is 16 Bytes, generate uint32_t W[44]. */

    /* W[0-3] */
    for (int i = 0; i < 4; ++i) {
        LOAD32H(w[i], key + 4*i);
    }

    /* W[4-43] */
    for (int i = 0; i < 10; ++i) {
        w[4] = w[0] ^ MIX(w[3]) ^ rcon[i];
        w[5] = w[1] ^ w[4];
        w[6] = w[2] ^ w[5];
        w[7] = w[3] ^ w[6];
        w += 4;
    }

    w = aesKey->eK+44 - 4;
    //解密秘钥矩阵为加密秘钥矩阵的倒序，方便使用，把ek的11个矩阵倒序排列分配给dk作为解密秘钥
    //即dk[0-3]=ek[41-44], dk[4-7]=ek[37-40]... dk[41-44]=ek[0-3]
    for (int j = 0; j < 11; ++j) {

        for (int i = 0; i < 4; ++i) {
            v[i] = w[i];
        }
        w -= 4;
        v += 4;
    }

    return 0;
}

// 轮秘钥加
int addRoundKey(uint8_t (*state)[4], const uint32_t *key) {
    uint8_t k[4][4];

    /* i: row, j: col */
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            k[i][j] = (uint8_t) BYTE(key[j], 3 - i);  /* 把 uint32 key[4] 先转换为矩阵 uint8 k[4][4] */
            state[i][j] ^= k[i][j];
        }
    }

    return 0;
}

//字节替换
int subBytes(uint8_t (*state)[4]) {
    /* i: row, j: col */
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = S[state[i][j]]; //直接使用原始字节作为S盒数据下标
        }
    }

    return 0;
}

//逆字节替换
int invSubBytes(uint8_t (*state)[4]) {
    /* i: row, j: col */
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = inv_S[state[i][j]];
        }
    }
    return 0;
}

//行移位
int shiftRows(uint8_t (*state)[4]) {
    uint32_t block[4] = {0};

    /* i: row */
    for (int i = 0; i < 4; ++i) {
    //便于行循环移位，先把一行4字节拼成uint_32结构，移位后再转成独立的4个字节uint8_t
        LOAD32H(block[i], state[i]);
        block[i] = ROF32(block[i], 8*i);
        STORE32H(block[i], state[i]);
    }

    return 0;
}

//逆行移位
int invShiftRows(uint8_t (*state)[4]) {
    uint32_t block[4] = {0};

    /* i: row */
    for (int i = 0; i < 4; ++i) {
        LOAD32H(block[i], state[i]);
        block[i] = ROR32(block[i], 8*i);
        STORE32H(block[i], state[i]);
    }

    return 0;
}

/* Galois Field (256) Multiplication of two Bytes */
// 两字节的伽罗华域乘法运算
uint8_t GMul(uint8_t u, uint8_t v) {
    uint8_t p = 0;

    for (int i = 0; i < 8; ++i) {
        if (u & 0x01) {    //
            p ^= v;
        }

        int flag = (v & 0x80);
        v <<= 1;
        if (flag) {
            v ^= 0x1B; /* x^8 + x^4 + x^3 + x + 1 */
        }

        u >>= 1;
    }

    return p;
}

// 列混合
int mixColumns(uint8_t (*state)[4]) {
    uint8_t tmp[4][4];
    uint8_t M[4][4] = {{0x02, 0x03, 0x01, 0x01},
                       {0x01, 0x02, 0x03, 0x01},
                       {0x01, 0x01, 0x02, 0x03},
                       {0x03, 0x01, 0x01, 0x02}};

    /* copy state[4][4] to tmp[4][4] */
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j){
            tmp[i][j] = state[i][j];
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {  //伽罗华域加法和乘法
            state[i][j] = GMul(M[i][0], tmp[0][j]) ^ GMul(M[i][1], tmp[1][j])
                        ^ GMul(M[i][2], tmp[2][j]) ^ GMul(M[i][3], tmp[3][j]);
        }
    }

    return 0;
}

// 逆列混合
int invMixColumns(uint8_t (*state)[4]) {
    uint8_t tmp[4][4];
    uint8_t M[4][4] = {{0x0E, 0x0B, 0x0D, 0x09},
                       {0x09, 0x0E, 0x0B, 0x0D},
                       {0x0D, 0x09, 0x0E, 0x0B},
                       {0x0B, 0x0D, 0x09, 0x0E}};  //使用列混合矩阵的逆矩阵

    /* copy state[4][4] to tmp[4][4] */
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j){
            tmp[i][j] = state[i][j];
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = GMul(M[i][0], tmp[0][j]) ^ GMul(M[i][1], tmp[1][j])
                          ^ GMul(M[i][2], tmp[2][j]) ^ GMul(M[i][3], tmp[3][j]);
        }
    }

    return 0;
}

// AES-128加密接口，输入key应为16字节长度，输入长度应该是16字节整倍数，
// 这样输出长度与输入长度相同，函数调用外部为输出数据分配内存
int aesEncrypt(const uint8_t *key, uint32_t keyLen, const uint8_t *pt, uint8_t *ct, uint32_t len) {

    AesKey aesKey;
    uint8_t *pos = ct;
    const uint32_t *rk = aesKey.eK;  //解密秘钥指针
    uint8_t out[BLOCKSIZE] = {0};
    uint8_t actualKey[16] = {0};
    uint8_t state[4][4] = {0};

    if (NULL == key || NULL == pt || NULL == ct){
        printf("param err.\n");
        return -1;
    }

    if (keyLen > 16){
        printf("keyLen must be 16.\n");
        return -1;
    }

    if (len % BLOCKSIZE){
        printf("inLen is invalid.\n");
        return -1;
    }

    memcpy(actualKey, key, keyLen);
    keyExpansion(actualKey, 16, &aesKey);  // 秘钥扩展

	// 使用ECB模式循环加密多个分组长度的数据
    for (int i = 0; i < len; i += BLOCKSIZE) {
		// 把16字节的明文转换为4x4状态矩阵来进行处理
        loadStateArray(state, pt);
        // 轮秘钥加
        addRoundKey(state, rk);

        for (int j = 1; j < 10; ++j) {
            rk += 4;
            subBytes(state);   // 字节替换
            shiftRows(state);  // 行移位
            mixColumns(state); // 列混合
            addRoundKey(state, rk); // 轮秘钥加
        }

        subBytes(state);    // 字节替换
        shiftRows(state);  // 行移位
        // 此处不进行列混合
        addRoundKey(state, rk+4); // 轮秘钥加
		
		// 把4x4状态矩阵转换为uint8_t一维数组输出保存
        storeStateArray(state, pos);

        pos += BLOCKSIZE;  // 加密数据内存指针移动到下一个分组
        pt += BLOCKSIZE;   // 明文数据指针移动到下一个分组
        rk = aesKey.eK;    // 恢复rk指针到秘钥初始位置
    }
    return 0;
}

// AES128解密， 参数要求同加密
int aesDecrypt(const uint8_t *key, uint32_t keyLen, const uint8_t *ct, uint8_t *pt, uint32_t len) {
    AesKey aesKey;
    uint8_t *pos = pt;
    const uint32_t *rk = aesKey.dK;  //解密秘钥指针
    uint8_t out[BLOCKSIZE] = {0};
    uint8_t actualKey[16] = {0};
    uint8_t state[4][4] = {0};

    if (NULL == key || NULL == ct || NULL == pt){
        printf("param err.\n");
        return -1;
    }

    if (keyLen > 16){
        printf("keyLen must be 16.\n");
        return -1;
    }

    if (len % BLOCKSIZE){
        printf("inLen is invalid.\n");
        return -1;
    }

    memcpy(actualKey, key, keyLen);
    keyExpansion(actualKey, 16, &aesKey);  //秘钥扩展，同加密

    for (int i = 0; i < len; i += BLOCKSIZE) {
        // 把16字节的密文转换为4x4状态矩阵来进行处理
        loadStateArray(state, ct);
        // 轮秘钥加，同加密
        addRoundKey(state, rk);

        for (int j = 1; j < 10; ++j) {
            rk += 4;
            invShiftRows(state);    // 逆行移位
            invSubBytes(state);     // 逆字节替换，这两步顺序可以颠倒
            addRoundKey(state, rk); // 轮秘钥加，同加密
            invMixColumns(state);   // 逆列混合
        }

        invSubBytes(state);   // 逆字节替换
        invShiftRows(state);  // 逆行移位
        // 此处没有逆列混合
        addRoundKey(state, rk+4);  // 轮秘钥加，同加密

        storeStateArray(state, pos);  // 保存明文数据
        pos += BLOCKSIZE;  // 输出数据内存指针移位分组长度
        ct += BLOCKSIZE;   // 输入数据内存指针移位分组长度
        rk = aesKey.dK;    // 恢复rk指针到秘钥初始位置
    }
    return 0;
}
int main(){
	uint8_t key[16]={0x52,0x65,0x5f,0x31,0x73,0x5f,0x65,0x61,0x53,0x79,0x31,0x32,0x33,0x34,0x35,0x36};
	uint8_t ct[16]={
		0x2b,0xc8,0x20,0x8b,0x5c,0x0d,0xa7,0x9b,0x2a,0x51,0x3a,0xd2,0x71,0x71,0xca,0x50	
	};
	uint8_t pt[1024];
	aesDecrypt(key,16,ct,pt,16);
	for(char c:pt){
		cout<<c;
	}
}
```

## AES Python

**AddRoundKey那里有个xor key 的魔改**

```py
import re
import binascii

class Aes:
    sbox = [0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
        0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
        0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
        0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
        0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
        0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
        0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
        0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
        0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
        0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
        0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
        0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
        0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
        0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
        0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
        0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16]
    s_box = {}
    ns_box = {   }

    Rcon = { 
        1: ['0x01', '0x00', '0x00', '0x00'],
        2: ['0x02', '0x00', '0x00', '0x00'],
        3: ['0x04', '0x00', '0x00', '0x00'],
        4: ['0x08', '0x00', '0x00', '0x00'],
        5: ['0x10', '0x00', '0x00', '0x00'],
        6: ['0x20', '0x00', '0x00', '0x00'],
        7: ['0x40', '0x00', '0x00', '0x00'],
        8: ['0x80', '0x00', '0x00', '0x00'],
        9: ['0x1B', '0x00', '0x00', '0x00'],
        10: ['0x36', '0x00', '0x00', '0x00']
    }
    Matrix = [
        ['0x02', '0x03', '0x01', '0x01'],
        ['0x01', '0x02', '0x03', '0x01'],
        ['0x01', '0x01', '0x02', '0x03'],
        ['0x03', '0x01', '0x01', '0x02']
    ]
    ReMatrix = [
        ['0x0e', '0x0b', '0x0d', '0x09'],
        ['0x09', '0x0e', '0x0b', '0x0d'],
        ['0x0d', '0x09', '0x0e', '0x0b'],
        ['0x0b', '0x0d', '0x09', '0x0e']
    ]
    plaintext = [[], [], [], []]
    plaintext1 = [[], [], [], []]
    subkey = [[], [], [], []]  

    def __init__(self, key): 
        self.s_box = dict(zip(["0x%02x"%i for i in range(256)], ["0x%02x"%i for i in self.sbox])) 
        self.ns_box = dict(zip(self.s_box.values(), self.s_box.keys()))
        for i in range(4):
            for j in range(0, 8, 2):
                self.subkey[i].append("0x" + key[i * 8 + j:i * 8 + j + 2]) 
        # print(self.subkey)
        for i in range(4, 44): 
            if i % 4 != 0: 
                tmp = xor_32(self.subkey[i - 1], self.subkey[i - 4],0)
                self.subkey.append(tmp)
            else:  # 4的倍数的时候执行
                tmp1 = self.subkey[i - 1][1:]
                tmp1.append(self.subkey[i - 1][0])
                # print(tmp1)
                for m in range(4):
                    tmp1[m] = self.s_box[tmp1[m]]
                # tmp1 = self.s_box['cf'] 
                tmp1 = xor_32(tmp1, self.Rcon[i / 4], 0) 
                self.subkey.append(xor_32(tmp1, self.subkey[i - 4],0))
        # print(self.subkey)

    def AddRoundKey(self, round): 
        for i in range(4):
            self.plaintext[i] = xor_32(self.plaintext[i], self.subkey[round * 4 + i],0x23) # <=============
        # print('AddRoundKey',self.plaintext)

    def PlainSubBytes(self): 
        for i in range(4):
            for j in range(4):
                self.plaintext[i][j] = self.s_box[self.plaintext[i][j]]
        # print('PlainSubBytes',self.plaintext)

    def RePlainSubBytes(self): 
        for i in range(4):
            for j in range(4):
                self.plaintext[i][j] = self.ns_box[self.plaintext[i][j]]

    def ShiftRows(self): 
        p1, p2, p3, p4 = self.plaintext[0][1], self.plaintext[1][1], self.plaintext[2][1], self.plaintext[3][1]
        self.plaintext[0][1] = p2
        self.plaintext[1][1] = p3
        self.plaintext[2][1] = p4
        self.plaintext[3][1] = p1
        p1, p2, p3, p4 = self.plaintext[0][2], self.plaintext[1][2], self.plaintext[2][2], self.plaintext[3][2]
        self.plaintext[0][2] = p3
        self.plaintext[1][2] = p4
        self.plaintext[2][2] = p1
        self.plaintext[3][2] = p2
        p1, p2, p3, p4 = self.plaintext[0][3], self.plaintext[1][3], self.plaintext[2][3], self.plaintext[3][3]
        self.plaintext[0][3] = p4
        self.plaintext[1][3] = p1
        self.plaintext[2][3] = p2
        self.plaintext[3][3] = p3
        # print('ShiftRows',self.plaintext)

    def ReShiftRows(self): 
        p1, p2, p3, p4 = self.plaintext[0][1], self.plaintext[1][1], self.plaintext[2][1], self.plaintext[3][1]
        self.plaintext[3][1] = p3
        self.plaintext[2][1] = p2
        self.plaintext[0][1] = p4
        self.plaintext[1][1] = p1
        p1, p2, p3, p4 = self.plaintext[0][2], self.plaintext[1][2], self.plaintext[2][2], self.plaintext[3][2]
        self.plaintext[0][2] = p3
        self.plaintext[1][2] = p4
        self.plaintext[2][2] = p1
        self.plaintext[3][2] = p2
        p1, p2, p3, p4 = self.plaintext[0][3], self.plaintext[1][3], self.plaintext[2][3], self.plaintext[3][3]
        self.plaintext[0][3] = p2
        self.plaintext[1][3] = p3
        self.plaintext[2][3] = p4
        self.plaintext[3][3] = p1

    def MixColumns(self): 
        for i in range(4):
            for j in range(4):
                self.plaintext1[i].append(MatrixMulti(self.Matrix[j], self.plaintext[i]))
        # print('MixColumns',self.plaintext1)

    def ReMixColumns(self): 
        for i in range(4):
            for j in range(4):
                self.plaintext1[i].append(MatrixMulti(self.ReMatrix[j], self.plaintext[i]))

    def AESEncryption(self, plaintext): 
        self.plaintext = [[], [], [], []]
        for i in range(4):
            for j in range(0, 8, 2):
                self.plaintext[i].append("0x" + plaintext[i * 8 + j:i * 8 + j + 2])
        self.AddRoundKey(0)
        for i in range(9):
            self.PlainSubBytes()
            self.ShiftRows()
            self.MixColumns()
            self.plaintext = self.plaintext1
            self.plaintext1 = [[], [], [], []]
            self.AddRoundKey(i + 1)

        self.PlainSubBytes()
        self.ShiftRows()
        self.AddRoundKey(10) 
        return Matrixtostr(self.plaintext)

    def AESDecryption(self, cipher): 
        self.plaintext = [[], [], [], []]
        for i in range(4):
            for j in range(0, 8, 2):
                self.plaintext[i].append('0x' + cipher[i * 8 + j:i * 8 + j + 2]) 

        # print(self.ns_box)
        self.AddRoundKey(10)
        for i in range(9):
            self.ReShiftRows()
            self.RePlainSubBytes()
            self.AddRoundKey(9-i)
            self.ReMixColumns()
            self.plaintext = self.plaintext1
            self.plaintext1 = [[], [], [], []]
        self.ReShiftRows()
        self.RePlainSubBytes()
        self.AddRoundKey(0)
        return Matrixtostr(self.plaintext)

    def Encryption(self, text): 
        group = PlaintextGroup(TextToByte(text), 32, 1)
        # print(group)
        cipher = ""
        for i in range(len(group)):
            cipher = cipher + self.AESEncryption(group[i])
        return cipher

    def Decryption(self, cipher): 
        group = PlaintextGroup(cipher, 32, 0)
        # print(group)
        text = ''
        for i in range(len(group)):
            text = text + self.AESDecryption(group[i])
        text = ByteToText(text)
        return text


def xor_32(start, end, key): 
    a = []
    for i in range(0, 4):
        xor_tmp = ""
        b = hextobin(start[i])
        c = hextobin(end[i])
        d = bin(key)[2:].rjust(8,'0')
        for j in range(8):
            tmp = int(b[j], 10) ^ int(c[j], 10) ^ int(d[j],10)
            xor_tmp += str(tmp )
        a.append(bintohex(xor_tmp))
    return a


def xor_8(begin, end): 
    xor_8_tmp = ""
    for i in range(8):
        xor_8_tmp += str(int(begin[i]) ^ int(end[i]))
    return xor_8_tmp


def hextobin(word): 
    word = bin(int(word, 16))[2:]
    for i in range(0, 8-len(word)): 
        word = '0'+word
    return word

def bintohex(word): 
    word = hex(int(word, 2))
    if len(word) == 4:
        return word
    elif len(word) < 4:
        return word.replace('x', 'x0')


def MatrixMulti(s1, s2): 
    result = []
    s3 = []
    for i in range(4):
        s3.append(hextobin(s2[i])) 
    for i in range(4):
        result.append(MultiProcess(int(s1[i], 16), s3[i]))
    for i in range(3):
        result[0] = xor_8(result[0], result[i+1])
    return bintohex(result[0])


def MultiProcess(a, b): 
    if a == 1:
        return b
    elif a == 2:
        if b[0] == '0':
            b = b[1:] + '0'
        else:
            b = b[1:] + '0'
            b = xor_8(b, '00011011')
        return b
    elif a == 3:
        tmp_b = b
        if b[0] == '0':
            b = b[1:] + '0'
        else:
            b = b[1:] + '0'
            b = xor_8(b, '00011011')
        return xor_8(b, tmp_b)

    elif a == 9:
        tmp_b = b
        return xor_8(tmp_b, MultiProcess(2, MultiProcess(2, MultiProcess(2, b))))
    elif a == 11:
        tmp_b = b
        return xor_8(tmp_b, xor_8(MultiProcess(2, MultiProcess(2, MultiProcess(2, b))), MultiProcess(2, b)))
    elif a == 13:
        tmp_b = b
        return xor_8(tmp_b, xor_8(MultiProcess(2, MultiProcess(2, MultiProcess(2, b))), MultiProcess(2, MultiProcess(2, b))))
    elif a == 14:
        return xor_8(MultiProcess(2, b), xor_8(MultiProcess(2, MultiProcess(2, MultiProcess(2, b))), MultiProcess(2, MultiProcess(2, b))))


def Matrixtostr(matrix): 
    result = ""
    for i in range(4):
        for j in range(4):
            result += matrix[i][j][2:]
    return result


def PlaintextGroup(plaintext, length, flag): 
    group = re.findall('.{'+str(length)+'}', plaintext)
    group.append(plaintext[len(group)*length:])
    if group[-1] == '' and flag:
        group[-1] = '16161616161616161616161616161616'
    elif len(group[-1]) < length and flag:
        tmp = int((length-len(group[-1])) / 2)
        if tmp < 10:
            for i in range(tmp):
                group[-1] = group[-1] + '0'+str(tmp)
        else:
            for i in range(tmp):
                group[-1] = group[-1] + str(tmp)
    elif not flag:
        del group[-1]
    return group


def TextToByte(words): 
    text = words.encode('utf-8').hex()
    return text


def ByteToText(encode): 
    tmp = int(encode[-2:])
    word = ''
    for i in range(len(encode)-tmp*2):
        word = word + encode[i]
    # print(word)
    word = bytes.decode(binascii.a2b_hex(word))
    return word
def xorbytes(bytes1,bytes2):
    length=min(len(bytes1),len(bytes2))
    output=bytearray()
    for i in range(length):
        output.append(bytes1[i]^bytes2[i])
    return bytes(output)

res='DCDCCC668DF33C15505EEF1646D9D7DF5027447765DCA73968CB7F7B88DD640F'.lower()
key = 'CA9D7FF8A4099004FAD40661E93B775A'.lower()
A1 = Aes(key)
p1 = A1.AESDecryption(res[:32])
p2 = A1.AESDecryption(res[32:])
from Crypto.Util.number import *
p1 = long_to_bytes(int(p1,16))
p2 = long_to_bytes(int(p2,16))
flag = p1.decode()
p1 = long_to_bytes(int(res[:32],16))
for i in range(len(p2)):
    flag += chr(((p2[i]-1)^p1[i])&0xff)
print(flag)
```


## AES 128/196/256
使用方法:
keylen: 128/196/256
T: 加解密次数
s: 明文/密文 0x开头 |s| = 34
k: 0x开头 密钥 |k| = {50,66}
op: 1:加密 0:解密

```py

class _AES_util():
    SBOX = [
        [0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76],
        [0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0],
        [0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15],
        [0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75],
        [0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84],
        [0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf],
        [0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8],
        [0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2],
        [0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73],
        [0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb],
        [0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79],
        [0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08],
        [0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a],
        [0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e],
        [0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf],
        [0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16]]

    SBOX_INV = [
        [0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb],
        [0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb],
        [0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e],
        [0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25],
        [0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92],
        [0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84],
        [0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06],
        [0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b],
        [0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73],
        [0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e],
        [0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b],
        [0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4],
        [0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f],
        [0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef],
        [0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61],
        [0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d]]

    def __init__(self) -> None:
        pass

    @classmethod
    def key_convert_to_matrix(cls, text, num):
        matrix = [[0 for i in range(4)] for j in range(num)]
        ptr = 0
        for i in range(num):
            for j in range(4):
                m = text[ptr] + text[ptr+1]
                matrix[i][j] = int(m, 16)
                ptr += 2
        # print(matrix)
        return matrix
    
    @classmethod
    def key_final(cls, key: list):
        matrix = []
        for k in range(len(key)//4):
            a = [[0 for i in range(4)] for j in range(4)]
            for i in range(4):
                for j in range(4):
                    a[j][i] = key[4*k+i][j]
            matrix.append(a)
        return matrix

    @classmethod
    def sbox(cls, word: int):
        row = word >> 4
        column = word & 0x0f
        return cls.SBOX[row][column]

    # convert the 32 bit string to 4x4 int matrix
    @classmethod
    def convert_to_matrix(cls, text):
        matrix = [[0 for i in range(4)] for j in range(4)]
        ptr = 0
        for i in range(4):
            for j in range(4):
                m = text[ptr] + text[ptr+1]
                matrix[j][i] = int(m, 16)
                ptr += 2
        # print(matrix)
        return matrix

    # convert the 4x4 int matrix to 32 bit string
    @classmethod
    def convert_to_string(cls, matrix):
        text = ""
        for i in range(4):
            for j in range(4):
                text += hex(matrix[j][i])[2:].rjust(2, '0')
        # print(text)
        return text

    # add round key
    @classmethod
    def add_round_key(cls, state, key):
        for i in range(4):
            for j in range(4):
                state[i][j] = state[i][j] ^ key[i][j]
        # print(state)
        return state

    # sub bytes
    @classmethod
    def sub_bytes(cls, state):
        for i in range(4):
            for j in range(4):
                row = state[i][j] >> 4
                column = state[i][j] & 0x0f
                state[i][j] = cls.SBOX[row][column]
        return state

    # shift rows
    @classmethod
    def shift_rows(cls, state):
        for i in range(1, 4):
            state[i] = state[i][i:] + state[i][:i]
        return state

    # multiply two numbers in GF(2^8)
    @classmethod
    def mul(cls, a, b):
        p = 0
        hi_bit_set = 0
        for i in range(8):
            if b & 1 == 1:
                p ^= a

            hi_bit_set = a & 0x80
            a <<= 1
            if hi_bit_set == 0x80:
                a ^= 0x1b
            b >>= 1
        return p % 256

    # mix columns
    @classmethod
    def mix_columns(cls, state):
        new_state = [[], [], [], []]

        for i in range(4):
            new_state[0].append(cls.mul(0x02, state[0][i]) ^ cls.mul(0x03, state[1][i]) ^ state[2][i] ^ state[3][i])
            new_state[1].append(state[0][i] ^ cls.mul(0x02, state[1][i]) ^ cls.mul(0x03, state[2][i]) ^ state[3][i])
            new_state[2].append(state[0][i] ^ state[1][i] ^ cls.mul(0x02, state[2][i]) ^ cls.mul(0x03, state[3][i]))
            new_state[3].append(cls.mul(0x03, state[0][i]) ^ state[1][i] ^ state[2][i] ^ cls.mul(0x02, state[3][i]))

        return new_state

    # inverse sub bytes
    @classmethod
    def inv_sub_bytes(cls, state):
        for i in range(4):
            for j in range(4):
                row = state[i][j] >> 4
                column = state[i][j] & 0x0f
                state[i][j] = cls.SBOX_INV[row][column]
        return state
    
    # inverse shift rows
    @classmethod
    def inv_shift_rows(cls, state):
        for i in range(1, 4):
            state[i] = state[i][-i:] + state[i][:-i]
        return state
    
    # inverse mix columns
    @classmethod
    def inv_mix_columns(cls, state):
        new_state = [[], [], [], []]
        for i in range(4):
            new_state[0].append(cls.mul(0x0e, state[0][i]) ^ cls.mul(0x0b, state[1][i]) ^ cls.mul(0x0d, state[2][i]) ^ cls.mul(0x09, state[3][i]))
            new_state[1].append(cls.mul(0x09, state[0][i]) ^ cls.mul(0x0e, state[1][i]) ^ cls.mul(0x0b, state[2][i]) ^ cls.mul(0x0d, state[3][i]))
            new_state[2].append(cls.mul(0x0d, state[0][i]) ^ cls.mul(0x09, state[1][i]) ^ cls.mul(0x0e, state[2][i]) ^ cls.mul(0x0b, state[3][i]))
            new_state[3].append(cls.mul(0x0b, state[0][i]) ^ cls.mul(0x0d, state[1][i]) ^ cls.mul(0x09, state[2][i]) ^ cls.mul(0x0e, state[3][i]))
        return new_state

class AES(_AES_util):
    def __init__(self, key: str):
        self.mode = len(key) * 4           # self.mode: int
        self.key = key                     # self.key : str
        self.subkey = self._key_expansion(key)
        # Nr: 加密轮数，值为10、12、14
        self.Nr = 10 + (self.mode - 128) // 32

    def _key_expansion(self, key: str):
        Rcon = [0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36]
        # Nr: 密钥拓展轮数，值为10、12、14
        Nr = 10 + (self.mode - 128) // 32
        # Nk: 密钥长度，值为4、6、8
        Nk = self.mode // 32
        # w: 密钥拓展结果，长度为4 * (Nr + 1)，存储类型为int
        w = [[0 for i in range(4)] for j in range(4 * (Nr+1))]

        key_m0 = _AES_util.key_convert_to_matrix(key, Nk)

        for i in range(Nk):
            w[i] = key_m0[i]
        
        for i in range(Nk, 4 * (Nr+1)):
            temp = w[i-1]
            if i % Nk == 0:
                temp = [temp[1], temp[2], temp[3], temp[0]]
                temp = [_AES_util.sbox(temp[j]) for j in range(4)]
                temp[0] ^= Rcon[i//Nk - 1]
            elif Nk > 6 and i % Nk == 4:
                temp = [_AES_util.sbox(temp[j]) for j in range(4)]
            w[i] = [ w[i - Nk][j] ^ temp[j] for j in range(4) ]
        
        return _AES_util.key_final(w)

    def _get_subkey(self, i: int):
        # for i in range(len(self.subkey)):
        #     print(i, self.subkey[i])
        return self.subkey[i]
    
    def encrypt(self, plaintext: str):
        """AES encryption algorithm
        plaintext is a 32-byte string
        during testing, the type of plaintext and key is int 
        """
        # convert plaintext and key to 4x4 matrix
        plaintext = _AES_util.convert_to_matrix(plaintext)
        # add round key
        state = _AES_util.add_round_key(plaintext, self._get_subkey(0))
        
        # 9 rounds
        for i in range(self.Nr-1):
            state = _AES_util.sub_bytes(state)                                  # 字节代替
            state = _AES_util.shift_rows(state)                                 # 行移位
            state = _AES_util.mix_columns(state)                                # 列混淆
            state = _AES_util.add_round_key(state, self._get_subkey(i+1))       # 轮密钥加
        
        # final round
        state = _AES_util.sub_bytes(state)
        state = _AES_util.shift_rows(state)
        state = _AES_util.add_round_key(state, self._get_subkey(self.Nr))

        # convert state to string
        ciphertext = _AES_util.convert_to_string(state)
        return ciphertext

    def decrypt(self, ciphertext: str):
        """AES decryption algorithm
        ciphertext is a 32-byte string
        """
        # convert ciphertext and key to 4x4 matrix
        ciphertext = _AES_util.convert_to_matrix(ciphertext)
        # add round key
        state = _AES_util.add_round_key(ciphertext, self._get_subkey(self.Nr))
        
        # 9 rounds
        for i in range(self.Nr-1):
            state = _AES_util.inv_shift_rows(state)
            state = _AES_util.inv_sub_bytes(state)
            state = _AES_util.add_round_key(state, self._get_subkey(self.Nr-i-1))
            state = _AES_util.inv_mix_columns(state)
        
        # final round
        state = _AES_util.inv_shift_rows(state)
        state = _AES_util.inv_sub_bytes(state)
        state = _AES_util.add_round_key(state, self._get_subkey(0))

        # convert state to string
        plaintext = _AES_util.convert_to_string(state)
        return plaintext


def main():
    keylen = int(input())
    num = int(input())
    msg = input().strip()[2:]
    key = input().strip()[2:]
    mode = int(input())
    aes = AES(key)
    if mode == 0:
        for i in range(num):
            msg = aes.decrypt(msg)
    else:
        for i in range(num):
            msg = aes.encrypt(msg)
    print('0x'+msg)
    
if __name__ == '__main__':
    main()
```


## AES白盒DFA恢复密钥
... qwb2023 dotdot

