ECHO Generating Visual Studio solution in build/ folder 
mkdir build 2> nul
cd build
cmake -DKITTY_TEST=ON -G "Visual Studio 15 2017" ..
cd ..
