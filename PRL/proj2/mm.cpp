/**
 * Project: Mesh Multiplication (PRL3 2016/2017)
 * Author: Dávid Mikuš (xmikus15)
 */

#include <iostream>
#include <string>
#include <fstream>
#include <mpi.h>
#include <vector>
#include <chrono>

using namespace std;

int rows, cols;

enum Tags
{
	tag_a,
	tag_b,
	tag_c,
	tag_p
};


static inline int getIndex(int i, int j, int cols)
{
	return cols * i + j;
}
static inline bool leftBorder(int id)
{
	return id % cols == 0;
}
static inline bool topBorder(int id)
{
	return id < cols;
}

static inline bool rightBorder(int id)
{
	return (id + 1) % cols == 0;
}

static inline bool bottomBorder(int id)
{
	return id >= cols * (rows - 1);
}

int main(int argc, char* argv[])
{
	int numProcs;
	int id;
	MPI_Status stat;

	vector<int> mat1, mat2;
	int n;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	int number;
	if (id == 0)
	{
		string line;
		ifstream file1("mat1");
		ifstream file2("mat2");
		// Get first dimension
		getline(file1, line);
		rows = stoi(line);
		if (rows <= 0)
		{
			cerr << "Invalid rows" << endl;
			MPI_Abort(MPI_COMM_WORLD,1);
		}
		// Get second dimension
		getline(file2, line);
		cols = stoi(line);
		if (cols <= 0)
		{
			cerr << "Invalid cols" << endl;
			MPI_Abort(MPI_COMM_WORLD,1);
		}
		while (file1 >> number)
			mat1.push_back(number);
		file1.close();

		while (file2 >> number)
			mat2.push_back(number);
		file2.close();

		n = mat1.size() / rows;
		if (n != mat2.size() / cols)
		{
			cerr << "Invalid size" << endl;
			MPI_Abort(MPI_COMM_WORLD,1);
		}
		// Send parameters
		for (int i=0; i < rows * cols; ++i)
		{
			MPI_Send(&n, 1, MPI_INT, i, tag_p, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, i, tag_p, MPI_COMM_WORLD);
			MPI_Send(&cols, 1, MPI_INT, i, tag_p, MPI_COMM_WORLD);
		}
 		// Send A values
		for(int i=0; i < rows; ++i)
			for(int j=0; j < n; ++j)
			{
				MPI_Send(&mat1[getIndex(i,j,n)], 1, MPI_INT, i * cols, tag_a, MPI_COMM_WORLD);
			}
		// Send B values
		for (int i=0; i < n; ++i)
			for (int j=0; j < cols; ++j)
			{
				MPI_Send(&mat2[getIndex(i,j,cols)], 1, MPI_INT, j, tag_b, MPI_COMM_WORLD);
			}
		
		
	}
	// Receive parameters
	MPI_Recv(&n, 1, MPI_INT, 0, tag_p, MPI_COMM_WORLD, &stat);
	MPI_Recv(&rows, 1, MPI_INT, 0, tag_p, MPI_COMM_WORLD, &stat);
	MPI_Recv(&cols, 1, MPI_INT, 0, tag_p, MPI_COMM_WORLD, &stat);

	vector<int> vecA;
    vector<int> vecB;
    // Receive A values from control processor
    if (leftBorder(id))
    {
    	for(int i=0; i<n; ++i)
    	{
    		MPI_Recv(&number, 1, MPI_INT, 0, tag_a, MPI_COMM_WORLD, &stat);
    		vecA.push_back(number);
    	}
    }
    // Receive B values from control processor
    if (topBorder(id))
    {
    	for(int i=0; i<n; ++i)
    	{
    		MPI_Recv(&number, 1, MPI_INT, 0, tag_b, MPI_COMM_WORLD, &stat);
    		vecB.push_back(number);
    		//cout << id << " <- " << number << endl;
    	}
    }
    int a,b,c=0;
    for (int i=0; i<n; ++i)
    {
    	// Get from buffer
    	if (leftBorder(id))
    		a = vecA[i];
    	// Get from left neighbor
    	else
    		MPI_Recv(&a, 1, MPI_INT, id - 1, tag_a, MPI_COMM_WORLD, &stat);

    	if(topBorder(id))
    		b = vecB[i];
    	// Get from top neighbor
    	else
    		MPI_Recv(&b, 1, MPI_INT, id - cols, tag_b, MPI_COMM_WORLD, &stat);

    	c += a*b;
    	// Send to right neihbor
    	if(!rightBorder(id))
    		MPI_Send(&a, 1, MPI_INT, id + 1, tag_a, MPI_COMM_WORLD);
    	// Send to left neighbor
    	if(!bottomBorder(id))
    		MPI_Send(&b, 1, MPI_INT, id + cols, tag_b, MPI_COMM_WORLD);
    }
    // Send result to control processor
    MPI_Send(&c, 1, MPI_INT, 0, tag_c, MPI_COMM_WORLD);
    if (id == 0)
    {
    	vector<int> result;
    	// Gather results
    	for(int i=0; i<numProcs; ++i)
    	{
    		MPI_Recv(&number, 1, MPI_INT, i, tag_c, MPI_COMM_WORLD, &stat);
    		result.push_back(number);
    	}
		
    	cout << rows << ":" << cols << endl;
    	for (int i=0; i < result.size(); ++i)
    	{
    		cout << result[i];
    		if ( (i+1) % cols == 0)
    			cout << endl;
    		else
    			cout << " ";

    	}
    }
	MPI_Finalize();
	return 0;
}
