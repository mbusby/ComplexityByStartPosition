#pragma once

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <stdlib.h>  // for srand(), rand() and RAND_MAX
#include <math.h>  // for sqrt(), log() and sin()
#include <complex> // for sqrt() of a negative
#include <time.h>  // for time()
////#include "AlignmentReader.h"


using namespace std;

//Structure for counting things
//with two sides (two strands or two halves)
struct doubleCt 
{
	unsigned int ct1;//forward strand or left half of read
	unsigned int ct2;

	doubleCt(void)
		: ct1(0)
		, ct2(0)
	
	{}
	
};

struct tripleCt 
{
	unsigned int ct1;//forward strand or left half of read
	unsigned int ct2;
	unsigned int ct3;

	tripleCt(void)
		: ct1(0)
		, ct2(0)
		, ct3(0)
	
	{}
	
};


struct quadCt 
{
	unsigned int ct1;
	unsigned int ct2;
	unsigned int ct3;
	unsigned int ct4;

	quadCt(void)
		: ct1(0)
		, ct2(0)
		, ct3(0)
		, ct4(0)
	
	{}
	
};

struct ctCoverage
{
	unsigned int forwardUnique;
	unsigned int forwardAll;
	unsigned int reverseUnique;
	unsigned int reverseAll;

	ctCoverage(void)
		: forwardUnique(0)
		, forwardAll(0)
		, reverseUnique(0)
		, reverseAll(0)
	
	{}
	
};
//Structure for my alignment
struct MyAlignment
{
		string ReadName;
		unsigned int ReferenceBegin;
		unsigned int ReferenceEnd;
		unsigned short ReferenceIndex;
		bool IsReverseComplement;
		string Reference;
		string Query;
		string Chromosome;	
		unsigned int Mismatches;
		unsigned int alignedPos;//number of aligned positions
		
		//Set defaults for my alignment
		MyAlignment(void)
			: ReferenceIndex(0)
			, IsReverseComplement(false)		
			, ReadName("")
			, alignedPos(0)
			
		{}

		bool operator<(const MyAlignment& as) const 
		{ 	
			if(Chromosome == as.Chromosome && ReferenceBegin==as.ReferenceBegin) return ReferenceEnd < as.ReferenceEnd; //chromosome start pos same then end
			if(Chromosome == as.Chromosome) return ReferenceBegin < as.ReferenceBegin; // chromosome then start
			return Chromosome < as.Chromosome;  //else just chromosome
		}
};



struct ReadData
{
	string seq;
	unsigned int aligned;
	double probPerfect;
	char annotatedArea; //0= never falls into or antisense to an annotated area; 
						//1= never in an annotated area, but antisense to an annotation area at least once;
						//2=falls into an annotation area (at least once); 
	ReadData(void)
		: annotatedArea('0')
	{}

};


//This structure defines what you count for an annotation record count

struct AnnCount
{
	//Sense
	unsigned int sReadsAligned;  //This is the total reads that align any portion of the annotated region, even by one base
	unsigned int sReadsAlignedUniquely;
	unsigned int sReadsAlignedCompletely;
	unsigned int sReadsAlignedUniquelyAndCompletely;
	unsigned int sBasesAligned;
	unsigned int sBasesAlignedUniquely;
	unsigned int sMismatches;
	unsigned int sInsertions;
	unsigned int sDeletions;
	unsigned int sSubstitutions;
	unsigned int sQuality;

	//Antisense
	unsigned int asReadsAligned;  //This is the total reads that align any portion of the annotated region, even by one base
	unsigned int asReadsAlignedUniquely;
	unsigned int asReadsAlignedCompletely;
	unsigned int asReadsAlignedUniquelyAndCompletely;
	unsigned int asBasesAligned;
	unsigned int asBasesAlignedUniquely;
	unsigned int asMismatches;
	unsigned int asInsertions;
	unsigned int asDeletions;
	unsigned int asSubstitutions;
	unsigned int asQuality;



	//Constructor
	AnnCount(void)
		: sReadsAligned(0)
		, sReadsAlignedUniquely(0)
		, sReadsAlignedCompletely(0)
		, sReadsAlignedUniquelyAndCompletely(0)
		, sBasesAligned(0)
		, sBasesAlignedUniquely(0)
		, sMismatches(0)
		, sInsertions(0)
		, sDeletions(0)
		, sSubstitutions(0)
		, sQuality(0)
		, asReadsAligned(0)
		, asReadsAlignedUniquely(0)
		, asReadsAlignedCompletely(0)
		, asReadsAlignedUniquelyAndCompletely(0)
		, asBasesAligned(0)
		, asBasesAlignedUniquely(0)
		, asMismatches(0)
		, asInsertions(0)
		, asDeletions(0)
		, asSubstitutions(0)
		, asQuality(0)
	{}
};



class Handy
{
	public:
			Handy(int);
			int getIntFromString(string);
			int getIntFromDouble(double);
			unsigned int getUnsignedIntFromString(string);
			double getDoubleFromString(string);
			double getMeanOfAVector(vector<double>);
			double getStdOfAVector(vector<double>);			
			string getStringFromInt(int);
			string getStringFromUnsignedInt(unsigned int);
			string getStringFromDouble(double);
			string getStringFromChar(char);
			string getStringFromBool(bool);
			string getRevComp(string);
			string trim(string);
			string trimFinalSlash(string);
			string removeAllSpace(string str);
			vector <string> getSplitString(string, string );
			vector <int> getUniformRandomIntegers(int, int, int);
			vector <int> getNormalRandomIntegers(int, double, double);
			

			int getUniformRandomInteger(int, int);
			int getNormalRandomInteger(double, double);
			string getReverse(string );
			string StringToUpper(string);
			string StringToLower(string);
			bool cmpStringNoCase(string, string);
			string getBaseSpaceFromColorSpace(string);
			char getSignOfInt(int i);
			unsigned int checkRead(string);
			unsigned int checkWrite(string);
			string getColorSpaceFromPseudoColorSpace(string);
			
			string getTime();	
			string getDateTimeAsStr();			
			void printTime();
	        
	private:
			string getCompBase( string inBase );
			map<string,string> rcMap;
			void genRCMap();
			map<string, string> colorSpaceMap;
			map<string, string> pseudoColorSpaceMap;
			void genColorSpaceMap();
};

