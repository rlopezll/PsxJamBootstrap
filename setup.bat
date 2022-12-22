git submodule init
git submodule update
python -m venv env
call .\env\scripts\activate.bat
pip install patool
pip install pyunpack
python setup.py
deactivate