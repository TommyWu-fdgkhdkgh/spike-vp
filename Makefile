all: spike-vp

spike-vp: ./src/main.cpp 
	g++ -g -std=c++17 -I /home/twu/side_project_vp/install/systemc-2.3.3/include/ -I //home/twu/side_project_vp/install/vcml/include/  /home/twu/side_project_vp/install/vcml/lib/libvcmld.a /home/twu/side_project_vp/install/vcml/lib/libmwrd.a /home/twu/side_project_vp/install/systemc-2.3.3/lib-linux64/libsystemc.a ./src/main.cpp -o ./spike-vp -lpthread -lstdc++fs
clean:
	rm -f ./spike-vp
