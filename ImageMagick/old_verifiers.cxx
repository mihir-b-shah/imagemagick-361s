
/*
static opj_image_t* jp2_image__verifier (opj_image_t* img) {
  if (
    (img->color_space < -1 || 5 < img->color_space) ||
    (!sandbox->sb()->is_in_same_sandbox(img, img->icc_profile_buf))
  ) {
    printf("ERROR: INVALID opj_image_t CAUGHT\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < img->numcomps; i++) {
    if (
      (!sandbox->sb()->is_in_same_sandbox(img, img->comps + i))
    ) {
      printf("ERROR: INVALID opj_image_t CAUGHT\n");
      exit(EXIT_FAILURE);
    }
  }

  return img;
}
*/

static opj_image_t*
untaint_img(tainted<opj_image_t*, rlbox_wasm2c_sandbox> tainted_jp2_image, opj_image_t* raw_img)
{
  auto safe_ptr = tainted_jp2_image.copy_and_verify(
    [](std::unique_ptr<tainted<opj_image, rlbox_wasm2c_sandbox>> ptr){ return ptr; }
  );
  OPJ_UINT32 x0 = safe_ptr.get()->x0.copy_and_verify([](OPJ_UINT32 x0){ return x0; });
  OPJ_UINT32 y0 = safe_ptr.get()->y0.copy_and_verify([](OPJ_UINT32 y0){ return y0; });
  OPJ_UINT32 x1 = safe_ptr.get()->x1.copy_and_verify([](OPJ_UINT32 x1){ return x1; });
  OPJ_UINT32 y1 = safe_ptr.get()->y1.copy_and_verify([](OPJ_UINT32 y1){ return y1; });
  OPJ_UINT32 numcomps = safe_ptr.get()->numcomps.copy_and_verify([](OPJ_UINT32 nc){ return nc; });
  OPJ_COLOR_SPACE color_space = safe_ptr.get()->color_space.copy_and_verify(
    [](OPJ_COLOR_SPACE cs){return cs; });

  for (int i = 0; i<numcomps; ++i) {
    auto comp_safe_ptr = (safe_ptr.get()->comps+i).copy_and_verify(jp2_com
      [](std::unique_ptr<tainted<opj_image_comp_t, rlbox_wasm2c_sandbox>> comp){ return comp; }
    );

    

    comp_safe_ptr->

  }

  /****************************************/

  bool new_img = false;
  if (!raw_img) {
    raw_img = (opj_image_t*) malloc(sizeof(opj_image_t));
    new_img = true;
  }

  auto ret = tainted_jp2_image.copy_and_verify_address([](uintptr_t p){
    printf("%lx\n", p);
    return true;
  });
  
  opj_image_t* untainted_img = tainted_jp2_image.UNSAFE_unverified();
  memcpy(raw_img, untainted_img, sizeof(opj_image_t));
  untainted_img = jp2_image__verifier(untainted_img);

  if (new_img) {
    tainted<opj_image_comp_t*, rlbox_wasm2c_sandbox> tcomps = (*tainted_jp2_image).comps;
    tainted<OPJ_BYTE*, rlbox_wasm2c_sandbox> ticc = tainted_jp2_image->icc_profile_buf;

    raw_img->comps = (opj_image_comp_t*) (tainted_jp2_image->comps.UNSAFE_unverified() != NULL ? malloc(sizeof(opj_image_comp_t)*(tainted_jp2_image->numcomps.UNSAFE_unverified())) : NULL);
    raw_img->icc_profile_buf = (OPJ_BYTE*) (tainted_jp2_image->icc_profile_buf.UNSAFE_unverified() != NULL ? malloc(sizeof(OPJ_BYTE)*(tainted_jp2_image->icc_profile_len.UNSAFE_unverified())) : NULL);
  }

  if (tainted_jp2_image->comps.UNSAFE_unverified() != NULL) {
    memcpy(raw_img->comps, tainted_jp2_image->comps.UNSAFE_unverified(), sizeof(opj_image_comp_t)*(tainted_jp2_image->numcomps.UNSAFE_unverified()));
    sandbox->sb()->free_in_sandbox(tainted_jp2_image->comps);
  }

  if (tainted_jp2_image->icc_profile_buf.UNSAFE_unverified() != NULL) {
    memcpy(raw_img->icc_profile_buf, tainted_jp2_image->icc_profile_buf.UNSAFE_unverified(), sizeof(OPJ_BYTE)*(tainted_jp2_image->icc_profile_len.UNSAFE_unverified()));
    sandbox->sb()->free_in_sandbox(tainted_jp2_image->icc_profile_buf);
  }

  sandbox->sb()->free_in_sandbox(tainted_jp2_image);
  printf("Finished untainting.\n");
  return raw_img;
}

