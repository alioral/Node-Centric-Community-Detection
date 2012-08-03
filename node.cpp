#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <windows.h>
#include <fstream>

//No weight is required!

using namespace std;

void removeNode(int **arr,int numberOfNodes,int nodeIndex)
{
    for(int i=0;i<numberOfNodes;i++){
        arr[nodeIndex][i]=0;
        arr[i][nodeIndex]=0;
    }
}
bool isCliqueGiven(int **arr,int numberOfNodes,vector<int>numbers)
{
    for(int i=0;i<numbers.size();i++)
        for(int j=i+1;j<numbers.size();j++)
            if(arr[numbers[i]][numbers[j]]<1)
                return false;
    return true;
}
bool isDuplicate(vector< vector<int> >listOfCliques,vector<int>nomineeClique)
{
    for(int i=0;i<listOfCliques.size();i++)
    {
        if(listOfCliques[i].size()==nomineeClique.size() &&
           std::equal(listOfCliques[i].begin(),listOfCliques[i].begin()+nomineeClique.size(),nomineeClique.begin()))
                return true;
    }
    return false;
}

bool verifyClique(int **arr,int numberOfNodes,vector<int>setOfCliques,int nodeToVerify)
{
    bool isConnectedToAll=false;
    for(int i=0;i<setOfCliques.size();i++){
        if(arr[setOfCliques[i]][nodeToVerify]>0)
            isConnectedToAll=true;
        else
        {
            isConnectedToAll=false;
            break;
        }
    }
    return isConnectedToAll;
}
void printArr(int **arr,int numberOfNodes)
{
    int i,j;
    for(i=0;i<numberOfNodes;i++)
    {
        for(j=0;j<numberOfNodes;j++)
            cout<<arr[i][j]<<" ";
        cout<<endl;
    }
}
void printVec(vector<int> vec)
{
    for(int i=0;i<vec.size();i++)
        cout<<vec[i]+1<<" ";
}
vector<int>getNonZeroDegrees(int **arr,int numberOfNodes)
{
    vector<int>nodesToReturn;
    for(int i=0;i<numberOfNodes;i++)
        for(int j=0;j<numberOfNodes;j++)
        {
            if(arr[i][j]>0){
                nodesToReturn.push_back(i);
                break;
            }
        }
    return nodesToReturn;
}
int getDegree(int **arr,int numberOfNodes,int nodeIndex)
{
    int count=0,i;

    for(i=0;i<numberOfNodes;i++)
        if(arr[nodeIndex][i]>0)
            count++;

    return count;
}
bool isNodeLeftToBePruned(int **arr,int numberOfNodes,int limit)
{
    for(int i=0;i<numberOfNodes;i++)
        if(getDegree(arr,numberOfNodes,i)>=limit)
            return true;
    return false;
}
vector<int>getNeighbours(int **arr,int numberOfNodes,int node)
{
    vector<int>neighbours;
    for(int i=0;i<numberOfNodes;i++)
        if(arr[node][i]>0)
            neighbours.push_back(i);
    return neighbours;
}

void pruneWithLimit(int **arr,int numberOfNodes,int limit)
{
    int degreeCount=0;
    vector<int>nodesToBeDeleted;
    for(int i=0;i<numberOfNodes;i++)
    {
        degreeCount=getDegree(arr,numberOfNodes,i);
        if(degreeCount>0 && degreeCount<limit)
            nodesToBeDeleted.push_back(i);
    }
    while(nodesToBeDeleted.size()>0)
    {
        for(int i=0;i<nodesToBeDeleted.size();i++)
        {
            cout<<"Pruning: "<<nodesToBeDeleted[i]+1<<endl;
            removeNode(arr,numberOfNodes,nodesToBeDeleted[i]);
        }
        nodesToBeDeleted.clear();
        for(int i=0;i<numberOfNodes;i++)
        {
            degreeCount=getDegree(arr,numberOfNodes,i);
            if(degreeCount>0 && degreeCount<limit)
                nodesToBeDeleted.push_back(i);
        }
    }
}
int **generatedSubgraph(int **arr,int numberOfNodes,int base,int limit)
{
    int numNodes=limit-base+1;
    int **newGraph=new int*[numNodes];

    for(int i=0;i<numNodes;i++) //Malloc and putting the values inside at the same time.
    {
        newGraph[i]=new int[numNodes];
        for(int j=0;j<numNodes;j++)
            newGraph[i][j]=arr[i+base][j+base];
    }
    return newGraph;
}

vector< vector<int> >findAllCliques(int **graph,int numberOfNodes)
{
    //Randomly select a node which will be our initial 1-sized clique
    int randomNode,generalMaxSize=0;
    vector<int>temp,temp2,temp3;
    vector< vector<int> >setOfCliques,solutionSet;
    for(int i=0;i<numberOfNodes;i++){ //Brute-force: Traverse for each node in network
        srand(time(0));
        randomNode=i;
        cout<<"First randomly selected node is Node #"<<randomNode+1<<endl;
        temp=getNeighbours(graph,numberOfNodes,randomNode) ;

        for(int i=0;i<temp.size();i++)
        {
            temp2.push_back(randomNode);
            temp2.push_back(temp[i]);
            setOfCliques.push_back(temp2);
            temp2.clear();
        }

        for(int i=0;i<setOfCliques.size();i++)
        {
            cout<<"{";
            printVec(setOfCliques[i]);
            cout<<"}"<<endl;
        }
        int sizeOfCliqueSet,currentSizeOfCliqueSet,tempSize,maxSize=1,x=0;
        cout<<"Checking the last elements"<<endl;
        cout<<"--------------------------"<<endl;
        do
        {
            sizeOfCliqueSet=setOfCliques.size();
            if(sizeOfCliqueSet<1)//if no neighbour found, nothing to process so break;
                break;
            cout<<"Old size:"<<sizeOfCliqueSet<<endl;
            for(int i=0;i<sizeOfCliqueSet;i++)
            {
                cout<<"Our element is:";
                printVec(setOfCliques[i]);
                cout<<endl;
                temp2=getNeighbours(graph,numberOfNodes,setOfCliques[i][setOfCliques[i].size()-1]);
                cout<<"Neighbours of the last element:";
                printVec(temp2);
                cout<<endl;

                for(int j=0;j<temp2.size();j++)
                {
                    if(std::find(setOfCliques[i].begin(),setOfCliques[i].end(), temp2[j]) == setOfCliques[i].end())
                    {
                        cout<<"A good candidate is Node #"<<temp2[j]+1<<endl;
                        if(verifyClique(graph,numberOfNodes,setOfCliques[i],temp2[j])==true)
                        {
                            temp3=setOfCliques[i];
                            temp3.push_back(temp2[j]);
                            sort(temp3.begin(),temp3.end());
                            if(isDuplicate(setOfCliques,temp3)==false)
                            {
                                maxSize=temp3.size();
                                if(maxSize>generalMaxSize)
                                    generalMaxSize=maxSize;
                                cout<<"Pushing back: ";
                                printVec(temp3);
                                cout<<endl;

                                setOfCliques.push_back(temp3);
                                temp3.clear();
                            }
                        }
                    }
                }
                temp2.clear();
                currentSizeOfCliqueSet=setOfCliques.size();
            }
        }while(sizeOfCliqueSet<currentSizeOfCliqueSet);
        cout<<"Printing with max elements"<<endl;
        for(int j=0;j<setOfCliques.size();j++)
        {
            if(setOfCliques[j].size()>=maxSize){
                printVec(setOfCliques[j]);
                if(isDuplicate(solutionSet,setOfCliques[j])==false)
                    solutionSet.push_back(setOfCliques[j]);
                cout<<endl;
            }
        }
        setOfCliques.clear();
    }
    cout<<"The biggest clique size is: "<<generalMaxSize<<"!"<<endl;
    
    return solutionSet;
}

vector< vector<int> >findMaximalCliques(int **graph,int numberOfNodes)
{
    //Randomly select a node which will be our initial 1-sized clique
    int randomNode,generalMaxSize=0;
    vector<int>temp,temp2,temp3;
    vector< vector<int> >setOfCliques,solutionSet;
    for(int i=0;i<numberOfNodes;i++){ //Brute-force: Traverse for each node in network
        srand(time(0));
        randomNode=i;
        temp=getNeighbours(graph,numberOfNodes,randomNode) ;

        for(int i=0;i<temp.size();i++)
        {
            temp2.push_back(randomNode);
            temp2.push_back(temp[i]);
            setOfCliques.push_back(temp2);
            temp2.clear();
        }

        int sizeOfCliqueSet,currentSizeOfCliqueSet,tempSize,maxSize=1,x=0;
        do
        {
            sizeOfCliqueSet=setOfCliques.size();
            if(sizeOfCliqueSet<1)//if no neighbour found, nothing to process so break;
                break;
            for(int i=0;i<sizeOfCliqueSet;i++)
            {
                temp2=getNeighbours(graph,numberOfNodes,setOfCliques[i][setOfCliques[i].size()-1]);
                bool okToDelete=false;

                for(int j=0;j<temp2.size();j++)
                {
                    if(std::find(setOfCliques[i].begin(),setOfCliques[i].end(), temp2[j]) == setOfCliques[i].end())
                    {
                        if(verifyClique(graph,numberOfNodes,setOfCliques[i],temp2[j])==true)
                        {
                            temp3=setOfCliques[i];
                            temp3.push_back(temp2[j]);
                            sort(temp3.begin(),temp3.end());
                            if(isDuplicate(setOfCliques,temp3)==false)
                            {
                                maxSize=temp3.size();
                                if(maxSize>generalMaxSize)
                                    generalMaxSize=maxSize;

                                setOfCliques.push_back(temp3);
                                temp3.clear();
                                okToDelete=true;
                            }
                        }
                    }
                }
                temp2.clear();
                currentSizeOfCliqueSet=setOfCliques.size();
            }
        }while(sizeOfCliqueSet<currentSizeOfCliqueSet);
        for(int j=0;j<setOfCliques.size();j++)
        {
            if(setOfCliques[j].size()>=maxSize){
                if(isDuplicate(solutionSet,setOfCliques[j])==false)
                    solutionSet.push_back(setOfCliques[j]);
            }
        }
        setOfCliques.clear();
    }
    vector< vector<int> >realSolutionSet;
    for(int i=0;i<solutionSet.size();i++)
        if(solutionSet[i].size()>=generalMaxSize)
            realSolutionSet.push_back(solutionSet[i]);

    return realSolutionSet;
}

vector< vector<int> > optimumCliqueSearch(int **graph,int numberOfNodes)
{

    vector < vector<int> >solutionSet;
    vector<int>chooseFrom;
    //Step 1: Create a random subgraph
    //Step 2: Derive the maximal cliques size which will be the lower boundry
    //Step 3: Prune connections with degree less than k-1
    int lower,upper,limitOfPruning=1;
    //I set the limitOfPruning 1, taking into consideration that there must be at least
    //one-sized clique, which being the node itself. Or putting it in another way in the
    //network in order for us to search cliques there must be at least one node with at least
    //one connection.
    cout<<"Number of nodes: "<<numberOfNodes<<endl;
    lower=0;
    upper=numberOfNodes/2;

    cout<<"Valid Lower: "<<lower<<endl;
    cout<<"Valid Upper: "<<upper<<endl;
    
    int cliqueBoundry;
    do
    {
        cout<<"Prune limit is: "<<limitOfPruning<<endl;
        cout<<"Subgraph will be generated from Node #"<<lower<<" and Node #"<<upper<<endl;
        int **subgraph=generatedSubgraph(graph,numberOfNodes,lower,upper);
        cliqueBoundry=findMaximalCliques(subgraph,upper-lower+1)[0].size();
        //The reason for findMaximalCliques(subgraph,upper-lower+1)[0].size();
        //findMaximalClique is a function that returns ONLY the maximal cliques of that graph
        //For instance if we have {1,3,4} and {1,2,3} it will return them in a vector< vector<int> >
        //Since we need the size of that maximal clique I've thought that the first elements
        //of the findMaximalCliques() will give us the result. There is no possible way for this
        //function to return empty since at least there is one-sized cliques inside.

        cliqueBoundry--;

        cout<<"Clique boundry is :"<<cliqueBoundry<<endl;

        if(limitOfPruning<cliqueBoundry)
            limitOfPruning=cliqueBoundry;
        pruneWithLimit(graph,numberOfNodes,limitOfPruning);

        int lowerIndex,upperIndex;
        chooseFrom=getNonZeroDegrees(graph,numberOfNodes);
        if(isCliqueGiven(graph,numberOfNodes,chooseFrom))
            break;
        if(chooseFrom.size()<2) //If nodes are eliminated
            break;

        lowerIndex=rand()%(chooseFrom.size()-1);
        upperIndex=rand()%chooseFrom.size();

        for(int t=0;upperIndex<=lowerIndex && t<100;t++) //Reason for 100 is if it cannot find
        {                                               //anything in 100 steps, terminate the program
            upperIndex=rand()%chooseFrom.size();
        }


        lower=chooseFrom[lowerIndex];
        upper=chooseFrom[upperIndex];


    }while(isNodeLeftToBePruned(graph,numberOfNodes,limitOfPruning));

    solutionSet=findMaximalCliques(graph,numberOfNodes);
    return solutionSet;
}



int main()
{
    srand(time(0));
    vector< vector<int> >solutionSet;
    ifstream file("graphFromBook.txt"); //In my case I've used graphFromBook.txt including only the connections between edges
    string word="",data="";				//Without the weight information. And first line MUST include the number of nodes in graph!
    getline(file,word);
    int node1,node2,weight,arrTemp[3];
    int numberOfNodes=atoi(word.c_str());
    int **graph=new int*[numberOfNodes];

    for(int i=0;i<numberOfNodes;i++)
        graph[i]=new int[numberOfNodes];

    for(int i=0;i<numberOfNodes;i++)
        for(int j=0;j<numberOfNodes;j++)
            graph[i][j]=0;

    while(file.good() && getline(file,word))
    {
           if(word.size()>0)
           {
               for(int i=0;i<3;i++)
               {
                   int cutFrom=word.find_first_of(' ');
                   arrTemp[i]=atoi(word.substr(0,cutFrom).c_str());
                   word=word.substr(cutFrom+1,word.length());
               }
               graph[arrTemp[0]-1][arrTemp[1]-1]=1;
               graph[arrTemp[1]-1][arrTemp[0]-1]=graph[arrTemp[0]-1][arrTemp[1]-1];
           }
           else
                break;
    }

    solutionSet=optimumCliqueSearch(graph,numberOfNodes);

    cout<<"The Solution Set"<<endl;
    cout<<"---------------------------------"<<endl;
    for(int i=0;i<solutionSet.size();i++)
    {
        cout<<"{ ";
        printVec(solutionSet[i]);
        cout<<" }"<<endl;
    }
    return 0;
}