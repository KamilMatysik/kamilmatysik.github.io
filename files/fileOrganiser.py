
import os
import shutil

#Can change the dir here in order to do somewhere else
homeDir = os.path.expanduser("~/Downloads/")
os.chdir(homeDir)

#Listing image and code files endings
imgList = ["jpg", "jpeg", "png", "svg", "pdf"]
codeList = ["py", "c", "cpp", "cs", "html", "js", "css"]

#Creates a list of all things in the dir
filesInDir = os.listdir(path=".")

for i in filesInDir:
    counter = 1

    #Divinding by each '.' in term
    x = i.split(".")

    #If it has no extension, dont touch it
    if len(x) == 1:
        continue
    #If it starts with . (hidden file) then skip
    elif i[0] == ".":
        continue

    #Full name of file (without extension)
    name = ".".join(x[:-1])

    #Path to check/move files to
    path = os.path.join(x[-1], i)

    #Calling so no error later
    newName = name

    #Before creating the dir, check if one with the same name already exists
    if os.path.isdir(x[-1]):
        while(os.path.isfile(path)):
            newName = name + f"({counter})"
            path = os.path.join(x[-1], newName + "." + x[-1])
            counter += 1
        shutil.move(i, path)

    #In not, create the dir
    else:
        os.mkdir(x[-1])
        while(os.path.isfile(path)):
            newName = name + f"({counter})"
            path = os.path.join(x[-1], newName + "." + x[-1])
            counter += 1
        shutil.move(i, path)

#Relisting files since I changed it
filesInDir = os.listdir(path=".")

#See if the dirs exist, if not create them
if os.path.isdir("images") == False:
    os.mkdir("images")
if os.path.isdir("coding") == False:
    os.mkdir("coding")

existingInImages = []
existingInCoding = []

for root, dirs, files in os.walk("images/"):
    for dir in dirs:
        existingInImages.append(dir)

for root, dirs, files in os.walk("coding/"):
    for dir in dirs:
        existingInCoding.append(dir)

for file in filesInDir:
    ext = file.split(".")[-1]
    if ext in imgList:
        target = os.path.join("images",ext)
        if ext in existingInImages:
            pass
        else:
            os.mkdir(target)

        shutil.move(file, target)
    elif ext in codeList:
        target = os.path.join("coding",ext)
        if ext in existingInCoding:
            pass
        else:
            os.mkdir(target)

        shutil.move(file, target)
