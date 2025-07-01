How to build on Docker
=======
git clone --recursive https://github.com/horsicq/XDataExtractor.git

cd XDataExtractor

docker build .

How to build on Linux based on Debian
=======

Install packages:

- sudo apt-get install --quiet --assume-yes git
- sudo apt-get install --quiet --assume-yes build-essential
- sudo apt-get install --quiet --assume-yes qtbase5-dev
- sudo apt-get install --quiet --assume-yes qttools5-dev-tools
- sudo apt-get install --quiet --assume-yes qt5-default (Ubuntu 14.04-20.04)
- sudo apt-get install --quiet --assume-yes qt5-qmake (Ubuntu 21.04-22.04)

git clone --recursive https://github.com/horsicq/XDataExtractor.git

cd XDataExtractor

Run build script: bash -x build_dpkg.sh

Install deb package: sudo dpkg -i release/XDataExtractor_[Version].deb

Run: *XDataExtractor [FileName]* or *xdec [FileName]*

How to build on macOS
=======

Install Qt 5.15.2: https://github.com/horsicq/build_tools

Clone project: git clone --recursive https://github.com/horsicq/XDataExtractor.git

cd Nauz-File-Detector

Edit build_mac.sh ( check QT_PATH variable)

Run build_mac.sh

How to build on Windows(XP)
=======

Install Visual Studio 2013: https://github.com/horsicq/build_tools

Install Qt 5.6.3 for VS2013: https://github.com/horsicq/build_tools

Install 7-Zip: https://www.7-zip.org/

Clone project: git clone --recursive https://github.com/horsicq/XDataExtractor.git

cd XDataExtractor

Edit build_winxp.bat ( check VS_PATH,  SEVENZIP_PATH, QT_PATH variables)

Run build_winxp.bat

How to build on Windows(7-11)
=======

Install Visual Studio 2019: https://github.com/horsicq/build_tools

Install Qt 5.15.2 for VS2019: https://github.com/horsicq/build_tools

Install 7-Zip: https://www.7-zip.org/

Clone project: git clone --recursive https://github.com/horsicq/XDataExtractor.git

cd XDataExtractor

Edit build_msvc_win32.bat ( check VSVARS_PATH, SEVENZIP_PATH, QMAKE_PATH variables)

Edit build_msvc_win64.bat ( check VSVARS_PATH, SEVENZIP_PATH, QMAKE_PATH variables)

Run build_win32.bat

Run build_win64.bat
