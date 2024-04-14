#include <iostream>
#include <cmath>
#include <omp.h>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

#define MESSAGE_SIZE 100000


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
	unsigned int message[MESSAGE_SIZE];
	long double encryptedMessage[MESSAGE_SIZE];	

	if (argc != 3)
    {
        printf("Usuage: ./<executable> <input_file> <thread_count>\n");
        return -1;
    }

    ifstream fInput(argv[1]);

    int thread_count = atoi(argv[2]);
    
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

    double startTime = omp_get_wtime();

	cout << "P: " << iP << endl;
	cout << "Q: " << iQ << endl;
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

	cout << "\n------ message encrypted -------" << endl;

	int iChunk = iE / thread_count;

    int iChunkRemainder = iE % thread_count;
    int startIndex, endIndex;
    long double dTemp;
	
	for(i = 0; i < MESSAGE_SIZE; i++)
	{
		encryptedMessage[i] = 1;	
	
		#pragma omp parallel firstprivate(startIndex, endIndex) num_threads(thread_count)
		{
			int myRank = omp_get_thread_num();

			dTemp = 1;

	        startIndex = myRank * iChunk;
	        if(iChunkRemainder && (startIndex + iChunk) > iE)
	            endIndex = iChunkRemainder;
	        else if (iChunkRemainder && myRank == thread_count - 1 && (startIndex + iChunk) < iE)
	        	endIndex = startIndex + iChunk + iChunkRemainder;
	        else
	            endIndex = startIndex + iChunk;

	        for(int index = startIndex; index < endIndex; index++)
	        	dTemp = dTemp * message[i];

	        encryptedMessage[i] = encryptedMessage[i] * dTemp;
	    }

	    encryptedMessage[i] = fmod(encryptedMessage[i], iN);
	}

	cout << "\n\n------ message decrypted -------" << endl;

	iChunk = iD / thread_count;
    iChunkRemainder = iD % thread_count;
    
	for(i = 0; i < MESSAGE_SIZE; i++)
	{
		long double decryptedText = 1;

		#pragma omp parallel firstprivate(startIndex, endIndex) num_threads(thread_count)
		{
			int myRank = omp_get_thread_num(); 

			dTemp = 1;

	        startIndex = myRank * iChunk;
	        if(iChunkRemainder && (startIndex + iChunk) > iD)
	            endIndex = iChunkRemainder;
	        else if (iChunkRemainder && myRank == thread_count - 1 && (startIndex + iChunk) < iD)
	        	endIndex = startIndex + iChunk + iChunkRemainder;
	        else
	            endIndex = startIndex + iChunk;

	        for(int index = startIndex; index < endIndex; index++)
	        	dTemp = dTemp * encryptedMessage[i];

	  
	        decryptedText = decryptedText * dTemp;
	    }

	    decryptedText = fmod(decryptedText, iN);
  	}
	
	double endTime = omp_get_wtime();

    cout << "\n\nTotal Runtime: " << endTime - startTime << endl;

    return 0;
}
