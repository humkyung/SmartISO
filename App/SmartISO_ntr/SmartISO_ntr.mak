# Microsoft Developer Studio Generated NMAKE File, Based on idms_ntr.dsp
!IF "$(CFG)" == ""
CFG=idms_ntr - Win32 Debug
!MESSAGE No configuration specified. Defaulting to idms_ntr - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "idms_ntr - Win32 Release" && "$(CFG)" != "idms_ntr - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "idms_ntr.mak" CFG="idms_ntr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "idms_ntr - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "idms_ntr - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "idms_ntr - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\Bin\idms_ntr.exe"

!ELSE 

ALL : "UserDLL - Win32 Release" "..\Bin\idms_ntr.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"UserDLL - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\idms_ntr.obj"
	-@erase "$(INTDIR)\IdmsNtrSetting.obj"
	-@erase "$(INTDIR)\QCCheck.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\Bin\idms_ntr.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\Is" /I "..\IsDGN" /I "..\userdll" /I "D:\boost_1_33_1\include\boost-1_33_1" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\idms_ntr.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\idms_ntr.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Is.lib IsDGN.lib UserDLL.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\idms_ntr.pdb" /machine:I386 /out:"..\Bin/idms_ntr.exe" /libpath:"..\Bin" /libpath:"D:\boost_1_33_1\lib" 
LINK32_OBJS= \
	"$(INTDIR)\idms_ntr.obj" \
	"$(INTDIR)\IdmsNtrSetting.obj" \
	"$(INTDIR)\QCCheck.obj" \
	"..\Bin\UserDLL.lib"

"..\Bin\idms_ntr.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "idms_ntr - Win32 Debug"

OUTDIR=.\..\Bin
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\Bin
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\idms_ntr.exe"

!ELSE 

ALL : "UserDLL - Win32 Debug" "$(OUTDIR)\idms_ntr.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"UserDLL - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\idms_ntr.obj"
	-@erase "$(INTDIR)\IdmsNtrSetting.obj"
	-@erase "$(INTDIR)\QCCheck.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\idms_ntr.exe"
	-@erase "$(OUTDIR)\idms_ntr.ilk"
	-@erase "$(OUTDIR)\idms_ntr.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\Is" /I "..\IsDGN" /I "..\userdll" /I "D:\boost_1_33_1\include\boost-1_33_1" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\idms_ntr.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\idms_ntr.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Is.lib IsDGN.lib UserDLL.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\idms_ntr.pdb" /debug /machine:I386 /out:"$(OUTDIR)\idms_ntr.exe" /pdbtype:sept /libpath:"..\Bin" /libpath:"D:\boost_1_33_1\lib" 
LINK32_OBJS= \
	"$(INTDIR)\idms_ntr.obj" \
	"$(INTDIR)\IdmsNtrSetting.obj" \
	"$(INTDIR)\QCCheck.obj" \
	"$(OUTDIR)\UserDLL.lib"

"$(OUTDIR)\idms_ntr.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("idms_ntr.dep")
!INCLUDE "idms_ntr.dep"
!ELSE 
!MESSAGE Warning: cannot find "idms_ntr.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "idms_ntr - Win32 Release" || "$(CFG)" == "idms_ntr - Win32 Debug"

!IF  "$(CFG)" == "idms_ntr - Win32 Release"

"UserDLL - Win32 Release" : 
   cd "\프로젝트\한화건설\UserDLL"
   $(MAKE) /$(MAKEFLAGS) /F ".\UserDLL.mak" CFG="UserDLL - Win32 Release" 
   cd "..\idms_ntr"

"UserDLL - Win32 ReleaseCLEAN" : 
   cd "\프로젝트\한화건설\UserDLL"
   $(MAKE) /$(MAKEFLAGS) /F ".\UserDLL.mak" CFG="UserDLL - Win32 Release" RECURSE=1 CLEAN 
   cd "..\idms_ntr"

!ELSEIF  "$(CFG)" == "idms_ntr - Win32 Debug"

"UserDLL - Win32 Debug" : 
   cd "\프로젝트\한화건설\UserDLL"
   $(MAKE) /$(MAKEFLAGS) /F ".\UserDLL.mak" CFG="UserDLL - Win32 Debug" 
   cd "..\idms_ntr"

"UserDLL - Win32 DebugCLEAN" : 
   cd "\프로젝트\한화건설\UserDLL"
   $(MAKE) /$(MAKEFLAGS) /F ".\UserDLL.mak" CFG="UserDLL - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\idms_ntr"

!ENDIF 

SOURCE=.\idms_ntr.cpp

"$(INTDIR)\idms_ntr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\IdmsNtrSetting.cpp

"$(INTDIR)\IdmsNtrSetting.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\QCCheck.cpp

"$(INTDIR)\QCCheck.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

