import os
outf = open("run.sh","w")
outf.write("#!/bin/sh\n")
tests = os.listdir("tests")
for x in tests:
  outf.write("wine ShaderTest.exe %s %s %s\n"%("./tests/"+x+"/vertex.vsh","./tests/"+x+"/pixel.psh","./tests/"+x+"/OutputLinux.bmp"))



for x in tests:
  outf.write("echo \"Checking %s with Win32...\"\n"%x)
  outf.write("./threshold.py compare -metric mae tests/%s/OutputLinux.bmp tests/%s/OutputWin32.bmp null:\n"%(x,x));
  outf.write("if [ \"$?\" == \"0\" ]; then echo \"-- OK\"; else echo \"-- Failed\"; fi\n");
outf.close()
os.chmod("run.sh",448)