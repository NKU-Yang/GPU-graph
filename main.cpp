#include"Graph.h"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/functional.h>
#include <thrust/sort.h>

#include <thread>
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

void bfsCPU(int start, Graph<uint> &G, std::vector<int> &distance,
            std::vector<uint> &parent, std::vector<bool> &visited) {
    distance[start] = 0;
    parent[start] = start;
    visited[start] = true;
    std::queue<int> Q;
    Q.push(start);

    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();
		
		for(ull i=0; i < G.nodePointers[u+1]-G.nodePointers[u]; i++)
		{
            int index=G.nodePointers[u]+i;
			uint v = G.edgeArray[i];
            if(!visited[v]){
                visited[v]=true;
                distance[v]=distance[u]+1;
                parent[v]=i;
                Q.push(v);
            }
		}
    }
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

void runCPUbfs(int start, Graph<uint> &G, std::vector<int> &distance,
            std::vector<uint> &parent, std::vector<bool> &visited)
{
   
    //check(G);
    auto startTime = chrono::steady_clock::now();
    bfsCPU(2048,G,distance,parent,visited);
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
    cout << "bfsCPU: " << duration << "ms" << endl;
    int num=0;
	for(int i=0;i<distance.size();i++)
    {
        if(distance[i]!=-1)
        {
            num++;
        }

    }
    cout<<"CPU visit "<<num<<" nodes"<<endl;
}
int main()
{
    Graph<uint> G;
    string filename="./dataset/twitter-2010.bcsr";
    //string test = "/home/lsy/data/twitter-2010/twitter-2010.bcsr";
    G.readDataFromFile(filename);
    vector<int>distance(G.vertexArrSize+1,-1);
    vector<uint>parent(G.vertexArrSize+1);
    vector<bool>visited(G.vertexArrSize+1,false);
    runCPUbfs(2048,G,distance,parent,visited);

    //vector<int>D_distance(G.vertexArrSize+1,-1);
    //vector<bool>D_visited(G.vertexArrSize+1,false);
    uint* D_distance = new uint[G.vertexArrSize];
    bool* D_visited = new bool[G.vertexArrSize];
    for(int i=0;i<G.vertexArrSize;i++)
    {
        D_distance[i]=-1;
        D_visited[i]=false;
    }
    runGPUbfs(G,2048,D_distance,D_visited);

    return 0;
}


