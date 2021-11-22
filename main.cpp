#include"Graph.h"

//#include <cuda_runtime.h>
//#include <device_launch_parameters.h>
//#include <thrust/host_vector.h>
//#include <thrust/device_vector.h>
//#include <thrust/functional.h>
//#include <thrust/sort.h>

#include <thread>
/*
#define gpuErrorcheck(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true) {
    if (code != cudaSuccess) {
        fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
        if (abort) exit(code);
    }
}

inline void checkError(std::string msg) {
    cudaError_t error = cudaGetLastError();
    printf("%s %s\n",msg.c_str(), cudaGetErrorString(error));
}


void runGPUbfs(Graph<uint>G, uint source, uint* distance, bool* visited)
{
    auto startTime = chrono::steady_clock::now();
    BFS_CUDA(G,source,distance,visited);
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
    cout << "bfsGPU: " << duration << "ms" << endl;
    int num=0;
	for(int i=0;i<G.vertexArrSize;i++)
    {
        if(distance[i]!=-1)
        {
            num++;
        }

    }
    cout<<"GPU visit "<<num<<" nodes"<<endl;
}
*/
#define MYINFINITY 0xFFFFFFFF

void checknode(ull node,Graph<uint> &G)
{
    ofstream outfile("check.out",ios::out);
    cout<<"check node "<<node<<endl;
    ull nodepointer = G.nodePointers[node];
    printf("nodePointer: %lu\n",nodepointer);
    ull degree = G.nodePointers[node+1]-nodepointer;
    printf("degree: %lu\n", degree);
    cout<<"edges: ";
    for(uint i=0;i<degree;i++)
    {
        outfile<<G.edgeArray[i+nodepointer]<<endl;
    }
    outfile.close();
    cout<<endl;

}
void bfsCPU(int start, Graph<uint> &G, std::vector<uint> &distance,
            std::vector<uint> &parent, std::vector<bool> &visited) {
    uint count=1;
    distance[start] = 0;
    parent[start] = start;
    visited[start] = true;
    std::queue<int> Q;
    Q.push(start);
    uint iter=0;
    while (!Q.empty()) {
        iter++;
        ull u = Q.front();
        Q.pop();
		if(u==G.vertexArrSize-1){
            for(ull i=G.nodePointers[u];i<G.edgeArrSize;i++)
            {
                uint v = G.edgeArray[i];
                if(!visited[v]){
                visited[v]=true;
                distance[v]=distance[u]+1;
                parent[v]=i;
                Q.push(v);
                count++; 
            }
            }
        }
        else{
		for(ull i=0; i < G.nodePointers[u+1]-G.nodePointers[u]; i++)
		{
            ull index=G.nodePointers[u]+i;
			uint v = G.edgeArray[index];
            if(!visited[v]){
                visited[v]=true;
                distance[v]=distance[u]+1;
                parent[v]=i;
                Q.push(v);
                count++;
            }
		}
        }
    }
    cout<<"cpu visit "<<count<<" nodes "<<iter<<" iters"<<endl;
}
void check(Graph<uint>& G)
{
    bool flag = true;
    for(int i=0;i<100;i++)
    {
        ull index = G.nodePointers[i];
        cout<<i<<": "<<index;
        /*if(index!=0)
        {
            uint edge = G.edgeArray[index];
            cout<<"edge: "<<edge;
        }*/
        cout<<endl;
    }
    
}

void runCPUbfs(int start, Graph<uint> &G, std::vector<uint> &distance,
            std::vector<uint> &parent, std::vector<bool> &visited)
{
    int degree = G.nodePointers[start+1]-G.nodePointers[start];
    cout<<"the drgree of sourceNode: "<<degree<<endl;
    
    
    auto startTime = chrono::steady_clock::now();
    bfsCPU(start,G,distance,parent,visited);
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
    cout << "bfsCPU: " << duration << "ms" << endl;
    uint num=0;
	for(uint i=0;i<distance.size();i++)
    {
        if(distance[i]!=MYINFINITY)
        {
            num++;
        }

    }
    cout<<"CPU visit "<<num<<" nodes"<<endl;
}
int main()
{
    Graph<uint> G;
    cout<<"input the data:"<<endl;
    string dataname;
    cin>>dataname;
    string filename="/home/lsy/data/"+dataname+"/"+dataname+".bcsr";
    
    //string test = "/home/lsy/data/twitter-2010/twitter-2010.bcsr";
    G.readDataFromFile(filename);
    checknode(229408,G);
    //ull bignode=G.findbignode();
    //G.printself();
    //getchar();
    /*
    vector<uint>distance(G.vertexArrSize+1,MYINFINITY);
    vector<uint>parent(G.vertexArrSize+1);
    vector<bool>visited(G.vertexArrSize+1,false);
    int sourceNode=0;
    cout<<"input the sourceNode"<<endl;
    cin>>sourceNode;
    runCPUbfs(sourceNode,G,distance,parent,visited);
    */
    //vector<int>D_distance(G.vertexArrSize+1,-1);
    //vector<bool>D_visited(G.vertexArrSize+1,false);
    //uint* D_distance = new uint[G.vertexArrSize];
    //bool* D_visited = new bool[G.vertexArrSize];
    //for(int i=0;i<G.vertexArrSize;i++)
    //{
       // D_distance[i]=-1;
       // D_visited[i]=false;
    //}
    //runGPUbfs(G,2048,D_distance,D_visited);
    
    return 0;
}


