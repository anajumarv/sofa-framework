#ifndef SHOWVECTOR_H
#define SHOWVECTOR_H

#include <stdio.h>
#include <iostream>
#include "../myopencl.h"
#include "../../cuda/mycuda.h"

class ShowVector
{
	FILE* _file;
	std::string _name;
public:
	ShowVector(char* fileName)
	{
		_file = fopen(fileName,"w");
		_name = std::string(fileName);
	}

	~ShowVector()
	{
		fclose(_file);
	}

	void writeVector(int v)
	{
		fprintf(_file,"%d",v);
	}

	void writeVector(float v)
	{
		fprintf(_file,"%f",v);
	}


	template <class T> void addVector(T* v,int size)
	{
		std::cout << "write in:" << _name << std::endl;
		for(int i=0;i<size;i++)
		{
			writeVector(v[i]);
			if(i%1024==1023)
				fprintf(_file,"\n");
			else fprintf(_file,";");
		}
		fprintf(_file,"\n\n");
	}

	template <class T> void addOpenCLVector(const sofa::gpu::opencl::_device_pointer &dp,int size)
	{
		T * tab = new T[size];
		sofa::gpu::opencl::myopenclEnqueueReadBuffer(0,tab,dp.m,dp.offset,size*sizeof(T));
		addVector<T>(tab,size);

		delete(tab);
	}

	template <class T> void addCudaVector(const void * dp,int size)
	{
		T * tab = new T[size];
		sofa::gpu::cuda::mycudaMemcpyDeviceToHost(tab,dp,size*sizeof(T),0);
		addVector<T>(tab,size);

		delete(tab);
	}
};

#endif // SHOWVECTOR_H
