
Name "Bomb Attack"

OutFile "bombattack-setup.exe"

InstallDir "$PROGRAMFILES\Bomb Attack\"

InstallDirRegKey HKLM "Software\Bomb Attack" "Install_Dir"

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

Section "Bomb Attack (required)"

  SectionIn RO
  
  SetOutPath $INSTDIR
  
  File /r "bombattack\*"


  ;WriteRegStr HKLM "SOFTWARE\Bomb Attack" "path" "$INSTDIR"
  
  WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Bomb Attack" "DisplayName" "Bomb Attack"
  WriteRegStr   HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Bomb Attack"  "UninstallString" '"$INSTDIR\Uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Bomb Attack"  "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Bomb Attack"  "NoRepair" 1
  WriteUninstaller "Uninstall.exe"
  
SectionEnd

Section "Start Menu & Desktop Shortcuts"
  CreateDirectory "$SMPROGRAMS\Bomb Attack"
  CreateShortCut "$SMPROGRAMS\Bomb Attack\Bomb Attack.lnk" "$INSTDIR\bombattack.exe" "" "$INSTDIR\bombattack.exe" 0
  CreateShortCut "$SMPROGRAMS\Bomb Attack\Bomb Attack (windowed mode).lnk" "$INSTDIR\bombattack.exe" "--windowed" "$INSTDIR\bombattack.exe" 0
  
  CreateShortCut "$SMPROGRAMS\Bomb Attack\Bomb Attack level editor.lnk" "$INSTDIR\bombattack-leveleditor.exe" "" "$INSTDIR\bombattack-leveleditor.exe" 0
  CreateShortCut "$SMPROGRAMS\Bomb Attack\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
  CreateShortCut "$DESKTOP\Bomb Attack.lnk" "$INSTDIR\bombattack.exe" "" "$INSTDIR\bombattack.exe" 0
  
SectionEnd

Section "Uninstall"
  
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Bomb Attack"
  DeleteRegKey HKLM "SOFTWARE\Bomb Attack"

  Delete "$INSTDIR\*"
  Delete "$INSTDIR\gfx\*"
  Delete "$INSTDIR\sfx\*"
  Delete "$INSTDIR\levels\*"

  RMDir "$INSTDIR\gfx\"
  RMDir "$INSTDIR\sfx\"
  RMDir "$INSTDIR\levels\"

  Delete "$SMPROGRAMS\Bomb Attack\Bomb Attack.lnk"
  Delete "$SMPROGRAMS\Bomb Attack\Bomb Attack (windowed mode).lnk"
  Delete "$SMPROGRAMS\Bomb Attack\Bomb Attack level editor.lnk"
  Delete "$SMPROGRAMS\Bomb Attack\Uninstall.lnk"
  Delete "$DESKTOP\Bomb Attack.lnk"

  RMDir "$SMPROGRAMS\Bomb Attack"
  RMDir "$INSTDIR"

SectionEnd
