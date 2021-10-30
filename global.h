#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <string>
#include <ctime>
#include <random>
#include <stdio.h>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include<queue>
using namespace std;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef uint SIZE_TYPE;
typedef ull EDGE_POINTER_TYPE;

struct OutEdge{
    uint end;
};

struct OutEdgeWeighted{
    uint end;
    uint w8;
};

struct Edge{
	uint source;
    uint end;
};

struct EdgeWeighted{
	uint source;
    uint end;
    uint w8;
};

struct llOutEdge{
    ull end;
};

struct llOutEdgeWeighted{
    ull end;
    ull w8;
};
