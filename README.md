ComplexityByStartPosition
=========================

Creates data for a rarefaction curve to measure complexity in RNA Seq data based on unique start positions.  The X axis is the number of reads sequenced and the Y axis is the number of unique start positions.

Developed by: Michele Busby, Computational Biologist
Broad Technology Labs/MBRD
Broad Institute 
10/23/2013


Why would I want to do this?

This is used mainly as a quality metric to detect libraries with complexity problems arising from too little input or libraries that have been over-sequenced. 

It may be sometimes informative for very problematic genomic libraries but when working with DNA data you usually just remove duplicates.  You cannot really do this for RNA libraries because highly expressed RNAs may have true duplicate reads.  However, if libraries are grossly oversequenced duplicates can wreak havoc on downstream statistics because sampling noise gets magnified.  One solution I have tried is to randomly downsampled bam files to a read depth where samples are still unsaturated.  This actually improves some statistics like correlation between replicates.  But if it is very bad you may need to resequence the libraries.

The output is two files:

stub_counts.txt - each start position and the number of times it showed up.  Chromosomes are named by the order they showed up.  You probably don't need this file unless you want to draw distributions or something.

stub_rarefactionData.txt - This file has the number of reads sequenced and how many unique reads there are.

Use this file to make a rarefaction plot where the number of reads sequenced is the x axis and the number of unique reads is the y axis.  For high complexity libraries that are not sequenced to saturation this will form a fairly straight line.  For oversequenced low complexity libraries the line will asymptote at the point where you stop adding new information when you add more reads (i.e. everything is just a PCR duplicate of what you already have -or- you have a very, very small transcriptome sequenced very, very deeply).

You have to make the rarefaction plot yourself.  This is easy and can be done in Excel, R, or Matlab. The easiest way is to open your rarefactionData.txt file in Excel and make a scatter plot with connected dots.

The file GoodVsBadLibraries.png shows an example a plot that combines the outputs of three rarefactionData.txt files in one plot.  You can see that the bad libraries are much less complex and ran out of information after fewer than a million reads. 

*Useage: 
 * ./ComplexityByStartPos -bam /FolderWhereAlignmentIs/myAlignment.bam -out ./output -points 30
 
 -bam Name of the bam file.  Only works for paired end.  I could fix this to do single end. email me if you need it.

-out Name of a stub to be used for the output (e.g. /myFiles/thisSample )

-points How many points you want to sample.  Default is 20.  If you want it to go faster use 10.  If you want a really smooth line use 50. If you are really neurotic use 100.

The program works by creating a hash map into memory.  It might segment fault if you have more read positions than memory available so make sure you give it enough memory.


How it works:
1)	It makes a hash map of the reads by aligned position (chr_Start1_strand-chr_Start2_strand ) and the count of how many times each read shows up.  I don�t look at the sequence of the read.

2)	Iterates through map and prints the read and the number of times it showed up

3) Does a downsampling at 20 points using 20 binomial probabilities.  This provides a virtual down sampling. 
(i.e. to get the unique reads at 10% you would take each gene and downsample the count by a binomial probability of 0.1)
Ignores whether they map uniquely or multiply so if you only want unique you have to filter your bam file first.

4)	Prints out the rarefaction data file.


Installation:
The file ComplexityByStartPos is a 64 bit binary.  This is easiest if it works on your server.  Otherwise you will need to compile it.

How to get it to compile:

Compiling the file may or may not be a big hassle because you need both Boost and bamtools.

First, though, copy all the files in here to a single directory on your unix server.  The server should have g++ is installed (it probably is).

Boost

You need the Boost C++ library to throw a binomial random number. If it is not already installed try downloading it here: http://www.boost.org/

I used Boost 1.54.0.

You need to install it and add it to your path so that g++ can find it.  I did not have to do this so I cannot offer any guidance.
If you happen to have use of a Broad server you can get it to use Boost by typing: use .boost-1.54.0
(Yes, we are very spoiled).
If you don't know if Boost is installed skip to the next section and see if it breaks.  It is fairly common so it may already be in the install.

Bamtools

To get this to compile you will need to download Derek Barnett's bamtools API and install it in a folder somewhere.
It is here: https://github.com/pezmaster31/bamtools
You need the API, not the command line program though is quite useful to have.
Then, after you install it, you need to edit the Makefile in this folder so that everywhere it says "FolderWhereBamToolsIs" you put in the folder where bamtools is located.

Compiling

Go to the folder where everything is installed in type "make".  Ignore the warnings about Handy doing stupid stuff.
If nothing says ERROR and it makes an executable called ComplexityByStartPos you should be all set.

Finding Saturation Point

It would be nice to be able to find the asymptope of the curve programatically to determine where the saturation point is.  I have tried to fit several equations but while Michaelis-Menten looks OK for some datasets it is not great for others.  Zipf's does not work.  I was expecting the underlying distribution to be Poisson-lognormal but it is not.  Any suggestions of other equations to try would be appreciated.
