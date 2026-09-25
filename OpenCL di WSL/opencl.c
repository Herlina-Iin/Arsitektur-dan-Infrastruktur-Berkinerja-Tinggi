#include <stdio.h>
#include <CL/cl.h>

const char *kernel_source =
"__kernel void hello(__global char *output) {"
"    const char text[] = \"Hello Herlina Iin Nur Soleha from GPU!\";"
"    int i = get_global_id(0);"
"    if (i < sizeof(text) - 1)"
"        output[i] = text[i];"
"}";

int main() {
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem buffer;
    cl_int err;

    char output[100] = {0};
    char device_name[256];

    clGetPlatformIDs(1, &platform, NULL);

    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    clGetDeviceInfo(device, CL_DEVICE_NAME,
                    sizeof(device_name), device_name, NULL);

    printf("Device: %s\n", device_name);
    printf("Device Type: GPU\n");

    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);

    queue = clCreateCommandQueueWithProperties(
        context, device, NULL, &err);

    program = clCreateProgramWithSource(
        context, 1, &kernel_source, NULL, &err);

    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    if (err != CL_SUCCESS) {
        printf("Failed to build OpenCL kernel.\n");
        return 1;
    }

    kernel = clCreateKernel(program, "hello", &err);

    buffer = clCreateBuffer(
        context, CL_MEM_WRITE_ONLY, sizeof(output), NULL, &err);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);

    size_t global_size = 40;

    clEnqueueNDRangeKernel(
        queue, kernel, 1, NULL,
        &global_size, NULL, 0, NULL, NULL);

    clFinish(queue);

    clEnqueueReadBuffer(
        queue, buffer, CL_TRUE, 0,
        sizeof(output), output, 0, NULL, NULL);

    printf("%s\n", output);

    clReleaseMemObject(buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}
