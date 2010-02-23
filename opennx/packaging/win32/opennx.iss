;
; $Id$
;
#undef DEBUG

#define APPNAME "OpenNX"
; Automatically get version from executable resp. dll
#define APPEXE "setupdir\bin\opennx.exe"
#include <version.iss>

#define MSWU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall"
#define APPIDSTR "{56D797D7-543C-408F-BBEB-B56787873D2F}"
#define APPIDVAL "{" + APPIDSTR

[Setup]
AppName={#=APPNAME}
AppVersion={#=APPFULLVER}
AppVerName={#=APPFULLVERNAME}
AppPublisher=The OpenNX Team
AppPublisherURL=http://www.sf.net/projects/opennx
AppCopyright=(C) 2010 The OpenNX Team
VersionInfoVersion={#=APPFULLVER}
DefaultDirName={pf}\{#=APPNAME}
DefaultGroupName={#=APPNAME}
#ifdef DEBUG
PrivilegesRequired=none
#endif
DisableStartupPrompt=true
ChangesAssociations=yes
OutputDir=.
OutputBaseFileName={#=SETUPFVNAME}
ShowLanguageDialog=no
MinVersion=0,5.0.2195sp3
AppID={#=APPIDVAL}
UninstallFilesDir={app}\uninstall
Compression=lzma/ultra64
SolidCompression=yes
WizardImageFile=compiler:wizmodernimage-IS.bmp
WizardSmallImageFile=compiler:wizmodernsmallimage-IS.bmp
;SetupIconFile=compiler:Examples\Setup.ico
UninstallDisplayIcon={app}\bin\opennx.exe
LicenseFile=lgpl.rtf

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "de"; MessagesFile: "compiler:Languages\German.isl"
Name: "es"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "fr"; MessagesFile: "compiler:Languages\French.isl"
Name: "ru"; MessagesFile: "compiler:Languages\Russian.isl"

[CustomMessages]
dticon=Create a &desktop icon
dticon_group=Additional icons:
cwizard=OpenNX Connection Wizard
sadmin=OpenNX Session Administrator
uninst_opennx=Uninstall OpenNX
fwadd=Adding firewall rules

de.dticon=Desktop-Verknüpfung &anlegen
de.dticon_group=Zusätzliche Verknüpfungen:
de.cwizard=OpenNX Verbindungs-Assistent
de.sadmin=OpenNX Sitzungsverwaltung
de.uninst_opennx=Deinstalliere OpenNX
de.fwadd=Erstelle Firewall-Regeln

[Tasks]
Name: "desktopicon"; Description: "{cm:dticon}"; GroupDescription: "{cm:dticon_group}";

[Files]
Source: setupdir\*; DestDir: {app}; Flags: recursesubdirs restartreplace replacesameversion

[Icons]
Name: "{group}\OpenNX"; Filename: "{app}\bin\opennx.exe";
Name: "{group}\{cm:cwizard}"; Filename: "{app}\bin\opennx.exe"; Parameters: "--wizard"; IconFilename: "{app}\bin\opennx.exe"; IconIndex: 2;
Name: "{group}\{cm:sadmin}"; Filename: "{app}\bin\opennx.exe"; Parameters: "--admin"; IconFilename: "{app}\bin\opennx.exe"; IconIndex: 3
Name: "{group}\{cm:uninst_opennx}"; Filename: "{uninstallexe}";
Name: "{commondesktop}\OpenNX"; Filename: "{app}\bin\opennx.exe"; Tasks: "desktopicon";

[Registry]
; Delete original NXClient file global associations
Root: HKLM; Subkey: "Software\Classes\.nxs"; ValueType: none; Flags: deletekey
Root: HKLM; Subkey: "Software\Classes\NXClient.session"; ValueType: none; Flags: deletekey;
;
Root: HKLM; Subkey: "Software\Classes\.nxs"; ValueType: String; ValueData: "OpenNX.session"; Flags: deletekey uninsdeletekey;
Root: HKLM; Subkey: "Software\Classes\.nxs"; ValueName: "Content Type"; ValueType: String; ValueData: "application/nx-session";
Root: HKLM; Subkey: "Software\Classes\OpenNX.session"; ValueType: String; ValueData: "OpenNX session file"; Flags: deletekey uninsdeletekey;
Root: HKLM; Subkey: "Software\Classes\OpenNX.session\DefaultIcon"; ValueType: String; ValueData: "{app}\bin\opennx.exe,1";
Root: HKLM; Subkey: "Software\Classes\OpenNX.session\shell\open\command"; ValueType: String; ValueData: """{app}\bin\opennx.exe"" --session ""%1""";
; Delete original NXClient class regs in HKCU
Root: HKCU; Subkey: "Software\Classes\.nxs"; ValueName: ""; ValueType: none; Flags: deletekey deletevalue
Root: HKCU; Subkey: "Software\Classes\.nxs"; ValueName: "Content Type"; ValueType: none; Flags: deletekey deletevalue
Root: HKCU; Subkey: "Software\Classes\NXClient.session"; ValueName: ""; ValueType: none; Flags: deletekey deletevalue
Root: HKCU; Subkey: "Software\Classes\OpenNX.session\DefaultIcon"; ValueType: none; Flags: deletekey deletevalue
Root: HKCU; Subkey: "Software\Classes\OpenNX.session\shell\open\command"; ValueType: none; Flags: deletekey deletevalue

[Run]
; Allow nxssh, nxesd and Xming in Windows firewall 
Filename: "{sys}\netsh.exe"; Parameters: "firewall add allowedprogram ""{app}\bin\nxssh.exe"" ""OpenNX nxssh"" ENABLE"; StatusMsg: {cm:fwadd}; Flags: runhidden skipifdoesntexist
Filename: "{sys}\netsh.exe"; Parameters: "firewall add allowedprogram ""{app}\bin\nxesd.exe"" ""OpenNX nxesd"" ENABLE"; StatusMsg: {cm:fwadd}; Flags: runhidden skipifdoesntexist
Filename: "{sys}\netsh.exe"; Parameters: "firewall add allowedprogram ""{app}\bin\Xming.exe"" ""OpenNX Xming"" ENABLE"; StatusMsg: {cm:fwadd}; Flags: runhidden skipifdoesntexist

[UninstallRun]
; Remove firewall exceptions
Filename: "{sys}\netsh.exe"; Parameters: "firewall delete allowedprogram ""{app}\bin\nxssh.exe"" ALL"; Flags: runhidden skipifdoesntexist
Filename: "{sys}\netsh.exe"; Parameters: "firewall delete allowedprogram ""{app}\bin\nxesd.exe"" ALL"; Flags: runhidden skipifdoesntexist
Filename: "{sys}\netsh.exe"; Parameters: "firewall delete allowedprogram ""{app}\bin\Xming.exe"" ALL"; Flags: runhidden skipifdoesntexist

[UninstallDelete]
Type: files; Name: "{app}\share\Xming\font-dirs"

[Code]
procedure CurStepChanged(step: TSetupStep);
var
    fontDir:  String;
    FindRec:  TFindRec;
    fontDirs: TStringList;

begin
    if step = ssPostInstall then begin
        (* Create {app}\share\Xming\font-dirs *)
        fontDir := ExpandConstant('{app}\share\Xming\fonts');
        fontDirs := TStringList.Create;
        (* First, create a list of all subdirs in fonts *)
        if DirExists(fontDir) then begin
            if FindFirst(fontDir + '\*', FindRec) then begin
                try
                    repeat
                        if FindRec.Attributes and FILE_ATTRIBUTE_DIRECTORY <> 0 then
                            fontDirs.Append(fontDir + '\' + FindRec.Name);
                    until not FindNext(FindRec);
                finally
                    FindClose(FindRec);
                end;
            end;
        end;
        (* Add the windows font dir to the list *)
        fontDirs.Append(ExpandConstant('{fonts}'));
        (* Finally, create the file, containing the comma-separated directory list. *)
        SaveStringToFile(ExpandConstant('{app}\share\Xming\font-dirs'), fontDirs.CommaText, false);
    end;
end;
