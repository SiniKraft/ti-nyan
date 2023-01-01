t3 = ""
t2 = "{"
t = []
count = 0
with open("result.txt", 'r') as file:
    t = file.readlines()
    for line in t:
        if line != "\n":
            t2 = t2 + line.split("\n")[0].split(",")[3] + ", "
        else:
            t2 = t2 + "}\n{"


    
with open("result2.txt", "w") as file2:
    file2.write(t2)