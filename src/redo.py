import ast

l = []
temp = ""

with open("bg.c", "r") as file:
    t = file.readlines()

for i in range(1, 12+1):
    for line in t:
        if line.startswith("    const uint8_t x_%s[]" % str(i)):
            temp = line.replace("    const uint8_t x_%s[] = " % str(i), "").replace(";", "").replace("{", "[").replace("}", "]")
            l.append(ast.literal_eval(temp))
temp2 = ""
for linee in l:
    for l2 in linee:
        l.append([l2])

with open("out.txt", "a") as f:
    f.write(str(l))