
#include "Handy.h"

using namespace std;

//Constructor:
Handy::Handy(int i)
{
    genRCMap();
	genColorSpaceMap();
	srand((unsigned) time(NULL));
}

/*************************************************************************************
CONVERSIONS
*************************************************************************************/

int Handy::getIntFromString(string str)
{
        int x;
        istringstream inputString(str);

        if(!(inputString>>x))
        {
            cerr << "\nError: Tried to convert this string "<<str<<" which is not an integer to an integer.";
            exit(-1);
        }

        return x;
}

//Basic round function
int Handy::getIntFromDouble(double r)
{    
    return (int)(r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

unsigned int Handy::getUnsignedIntFromString(string str)
{
        unsigned int x;
        istringstream inputString(str);

        if(!(inputString>>x))
        {
            cerr << "\nError: Tried to convert this string "<<str<<" which is not an integer to an integer.";
            exit(-1);
        }

        return x;

}
string Handy::getStringFromInt(int i)
{
	string s;
	std::ostringstream os; 
	os << i;
	s += os.str(); 
	return s;
} 


string Handy::getStringFromChar(char c)
{
	string s;
	std::ostringstream os; 
	os << c;
	s += os.str(); 
	return s;
} 

string Handy::getStringFromBool(bool b)
{
	string s;
	std::ostringstream os; 
	os << b;
	s += os.str(); 
	return s;
} 




string Handy::getStringFromUnsignedInt(unsigned int i)
{
	string s;
	std::ostringstream os; 
	os << i;
	s += os.str(); 
	return s;
} 

string Handy::getStringFromDouble(double i)
{
	string s;
	std::ostringstream os; 
	os << i;
	s += os.str(); 
	return s;
} 


/*************************************************************************************
REVERSE COMPLEMENT STUFF
*************************************************************************************/
//Returns the reverse complement sequence
//assumes a lower case sequence including a, c, g, t, u only
string Handy::getRevComp(string watson)
{
	    string crick;
        crick="";
        
        for(int i=watson.length()-1; i>-1; --i)
        {
              crick.append(getCompBase(watson.substr(i,1)));
        }
		
	    return crick;

}

//Returns complementary base
string Handy::getCompBase( string inBase )
{
        string outBase;
        outBase="X";

        if(rcMap.find(inBase)==rcMap.end())
		{
                cout<<"\nWarning: A Non ATGCURYKMSWBHDVN- base in getCompBase. Replacing it with X. Base: "<<inBase;
        }
		else
		{
			outBase=rcMap[inBase];
		}
	
        return outBase;
}



/*********************************************************************************************
SPLIT A STRING STUFF
**********************************************************************************************/
vector <string> Handy::getSplitString(string str, string delim )
{
        size_t cutAt;
        vector <string> results;
        
        while( (cutAt = str.find(delim)) != str.npos )
        {
                if(cutAt > 0)
                {
                        results.push_back(str.substr(0,cutAt));
                }
                str = str.substr(cutAt+delim.length());  //cuts the string smaller each time ... maybe could get this more efficient
        }

        if(str.length() > 0)
        {
                results.push_back(str);
        }

        return results;
}
/*********************************************************************************************
Random Integer Stuff
**********************************************************************************************/
//Returns a vector of size numIntegers ranging from min to max
vector <int> Handy::getUniformRandomIntegers(int values, int min, int max)
{	
	vector<int> randomUniformInts;
	int i;

	for(i=0; i<values; ++i)
	{
		randomUniformInts.push_back((int) min+ rand()/(RAND_MAX/(max - min + 1) + 1));
	}
	
	return randomUniformInts;

}

//Returns a vector of size numIntegers ranging from min to max
int Handy::getUniformRandomInteger(int min, int max)
{	
	vector<int> randomUniformInts;
	
	return ((int) min+ rand()/(RAND_MAX/(max - min + 1) + 1));

}



//Returns a vector of size numIntegers ranging from min to max with a mean of...
vector <int> Handy::getNormalRandomIntegers(int values, double mean, double std)
{
	static const double pi=3.1415926535879; //how I need a drink alcholic of course after the heavy lectures involving quantum mechanics
	static const double r_max=RAND_MAX+1;
	
	vector<int> randomNormalInts;
	int i;
	
	for(i=0; i<values; ++i)
	{
		randomNormalInts.push_back((int) std*sqrt(-2*log((rand()+1)/r_max))*sin(2*pi*rand()/r_max)+mean);
	}

	return randomNormalInts;
}
//Returns an integer from the normal distribution..
int Handy::getNormalRandomInteger(double mean, double std)
{
	static const double pi=3.1415926535879; //how I need a drink alcholic of course after the heavy lectures involving quantum mechanics
	static const double r_max=RAND_MAX+1;

	vector<int> randomNormalInts;

	return ((int) std*sqrt(-2*log((rand()+1)/r_max))*sin(2*pi*rand()/r_max)+mean);
	
}

string Handy::getReverse(string forwards)
{
        int i;
        string backwards;
        backwards="";
   
        for(i=forwards.length()-1; i>-1; --i)
        {
              backwards.append(forwards.substr(i,1));
        }

        return backwards;
}
//Copied from http://www.dreamincode.net/code/snippet39.htm
string Handy::StringToUpper(string strToConvert)
{//change each element of the string to upper case
   for(unsigned int i=0;i<strToConvert.length();i++)
   {
      strToConvert[i] = toupper(strToConvert[i]);
   }
   return strToConvert;//return the converted string
}

//Copied from http://www.dreamincode.net/code/snippet39.htm
string Handy::StringToLower(string strToConvert)
{//change each element of the string to lower case
   for(unsigned int i=0;i<strToConvert.length();i++)
   {
      strToConvert[i] = tolower(strToConvert[i]);
   }
   return strToConvert;//return the converted string
}

//Compare strings but ignore the case
bool Handy::cmpStringNoCase(string string1, string string2)
{
   string1=StringToLower(string1);
   string2=StringToLower(string2);
   
   if(string1==string2)
   {
		return 1;
   }
   else
   {
		return 0;
   }
   
   
}



/*===============================================
This converts a string AB ColorSpace into 
Base Space
====================================================*/
string Handy::getBaseSpaceFromColorSpace(string csSeq)
{

	string bsSeq="";

	if(csSeq.length()>0)
	{
		string prev=csSeq.substr(0,1);

		for(unsigned int i=1; i<csSeq.length(); ++i)
		{		
			string seqPart=prev+csSeq.substr(i,1);					
					
			if(colorSpaceMap.find(seqPart)==colorSpaceMap.end())
			{
				prev="N";
			}

			else
			{
				prev=colorSpaceMap[seqPart];				
			}

			bsSeq.append(prev);
		}
	}

    return bsSeq;

}

/*===============================================
This converts a string in Mosaik pseudo colorspace to 
actual color space
====================================================*/
string Handy::getColorSpaceFromPseudoColorSpace(string csSeq)
{
	string bsSeq="T";
	
	for(unsigned int i=0; i<csSeq.length(); ++i)
	{		
		string seqPart=csSeq.substr(i,1);	
		string prev="";
					
		if(pseudoColorSpaceMap.find(seqPart)==pseudoColorSpaceMap.end())
		{
			prev=".";
		}

		else
		{
			prev=pseudoColorSpaceMap[seqPart];				
		}

		bsSeq.append(prev);	

		
	}

    return bsSeq;

}

/*********************************************************************************************
Populating Maps Stuff
**********************************************************************************************/
//Returns a vector of size numIntegers ranging from min to max

void Handy::genColorSpaceMap()
{
	
	colorSpaceMap.insert(map< string, string >::value_type("A0", "A"));
	colorSpaceMap.insert(map< string, string >::value_type("A1", "C"));
	colorSpaceMap.insert(map< string, string >::value_type("A2", "G"));
	colorSpaceMap.insert(map< string, string >::value_type("A3", "T"));

	colorSpaceMap.insert(map< string, string >::value_type("C0", "C"));
	colorSpaceMap.insert(map< string, string >::value_type("C1", "A"));
	colorSpaceMap.insert(map< string, string >::value_type("C2", "G"));
	colorSpaceMap.insert(map< string, string >::value_type("C3", "T"));

	colorSpaceMap.insert(map< string, string >::value_type("G0", "G"));
	colorSpaceMap.insert(map< string, string >::value_type("G1", "T"));
	colorSpaceMap.insert(map< string, string >::value_type("G2", "A"));
	colorSpaceMap.insert(map< string, string >::value_type("G3", "C"));

	colorSpaceMap.insert(map< string, string >::value_type("T0", "T"));
	colorSpaceMap.insert(map< string, string >::value_type("T1", "G"));
	colorSpaceMap.insert(map< string, string >::value_type("T2", "C"));
	colorSpaceMap.insert(map< string, string >::value_type("T3", "A"));

	pseudoColorSpaceMap.insert(map< string, string >::value_type("A", "0"));
	pseudoColorSpaceMap.insert(map< string, string >::value_type("C", "1"));
	pseudoColorSpaceMap.insert(map< string, string >::value_type("G", "2"));
	pseudoColorSpaceMap.insert(map< string, string >::value_type("T", "3"));

}

/*This generates map of reverse complements of bases
Note that U/u returns A/a and A/a returns T/t
*/
void Handy::genRCMap()
{		
        /*Lower Case*/
		rcMap.insert(map< string, string >::value_type("a", "t"));
		rcMap.insert(map< string, string >::value_type("c", "g"));
		rcMap.insert(map< string, string >::value_type("g", "c"));
		rcMap.insert(map< string, string >::value_type("t", "a"));
		rcMap.insert(map< string, string >::value_type("u", "a"));
		rcMap.insert(map< string, string >::value_type("r", "y"));
		rcMap.insert(map< string, string >::value_type("y", "r"));
		rcMap.insert(map< string, string >::value_type("k", "m"));
		rcMap.insert(map< string, string >::value_type("m", "k"));
		rcMap.insert(map< string, string >::value_type("s", "w"));
		rcMap.insert(map< string, string >::value_type("w", "s"));
		rcMap.insert(map< string, string >::value_type("b", "v"));
		rcMap.insert(map< string, string >::value_type("d", "h"));
		rcMap.insert(map< string, string >::value_type("h", "d"));
		rcMap.insert(map< string, string >::value_type("v", "b"));
		rcMap.insert(map< string, string >::value_type("A", "T"));
		rcMap.insert(map< string, string >::value_type("C", "G"));
		rcMap.insert(map< string, string >::value_type("G", "C"));
		rcMap.insert(map< string, string >::value_type("T", "A"));
		rcMap.insert(map< string, string >::value_type("U", "A"));
		rcMap.insert(map< string, string >::value_type("R", "Y"));
		rcMap.insert(map< string, string >::value_type("Y", "R"));
		rcMap.insert(map< string, string >::value_type("K", "M"));
		rcMap.insert(map< string, string >::value_type("M", "K"));
		rcMap.insert(map< string, string >::value_type("S", "W"));
		rcMap.insert(map< string, string >::value_type("W", "S"));
		rcMap.insert(map< string, string >::value_type("B", "V"));
		rcMap.insert(map< string, string >::value_type("V", "B"));
		rcMap.insert(map< string, string >::value_type("D", "H"));
		rcMap.insert(map< string, string >::value_type("H", "D"));
		rcMap.insert(map< string, string >::value_type("N", "N"));
		
}

//Get sign of an integer
char Handy::getSignOfInt(int i)
{
	if(i>=0)
	{
		return '+';
	}

	else
	{
		return '-';
	}
}

unsigned int Handy::checkRead(string inFileName)
{
	unsigned int ok=0;

	ifstream inStream(inFileName.c_str(), ios::in);		

	if (!inStream )
	{
		ok=1;
		
	}

	inStream.close();

	return ok;
}

unsigned int Handy::checkWrite(string outFileName)
{
	unsigned int ok=0;

	ofstream outStream(outFileName.c_str(), ios::app);

	if ( !outStream ){

		ok=1;
		cout<<"WARNING: The file "<<outFileName<<" cannot be opened for writing."<<endl;
    }

	outStream.close();

	return ok;
}


double Handy::getDoubleFromString(string str)
{
        double x;
        istringstream inputString(str);

        if(!(inputString>>x))
        {
			
            cerr << "Error: Tried to convert this string "<<str<<" which is not a number to a double."<<endl;
            exit(-1);
        }

        return x;
}

void Handy::printTime()
{	
	time_t t;

    time(&t);

	if(ctime(&t))
    {
        cout<<"Current time is: "<<ctime(&t)<<endl;
    }

    else
    {
		cout<<"WARNING: Something went wrong when printing time was called!";
    }
}

string Handy::getTime()
{	
	time_t t;

    time(&t);

	if(ctime(&t))
    {
        return string(ctime(&t));
    }

    else
    {
		return("Time unavailable");
    }
}

string Handy::getDateTimeAsStr()
{	
   time_t now;
   char the_date[18];

   the_date[0] = '\0';

   now = time(NULL);

   if (now != -1)
   {
      strftime(the_date, 18, "%H_%M_%d_%m_%Y", gmtime(&now));
   }

   return std::string(the_date);

}



//This trims trailing and ending spaces from a string
string Handy::trim(string str)
{
	//Remove trailing spaces
	while(str.substr(str.length()-1,1)==" ")
	{
		str=str.substr(0,str.length()-1);
	}


	//Remove leading spaces
	while(str.substr(0,1)==" ")
	{
		str=str.substr(1);
	}

	return str;
}

//This trims trailing slash from the end of a string (for consistency in directory names)
string Handy::trimFinalSlash(string str)
{
	//Remove trailing spaces
	while(str.substr(str.length()-1,1)=="\\" || str.substr(str.length()-1,1)=="/" )
	{
		str=str.substr(0,str.length()-1);
	}

	return str;
}

//This trims trailing and ending spaces from a string
string Handy::removeAllSpace(string str)
{
	string str1="";
	string s;
	
	for(int i=0; i<str.length(); ++i)
	{
		s=str.substr(i,1);
		if(s!=" " && s!="\\")
		{
			str1.append(s);
		}
	
	}
	
	return str1;
}


/*********************************************************************************************
Math Etc.
**********************************************************************************************/
//Mean

double Handy::getMeanOfAVector(vector<double> v)
{
	double mean;
	double st=0.0;
	int i;
	double n=0.0;
	

	for(i=0;i<v.size();i++)
	{
        st=st+v[i];
		++n;
	}
		
	if(n ==0)
	{
		return 999999999999;
	}
	
	mean=st/(n);
	
	
	return mean;
}

//Std
double Handy::getStdOfAVector(vector<double> v)
{
	double stDev;
	double m;
	double sumDev;
	int i;
	double n=0;//Number of elements
		
	m=getMeanOfAVector(v);
	
	sumDev=0.0;
	
	for(i=0; i<v.size(); i++)
	{
        sumDev=sumDev+(pow(v[i]-m,2));
		
		++n;
	}
		
	if(n>1.0) 
	{
		double sd1;
		double sd2;
		sd1=(1/(n-1));
		sd2=sd1*sumDev;	
		stDev=sqrt(sd2);
	}
	else{
		stDev=99999999999;
	}
	
	return stDev;
}
