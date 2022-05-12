
#define RLBOX_SINGLE_THREADED_INVOCATIONS

#include "rlbox_wasm2c_sandbox.hpp"
#include "rlbox.hpp"
#include "openjpeg.h"
#include <stdio.h>

using namespace rlbox;

static tainted<OPJ_BOOL, rlbox_wasm2c_sandbox> JP2SeekHandler(
                               rlbox_sandbox<rlbox_wasm2c_sandbox>& sbx,
                               tainted<OPJ_OFF_T, rlbox_wasm2c_sandbox> tainted_offs,
                               tainted<void*, rlbox_wasm2c_sandbox> tainted_ctx)
{
  OPJ_OFF_T offset = tainted_offs.UNSAFE_unverified();
  void* context = sbx.lookup_app_ptr(tainted_ctx);
  return OPJ_FALSE;
}

int main()
{
   rlbox_sandbox<rlbox_wasm2c_sandbox> sandbox;
   sandbox.create_sandbox("./libopenjp2.so");
   auto seek_cb = sandbox.register_callback(JP2SeekHandler);
   sandbox.destroy_sandbox();
   return 0;
}
