cmake -Bbuild -DCMAKE-BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cd build 
make
echo ""
make test
