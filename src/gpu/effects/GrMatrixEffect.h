/*
 * Copyright 2020 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrMatrixEffect_DEFINED
#define GrMatrixEffect_DEFINED

#include "include/core/SkM44.h"
#include "include/core/SkTypes.h"

#include "src/gpu/GrCoordTransform.h"
#include "src/gpu/GrFragmentProcessor.h"

class GrMatrixEffect : public GrFragmentProcessor {
public:
    static std::unique_ptr<GrFragmentProcessor> Make(
            const SkMatrix& matrix, std::unique_ptr<GrFragmentProcessor> child) {
        if (matrix.isIdentity()) {
            return child;
        }
        SkASSERT(!child->isSampledWithExplicitCoords());
        SkASSERT(child->sampleMatrix().fKind == SkSL::SampleMatrix::Kind::kNone);
        return std::unique_ptr<GrFragmentProcessor>(
                new GrMatrixEffect(matrix, std::move(child)));
    }

    std::unique_ptr<GrFragmentProcessor> clone() const override;
    const char* name() const override { return "MatrixEffect"; }
    const SkMatrix& matrix() const { return fMatrix; }

private:
    GrMatrixEffect(const GrMatrixEffect& src);

    GrMatrixEffect(SkMatrix matrix, std::unique_ptr<GrFragmentProcessor> child)
            : INHERITED(kGrMatrixEffect_ClassID, kNone_OptimizationFlags)
            , fMatrix(matrix) {
        SkASSERT(child);
        child->setSampleMatrix(
                SkSL::SampleMatrix(SkSL::SampleMatrix::Kind::kConstantOrUniform, this, "matrix"));
        this->registerChildProcessor(std::move(child));
    }

    GrGLSLFragmentProcessor* onCreateGLSLInstance() const override;
    void onGetGLSLProcessorKey(const GrShaderCaps&, GrProcessorKeyBuilder*) const override;
    bool onIsEqual(const GrFragmentProcessor&) const override;

    SkMatrix fMatrix;

    GR_DECLARE_FRAGMENT_PROCESSOR_TEST
    typedef GrFragmentProcessor INHERITED;
};
#endif
