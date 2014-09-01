#!/usr/bin/python
import sys
import commands
out = commands.getoutput(" ".join(sys.argv[1:]))
out = out.split(" ")[0]
print out
if float(out) > 10.0:
  quit(1)
else:
  quit(0)