##############################################################################
#
#  PROJECT:     Multi Theft Auto v1.0
#  LICENSE:     See LICENSE in the top level directory
#  FILE:        utils/build_gettext_catalog.py
#  PURPOSE:     Create a template .pot file from .cpp and .h project files for client and server
#  DEVELOPERS:  Dan Chowdhury <>
#
#  Multi Theft Auto is available from http://www.multitheftauto.com/
#
##############################################################################

import os
import subprocess
import tempfile
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-e", "--exe", dest="exe",
                  help="xgettext executable location", default="xgettext/xgettext.exe" )
#parser.add_option("-f", "--file", dest="output",
#                  help="POT File output directory", default="messages")
parser.add_option("-v", "--version", dest="version",
                  help="MTA:SA Version to write to the POT file", default="1.x")

(options, args) = parser.parse_args()

directories = {
    "client.pot" : [ "..\\Client", "..\\Shared" ],
    #"../Server/locale/client.pot" : [ "../Server", "../Shared" ],
}

# You MUST have a pootle_path for every `.pot` file,
# otherwise Pootle will not accept your pot
pootle_path = {
    "client.pot": "/templates/client/client.pot"
}

def pause():
    raw_input("\nPress enter to continue...")

def add_pot_header(filename, *headers):
    with open(filename, "r+") as f:
        sep = 'msgstr ""\n'
        content = f.read()
        content = content.split(sep, 1)
        for header in headers:
            content.insert(1, '"' + header + '\\n"\n')
        content.insert(1, sep)
        content = "".join(content)

        f.seek(0)
        f.write(content)
        f.truncate()

def main():
    # Verify pot
    for filename, _ in directories.iteritems():
        if filename not in pootle_path:
            print("pootle_path error! %s is missing." % filename)
            pause()
            return

    scanDirsList = []

    fd,temp_path = tempfile.mkstemp()

    # The objective here is to scan for all our .cpp's and .h's in each root directory
    # We then compile a list of these files into a temporary file, which is given to xgettext
    # xgettext then reads this list, and produces our template .po file which is renamed to .pot
    for output,dirList in directories.iteritems():
        scanDirsFile = open(temp_path, 'w')
        # Scan for .cpp and .h files
        for dir in dirList:
            for root,dirs,files in os.walk(dir):
                for file in files:
                    filename,ext = os.path.splitext(file)
                    if ext == ".c" or ext == ".cpp" or ext == ".h" or ext == ".hpp":
                        filePath = os.path.join(root,file)
                        print ( filePath )
                        # Add each file to a list
                        scanDirsList.append ( filePath + "\n" )
                    
        print ( "Files found: " + str(len(scanDirsList)) )

        # Write this to our temporary file
        scanDirsFile.writelines(scanDirsList)
        scanDirsFile.close()

        # Generate X-Pootle-Path
        x_pootle_path = "X-Pootle-Path: " + pootle_path[output]

        # If we have .pot in the destination, strip it (xgettext seems to append an extension regardless)
        path,ext = os.path.splitext(output)
        if ext == ".pot":
            output = path

        # Give xgettext our temporary file to produce our .po
        cmdArgs = [options.exe,"-f",os.path.abspath(scanDirsFile.name),"-d",output,
                "--c++","--from-code=UTF-8","--add-comments",
                "--keyword=_", "--keyword=_td", "--keyword=_tn:1,2", "--keyword=_tc:1c,2", "--keyword=_tcn:1c,2,3",
                "--package-name=MTA San Andreas","--package-version="+options.version]

        proc = subprocess.Popen(cmdArgs)
        stdout, stderr = proc.communicate()
        print stdout
        print stderr

        # Apply x_pootle_path
        add_pot_header(output+".po", x_pootle_path, "X-Pootle-Revision: 0")
        
        # Remove old pot
        if os.path.isfile(output + ".pot"):
            os.remove(output + ".pot")

        # Rename our template to .pot (xgettext always outputs .po)
        if os.path.isfile(output + ".po"):
            os.rename(output + ".po", output + ".pot")

    # Delete our temporary file
    os.close(fd)
    os.remove(temp_path)


    print ( "POT Generation Operation Complete" )

main()
