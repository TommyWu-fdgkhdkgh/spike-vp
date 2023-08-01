all: spike-vp

spike-vp: ./src/main.cpp 
	g++ -g -std=c++17 -I /home/twu/side_project_vp/install/systemc-2.3.3/include/  -lpthread /home/twu/side_project_vp/install/systemc-2.3.3/lib-linux64/libsystemc.a ./src/main.cpp -o ./spike-vp

clean:
	rm -f ./spike-vp
