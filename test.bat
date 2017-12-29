@ECHO OFF
REM Simple batch test
REM loops throu a rang of numbers, feeds the current value in the loop to generator
REM generator in return generates 2 random string with the passed length and
REM stores them in input.txt
REM main.exe is then executed and caluclates the difference of the two strings in 
REM file input.txt


REM default values
set step=1
set max_length=10000
set min_length=1000

REM check input parameters
if "%~1"=="" (

REM if no params echo usage msg, continue working with default params
    echo  "Change test params by invoking test like -> test <min_length> <max_length> <step>."
	
) else (

REM set input parameters
    set min_length=%1
	set max_length=%2
	set step=%3
)

echo Staring test: string length range %min_length% - %max_length%  step %step%
echo.

REM TODO: testirati !
del *.o main.exe generator.exe
make

REM loop throu given range 
for /l %%x in (%min_length%, %step%, %max_length%) do (

	echo **************************************************************
	echo Testing input with %%x characters
	
REM execute generator with the current value in the loop, generates two strings in input.txt
	generator %%x
	
REM read input.txt and calculate edit distance
	main
	echo.  
)

:exit