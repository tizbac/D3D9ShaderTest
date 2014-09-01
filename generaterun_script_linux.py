import os
outf = open("run.sh","w")
outf.write("#!/bin/sh\n")
tests = os.listdir("tests")
for x in tests:
  outf.write("wine ShaderTest.exe %s %s %s\n"%("./tests/"+x+"/vertex.vsh","./tests/"+x+"/pixel.psh","./tests/"+x+"/OutputLinux.bmp"))



for x in tests:
  outf.write("echo \"Checking %s with Win32...\"\n"%x)
  outf.write("./threshold.py compare -metric mae tests/%s/OutputLinux.bmp tests/%s/OutputWin32.bmp null:\n"%(x,x));
  outf.write("if [ \"$?\" == \"0\" ]; then echo -e \"-- \\033[32mOK\\033[0m\"; else echo -e \"\\033[31m-- Failed\\033[0m\"; fi\n");
outf.close()
os.chmod("run.sh",448)