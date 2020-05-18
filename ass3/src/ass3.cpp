#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

int numDoublesToParse(int f_nibble, int l_nibble);

const unsigned char FIRST_BIT = 128;
const unsigned char LAST_BIT = 1;
const unsigned char LOWER_MASK = 15; //Keep the lower bits

enum Geometric_Type 
{
	Cylinder = 1 << 0,
	Sphere = 1 << 1,
	Rectangular_Prism = 1 << 2,
	Cube = 1 << 3,
};

enum Color_Code
{
	No_color = 0,
	Red = 1 << 0,
	Blue = 1 << 1,
	Green = 1 << 2,
	Yellow = 1 << 3,
};

class Thing
{
protected:
	string class_name;
public:
	Thing(string n) : class_name(n) {}
	Thing() = default;
	string name() const { return class_name;}
	friend ostream& operator<<(ostream&, const Thing*&);
};

ostream& operator<<(ostream& out, const Thing* &thing)
{

	return out;
}

class GeometricSolid : public Thing
{
public:
	virtual double volume() const = 0;
};

class Circular : public GeometricSolid
{
protected:
	float radius;
};

class RectangularPrism : virtual public GeometricSolid
{
protected:
	float l, w, h, s;
	
public:
	double volume() const;
};

double RectangularPrism::volume() const
{ return (l*w*h);}

class ColoredThing : public Thing
{
protected:
	string color;
	double weight;
public:
	virtual double density() const = 0;
};

class Cylinder : virtual public Circular
{
protected:
	double height;

public:
	double volume() const;
};

double Cylinder::volume() const
{ return M_PI * (radius * radius) * height;}

class Sphere : virtual public Circular
{
public:
	double volume() const;
};

double Sphere::volume() const
{ return (4./3.) * M_PI * (radius*radius*radius);}

class Cube : public RectangularPrism
{
public:
	double volume() const;
};

double Cube::volume() const
{ return s*s*s;}

class ColoredCylinder : virtual public ColoredThing , public Cylinder
{
public:
	double density() const;
};

double ColoredCylinder::density() const
{ return weight / volume();}

class ColoredSphere : virtual public ColoredThing, public Sphere
{
public:
	double density() const;
};

double ColoredSphere::density() const
{ return weight / volume();}

class ColoredRectangularPrism : virtual public ColoredThing, public RectangularPrism
{
public:
	double density() const;
};

double ColoredRectangularPrism::density() const
{ return weight / volume();}

class ColoredCube : virtual public ColoredThing, public Cube
{
public:
	double density() const;
};

double ColoredCube::density() const
{ return weight / volume();}

int main()
{
	int numDoubles;
	unsigned char byte;
	int f_nibble, l_nibble;
	double number;
	char *memblock;
    ifstream fin("ass3data.bin", ios::binary);

    if (!fin)
    {
        cerr << "Input binary file could not be opened" << endl;
        exit(1);
    }
    while (fin.read(reinterpret_cast<char*>(&byte), 1)) //Using fin >> byte would skip over whitespace chars which are considered to be encoded as well
    {
		byte ^= FIRST_BIT;
        byte ^= LAST_BIT;
        f_nibble = static_cast<int>((byte>>4) & LOWER_MASK); //Shift the higher end nibble to the lower end nibble, then apply mask
		l_nibble = static_cast<int>(byte & LOWER_MASK); //Apply mask to obtain lower end nibble
		cout << "first nibble: " << f_nibble << " last nibble: " << l_nibble << endl;
		for (int i = 0; i < numDoubles; i++)
		{
			memblock = new char[8];
			fin.read(memblock, 8);
			number = *(reinterpret_cast<double*>(memblock));
            delete [] memblock;
		}
		abort();
		//pass first & last nibble to function
    }
	fin.close();
    return 0;
}

int numDoublesToParse(int f_nibble, int l_nibble)
{
	int ret;

	switch(f_nibble)
	{
		case Cylinder:
		{
			ret = 2;
			break;
		}
		case Sphere:
		{
			ret = 1;
			break; 
		}
		case Rectangular_Prism:
		{
			ret = 3;
			break;
		}
		case Cube:
		{
			ret = 1;
			break;
		}
	}
	return (l_nibble ? ++ret : ret);
}