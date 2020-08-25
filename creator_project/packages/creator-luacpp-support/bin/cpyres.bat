cd  %~dp0

set pathSrc=%1
set pathDst=%2


if not exist %pathDst% mkdir %pathDst%

XCOPY %pathSrc%\assets\*.png %pathDst% /S /E /Y /D
XCOPY %pathSrc%\assets\*.ttf %pathDst% /S /E /Y /D
XCOPY %pathSrc%\assets\*.jpg %pathDst% /S /E /Y /D
XCOPY %pathSrc%\assets\*.plist %pathDst% /S /E /Y /D
XCOPY %pathSrc%\assets\*.fnt %pathDst% /S /E /Y /D
XCOPY %pathSrc%\assets\*.json %pathDst% /S /E /Y /D
XCOPY %pathSrc%\assets\*.atlas %pathDst% /S /E /Y /D
XCOPY %pathSrc%\ccreator\*.ccreator %pathDst% /S /E /Y /D





