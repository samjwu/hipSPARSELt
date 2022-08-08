# hipSPARSELt
hipSPARSELt is a SPARSE marshalling library, with multiple supported backends. It sits between the application and a 'worker' SPARSE library, marshalling inputs into the backend library and marshalling results back to the application. hipSPARSELt exports an interface that does not require the client to change, regardless of the chosen backend. Currently, hipSPARSELt supports [rocSPARSELt](library/src/hcc_detial/rocsparselt) and [cuSPARSELt v0.3](https://docs.nvidia.com/cuda/cusparselt) as backends.

## Installing pre-built packages
Download pre-built packages either from [ROCm's package servers](https://rocm.github.io/install.html#installing-from-amd-rocm-repositories) or by clicking the github releases tab and manually downloading, which could be newer. Release notes are available for each release on the releases tab.
* `sudo apt update && sudo apt install hipsparselt`
* (TDB - hipsparselt needs to be published to ROCm's package servers )

## Quickstart hipSPARSELt build

### Bash helper build script
The root of this repository has a helper bash script `install.sh` to build and install hipSPARSELt on Ubuntu with a single command.  It does not take a lot of options and hard-codes configuration that can be specified through invoking cmake directly, but it's a great way to get started quickly and can serve as an example of how to build/install. A few commands in the script need sudo access, so it may prompt you for a password.
```
# Run install.sh script
# Command line options:
#   -h|--help            - prints help message
#   -i|--install         - install after build
#   -d|--dependencies    - install build dependencies
#   -c|--clients         - build library clients too (combines with -i & -d)
#   -g|--debug           - build with debug flag
#   -k|--relwithdebinfo  - build with RelWithDebInfo

./install.sh -dc
```
## Functions supported
- ROCm
   - AMD sparse MFMA matrix core support
   - Mixed-precision computation support:
     - <span style="color:green">FP16</span> input/output, <span style="color:green">FP32</span> Matrix Core accumulate
     - <span style="color:green">BFLOAT16</span> input/output, <span style="color:green">FP32</span> Matrix Core accumulate
     - <span style="color:green">INT8</span> inpput/output, <span style="color:green">INT32</span> Matrix Core accumulate
   - Matrix pruning and compression functionalities
   - Auto-tuning functionality (see hipsparseLtMatmulSearch())
   - Batched Sparse Gemm support:
     - Single sparse matrix / Multiple dense matrices (Broadcast)
     - Multiple sparse and dense matrices
   - Activation function fuse in spmm kernel support:
     - ReLU
     - ClippedReLU (ReLU with uppoer bound and threshold setting)
     - GeLU
     - Abs
     - LeakyReLU
     - Sigmoid
     - Tanh
   - On Going Feature Development
     - Add support for Mixed-precision computation
       - <span style="color:green">FP8</span> input/output, <span style="color:green">FP32</span> Matrix Core accumulate
       - <span style="color:green">BF8</span> input/output, <span style="color:green">FP32</span> Matrix Core accumulate
     - Add kernel selection and genroator, used to provide the appropriate solution for the specific problem.
- CUDA
  - Support cusparseLt v0.3

## Documentation
The latest hipSPARSELt documentation and API description can be found [here](doc/hipSPARSELt_api.docx) or downloaded as [pdf](doc/hipSPARSELt_api.pdf).

## hipSPARSELt interface examples
The hipSPARSELt interface is compatible with cuSPARSELt APIs. Porting a CUDA application which originally calls the cuSPARSELt API to an application calling hipSPARSELt API should be relatively straightforward. For example, the hipSPARSELt matmul interface is


### matmul API

```c
hipsparseStatus_t hipsparseLtMatmul(const hipsparseLtHandle_t*     handle,
                                    const hipsparseLtMatmulPlan_t* plan,
                                    const void*                    alpha,
                                    const void*                    d_A,
                                    const void*                    d_B,
                                    const void*                    beta,
                                    const void*                    d_C,
                                    void*                          d_D,
                                    void*                          workspace,
                                    hipStream_t*                   streams,
                                    int32_t                        numStreams);

```

hipSPARSELt assumes matrix A, B, C, D and workspace are allocated in GPU memory space filled with data. Users are responsible for copying data from/to the host and device memory.

## Running tests and benchmark tool
### Unit tests
To run unit tests, hipSPARSELt has to be built with option -DBUILD_CLIENTS_TESTS=ON (or using ./install.sh -c)
```
# Go to hipSPARSELt build directory
cd hipSPARSELt; cd build/release

# Run all tests
./clients/staging/hipsparselt-test
```

### Benchmarks
To run benchmarks, hipSPARSELt has to be built with option -DBUILD_CLIENTS_BENCHMARKS=ON (or using ./install.sh -c).
```
# Go to hipSPARSELt build directory
cd hipSPARSELt/build/release

# Run benchmark, e.g.
./clients/staging/hipsparselt-bench -f spmm -i 200 -m 256 -n 256 -k 256
