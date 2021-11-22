#include"Graph.h"

extern "C"{
__global__
void simpleBFS_CUDA(uint N, uint level, EDGE_POINTER_TYPE* nodePointers, uint* edgeArr, uint* distance, bool* visited){
    int thid = blockIdx.x * blockDim.x + threadIdx.x;
    if(thid<N && distance[thid]==level)
    {
        int u= thid;
        for(uint i=0;i<nodePointers[u+1]-nodePointers[u];i++)
        {
            uint v = edgeArr[u+i];
            if(visited[v]==false&&(distance[v]==-1||distance[v]>level+1))
            {
                distance[v]=level+1;
            }
        }
    }
}

void BFS_CUDA(Graph<uint> G, uint source, uint* distance, bool* visited)
    {
        EDGE_POINTER_TYPE *d_nodePointers;
        uint *d_edgeArr;
        uint nodeSize = G.vertexArrSize;
        uint edgeSize = G.edgeArrSize;
        uint* d_distacne;
        bool* d_visited;
        uint* d_cur_frointers;
        queue<uint> frointers;
        
        ///initial CUDA
        //malloc mem
        cudaMalloc(&d_nodePointers, sizeof(EDGE_POINTER_TYPE)*nodeSize);
        checkError("cudaMalloc d_nodePointers ");
        cudaMalloc(&d_edgeArr, sizeof(uint)*edgeSize);
        checkError("cudaMalloc d_edgeArr ");
        cudaMalloc(&d_distacne, sizeof(uint)*nodeSize);
        checkError("cudaMalloc d_distance ");
        cudaMalloc(&d_visited, sizeof(bool)*nodeSize);
        checkError("cudaMalloc d_visited ");
        cudaMalloc(&d_cur_frointers, sizeof(uint)*nodeSize);
        //copy init data
        gpuErrorcheck(cudaMemcpy(d_nodePointers, G.nodePointers, sizeof(EDGE_POINTER_TYPE)*nodeSize, cudaMemcpyHostToDevice));
        gpuErrorcheck(cudaMemcpy(d_edgeArr, G.edgeArray, sizeof(uint)*edgeSize, cudaMemcpyHostToDevice));
        gpuErrorcheck(cudaMemcpy(d_distacne, distance, sizeof(uint)*nodeSize, cudaMemcpyHostToDevice));
        gpuErrorcheck(cudaMemcpy(d_visited, visited, sizeof(bool)*nodeSize, cudaMemcpyHostToDevice));
        
        frointers.push(source);
        visited[source]=true;
        distance[source]=0;
        dim3 grid=(56,1,1);
        dim3 block=(1024,1,1);
        uint level=0;
        while(!frointers.empty())
        {
            uint u=frointers.front();
            //bfs_kernel
            simpleBFS_CUDA<<<grid, block>>>(nodeSize, level, d_nodePointers, d_edgeArr, d_distacne, d_visited);
            frointers.pop();
        }
    }
}
