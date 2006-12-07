# Microsoft Developer Studio Project File - Name="Opennx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Opennx - Win32 Release Unicode DLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Opennx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Opennx.mak" CFG="Opennx - Win32 Release Unicode DLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Opennx - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Opennx - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Opennx - Win32 Debug Unicode" (based on "Win32 (x86) Console Application")
!MESSAGE "Opennx - Win32 Release Unicode" (based on "Win32 (x86) Console Application")
!MESSAGE "Opennx - Win32 Debug DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "Opennx - Win32 Release DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "Opennx - Win32 Debug Unicode DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "Opennx - Win32 Release Unicode DLL" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Opennx - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "$(WXWIN)\contrib\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_MT" /D "_WINDOWS" /D "__WINDOWS__" /D "__WIN32__" /D WINVER=0x0400 /D "__WXMSW__" /D wxUSE_GUI=1 /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x409 /i "$(WXWIN)\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 wxzlib.lib wxpng.lib wxtiff.lib wxjpeg.lib wxexpat.lib wxregex.lib wxbase26.lib wxmsw26_core.lib wxbase26_xml.lib wxmsw26_xrc.lib wxbase26_net.lib wxmsw26_adv.lib wxmsw26_html.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib version.lib /nologo /subsystem:windows /machine:I386 /libpath:"$(WXWIN)\lib\vc_lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Opennx - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "$(WXWIN)\contrib\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_MT" /D "_WINDOWS" /D "__WINDOWS__" /D "__WIN32__" /D WINVER=0x0400 /D "__WXMSW__" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(WXWIN)\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wxzlibd.lib wxpngd.lib wxtiffd.lib wxjpegd.lib wxexpatd.lib wxregexd.lib wxbase26d.lib wxmsw26d_core.lib wxbase26d_xml.lib wxmsw26d_xrc.lib wxbase26d_net.lib wxmsw26d_adv.lib wxmsw26d_html.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"$(WXWIN)\lib\vc_lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Opennx - Win32 Debug Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Opennx___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "Opennx___Win32_Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "$(WXWIN)\contrib\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_UNICODE" /D "_MT" /D "_WINDOWS" /D "__WINDOWS__" /D "__WIN32__" /D WINVER=0x0400 /D "__WXMSW__" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(WXWIN)\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wxzlibd.lib wxpngd.lib wxtiffd.lib wxjpegd.lib wxexpatd.lib wxregexud.lib wxbase26ud.lib wxmsw26ud_core.lib wxbase26ud_xml.lib wxmsw26ud_xrc.lib wxbase26ud_net.lib wxmsw26ud_adv.lib wxmsw26ud_html.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"$(WXWIN)\lib\vc_lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Opennx - Win32 Release Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Opennx___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "Opennx___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release_Unicode"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "$(WXWIN)\contrib\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "NDEBUG" /D "_UNICODE" /D "_MT" /D "_WINDOWS" /D "__WINDOWS__" /D "__WIN32__" /D WINVER=0x0400 /D "__WXMSW__" /D wxUSE_GUI=1 /YX /FD /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(WXWIN)\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wxzlib.lib wxpng.lib wxtiff.lib wxjpeg.lib wxexpat.lib wxregexu.lib wxbase26u.lib wxmsw26u_core.lib wxbase26u_xml.lib wxmsw26u_xrc.lib wxbase26u_net.lib wxmsw26u_adv.lib wxmsw26u_html.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib version.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"$(WXWIN)\lib\vc_lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Opennx - Win32 Debug DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Opennx___Win32_Debug_DLL"
# PROP BASE Intermediate_Dir "Opennx___Win32_Debug_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDLL"
# PROP Intermediate_Dir "DebugDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "$(WXWIN)\contrib\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_MT" /D "_WINDOWS" /D "__WINDOWS__" /D "__WIN32__" /D WINVER=0x0400 /D "__WXMSW__" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /D WXUSINGDLL=1 /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(WXWIN)\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wxzlibd.lib wxpngd.lib wxtiffd.lib wxjpegd.lib wxexpatd.lib wxregexd.lib wxbase26d.lib wxmsw26d_core.lib wxbase26d_xml.lib wxmsw26d_xrc.lib wxbase26d_net.lib wxmsw26d_adv.lib wxmsw26d_html.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"$(WXWIN)\lib\vc_dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Opennx - Win32 Release DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Opennx___Win32_Release_DLL"
# PROP BASE Intermediate_Dir "Opennx___Win32_Release_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ReleaseDLL"
# PROP Intermediate_Dir "ReleaseDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "$(WXWIN)\contrib\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "NDEBUG" /D "_MT" /D "_WINDOWS" /D "__WINDOWS__" /D "__WIN32__" /D WINVER=0x0400 /D "__WXMSW__" /D wxUSE_GUI=1 /D WXUSINGDLL=1 /YX /FD /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(WXWIN)\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wxzlib.lib wxpng.lib wxtiff.lib wxjpeg.lib wxexpat.lib wxregex.lib wxbase26.lib wxmsw26_core.lib wxbase26_xml.lib wxmsw26_xrc.lib wxbase26_net.lib wxmsw26_adv.lib wxmsw26_html.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib version.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"$(WXWIN)\lib\vc_dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Opennx - Win32 Debug Unicode DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Opennx___Win32_Debug_Unicode_DLL"
# PROP BASE Intermediate_Dir "Opennx___Win32_Debug_Unicode_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDLL_Unicode"
# PROP Intermediate_Dir "DebugDLL_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "$(WXWIN)\contrib\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_UNICODE" /D "_MT" /D "_WINDOWS" /D "__WINDOWS__" /D "__WIN32__" /D WINVER=0x0400 /D "__WXMSW__" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /D WXUSINGDLL=1 /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(WXWIN)\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wxzlibd.lib wxpngd.lib wxtiffd.lib wxjpegd.lib wxexpatd.lib wxregexud.lib wxbase26ud.lib wxmsw26ud_core.lib wxbase26ud_xml.lib wxmsw26ud_xrc.lib wxbase26ud_net.lib wxmsw26ud_adv.lib wxmsw26ud_html.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"$(WXWIN)\lib\vc_dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Opennx - Win32 Release Unicode DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Opennx___Win32_Release_Unicode_DLL"
# PROP BASE Intermediate_Dir "Opennx___Win32_Release_Unicode_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ReleaseDLL_Unicode"
# PROP Intermediate_Dir "ReleaseDLL_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "$(WXWIN)\contrib\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "NDEBUG" /D "_UNICODE" /D "_MT" /D "_WINDOWS" /D "__WINDOWS__" /D "__WIN32__" /D WINVER=0x0400 /D "__WXMSW__" /D wxUSE_GUI=1 /D WXUSINGDLL=1 /YX /FD /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(WXWIN)\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wxzlib.lib wxpng.lib wxtiff.lib wxjpeg.lib wxexpat.lib wxregexu.lib wxbase26u.lib wxmsw26u_core.lib wxbase26u_xml.lib wxmsw26u_xrc.lib wxbase26u_net.lib wxmsw26u_adv.lib wxmsw26u_html.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib version.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"$(WXWIN)\lib\vc_dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Opennx - Win32 Release"
# Name "Opennx - Win32 Debug"
# Name "Opennx - Win32 Debug Unicode"
# Name "Opennx - Win32 Release Unicode"
# Name "Opennx - Win32 Debug DLL"
# Name "Opennx - Win32 Release DLL"
# Name "Opennx - Win32 Debug Unicode DLL"
# Name "Opennx - Win32 Release Unicode DLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Icon.cpp
# End Source File
# Begin Source File

SOURCE=.\LogDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\md5.c
# End Source File
# Begin Source File

SOURCE=.\Opennx.pjd

!IF  "$(CFG)" == "Opennx - Win32 Release"

# Begin Custom Build
IntDir=.\Release
InputPath=.\Opennx.pjd

"memres.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(IntDir)\tmpres.zip res -i *.xrc *.png *.rtf *.html -x *.prv.* 
	bin2hdr $(IntDir)\tmpres.zip memres.h mem_res 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Opennx - Win32 Debug"

# Begin Custom Build
IntDir=.\Debug
InputPath=.\Opennx.pjd

"memres.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(IntDir)\tmpres.zip res -i *.xrc *.png *.rtf *.html -x *.prv.* 
	bin2hdr $(IntDir)\tmpres.zip memres.h mem_res 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Opennx - Win32 Debug Unicode"

# Begin Custom Build
IntDir=.\Debug_Unicode
InputPath=.\Opennx.pjd

"memres.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(IntDir)\tmpres.zip res -i *.xrc *.png *.rtf *.html -x *.prv.* 
	bin2hdr $(IntDir)\tmpres.zip memres.h mem_res 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Opennx - Win32 Release Unicode"

# Begin Custom Build
IntDir=.\Release_Unicode
InputPath=.\Opennx.pjd

"memres.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(IntDir)\tmpres.zip res -i *.xrc *.png *.rtf *.html -x *.prv.* 
	bin2hdr $(IntDir)\tmpres.zip memres.h mem_res 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Opennx - Win32 Debug DLL"

# Begin Custom Build
IntDir=.\DebugDLL
InputPath=.\Opennx.pjd

"memres.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(IntDir)\tmpres.zip res -i *.xrc *.png *.rtf *.html -x *.prv.* 
	bin2hdr $(IntDir)\tmpres.zip memres.h mem_res 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Opennx - Win32 Release DLL"

# Begin Custom Build
IntDir=.\ReleaseDLL
InputPath=.\Opennx.pjd

"memres.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(IntDir)\tmpres.zip res -i *.xrc *.png *.rtf *.html -x *.prv.* 
	bin2hdr $(IntDir)\tmpres.zip memres.h mem_res 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Opennx - Win32 Debug Unicode DLL"

# Begin Custom Build
IntDir=.\DebugDLL_Unicode
InputPath=.\Opennx.pjd

"memres.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(IntDir)\tmpres.zip res -i *.xrc *.png *.rtf *.html -x *.prv.* 
	bin2hdr $(IntDir)\tmpres.zip memres.h mem_res 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Opennx - Win32 Release Unicode DLL"

# Begin Custom Build
IntDir=.\ReleaseDLL_Unicode
InputPath=.\Opennx.pjd

"memres.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(IntDir)\tmpres.zip res -i *.xrc *.png *.rtf *.html -x *.prv.* 
	bin2hdr $(IntDir)\tmpres.zip memres.h mem_res 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Opennx.rc
# End Source File
# Begin Source File

SOURCE=.\OpennxApp.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=.\MxIPC.cpp
# End Source File
# Begin Source File

SOURCE=.\MxSession.cpp
# End Source File
# Begin Source File

SOURCE=.\MxValidator.cpp
# End Source File
# Begin Source File

SOURCE=.\MxWizard.cpp
# End Source File
# Begin Source File

SOURCE=.\MxXmlConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\pwcrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\RdpImageSettingsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RdpPropertyDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionAdmin.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionList.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\SmbShareProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\UnixImageSettingsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\VncImageSettingsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\VncPropertyDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WinShare.cpp
# End Source File
# Begin Source File

SOURCE=.\wxprec.cpp
# End Source File
# Begin Source File

SOURCE=.\X11PropertyDialog.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDialog.h
# End Source File
# Begin Source File

SOURCE=.\AboutDialog_symbols.h
# End Source File
# Begin Source File

SOURCE=.\ConnectDialog.h
# End Source File
# Begin Source File

SOURCE=.\ConnectDialog_symbols.h
# End Source File
# Begin Source File

SOURCE=.\debugleak.h
# End Source File
# Begin Source File

SOURCE=.\Icon.h
# End Source File
# Begin Source File

SOURCE=.\LogDialog.h
# End Source File
# Begin Source File

SOURCE=.\LogDialog_symbols.h
# End Source File
# Begin Source File

SOURCE=.\LoginDialog.h
# End Source File
# Begin Source File

SOURCE=.\LoginDialog_symbols.h
# End Source File
# Begin Source File

SOURCE=.\md5.h
# End Source File
# Begin Source File

SOURCE=.\OpennxApp.h
# End Source File
# Begin Source File

SOURCE=.\MxIPC.h
# End Source File
# Begin Source File

SOURCE=.\MxSession.h
# End Source File
# Begin Source File

SOURCE=.\MxValidator.h
# End Source File
# Begin Source File

SOURCE=.\MxWizard.h
# End Source File
# Begin Source File

SOURCE=.\MxWizard_symbols.h
# End Source File
# Begin Source File

SOURCE=.\MxXmlConfig.h
# End Source File
# Begin Source File

SOURCE=.\pwcrypt.h
# End Source File
# Begin Source File

SOURCE=.\RdpImageSettingsDialog.h
# End Source File
# Begin Source File

SOURCE=.\RdpImageSettingsDialog_symbols.h
# End Source File
# Begin Source File

SOURCE=.\RdpPropertyDialog.h
# End Source File
# Begin Source File

SOURCE=.\RdpPropertyDialog_symbols.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SessionAdmin.h
# End Source File
# Begin Source File

SOURCE=.\SessionAdmin_symbols.h
# End Source File
# Begin Source File

SOURCE=.\SessionList.h
# End Source File
# Begin Source File

SOURCE=.\SessionProperties.h
# End Source File
# Begin Source File

SOURCE=.\SessionProperties_symbols.h
# End Source File
# Begin Source File

SOURCE=.\SmbShareProperties.h
# End Source File
# Begin Source File

SOURCE=.\SmbShareProperties_symbols.h
# End Source File
# Begin Source File

SOURCE=.\UnixImageSettingsDialog.h
# End Source File
# Begin Source File

SOURCE=.\UnixImageSettingsDialog_symbols.h
# End Source File
# Begin Source File

SOURCE=.\VncImageSettingsDialog.h
# End Source File
# Begin Source File

SOURCE=.\VncImageSettingsDialog_symbols.h
# End Source File
# Begin Source File

SOURCE=.\VncPropertyDialog.h
# End Source File
# Begin Source File

SOURCE=.\VncPropertyDialog_symbols.h
# End Source File
# Begin Source File

SOURCE=.\WinShare.h
# End Source File
# Begin Source File

SOURCE=.\X11PropertyDialog.h
# End Source File
# Begin Source File

SOURCE=.\X11PropertyDialog_symbols.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\admin.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\blank.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\bullseye.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\cdrom.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\colours.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\computer.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\csquery.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\disable.bmp
# End Source File
# Begin Source File

SOURCE=.\wx\msw\drive.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\file1.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\floppy.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\folder1.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\folder2.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\hand.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\magnif1.cur
# End Source File
# Begin Source File

SOURCE=.\res\Opennx.xrc
# End Source File
# Begin Source File

SOURCE=.\wx\msw\noentry.cur
# End Source File
# Begin Source File

SOURCE=".\res\nx-desktop.ico"
# End Source File
# Begin Source File

SOURCE=.\res\nx.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pbrush.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pencil.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pntleft.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pntright.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\query.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\removble.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\rightarr.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\roller.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\watch1.cur
# End Source File
# Begin Source File

SOURCE=.\res\wizard.ico
# End Source File
# End Group
# End Target
# End Project
