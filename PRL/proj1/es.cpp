#include <mpi.h>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <limits>
#include <vector>

#include <chrono>

using namespace std;

static const char *inputFile = "numbers";
static const uint16_t EMPTY = numeric_limits<uint16_t>::max();

enum Tags
{
	tag_x,
	tag_y,
	tag_z,
	tag_o
};

typedef chrono::high_resolution_clock Clock;
typedef chrono::microseconds microseconds;
Clock::time_point t0, t1;

int main(int argc, char *argv[])
{
	int numProcs;
	int id;
	MPI_Status stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	// Init to 'empty'
	uint16_t x = EMPTY;
	uint16_t y = EMPTY;
	uint16_t z = EMPTY;
	// Step 1
	uint16_t c = 1;

	// Master
	if (id ==0)
	{
		fstream in;
		uint16_t number;
		vector<uint16_t> numbers;
		numbers.resize(numProcs - 1);
		in.open(inputFile, ios::in | ios::binary);
		if (!in.is_open())
			return 0;

		for(int i = 1; in.good(); ++i)
		{
			number = in.get();
			if (!in.good())
				break;
			if (i != 1)
				cout << " ";
			cout << number;
			// Send number to X for each processor
			MPI_Send(&number, 1, MPI_UNSIGNED_SHORT, i, tag_x, MPI_COMM_WORLD);
			// Profiling
			//if (i == 1)
				//t0 = Clock::now();
			// Send number to Y for first slave processor
			MPI_Send(&number, 1, MPI_UNSIGNED_SHORT, 1, tag_y, MPI_COMM_WORLD);

		}
		in.close();
		cout << endl;
		// Step 3
		uint16_t recv;
		for (int i = 1; i < numProcs; ++i)
		{
			MPI_Recv(&recv, 1, MPI_UNSIGNED_SHORT, numProcs - 1, tag_o, MPI_COMM_WORLD, &stat);
            numbers[numProcs - i - 1] = recv;
		}

		//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
		//t1 = Clock::now();

		for(const auto& n: numbers)
			cout << n << endl;

		//microseconds us = std::chrono::duration_cast<microseconds>(t1 - t0);
		//cout <<  us.count() << endl;


	}
	// Slaves
	else
	{
		// Receive X from Master
		MPI_Recv(&x, 1, MPI_UNSIGNED_SHORT, 0, tag_x, MPI_COMM_WORLD, &stat);

		for(int i = 1; i < numProcs; ++i)
		{
			// Receive Y (simulating shifting register)
			MPI_Recv(&y, 1, MPI_UNSIGNED_SHORT, id - 1, tag_y, MPI_COMM_WORLD, &stat);
			// Send Y (simulating shifting register)
			if (id != numProcs - 1)
				MPI_Send(&y, 1, MPI_UNSIGNED_SHORT, id + 1, tag_y, MPI_COMM_WORLD);
			if (x != EMPTY && y != EMPTY)
			{
				// Solution for duplicates
				if((i < id && x >= y) || x > y)
	            	++c;
	        }


		}

		MPI_Send(&x, 1, MPI_UNSIGNED_SHORT, c, tag_z, MPI_COMM_WORLD);
        MPI_Recv(&z, 1, MPI_UNSIGNED_SHORT, MPI_ANY_SOURCE, tag_z, MPI_COMM_WORLD, &stat);
        
        int recv = (id != numProcs - 1) ? id + 1 : 0;
        for(int i=1; i < id; ++i)
        {
        	MPI_Send(&z, 1, MPI_UNSIGNED_SHORT, recv, tag_o, MPI_COMM_WORLD);
            MPI_Recv(&z, 1, MPI_UNSIGNED_SHORT, id - 1, tag_o, MPI_COMM_WORLD, &stat);
        }

        MPI_Send(&z, 1, MPI_UNSIGNED_SHORT, recv, tag_o, MPI_COMM_WORLD);
	}


	MPI_Finalize();
	return 0;
}
