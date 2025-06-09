cmake -Bbuild -DCMAKE-BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cd build
cmake --build . --config Release
echo.

main.exe

