#include "CalculateOpt.cuh"
//#include "main.cuh"
#include "constants.cuh"
#include "ArgumentParser.cuh"
int main(int argc, char** argv) {
    cudaFree(0);
    ArgumentParser arguments(argc, argv, true);
    if (arguments.algo.empty()) {
        arguments.algo = "bfs";
    }
    
    if (arguments.algo == "bfs") {
        cout << "arguments.algo " << arguments.algo << endl;
    cout<<"arguments.sourceNode "<<arguments.sourceNode<<endl;
        bfs_opt(arguments.input, arguments.sourceNode, arguments.adviseK);
    } else if (arguments.algo == "cc") {
        cc_opt(arguments.input, arguments.adviseK);
    } else if (arguments.algo == "sssp") {
        sssp_opt(arguments.input, arguments.sourceNode, arguments.adviseK);
    } else if (arguments.algo == "pr") {
        pr_opt(arguments.input, arguments.adviseK);
    }
    
    return 0;
}

