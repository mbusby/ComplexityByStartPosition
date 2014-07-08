/*
Developed by: Michele Busby, Computational Biologist
Broad Technology Labs/MBRD
Broad Institute 
7/02/2014

Gets the complexity of a library based on the number of 
unique start positions that are found as a function of
read depth.  

This is used mainly as a quality metric to detect libraries 
with complexity problems and libraries have been over-sequenced.

I have used it on RNA Seq libraries. 

The output is two files:
	stub_counts.txt - each start position and the number of times it showed up.  Chromosomes are named by the order they showed up
			You probably don't need this file unless you want to draw distributions or something.
	stub_rarefactionData.txt - This file has the number of reads sequenced and how many unique reads there are.
			Use this one to make a rarefaction plot where the number of reads sequenced is the x axis and the number of unique reads 
			is the y axis.  For high complexity libraries that are not sequenced to saturation this will form a fairly straight line.
			For oversequenced low complexity libraries the line will asymptote at the point where you stop 
			adding new information when you add more reads (i.e. everything is just a PCR duplicate of what you already have -or- you 
			have a very, very small transcriptome sequenced very, very deeply).
	

It dumps a hash map into memory.  It might segment fault
if you have more read positions than memory available.

*Useage: 
 * ./ComplexityByStartPos -bam /FolderWhereAlignmentIs/myAlignment.bam -out ./output -points 30
 
 	-bam Name of the bam file.  Only works for paired end.  I could fix this to do single end. email me if you need it.
	-out Name of a stub to be used for the output (e.g. /myFiles/thisSample )
	-points How many points you want to sample.  Default is 20.  If you want it to go faster use 10.
	-single_end add if the read is a single end library (default assumes a paired end library)

How it works:
	Makes a map of reads (chr_Start1_strand-chr_Start2_strand ) and the count of how many times each read shows up

	Iterates through map and prints:
		Read and the number of times it showed up
		Does a downsampling at 20 points using 20 binomial probabilities.  This provides a virtual down sampling. 
			(i.e. to get the unique reads at 10% you would take each gene and downsample the count by a binomial probability of 0.1)
		Ignores whether they map uniquely or multiply so if you only want unique you have to filter your bam file first.
		Accordingly, counts each alignment as a "Read Sequenced." Therefore, if it maps 10 places it counts it as 10 reads. This 
			is not wonderful but seems internally consistent.
		More specifically:
			Paired end:
			Pairs Sequenced = number of first mates found in the bam file (mapped or unmapped)
			Pairs Aligned = number of mapped first mates found in the bam file which have a mapped mate
			Unique Alignment Positions = the number of unique positions based on a unique start pos of first and second mate
			
			Single end:
			Reads Sequenced = number of reads found in the bam file (mapped or unmapped)
			Reads Aligned = number of mapped reads in the bam file
			Unique Alignment Positions = the number of positions where a read aligns
			
	
	
How to get it to compile:
	If the unix binary doesn't work for you, you may need to compile it.
	This may or may not be a big hassle because you need both Boost and bamtools.
	
	First, though, copy all the files in here to a single directory on your unix server where g++ is installed (it probably is).
	
	Boost
	
	You need the Boost C++ library to throw a binomial random number.
	If it is not already installed try downloading it here: http://www.boost.org/
	
	I did not have to install it but you need to install it and add it to your path so that
	g++ can find it.  
	If you happen to have use of a Broad server you can get it to use Boost by typing: use .boost-1.54.0
	(Yes, we are very spoiled).
	If you don't know if Boost is installed skip to the next section and see if it breaks.  It is fairly common
	so it may already be in the install.

	Bamtools
	
	To get this to compile you will need to download Derek Barnett's bamtools API and install it in some folder.
	It is here: https://github.com/pezmaster31/bamtools	
	You need the API, not the command line program though that has some useful stuff in it.
	Then, after you install it, you need to edit the Make file so that everywhere it says "FolderWhereBamToolsIs"
	you put in the folder where bam tools is located.
	
	Compiling
	
	Go to the folder where everything is installed in type "Make".  Ignore the warnings about Handy doing stupid stuff.
	If nothing says Error and it makes an executable called ComplexityByStartPos you're all set.	
	

*/

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "api/BamReader.h"
#include "Handy.h"
#include "boost/random.hpp"
#include "boost/generator_iterator.hpp"
#include "boost/math/distributions/binomial.hpp"


unsigned int checkErrors();
void displayHelp();
void somethingsGoneWrong(unsigned int);
void writeUnique();
void countUnique();
void generateRarefaction();
void writeRarefaction();

//Variables taken from the inputs
string bamFileName="";
string outFileName="";
string jobName;
string outDirName="./";
unsigned int samplePoints=20;
bool singleEnd=false;


double mapQualCut=0;
unsigned int totalReads=0;
unsigned int totalAlignedReads=0;

map<string, int> ctReads;

vector<int>rarReads;
vector<int>rarAlignedReads;
vector<int>rarUniqueReads;

using boost::math::binomial_distribution;
using namespace std;
using namespace BamTools;



int main(int argc, char* argv[]) 
{	
	
	//Intialize library of useful things
	Handy h(0);
	
	int optind=1;


	while ((optind < argc) && (argv[optind][0]=='-')) 
	{	
        string sw = argv[optind];
				
		if (sw=="-h") 
		{	
            optind++;
			displayHelp();
			return 1;
        }
		
		else if(optind >=  argc-1 && sw!="-single_end" )
		{
			cerr<<"Your final parameter, "<<sw<<" is missing a value."<<endl;
			return 1;
		}

		else if (sw=="-bam")
		{	
            optind++;
			bamFileName = argv[optind];		
			optind++;
        }
		
		else if (sw=="-out")
		{	
            optind++;
			outFileName = argv[optind];		
			optind++;
        }
		
		else if (sw=="-points")
		{	
            optind++;
			samplePoints = h.getIntFromString(argv[optind]);	
			optind++;
        }
		else if (sw=="-single_end")
		{	
            optind++;
			singleEnd = true;	
			//does not increment, only one option
        }
		else
		{
			cerr<<"Main: Unknown parameter:"<<sw<<endl;
			displayHelp();
			return 1;
		}
	}	
	
	unsigned int errCheck = checkErrors();

	countUnique();
	cout<<ctReads.size()<<" unique mapping positions were found."<<endl;
	
	writeUnique();	
	generateRarefaction();
	writeRarefaction();	
	
	cout<<"Done"<<endl;
		
}

void writeUnique()
{
	
	//Open output stream to write the findings for each transcript
	ofstream outputStream;
	string outFileCountsName=outFileName+"_counts.txt";
	outputStream.open(outFileCountsName.c_str());
	
	outputStream<<"Position"<< "\t"<<"Count"<<"\n";	
		
	typedef std::map< string, int>::iterator it_type;
	
	for(it_type iterator = ctReads.begin(); iterator != ctReads.end(); iterator++) 
	{
		outputStream<< iterator->first << "\t"<<iterator->second<<"\n";	
	}

}

/*===========================================
Generate the data for a rarefaction plot
============================================*/

void generateRarefaction()
{	
	int foundUnique=0;
	
	if(ctReads.size()==0)
	{
		cout<<"No data available."<<endl;
		return;
	}
	
	cout<<"Running rarefaction sampling."<<endl;
	
	rarAlignedReads.push_back(0);
	rarReads.push_back(0);
	rarUniqueReads.push_back(0);
	
	for(int i=1; i<samplePoints; ++i)
	{
		cout<<"Sampling "<<i<<" of "<<samplePoints<<endl;
		
		foundUnique=0;
		const double success_fraction = double(i)/double(samplePoints); 
		
		typedef std::map< string, int>::iterator it_type;
		boost::random::mt19937 rng;  // produces randomness out of thin air
	
		for(it_type iterator = ctReads.begin(); iterator != ctReads.end(); iterator++) 		{
						       
                                    // see pseudo-random number generators
			boost::random::binomial_distribution<> rndBinomial(iterator->second,success_fraction);
                                    // distribution that maps to 1..6
                                    // see random number distributions
			int x = rndBinomial(rng);   
			
			if(x>0)
			{
				++foundUnique;
			}
		}	
		
		rarAlignedReads.push_back(int(totalAlignedReads*success_fraction));
		rarReads.push_back(int(totalReads*success_fraction));
		rarUniqueReads.push_back(foundUnique);
	}
	
	rarReads.push_back(totalReads);
	rarAlignedReads.push_back(totalAlignedReads);
	rarUniqueReads.push_back(ctReads.size());
	
	

}

/*===========================================
Write the data for a rarefaction plot
============================================*/

void writeRarefaction()
{	
	//Open output stream to write the findings for each transcript
	ofstream outputStream;
	string outFileRarefactionName=outFileName+"_rarefactionData.txt";
	outputStream.open(outFileRarefactionName.c_str());
	
	outputStream<< "Bam file: " << bamFileName<<endl;
	
	
		
	if(singleEnd==true)
	{
		outputStream<< "Pairs Sequenced" << "\t"<<"Pairs Aligned" << "\t"<<"Unique Alignment Positions"<<endl;		
	}
	else
	{
		outputStream<< "Reads Sequenced" << "\t"<<"Reads Aligned" << "\t"<<"Unique Alignment Positions"<<endl;	
	}
	
	
	for(int i=0; i<rarReads.size(); ++i)
	{
		outputStream<< rarReads[i]<<"\t"<< rarAlignedReads[i]<<"\t"<<rarUniqueReads[i]<<endl;
	}
}


void countUnique()
{
	Handy h(0);
	BamReader reader;
	BamAlignment al;
	RefVector refVector;
		
	int ch=0;
	
	if ( !reader.Open(bamFileName) ) 
	{
		cerr << "ERROR: Could not open input BAM file " <<bamFileName<< endl;
		return;
	}
	
	int i=0;
		
	while(reader.GetNextAlignment(al))
	{	
		++i;	
		if (al.IsFirstMate()==true)
		{
			++totalReads;
		}
		
		if(i%1000000==0)
		{
			cout<<"Reading line "<<i<<" of alignment for position map."<<endl;
		}
		
		if(singleEnd==true)
		{	
			
			if (al.IsMateMapped()==true)
			{
				string uniqueMap = 	h.getStringFromInt(al.RefID) + "-" + h.getStringFromInt(al.Position) + "-" + 
						h.getStringFromBool(al.IsReverseStrand());				
				++ctReads[uniqueMap];	
				++totalAlignedReads;
			}
		}
		else
		{
			if (al.IsFirstMate()==true  && al.IsMateMapped()==true)
			{
				string uniqueMap = 	h.getStringFromInt(al.RefID) + "-" + h.getStringFromInt(al.Position) + "-" + 
						h.getStringFromBool(al.IsReverseStrand())+"|"+ 
						h.getStringFromInt(al.MateRefID) + "-" + h.getStringFromInt(al.MatePosition) + "-" + 
						h.getStringFromBool(al.IsMateReverseStrand());
				
				++ctReads[uniqueMap];	
				++totalAlignedReads;
			}
		}
	}
	
	cout<<"Total Pairs: "<<totalReads<<endl;
	cout<<"Total Aligned Pairs: "<<totalAlignedReads<<endl;
	
	reader.Close();
}


/*===========================================================================================
Check that all of the necessary fields exist.
===========================================================================================*/

unsigned int checkErrors()
{
	//Errors 

	int err=0;
	Handy h(0);
	string problems="";
	
	if(bamFileName.length()==0)
	{
		problems.append("A bam file containing the reads is needed (-bam).\n");
		++err;
	}
	
	if(outFileName.length()==0)
	{
		problems.append("A stub for an outfile is needed (-out).\n");
		++err;
	}		
		
	/*============================================================
	Check that all the relevant files can be read/written to
	==============================================================*/

	err=err+h.checkRead(bamFileName);	

	return err;
	
}


void displayHelp()
{

	cout<<"Required options:\n";	
	cout<<"-bam Name of the bam file\n";
	cout<<"-out Name of a stub to be used for the output (e.g. /myFiles/thisSample \n";
	cout<<"Optional options:\n";
	cout<<"-single_end Use for libraries which are single end (default is paired end)\n";
	cout<<"-points How many points you want to sample.  Default is 20.  If you want it to go faster use 10.  ";
	cout<<"If you want it to make a smooth line use 50. \n";

}


void somethingsGoneWrong(string whatsGoneWrong)
{

	cout<<"ERROR: Oh noooooooooooooo! Something has gone horribly wrong.\n";	
	cout<<whatsGoneWrong;
	cout<<"\n";	
	cerr<<"\nPlease try again.";
	
}
