import os
outf = open("run.bat","w")

tests = os.listdir("tests")
for x in tests:
  outf.write("ShaderTest.exe %s %s %s\n"%(".\\tests\\"+x+"\\vertex.vsh",".\\tests\\"+x+"\\pixel.psh",".\\tests\\"+x+"\\OutputWin32.bmp"))


