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
AppCopyright=(C) 2009 The OpenNX Team
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
UninstallDisplayIcon={app}\opennx.exe
LicenseFile=lgpl.rtf

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "de"; MessagesFile: "compiler:Languages\German.isl"
Name: "es"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "fr"; MessagesFile: "compiler:Languages\French.isl"

[CustomMessages]
dticon=Create a &desktop icon
dticon_group=Additional icons:
cwizard=OpenNX Connection Wizard
sadmin=OpenNX Session Administrator
uninst_opennx=Uninstall OpenNX

de.dticon=Desktop-Verknüpfung &anlegen
de.dticon_group=Zusätzliche Verknüpfungen:
de.cwizard=OpenNX Verbindungs-Assistent
de.sadmin=OpenNX Sitzungsverwaltung
de.uninst_opennx=Deinstalliere OpenNX

[Tasks]
Name: "desktopicon"; Description: "{cm:dticon}"; GroupDescription: "{cm:dticon_group}";

[Files]
Source: setupdir\*; DestDir: {app}; Flags: recursesubdirs

[Icons]
Name: "{group}\OpenNX"; Filename: "{app}\bin\opennx.exe";
Name: "{group}\{cm:cwizard}"; Filename: "{app}\bin\opennx.exe"; Parameters: "--wizard"; IconFilename: "{app}\bin\opennx.exe"; IconIndex: 2;
Name: "{group}\{cm:sadmin}"; Filename: "{app}\bin\opennx.exe"; Parameters: "--admin"; IconFilename: "{app}\bin\opennx.exe"; IconIndex: 3
Name: "{group}\{cm:uninst_opennx}"; Filename: "{uninstallexe}";
Name: "{commondesktop}\OpenNX"; Filename: "{app}\bin\opennx.exe"; Tasks: "desktopicon";

[Registry]
Root: HKLM; Subkey: "Software\Classes\.nxs"; ValueType: none; Flags: deletekey
Root: HKLM; Subkey: "Software\Classes\NXClient.session"; ValueType: none; Flags: deletekey;
;
Root: HKLM; Subkey: "Software\Classes\.nxs"; ValueType: String; ValueData: "OpenNX.session"; Flags: deletekey uninsdeletekey;
Root: HKLM; Subkey: "Software\Classes\.nxs"; ValueName: "Content Type"; ValueType: String; ValueData: "application/nx-session";
Root: HKLM; Subkey: "Software\Classes\OpenNX.session"; ValueType: String; ValueData: "OpenNX session file"; Flags: deletekey uninsdeletekey;
Root: HKLM; Subkey: "Software\Classes\OpenNX.session\DefaultIcon"; ValueType: String; ValueData: "{app}\bin\opennx.exe,1";
Root: HKLM; Subkey: "Software\Classes\OpenNX.session\shell\open\command"; ValueType: String; ValueData: """{app}\bin\opennx.exe"" --session ""%1""";
; Delete original NXClient class regs
Root: HKCU; Subkey: "Software\Classes\.nxs"; ValueName: ""; ValueType: none; Flags: deletekey deletevalue
Root: HKCU; Subkey: "Software\Classes\.nxs"; ValueName: "Content Type"; ValueType: none; Flags: deletekey deletevalue
Root: HKCU; Subkey: "Software\Classes\NXClient.session"; ValueName: ""; ValueType: none; Flags: deletekey deletevalue
Root: HKCU; Subkey: "Software\Classes\OpenNX.session\DefaultIcon"; ValueType: none; Flags: deletekey deletevalue
Root: HKCU; Subkey: "Software\Classes\OpenNX.session\shell\open\command"; ValueType: none; Flags: deletekey deletevalue
