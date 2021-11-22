#include"global.h"



template<class EdgeType>
class Graph{
    private:
    public:
        SIZE_TYPE sourcenode = 0;
    SIZE_TYPE vertexArrSize;
    EDGE_POINTER_TYPE edgeArrSize;
    EDGE_POINTER_TYPE *nodePointers;
    EdgeType *edgeArray;
        void setSourceNode(uint s)
        {
            this->sourcenode=s;
        }
        void readDataFromFile(const string &filename);
        ~Graph();
        void printself(){
            cout<<"vertexArrSize:"<<this->vertexArrSize<<" "<<"edgeArrSize:"<<this->edgeArrSize<<endl;
            cout<<"nodePointers:"<<endl;
            for(int i=0;i<10;i++)
            {
                cout<<nodePointers[i]<<" ";
            }
            cout<<endl;
            cout<<"edgeArr:"<<endl;
            for(int i=0;i<10;i++)
            {
                cout<<edgeArray[i]<<" ";
            }
        }
        ull findbignode();
};
template<class EdgeType>
void Graph<EdgeType>::readDataFromFile(const string &fileName)
{
    cout << "readDataFromFile" << endl;
    auto startTime = chrono::steady_clock::now();
    ifstream infile(fileName, ios::in | ios::binary);
    infile.read((char *) &this->vertexArrSize, sizeof(EDGE_POINTER_TYPE));
    infile.read((char *) &this->edgeArrSize, sizeof(EDGE_POINTER_TYPE));
    cout << "vertex num: " << this->vertexArrSize << " edge num: " << this->edgeArrSize << endl;
    
    nodePointers = new EDGE_POINTER_TYPE[vertexArrSize];
    infile.read((char *) nodePointers, sizeof(EDGE_POINTER_TYPE) * vertexArrSize);
    edgeArray = new EdgeType[edgeArrSize];
    infile.read((char *) edgeArray, sizeof(EdgeType) * edgeArrSize);
    infile.close();
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
    cout << "readDataFromFile " << duration << " ms" << endl;
}

template<class EdgeType>
ull Graph<EdgeType>::findbignode()
{
    for(ull i=0;i<vertexArrSize-1;i++)
    {
        ull degree = nodePointers[i+1]-nodePointers[i];
        if(degree > 1000)
        {
            cout<<"find a big node:"<<i<<endl;
            return i;
        }
    }
    cout<<"failed to find a big node"<<endl;
    getchar();
}

template<class EdgeType>
Graph<EdgeType>::~Graph()
{
    delete[] nodePointers;
    delete[] edgeArray;
    cout << "~GraphMeta" << endl;
}