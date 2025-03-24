key = "letsplaychess"

with open("file","rb+") as f:
    data = f.read()
dec_data = []
for i in range(len(data)):
    dec_data.append(data[i]^0xFF^ord(key[i%len(key)]))

f = open("dec_file","wb+")
f.write(bytes(dec_data))
f.close()