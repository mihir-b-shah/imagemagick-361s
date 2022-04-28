
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
   auto tainted_version = sandbox.invoke_sandbox_function(opj_version);
   const char* version = tainted_version.UNSAFE_unverified();
   printf("%s\n", version);
   sandbox.destroy_sandbox();
   return 0;
}
