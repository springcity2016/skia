/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/**************************************************************************************************
 *** This file was autogenerated from GrRRectBlurEffect.fp; do not modify.
 **************************************************************************************************/
#include "GrRRectBlurEffect.h"

std::unique_ptr<GrFragmentProcessor> GrRRectBlurEffect::Make(
        std::unique_ptr<GrFragmentProcessor> inputFP,
        GrRecordingContext* context,
        float sigma,
        float xformedSigma,
        const SkRRect& srcRRect,
        const SkRRect& devRRect) {
    SkASSERT(!SkRRectPriv::IsCircle(devRRect) &&
             !devRRect.isRect());  // Should've been caught up-stream

    // TODO: loosen this up
    if (!SkRRectPriv::IsSimpleCircular(devRRect)) {
        return nullptr;
    }

    // Make sure we can successfully ninepatch this rrect -- the blur sigma has to be
    // sufficiently small relative to both the size of the corner radius and the
    // width (and height) of the rrect.
    SkRRect rrectToDraw;
    SkISize dimensions;
    SkScalar ignored[kSkBlurRRectMaxDivisions];
    int ignoredSize;
    uint32_t ignored32;

    bool ninePatchable = SkComputeBlurredRRectParams(
            srcRRect, devRRect, SkRect::MakeEmpty(), sigma, xformedSigma, &rrectToDraw, &dimensions,
            ignored, ignored, ignored, ignored, &ignoredSize, &ignoredSize, &ignored32);
    if (!ninePatchable) {
        return nullptr;
    }

    std::unique_ptr<GrFragmentProcessor> maskFP =
            find_or_create_rrect_blur_mask_fp(context, rrectToDraw, dimensions, xformedSigma);
    if (!maskFP) {
        return nullptr;
    }

    return std::unique_ptr<GrFragmentProcessor>(
            new GrRRectBlurEffect(std::move(inputFP), xformedSigma, devRRect.getBounds(),
                                  SkRRectPriv::GetSimpleRadii(devRRect).fX, std::move(maskFP)));
}
#include "src/gpu/GrTexture.h"
#include "src/gpu/glsl/GrGLSLFragmentProcessor.h"
#include "src/gpu/glsl/GrGLSLFragmentShaderBuilder.h"
#include "src/gpu/glsl/GrGLSLProgramBuilder.h"
#include "src/sksl/SkSLCPP.h"
#include "src/sksl/SkSLUtil.h"
class GrGLSLRRectBlurEffect : public GrGLSLFragmentProcessor {
public:
    GrGLSLRRectBlurEffect() {}
    void emitCode(EmitArgs& args) override {
        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
        const GrRRectBlurEffect& _outer = args.fFp.cast<GrRRectBlurEffect>();
        (void)_outer;
        auto sigma = _outer.sigma;
        (void)sigma;
        auto rect = _outer.rect;
        (void)rect;
        auto cornerRadius = _outer.cornerRadius;
        (void)cornerRadius;
        cornerRadiusVar = args.fUniformHandler->addUniform(&_outer, kFragment_GrShaderFlag,
                                                           kHalf_GrSLType, "cornerRadius");
        proxyRectVar = args.fUniformHandler->addUniform(&_outer, kFragment_GrShaderFlag,
                                                        kFloat4_GrSLType, "proxyRect");
        blurRadiusVar = args.fUniformHandler->addUniform(&_outer, kFragment_GrShaderFlag,
                                                         kHalf_GrSLType, "blurRadius");
        fragBuilder->codeAppendf(
                "half2 translatedFragPos = half2(sk_FragCoord.xy - %s.xy);\nhalf2 proxyCenter = "
                "half2((%s.zw - %s.xy) * 0.5);\nhalf edgeSize = (2.0 * %s + %s) + "
                "0.5;\ntranslatedFragPos -= proxyCenter;\nhalf2 fragDirection = "
                "sign(translatedFragPos);\ntranslatedFragPos = "
                "abs(translatedFragPos);\ntranslatedFragPos -= proxyCenter - "
                "edgeSize;\ntranslatedFragPos = max(translatedFragPos, 0.0);\ntranslatedFragPos *= "
                "fragDirection;\ntranslatedFragPos += half2(edgeSize);\nhalf2 proxyDims = "
                "half2(2.0 * edgeSize);\nhalf2 texCoord = tra",
                args.fUniformHandler->getUniformCStr(proxyRectVar),
                args.fUniformHandler->getUniformCStr(proxyRectVar),
                args.fUniformHandler->getUniformCStr(proxyRectVar),
                args.fUniformHandler->getUniformCStr(blurRadiusVar),
                args.fUniformHandler->getUniformCStr(cornerRadiusVar));
        fragBuilder->codeAppendf("nslatedFragPos / proxyDims;");
        SkString _input9604 = SkStringPrintf("%s", args.fInputColor);
        SkString _sample9604;
        if (_outer.inputFP_index >= 0) {
            _sample9604 = this->invokeChild(_outer.inputFP_index, _input9604.c_str(), args);
        } else {
            _sample9604 = _input9604;
        }
        fragBuilder->codeAppendf("\nhalf4 inputColor = %s;", _sample9604.c_str());
        SkString _sample9664;
        SkString _coords9664("float2(texCoord)");
        _sample9664 = this->invokeChild(_outer.ninePatchFP_index, args, _coords9664.c_str());
        fragBuilder->codeAppendf("\n%s = inputColor * %s;\n", args.fOutputColor,
                                 _sample9664.c_str());
    }

private:
    void onSetData(const GrGLSLProgramDataManager& pdman,
                   const GrFragmentProcessor& _proc) override {
        const GrRRectBlurEffect& _outer = _proc.cast<GrRRectBlurEffect>();
        { pdman.set1f(cornerRadiusVar, (_outer.cornerRadius)); }
        auto sigma = _outer.sigma;
        (void)sigma;
        auto rect = _outer.rect;
        (void)rect;
        UniformHandle& cornerRadius = cornerRadiusVar;
        (void)cornerRadius;
        UniformHandle& proxyRect = proxyRectVar;
        (void)proxyRect;
        UniformHandle& blurRadius = blurRadiusVar;
        (void)blurRadius;

        float blurRadiusValue = 3.f * SkScalarCeilToScalar(sigma - 1 / 6.0f);
        pdman.set1f(blurRadius, blurRadiusValue);

        SkRect outset = rect;
        outset.outset(blurRadiusValue, blurRadiusValue);
        pdman.set4f(proxyRect, outset.fLeft, outset.fTop, outset.fRight, outset.fBottom);
    }
    UniformHandle proxyRectVar;
    UniformHandle blurRadiusVar;
    UniformHandle cornerRadiusVar;
};
GrGLSLFragmentProcessor* GrRRectBlurEffect::onCreateGLSLInstance() const {
    return new GrGLSLRRectBlurEffect();
}
void GrRRectBlurEffect::onGetGLSLProcessorKey(const GrShaderCaps& caps,
                                              GrProcessorKeyBuilder* b) const {}
bool GrRRectBlurEffect::onIsEqual(const GrFragmentProcessor& other) const {
    const GrRRectBlurEffect& that = other.cast<GrRRectBlurEffect>();
    (void)that;
    if (sigma != that.sigma) return false;
    if (rect != that.rect) return false;
    if (cornerRadius != that.cornerRadius) return false;
    return true;
}
GrRRectBlurEffect::GrRRectBlurEffect(const GrRRectBlurEffect& src)
        : INHERITED(kGrRRectBlurEffect_ClassID, src.optimizationFlags())
        , sigma(src.sigma)
        , rect(src.rect)
        , cornerRadius(src.cornerRadius) {
    if (src.inputFP_index >= 0) {
        inputFP_index = this->cloneAndRegisterChildProcessor(src.childProcessor(src.inputFP_index));
    }
    {
        ninePatchFP_index =
                this->cloneAndRegisterChildProcessor(src.childProcessor(src.ninePatchFP_index));
    }
}
std::unique_ptr<GrFragmentProcessor> GrRRectBlurEffect::clone() const {
    return std::unique_ptr<GrFragmentProcessor>(new GrRRectBlurEffect(*this));
}
GR_DEFINE_FRAGMENT_PROCESSOR_TEST(GrRRectBlurEffect);
#if GR_TEST_UTILS
std::unique_ptr<GrFragmentProcessor> GrRRectBlurEffect::TestCreate(GrProcessorTestData* d) {
    SkScalar w = d->fRandom->nextRangeScalar(100.f, 1000.f);
    SkScalar h = d->fRandom->nextRangeScalar(100.f, 1000.f);
    SkScalar r = d->fRandom->nextRangeF(1.f, 9.f);
    SkScalar sigma = d->fRandom->nextRangeF(1.f, 10.f);
    SkRRect rrect;
    rrect.setRectXY(SkRect::MakeWH(w, h), r, r);
    return GrRRectBlurEffect::Make(/*inputFP=*/nullptr, d->context(), sigma, sigma, rrect, rrect);
}
#endif
