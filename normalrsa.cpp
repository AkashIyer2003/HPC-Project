#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <chrono>
using namespace std::chrono;
using namespace std;

#define MESSAGE_SIZE 1000

unsigned int gcd(unsigned int a, unsigned int b)
{
	unsigned int x;
	while (b)
	{
		x = a % b;
		a = b;
		b = x;
    }
    return a;
}

int main(int argc, char *argv[])
{
	auto start = high_resolution_clock::now();
	unsigned int message[MESSAGE_SIZE];
	long double encryptedMessage[MESSAGE_SIZE];

	if (argc != 2)
    {
        printf("Usuage: ./<executable> <input_file>\n");
        return -1;
    }

    ifstream fInput(argv[1]);

    if (!fInput)
	{
		printf("Error opening the input file.\n");
		return -1;
	}

	unsigned int iP, iQ, iD, iE, iN, iTotientN;

	fInput >> iP >> iQ;

	int i = 0;
	char cItem;

	while(fInput >> cItem)
	{
		message[i] = cItem - '0';
		i++;
	}

	fInput.close();

	cout << "P: " << iP << endl;
	cout << "Q: " << iQ << endl;

	cout << "------ Plaintext message -------" << endl;
	for (i = 0; i < MESSAGE_SIZE; i++)
		cout << message[i] << ' ';
	cout << endl;
	
	iN = iP * iQ;
	iP -= 1;
	iQ -= 1;
	iTotientN = iP * iQ;
	
	iE = 7;
	while(i < (iN - 1))
	{
		if(gcd(iE, iTotientN) == 1)
			break;
			
		iE++;
	}

	i = 1;
	while(i < iN)
	{
		iD = (iE * i - 1) % iTotientN;
		if(!iD)
		{
			iD = i;
			break;
		}
		i++;
	}

	cout << "------ Encrypted message -------" << endl;
	cout << "E: " << iE << endl;
	cout << "N: " << iN << endl;
	for(i = 0; i < MESSAGE_SIZE; i++)
	{
		encryptedMessage[i] = 1;
		for (int j = 1; j <= iE; j++)
			encryptedMessage[i] = encryptedMessage[i] * message[i];
		
		encryptedMessage[i] = fmod(encryptedMessage[i], iN);

		cout << encryptedMessage[i]  << ' ';
	}
	cout << endl;

	cout << "------ Decrypted message -------" << endl;
	cout << "D: " << iD << endl;
	cout << "N: " << iN << endl;
	long double decryptedText;

	for(i = 0; i < MESSAGE_SIZE; i++)
	{
		decryptedText = 1;
		for (int j = 1; j <= iD; j++)
			decryptedText = decryptedText * encryptedMessage[i];
		
		decryptedText = fmod(decryptedText, iN);

		cout << decryptedText << ' ';
	}
	cout << endl;
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout <<"0."<< duration.count() <<" seconds"<< endl;
	cout << endl;
    return 0;
}
