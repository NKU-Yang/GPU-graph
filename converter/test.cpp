#include "./globals.hpp"
#include <cstdlib>
#include <ctime>
bool IsWeightedFormat(string format)
{
	if((format == "bwcsr")	||
		(format == "wcsr")	||
		(format == "wel"))
			return true;
	return false;
}

string GetFileExtension(string fileName)
{
    if(fileName.find_last_of(".") != string::npos)
        return fileName.substr(fileName.find_last_of(".")+1);
    return "";
}

void convertTxTtoByte(string input)
{
    cout<<"convert txt to Byte"<<endl;
    clock_t startTime, endTime;
	startTime = clock();
	ifstream infile;
    infile.open(input);
    std::ofstream outfile(input.substr(0, input.length()-3)+"B", std::ofstream::binary);
    stringstream ss;
    uint max = 0;
    string line;
    ull edgeCounter = 0;
    vector<Edge> edges;
    Edge newEdge;
    ull testIndex = 0;
    while(getline(infile, line))
    {
        testIndex++;
        ss.str("");
        ss.clear();
        ss << line;
        ss >> newEdge.source;
        ss >> newEdge.end;
        if(newEdge.source==1)
        cout << newEdge.source << "  " << newEdge.end << endl;
        edges.push_back(newEdge);
        edgeCounter++;
        if(max < newEdge.source)
            max = newEdge.source;
        if(max < newEdge.end)
            max = newEdge.end;
        if (testIndex % 1000000000 == 1)
    	{
    		int billionLines = testIndex / 1000000000;
    		cout << billionLines << " billion lines " << endl;
    		if (billionLines % 5 == 1)
    		{
    			outfile.write((char*)edges.data(), sizeof(Edge) * edges.size());
    			edges.clear();
    			cout << "clear edges = " << edges.size() << endl;
    		}
    	}
    }
    if (edges.size() > 0)
	{
		outfile.write((char*)edges.data(), sizeof(Edge) * edges.size());
		edges.clear();
	}
    cout << "write " << testIndex << " lines" << endl;
	cout << "max node " << max << endl;
    outfile.close();
    infile.close();
    endTime = clock();
    cout << setprecision(9)  << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s | " << "S0:Prepare [edges] OK" << endl;
}

void converByteToCSR(string input)
{
    cout << "convertByteToBCSR" << endl;
	clock_t startTime, endTime;
	startTime = clock();
	ull totalSize = 10000000000;
	uint partSize = 1000000000;
	uint vertexSize = 100000000;
	ull* nodePointers = new ull[vertexSize];
	ull* nodePointersAnkor = new ull[vertexSize];
	ull* degree = new ull[vertexSize];
	OutEdge* edges = new OutEdge[totalSize];
	Edge *byteEdges = new Edge[partSize];
	ull counter = 0;
	for(uint i = 0; i < vertexSize; i++)
	 	degree[i] = 0;
    ifstream infile(input, ios::in | ios::binary);
	infile.seekg(0, std::ios::end);
	ull size = infile.tellg();
    totalSize = size / sizeof(Edge);
    cout << "total edge Size " << totalSize << endl;
    infile.clear();
	infile.seekg(0);
    uint maxNode;

    //比较小的数据集
    if(totalSize<partSize)
    {
        infile.read((char*)byteEdges, sizeof(Edge)*totalSize);
        for(uint i=0;i<totalSize;i++)
        {
            degree[byteEdges[i].source]++;
			if (maxNode < byteEdges[i].source)
			{
				maxNode = byteEdges[i].source;
			}
            if(maxNode < byteEdges[i].end)
            {
                maxNode = byteEdges[i].end;
            }
        }
    }
    else{//大数据集
    while(counter < totalSize) {
		cout << counter << " to " << counter + partSize << endl;
		counter += partSize;
		infile.read((char *) byteEdges, sizeof(Edge) * partSize);
		    for (uint i = 0; i < partSize; ++i)
		    {
			    degree[byteEdges[i].source]++;
			    if (maxNode < byteEdges[i].source)
			    {
				    maxNode = byteEdges[i].source;
			    }
                if(maxNode < byteEdges[i].end)
                {
                    maxNode = byteEdges[i].end;
                }
		    }
	    }
    }

    vertexSize = maxNode + 1;
	ull tempPointer = 0;
	for(uint i=0; i<vertexSize; i++)
	{
		nodePointers[i] = tempPointer;
		tempPointer += degree[i];
	}

    
}

int main(int argc, char** argv)
{
    if(argc!= 2)
	{
		cout << "\nThere was an error parsing command line arguments\n";
		exit(0);
	}

	string input = string(argv[1]);
    convertTxTtoByte(input);
    string binput=input.substr(0, input.length()-3);
    binput+="B";
    converByteToCSR(binput);
    return 0;
}