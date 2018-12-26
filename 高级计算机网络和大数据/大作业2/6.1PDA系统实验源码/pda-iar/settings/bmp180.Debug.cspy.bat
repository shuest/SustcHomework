@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM
@REM Read about available command line parameters in the C-SPY Debugging
@REM Guide. Hints about additional command line parameters that may be
@REM useful in specific cases:
@REM   --download_only   Downloads a code image without starting a debug
@REM                     session afterwards.
@REM   --silent          Omits the sign-on message.
@REM   --timeout         Limits the maximum allowed execution time.
@REM 


@echo off 

if not "%~1" == "" goto debugFile 

@echo on 

"E:\iar7.4\common\bin\cspybat" -f "D:\My Work\STM32 NBIOT 电信版库函数\传感器实验源程序\15.数字气压\settings\bmp180.Debug.general.xcl" --backend -f "D:\My Work\STM32 NBIOT 电信版库函数\传感器实验源程序\15.数字气压\settings\bmp180.Debug.driver.xcl" 

@echo off 
goto end 

:debugFile 

@echo on 

"E:\iar7.4\common\bin\cspybat" -f "D:\My Work\STM32 NBIOT 电信版库函数\传感器实验源程序\15.数字气压\settings\bmp180.Debug.general.xcl" "--debug_file=%~1" --backend -f "D:\My Work\STM32 NBIOT 电信版库函数\传感器实验源程序\15.数字气压\settings\bmp180.Debug.driver.xcl" 

@echo off 
:end