/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                              JJJ  PPPP    222                               %
%                               J   P   P  2   2                              %
%                               J   PPPP     22                               %
%                            J  J   P       2                                 %
%                             JJ    P      22222                              %
%                                                                             %
%                                                                             %
%                     Read/Write JPEG-2000 Image Format                       %
%                                                                             %
%                                   Cristy                                    %
%                                Nathan Brown                                 %
%                                 June 2001                                   %
%                                                                             %
%                                                                             %
%  Copyright @ 2001 ImageMagick Studio LLC, a non-profit organization         %
%  dedicated to making software imaging solutions freely available.           %
%                                                                             %
%  You may not use this file except in compliance with the License.  You may  %
%  obtain a copy of the License at                                            %
%                                                                             %
%    https://imagemagick.org/script/license.php                               %
%                                                                             %
%  Unless required by applicable law or agreed to in writing, software        %
%  distributed under the License is distributed on an "AS IS" BASIS,          %
%  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   %
%  See the License for the specific language governing permissions and        %
%  limitations under the License.                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
*/


#define RLBOX_SINGLE_THREADED_INVOCATIONS

/*
  Include declarations.
*/
#include "MagickCore/studio.h"
#include "MagickCore/artifact.h"
#include "MagickCore/attribute.h"
#include "MagickCore/blob.h"
#include "MagickCore/blob-private.h"
#include "MagickCore/cache.h"
#include "MagickCore/colorspace.h"
#include "MagickCore/colorspace-private.h"
#include "MagickCore/color.h"
#include "MagickCore/color-private.h"
#include "MagickCore/exception.h"
#include "MagickCore/exception-private.h"
#include "MagickCore/image.h"
#include "MagickCore/image-private.h"
#include "MagickCore/list.h"
#include "MagickCore/magick.h"
#include "MagickCore/memory_.h"
#include "MagickCore/monitor.h"
#include "MagickCore/monitor-private.h"
#include "MagickCore/option.h"
#include "MagickCore/pixel-accessor.h"
#include "MagickCore/profile.h"
#include "MagickCore/property.h"
#include "MagickCore/quantum-private.h"
#include "MagickCore/resource_.h"
#include "MagickCore/semaphore.h"
#include "MagickCore/static.h"
#include "MagickCore/statistic.h"
#include "MagickCore/string_.h"
#include "MagickCore/string-private.h"
#include "MagickCore/module.h"
#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)

#include <openjpeg.h>
#include <memory>
#include <rlbox_wasm2c_sandbox.hpp>
#include <rlbox.hpp>
#include <type_traits>

using namespace rlbox;

/**
 * Verifier unit test definitions
 */
// #define JP2_COMP__VERIFIER__TEST
#define JP2_COMP__VERIFIER__TEST_VALUE 0

// #define JP2_IMG_NUMCOMPS__VERIFIER__TEST
#define JP2_IMG_NUMCOMPS__VERIFIER__TEST_VALUE 0

// #define JP2_IMAGE__VERIFIER__TEST
#define JP2_IMAGE__VERIFIER__TEST_VALUE 0

// #define COMP_SGND__VERIFIER__TEST
#define COMP_SGND__VERIFIER__TEST_VALUE 2

// #define COMP_DATA__VERIFIER__TEST
#define COMP_DATA__VERIFIER__TEST_VALUE malloc(1)

// #define ICC_PROFILE_BUF__VERIFIER__TEST
#define ICC_PROFILE_BUF__VERIFIER__TEST_VALUE malloc(1)

// #define ERROR_MESSAGE__VERIFIER__TEST
#define ERROR_MESSAGE__VERIFIER__TEST_VALUE "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789x" // 301 characters + null

// #define CLIENT_DATA__VERIFIER__TEST
#define CLIENT_DATA__VERIFIER__TEST_VALUE malloc(1)

// #define READ_BUFFER__VERIFIER__TEST
#define READ_BUFFER__VERIFIER__TEST_VALUE 0

// #define READ_LENGTH__VERIFIER__TEST
#define READ_LENGTH__VERIFIER__TEST_VALUE 0

// #define GENERIC_IMAGE__VERIFIER__TEST
#define GENERIC_IMAGE__VERIFIER__TEST_VALUE 0

// #define CONTEXT_IMAGE__VERIFIER__TEST
#define CONTEXT_IMAGE__VERIFIER__TEST_VALUE 0

// #define SEEK_OFFSET__VERIFIER__TEST
#define SEEK_OFFSET__VERIFIER__TEST_VALUE 0

// #define GENERIC_EVENT_MANAGER__VERIFIER__TEST
#define GENERIC_EVENT_MANAGER__VERIFIER__TEST_VALUE 0

// #define CONTEXT_EVENT_MANAGER__VERIFIER__TEST
#define CONTEXT_EVENT_MANAGER__VERIFIER__TEST_VALUE 0

// #define SKIP_OFFSET__VERIFIER__TEST
#define SKIP_OFFSET__VERIFIER__TEST_VALUE 0

// #define WARNING_MESSAGE__VERIFIER__TEST
#define WARNING_MESSAGE__VERIFIER__TEST_VALUE "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789x" // 301 characters + null

// #define WRITE_BUFFER__VERIFIER__TEST
#define WRITE_BUFFER__VERIFIER__TEST_VALUE 0

// #define WRITE_LENGTH__VERIFIER__TEST
#define WRITE_LENGTH__VERIFIER__TEST_VALUE 0

// #define J2K_STATE__VERIFIER__TEST
#define J2K_STATE__VERIFIER__TEST_VALUE 0

// #define JP2_STATUS__SET_DECODE_AREA__VERIFIER__TEST
#define JP2_STATUS__SET_DECODE_AREA__VERIFIER__TEST_VALUE 0

// #define OPJ_VERSION__TEST
#define OPJ_VERSION__TEST_VALUE 0

// #define SETUP_DECODER__STATUS__VERIFIER__TEST
#define SETUP_DECODER__STATUS__VERIFIER__TEST_VALUE 0

// #define READ_HEADER__STATUS__VERIFIER__TEST
#define READ_HEADER__STATUS__VERIFIER__TEST_VALUE 0

// #define JP2_STATUS__OPJ_GET_DECODED_TILE__VERIFIER__TEST
#define JP2_STATUS__OPJ_GET_DECODED_TILE__VERIFIER__TEST_VALUE 0

// #define JP2_STATUS__OPJ_DECODE__VERIFIER__TEST
#define JP2_STATUS__OPJ_DECODE__VERIFIER__TEST_VALUE 0

// #define JP2_STATUS__OPJ_END_DECOMPRESS__VERIFIER__TEST
#define JP2_STATUS__OPJ_END_DECOMPRESS__VERIFIER__TEST_VALUE 0

// #define JP2_CPARAM_DXY__VERIFIER__TEST
#define JP2_CPARAM_DXY__VERIFIER__TEST_VALUE 0

// #define JP2_CPARAM_OFFSET_DXY__VERIFIER__TEST
#define JP2_CPARAM_OFFSET_DXY__VERIFIER__TEST_VALUE 0

// #define JP2_CPARAMETERS__VERIFIER__TEST
#define JP2_CPARAMETERS__VERIFIER__TEST_VALUE 0

// #define JP2_STATUS__OPJ_START_COMPRESS__VERIFIER__TEST
#define JP2_STATUS__OPJ_START_COMPRESS__VERIFIER__TEST_VALUE 0

// #define JP2_STATUS__OPJ_ENCODE__VERIFIER__TEST
#define JP2_STATUS__OPJ_ENCODE__VERIFIER__TEST_VALUE 0

// #define JP2_STATUS__OPJ_END_COMPRESS__VERIFIER__TEST
#define JP2_STATUS__OPJ_END_COMPRESS__VERIFIER__TEST_VALUE 0


// reflection stuff allowing us to use fields in rlbox.
#define sandbox_fields_reflection_openjp2_class_opj_image_t(f, g, ...) \
  f(OPJ_UINT32        , x0              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , y0              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , x1              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , y1              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , numcomps        , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_COLOR_SPACE   , color_space     , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(opj_image_comp_t* , comps           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_BYTE*         , icc_profile_buf , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , icc_profile_len , FIELD_NORMAL, ##__VA_ARGS__) g()

#define sandbox_fields_reflection_openjp2_class_opj_image_comp_t(f, g, ...) \
  f(OPJ_UINT32        , dx              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , dy              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , w               , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , h               , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , x0              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , y0              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , prec            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , bpp             , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , sgnd            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , resno_decoded   , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , factor          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32*       , data            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT16        , alpha           , FIELD_NORMAL, ##__VA_ARGS__) g()

#define sandbox_fields_reflection_openjp2_class_opj_poc_t(f, g, ...) \
  f(OPJ_UINT32        , resno0          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , compno0         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , layno1          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , resno1          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , compno1         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , layno0          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , precno0         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , precno1         , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_PROG_ORDER    , prg1            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_PROG_ORDER    , prg             , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_CHAR[5]       , progorder       , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , tile            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32_SEMANTICALLY_BUT_INT32, tx0, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32_SEMANTICALLY_BUT_INT32, tx1, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32_SEMANTICALLY_BUT_INT32, ty0, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32_SEMANTICALLY_BUT_INT32, ty1, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , layS            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , resS            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , compS           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , prcS            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , layE            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , resE            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , compE           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , prcE            , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , txS             , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , txE             , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , tyS             , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , tyE             , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , dx              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , dy              , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , lay_t           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , res_t           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , comp_t          , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , prc_t           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , tx0_t           , FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32        , ty0_t           , FIELD_NORMAL, ##__VA_ARGS__) g()

#define sandbox_fields_reflection_openjp2_class_opj_cparameters_t(f, g, ...) \
  f(OPJ_BOOL, tile_size_on, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, cp_tx0, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, cp_ty0, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, cp_tdx, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, cp_tdy, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, cp_disto_alloc, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, cp_fixed_alloc, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, cp_fixed_quality, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int*, cp_matrice, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(char*, cp_comment, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, csty, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_PROG_ORDER, prog_order, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(unsigned char[sizeof(opj_poc_t)*32], POC, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT32, numpocs, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, tcp_numlayers, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(float[100], tcp_rates, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(float[100], tcp_distoratio, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, numresolution, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, cblockw_init, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, cblockh_init, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, mode, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, irreversible, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, roi_compno, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, roi_shift, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, res_spec, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int[OPJ_J2K_MAXRLVLS], prcw_init, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int[OPJ_J2K_MAXRLVLS], prch_init, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(char[OPJ_PATH_LEN], infile, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(char[OPJ_PATH_LEN], outfile, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, index_on, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(char[OPJ_PATH_LEN], index, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, image_offset_x0, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, image_offset_y0, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, subsampling_dx, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, subsampling_dy, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, decod_format, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, cod_format, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_BOOL, jpwl_epc_on, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, jpwl_hprot_MH, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int[JPWL_MAX_NO_TILESPECS], jpwl_hprot_TPH_tileno, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int[JPWL_MAX_NO_TILESPECS], jpwl_hprot_TPH, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int[JPWL_MAX_NO_TILESPECS], jpwl_pprot_tileno, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int[JPWL_MAX_NO_TILESPECS], jpwl_pprot_packno, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int[JPWL_MAX_NO_TILESPECS], jpwl_pprot, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, jpwl_sens_size, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, jpwl_sens_addr, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, jpwl_sens_range, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, jpwl_sens_MH, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int[JPWL_MAX_NO_TILESPECS], jpwl_sens_TPH_tileno, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int[JPWL_MAX_NO_TILESPECS], jpwl_sens_TPH, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_CINEMA_MODE, cp_cinema, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, max_comp_size, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_RSIZ_CAPABILITIES, cp_rsiz, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(char, tp_on, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(char, tp_flag, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(char, tcp_mct, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_BOOL, jpip_on, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(void*, mct_data, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(int, max_cs_size, FIELD_NORMAL, ##__VA_ARGS__) g() \
  f(OPJ_UINT16, rsiz, FIELD_NORMAL, ##__VA_ARGS__) g()

#define sandbox_fields_reflection_openjp2_allClasses(f, ...) \
  f(opj_image_t      , openjp2, ##__VA_ARGS__)               \
  f(opj_image_comp_t , openjp2, ##__VA_ARGS__)               \
  f(opj_poc_t        , openjp2, ##__VA_ARGS__)               \
  f(opj_cparameters_t, openjp2, ##__VA_ARGS__)
    
rlbox_load_structs_from_library(openjp2);

#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)
/* forward declarations */
static void JP2ErrorHandler(rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                            tainted<const char*, rlbox_wasm2c_sandbox> tainted_msg,
                            tainted<void*, rlbox_wasm2c_sandbox> tainted_cl_data);
static tainted<OPJ_SIZE_T, rlbox_wasm2c_sandbox> JP2ReadHandler(
                                 rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                                 tainted<void*, rlbox_wasm2c_sandbox> tainted_buf,
                                 tainted<OPJ_SIZE_T, rlbox_wasm2c_sandbox> tainted_len,
                                 tainted<void*, rlbox_wasm2c_sandbox> tainted_ctx);
static tainted<OPJ_BOOL, rlbox_wasm2c_sandbox> JP2SeekHandler(
                               rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                               tainted<OPJ_OFF_T, rlbox_wasm2c_sandbox> tainted_offs,
                               tainted<void*, rlbox_wasm2c_sandbox> tainted_ctx);
static tainted<OPJ_OFF_T, rlbox_wasm2c_sandbox> JP2SkipHandler(
                                rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                                tainted<OPJ_OFF_T, rlbox_wasm2c_sandbox> tainted_offs,
                                tainted<void*, rlbox_wasm2c_sandbox> tainted_ctx);
static void JP2WarningHandler(rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                              tainted<const char*, rlbox_wasm2c_sandbox> tainted_msg,
                              tainted<void*, rlbox_wasm2c_sandbox> tainted_cl_data);
static tainted<OPJ_SIZE_T, rlbox_wasm2c_sandbox> JP2WriteHandler(
                                  rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                                  tainted<void*, rlbox_wasm2c_sandbox> tainted_buf,
                                  tainted<OPJ_SIZE_T, rlbox_wasm2c_sandbox> tainted_len,
                                  tainted<void*, rlbox_wasm2c_sandbox> tainted_ctx);
#endif

#define CB_TYPE(name) decltype(std::declval<rlbox_sandbox<rlbox_wasm2c_sandbox>>().register_callback( name ))

class jp2_sandbox {
public:
  jp2_sandbox() : sandbox() {
    sandbox.create_sandbox("/home/mihirs/nets/imagemagick-361s/openjpeg/src/lib/openjp2/libopenjp2.so");
    error_cb = sandbox.register_callback(JP2ErrorHandler);
    read_cb = sandbox.register_callback(JP2ReadHandler);
    seek_cb = sandbox.register_callback(JP2SeekHandler);
    skip_cb = sandbox.register_callback(JP2SkipHandler);
    warn_cb = sandbox.register_callback(JP2WarningHandler);
    write_cb = sandbox.register_callback(JP2WriteHandler);
    tmp = this->sb()->malloc_in_sandbox<uint8_t>();
  }
  ~jp2_sandbox() {
    this->sb()->free_in_sandbox(tmp);
    sandbox.destroy_sandbox();
  }
  rlbox_sandbox<rlbox_wasm2c_sandbox>* sb(){ return &sandbox; }

public:
  CB_TYPE(JP2ErrorHandler) error_cb;
  CB_TYPE(JP2ReadHandler) read_cb;
  CB_TYPE(JP2SeekHandler) seek_cb;
  CB_TYPE(JP2SkipHandler) skip_cb;
  CB_TYPE(JP2WarningHandler) warn_cb;
  CB_TYPE(JP2WriteHandler) write_cb;

  template<typename T>
  bool is_in_sandbox(tainted<T*, rlbox_wasm2c_sandbox> ptr) {
    return this->sb()->is_in_same_sandbox(ptr.unverified_safe_pointer_because(1, "Only passing to rlbox"), tmp.unverified_safe_pointer_because(1, "Only passing to rlbox"));
  }
  
  template<typename T>
  bool is_in_sandbox(T* ptr) {
    return this->sb()->is_in_same_sandbox(ptr, tmp.unverified_safe_pointer_because(1, "Only passing to rlbox"));
  }

  bool is_in_sandbox(uintptr_t ptr) {
    return this->sb()->is_in_same_sandbox(reinterpret_cast<const void*>(ptr), tmp.unverified_safe_pointer_because(1, "Only passing to rlbox"));
  }


  void fail(const char *thing) {
    printf("ERROR: INVALID %s CAUGHT\n", thing);
    exit(EXIT_FAILURE);
  }

private:
  rlbox_sandbox<rlbox_wasm2c_sandbox> sandbox;
  tainted<uint8_t*, rlbox_wasm2c_sandbox> tmp;
};

jp2_sandbox* sandbox = nullptr;

#define INIT_SANDBOX() if (sandbox == nullptr) { sandbox = new jp2_sandbox(); }

#endif

/*
  Forward declarations.
*/
#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)
static MagickBooleanType
  WriteJP2Image(const ImageInfo *,Image *,ExceptionInfo *);
#endif

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   I s J 2 K                                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  IsJ2K() returns MagickTrue if the image format type, identified by the
%  magick string, is J2K.
%
%  The format of the IsJ2K method is:
%
%      MagickBooleanType IsJ2K(const unsigned char *magick,const size_t length)
%
%  A description of each parameter follows:
%
%    o magick: compare image format pattern against these bytes.
%
%    o length: Specifies the length of the magick string.
%
*/
static MagickBooleanType IsJ2K(const unsigned char *magick,const size_t length)
{
  if (length < 4)
    return(MagickFalse);
  if (memcmp(magick,"\xff\x4f\xff\x51",4) == 0)
    return(MagickTrue);
  return(MagickFalse);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   I s J P 2                                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  IsJP2() returns MagickTrue if the image format type, identified by the
%  magick string, is JP2.
%
%  The format of the IsJP2 method is:
%
%      MagickBooleanType IsJP2(const unsigned char *magick,const size_t length)
%
%  A description of each parameter follows:
%
%    o magick: compare image format pattern against these bytes.
%
%    o length: Specifies the length of the magick string.
%
*/
static MagickBooleanType IsJP2(const unsigned char *magick,const size_t length)
{
  if (length < 4)
    return(MagickFalse);
  if (memcmp(magick,"\x0d\x0a\x87\x0a",4) == 0)
    return(MagickTrue);
  if (length < 12)
    return(MagickFalse);
  if (memcmp(magick,"\x00\x00\x00\x0c\x6a\x50\x20\x20\x0d\x0a\x87\x0a",12) == 0)
    return(MagickTrue);
  return(MagickFalse);
}

static tainted<opj_image_t*, rlbox_wasm2c_sandbox>
taint_img(opj_image_t* raw_img)
{
  tainted<opj_image_t*, rlbox_wasm2c_sandbox> tainted_jp2_image = sandbox->sb()->malloc_in_sandbox<opj_image_t>();
  memset(tainted_jp2_image.unverified_safe_pointer_because(sizeof(opj_image_t), "Copying jp2_image over"), 0, sizeof(opj_image_t));
  /*
  memcpy(tainted_jp2_image.unverified_safe_pointer_because(sizeof(opj_image_t), "Copying jp2_image over"), raw_img, sizeof(opj_image_t));

  tainted<opj_image_comp_t*, rlbox_wasm2c_sandbox> tainted_comps = sandbox->sb()->malloc_in_sandbox<opj_image_comp_t>(raw_img->numcomps);
  memcpy(tainted_comps.unverified_safe_pointer_because(sizeof(opj_image_comp_t)*raw_img->numcomps, "Copying comps over"), raw_img->comps, sizeof(opj_image_comp_t)*(raw_img->numcomps));
  tainted_jp2_image->comps = tainted_comps;

  for (int i = 0; i<raw_img->numcomps; ++i) {
    if (raw_img->comps[i].data != NULL) {
      size_t size = raw_img->comps[i].w * raw_img->comps[i].h * sizeof(OPJ_INT32);
      tainted<OPJ_INT32*, rlbox_wasm2c_sandbox> tainted_data = sandbox->sb()->malloc_in_sandbox<OPJ_INT32>(size);
      memcpy(tainted_data.unverified_safe_pointer_because(size, "Copying comps over"), raw_img->comps[i].data, size);
    }
  }

  tainted_jp2_image->icc_profile_len = 0;
  */
  //printf("Finished tainting.\n");
  return tainted_jp2_image;
}

static opj_image_comp_t*
jp2_comp__verifier(std::unique_ptr<tainted<opj_image_comp_t, rlbox_wasm2c_sandbox>> safe_ptr_)
{
  std::unique_ptr<tainted<opj_image_comp_t, rlbox_wasm2c_sandbox>> safe_ptr;
#ifdef JP2_COMP__VERIFIER__TEST
  safe_ptr = JP2_COMP__VERIFIER__TEST_VALUE;
#else
  safe_ptr = std::move(safe_ptr_);
#endif

  opj_image_comp_t* comp = (opj_image_comp_t*) malloc(sizeof(opj_image_comp_t));

  comp->dx = safe_ptr.get()->dx.copy_and_verify([](OPJ_UINT32 dx){
    if (dx == 0) sandbox->fail("comp->dx");
    return dx;
  });
  comp->dy = safe_ptr.get()->dy.copy_and_verify([](OPJ_UINT32 dy){
    if (dy == 0) sandbox->fail("comp->dy");
    return dy;
  });
  comp->w = safe_ptr.get()->w.copy_and_verify([](OPJ_UINT32 w){ return w; });
  comp->h = safe_ptr.get()->h.copy_and_verify([](OPJ_UINT32 h){ return h; });
  comp->x0 = safe_ptr.get()->x0.copy_and_verify([](OPJ_UINT32 x0){ return x0; });
  comp->y0 = safe_ptr.get()->y0.copy_and_verify([](OPJ_UINT32 y0){ return y0; });
  comp->prec = safe_ptr.get()->prec.copy_and_verify([](OPJ_UINT32 prec){ return prec; });
  comp->bpp = safe_ptr.get()->bpp.copy_and_verify([](OPJ_UINT32 bpp){ return bpp; });
  comp->sgnd = safe_ptr.get()->sgnd.copy_and_verify([](OPJ_UINT32 sgnd_){
    OPJ_UINT32 sgnd;
#ifdef COMP_SGND__VERIFIER__TEST
    sgnd = COMP_SGND__VERIFIER__TEST_VALUE;
#else
    sgnd = sgnd_;
#endif

    if (!(sgnd == 0 || sgnd == 1)) sandbox->fail("comp->sgnd");
    return sgnd;
  });
  comp->resno_decoded = safe_ptr.get()->resno_decoded.copy_and_verify([](OPJ_UINT32 r){ return r; });
  comp->factor = safe_ptr.get()->factor.copy_and_verify([](OPJ_UINT32 fac){ return fac; });
  comp->alpha = safe_ptr.get()->alpha.copy_and_verify([](OPJ_UINT32 alpha){ return alpha; });
  comp->data = safe_ptr.get()->data.copy_and_verify_address([&comp](uintptr_t addr_){
    uintptr_t addr;
#ifdef COMP_DATA__VERIFIER__TEST
    addr = COMP_DATA__VERIFIER__TEST_VALUE;
#else
    addr = addr_;
#endif

    if (addr != 0) {
      OPJ_INT32* data = (OPJ_INT32*) malloc(sizeof(OPJ_INT32)*(comp->w)*(comp->h));
      //printf("Copying %lu bytes at line %d\n", sizeof(OPJ_INT32)*(comp->w)*(comp->h), __LINE__);
      memcpy(data, (OPJ_INT32*) addr, sizeof(OPJ_INT32)*(comp->w)*(comp->h)); 
      return data;
    } else {
      return (int*) NULL;
    }
  });
  
  return comp;
}

static int 
jp2_img_numcomps__verifier(int nc_)
{
  int nc;
#ifdef JP2_IMG_NUMCOMPS__VERIFIER__TEST
  nc = JP2_IMG_NUMCOMPS__VERIFIER__TEST_VALUE;
#else
  nc = nc_;
#endif

  if (nc < 0) {
    sandbox->fail("numcomps");
  }
  return nc;
}

#define MINIMUM_COLOR_SPACE -1
#define MAXIMUM_COLOR_SPACE 5
static opj_image_t*
jp2_image__verifier(std::unique_ptr<tainted<opj_image_t, rlbox_wasm2c_sandbox>> safe_ptr_)
{
  std::unique_ptr<tainted<opj_image_t, rlbox_wasm2c_sandbox>> safe_ptr;
#ifdef JP2_IMAGE__VERIFIER__TEST
  safe_ptr = JP2_IMAGE__VERIFIER__TEST_VALUE;
#else
  safe_ptr = std::move(safe_ptr_);
#endif

  opj_image_t* img = (opj_image_t*) malloc(sizeof(opj_image_t));

  img->x0 = safe_ptr.get()->x0.copy_and_verify([](OPJ_UINT32 x0){ return x0; });
  img->y0 = safe_ptr.get()->y0.copy_and_verify([](OPJ_UINT32 y0){ return y0; });
  img->x1 = safe_ptr.get()->x1.copy_and_verify([](OPJ_UINT32 x1){ return x1; });
  img->y1 = safe_ptr.get()->y1.copy_and_verify([](OPJ_UINT32 y1){ return y1; });
  img->numcomps = safe_ptr.get()->numcomps.copy_and_verify([](OPJ_UINT32 nc){ return nc; });
  img->color_space = safe_ptr.get()->color_space.copy_and_verify([](OPJ_COLOR_SPACE cs){
    if (!(cs >= MINIMUM_COLOR_SPACE || cs <= MAXIMUM_COLOR_SPACE)) {
      sandbox->fail("opj_image_t");
    }
    return cs;
  });

  img->comps = (opj_image_comp_t*) malloc(sizeof(opj_image_comp_t)*img->numcomps);
  for (int i = 0; i<img->numcomps; ++i) {
    // free the src op
    if (!(sandbox->is_in_sandbox(safe_ptr.get()->comps+i))) {
      sandbox->fail("opj_image_t");
    }
    memcpy(&(img->comps[i]), (safe_ptr.get()->comps+i).copy_and_verify(jp2_comp__verifier),
      sizeof(opj_image_comp_t));
  }

  img->icc_profile_len = safe_ptr.get()->icc_profile_len.copy_and_verify([](OPJ_UINT32 icclen){
    if (icclen < 0) {
      sandbox->fail("icc_profile_len");
    }
    return icclen;
  });

  img->icc_profile_buf = (OPJ_BYTE*) malloc(sizeof(OPJ_BYTE)*(img->icc_profile_len));
  memcpy(
    img->icc_profile_buf,
    // safe_ptr.get()->icc_profile_buf.unverified_safe_pointer_because(img->icc_profile_len, "Copying icc profile buf"),
    safe_ptr.get()->icc_profile_buf.copy_and_verify_address([img] (OPJ_BYTE *buffer) {
      OPJ_BYTE *buffer;
#ifdef ICC_PROFILE_BUF__VERIFIER__TEST
      buffer = ICC_PROFILE_BUF__VERIFIER__TEST_VALUE;
#else
      buffer = std::move(buffer_);
#endif

      if (
        (img->icc_profile_len == 0 && buffer != nullptr) ||
        (sandbox->is_in_sandbox(img->icc_profile_buf))
      ) {
        sandbox->fail("img_profile_buf");
      }
      return (void*) buffer;
    }),
    img->icc_profile_len
  );

  return img;
}

static opj_image_t*
untaint_img(tainted<opj_image_t*, rlbox_wasm2c_sandbox> tainted_jp2_image, opj_image_t* raw_img)
{
  opj_image_t* ret = tainted_jp2_image.copy_and_verify(jp2_image__verifier);
  //printf("Untainted img.\n");
  return ret;
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e a d J P 2 I m a g e                                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  ReadJP2Image() reads a JPEG 2000 Image file (JP2) or JPEG 2000
%  codestream (JPC) image file and returns it.  It allocates the memory
%  necessary for the new Image structure and returns a pointer to the new
%  image or set of images.
%
%  JP2 support is originally written by Nathan Brown, nathanbrown@letu.edu.
%
%  The format of the ReadJP2Image method is:
%
%      Image *ReadJP2Image(const ImageInfo *image_info,
%        ExceptionInfo *exception)
%
%  A description of each parameter follows:
%
%    o image_info: the image info.
%
%    o exception: return any errors or warnings in this structure.
%
*/

#define MAXIMUM_ERROR_MESSAGE_LENGTH 300
static const char* error_message__verifier(std::unique_ptr<const char[]> message_) {
  std::unique_ptr<const char[]> message;
#ifdef ERROR_MESSAGE__VERIFIER__TEST
  message = ERROR_MESSAGE__VERIFIER__TEST_VALUE;
#else
  message = std::move(message_);
#endif

  if (strlen(message.get()) > MAXIMUM_ERROR_MESSAGE_LENGTH) {
    sandbox->fail("error_message");
  }
  return message.release();
}

#define MINIMUM_SEVERITY 0
#define MAXIMUM_SEVERITY 799
#define MAXIMUM_EXCEPTION_REASON_LENGTH 300
#define MAXIMUM_EXCEPTION_DESCRIPTION_LENGTH 300
static void* client_data__verifier(uintptr_t client_data_) {
  uintptr_t client_data;
#ifdef CLIENT_DATA__VERIFIER__TEST
  client_data = CLIENT_DATA__VERIFIER__TEST_VALUE;
#else
  client_data = client_data_;
#endif

  ExceptionInfo *exception = (ExceptionInfo *) client_data;

  if (
    !(sandbox->is_in_sandbox(exception)) ||
    !(MINIMUM_SEVERITY <= exception->severity && exception->severity <= MAXIMUM_SEVERITY) ||
    !(sandbox->is_in_sandbox(exception->reason)) ||
    (strlen(exception->reason) > MAXIMUM_EXCEPTION_REASON_LENGTH) ||
    !(sandbox->is_in_sandbox(exception->description)) ||
    (strlen(exception->description) > MAXIMUM_EXCEPTION_DESCRIPTION_LENGTH) ||
    !(sandbox->is_in_sandbox(exception->exceptions)) ||
    !(sandbox->is_in_sandbox(exception->semaphore)) ||
    !(exception->signature == MagickCoreSignature || exception->signature == (~MagickCoreSignature))
  ) {
    sandbox->fail("client_data");
  }

  return reinterpret_cast<void*>(client_data);
}

#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)
// TODO: should this be static?
static void JP2ErrorHandler(rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                            tainted<const char*, rlbox_wasm2c_sandbox> tainted_msg,
                            tainted<void*, rlbox_wasm2c_sandbox> tainted_cl_data)
{
  const char* message = tainted_msg.copy_and_verify_string([](std::unique_ptr<const char[]> val){
    return error_message__verifier(std::move(val));
  });
  void* client_data = sandbox->sb()->lookup_app_ptr(tainted_cl_data);

  ExceptionInfo
    *exception;

  exception=(ExceptionInfo *) client_data;
  (void) ThrowMagickException(exception,GetMagickModule(),CoderError,
    message,"`%s'","OpenJP2");
}

static void* read_buffer__verifier(uintptr_t buffer_) {
  uintptr_t buffer;
#ifdef READ_BUFFER__VERIFIER__TEST
  buffer = READ_BUFFER__VERIFIER__TEST_VALUE;
#else
  buffer = buffer_;
#endif

  if (!(sandbox->is_in_sandbox(buffer))) {
    sandbox->fail("Read buffer");
  }
  return reinterpret_cast<void*>(buffer);
}

static OPJ_SIZE_T read_length__verifier(OPJ_SIZE_T length_) {
  OPJ_SIZE_T length;
#ifdef READ_LENGTH__VERIFIER__TEST
  length = READ_LENGTH__VERIFIER__TEST_VALUE;
#else
  length = length_;
#endif

  if (length > OPJ_J2K_STREAM_CHUNK_SIZE) {
    sandbox->fail("read length");
  }
  return length;
}

#define MINIMUM_COMPOSITION 0
#define MAXIMUM_COMPOSITION 81
static Image* generic_image__verifier(Image* image_) {
  Image *image;
#ifdef GENERIC_IMAGE__VERIFIER__TEST
  image = GENERIC_IMAGE__VERIFIER__TEST_VALUE;
#else
  image = image_;
#endif

  if (!(MINIMUM_COMPOSITION <= image->compose && image->compose <= MAXIMUM_COMPOSITION)) {
    sandbox->fail("image");
  }
  return image;
}

static uintptr_t context_image__verifier(uintptr_t context_) {
  uintptr_t context;
#ifdef CONTEXT_IMAGE__VERIFIER__TEST
  context = CONTEXT_IMAGE__VERIFIER__TEST_VALUE;
#else
  context = context_;
#endif

  if (!(sandbox->is_in_sandbox(context))) {
    sandbox->fail("read header context");
  }

  Image *image = reinterpret_cast<Image *>(context);
  return reinterpret_cast<uintptr_t>(generic_image__verifier(image));
}

// TODO: should these return types be tainted?
static tainted<OPJ_SIZE_T, rlbox_wasm2c_sandbox> JP2ReadHandler(
                                 rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                                 tainted<void*, rlbox_wasm2c_sandbox> tainted_buf,
                                 tainted<OPJ_SIZE_T, rlbox_wasm2c_sandbox> tainted_len,
                                 tainted<void*, rlbox_wasm2c_sandbox> tainted_ctx)
{
  void* buffer = tainted_buf.copy_and_verify_address(read_buffer__verifier);
  OPJ_SIZE_T length = tainted_len.copy_and_verify(read_length__verifier);
  void* context = sandbox->sb()->lookup_app_ptr(tainted_ctx);

  Image
    *image;

  ssize_t
    count;

  image = reinterpret_cast<Image *>(context);
  count=ReadBlob(image,(ssize_t) length, buffer);
  if (count == 0)
    return((OPJ_SIZE_T) -1);
  return((OPJ_SIZE_T) count);
}

OPJ_BOOL global__use_initial_user_data_length = false;
OPJ_OFF_T global__initial_user_data_length = -1;
static OPJ_OFF_T seek_offset__verifier(OPJ_OFF_T offset_) {
  OPJ_OFF_T offset;
#ifdef SEEK_OFFSET__VERIFIER__TEST
  offset = SEEK_OFFSET__VERIFIER__TEST_VALUE;
#else
  offset = offset_;
#endif

  if (global__use_initial_user_data_length && offset > global__initial_user_data_length) {
    sandbox->fail("seek offset");
  }
  return offset;
}

/*
static opj_event_mgr_t *generic_event_manager__verifier(opj_event_mgr_t *event_manager_) {
  opj_event_mgr_t event_manager;
#ifdef GENERIC_EVENT_MANAGER__VERIFIER__TEST
  event_manager = GENERIC_EVENT_MANAGER__VERIFIER__TEST_VALUE;
#else
  event_manager = event_manager_;
#endif

  opj_msg_callback eh = event_manager->error_handler == NULL ? 0 : 1;
  opj_msg_callback wh = event_manager->warning_handler == NULL ? 0 : 1;
  opj_msg_callback ih = event_manager->info_handler == NULL ? 0 : 1;

  if (
    !(sandbox->is_in_sandbox(event_manager->m_error_data)) ||
    !(sandbox->is_in_sandbox(event_manager->m_warning_data)) ||
    !(sandbox->is_in_sandbox(event_manager->m_info_data)) ||
    !(eh + wh + ih == 1) // Only one is non-NULL
  ) {
    sandbox->fail("event manager")
  }

  return event_manager;
}

static uintptr_t context_event_manager__verifier(uintptr_t context_) {
  uintptr_t context;
#ifdef CONTEXT_EVENT_MANAGER__VERIFIER__TEST
  context = CONTEXT_EVENT_MANAGER__VERIFIER__TEST_VALUE;
#else
  context = context_;
#endif

  if (!sandbox->is_in_sandbox(context)) {
    sandbox->fail("seek context");
  }

  opj_event_mgr_t *event_manager = generic_event_manager__verifier(reinterpret_cast<opj_event_mgr_t *>(context));
  return reinterpret_cast<uintptr_t>(event_manager);
}
*/

static tainted<OPJ_BOOL, rlbox_wasm2c_sandbox> JP2SeekHandler(
                               rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                               tainted<OPJ_OFF_T, rlbox_wasm2c_sandbox> tainted_offs,
                               tainted<void*, rlbox_wasm2c_sandbox> tainted_ctx)
{
  //printf("Hit seek handler.\n");
  OPJ_OFF_T offset = tainted_offs.copy_and_verify(seek_offset__verifier);
  void* context = sandbox->sb()->lookup_app_ptr(tainted_ctx);

  Image
    *image;

  image = reinterpret_cast<Image *>(context);
  auto ret = (SeekBlob(image,offset,SEEK_SET) < 0 ? OPJ_FALSE : OPJ_TRUE);
  //printf("Leaving seek handler.\n");
  return ret;
}

static OPJ_OFF_T skip_offset__verifier(OPJ_OFF_T offset_) {
  OPJ_OFF_T offset;
#ifdef SKIP_OFFSET__VERIFIER__TEST
  offset = SKIP_OFFSET__VERIFIER__TEST_VALUE;
#else
  offset = offset_;
#endif

  if (global__use_initial_user_data_length && offset >= global__initial_user_data_length) {
    sandbox->fail("skip offset");
  }
  return offset;
}

static tainted<OPJ_OFF_T, rlbox_wasm2c_sandbox> JP2SkipHandler(
                                rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                                tainted<OPJ_OFF_T, rlbox_wasm2c_sandbox> tainted_offs,
                                tainted<void*, rlbox_wasm2c_sandbox> tainted_ctx)
{
  OPJ_OFF_T offset = tainted_offs.copy_and_verify(skip_offset__verifier);
  void* context = sandbox->sb()->lookup_app_ptr(tainted_ctx);

  Image
    *image;

  image = reinterpret_cast<Image *>(context);
  return(SeekBlob(image,offset,SEEK_CUR) < 0 ? -1 : offset);
}

#define MAXIMUM_WARNING_MESSAGE_LENGTH 300
static const char* warning_message__verifier(std::unique_ptr<const char[]> message_) {
  std::unique_ptr<const char[]> message;
#ifdef WARNING_MESSAGE__VERIFIER__TEST
  message = WARNING_MESSAGE__VERIFIER__TEST_VALUE;
#else
  message = std::move(message_);
#endif

  if (strlen(message.get()) > MAXIMUM_WARNING_MESSAGE_LENGTH) {
    sandbox->fail("warning message");
  }
  return message.release();
}

static void JP2WarningHandler(rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                              tainted<const char*, rlbox_wasm2c_sandbox> tainted_msg,
                              tainted<void*, rlbox_wasm2c_sandbox> tainted_cl_data)
{
  const char* message = tainted_msg.copy_and_verify_string([](std::unique_ptr<const char[]> val){
    return warning_message__verifier(std::move(val));
  });
  void* client_data = sandbox->sb()->lookup_app_ptr(tainted_cl_data);

  ExceptionInfo
    *exception;

  exception = reinterpret_cast<ExceptionInfo *>(client_data);
  (void) ThrowMagickException(exception,GetMagickModule(),CoderWarning,
    message,"`%s'","OpenJP2");
}

static void* write_buffer__verifier(uintptr_t buffer_) {
  uintptr_t buffer;
#ifdef WRITE_BUFFER__VERIFIER__TEST
  buffer = WRITE_BUFFER__VERIFIER__TEST_VALUE;
#else
  buffer = buffer_;
#endif

  if (!(sandbox->is_in_sandbox(buffer))) {
    sandbox->fail("write buffer");
  }
  return reinterpret_cast<void*>(buffer);
}

static OPJ_SIZE_T write_length__verifier(OPJ_SIZE_T length_) {
  OPJ_SIZE_T length;
#ifdef WRITE_LENGTH__VERIFIER__TEST
  length = WRITE_LENGTH__VERIFIER__TEST_VALUE;
#else
  length = length_;
#endif

  if (length > OPJ_J2K_STREAM_CHUNK_SIZE) {
    sandbox->fail("write length");
  }
  return length;
}

static tainted<OPJ_SIZE_T, rlbox_wasm2c_sandbox> JP2WriteHandler(
                                  rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                                  tainted<void*, rlbox_wasm2c_sandbox> tainted_buf,
                                  tainted<OPJ_SIZE_T, rlbox_wasm2c_sandbox> tainted_len,
                                  tainted<void*, rlbox_wasm2c_sandbox> tainted_ctx)
{
  global__use_initial_user_data_length = false;

  void* buffer = tainted_buf.copy_and_verify_address(write_buffer__verifier);
  OPJ_SIZE_T length = tainted_len.copy_and_verify(write_length__verifier);
  //printf("Writing length %lu\n", length);
  void* context = sandbox->sb()->lookup_app_ptr(tainted_ctx);

  Image
    *image;

  ssize_t
    count;

  image=(Image *) context;
  count=WriteBlob(image,(ssize_t) length,(unsigned char *) buffer);
  //printf("Writing count %lu\n", count);
  return((OPJ_SIZE_T) count);
}

/*
static OPJ_UINT32 j2k_state__verifier(OPJ_UINT32 state_) {
  OPJ_UINT32 state;
#ifdef J2K_STATE__VERIFIER__TEST
  state = J2K_STATE__VERIFIER__TEST_VALUE;
#else
  state = state_;
#endif

  if (!(
    (state == J2K_STATE_NONE) ||
    (state == J2K_STATE_MHSOC) ||
    (state == J2K_STATE_MHSIZ) ||
    (state == J2K_STATE_MH) ||
    (state == J2K_STATE_TPHSOT) ||
    (state == J2K_STATE_TPH) ||
    (state == J2K_STATE_MT) ||
    (state == J2K_STATE_NEOC) ||
    (state == J2K_STATE_DATA) ||
    (state == J2K_STATE_EOC) ||
    (state == J2K_STATE_ERR) ||
  )) {
    sandbox->fail("j2k state");
  }

  return state;
}
*/

static OPJ_BOOL jp2_status__set_decode_area__verifier(OPJ_BOOL status_, tainted<opj_codec_t*, rlbox_wasm2c_sandbox> codec) {
  // ! opj_codec_private_t not exported by openjpeg
  /*
  tainted<opj_codec_private_t *> tainted_codec_private = sandbox_reinterpret_cast<opj_codec_private_t *>(tainted_codec);
  tainted<opj_jp2_t *>p_jp2 = sandbox_reinterpret_cast<opj_jp2_t *>(tainted_codec_private->m_codec);
  tainted<opj_j2k_t *>p_j2k = p_jp2->j2k;
  OPJ_UINT32 tile_width = p_j2k->m_cp.tw.unverified_safe_because("required == 1 in following if condition");
  OPJ_UINT32 tile_height = p_j2k->m_cp.th.unverified_safe_because("required == 1 in following if condition");
  OPJ_UINT32 j2k_state = p_j2k->m_specific_param.m_decoder.m_state.copy_and_verify(j2k_state__verifier);

  if (status != OPJ_FALSE && ( // this condition implies status should be false
    !tainted_codec_private ||
    !tainted_codec_private->is_decompressor.unverified_safe_because("tainted boolean property access") ||
    !(
      (tile_width == 1 && tile_height == 1 && p_j2k->m_cp.tcps[0].m_data != nullptr) ||
      !(j2k_state != J2K_STATE_TPHSOT)
    )
    // non-exhaustive, but further specificity is not O(1) and is excessively complex
  )) {
    sandbox->fail("opj_set_decode_area status");
  }
  */
  OPJ_BOOL status;
#ifdef JP2_STATUS__SET_DECODE_AREA__VERIFIER__TEST
  status = JP2_STATUS__SET_DECODE_AREA__VERIFIER__TEST_VALUE;
#else
  status = status_;
#endif
  if (status != OPJ_FALSE && ( // this condition implies status should be false
    !codec
  )) {
    sandbox->fail("opj_set_decode_area status");
  }

  return status;
}

#define MAXIMUM_VERSION_LENGTH 1000
static const char *opj_version__verifier(std::unique_ptr<const char[]> version_) {
  std::unique_ptr<const char[]> version;
#ifdef OPJ_VERSION__TEST
  version = OPJ_VERSION__TEST_VALUE;
#else
  version = std::move(version_);
#endif

  if (strlen(version.get()) > MAXIMUM_ERROR_MESSAGE_LENGTH) {
    sandbox->fail("version");
  }
  return version.release();
}

static Image *ReadJP2Image(const ImageInfo *image_info,ExceptionInfo *exception)
{
  /**
   * jp2_image  -> tainted_jp2_image
   * image      -> tainted_image
   * jp2_codec  -> tainted_codec
   * parameters -> tainted_params
   * jp2_stram  -> tainted_stream
   * exception  -> tainted_excp
   */

  const char
    *option;

  Image
    *image;

  int
    jp2_status;

  MagickBooleanType
    status;

  opj_dparameters_t
    parameters;

  opj_image_t
    *jp2_image;

  ssize_t
    i;

  ssize_t
    y;

  unsigned char
    sans[4];

  /*
    Open image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickCoreSignature);
  if (IsEventLogging() != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",
      image_info->filename);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickCoreSignature);
  image=AcquireImage(image_info,exception);
  status=OpenBlob(image_info,image,ReadBinaryBlobMode,exception);
  if (status == MagickFalse)
    {
      image=DestroyImageList(image);
      return((Image *) NULL);
    }
  /*
    Initialize JP2 codec.
  */
  if (ReadBlob(image,4,sans) != 4)
    {
      image=DestroyImageList(image);
      return((Image *) NULL);
    }
  (void) SeekBlob(image,SEEK_SET,0);

  tainted<opj_codec_t*, rlbox_wasm2c_sandbox> tainted_codec;

  INIT_SANDBOX()

  if (LocaleCompare(image_info->magick,"JPT") == 0) {
    tainted_codec = sandbox->sb()->invoke_sandbox_function(opj_create_decompress, OPJ_CODEC_JPT);
  } else if (IsJ2K(sans,4) != MagickFalse) {
    tainted_codec = sandbox->sb()->invoke_sandbox_function(opj_create_decompress, OPJ_CODEC_J2K);
  } else {
    tainted_codec = sandbox->sb()->invoke_sandbox_function(opj_create_decompress, OPJ_CODEC_JP2);
  }
  
  tainted_codec.copy_and_verify_address([](uintptr_t p){
    void** vp = (void**) p;
    //printf("codec: %p\n", *vp);
  });
  
  /* XXX: AT THIS POINT, invalid:
     jp2_codec (tainted_codec)
  */
  
  app_pointer<void*, rlbox_wasm2c_sandbox> excp = sandbox->sb()->get_app_pointer(static_cast<void*>(exception));
  tainted<void*, rlbox_wasm2c_sandbox> tainted_excp = excp.to_tainted();
  //printf("Is excp app ptr registered? %d\n", !excp.is_unregistered());

  /* TODO: note that the parameters like tainted_codec can be modified, and as such, reading them in 
     a non-library operation MUST be copied back */
  sandbox->sb()->invoke_sandbox_function(opj_set_warning_handler, tainted_codec, sandbox->warn_cb, tainted_excp);
  sandbox->sb()->invoke_sandbox_function(opj_set_error_handler, tainted_codec, sandbox->error_cb, tainted_excp);

  tainted<opj_dparameters_t*, rlbox_wasm2c_sandbox> tainted_params = sandbox->sb()->malloc_in_sandbox<opj_dparameters_t>();
  sandbox->sb()->invoke_sandbox_function(opj_set_default_decoder_parameters, tainted_params);

  /* XXX: AT THIS POINT, invalid:
     jp2_codec (tainted_codec)
     exception (tainted_excp)
     parameters (tainted_params)
  */

  option=GetImageOption(image_info,"jp2:reduce-factor");
  if (option != (const char *) NULL)
    parameters.cp_reduce=StringToInteger(option);
  option=GetImageOption(image_info,"jp2:quality-layers");
  if (option != (const char *) NULL)
    parameters.cp_layer=StringToInteger(option);

  OPJ_BOOL setup_decoder__status = sandbox->sb()->invoke_sandbox_function(
    opj_setup_decoder, tainted_codec, tainted_params
  ).copy_and_verify([tainted_codec, tainted_params] (OPJ_BOOL status_) {
    OPJ_BOOL status;
#ifdef SETUP_DECODER__STATUS__VERIFIER__TEST
    status = SETUP_DECODER__STATUS__VERIFIER__TEST_VALUE;
#else
    status = status_;
#endif

    // This condition implies status should be false
    if (status != OPJ_FALSE && !tainted_params) {
      sandbox->fail("opj_setup_decoder status");
    }
    return status;
  });

  if (setup_decoder__status == 0) {
    sandbox->sb()->invoke_sandbox_function(opj_destroy_codec, tainted_codec);
    ThrowReaderException(DelegateError,"UnableToManageJP2Stream");
  }

  tainted<opj_stream_t*, rlbox_wasm2c_sandbox> tainted_stream = sandbox->sb()->invoke_sandbox_function(opj_stream_create, OPJ_J2K_STREAM_CHUNK_SIZE, 1);

  sandbox->sb()->invoke_sandbox_function(
    opj_stream_set_read_function, tainted_stream, sandbox->read_cb
  );
  sandbox->sb()->invoke_sandbox_function(
    opj_stream_set_write_function, tainted_stream, sandbox->write_cb
  );
  sandbox->sb()->invoke_sandbox_function(
    opj_stream_set_seek_function, tainted_stream, sandbox->seek_cb
  );
  sandbox->sb()->invoke_sandbox_function(
    opj_stream_set_skip_function, tainted_stream, sandbox->skip_cb
  );

  app_pointer<void*, rlbox_wasm2c_sandbox> ap = sandbox->sb()->get_app_pointer(static_cast<void*>(image));
  tainted<void*, rlbox_wasm2c_sandbox> tainted_image = ap.to_tainted();
  //printf("Is app ptr registered? %d\n", !ap.is_unregistered());

  tainted<opj_stream_free_user_data_fn, rlbox_wasm2c_sandbox> tainted_free_func = NULL;
  sandbox->sb()->invoke_sandbox_function(
    opj_stream_set_user_data, tainted_stream, tainted_image, tainted_free_func
  );
  global__initial_user_data_length = GetBlobSize(image); // This value will be used in verifiers
  global__use_initial_user_data_length = true;
  sandbox->sb()->invoke_sandbox_function(
    opj_stream_set_user_data_length, tainted_stream, global__initial_user_data_length // should be safe, set_user_data ddoesn't modify anything
  );

  tainted<opj_image_t**, rlbox_wasm2c_sandbox> tainted_jp2_image_ptr = sandbox->sb()->malloc_in_sandbox<opj_image_t*>();

  OPJ_BOOL read_header__status = sandbox->sb()->invoke_sandbox_function(
    opj_read_header, tainted_stream, tainted_codec, tainted_jp2_image_ptr
  ).copy_and_verify([tainted_codec, tainted_stream] (OPJ_BOOL status_) {
    OPJ_BOOL status;
#ifdef READ_HEADER__STATUS__VERIFIER__TEST
    status = READ_HEADER__STATUS__VERIFIER__TEST_VALUE;
#else
    status = status_;
#endif

    if (status != OPJ_FALSE && !tainted_stream) {
      sandbox->fail("opj_setup_decoder status");
    }
    return status;
  });

  if (read_header__status == 0)
  {
    sandbox->sb()->invoke_sandbox_function(
      opj_stream_destroy, tainted_stream
    );
    sandbox->sb()->invoke_sandbox_function(
      opj_destroy_codec, tainted_codec
    );
    ThrowReaderException(DelegateError,"UnableToDecodeImageFile");
  }

  tainted<opj_image_t*, rlbox_wasm2c_sandbox> tainted_jp2_image = *tainted_jp2_image_ptr;

  jp2_status=OPJ_TRUE;

  // read-only use so its safe
  opj_image_comp_t* comp0 = (tainted_jp2_image->comps+0).copy_and_verify(jp2_comp__verifier);
    
  if ((AcquireMagickResource(WidthResource,(size_t) comp0->w) == MagickFalse) ||
      (AcquireMagickResource(HeightResource,(size_t) comp0->h) == MagickFalse))
  {
    sandbox->sb()->invoke_sandbox_function(
      opj_stream_destroy, tainted_stream
    );
    sandbox->sb()->invoke_sandbox_function(
      opj_destroy_codec, tainted_codec
    );
    sandbox->sb()->invoke_sandbox_function(
      opj_image_destroy, tainted_jp2_image
    );
    ThrowReaderException(DelegateError,"UnableToDecodeImageFile");
  }

  if (image->ping == MagickFalse)
  {
    if ((image->columns != 0) && (image->rows != 0))
    {
      /*
        Extract an area from the image.
      */
      
      //printf("At line %d\n", __LINE__);

      jp2_status = sandbox->sb()->invoke_sandbox_function(
        opj_set_decode_area, tainted_codec, tainted_jp2_image,
        (OPJ_INT32) image->extract_info.x,
        (OPJ_INT32) image->extract_info.y,
        (OPJ_INT32) (image->extract_info.x + (ssize_t) image->columns),
        (OPJ_INT32) (image->extract_info.y + (ssize_t) image->rows)
      ).copy_and_verify([tainted_codec] (OPJ_BOOL status) {
        return jp2_status__set_decode_area__verifier(status, tainted_codec);
      });

    } else {
      //printf("At line %d\n", __LINE__);

      jp2_status = sandbox->sb()->invoke_sandbox_function(
        opj_set_decode_area, tainted_codec, tainted_jp2_image, 0, 0,
        comp0->w, comp0->h
      ).copy_and_verify([tainted_codec] (OPJ_BOOL status) {
        return jp2_status__set_decode_area__verifier(status, tainted_codec);
      });
    }

    if (jp2_status == OPJ_FALSE)
    {
      sandbox->sb()->invoke_sandbox_function(
        opj_stream_destroy, tainted_stream
      );
      sandbox->sb()->invoke_sandbox_function(
        opj_destroy_codec, tainted_codec
      );
      sandbox->sb()->invoke_sandbox_function(
        opj_image_destroy, tainted_jp2_image
      );
      ThrowReaderException(DelegateError,"UnableToDecodeImageFile");
    }
  }
  comp0 = NULL; // TODO: free this properly

  if ((image_info->number_scenes != 0) && (image_info->scene != 0)) {
    //printf("At line %d\n", __LINE__);
    jp2_status = sandbox->sb()->invoke_sandbox_function(
      opj_get_decoded_tile, tainted_codec, tainted_stream, tainted_jp2_image,
      (unsigned int) image_info->scene - 1
    ).copy_and_verify([tainted_codec, tainted_stream] (OPJ_BOOL status_) {
      OPJ_BOOL status;
#ifdef JP2_STATUS__OPJ_GET_DECODED_TILE__VERIFIER__TEST
      status = JP2_STATUS__OPJ_GET_DECODED_TILE__VERIFIER__TEST_VALUE;
#else
      status = status_;
#endif

      if (status != OPJ_FALSE && !tainted_stream) {
        sandbox->fail("opj_get_decoded_tile status");
      }
      return status;
    });
  }
  else
    if (image->ping == MagickFalse)
    {
      //printf("At line %d\n", __LINE__);
      jp2_status = sandbox->sb()->invoke_sandbox_function(
        opj_decode, tainted_codec, tainted_stream, tainted_jp2_image
      ).copy_and_verify([tainted_codec, tainted_stream] (OPJ_BOOL status_) {
        OPJ_BOOL status;
#ifdef JP2_STATUS__OPJ_DECODE__VERIFIER__TEST
        status = JP2_STATUS__OPJ_DECODE__VERIFIER__TEST_VALUE;
#else
        status = status_;
#endif

        if (status != OPJ_FALSE && !tainted_stream) {
          sandbox->fail("opj_decode status");
        }
        return status;
      });
  
      //printf("At line %d\n", __LINE__);
      if (jp2_status != OPJ_FALSE) {
        jp2_status = sandbox->sb()->invoke_sandbox_function(
          opj_end_decompress, tainted_codec, tainted_stream
        ).copy_and_verify([tainted_codec, tainted_stream] (OPJ_BOOL status_) {
          OPJ_BOOL status;
#ifdef JP2_STATUS__OPJ_END_DECOMPRESS__VERIFIER__TEST
          status = JP2_STATUS__OPJ_END_DECOMPRESS__VERIFIER__TEST_VALUE;
#else
          status = status_;
#endif

          if (status != OPJ_FALSE && !tainted_stream) {
            sandbox->fail("opj_end_decompress status");
          }
          return status;
        });
      }
    }
  if (jp2_status == OPJ_FALSE)
  {
    sandbox->sb()->invoke_sandbox_function(
      opj_stream_destroy, tainted_stream
    );
    sandbox->sb()->invoke_sandbox_function(
      opj_destroy_codec, tainted_codec
    );
    //printf("At line %d\n", __LINE__);
    sandbox->sb()->invoke_sandbox_function(
      opj_image_destroy, tainted_jp2_image
    );
    ThrowReaderException(DelegateError,"UnableToDecodeImageFile");
  }
  //printf("At line %d\n", __LINE__);
  sandbox->sb()->invoke_sandbox_function(
    opj_stream_destroy, tainted_stream
  );

  comp0 = NULL;
  if (tainted_jp2_image->numcomps.copy_and_verify(jp2_img_numcomps__verifier) > 0) {
    comp0 = (tainted_jp2_image->comps+0).copy_and_verify(jp2_comp__verifier);
  }

  for (i=0; i < (ssize_t) tainted_jp2_image->numcomps.copy_and_verify(jp2_img_numcomps__verifier); i++)
  {
    opj_image_comp_t* comp_i = (tainted_jp2_image->comps+i).copy_and_verify(jp2_comp__verifier);
    //printf("comps[i].dx: %u\n", comp_i->dx);

    if ((comp_i->dx == 0) || (comp_i->dy == 0) ||
        (comp0->prec != comp_i->prec) ||
        (comp0->prec > 64) ||
        (comp0->sgnd != comp_i->sgnd) ||
        ((image->ping == MagickFalse) && (comp_i->data == NULL)))
    {
      sandbox->sb()->invoke_sandbox_function(
        opj_destroy_codec, tainted_codec
      );
      sandbox->sb()->invoke_sandbox_function(
        opj_image_destroy, tainted_jp2_image
      );
      ThrowReaderException(CoderError,"IrregularChannelGeometryNotSupported")
    }

    // TODO: free comp_i
    comp_i = NULL;
  }
  
  //printf("At line %d\n", __LINE__);

  /*
    Convert JP2 image.
  */

  // XXX: guaranteed that jp2_image does not change in application code.
  jp2_image = tainted_jp2_image.copy_and_verify(jp2_image__verifier);

  image->columns= (size_t) jp2_image->comps[0].w;
  image->rows=(size_t) jp2_image->comps[0].h;
  image->depth= jp2_image->comps[0].prec;
  image->compression=JPEG2000Compression;
  if (jp2_image->numcomps == 1)
    SetImageColorspace(image,GRAYColorspace,exception);
  else
    if (jp2_image->color_space == 2)
      {
        SetImageColorspace(image,GRAYColorspace,exception);
        if (jp2_image->numcomps > 1)
          image->alpha_trait=BlendPixelTrait;
      }
    else
      if (jp2_image->color_space == 3)
        SetImageColorspace(image,Rec601YCbCrColorspace,exception);
  
  //printf("At line %d\n", __LINE__);

  if (jp2_image->numcomps > 3)
    image->alpha_trait=BlendPixelTrait;
  if (jp2_image->icc_profile_buf != (unsigned char *) NULL)
    {
      StringInfo
        *profile;

      profile=BlobToStringInfo(jp2_image->icc_profile_buf,
        jp2_image->icc_profile_len);
      if (profile != (StringInfo *) NULL)
        {
          SetImageProfile(image,"icc",profile,exception);
          profile=DestroyStringInfo(profile);
        }
    }
  if (image->ping != MagickFalse)
    {
      sandbox->sb()->invoke_sandbox_function(
        opj_destroy_codec, tainted_codec
      );
      sandbox->sb()->invoke_sandbox_function(
        opj_image_destroy, tainted_jp2_image
      );
      return(GetFirstImageInList(image));
    }
  status=SetImageExtent(image,image->columns,image->rows,exception);
  //printf("At line %d\n", __LINE__);
  if (status == MagickFalse)
  {
    sandbox->sb()->invoke_sandbox_function(
      opj_destroy_codec, tainted_codec
    );
    sandbox->sb()->invoke_sandbox_function(
      opj_image_destroy, tainted_jp2_image
    );
    return(DestroyImageList(image));
  }

  //printf("At line %d\n", __LINE__);
  // TODO: is this necessary, need to free old one?
  jp2_image = tainted_jp2_image.copy_and_verify(jp2_image__verifier);

  for (y=0; y < (ssize_t) image->rows; y++)
  {
    Quantum
      *magick_restrict q;

    ssize_t
      x;

    q=GetAuthenticPixels(image,0,y,image->columns,1,exception);
    if (q == (Quantum *) NULL)
      break;
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      for (i=0; i < (ssize_t) jp2_image->numcomps; i++)
      {
        double
          pixel,
          scale;

        ssize_t
          index,
          pad;

        pad=image->columns % jp2_image->comps[i].dx;
        index=y/jp2_image->comps[i].dy*(image->columns+pad)/
        jp2_image->comps[i].dx+x/jp2_image->comps[i].dx;
        if ((index < 0) ||
            (index >= (jp2_image->comps[i].h*jp2_image->comps[i].w)))
          {
            sandbox->sb()->invoke_sandbox_function(
              opj_destroy_codec, tainted_codec
            );
            sandbox->sb()->invoke_sandbox_function(
              opj_image_destroy, tainted_jp2_image
            );
            ThrowReaderException(CoderError,
              "IrregularChannelGeometryNotSupported")
          }
        scale=QuantumRange/(double) ((1UL << jp2_image->comps[i].prec)-1);
        pixel=scale*(jp2_image->comps[i].data[index]+
          (jp2_image->comps[i].sgnd ? 1UL << (jp2_image->comps[i].prec-1) : 0));
        switch (i)
        {
           case 0:
           {
             if (jp2_image->numcomps == 1)
               {
                 SetPixelGray(image,ClampToQuantum(pixel),q);
                 SetPixelAlpha(image,OpaqueAlpha,q);
                 break;
               }
             SetPixelRed(image,ClampToQuantum(pixel),q);
             SetPixelGreen(image,ClampToQuantum(pixel),q);
             SetPixelBlue(image,ClampToQuantum(pixel),q);
             SetPixelAlpha(image,OpaqueAlpha,q);
             break;
           }
           case 1:
           {
             if (jp2_image->numcomps == 2)
               {
                 SetPixelAlpha(image,ClampToQuantum(pixel),q);
                 break;
               }
             SetPixelGreen(image,ClampToQuantum(pixel),q);
             break;
           }
           case 2:
           {
             SetPixelBlue(image,ClampToQuantum(pixel),q);
             break;
           }
           case 3:
           {
             SetPixelAlpha(image,ClampToQuantum(pixel),q);
             break;
           }
        }
      }
      q+=GetPixelChannels(image);
    }
    if (SyncAuthenticPixels(image,exception) == MagickFalse)
      break;
    status=SetImageProgress(image,LoadImageTag,(MagickOffsetType) y,
      image->rows);
    if (status == MagickFalse)
      break;
  }

  //printf("At line %d\n", __LINE__);

  /*
    Free resources.
  */
  sandbox->sb()->invoke_sandbox_function(
    opj_destroy_codec, tainted_codec
  );
  sandbox->sb()->invoke_sandbox_function(
    opj_image_destroy, tainted_jp2_image
  );
  (void) CloseBlob(image);
  if ((image_info->number_scenes != 0) && (image_info->scene != 0))
    AppendImageToList(&image,CloneImage(image,0,0,MagickTrue,exception));
  return(GetFirstImageInList(image));
}
#endif

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e g i s t e r J P 2 I m a g e                                           %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  RegisterJP2Image() adds attributes for the JP2 image format to the list of
%  supported formats.  The attributes include the image format tag, a method
%  method to read and/or write the format, whether the format supports the
%  saving of more than one frame to the same file or blob, whether the format
%  supports native in-memory I/O, and a brief description of the format.
%
%  The format of the RegisterJP2Image method is:
%
%      size_t RegisterJP2Image(void)
%
*/
ModuleExport size_t RegisterJP2Image(void)
{
  char
    version[MagickPathExtent];

  MagickInfo
    *entry;

  *version='\0';
#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)

  INIT_SANDBOX()
  const char* vstr = sandbox->sb()->invoke_sandbox_function(opj_version).copy_and_verify_string([](std::unique_ptr<const char[]> val){
    return opj_version__verifier(std::move(val));
  });
  (void) FormatLocaleString(version,MagickPathExtent,"%s",vstr);
#endif
  entry=AcquireMagickInfo("JP2","JP2","JPEG-2000 File Format Syntax");
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->mime_type=ConstantString("image/jp2");
  entry->magick=(IsImageFormatHandler *) IsJP2;
  entry->flags^=CoderAdjoinFlag;
  entry->flags|=CoderDecoderSeekableStreamFlag;
  entry->flags|=CoderEncoderSeekableStreamFlag;
#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadJP2Image;
  entry->encoder=(EncodeImageHandler *) WriteJP2Image;
#endif
  (void) RegisterMagickInfo(entry);
  entry=AcquireMagickInfo("JP2","J2C","JPEG-2000 Code Stream Syntax");
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->mime_type=ConstantString("image/jp2");
  entry->magick=(IsImageFormatHandler *) IsJ2K;
  entry->flags^=CoderAdjoinFlag;
  entry->flags|=CoderDecoderSeekableStreamFlag;
  entry->flags|=CoderEncoderSeekableStreamFlag;
#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadJP2Image;
  entry->encoder=(EncodeImageHandler *) WriteJP2Image;
#endif
  (void) RegisterMagickInfo(entry);
  entry=AcquireMagickInfo("JP2","J2K","JPEG-2000 Code Stream Syntax");
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->mime_type=ConstantString("image/jp2");
  entry->magick=(IsImageFormatHandler *) IsJ2K;
  entry->flags^=CoderAdjoinFlag;
  entry->flags|=CoderDecoderSeekableStreamFlag;
  entry->flags|=CoderEncoderSeekableStreamFlag;
#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadJP2Image;
  entry->encoder=(EncodeImageHandler *) WriteJP2Image;
#endif
  (void) RegisterMagickInfo(entry);
  entry=AcquireMagickInfo("JP2","JPM","JPEG-2000 File Format Syntax");
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->mime_type=ConstantString("image/jp2");
  entry->magick=(IsImageFormatHandler *) IsJP2;
  entry->flags^=CoderAdjoinFlag;
  entry->flags|=CoderDecoderSeekableStreamFlag;
  entry->flags|=CoderEncoderSeekableStreamFlag;
#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadJP2Image;
  entry->encoder=(EncodeImageHandler *) WriteJP2Image;
#endif
  (void) RegisterMagickInfo(entry);
  entry=AcquireMagickInfo("JP2","JPT","JPEG-2000 File Format Syntax");
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->mime_type=ConstantString("image/jp2");
  entry->magick=(IsImageFormatHandler *) IsJP2;
  entry->flags^=CoderAdjoinFlag;
  entry->flags|=CoderDecoderSeekableStreamFlag;
  entry->flags|=CoderEncoderSeekableStreamFlag;
#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadJP2Image;
  entry->encoder=(EncodeImageHandler *) WriteJP2Image;
#endif
  (void) RegisterMagickInfo(entry);
  entry=AcquireMagickInfo("JP2","JPC","JPEG-2000 Code Stream Syntax");
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->mime_type=ConstantString("image/jp2");
  entry->magick=(IsImageFormatHandler *) IsJP2;
  entry->flags^=CoderAdjoinFlag;
  entry->flags|=CoderDecoderSeekableStreamFlag;
  entry->flags|=CoderEncoderSeekableStreamFlag;
#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadJP2Image;
  entry->encoder=(EncodeImageHandler *) WriteJP2Image;
#endif
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   U n r e g i s t e r J P 2 I m a g e                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  UnregisterJP2Image() removes format registrations made by the JP2 module
%  from the list of supported formats.
%
%  The format of the UnregisterJP2Image method is:
%
%      UnregisterJP2Image(void)
%
*/
ModuleExport void UnregisterJP2Image(void)
{
  (void) UnregisterMagickInfo("JPC");
  (void) UnregisterMagickInfo("JPT");
  (void) UnregisterMagickInfo("JPM");
  (void) UnregisterMagickInfo("JP2");
  (void) UnregisterMagickInfo("J2K");
}

#if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)
/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e J P 2 I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  WriteJP2Image() writes an image in the JPEG 2000 image format.
%
%  JP2 support originally written by Nathan Brown, nathanbrown@letu.edu
%
%  The format of the WriteJP2Image method is:
%
%      MagickBooleanType WriteJP2Image(const ImageInfo *image_info,Image *image,
%        ExceptionInfo *exception)
%
%  A description of each parameter follows.
%
%    o image_info: the image info.
%
%    o image:  The image.
%
*/

static void CinemaProfileCompliance(
  tainted<opj_image_t*, rlbox_wasm2c_sandbox> jp2_image,
  tainted<opj_cparameters_t*, rlbox_wasm2c_sandbox> parameters)
{
  opj_image_comp_t* comp0 = (jp2_image->comps+0).copy_and_verify(jp2_comp__verifier);

  /*
    Digital Cinema 4K profile compliant codestream.
  */
  parameters->tile_size_on=OPJ_FALSE;
  parameters->cp_tdx=1;
  parameters->cp_tdy=1;
  parameters->tp_flag='C';
  parameters->tp_on=1;
  parameters->cp_tx0=0;
  parameters->cp_ty0=0;
  parameters->image_offset_x0=0;
  parameters->image_offset_y0=0;
  parameters->cblockw_init=32;
  parameters->cblockh_init=32;
  parameters->csty|=0x01;
  parameters->prog_order=OPJ_CPRL;
  parameters->roi_compno=(-1);
  parameters->subsampling_dx=1;
  parameters->subsampling_dy=1;
  parameters->irreversible=1;

  if ((comp0->w == 2048) || (comp0->h == 1080))
    {
      /*
        Digital Cinema 2K.
      */
      parameters->cp_cinema=OPJ_CINEMA2K_24;
      parameters->cp_rsiz=OPJ_CINEMA2K;
      parameters->max_comp_size=1041666;
      if (parameters->numresolution.copy_and_verify([](int nr){ return nr; }) > 6)
        parameters->numresolution=6;

    }
  if ((comp0->w == 4096) || (comp0->h == 2160))
    {
      /*
        Digital Cinema 4K.
      */
      parameters->cp_cinema=OPJ_CINEMA4K_24;
      parameters->cp_rsiz=OPJ_CINEMA4K;
      parameters->max_comp_size=1041666;
      if (parameters->numresolution.copy_and_verify([](int nr){ return nr; }) < 1)
        parameters->numresolution=1;
      if (parameters->numresolution.copy_and_verify([](int nr){ return nr; }) > 7)
        parameters->numresolution=7;
      parameters->numpocs=2;

      tainted<opj_poc_t*, rlbox_wasm2c_sandbox> pocs = sandbox_reinterpret_cast<opj_poc_t*>(sizeof(OPJ_PROG_ORDER)+&(parameters->prog_order));

      pocs[0].tile=1;
      pocs[0].resno0=0;
      pocs[0].compno0=0;
      pocs[0].layno1=1;
      pocs[0].resno1=parameters->numresolution-1;
      pocs[0].compno1=3;
      pocs[0].prg1=OPJ_CPRL;
      pocs[1].tile=1;
      pocs[1].resno0=parameters->numresolution-1;
      pocs[1].compno0=0;
      pocs[1].layno1=1;
      pocs[1].resno1=parameters->numresolution;
      pocs[1].compno1=3;
      pocs[1].prg1=OPJ_CPRL;
    }
  parameters->tcp_numlayers=1;
  parameters->tcp_rates[0]=((float) (jp2_image->numcomps.copy_and_verify(jp2_img_numcomps__verifier) * comp0->w * comp0->h * comp0->prec)) / (parameters->max_comp_size.copy_and_verify([](int mcs){ return mcs; }) * 8 * comp0->dx * comp0->dy);
  parameters->cp_disto_alloc=1;
}

static inline int CalculateNumResolutions(size_t width,size_t height)
{
  int
    i;

  for (i=1; i < 6; i++)
    if ((width < ((size_t) 1UL << i)) || (height < ((size_t) 1UL << i)))
      break;
  return(i);
}

static int jp2_cparam_dxy__verifier(int dv_) {
  int dv;
#ifdef JP2_CPARAM_DXY__VERIFIER__TEST
  dv = JP2_CPARAM_DXY__VERIFIER__TEST_VALUE;
#else
  dv = dv_;
#endif
  //printf("Encountered dv: %d\n", dv_);

  if (dv < -1 || dv == 0) {
    sandbox->fail("subsampling_d[xy]");
  }
  return dv;
}

static int jp2_cparam_offset_xy__verifier(int offset_) {
  int offset;
#ifdef JP2_CPARAM_OFFSET_DXY__VERIFIER__TEST
  offset = JP2_CPARAM_OFFSET_DXY__VERIFIER__TEST_VALUE;
#else
  offset = offset_;
#endif

  return offset;
}

static opj_cparameters_t* jp2_cparameters__verifier(opj_cparameters_t* params_) {
  opj_cparameters_t* params;
#ifdef JP2_CPARAMETERS__VERIFIER__TEST
  params = JP2_CPARAMETERS__VERIFIER__TEST_VALUE;
#else
  params = params_;
#endif

  bool fine = params->numpocs < 32 &&
              params->tcp_numlayers < 100 &&
                (params->prog_order == OPJ_PROG_UNKNOWN || params->prog_order == OPJ_LRCP ||
                 params->prog_order == OPJ_RLCP || params->prog_order == OPJ_RPCL ||
                 params->prog_order == OPJ_PCRL || params->prog_order == OPJ_CPRL) &&
              params->max_comp_size >= 0 &&
              params->max_cs_size >= 0;
  if (!fine) {
    sandbox->fail("opj_cparameters_t");
  }
  return params;
}

static OPJ_BOOL jp2_bool__verifier(OPJ_BOOL b_) {
  int b;
#ifdef JP2_BOOL__VERIFIER__TEST
  b = JP2_BOOL__VERIFIER__TEST_VALUE;
#else
  b = b_;
#endif

  if (b != 0 && b != 1) {
    sandbox->fail("boolean");
  }
  return b;
}

static MagickBooleanType WriteJP2Image(const ImageInfo *image_info,Image *image,
  ExceptionInfo *exception)
{
  const char
    *option,
    *property;

  int
    jp2_status;

  MagickBooleanType
    status;

  OPJ_COLOR_SPACE
    jp2_colorspace;

  opj_cparameters_t
    *raw_parameters;

  opj_image_cmptparm_t
    jp2_info[5];

  opj_image_t
    *jp2_image__raw;

  ssize_t
    i;

  ssize_t
    y;

  unsigned int
    channels;

  INIT_SANDBOX()

  /*
    Open image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickCoreSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickCoreSignature);
  status=OpenBlob(image_info,image,WriteBinaryBlobMode,exception);
  if (status == MagickFalse)
    return(status);
  /*
    Initialize JPEG 2000 API.
  */

  tainted<opj_cparameters_t*, rlbox_wasm2c_sandbox> parameters = sandbox->sb()->malloc_in_sandbox<opj_cparameters_t>();
  sandbox->sb()->invoke_sandbox_function(opj_set_default_encoder_parameters, parameters);

  option=GetImageOption(image_info,"jp2:number-resolutions");
  if (option != (const char *) NULL)
    parameters->numresolution=StringToInteger(option);
  else
    parameters->numresolution=CalculateNumResolutions(image->columns,
      image->rows);
  parameters->tcp_numlayers=1;
  parameters->tcp_rates[0]=0.0;  /* lossless */
  parameters->cp_disto_alloc=1;
  if ((image_info->quality != 0) && (image_info->quality != 100))
    {
      parameters->tcp_distoratio[0]=(float) image_info->quality;
      parameters->cp_fixed_quality=OPJ_TRUE;
      parameters->cp_disto_alloc=0;
    }
  if (image_info->extract != (char *) NULL)
    {
      RectangleInfo
        geometry;

      int
        flags;

      /*
        Set tile size.
      */
      (void) memset(&geometry,0,sizeof(geometry));
      flags=ParseAbsoluteGeometry(image_info->extract,&geometry);
      parameters->cp_tdx=(int) geometry.width;
      parameters->cp_tdy=(int) geometry.width;
      if ((flags & HeightValue) != 0)
        parameters->cp_tdy=(int) geometry.height;
      if ((flags & XValue) != 0)
        parameters->cp_tx0=geometry.x;
      if ((flags & YValue) != 0)
        parameters->cp_ty0=geometry.y;
      parameters->tile_size_on=OPJ_TRUE;
      parameters->numresolution=CalculateNumResolutions(
        parameters->cp_tdx.copy_and_verify([](int dx){ return dx; }),
        parameters->cp_tdy.copy_and_verify([](int dy){ return dy; })
      );
    }
  option=GetImageOption(image_info,"jp2:quality");
  if (option != (const char *) NULL)
    {
      const char
        *p;

      /*
        Set quality PSNR.
      */
      p=option;
      float tcp_dist_buf;
      for (i=0; sscanf(p,"%f", &tcp_dist_buf) == 1; i++)
      {
        // TODO: is this safe- does the scanf execute if and only if this does?
        parameters->tcp_distoratio[i] = tcp_dist_buf;

        if (i > 100)
          break;
        while ((*p != '\0') && (*p != ','))
          p++;
        if (*p == '\0')
          break;
        p++;
      }
      parameters->tcp_numlayers=i+1;
      parameters->cp_fixed_quality=OPJ_TRUE;
      parameters->cp_disto_alloc=0;
    }
  option=GetImageOption(image_info,"jp2:progression-order");
  if (option != (const char *) NULL)
    {
      if (LocaleCompare(option,"LRCP") == 0)
        parameters->prog_order=OPJ_LRCP;
      if (LocaleCompare(option,"RLCP") == 0)
        parameters->prog_order=OPJ_RLCP;
      if (LocaleCompare(option,"RPCL") == 0)
        parameters->prog_order=OPJ_RPCL;
      if (LocaleCompare(option,"PCRL") == 0)
        parameters->prog_order=OPJ_PCRL;
      if (LocaleCompare(option,"CPRL") == 0)
        parameters->prog_order=OPJ_CPRL;
    }
  option=GetImageOption(image_info,"jp2:rate");
  if (option != (const char *) NULL)
    {
      const char
        *p;

      /*
        Set compression rate.
      */
      p=option;
      float tcp_rate_buf;
      for (i=0; sscanf(p,"%f", &tcp_rate_buf) == 1; i++)
      {
        // TODO: same, is this safe?
        parameters->tcp_rates[i] = tcp_rate_buf;
        if (i >= 100)
          break;
        while ((*p != '\0') && (*p != ','))
          p++;
        if (*p == '\0')
          break;
        p++;
      }
      parameters->tcp_numlayers=i+1;
      parameters->cp_disto_alloc=OPJ_TRUE;
    }
  if (image_info->sampling_factor != (const char *) NULL) {
    int dx_buf;
    int dy_buf;

    (void) sscanf(image_info->sampling_factor,"%d:%d", &dx_buf, &dy_buf);
    //printf("before: %d %d\n", parameters->subsampling_dx.copy_and_verify(jp2_cparam_dxy__verifier), parameters->subsampling_dy.copy_and_verify(jp2_cparam_dxy__verifier));
    //printf("read dx: %d, dy: %d\n", dx_buf, dy_buf);
    parameters->subsampling_dx = dx_buf;
    parameters->subsampling_dy = dy_buf;
  }
  property=GetImageProperty(image,"comment",exception);
  if (property != (const char *) NULL) {
    tainted<char*, rlbox_wasm2c_sandbox> prop_sbx = sandbox->sb()->malloc_in_sandbox<char>(strlen(property));
    std::strncpy(prop_sbx.unverified_safe_pointer_because(strlen(property), "Writing a string."), property, strlen(property));
    parameters->cp_comment= prop_sbx;
  }

  channels=3;
  jp2_colorspace=OPJ_CLRSPC_SRGB;
  if (image->colorspace == YUVColorspace)
    {
      jp2_colorspace=OPJ_CLRSPC_SYCC;
      parameters->subsampling_dx=2;
    }
  else
    {
      if (IsGrayColorspace(image->colorspace) != MagickFalse)
        {
          channels=1;
          jp2_colorspace=OPJ_CLRSPC_GRAY;
        }
      else
        if (IssRGBCompatibleColorspace(image->colorspace) == MagickFalse)
          (void) TransformImageColorspace(image,sRGBColorspace,exception);
      if (image->alpha_trait != UndefinedPixelTrait)
        channels++;
    }
  parameters->tcp_mct= (channels == 3);
  memset(jp2_info,0,sizeof(jp2_info));
  for (i=0; i < (ssize_t) channels; i++)
  {
    jp2_info[i].prec=(OPJ_UINT32) image->depth;
    jp2_info[i].bpp=(OPJ_UINT32) image->depth;
    if ((image->depth == 1) &&
        ((LocaleCompare(image_info->magick,"JPT") == 0) ||
         (LocaleCompare(image_info->magick,"JP2") == 0)))
      {
        jp2_info[i].prec++;  /* OpenJPEG returns exception for depth @ 1 */
        jp2_info[i].bpp++;
      }
    jp2_info[i].sgnd=0;
    jp2_info[i].dx=parameters->subsampling_dx.copy_and_verify(jp2_cparam_dxy__verifier);
    jp2_info[i].dy=parameters->subsampling_dy.copy_and_verify(jp2_cparam_dxy__verifier);
    jp2_info[i].w=(OPJ_UINT32) image->columns;
    jp2_info[i].h=(OPJ_UINT32) image->rows;
  }

  tainted<opj_image_cmptparm_t*, rlbox_wasm2c_sandbox> tainted_info = sandbox->sb()->malloc_in_sandbox<opj_image_cmptparm_t>(5);
  // OK, since only primitives.
  memcpy(tainted_info.unverified_safe_pointer_because(sizeof(jp2_info), "Memcpying to the info object"), 
    &jp2_info, sizeof(jp2_info));

  // TODO: need to use untaint and taint img here.
  tainted<opj_image_t*, rlbox_wasm2c_sandbox> jp2_image = sandbox->sb()->invoke_sandbox_function(opj_image_create, (OPJ_UINT32) channels, tainted_info, jp2_colorspace);

  if (jp2_image == nullptr)
  {
    ThrowWriterException(DelegateError,"UnableToEncodeImageFile");
  }

  jp2_image->x0 = parameters->image_offset_x0.copy_and_verify(jp2_cparam_offset_xy__verifier);
  jp2_image->y0 = parameters->image_offset_y0.copy_and_verify(jp2_cparam_offset_xy__verifier);
  jp2_image->x1 = (unsigned int) (2*parameters->image_offset_x0.copy_and_verify(jp2_cparam_offset_xy__verifier) + (image->columns-1)* (parameters->subsampling_dx.copy_and_verify(jp2_cparam_dxy__verifier))+1);
  jp2_image->y1 = (unsigned int) (2*parameters->image_offset_y0.copy_and_verify(jp2_cparam_offset_xy__verifier) + (image->rows-1)* (parameters->subsampling_dx.copy_and_verify(jp2_cparam_dxy__verifier))+1);
  if ((image->depth == 12) &&
      ((image->columns == 2048) || (image->rows == 1080) ||
       (image->columns == 4096) || (image->rows == 2160)))
    CinemaProfileCompliance(jp2_image,parameters);
  if (channels == 4)
    jp2_image->comps[3].alpha=1;
  else
   if ((channels == 2) && (jp2_colorspace == OPJ_CLRSPC_GRAY))
     jp2_image->comps[1].alpha=1;

  /*
    Convert to JP2 pixels.
  */
  opj_image_comp_t** channel_infos = (opj_image_comp_t**) calloc(channels, sizeof(opj_image_comp_t*));
  for (int i = 0; i<channels; ++i) {
    channel_infos[i] = (jp2_image->comps + i).copy_and_verify(jp2_comp__verifier);
  }

  for (y=0; y < (ssize_t) image->rows; y++)
  {
    const Quantum
      *p;

    ssize_t
      x;

    p=GetVirtualPixels(image,0,y,image->columns,1,exception);
    if (p == (const Quantum *) NULL)
      break;
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      for (i=0; i < (ssize_t) channels; i++)
      {
        opj_image_comp_t* comp_i = channel_infos[i];

        double
          scale;

        int
          *q;

        scale=(double) (((size_t) 1UL << comp_i->prec)-1)/
          QuantumRange;
        q=comp_i->data+(ssize_t) (y*PerceptibleReciprocal(
          comp_i->dy)*image->columns*PerceptibleReciprocal(
          comp_i->dx)+x*PerceptibleReciprocal(
          comp_i->dx));
        switch (i)
        {
          case 0:
          {
            if (jp2_colorspace == OPJ_CLRSPC_GRAY)
              {
                *q=(int) (scale*GetPixelGray(image,p));
                break;
              }
            *q=(int) (scale*GetPixelRed(image,p));
            break;
          }
          case 1:
          {
            if (jp2_colorspace == OPJ_CLRSPC_GRAY)
              {
                *q=(int) (scale*GetPixelAlpha(image,p));
                break;
              }
            *q=(int) (scale*GetPixelGreen(image,p));
            break;
          }
          case 2:
          {
            *q=(int) (scale*GetPixelBlue(image,p));
            break;
          }
          case 3:
          {
            *q=(int) (scale*GetPixelAlpha(image,p));
            break;
          }
        }
      }
      p+=GetPixelChannels(image);
    }
    status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
      image->rows);
    if (status == MagickFalse)
      break;
  }

  // TODO: free comp_i's
  for (int i = 0; i<channels; ++i) {
    // copy back data- only field that changes in this loop.
    opj_image_comp_t* comp = channel_infos[i];
    size_t bytes_to_copy = sizeof(OPJ_INT32)*(comp->w)*(comp->h);
    memcpy(jp2_image->comps[i].data.unverified_safe_pointer_because(bytes_to_copy, "Copying data bytes to comp"), comp->data, bytes_to_copy); 
  }
  
  tainted<opj_codec_t*, rlbox_wasm2c_sandbox> tainted_codec;
  if (LocaleCompare(image_info->magick,"JPT") == 0) {
    tainted_codec = sandbox->sb()->invoke_sandbox_function(opj_create_compress, OPJ_CODEC_JPT);
  } else if (LocaleCompare(image_info->magick, "J2K") == 0) {
    tainted_codec = sandbox->sb()->invoke_sandbox_function(opj_create_compress, OPJ_CODEC_J2K);
  } else {
    tainted_codec = sandbox->sb()->invoke_sandbox_function(opj_create_compress, OPJ_CODEC_JP2);
  }
  
  tainted_codec.copy_and_verify_address([](uintptr_t p){
    void** vp = (void**) p;
    //printf("codec: %p\n", *vp);
  });
  
  app_pointer<void*, rlbox_wasm2c_sandbox> excp = sandbox->sb()->get_app_pointer(static_cast<void*>(exception));
  tainted<void*, rlbox_wasm2c_sandbox> tainted_excp = excp.to_tainted();
  //printf("Is excp app ptr registered? %d\n", !excp.is_unregistered());
  
  sandbox->sb()->invoke_sandbox_function(
    opj_set_warning_handler, tainted_codec, sandbox->warn_cb, tainted_excp
  );
  sandbox->sb()->invoke_sandbox_function(
    opj_set_error_handler, tainted_codec, sandbox->error_cb, tainted_excp
  );

  sandbox->sb()->invoke_sandbox_function(
    opj_setup_encoder, tainted_codec, parameters, jp2_image
  );

  tainted<opj_stream_t*, rlbox_wasm2c_sandbox> tainted_stream = sandbox->sb()->invoke_sandbox_function(
    opj_stream_create, OPJ_J2K_STREAM_CHUNK_SIZE, OPJ_FALSE
  );

  if (tainted_stream == nullptr) 
  {
    sandbox->sb()->invoke_sandbox_function(opj_destroy_codec, tainted_codec);
    sandbox->sb()->invoke_sandbox_function(opj_image_destroy, jp2_image);
    ThrowWriterException(DelegateError,"UnableToEncodeImageFile");
  }
  
  tainted_codec.copy_and_verify_address([](uintptr_t p){
    void** vp = (void**) p;
    //printf("codec: %p\n", *vp);
  });
  
  sandbox->sb()->invoke_sandbox_function(opj_stream_set_read_function, tainted_stream, sandbox->read_cb);
  sandbox->sb()->invoke_sandbox_function(opj_stream_set_write_function, tainted_stream, sandbox->write_cb);
  sandbox->sb()->invoke_sandbox_function(opj_stream_set_seek_function, tainted_stream, sandbox->seek_cb);
  sandbox->sb()->invoke_sandbox_function(opj_stream_set_skip_function, tainted_stream, sandbox->skip_cb);
  
  tainted<opj_stream_free_user_data_fn, rlbox_wasm2c_sandbox> tainted_free_func = NULL;

  app_pointer<void*, rlbox_wasm2c_sandbox> ap = sandbox->sb()->get_app_pointer(static_cast<void*>(image));
  tainted<void*, rlbox_wasm2c_sandbox> tainted_image = ap.to_tainted();
  //printf("Is app ptr registered? %d\n", !ap.is_unregistered());

  sandbox->sb()->invoke_sandbox_function(
    opj_stream_set_user_data, tainted_stream, tainted_image, tainted_free_func
  );

  jp2_status = sandbox->sb()->invoke_sandbox_function(
    opj_start_compress, tainted_codec, jp2_image, tainted_stream
  ).copy_and_verify([tainted_codec, tainted_stream] (OPJ_BOOL status_) {
    OPJ_BOOL status;
#ifdef JP2_STATUS__OPJ_START_COMPRESS__VERIFIER__TEST
    status = JP2_STATUS__OPJ_START_COMPRESS__VERIFIER__TEST_VALUE;
#else
    status = status_;
#endif

    if (status != OPJ_FALSE && !tainted_stream) {
      sandbox->fail("opj_start_compress status");
    }
    return status;
  });

  tainted_codec.copy_and_verify_address([](uintptr_t p){
    void** vp = (void**) p;
    //printf("codec: %p\n", *vp);
  });
  
  OPJ_BOOL encode_status = sandbox->sb()->invoke_sandbox_function(
    opj_encode, tainted_codec, tainted_stream
  ).copy_and_verify([tainted_codec, tainted_stream] (OPJ_BOOL status_) {
    OPJ_BOOL status;
#ifdef JP2_STATUS__OPJ_ENCODE__VERIFIER__TEST
    status = JP2_STATUS__OPJ_ENCODE__VERIFIER__TEST_VALUE;
#else
    status = status_;
#endif

    if (status != OPJ_FALSE && !tainted_stream) {
      sandbox->fail("opj_encode status");
    }
    return status;
  });

  OPJ_BOOL end_compress_status = sandbox->sb()->invoke_sandbox_function(
    opj_end_compress, tainted_codec, tainted_stream
  ).copy_and_verify([tainted_codec, tainted_stream] (OPJ_BOOL status_) {
    OPJ_BOOL status;
#ifdef JP2_STATUS__OPJ_END_COMPRESS__VERIFIER__TEST
    status = JP2_STATUS__OPJ_END_COMPRESS__VERIFIER__TEST_VALUE;
#else
    status = status_;
#endif

    if (status != OPJ_FALSE && !tainted_stream) {
      sandbox->fail("opj_end_compress status");
    }
    return status;
  });


  if ((jp2_status == 0) || 
      (encode_status == 0) ||
      (end_compress_status == 0))
    {
      sandbox->sb()->invoke_sandbox_function(opj_stream_destroy, tainted_stream);
      sandbox->sb()->invoke_sandbox_function(opj_destroy_codec, tainted_codec);
      sandbox->sb()->invoke_sandbox_function(opj_image_destroy, jp2_image);
      ThrowWriterException(DelegateError,"UnableToEncodeImageFile");
    }
  /*
    Free resources.
  */
  sandbox->sb()->invoke_sandbox_function(opj_stream_destroy, tainted_stream);
  sandbox->sb()->invoke_sandbox_function(opj_destroy_codec, tainted_codec);
  sandbox->sb()->invoke_sandbox_function(opj_image_destroy, jp2_image);
  (void) CloseBlob(image);

  //printf("Leaving writejp2image.\n");

  return(MagickTrue);
}
#endif
