#!/bin/sh
wine ShaderTest.exe ./tests/add1/vertex.vsh ./tests/add1/pixel.psh ./tests/add1/OutputLinux.bmp
wine ShaderTest.exe ./tests/sub4/vertex.vsh ./tests/sub4/pixel.psh ./tests/sub4/OutputLinux.bmp
wine ShaderTest.exe ./tests/add4/vertex.vsh ./tests/add4/pixel.psh ./tests/add4/OutputLinux.bmp
wine ShaderTest.exe ./tests/add2/vertex.vsh ./tests/add2/pixel.psh ./tests/add2/OutputLinux.bmp
wine ShaderTest.exe ./tests/sub1/vertex.vsh ./tests/sub1/pixel.psh ./tests/sub1/OutputLinux.bmp
wine ShaderTest.exe ./tests/add3/vertex.vsh ./tests/add3/pixel.psh ./tests/add3/OutputLinux.bmp
wine ShaderTest.exe ./tests/sub2/vertex.vsh ./tests/sub2/pixel.psh ./tests/sub2/OutputLinux.bmp
wine ShaderTest.exe ./tests/sub3/vertex.vsh ./tests/sub3/pixel.psh ./tests/sub3/OutputLinux.bmp
echo "Checking add1 with Win32..."
./threshold.py compare -metric mae tests/add1/OutputLinux.bmp tests/add1/OutputWin32.bmp tests/add1/diff.png
if [ "$?" == "0" ]; then echo "-- OK"; else echo "-- Failed"; fi
echo "Checking sub4 with Win32..."
./threshold.py compare -metric mae tests/sub4/OutputLinux.bmp tests/sub4/OutputWin32.bmp tests/sub4/diff.png
if [ "$?" == "0" ]; then echo "-- OK"; else echo "-- Failed"; fi
echo "Checking add4 with Win32..."
./threshold.py compare -metric mae tests/add4/OutputLinux.bmp tests/add4/OutputWin32.bmp tests/add4/diff.png
if [ "$?" == "0" ]; then echo "-- OK"; else echo "-- Failed"; fi
echo "Checking add2 with Win32..."
./threshold.py compare -metric mae tests/add2/OutputLinux.bmp tests/add2/OutputWin32.bmp tests/add2/diff.png
if [ "$?" == "0" ]; then echo "-- OK"; else echo "-- Failed"; fi
echo "Checking sub1 with Win32..."
./threshold.py compare -metric mae tests/sub1/OutputLinux.bmp tests/sub1/OutputWin32.bmp tests/sub1/diff.png
if [ "$?" == "0" ]; then echo "-- OK"; else echo "-- Failed"; fi
echo "Checking add3 with Win32..."
./threshold.py compare -metric mae tests/add3/OutputLinux.bmp tests/add3/OutputWin32.bmp tests/add3/diff.png
if [ "$?" == "0" ]; then echo "-- OK"; else echo "-- Failed"; fi
echo "Checking sub2 with Win32..."
./threshold.py compare -metric mae tests/sub2/OutputLinux.bmp tests/sub2/OutputWin32.bmp tests/sub2/diff.png
if [ "$?" == "0" ]; then echo "-- OK"; else echo "-- Failed"; fi
echo "Checking sub3 with Win32..."
./threshold.py compare -metric mae tests/sub3/OutputLinux.bmp tests/sub3/OutputWin32.bmp tests/sub3/diff.png
if [ "$?" == "0" ]; then echo "-- OK"; else echo "-- Failed"; fi
