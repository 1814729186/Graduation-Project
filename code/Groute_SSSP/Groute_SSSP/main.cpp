#include <cstdio>
#include <cuda_runtime.h>
#include <gflags/gflags.h>

#include <iostream>

#include <utils/utils.h>
#include <utils/interactor.h>
#include <utils/app_skeleton.h>


DEFINE_bool(nf, false, "Run the Near-Far SSSP solver");

bool TestSSSPAsyncMulti__NF__(int ngpus);
bool TestSSSPAsyncMulti(int ngpus);
bool TestSSSPAsyncMultiOptimized(int ngpus);

bool TestSSSPSingle__NF__();
bool TestSSSPSingle();

void CleanupGraphs();


namespace sssp
{
    struct App
    {
        static const char* Name() { return FLAGS_nf ? "sssp-nf" : "sssp"; }
        static const char* NameUpper() { return FLAGS_nf ? "SSSP-nf" : "SSSP"; }

        static bool Single()
        {
            return FLAGS_nf ? TestSSSPSingle__NF__() : TestSSSPSingle();
        }

        static bool AsyncMulti(int G)
        {
            return FLAGS_opt
                ? TestSSSPAsyncMultiOptimized(G)
                : FLAGS_nf
                ? TestSSSPAsyncMulti__NF__(G)
                : TestSSSPAsyncMulti(G);
        }

        static void Cleanup() { CleanupGraphs(); }
    };
}

int main(int argc, char** argv)
{
    Skeleton<sssp::App> app;
    int exit = app(argc, argv);

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaError_t cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return exit;
}