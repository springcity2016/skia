/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/**************************************************************************************************
 *** This file was autogenerated from GrTextureGradientColorizer.fp; do not modify.
 **************************************************************************************************/
#ifndef GrTextureGradientColorizer_DEFINED
#define GrTextureGradientColorizer_DEFINED

#include "include/core/SkM44.h"
#include "include/core/SkTypes.h"

#include "src/gpu/GrCoordTransform.h"
#include "src/gpu/GrFragmentProcessor.h"

class GrTextureGradientColorizer : public GrFragmentProcessor {
public:
    static std::unique_ptr<GrFragmentProcessor> Make(GrSurfaceProxyView gradient) {
        return std::unique_ptr<GrFragmentProcessor>(
                new GrTextureGradientColorizer(std::move(gradient)));
    }
    GrTextureGradientColorizer(const GrTextureGradientColorizer& src);
    std::unique_ptr<GrFragmentProcessor> clone() const override;
    const char* name() const override { return "TextureGradientColorizer"; }
    TextureSampler gradient;

private:
    GrTextureGradientColorizer(GrSurfaceProxyView gradient)
            : INHERITED(kGrTextureGradientColorizer_ClassID, kNone_OptimizationFlags)
            , gradient(std::move(gradient), GrSamplerState::Filter::kBilerp) {
        this->setTextureSamplerCnt(1);
    }
    GrGLSLFragmentProcessor* onCreateGLSLInstance() const override;
    void onGetGLSLProcessorKey(const GrShaderCaps&, GrProcessorKeyBuilder*) const override;
    bool onIsEqual(const GrFragmentProcessor&) const override;
    const TextureSampler& onTextureSampler(int) const override;
    GR_DECLARE_FRAGMENT_PROCESSOR_TEST
    typedef GrFragmentProcessor INHERITED;
};
#endif
