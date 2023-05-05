
IF "x%1" == "x" (
  %GDK_WIN%\bin\make -f %GDK_WIN%\makefile.gen
  %GENS% %CD%\out\rom.bin
) ELSE IF "%1" == "test" (
  %GENS% %CD%\out\rom.bin
) ELSE IF "%1" == "asm" (
  %GDK_WIN%\bin\make -f %GDK_WIN%\makefile.gen asm
) ELSE IF "%1" == "debug" (
  %GDK_WIN%\bin\make -f %GDK_WIN%\makefile.gen debug
  REM %GENS% %CD%\out\rom.bin -D
  REM %GENS_KMOD% %CD%\out\rom.bin
  %BLASTEM% %CD%\out\rom.bin -D
) ELSE IF "%1" == "cleanbuild" (
  
  %GDK_WIN%\bin\make -f %GDK_WIN%\makefile.gen clean
  %GDK_WIN%\bin\make -f %GDK_WIN%\makefile.gen release
) ELSE IF "%1" == "medbuild" (
  %GDK_WIN%\bin\make -f %GDK_WIN%\makefile.gen
  %MEGALINK% %CD%\out\rom.bin
  %MEGALINK% -memwr %CD%\out\rom.bin  0x0
  %MEGALINK% reset
) ELSE (
  %GDK_WIN%\bin\make -f %GDK_WIN%\makefile.gen %1
) 
