def calculate(name,orga,usernum):
    serial = 0
    for i in range(len(orga)):
        serial += ord(orga[i]) * ord(name[i])
    return serial
    

print(calculate("N0zoM1z0123","Kitauji",114514))

print(0xE6898150)