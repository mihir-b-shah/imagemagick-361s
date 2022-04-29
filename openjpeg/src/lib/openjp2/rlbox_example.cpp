
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
   auto v = sandbox.invoke_sandbox_function(opj_get_num_cpus).UNSAFE_unverified();
   printf("%d\n", v);
   sandbox.destroy_sandbox();
   return 0;
}
