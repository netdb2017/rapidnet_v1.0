import sys
import glob
import subprocess

lines = []
for line in sys.stdin.readlines():
    lines.append(line)


isSuccesful = "\'build\' finished successfully"

if isSuccesful in lines[-1]:
    print "Build successful"
    sys.stderr.write(''.join(lines))
else:
    print "Build failed"

    print "Checking if Tools/python.py needs to be changed"

    if "for incstr in Utils.cmd_output(\"%s %s --includes\"%(python,python_config)).strip().split():" in '\n'.join(lines):
        print "Tools/python.py needs to be changed.....changing python.py"
        waf_directory=glob.glob(".waf-*/")
        print "Opening", waf_directory[0]+"wafadmin/Tools/python.py"
        python_file = open(waf_directory[0]+"wafadmin/Tools/python.py","r")
        file_lines = python_file.readlines()
        python_file.close()

        #Update line 190
        print "Updating line 190","for incstr in Utils.cmd_output(\"%s %s --includes\"%(python,python_config)).strip().split():","----->","for incstr in Utils.cmd_output(\"%s --includes\"%(python_config)).strip().split():"
        file_lines[189]=file_lines[189].replace("for incstr in Utils.cmd_output(\"%s %s --includes\"%(python,python_config)).strip().split():","for incstr in Utils.cmd_output(\"%s --includes\"%(python_config)).strip().split():")

        python_file = open(waf_directory[0]+"wafadmin/Tools/python.py","w")
        python_file.write(''.join(file_lines))
        python_file.close()

        "Rerun waf"
        subprocess.call(["./waf"])
