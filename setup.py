import urllib.request
import zipfile
import os
from pyunpack import Archive

def CloneRepo(repourl):
    os.system("git clone " + repourl)


def Unzip(filename, dest = None):
    archive_file = Archive(filename)
    if not dest:
        dest = filename.split('.')[0]
    if archive_file:
        if not os.path.exists(dest):
            os.mkdir(dest)
        archive_file.extractall(dest)

def Download(url, file_name = None):

    if not file_name:
        file_name = url.split('/')[-1]
    u = urllib.request.urlopen(url)
    f = open(file_name, 'wb')
    file_size = int(u.getheader("Content-Length"))
    print("Downloading: %s Bytes: %s" % (file_name, file_size))

    file_size_dl = 0
    block_sz = 8192
    while True:
        buffer = u.read(block_sz)
        if not buffer:
            break

        file_size_dl += len(buffer)
        f.write(buffer)
        status = "%10d  [%3.2f%%]" % (file_size_dl, file_size_dl * 100. / file_size)
        status = status;# + chr(8)*(len(status)+1)
        print(status, end='\r')
    f.close()
    return file_name

# gcc mipsel
gccmips = Download("https://static.grumpycoder.net/pixel/mips/g++-mipsel-none-elf-12.1.0.zip")
Unzip(gccmips, 'tools/g++-mipsel')
os.system("rm " + gccmips)

# gdb multiarch
gdb = Download('https://static.grumpycoder.net/pixel/gdb-multiarch-windows/gdb-multiarch-12.1.zip')
Unzip(gdb, 'tools/gdb-multiarch')
os.system("rm " + gdb)

# pcsx-redux
pcsxurl = "https://gamedevchallenge.000webhostapp.com/downloads/pcsx-redux-nightly-12395.20221220.3-x64.zip"
pcsx = Download(pcsxurl, 'pcsx-redux.zip')
Unzip(pcsx, 'tools/pcsx-redux')
os.system("rm " + pcsx)

# mkpsxiso
mkpsxiso = Download("https://github.com/Lameguy64/mkpsxiso/releases/download/v2.02/mkpsxiso-2.02-win64.zip")
Unzip(mkpsxiso, 'tools/mkpsxiso')
os.system("rm " + mkpsxiso)

# psyq converted libs
psyq = Download("https://psx.arthus.net/sdk/Psy-Q/psyq-4.7-converted-full.7z")
Unzip(psyq, "third_party/psyq")
os.system("rm " + psyq)


