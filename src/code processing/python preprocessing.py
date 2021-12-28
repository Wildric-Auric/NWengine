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
        


def AutoComplete(path):
     state = -1
     lines = []
     l = ["Image", "Texture", "Shader", "GameObject"]
     textures, images, shaders, objects = [],[],[],[]
     info = {
         l[0]: images,
         l[1]: textures,
         l[2]: shaders, 
         l[3]: objects
         }
     with open(path, "r") as file:
         lines = file.readlines()
         
     alll = ""
     for line in lines:
         alll += (state == -1)*line
         if "LoadImages" in line: state = 0
         elif "LoadTextures" in line: state = 1
         elif "LoadShaders" in line: state = 2
         elif "LoadGameObjects" in line: state = 3
         elif "=" in line : 
             temp = line[:line.find("=")].strip()
             temp1 = temp.replace("*", "")
             temp = temp1
             temp1 += " "
             if state == 0 and "Image" in line:
                 if temp1 in alll: 
                     continue
                 images.append( temp)
             elif state == 1 and "Texture" in line:
                 if temp1 in alll: 
                     continue
                 textures.append( temp )
             elif state == 2 and "Shader":
                 if temp1 in alll: 
                     continue
                 shaders.append( temp )
             elif state == 3 and "GameObject" in line:
                 if temp1 in alll: 
                     continue
                 objects.append( temp )
     with open(path, "w+") as file:
         current = 0
         write = ""

         for line in lines:
             if current < len(l):
                 if l[current] and "malloc" in line:
                     added = 0
                     for i in info[l[current]]:
                         added = 1
                         write += "\n{}* {} = ({}*)malloc(sizeof({}));".format(l[current],i, l[current], l[current].replace("*", ""))
                     current +=1
                     write += "\n"*added
             write += line
         file.write(write)


AutoComplete(PATH)
Organize(PATH)
