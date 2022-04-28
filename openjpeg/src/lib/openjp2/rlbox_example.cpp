
#define RLBOX_SINGLE_THREADED_INVOCATIONS

#include "rlbox_wasm2c_sandbox.hpp"
#include "rlbox.hpp"
#include "openjpeg.h"
#include <stdio.h>

using namespace rlbox;

int main()
{
   rlbox_sandbox<rlbox_wasm2c_sandbox> sandbox;
   sandbox.create_sandbox("./libopenjp2.so");
   // Invoke function bar with parameter 1
   auto tainted_ncpus = sandbox.invoke_sandbox_function(opj_get_num_cpus);
   int n_cpus = tainted_ncpus.copy_and_verify([](int fn){ return fn; });
   printf("n_cpus: %d\n", n_cpus);
   sandbox.destroy_sandbox();
   return 0;
}
