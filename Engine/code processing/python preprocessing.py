PATH = r"C:\Users\HP\Desktop\OpenGL_TopDownShooter\src\RessourcesLoader.cpp"
HEADER_PATH = r"C:\Users\HP\Desktop\OpenGL_TopDownShooter\src\RessourcesManager.h"


def Organize(path):
    lines = []
    length = 35
    spaceAfter = 4
    write = ""
    with open(path, "r") as file:
        lines = file.readlines()
    for line in lines:
        current = 0
        space = 0
        for letter in line:
            if space and (letter == " " or letter == "\t"):
                continue
            space = 0
            if letter!= "=": write += letter
            else: 
                i = -1;
                space = 1;
                while write[i] == " " or write[i] == "\t":
                    i-=1
                    current -=1 
                if i != -1:
                    write = write[:i+1] + (max(length-current, 1))* " " + "=" + spaceAfter*" "
                else:
                    write += (max(length-current, 1))* " " + "=" + spaceAfter*" "
            current+=1
    with open(path, "w+") as file:
        file.write(write)
        
Organize(PATH)
