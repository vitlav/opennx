# Microsoft Developer Studio Project File - Name="PackedResources" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=PackedResources - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PackedResources.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PackedResources.mak" CFG="PackedResources - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PackedResources - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "PackedResources - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE "PackedResources - Win32 Debug Unicode" (based on "Win32 (x86) Generic Project")
!MESSAGE "PackedResources - Win32 Release Unicode" (based on "Win32 (x86) Generic Project")
!MESSAGE "PackedResources - Win32 Release Unicode DLL" (based on "Win32 (x86) Generic Project")
!MESSAGE "PackedResources - Win32 Debug Unicode DLL" (based on "Win32 (x86) Generic Project")
!MESSAGE "PackedResources - Win32 Release DLL" (based on "Win32 (x86) Generic Project")
!MESSAGE "PackedResources - Win32 Debug DLL" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "PackedResources - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Packed_Resources___Win32_Release"
# PROP BASE Intermediate_Dir "Packed_Resources___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Packed_Resources___Win32_Debug"
# PROP BASE Intermediate_Dir "Packed_Resources___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Debug Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Packed_Resources___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "Packed_Resources___Win32_Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Release Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Packed_Resources___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "Packed_Resources___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Release Unicode DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Packed_Resources___Win32_Release_Unicode_DLL"
# PROP BASE Intermediate_Dir "Packed_Resources___Win32_Release_Unicode_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseDLL_Unicode"
# PROP Intermediate_Dir "ReleaseDLL_Unicode"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Debug Unicode DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Packed_Resources___Win32_Debug_Unicode_DLL"
# PROP BASE Intermediate_Dir "Packed_Resources___Win32_Debug_Unicode_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DebugDLL_Unicode"
# PROP Intermediate_Dir "DebugDLL_Unicode"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Release DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Packed_Resources___Win32_Release_DLL"
# PROP BASE Intermediate_Dir "Packed_Resources___Win32_Release_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseDLL"
# PROP Intermediate_Dir "ReleaseDLL"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Debug DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Packed_Resources___Win32_Debug_DLL"
# PROP BASE Intermediate_Dir "Packed_Resources___Win32_Debug_DLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DebugDLL"
# PROP Intermediate_Dir "DebugDLL"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "PackedResources - Win32 Release"
# Name "PackedResources - Win32 Debug"
# Name "PackedResources - Win32 Debug Unicode"
# Name "PackedResources - Win32 Release Unicode"
# Name "PackedResources - Win32 Release Unicode DLL"
# Name "PackedResources - Win32 Debug Unicode DLL"
# Name "PackedResources - Win32 Release DLL"
# Name "PackedResources - Win32 Debug DLL"
# Begin Source File

SOURCE=.\res\opennx.xrc

!IF  "$(CFG)" == "PackedResources - Win32 Release"

# Begin Custom Build
OutDir=.\Release
ProjDir=.
InputPath=.\res\opennx.xrc
InputName=opennx

"$(ProjDir)\$(InputName).rsc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(OutDir)\$(InputName).rsc res -i *.png -i *.xrc

# End Custom Build

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
ProjDir=.
InputPath=.\res\opennx.xrc
InputName=opennx

"$(ProjDir)\$(InputName).rsc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(OutDir)\$(InputName).rsc res -i *.png -i *.xrc

# End Custom Build

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Debug Unicode"

# Begin Custom Build
OutDir=.\Debug_Unicode
ProjDir=.
InputPath=.\res\opennx.xrc
InputName=opennx

"$(ProjDir)\$(InputName).rsc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(OutDir)\$(InputName).rsc res -i *.png -i *.xrc

# End Custom Build

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Release Unicode"

# Begin Custom Build
OutDir=.\Release_Unicode
ProjDir=.
InputPath=.\res\opennx.xrc
InputName=opennx

"$(ProjDir)\$(InputName).rsc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(OutDir)\$(InputName).rsc res -i *.png -i *.xrc

# End Custom Build

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Release Unicode DLL"

# Begin Custom Build
OutDir=.\ReleaseDLL_Unicode
ProjDir=.
InputPath=.\res\opennx.xrc
InputName=opennx

"$(ProjDir)\$(InputName).rsc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(OutDir)\$(InputName).rsc res -i *.png -i *.xrc

# End Custom Build

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Debug Unicode DLL"

# Begin Custom Build
OutDir=.\DebugDLL_Unicode
ProjDir=.
InputPath=.\res\opennx.xrc
InputName=opennx

"$(ProjDir)\$(InputName).rsc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(OutDir)\$(InputName).rsc res -i *.png -i *.xrc

# End Custom Build

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Release DLL"

# Begin Custom Build
OutDir=.\ReleaseDLL
ProjDir=.
InputPath=.\res\opennx.xrc
InputName=opennx

"$(ProjDir)\$(InputName).rsc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(OutDir)\$(InputName).rsc res -i *.png -i *.xrc

# End Custom Build

!ELSEIF  "$(CFG)" == "PackedResources - Win32 Debug DLL"

# Begin Custom Build
OutDir=.\DebugDLL
ProjDir=.
InputPath=.\res\opennx.xrc
InputName=opennx

"$(ProjDir)\$(InputName).rsc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	zip -9r $(OutDir)\$(InputName).rsc res -i *.png -i *.xrc

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
