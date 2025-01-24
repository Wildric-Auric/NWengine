import os


outDir = "..\\\\Template\\\\"
libDir = outDir + "lib\\\\"
hdrDir = outDir + "include\\\\"


def makeDir():
    os.system("mkdir " + outDir)
    os.system("mkdir " + libDir)
    os.system("mkdir " + hdrDir)


def genHdrs():
    os.system(f"mkdir {hdrDir}\\temp")
    for root, dirs, files in os.walk("..\\Engine"):
        for file in files:
            filepath = os.path.join(root, file)
            dst = f"{hdrDir}\\temp\\{file}"
            if (len(file) > 2 and file[len(file) - 2::] == ".h") or (len(file) > 4 and file[len(file) - 4::] == ".hpp"):
                os.system(f'copy "{filepath}" "{dst}"')

#    os.system(f"xcopy ..\\dependencies\\freetype\\include {hdrDir}\\temp /s /e /i")
#    os.system(f"xcopy ..\\dependencies\\GLEW\\include {hdrDir}\\temp /s /e /i")
#    os.system(f"xcopy ..\\dependencies\\OPENAL\\include {hdrDir}\\temp /s /e /i")
#    os.system(f"xcopy ..\\dependencies\\SND\\include {hdrDir}\\temp /s /e /i")
#    os.system(f"xcopy ..\\dependencies\\vendor\\stb {hdrDir}\\temp  /s /e /i")

    asm = f'hdrasm "{hdrDir}temp\\\\" "{hdrDir}" NWengineCore.h'
    os.system(asm)
    os.system(f"rm -rf {hdrDir}\\temp")


def buildBin():
    version = '"1.0.234567"'
    mac = f';PreprocessorDefinitions="NW_VERSION={version};$(PreprocessorDefinitions)"'
    dbg = "msbuild ..\\Build\\NWengineCore.vcxproj -p:Configuration=Debug;Platform=x64"
    rls = "msbuild ..\\Build\\NWengineCore.vcxproj -p:Configuration=Release;Platform=x64"
    dbg += mac
    rls += mac
    os.system(dbg)
    os.system(rls)

    os.system("copy ..\\Build\\Bin\\NWengineCore\\Debug64\\NWengineCore64d.idb "
              + libDir + "NWengineCore64d.idb")
    os.system("copy ..\\Build\\Bin\\NWengineCore\\Debug64\\NWengineCore64d.lib "
              + libDir + "NWengineCore64d.lib")
    os.system("copy ..\\Build\\Bin\\NWengineCore\\Debug64\\NWengineCore64d.pdb "
              + libDir + "NWengineCore64d.pdb")
    os.system("copy ..\\Build\\Bin\\NWengineCore\\Release64\\NWengineCore64.lib "
              + libDir + "NWengineCore64.lib")


makeDir()
buildBin()
genHdrs()
