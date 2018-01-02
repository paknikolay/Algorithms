#pragma once
#include<fstream>
#include <sstream>
typedef char byte;
std::ifstream in_compressed("compressed.txt");
std::ofstream out_original("unzipped.txt");
std::ifstream in_original("input.txt");
std::ofstream out_compressed("compressed.txt");
std::stringstream stream;
class IInputStream {
public:
	virtual bool Read(byte& p)
	{
		in_original.read(&p, 1);
		
		return !in_original.eof();
	}
};
class IOutputStream {

	public:
	IOutputStream()
	{ //stream=new stringstream;
	}
	virtual	bool Write(byte& p)
	{
		stream.put(p);
		out_compressed.write(&p, 1);

		return true;
	}
};

class IInputStream2 {

public:

	virtual bool Read(byte& p)
	{
	//	in_compressed.read(&p, 1);
		//return in_compressed.gcount();
		stream.get(p);
		return !stream.eof();
	}
};
class IOutputStream2 {
public:
	virtual	bool Write(byte& p)
	{
		out_original.write(&p, 1);
		return true;
	}
};