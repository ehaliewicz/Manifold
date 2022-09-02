REM create virtualenv
python -m venv ..\..\..\.venv
REM activate virtualenv
call ..\..\..\.venv\Scripts\activate.bat
REM install dependencies if necessary
python -m pip install -r requirements.txt
REM set pysdl2 dll path
set PYSDL2_DLL_PATH=..\..\..\.venv\Lib\site-packages\sdl2dll\dll\
REM launch editor
python editor.py