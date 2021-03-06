/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/**************************************************************************************************
 *** This file was autogenerated from GrLumaColorFilterEffect.fp; do not modify.
 **************************************************************************************************/
#include "GrLumaColorFilterEffect.h"

#include "src/gpu/GrTexture.h"
#include "src/gpu/glsl/GrGLSLFragmentProcessor.h"
#include "src/gpu/glsl/GrGLSLFragmentShaderBuilder.h"
#include "src/gpu/glsl/GrGLSLProgramBuilder.h"
#include "src/sksl/SkSLCPP.h"
#include "src/sksl/SkSLUtil.h"
class GrGLSLLumaColorFilterEffect : public GrGLSLFragmentProcessor {
public:
    GrGLSLLumaColorFilterEffect() {}
    void emitCode(EmitArgs& args) override {
        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
        const GrLumaColorFilterEffect& _outer = args.fFp.cast<GrLumaColorFilterEffect>();
        (void)_outer;
        SkString _input953 = SkStringPrintf("%s", args.fInputColor);
        SkString _sample953;
        if (_outer.inputFP_index >= 0) {
            _sample953 = this->invokeChild(_outer.inputFP_index, _input953.c_str(), args);
        } else {
            _sample953 = _input953;
        }
        fragBuilder->codeAppendf(
                "half4 inputColor = %s;\n\nhalf luma = clamp(dot(half3(0.2125999927520752, "
                "0.71520000696182251, 0.072200000286102295), inputColor.xyz), 0.0, 1.0);\n%s = "
                "half4(0.0, 0.0, 0.0, luma);\n",
                _sample953.c_str(), args.fOutputColor);
    }

private:
    void onSetData(const GrGLSLProgramDataManager& pdman,
                   const GrFragmentProcessor& _proc) override {}
};
GrGLSLFragmentProcessor* GrLumaColorFilterEffect::onCreateGLSLInstance() const {
    return new GrGLSLLumaColorFilterEffect();
}
void GrLumaColorFilterEffect::onGetGLSLProcessorKey(const GrShaderCaps& caps,
                                                    GrProcessorKeyBuilder* b) const {}
bool GrLumaColorFilterEffect::onIsEqual(const GrFragmentProcessor& other) const {
    const GrLumaColorFilterEffect& that = other.cast<GrLumaColorFilterEffect>();
    (void)that;
    return true;
}
GrLumaColorFilterEffect::GrLumaColorFilterEffect(const GrLumaColorFilterEffect& src)
        : INHERITED(kGrLumaColorFilterEffect_ClassID, src.optimizationFlags()) {
    if (src.inputFP_index >= 0) {
        inputFP_index = this->cloneAndRegisterChildProcessor(src.childProcessor(src.inputFP_index));
    }
}
std::unique_ptr<GrFragmentProcessor> GrLumaColorFilterEffect::clone() const {
    return std::unique_ptr<GrFragmentProcessor>(new GrLumaColorFilterEffect(*this));
}
