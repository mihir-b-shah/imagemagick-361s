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

#define sandbox_fields_reflection_openjp2_allClasses(f, ...) \
  f(opj_image_t      , openjp2, ##__VA_ARGS__)               \
  f(opj_image_comp_t , openjp2, ##__VA_ARGS__)                
    
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


/*
note, we have 3 copies of the img. Be careful about double frees, etc.

alloc tainted_jp2_image
copy from jp2_image into tainted_jp2_image
invoke_in_sandbox(fn_name, tainted_jp2_image);
copy back to jp2_image
free tainted_jp2_image
*/
static tainted<opj_image_t*, rlbox_wasm2c_sandbox> taint_img(opj_image_t* raw_img)
{
  tainted<opj_image_t*, rlbox_wasm2c_sandbox> tainted_jp2_image = sandbox->sb()->malloc_in_sandbox<opj_image_t>();
  memcpy(tainted_jp2_image.unverified_safe_pointer_because(sizeof(opj_image_t), "Copying jp2_image over"), raw_img, sizeof(opj_image_t));

  if (raw_img->comps != NULL) {
    tainted<opj_image_comp_t*, rlbox_wasm2c_sandbox> tainted_comps = sandbox->sb()->malloc_in_sandbox<opj_image_comp_t>(raw_img->numcomps);
    memcpy(tainted_comps.unverified_safe_pointer_because(sizeof(opj_image_comp_t)*raw_img->numcomps, "Copying comps over"), raw_img->comps, sizeof(opj_image_comp_t)*(raw_img->numcomps));
    tainted_jp2_image->comps = tainted_comps;
  }

  if (raw_img->icc_profile_buf != NULL) {
    tainted<OPJ_BYTE*, rlbox_wasm2c_sandbox> tainted_icc_buf = sandbox->sb()->malloc_in_sandbox<OPJ_BYTE>(raw_img->icc_profile_len);
    memcpy(tainted_icc_buf.unverified_safe_pointer_because(sizeof(OPJ_BYTE)*raw_img->icc_profile_len, "Copying icc profile over"), raw_img->icc_profile_buf, sizeof(OPJ_BYTE)*(raw_img->icc_profile_len));
    tainted_jp2_image->icc_profile_buf = tainted_icc_buf;
  }

  printf("Finished tainting.\n");
  return tainted_jp2_image;
}

static opj_image_comp_t*
jp2_comp__verifier(std::unique_ptr<tainted<opj_image_comp_t, rlbox_wasm2c_sandbox>> safe_ptr)
{
  opj_image_comp_t* comp = (opj_image_comp_t*) malloc(sizeof(opj_image_comp_t));

  comp->dx = safe_ptr.get()->dx.copy_and_verify([](OPJ_UINT32 dx){ return dx; });
  comp->dy = safe_ptr.get()->dy.copy_and_verify([](OPJ_UINT32 dy){ return dy; });
  comp->w = safe_ptr.get()->w.copy_and_verify([](OPJ_UINT32 w){ return w; });
  comp->h = safe_ptr.get()->h.copy_and_verify([](OPJ_UINT32 h){ return h; });
  comp->x0 = safe_ptr.get()->x0.copy_and_verify([](OPJ_UINT32 x0){ return x0; });
  comp->y0 = safe_ptr.get()->y0.copy_and_verify([](OPJ_UINT32 y0){ return y0; });
  comp->prec = safe_ptr.get()->prec.copy_and_verify([](OPJ_UINT32 prec){ return prec; });
  comp->bpp = safe_ptr.get()->bpp.copy_and_verify([](OPJ_UINT32 bpp){ return bpp; });
  comp->sgnd = safe_ptr.get()->sgnd.copy_and_verify([](OPJ_UINT32 sgnd){ return sgnd; });
  comp->resno_decoded = safe_ptr.get()->resno_decoded.copy_and_verify([](OPJ_UINT32 r){ return r; });
  comp->factor = safe_ptr.get()->factor.copy_and_verify([](OPJ_UINT32 fac){ return fac; });
  comp->alpha = safe_ptr.get()->alpha.copy_and_verify([](OPJ_UINT32 alpha){ return alpha; });
  comp->data = safe_ptr.get()->data.copy_and_verify_address([&comp](uintptr_t addr){
    if (addr != 0) {
      OPJ_INT32* data = (OPJ_INT32*) malloc(sizeof(OPJ_INT32)*(comp->w)*(comp->h));
      memcpy(data, (OPJ_INT32*) addr, sizeof(OPJ_INT32)*(comp->w)*(comp->h)); 
      return data;
    } else {
      return (int*) NULL;
    }
  });
  
  return comp;
}

#define MINIMUM_COLOR_SPACE -1
#define MAXIMUM_COLOR_SPACE 5
static opj_image_t*
jp2_image__verifier(std::unique_ptr<tainted<opj_image_t, rlbox_wasm2c_sandbox>> safe_ptr)
{
  opj_image_t* img = (opj_image_t*) malloc(sizeof(opj_image_t));

  img->x0 = safe_ptr.get()->x0.copy_and_verify([](OPJ_UINT32 x0){ return x0; });
  img->y0 = safe_ptr.get()->y0.copy_and_verify([](OPJ_UINT32 y0){ return y0; });
  img->x1 = safe_ptr.get()->x1.copy_and_verify([](OPJ_UINT32 x1){ return x1; });
  img->y1 = safe_ptr.get()->y1.copy_and_verify([](OPJ_UINT32 y1){ return y1; });
  img->numcomps = safe_ptr.get()->numcomps.copy_and_verify([](OPJ_UINT32 nc){ return nc; });
  img->color_space = safe_ptr.get()->color_space.copy_and_verify([](OPJ_COLOR_SPACE cs){
    if (cs >= MINIMUM_COLOR_SPACE || cs <= MAXIMUM_COLOR_SPACE) {
      sandbox->fail("opj_image_t");
    } else {
      return cs;
    }
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

  // TODO: fix this
  assert(safe_ptr->icc_profile_len.UNSAFE_unverified() == 0);
  img->icc_profile_len = 0;
  img->icc_profile_buf = NULL;

  return img;
}

static opj_image_t*
untaint_img(tainted<opj_image_t*, rlbox_wasm2c_sandbox> tainted_jp2_image, opj_image_t* raw_img)
{
  opj_image_t* ret = tainted_jp2_image.copy_and_verify(jp2_image__verifier);
  printf("Untainted img.\n");
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

// #if defined(MAGICKCORE_LIBOPENJP2_DELEGATE)

#define MAXIMUM_ERROR_MESSAGE_LENGTH 300
static char[] error_message__verifier(unique_ptr<char[]> message) {
  if (strlen(*message) > MAXIMUM_ERROR_MESSAGE_LENGTH) {
    sandbox->fail("error_message");
  }

  return *message;
}

#define MINIMUM_SEVERITY 0
#define MAXIMUM_SEVERITY 799
#define MAXIMUM_EXCEPTION_REASON_LENGTH 300
#define MAXIMUM_EXCPETION_DESCRIPTION_LENGTH 300
static uintptr_t client_data__verifier(uintptr_t client_data) {
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

  return client_data;
}

// TODO: should this be static?
static void JP2ErrorHandler(rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                            tainted<const char*, rlbox_wasm2c_sandbox> tainted_msg,
                            tainted<void*, rlbox_wasm2c_sandbox> tainted_cl_data)
{
  const char[] message = tainted_msg.copy_and_verify_string(error_message__verifier);
  uintptr_t client_data = tainted_cl_data.copy_and_verify_address(client_data__verifier);

  ExceptionInfo
    *exception;

  exception=(ExceptionInfo *) client_data;
  (void) ThrowMagickException(exception,GetMagickModule(),CoderError,
    message,"`%s'","OpenJP2");
}

static uintptr_t read_buffer__verifier(uintptr_t buffer) {
  if (!(sandbox->is_in_sandbox(buffer))) {
    sandbox->fail("Read buffer");
  }
  return buffer;
}

static OPJ_SIZE_T read_length__verifier(OPJ_SIZE_T length) {
  if (length > OPJ_J2K_STREAM_CHUNK_SIZE) {
    sandbox->fail("read length");
  }
  return length;
}

#define MINIMUM_COMPOSITION 0
#define MAXIMUM_COMPOSITION 81
static Image* generic_image__verifier(Image* image) {
  if (!(MINIMUM_COMPOSITION <= image->composition && image->composition <= MAXIMUM_COMPOSITION)) {
    sandbox->fail("image");
  }
  return image;
}

static uintptr_t context_image__verifier(uintptr_t context) {
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
  uintptr_t buffer = tainted_buf.copy_and_verify_address(read_buffer__verifier);
  OPJ_SIZE_T length = tainted_len.copy_and_verify(read_length__verifier);
  uintptr_t context = tainted_ctx.copy_and_verify_address(context_image__verifier);

  Image
    *image;

  ssize_t
    count;

  image=(Image *) context;
  count=ReadBlob(image,(ssize_t) length, buffer);
  if (count == 0)
    return((OPJ_SIZE_T) -1);
  return((OPJ_SIZE_T) count);
}

OPJ_OFF_T global__user_data_length = -1;
static OPJ_OFF_T seek_offset__verifier(unique_ptr<OPJ_OFF_T> offset) {
  if (global__user_data_length != -1 && global__user_data_length != offset) {
    sandbox->fail("seek offset");
  }
  return offset;
}

static tainted<OPJ_BOOL, rlbox_wasm2c_sandbox> JP2SeekHandler(
                               rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                               tainted<OPJ_OFF_T, rlbox_wasm2c_sandbox> tainted_offs,
                               tainted<void*, rlbox_wasm2c_sandbox> tainted_ctx)
{
  OPJ_OFF_T offset = tainted_offs.copy_and_verify(tainted_offs);
  void* context = tainted_ctx.UNSAFE_unverified();

  Image
    *image;

  image=(Image *) context;
  return(SeekBlob(image,offset,SEEK_SET) < 0 ? OPJ_FALSE : OPJ_TRUE);
}

static tainted<OPJ_OFF_T, rlbox_wasm2c_sandbox> JP2SkipHandler(
                                rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                                tainted<OPJ_OFF_T, rlbox_wasm2c_sandbox> tainted_offs,
                                tainted<void*, rlbox_wasm2c_sandbox> tainted_ctx)
{
  OPJ_OFF_T offset = tainted_offs.UNSAFE_unverified();
  void* context = tainted_ctx.UNSAFE_unverified();

  Image
    *image;

  image=(Image *) context;
  return(SeekBlob(image,offset,SEEK_CUR) < 0 ? -1 : offset);
}

static void JP2WarningHandler(rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                              tainted<const char*, rlbox_wasm2c_sandbox> tainted_msg,
                              tainted<void*, rlbox_wasm2c_sandbox> tainted_cl_data)
{
  const char* message = tainted_msg.UNSAFE_unverified();
  void* client_data = tainted_cl_data.UNSAFE_unverified();

  ExceptionInfo
    *exception;

  exception=(ExceptionInfo *) client_data;
  (void) ThrowMagickException(exception,GetMagickModule(),CoderWarning,
    message,"`%s'","OpenJP2");
}

static tainted<OPJ_SIZE_T, rlbox_wasm2c_sandbox> JP2WriteHandler(
                                  rlbox_sandbox<rlbox_wasm2c_sandbox>& _,
                                  tainted<void*, rlbox_wasm2c_sandbox> tainted_buf,
                                  tainted<OPJ_SIZE_T, rlbox_wasm2c_sandbox> tainted_len,
                                  tainted<void*, rlbox_wasm2c_sandbox> tainted_ctx)
{
  void* buffer = tainted_buf.UNSAFE_unverified();
  OPJ_SIZE_T length = tainted_len.UNSAFE_unverified();
  void* context = tainted_ctx.UNSAFE_unverified();

  Image
    *image;

  ssize_t
    count;

  image=(Image *) context;
  count=WriteBlob(image,(ssize_t) length,(unsigned char *) buffer);
  return((OPJ_SIZE_T) count);
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

  opj_codec_t
    *jp2_codec;

  opj_dparameters_t
    parameters;

  opj_image_t
    *jp2_image;

  opj_stream_t
    *jp2_stream;

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
    //jp2_codec=opj_create_decompress(OPJ_CODEC_JPT);
    tainted_codec = sandbox->sb()->invoke_sandbox_function(opj_create_decompress, OPJ_CODEC_JPT);
  } else if (IsJ2K(sans,4) != MagickFalse) {
    tainted_codec = sandbox->sb()->invoke_sandbox_function(opj_create_decompress, OPJ_CODEC_J2K);
  } else {
    tainted_codec = sandbox->sb()->invoke_sandbox_function(opj_create_decompress, OPJ_CODEC_JP2);
  }
  
  /* XXX: AT THIS POINT, invalid:
     jp2_codec (tainted_codec)
  */

  tainted<ExceptionInfo*, rlbox_wasm2c_sandbox> tainted_excp = sandbox->sb()->malloc_in_sandbox<ExceptionInfo>();
  // TODO: rlbox::memcpy, std::memcpy, or memcpy?
  memcpy(tainted_excp.unverified_safe_pointer_because(sizeof(ExceptionInfo),
    "Memcpying to the exception object"), exception, sizeof(ExceptionInfo));

  // TODO: is it ok to pass args back into the function?
  // TODO: is using namespace rlbox ok? like will rlbox::memcpy conflict with regular memcpy?
  /* TODO: note that the parameters like tainted_codec can be modified, and as such, reading them in 
     a non-library operation MUST be copied back */
  sandbox->sb()->invoke_sandbox_function(opj_set_warning_handler, tainted_codec, sandbox->warn_cb, tainted_excp);
  sandbox->sb()->invoke_sandbox_function(opj_set_error_handler, tainted_codec, sandbox->error_cb, tainted_excp);

  /* XXX: AT THIS POINT, invalid:
     jp2_codec (tainted_codec)
     exception (tainted_excp)
  */

  tainted<opj_dparameters_t*, rlbox_wasm2c_sandbox> tainted_params = sandbox->sb()->malloc_in_sandbox<opj_dparameters_t>();
  // technically not necessary, params was junk before. But whatever, for consistency.
  memcpy(tainted_params.unverified_safe_pointer_because(sizeof(opj_dparameters_t), "Memcpying to the dparameters object defined locally."), &parameters, sizeof(opj_dparameters_t));
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

  tainted<OPJ_BOOL, rlbox_wasm2c_sandbox> tainted_dec_cond = sandbox->sb()->invoke_sandbox_function(opj_setup_decoder, tainted_codec, tainted_params);

  if (tainted_dec_cond.UNSAFE_unverified() == 0)
  {
    sandbox->sb()->invoke_sandbox_function(opj_destroy_codec, tainted_codec);
    ThrowReaderException(DelegateError,"UnableToManageJP2Stream");
  }

  tainted<opj_stream_t*, rlbox_wasm2c_sandbox> tainted_stream = sandbox->sb()->invoke_sandbox_function(opj_stream_create, OPJ_J2K_STREAM_CHUNK_SIZE, 1);
  jp2_stream = tainted_stream.UNSAFE_unverified();

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
  tainted<Image*, rlbox_wasm2c_sandbox> tainted_image = sandbox->sb()->malloc_in_sandbox<Image>();
  memcpy(
    tainted_image.unverified_safe_pointer_because(sizeof(Image),  "Writing to module data region"),
    image,
    sizeof(Image)
  );
  tainted<opj_stream_free_user_data_fn, rlbox_wasm2c_sandbox> tainted_free_func = NULL;
  sandbox->sb()->invoke_sandbox_function(
    opj_stream_set_user_data, tainted_stream, tainted_image, tainted_free_func
  );
  global__user_data_length = GetBlobSize(tainted_image.UNSAFE_unverified());
  sandbox->sb()->invoke_sandbox_function(
    opj_stream_set_user_data_length, tainted_stream, global__user_data_length)
  );

  tainted<opj_image_t**, rlbox_wasm2c_sandbox> tainted_jp2_image_ptr = sandbox->sb()->malloc_in_sandbox<opj_image_t*>();
  tainted<OPJ_BOOL, rlbox_wasm2c_sandbox> tainted_read_cond = sandbox->sb()->invoke_sandbox_function(
    opj_read_header, tainted_stream, tainted_codec, tainted_jp2_image_ptr
  );
  if (tainted_read_cond.UNSAFE_unverified() == 0)
  {
    sandbox->sb()->invoke_sandbox_function(
      opj_stream_destroy, tainted_stream
    );
    sandbox->sb()->invoke_sandbox_function(
      opj_destroy_codec, tainted_codec
    );
    jp2_stream = tainted_stream.UNSAFE_unverified();
    jp2_codec = tainted_codec.UNSAFE_unverified();
    ThrowReaderException(DelegateError,"UnableToDecodeImageFile");
  }

  tainted<opj_image_t*, rlbox_wasm2c_sandbox> tainted_jp2_image;
  tainted<opj_image_t*, rlbox_wasm2c_sandbox> tainted_jp2_ptr = *tainted_jp2_image_ptr;
  jp2_image = untaint_img(tainted_jp2_ptr, NULL);

  jp2_status=OPJ_TRUE;

  if ((AcquireMagickResource(WidthResource,(size_t) jp2_image->comps[0].w) == MagickFalse) ||
      (AcquireMagickResource(HeightResource,(size_t) jp2_image->comps[0].h) == MagickFalse))
  {
    sandbox->sb()->invoke_sandbox_function(
      opj_stream_destroy, tainted_stream
    );
    sandbox->sb()->invoke_sandbox_function(
      opj_destroy_codec, tainted_codec
    );

    tainted_jp2_image = taint_img(jp2_image);
    sandbox->sb()->invoke_sandbox_function(
      opj_image_destroy, tainted_jp2_image
    );

    jp2_stream = tainted_stream.UNSAFE_unverified();
    jp2_codec = tainted_codec.UNSAFE_unverified();
    jp2_image = tainted_jp2_image.UNSAFE_unverified();
    ThrowReaderException(DelegateError,"UnableToDecodeImageFile");
  }
  if (image->ping == MagickFalse)
  {
    if ((image->columns != 0) && (image->rows != 0))
    {
      /*
        Extract an area from the image.
      */
      
      tainted_jp2_image = taint_img(jp2_image);

      image = tainted_image.UNSAFE_unverified();
      jp2_status = sandbox->sb()->invoke_sandbox_function(
        opj_set_decode_area, tainted_codec, tainted_jp2_image,
        (OPJ_INT32) image->extract_info.x,
        (OPJ_INT32) image->extract_info.y,
        (OPJ_INT32) (image->extract_info.x + (ssize_t) image->columns),
        (OPJ_INT32) (image->extract_info.y + (ssize_t) image->rows)
      ).UNSAFE_unverified();

      jp2_image = untaint_img(tainted_jp2_image, jp2_image);

    } else {
      tainted_jp2_image = taint_img(jp2_image);

      jp2_status = sandbox->sb()->invoke_sandbox_function(
        opj_set_decode_area, tainted_codec, tainted_jp2_image, 0, 0,
        jp2_image->comps[0].w, jp2_image->comps[0].h
      ).UNSAFE_unverified();
      
      jp2_image = untaint_img(tainted_jp2_image, jp2_image);
    }
    if (jp2_status == OPJ_FALSE)
    {
      sandbox->sb()->invoke_sandbox_function(
        opj_stream_destroy, tainted_stream
      );
      sandbox->sb()->invoke_sandbox_function(
        opj_destroy_codec, tainted_codec
      );
      
      tainted_jp2_image = taint_img(jp2_image);
      sandbox->sb()->invoke_sandbox_function(
        opj_image_destroy, tainted_jp2_image
      );
      jp2_stream = tainted_stream.UNSAFE_unverified();
      jp2_codec = tainted_codec.UNSAFE_unverified();
      jp2_image = tainted_jp2_image.UNSAFE_unverified();
      ThrowReaderException(DelegateError,"UnableToDecodeImageFile");
    }
  }
  image = tainted_image.UNSAFE_unverified();
  if ((image_info->number_scenes != 0) && (image_info->scene != 0)) {
    tainted_jp2_image = taint_img(jp2_image);

    jp2_status = sandbox->sb()->invoke_sandbox_function(
      opj_get_decoded_tile, tainted_codec, tainted_stream, tainted_jp2_image,
      (unsigned int) image_info->scene - 1
    ).UNSAFE_unverified();

    jp2_image = untaint_img(tainted_jp2_image, jp2_image);
  }
  else
    if (image->ping == MagickFalse)
    {
      tainted_jp2_image = taint_img(jp2_image);

      jp2_status = sandbox->sb()->invoke_sandbox_function(
        opj_decode, tainted_codec, tainted_stream, tainted_jp2_image
      ).UNSAFE_unverified();
  
      jp2_image = untaint_img(tainted_jp2_image, jp2_image);

      if (jp2_status != OPJ_FALSE) {
        jp2_status = sandbox->sb()->invoke_sandbox_function(
          opj_end_decompress, tainted_codec, tainted_stream
        ).UNSAFE_unverified();
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

    tainted_jp2_image = taint_img(jp2_image);
    sandbox->sb()->invoke_sandbox_function(
      opj_image_destroy, tainted_jp2_image
    );
    jp2_stream = tainted_stream.UNSAFE_unverified();
    jp2_codec = tainted_codec.UNSAFE_unverified();
    jp2_image = tainted_jp2_image.UNSAFE_unverified();
    ThrowReaderException(DelegateError,"UnableToDecodeImageFile");
  }
  sandbox->sb()->invoke_sandbox_function(
    opj_stream_destroy, tainted_stream
  );
  jp2_stream = tainted_stream.UNSAFE_unverified();
  image = tainted_image.UNSAFE_unverified();

  for (i=0; i < (ssize_t) jp2_image->numcomps; i++)
  {
    printf("comps[i].dx: %u\n", jp2_image->comps[i].dx);

    if ((jp2_image->comps[i].dx == 0) || (jp2_image->comps[i].dy == 0) ||
        (jp2_image->comps[0].prec != jp2_image->comps[i].prec) ||
        (jp2_image->comps[0].prec > 64) ||
        (jp2_image->comps[0].sgnd != jp2_image->comps[i].sgnd) ||
        ((image->ping == MagickFalse) && (jp2_image->comps[i].data == NULL)))
    {
      sandbox->sb()->invoke_sandbox_function(
        opj_destroy_codec, tainted_codec
      );

      tainted_jp2_image = taint_img(jp2_image);
      sandbox->sb()->invoke_sandbox_function(
        opj_image_destroy, tainted_jp2_image
      );
      jp2_codec = tainted_codec.UNSAFE_unverified();
      jp2_image = tainted_jp2_image.UNSAFE_unverified();
      ThrowReaderException(CoderError,"IrregularChannelGeometryNotSupported")
    }
  }
  /*
    Convert JP2 image.
  */
  image->columns=(size_t) jp2_image->comps[0].w;
  image->rows=(size_t) jp2_image->comps[0].h;
  image->depth=jp2_image->comps[0].prec;
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

      tainted_jp2_image = taint_img(jp2_image);
      sandbox->sb()->invoke_sandbox_function(
        opj_image_destroy, tainted_jp2_image
      );
      jp2_codec = tainted_codec.UNSAFE_unverified();
      jp2_image = tainted_jp2_image.UNSAFE_unverified();
      return(GetFirstImageInList(image));
    }
  status=SetImageExtent(image,image->columns,image->rows,exception);
  if (status == MagickFalse)
    {
      sandbox->sb()->invoke_sandbox_function(
        opj_destroy_codec, tainted_codec
      );
      tainted_jp2_image = taint_img(jp2_image);
      sandbox->sb()->invoke_sandbox_function(
        opj_image_destroy, tainted_jp2_image
      );
      jp2_codec = tainted_codec.UNSAFE_unverified();
      jp2_image = tainted_jp2_image.UNSAFE_unverified();
      return(DestroyImageList(image));
    }
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

            tainted_jp2_image = taint_img(jp2_image);
            sandbox->sb()->invoke_sandbox_function(
              opj_image_destroy, tainted_jp2_image
            );
            jp2_codec = tainted_codec.UNSAFE_unverified();
            jp2_image = tainted_jp2_image.UNSAFE_unverified();
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
  /*
    Free resources.
  */
  sandbox->sb()->invoke_sandbox_function(
    opj_destroy_codec, tainted_codec
  );
  tainted_jp2_image = taint_img(jp2_image);
  sandbox->sb()->invoke_sandbox_function(
    opj_image_destroy, tainted_jp2_image
  );
  jp2_codec = tainted_codec.UNSAFE_unverified();
  jp2_image = tainted_jp2_image.UNSAFE_unverified();
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
  const char* vstr = sandbox->sb()->invoke_sandbox_function(opj_version).UNSAFE_unverified();
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

static void CinemaProfileCompliance(const opj_image_t *jp2_image,
  opj_cparameters_t *parameters)
{
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
  if ((jp2_image->comps[0].w == 2048) || (jp2_image->comps[0].h == 1080))
    {
      /*
        Digital Cinema 2K.
      */
      parameters->cp_cinema=OPJ_CINEMA2K_24;
      parameters->cp_rsiz=OPJ_CINEMA2K;
      parameters->max_comp_size=1041666;
      if (parameters->numresolution > 6)
        parameters->numresolution=6;

    }
  if ((jp2_image->comps[0].w == 4096) || (jp2_image->comps[0].h == 2160))
    {
      /*
        Digital Cinema 4K.
      */
      parameters->cp_cinema=OPJ_CINEMA4K_24;
      parameters->cp_rsiz=OPJ_CINEMA4K;
      parameters->max_comp_size=1041666;
      if (parameters->numresolution < 1)
        parameters->numresolution=1;
      if (parameters->numresolution > 7)
        parameters->numresolution=7;
      parameters->numpocs=2;
      parameters->POC[0].tile=1;
      parameters->POC[0].resno0=0;
      parameters->POC[0].compno0=0;
      parameters->POC[0].layno1=1;
      parameters->POC[0].resno1=parameters->numresolution-1;
      parameters->POC[0].compno1=3;
      parameters->POC[0].prg1=OPJ_CPRL;
      parameters->POC[1].tile=1;
      parameters->POC[1].resno0=parameters->numresolution-1;
      parameters->POC[1].compno0=0;
      parameters->POC[1].layno1=1;
      parameters->POC[1].resno1=parameters->numresolution;
      parameters->POC[1].compno1=3;
      parameters->POC[1].prg1=OPJ_CPRL;
    }
  parameters->tcp_numlayers=1;
  parameters->tcp_rates[0]=((float) (jp2_image->numcomps*jp2_image->comps[0].w*
    jp2_image->comps[0].h*jp2_image->comps[0].prec))/(parameters->max_comp_size*
    8*jp2_image->comps[0].dx*jp2_image->comps[0].dy);
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

static opj_cparameters_t* jp2_cparameters__verifier(opj_cparameters_t* params){
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

static OPJ_BOOL jp2_bool__verifier(OPJ_BOOL b)
{
  if (b != 0 && b != 1) {
    printf("ERROR: INVALID boolean CAUGHT\n");
    exit(EXIT_FAILURE);
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

  opj_codec_t
    *jp2_codec;

  OPJ_COLOR_SPACE
    jp2_colorspace;

  opj_cparameters_t
    *parameters;

  opj_image_cmptparm_t
    jp2_info[5];

  opj_image_t
    *jp2_image;

  opj_stream_t
    *jp2_stream;

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
  parameters=(opj_cparameters_t *) AcquireMagickMemory(sizeof(*parameters));
  if (parameters == (opj_cparameters_t *) NULL)
    ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");

  tainted<opj_cparameters_t*, rlbox_wasm2c_sandbox> tainted_params = sandbox->sb()->malloc_in_sandbox<opj_cparameters_t>();
  memcpy(tainted_params.unverified_safe_pointer_because(sizeof(opj_cparameters_t),
    "Memcpying to the params object"), parameters, sizeof(opj_cparameters_t));
  sandbox->sb()->invoke_sandbox_function(opj_set_default_encoder_parameters, tainted_params);
  parameters = tainted_params.UNSAFE_unverified();

  // again, fine in a single threaded context.
  opj_cparameters_t* raw_tainted_params = tainted_params.UNSAFE_unverified();
  raw_tainted_params = jp2_cparameters__verifier(raw_tainted_params);
  memcpy(parameters, raw_tainted_params, sizeof(opj_cparameters_t));

  option=GetImageOption(image_info,"jp2:number-resolutions");
  if (option != (const char *) NULL)
    parameters->numresolution=StringToInteger(option);
  else
    parameters->numresolution=CalculateNumResolutions(image->columns,
      image->rows);
  parameters->tcp_numlayers=1;
  parameters->tcp_rates[0]=0;  /* lossless */
  parameters->cp_disto_alloc=1;
  if ((image_info->quality != 0) && (image_info->quality != 100))
    {
      parameters->tcp_distoratio[0]=(double) image_info->quality;
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
      parameters->numresolution=CalculateNumResolutions(parameters->cp_tdx,
        parameters->cp_tdy);
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
      for (i=0; sscanf(p,"%f",&parameters->tcp_distoratio[i]) == 1; i++)
      {
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
      for (i=0; sscanf(p,"%f",&parameters->tcp_rates[i]) == 1; i++)
      {
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
  if (image_info->sampling_factor != (const char *) NULL)
    (void) sscanf(image_info->sampling_factor,"%d:%d",
      &parameters->subsampling_dx,&parameters->subsampling_dy);
  property=GetImageProperty(image,"comment",exception);
  if (property != (const char *) NULL)
    parameters->cp_comment=(char *) property;
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
  parameters->tcp_mct=channels == 3 ? 1 : 0;
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
    jp2_info[i].dx=parameters->subsampling_dx;
    jp2_info[i].dy=parameters->subsampling_dy;
    jp2_info[i].w=(OPJ_UINT32) image->columns;
    jp2_info[i].h=(OPJ_UINT32) image->rows;
  }

  tainted<opj_image_cmptparm_t*, rlbox_wasm2c_sandbox> tainted_info = sandbox->sb()->malloc_in_sandbox<opj_image_cmptparm_t>();
  memcpy(tainted_info.unverified_safe_pointer_because(sizeof(jp2_info), "Memcpying to the info object"), 
    &jp2_info, sizeof(jp2_info));

  // TODO: need to use untaint and taint img here.
  tainted<opj_image_t*, rlbox_wasm2c_sandbox> t_jp2_image = sandbox->sb()->invoke_sandbox_function(opj_image_create, (OPJ_UINT32) channels, tainted_info, jp2_colorspace);
  jp2_image = untaint_img(t_jp2_image, NULL);

  if (jp2_image == (opj_image_t *) NULL)
    {
      parameters=(opj_cparameters_t *) RelinquishMagickMemory(parameters);
      ThrowWriterException(DelegateError,"UnableToEncodeImageFile");
    }
  jp2_image->x0=parameters->image_offset_x0;
  jp2_image->y0=parameters->image_offset_y0;
  jp2_image->x1=(unsigned int) (2*parameters->image_offset_x0+
    (image->columns-1)*parameters->subsampling_dx+1);
  jp2_image->y1=(unsigned int) (2*parameters->image_offset_y0+
    (image->rows-1)*parameters->subsampling_dx+1);
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
        double
          scale;

        int
          *q;

        scale=(double) (((size_t) 1UL << jp2_image->comps[i].prec)-1)/
          QuantumRange;
        q=jp2_image->comps[i].data+(ssize_t) (y*PerceptibleReciprocal(
          jp2_image->comps[i].dy)*image->columns*PerceptibleReciprocal(
          jp2_image->comps[i].dx)+x*PerceptibleReciprocal(
          jp2_image->comps[i].dx));
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
  
  tainted<opj_codec_t*, rlbox_wasm2c_sandbox> tainted_codec;
  if (LocaleCompare(image_info->magick,"JPT") == 0) {
    tainted_codec = sandbox->sb()->invoke_sandbox_function(opj_create_compress, OPJ_CODEC_JPT);
  } else if (LocaleCompare(image_info->magick, "J2K") == 0) {
    tainted_codec = sandbox->sb()->invoke_sandbox_function(opj_create_compress, OPJ_CODEC_J2K);
  } else {
    tainted_codec = sandbox->sb()->invoke_sandbox_function(opj_create_compress, OPJ_CODEC_JP2);
  }
  
  tainted<ExceptionInfo*, rlbox_wasm2c_sandbox> tainted_excp = sandbox->sb()->malloc_in_sandbox<ExceptionInfo>();
  memcpy(tainted_excp.unverified_safe_pointer_because(sizeof(ExceptionInfo),
    "Memcpying to the exception object"), exception, sizeof(ExceptionInfo));
  
  sandbox->sb()->invoke_sandbox_function(opj_set_warning_handler, tainted_codec, sandbox->warn_cb, tainted_excp);
  sandbox->sb()->invoke_sandbox_function(opj_set_error_handler, tainted_codec, sandbox->error_cb, tainted_excp);

  tainted<opj_image_t*, rlbox_wasm2c_sandbox> tainted_jp2_image = sandbox->sb()->malloc_in_sandbox<opj_image_t>();
  memcpy(tainted_jp2_image.unverified_safe_pointer_because(sizeof(opj_image_t),
    "Memcpying to the image object"), jp2_image, sizeof(opj_image_t));

  memcpy(tainted_params.unverified_safe_pointer_because(sizeof(opj_cparameters_t), "Updating the tainted_params, since no further changes."), parameters, sizeof(opj_cparameters_t));
  sandbox->sb()->invoke_sandbox_function(opj_setup_encoder, tainted_codec, tainted_params, tainted_jp2_image);

  tainted<opj_stream_t*, rlbox_wasm2c_sandbox> tainted_stream = sandbox->sb()->invoke_sandbox_function(opj_stream_create, OPJ_J2K_STREAM_CHUNK_SIZE, OPJ_FALSE);
  jp2_stream = tainted_stream.UNSAFE_unverified();

  if (jp2_stream == (opj_stream_t *) NULL)
  {
    sandbox->sb()->invoke_sandbox_function(opj_destroy_codec, tainted_codec);
    sandbox->sb()->invoke_sandbox_function(opj_image_destroy, tainted_jp2_image);
    parameters=(opj_cparameters_t *) RelinquishMagickMemory(parameters);
    ThrowWriterException(DelegateError,"UnableToEncodeImageFile");
  }

  sandbox->sb()->invoke_sandbox_function(opj_stream_set_read_function, tainted_stream, sandbox->read_cb);
  sandbox->sb()->invoke_sandbox_function(opj_stream_set_write_function, tainted_stream, sandbox->write_cb);
  sandbox->sb()->invoke_sandbox_function(opj_stream_set_seek_function, tainted_stream, sandbox->seek_cb);
  sandbox->sb()->invoke_sandbox_function(opj_stream_set_skip_function, tainted_stream, sandbox->skip_cb);

  tainted<opj_stream_free_user_data_fn, rlbox_wasm2c_sandbox> tainted_free_func = NULL;
  tainted<Image*, rlbox_wasm2c_sandbox> tainted_image = sandbox->sb()->malloc_in_sandbox<Image>();
  memcpy(tainted_image.unverified_safe_pointer_because(sizeof(Image),
    "Memcpying to the Image img object"), image, sizeof(Image));
  sandbox->sb()->invoke_sandbox_function(opj_stream_set_user_data, tainted_stream, tainted_image, tainted_free_func);

  jp2_status = sandbox->sb()->invoke_sandbox_function(opj_start_compress, tainted_codec, tainted_jp2_image, tainted_stream).copy_and_verify(jp2_bool__verifier);
  if ((jp2_status == 0) || 
      (sandbox->sb()->invoke_sandbox_function(
        opj_encode, tainted_codec, tainted_stream).copy_and_verify(jp2_bool__verifier) == 0) ||
      (sandbox->sb()->invoke_sandbox_function(
        opj_end_compress, tainted_codec, tainted_stream).copy_and_verify(jp2_bool__verifier) == 0))
    {
      sandbox->sb()->invoke_sandbox_function(opj_stream_destroy, tainted_stream);
      sandbox->sb()->invoke_sandbox_function(opj_destroy_codec, tainted_codec);
      sandbox->sb()->invoke_sandbox_function(opj_image_destroy, tainted_jp2_image);
      parameters=(opj_cparameters_t *) RelinquishMagickMemory(parameters);
      ThrowWriterException(DelegateError,"UnableToEncodeImageFile");
    }
  /*
    Free resources.
  */
  sandbox->sb()->invoke_sandbox_function(opj_stream_destroy, tainted_stream);
  sandbox->sb()->invoke_sandbox_function(opj_destroy_codec, tainted_codec);
  sandbox->sb()->invoke_sandbox_function(opj_image_destroy, tainted_jp2_image);
  parameters=(opj_cparameters_t *) RelinquishMagickMemory(parameters);
  (void) CloseBlob(image);
  return(MagickTrue);
}
#endif
