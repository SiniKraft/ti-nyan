a = input("")
a = a.replace("            ", "").replace("gfx_FillRectangle_NoClip(", "{").replace(");", "},")
print(a)