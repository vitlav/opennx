# Microsoft Developer Studio Project File - Name="Translations" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=Translations - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Translations.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Translations.mak" CFG="Translations - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Translations - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "Translations - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE "Translations - Win32 Debug Unicode" (based on "Win32 (x86) Generic Project")
!MESSAGE "Translations - Win32 Release Unicode" (based on "Win32 (x86) Generic Project")
!MESSAGE "Translations - Win32 Release Unicode DLL" (based on "Win32 (x86) Generic Project")
!MESSAGE "Translations - Win32 Release DLL" (based on "Win32 (x86) Generic Project")
!MESSAGE "Translations - Win32 Debug Unicode DLL" (based on "Win32 (x86) Generic Project")
!MESSAGE "Translations - Win32 Debug DLL" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "Translations - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Translations___Win32_Release"
# PROP BASE Intermediate_Dir "Translations___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "locale"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Translations - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Translations___Win32_Debug"
# PROP BASE Intermediate_Dir "Translations___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "locale"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Translations - Win32 Debug Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug Unicode"
# PROP BASE Intermediate_Dir "Debug Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "locale"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Translations - Win32 Release Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release Unicode"
# PROP BASE Intermediate_Dir "Release Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "locale"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Translations - Win32 Release Unicode DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Translations___Win32_Release_Unicode_DLL"
# PROP BASE Intermediate_Dir "Translations___Win32_Release_Unicode_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "locale"
# PROP Intermediate_Dir "ReleaseDLL_Unicode"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Translations - Win32 Release DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Translations___Win32_Release_DLL"
# PROP BASE Intermediate_Dir "Translations___Win32_Release_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "locale"
# PROP Intermediate_Dir "ReleaseDLL"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Translations - Win32 Debug Unicode DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Translations___Win32_Debug_Unicode_DLL"
# PROP BASE Intermediate_Dir "Translations___Win32_Debug_Unicode_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "locale"
# PROP Intermediate_Dir "DebugDLL_Unicode"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Translations - Win32 Debug DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Translations___Win32_Debug_DLL"
# PROP BASE Intermediate_Dir "Translations___Win32_Debug_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "locale"
# PROP Intermediate_Dir "DebugDLL"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "Translations - Win32 Release"
# Name "Translations - Win32 Debug"
# Name "Translations - Win32 Debug Unicode"
# Name "Translations - Win32 Release Unicode"
# Name "Translations - Win32 Release Unicode DLL"
# Name "Translations - Win32 Release DLL"
# Name "Translations - Win32 Debug Unicode DLL"
# Name "Translations - Win32 Debug DLL"
# Begin Group "Source Files"

# PROP Default_Filter "po;pot"
# Begin Source File

SOURCE=.\po\de.po

!IF  "$(CFG)" == "Translations - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
OutDir=.\locale
InputPath=.\po\de.po
InputName=de

"$(OutDir)/$(InputName)/mxclient.mo" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if not exist $(OutDir)\$(InputName) mkdir $(OutDir)\$(InputName) 
	msgfmt -c -o $(OutDir)/$(InputName)/mxclient.mo $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Translations - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
OutDir=.\locale
InputPath=.\po\de.po
InputName=de

"$(OutDir)/$(InputName)/mxclient.mo" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if not exist $(OutDir)\$(InputName) mkdir $(OutDir)\$(InputName) 
	msgfmt -c -o $(OutDir)/$(InputName)/mxclient.mo $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Translations - Win32 Debug Unicode"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
OutDir=.\locale
InputPath=.\po\de.po
InputName=de

"$(OutDir)/$(InputName)/mxclient.mo" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if not exist $(OutDir)\$(InputName) mkdir $(OutDir)\$(InputName) 
	msgfmt -c -o $(OutDir)/$(InputName)/mxclient.mo $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Translations - Win32 Release Unicode"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
OutDir=.\locale
InputPath=.\po\de.po
InputName=de

"$(OutDir)/$(InputName)/mxclient.mo" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if not exist $(OutDir)\$(InputName) mkdir $(OutDir)\$(InputName) 
	msgfmt -c -o $(OutDir)/$(InputName)/mxclient.mo $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Translations - Win32 Release Unicode DLL"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build
OutDir=.\locale
InputPath=.\po\de.po
InputName=de

"$(OutDir)/$(InputName)/mxclient.mo" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if not exist $(OutDir)\$(InputName) mkdir $(OutDir)\$(InputName) 
	msgfmt -c -o $(OutDir)/$(InputName)/mxclient.mo $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Translations - Win32 Release DLL"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build
OutDir=.\locale
InputPath=.\po\de.po
InputName=de

"$(OutDir)/$(InputName)/mxclient.mo" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if not exist $(OutDir)\$(InputName) mkdir $(OutDir)\$(InputName) 
	msgfmt -c -o $(OutDir)/$(InputName)/mxclient.mo $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Translations - Win32 Debug Unicode DLL"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build
OutDir=.\locale
InputPath=.\po\de.po
InputName=de

"$(OutDir)/$(InputName)/mxclient.mo" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if not exist $(OutDir)\$(InputName) mkdir $(OutDir)\$(InputName) 
	msgfmt -c -o $(OutDir)/$(InputName)/mxclient.mo $(InputPath) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Translations - Win32 Debug DLL"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build
OutDir=.\locale
InputPath=.\po\de.po
InputName=de

"$(OutDir)/$(InputName)/mxclient.mo" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if not exist $(OutDir)\$(InputName) mkdir $(OutDir)\$(InputName) 
	msgfmt -c -o $(OutDir)/$(InputName)/mxclient.mo $(InputPath) 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
