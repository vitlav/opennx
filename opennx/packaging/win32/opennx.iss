;
; $Id$
;
#undef DEBUG

#define APPNAME "OpenNX"
; Automatically get version from executable resp. dll
#define APPEXE "..\..\opennx.exe"
#include <instutils.iss>
#include <anydl.iss>
#include <gpl.iss>

#define MSWU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall"
#define MINGW "D:\MinGW"
#define APPIDSTR "{56D797D7-543C-408F-BBEB-B56787873D2F}"
#define APPIDVAL "{" + APPIDSTR

//#define NXDLURL "http://64.34.161.181/download/3.2.0/Windows/nxclient-3.2.0-13.exe"
#define NXDLPAGE "http://www.nomachine.com/download-package.php?Prod_Id=65"
#define NXRELEASE "3.2.0-13"
#define NXINSTALLER "nxclient-setup.exe"

[Setup]
AppName={#=APPNAME}
AppVersion={#=APPVERSION}
AppVerName={#=APPVERNAME}
AppPublisher=The OpenNX Team
AppPublisherURL=http://www.sf.net/projects/opennx
AppCopyright=(C) 2008 The OpenNX Team
VersionInfoVersion={#=APPVERSION}
DefaultDirName={code:getStr|appdefault}
DefaultGroupName={#=APPNAME}
#ifdef DEBUG
PrivilegesRequired=none
#endif
DisableStartupPrompt=true
ChangesAssociations=yes
OutputDir=.
OutputBaseFileName={#=APPNAME}-{#=APPVERSION}-Setup
ShowLanguageDialog=no
MinVersion=0,5.0.2195sp3
AppID={#=APPIDVAL}
UninstallFilesDir={app}\opennx_uninstall
Compression=lzma/ultra64
SolidCompression=yes
WizardImageFile=compiler:wizmodernimage-IS.bmp
WizardSmallImageFile=compiler:wizmodernsmallimage-IS.bmp
SetupIconFile=compiler:Examples\Setup.ico
UninstallDisplayIcon={app}\opennx.exe
LicenseFile={#=LGPLRTF}

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "de"; MessagesFile: "compiler:Languages\German.isl"
Name: "es"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "fr"; MessagesFile: "compiler:Languages\French.isl"

[CustomMessages]
compnx=Download and install NoMachine NXclient
dticon=Create a &desktop icon
dticon_group=Additional icons:
cwizard=OpenNX Connection Wizard
sadmin=OpenNX Session Administrator
uninst_opennx=Uninstall OpenNX

de.compnx=NoMachine NXclient herunterladen und installieren
de.dticon=Desktop-Verknüpfung &anlegen
de.dticon_group=Zusätzliche Verknüpfungen:
de.cwizard=OpenNX Verbindungs-Assistent
de.sadmin=OpenNX Sitzungsverwaltung
de.uninst_opennx=Deinstalliere OpenNX

[Components]
Name: main; Description: "OpenNX"; Types: full compact custom; Flags: fixed;
Name: nx; Description: "{cm:compnx}"; Types: full compact custom; ExtraDiskSpaceRequired: 19922944

[Tasks]
Name: "desktopicon"; Description: "{cm:dticon}"; GroupDescription: "{cm:dticon_group}";

[Dirs]
Name: {app}\backup; Flags: uninsalwaysuninstall

[Files]
Source: {#=APPEXE}; DestDir: {app}
Source: {#=APPEXE}; DestDir: {app}; DestName: nxclient.exe
Source: {#=MINGW}\bin\mingwm10.dll; DestDir: {app}

[Icons]
Name: "{group}\OpenNX"; Filename: "{app}\opennx.exe";
Name: "{group}\{cm:cwizard}"; Filename: "{app}\opennx.exe"; Parameters: "--wizard"; IconFilename: "{app}\opennx.exe"; IconIndex: 2;
Name: "{group}\{cm:sadmin}"; Filename: "{app}\nxclient.exe"; Parameters: "--admin"; IconFilename: "{app}\opennx.exe"; IconIndex: 3
Name: "{group}\{cm:uninst_opennx}"; Filename: "{uninstallexe}";
Name: "{commondesktop}\OpenNX"; Filename: "{app}\opennx.exe"; Tasks: "desktopicon";

[Registry]
Root: HKLM; Subkey: "Software\Classes\.nxs"; ValueType: none; Flags: deletekey
Root: HKLM; Subkey: "Software\Classes\NXClient.session"; ValueType: none; Flags: deletekey; Components: nx
;
Root: HKLM; Subkey: "Software\Classes\.nxs"; ValueType: String; ValueData: "OpenNX.session"; Flags: deletekey uninsdeletekey;
Root: HKLM; Subkey: "Software\Classes\.nxs"; ValueName: "Content Type"; ValueType: String; ValueData: "application/nx-session";
Root: HKLM; Subkey: "Software\Classes\OpenNX.session"; ValueType: String; ValueData: "OpenNX session file"; Flags: deletekey uninsdeletekey;
Root: HKLM; Subkey: "Software\Classes\OpenNX.session\DefaultIcon"; ValueType: String; ValueData: "{app}\opennx.exe,1";
Root: HKLM; Subkey: "Software\Classes\OpenNX.session\shell\open\command"; ValueType: String; ValueData: """{app}\opennx.exe"" --session ""%1""";
; Delete original NXClient class regs
Root: HKCU; Subkey: "Software\Classes\.nxs"; ValueName: ""; ValueType: none; Flags: deletekey deletevalue
Root: HKCU; Subkey: "Software\Classes\.nxs"; ValueName: "Content Type"; ValueType: none; Flags: deletekey deletevalue
Root: HKCU; Subkey: "Software\Classes\NXClient.session"; ValueName: ""; ValueType: none; Flags: deletekey deletevalue
Root: HKCU; Subkey: "Software\Classes\OpenNX.session\DefaultIcon"; ValueType: none; Flags: deletekey deletevalue
Root: HKCU; Subkey: "Software\Classes\OpenNX.session\shell\open\command"; ValueType: none; Flags: deletekey deletevalue

[UninstallRun]
Filename: "{app}\bin\nxfind.exe"; Parameters: "nxwin.exe kill";
Filename: "{app}\bin\nxfind.exe"; Parameters: "nxssh.exe kill";
Filename: "{app}\bin\nxfind.exe"; Parameters: "nxesd.exe kill";

[Code]
var
	nxdir: String;
	neednx: Boolean;

function getStr(param: String): String;
var ok: Boolean;
begin
  ok := false;
  if (param = 'appdefault') then begin
    ok := true;
    if neednx then
      Result := ExpandConstant('{pf}\{#=APPNAME}')
    else
      Result := nxdir;
  end;
  if (param = 'nxdir') then begin
    ok := true;
    Result := nxdir;
  end;

  if (not ok) then begin
    MsgBox('Internal error: getStr() called with unknown parameter ' + param, mbError, MB_OK);
    Abort();
  end;
end;

function InitializeSetup(): Boolean;
var tmp: String;
begin
	Result := true;
	if (RegQueryStringValue(HKLM, '{#=MSWU}\nxclient_is1', 'Inno Setup: App Path', nxdir)) then begin
	  neednx := false;
	end else begin
 	  neednx := true;
	end;
	if (RegQueryStringValue(HKLM, '{#=MSWU}\{#=APPIDSTR}_is1', 'Inno Setup: Selected Components', tmp)) then begin
    neednx := Pos('nx', tmp) <> 0;
	end;
end;

procedure InitializeWizard();
begin
  if (neednx) then begin
    WizardForm.ComponentsList.ItemEnabled[1] := false;
  end else begin
    WizardForm.ComponentsList.Checked[1] := false;
  end;
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
  Result := false;
  case PageID of
    wpSelectDir:
      Result := not neednx;
  end;
end;

function fmtsuccess(b:Boolean):String;
begin
  Result := 'failed';
  if b then
    Result := 'done';
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  tmp : String;
  content : String;
  params: String;
  nxver: String;
  nxurl: String;
  bres: Boolean;
  ires: Integer;
  j: Integer;
  hwnd: Integer;
begin
  case CurStep of
    ssInstall:
      if IsComponentSelected('nx') then begin
        isxdl_InitLanguage;
        tmp := ExpandConstant('{tmp}\nxdlpage.txt');
        hwnd := StrToInt(ExpandConstant('{wizardhwnd}'));
        if isxdl_Download(hwnd, '{#=NXDLPAGE}', tmp) <> 1 then
          RaiseException('Could not fetch NoMachine download page. Aborting setup.');
        isxdl_ClearFiles;
        if (not LoadStringFromFile(tmp, content)) then
          RaiseException('Could not read NoMachine download page. Aborting setup.');

        ires := Pos('<B>Release:', content);
        if (ires = 0) then
          RaiseException('Could not parse NoMachine download page. Aborting setup.');
        Delete(content, 1, ires + 28);
        ires := Pos('</FONT>', content);
        if (ires = 0) then
          RaiseException('Could not parse NoMachine download page. Aborting setup.');
        j := ires - 1;
        while ((j > 1) and (content[j-1] <> '>')) do j := j - 1;
        nxver := Copy(content, j, ires - j);
        Delete(content, 1, ires + 7);
        ires := Pos('method="post"', content);
        if (ires = 0) then
          RaiseException('Could not parse NoMachine download page. Aborting setup.');
        Delete(content, 1, ires + 13);
        ires := Pos('HREF="', content);
        if (ires = 0) then
          RaiseException('Could not parse NoMachine download page. Aborting setup.');
        Delete(content, 1, ires + 5);
        ires := Pos('"', content);
        if (ires < 2) then
          RaiseException('Could not parse NoMachine download page. Aborting setup.');
        nxurl := Copy(content, 1, ires - 1);

        tmp := ExpandConstant('{tmp}\{#=NXINSTALLER}');
        isxdl_AddFile(nxurl, tmp);
        Log('Downloading "' + nxurl + '" to "' + tmp + '"');
        if isxdl_DownloadFiles(hwnd) <> 1 then
          RaiseException('Could not download NXclient. Aborting setup.');
        params := '/SILENT /SUPPRESSMSGBOXES /NOCANCEL /NOICONS /TASKS="" /DIR="' +
          ExpandConstant('{app}') + '"'
        Log('Executing ' + tmp + ' ' + params);
        bres := Exec(tmp, params, ExpandConstant('{tmp}'), SW_SHOWNORMAL, ewWaitUntilTerminated, ires);
        BringToFrontAndRestore();
        if (not bres) then
          RaiseException('Could not run NXclient installer. Aborting setup.');
        if (ires <> 0) then
        RaiseException('Error while installing NXclient. Aborting setup.');
      	if (RegQueryStringValue(HKLM, '{#=MSWU}\nxclient_is1', 'Inno Setup: Icon Group', tmp)) then begin
       	  tmp := AddBackslash(ExtractFileDir(ExpandConstant('{group}'))) + tmp;
     	    Log('Deleting start menu "' + tmp + '": ' + fmtsuccess(DelTree(tmp, true, true, true)));
       	end;
        tmp := ExpandConstant('{app}\nxclient.exe');
     	  Log('Deleting file "' + tmp + '": ' + fmtsuccess(DeleteFile(tmp)));
     	  Log('Hiding nxclient uninstall key "HKLM\{#=MSWU}\nxclient_is1": ' +
          fmtsuccess(RegWriteDWordValue(HKLM, '{#=MSWU}\nxclient_is1', 'SystemComponent', 1)));
      end else begin
        tmp := ExpandConstant('{app}\nxclient.exe');
     	  Log('Renaming file "' + tmp + '" to "' + ExpandConstant('{app}\nxclient-orig.exe')
         + '": ' + fmtsuccess(RenameFile(tmp, ExpandConstant('{app}\nxclient-orig.exe'))));
      end;
  end;
end;

function InitializeUninstall(): Boolean;
var tmp: String;
begin
  Result := true;
	if (RegQueryStringValue(HKLM, '{#=MSWU}\{#=APPIDSTR}_is1', 'Inno Setup: Selected Components', tmp)) then begin
    neednx := Pos('nx', tmp) <> 0;
	end;
end;

procedure CurUninstallStepChanged(CurStep: TUninstallStep);
var
  tmp : String;
  ires: Integer;
begin
  case CurStep of
    usPostUninstall:
      if neednx then begin
      	if (RegQueryStringValue(HKLM, '{#=MSWU}\nxclient_is1', 'UninstallString', tmp)) then begin
      	  tmp := RemoveQuotes(tmp);
      	  RegDeleteValue(HKLM, '{#=MSWU}\nxclient_is1', 'SystemComponent');
          Exec(tmp, '/SILENT', ExpandConstant('{tmp}'), SW_SHOWNORMAL, ewWaitUntilTerminated, ires);
          BringToFrontAndRestore();
      	end;
      end else begin
        RenameFile(ExpandConstant('{app}\nxclient-orig.exe'), ExpandConstant('{app}\nxclient.exe'));
        RegWriteStringValue(HKLM, 'Software\Classes\.nxs', '', 'NXClient.session');
        RegWriteStringValue(HKLM, 'Software\Classes\.nxs', 'Content Type', 'application/nx-session');
      end;
  end;
end;
