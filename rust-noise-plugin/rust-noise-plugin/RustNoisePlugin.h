#pragma once
#include "Foundation.h"

class RustNoisePlugin : public MPxCommand
{
public:
    RustNoisePlugin();
    ~RustNoisePlugin() override;
    static void* creator();
    MStatus doIt(const MArgList& args) override;
    static MSyntax createSyntax();
private:
    MStatus rustNoise(MString objectName, double displacementNum);
};
