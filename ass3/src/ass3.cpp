#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <typeinfo>
using namespace std;

const unsigned char FIRST_BIT = 128;
const unsigned char LAST_BIT = 1;
const unsigned char LOWER_MASK = 15; //Keep the lower bits

int numDoublesToParse(int f_nibble, int l_nibble);
string getColor(int l_nibble);

class Thing
{
public:
	virtual ~Thing () {};
	string getName() const;

	enum Thing_Type 
	{
		Cylinder = 1 << 0,
		Sphere = 1 << 1,
		RectangularPrism = 1 << 2,
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
};

string Thing::getName() const
{
	string type_name = typeid(*this).name();
#ifdef _MSC_VER       // for MS Visual Studio
    type_name = type_name.substr(6);
#else                 // for other compilers
    type_name = type_name.substr(type_name.find_first_not_of("0123456789"));
#endif
	return type_name;
}

class GeometricSolid : virtual public Thing
{
public:
	virtual float volume() const = 0;
	~GeometricSolid() {}
	friend ostream& operator<<(ostream&, const GeometricSolid*);
};

ostream& operator<<(ostream& out, const GeometricSolid* ptrGS)
{
	out << setprecision(4) << fixed;
	out << setw(27) << left << ptrGS->getName() 
	<< setw(9) << right << ptrGS->volume();
	return out;
}

class Circular : virtual public GeometricSolid
{
protected:
	float radius;
public:
	Circular(float r) : radius(r) {}
};

class RectangularPrism : virtual public GeometricSolid
{
protected:
	float l, w, h, s;
	
public:
	RectangularPrism(float length, float width, float height)
		: l(length), w(width), h(height){}
	RectangularPrism(float side)
		: s(side) {}
	~RectangularPrism() {}
	float volume() const;
};

float RectangularPrism::volume() const
{ return (l*w*h);}

class ColoredThing : virtual public Thing
{
protected:
	string color;
	double weight;
public:
	ColoredThing() = default;
	ColoredThing(string c, double w) : color(c), weight(w) {}
	~ColoredThing() {}
	virtual double density() const = 0;
	friend ostream& operator<<(ostream&, const ColoredThing*);
};

ostream& operator<<(ostream& out, const ColoredThing* ptrCT)
{
	const Thing *pT = dynamic_cast<const Thing *>(ptrCT); //Upcast
	out << dynamic_cast<const GeometricSolid*>(pT); //Downcast
	out << " " << setw(14) << left << ptrCT->color << ptrCT->density();
	return out;
}

class Cylinder : public Circular
{
protected:
	double height;

public:
	Cylinder(float r, double h) 
		: Circular(r), height(h) {}
	float volume() const;
};

float Cylinder::volume() const
{ return M_PI * (radius * radius) * height;}

class Sphere : public Circular
{
public:
	Sphere(float r) : Circular(r) {} 
	float volume() const;
};

float Sphere::volume() const
{ return (4.f/3.f) * M_PI * (radius*radius*radius); }

class Cube : public RectangularPrism
{
public:
	Cube(double s) : RectangularPrism(s) {}
	float volume() const;
};

float Cube::volume() const
{ return s*s*s;}

class ColoredCylinder : virtual public ColoredThing , public Cylinder
{
public:
	ColoredCylinder(double h, float r, string c, double w)
		: ColoredThing(c, w), Cylinder(h, r) {}
	double density() const;
};

double ColoredCylinder::density() const
{ return weight / volume();}

class ColoredSphere : virtual public ColoredThing, public Sphere
{
public:
	ColoredSphere(float r, string c, double w)
		: ColoredThing(c, w), Sphere(r) {}
	double density() const;
};

double ColoredSphere::density() const
{ return weight / volume();}

class ColoredRectangularPrism : virtual public ColoredThing, public RectangularPrism
{
public:
	ColoredRectangularPrism(float length, float width, float height, string color, double weight)
		: ColoredThing(color, weight),  RectangularPrism(length, width, height) {}
	double density() const;
};

double ColoredRectangularPrism::density() const
{ return weight / volume();}

class ColoredCube : virtual public ColoredThing, public Cube
{
public:
	ColoredCube(double s, string c, double w)
		: ColoredThing(c, w), Cube(s) {}
	double density() const;
};

double ColoredCube::density() const
{ return weight / volume();}


void printThingOutput(Thing* ptrThing, int hasColor);
void assignTypeToThing(Thing* &pThing, double d_values[4], int f_nibble, int l_nibble);

int main()
{
	int numDoubles;
	unsigned char byte;
	int f_nibble, l_nibble;
	double d_values[4];
	char *memblock;
    ifstream fin("ass3data.bin", ios::binary);
	Thing *ptrThing;

    if (!fin)
    {
        cerr << "Input binary file could not be opened" << endl;
        exit(1);
    }
	cout << "Type of Object" << setw(28) << "Volume Color" << setw(15) << "Density" << endl;
    while (fin.read(reinterpret_cast<char*>(&byte), 1)) //Using fin >> byte would skip over whitespace chars which are considered to be encoded as well
    {
		byte ^= FIRST_BIT;
        byte ^= LAST_BIT;
        f_nibble = static_cast<int>((byte>>4) & LOWER_MASK); //Shift the higher end nibble to the lower end nibble, then apply mask
		l_nibble = static_cast<int>(byte & LOWER_MASK); //Apply mask to obtain lower end nibble
		numDoubles = numDoublesToParse(f_nibble, l_nibble); //Determines how many doubles to read in after first byte
		for (int i = 0; i < numDoubles; i++)
		{
			memblock = new char[8];
			fin.read(memblock, 8);
			d_values[i] = *(reinterpret_cast<double*>(memblock));
            delete [] memblock;
		}
		assignTypeToThing(ptrThing, d_values, f_nibble, l_nibble);
		printThingOutput(ptrThing, l_nibble);
		delete ptrThing;
    }
	fin.close();
    return 0;
}

int numDoublesToParse(int f_nibble, int l_nibble)
{
	int ret;

	switch(f_nibble)
	{
		case (Thing::Cylinder):
			ret = 2;
			break;
		case (Thing::Sphere):
			ret = 1;
			break; 
		case (Thing::RectangularPrism):
			ret = 3;
			break;
		case (Thing::Cube):
			ret = 1;
			break;
		default:
			;
	}
	return (l_nibble ? ++ret : ret);
}

void printThingOutput(Thing* ptrThing, int hasColor)
{
	if(hasColor)
		cout << dynamic_cast<ColoredThing *>(ptrThing);
	else
		cout << dynamic_cast<GeometricSolid *>(ptrThing);
	cout << endl;
}

void assignTypeToThing(Thing* &pThing, double d_values[4], int f_nibble, int l_nibble)
{
	switch(f_nibble)
	{
		case (Thing::Cylinder):
		{
			if (l_nibble)
				pThing = new ColoredCylinder(d_values[0], d_values[1], getColor(l_nibble), d_values[2]);
			else
				pThing = new Cylinder(static_cast<float>(d_values[0]), d_values[1]);
			break;
		}
		case (Thing::Sphere):
		{
			if (l_nibble)
				pThing = new ColoredSphere(static_cast<float>(d_values[0]), getColor(l_nibble), d_values[1]);
			else
				pThing = new Sphere(static_cast<float>(d_values[0]));
			break; 
		}
		case (Thing::RectangularPrism):
		{
			if (l_nibble)
				pThing = new ColoredRectangularPrism(d_values[0], d_values[1], d_values[2], getColor(l_nibble), d_values[3]);
			else
				pThing = new RectangularPrism(static_cast<float>(d_values[0]), static_cast<float>(d_values[1]), static_cast<float>(d_values[2]));
			break;
		}
		case (Thing::Cube):
		{
			if(l_nibble)
				pThing = new ColoredCube(d_values[0], getColor(l_nibble), d_values[1]);
			else
			{
				pThing = new Cube(d_values[0]);
			}
			break;
		}
		default:
			;
	}
}

string getColor(int l_nibble)
{
	string color;
	switch(l_nibble)
	{
		case (Thing::Red):
			color = "Red";
			break;
		case (Thing::Blue):
			color = "Blue";
			break;
		case (Thing::Green):
			color = "Green";
			break;
		case (Thing::Yellow):
			color = "Yellow";
			break;
		default:
			;
	}
	return color;
}

