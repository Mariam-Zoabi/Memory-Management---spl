#include <iostream>
#include <fstream>
#include "../include/Session.h"
using namespace std;


int main(int argc, char** argv){

	if(argc!=2)
	{
		cout << "usage splflix input_file" << endl;
		return 0;
	}


	Session s = Session("/users/studs/bsc/2019/arielron/Desktop/New/HW1-v9.10/config1.json");
	s.start();

	return 0;
}
