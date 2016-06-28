// Name: Hanif Mirza
// File Name: apriori.cpp
// Date: 14 April, 2016
// Program Description: This program will generate all large itemsets using the Apriori algorithm.


#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdlib.h>

using namespace std;

double		MIN_SUP;	// a minimum support threshold provided from the command line
int	   		TOTAL_TRAN; // total number of transactions

vector<string> 		split(string str, char delimiter);
void		   		readFile( vector<string>  &candidates,vector<vector<string> > &transactions,string fileName);
bool		   		checkMinmumSupport(string candidate, vector<vector<string> > transactions);
void 				displayLargeItemSet(vector<string>  largeItemSet,int n);
string 				removeDuplicates(string str);
bool				doPruneStep(string str,vector<string>  largeItemSet);
void				findAllLargeItemsets( vector<string>  urCandidates, vector<vector<string> > tran);


int	main(int argc, char* argv[])
{
    vector<vector<string> > 	transactions;	// vector that contains all the transactions
	vector<string>				candidates;		// vector represents all the items

	if (argc < 4)
	{
		cout<<"Please enter file name and minimum support, e.g. ./apriori data.txt -s .2 "<<endl;
		exit(1);
	}

	string fileName = argv[1];
	string minSup = argv[3];
	MIN_SUP = atof(minSup.c_str());

	readFile(candidates,transactions,fileName); // read the content of a sample data file
	TOTAL_TRAN = transactions.size();	// total number of transactions will be the size of the vector
	cout<<endl;
	findAllLargeItemsets(candidates,transactions);	// finding all large itemsets

	return 0;
}

// This function will find all large itemsets
void  findAllLargeItemsets(vector<string>  urCandidates, vector<vector<string> >  tran)
{
	vector<string>				candidateKeys;
	vector<string>				largeItemSet;
	vector<string>				candidates = urCandidates;
	vector<vector<string> > 	transactions = tran;

	for (int i = 1; i<= candidates.size() ; i++ )
	{
		if ( i == 1 )
		{
			candidateKeys = candidates; // the initial candidate itemsets will be all items when i = 1
			for(int x=0; x<candidateKeys.size(); x++)
			{

				if (checkMinmumSupport(candidateKeys[x],transactions) )
				{
					// add each candidate item to large item set, if it's support >= MIN_SUP
					largeItemSet.push_back( candidateKeys[x] );
				}
			}
			displayLargeItemSet(largeItemSet,i); // display the large item set
			candidateKeys.clear(); // clear the candidate itemsets
		}

		else if (i > 1)
		{
			for (int j = 0; j< largeItemSet.size(); j++ )
			{
				for(int k = j+1; k< largeItemSet.size(); k++ )
				{
					string canKey = largeItemSet[j] + largeItemSet[k]; // joining two itemsets into a new candidate item
					canKey = removeDuplicates(canKey);	// removing all duplicate character in new candidate item

					/* after removing all duplicate, if new candidate item's length is i
					   then it's  previous  two itemsets agree on i-2 items */
					if(canKey.length() == i )
					{
						// add the new candidate item to candidateKeys vector , only if the vector doesn't contain that new candidate
						if( find(candidateKeys.begin(), candidateKeys.end(), canKey) == candidateKeys.end() )
						{
							if ( doPruneStep(canKey,largeItemSet) )
							{
								// if the new candidate item passes the Prune step then add it to the candidateKeys vector
								candidateKeys.push_back(canKey);
							}
						}
					}

				}
			}

			largeItemSet.clear(); // clear the candidate itemsets
			for(int x=0; x<candidateKeys.size(); x++)
			{
				if (checkMinmumSupport(candidateKeys[x],transactions) )
				{
					// add each new candidate item to large item set, if it's support >= MIN_SUP
					largeItemSet.push_back( candidateKeys[x] );
				}
			}
			displayLargeItemSet(largeItemSet,i); // display the large item set
			candidateKeys.clear(); // clear the candidate itemsets
		}

	}
}

// this function will do the Pruning step, it will return true if the new candidate item passes the Prune step
bool doPruneStep(string str,vector<string>  largeItemSet)
{
	vector<string>  subset;

	// creating (i -1) element subset of the new candidate item
    for (int pos = 0; pos < str.length(); pos++)
    {
		if (pos == 0 )
		{
			subset.push_back( str.substr(pos+1,str.length()) );
		}
		else if (pos == str.length()-1)
		{
			subset.push_back( str.substr(0,str.length()-1) );
		}
		else
		{
			string start = str.substr(0,pos);
			string end = str.substr(pos+1,str.length());
			subset.push_back( start+end );
		}
    }
    for(int x=0; x<subset.size(); x++)
    {
		// return false if any element of the subset doesn't contain in the large itemset vector
		if( find(largeItemSet.begin(), largeItemSet.end(), subset[x]) == largeItemSet.end() )
		{
			return false;
		}
	}
	return true;
}

// This function will remove all duplicate characters in a string
string removeDuplicates(string str)
{
	std::sort(str.begin(), str.end()); // sort the string
	str.erase(std::unique(str.begin(), str.end()), str.end()); // remove all duplicate characters
	return str;
}

// This function will display the items in a large item set
void displayLargeItemSet(vector<string>  largeItemSet,int n)
{
	cout<<"Large "<< n <<" -itemsets: ";;

	for(int x=0; x<largeItemSet.size(); x++)
	{
		cout<<largeItemSet[x]<<" ";
	}
	cout<<endl;
}

// This function will read the content of a sample data file
void readFile(vector<string>  &candidates, vector< vector<string> > 	&transactions,string fileName)
{
    ifstream 		inFile;
    string 			eachLine;
    inFile.open(fileName.c_str());
	if (!inFile)
	{
  		cout << "Can't open input file "<< endl;
	}

    getline(inFile, eachLine,'\r'); // the firt line contains all the items
    inFile.ignore(256,'\n');
	candidates = split(eachLine, ' '); // store all the items in candidates vector

    while (getline(inFile, eachLine,'\r'))
    {
		inFile.ignore(256,'\n');
        vector<string> eachTransaction = split(eachLine, ' ');
        transactions.push_back(eachTransaction);	// store each transaction (individual) vector into a transactions vector
    }
}

// This function will return true if new candidate item's support >= MIN_SUP
bool checkMinmumSupport(string candidate, vector<vector<string> > transactions)
{
	int  matchCount = 0;
	bool match =false;

	for(int x=0; x<transactions.size(); x++)
	{
		// get each transaction (individual) vector from the all transactions vector
	    vector<string> eachTran = transactions[x];

	    for (int m = 0; m < candidate.length(); m++)
    	{
			string alphabet = string(1,candidate[m]); //get each alphabet from the candidate item
			for(int y=0; y<eachTran.size(); y++)
			{
				if ( eachTran[y] == alphabet )
				{
					match = true; // alphabet matches with an element of each transaction vector
					y = eachTran.size()+ 2; // matches found so exit the 3rd nested for loop
				}
				else
				{
					match = false; // alphabet doesn't match with an element of each transaction vector
				}
			}
			if ( match == false ) // alphabet didn't match with any element of each transaction vector
			{
				m = candidate.length() + 2; // alphabet didn't match so exit the 2nd nested for loop
			}
		}

		if ( match == true )
		{
			matchCount++; // increment the match count if the candidate item matches with a transaction (individual)
		}
    }
    double support = matchCount/(double)TOTAL_TRAN;

    if ( support >= MIN_SUP)
    {
		return true; // return true if new candidate item's support >= MIN_SUP
	}
	else
	{
		return false;
	}
}

// This function will split a string with a delimiter (white spaces)
vector<string> split(string str, char delimiter)
{
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while(getline(ss, tok, delimiter))
	{
		internal.push_back(tok); // add each token of the string to a vector
	}
	return internal;
}
