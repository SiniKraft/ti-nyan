t3 = ""
with open("bg.c", 'r') as file:
    t = file.read()
    frames = 11
    t2 = ""
    for frame in range(0, frames):
        t2 = t.split("frame == " + str(frame + 1) + ") {\n        ")[1]
        t2 = t2.split("}")[0]
        t2 = t2.split("gfx_FillRectangle_NoClip(")
        for s in t2:
            t3 = t3 + (s.split(");")[0]) + "\n"
with open("result.txt", "w") as file2:
    file2.write(t3)