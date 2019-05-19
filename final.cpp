#include "AES.h"
#include "mySerial.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <sstream> 
#include <string>
#include <windows.h>

using namespace std;

void print(unsigned char* state, int len);
void plain(unsigned char* state, int len);

char* readFileBytes(const char *name)
{
    std::ifstream fl(name);
    fl.seekg( 0, std::ios::end );
    size_t len = fl.tellg();
    char *ret = new char[len];
    fl.seekg(0, std::ios::beg); 
    fl.read(ret, len);
    fl.close();
    return ret;
}

int tambahKey = 0;
int hasilKey ;

int main(int argc, char* argv[])
{		
	tanggal sebagai KEY
  	time_t rawtime;
  	struct tm * timeinfo;
  	char buffer[80];

  	time (&rawtime);
  	timeinfo = localtime(&rawtime);

  	strftime(buffer,sizeof(buffer),"%d%m%Y",timeinfo);
  	string awal(buffer);
  	//reverse tanggal
	string reverse_str(awal.rbegin(), awal.rend());
	string awalKey;
	awalKey = awal + reverse_str;
	//cout << awalKey;
	//keyBerubah
	stringstream toInt(reverse_str); 
	int x = 0; 
    toInt >> x;
    //cout << "Value of x : " << x; 
	hasilKey = x + tambahKey;
	//cout << "Value of hasilkey : " << hasilKey;
	stringstream ss;
	ss << hasilKey;
	string stri = ss.str();
	//cout << "Value of stri : " << stri;
	string akhirKey = awal + stri;
	
	//ubah ke ASCII
	string str=akhirKey;
	int lengthChar = str.length();
	unsigned char key[lengthChar];
	for(int i = 0; i<str.length(); i++){
		key[i] = str.at(i);
		//printf("%d ", key[i]); 
	}
	//unsigned char input[] = {49, 48, 55, 46, 54, 49, 53, 50, 49, 50, 44, 45, 54, 46, 56, 56, 54, 57, 49, 55, 35, 45, 50, 48, 35, 45, 49, 48, 35, 53, 48, 35, 55, 53, 48, 48, 35, 49, 48, 48, 35, 50, 48, 48, 35, 51, 48, 48, 35, 52, 48, 48, 35, 53, 48, 48, 35, 48, 57, 48, 35, 48, 57, 48};
	unsigned char iv[]    = {49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 52, 53, 54};
	//unsigned char key[]   = {49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 52, 53, 54};
	unsigned char output[100] ={0};
    unsigned char temp[100] = {0};

	//file to input
	char *input = readFileBytes("input.txt");
	printf("isi file : %s\n",input);
	printf("panjang string = %d\n", strlen(input));  
    //padding
    //int lenin = strlen(input);
    int pad_length = 16 - (64 % 16);
    unsigned char input_new[64+ pad_length];
    int i;
    for(i=0;i<64 + pad_length;i++) {
    	if(i < 64) {
    		input_new[i] = input[i];
		} else {
			input_new[i] = pad_length;
		}
	}
//	unsigned char 
	AESModeOfOperation moo;
	moo.set_key(key);
	moo.set_mode(MODE_CBC);
	moo.set_iv(iv);
    int olen = sizeof input_new;

  	
    memcpy(temp, input_new, sizeof input_new);
	int len = moo.Encrypt(temp, olen, output);
	printf("len = %d\n", len);
    printf("output");
	print(output, len);
	//print output chipper
	printf("output = %s\n", output);
	cout << akhirKey;
	
	//save to file
  	ofstream myfile;
  	myfile.open ("test.txt");
  	myfile << output;
  	myfile.close();
  	
  	//tambahkey
  	tambahKey = tambahKey+1;
  	//serial
	mySerial serial("/dev/ttyUSB0",9600);
	serial.Send(output,sizeof(output));
	return 0;
}