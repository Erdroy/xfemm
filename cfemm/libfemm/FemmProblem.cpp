#include "FemmProblem.h"

femm::FemmProblem::~FemmProblem()
{
}

femm::FemmProblem::FemmProblem()
    : FileFormat(-1)
    , Frequency(0.0)
    , Precision(1.e-08)
    , MinAngle(0)
    , Depth(-1)
    , LengthUnits(LengthInches)
    , Coords(CART)
    , ProblemType(PLANAR)
    , extZo(0)
    , extRo(0)
    , extRi(0)
    , comment()
    , ACSolver(0)
    , dT(0)
    , PrevSoln()
    , DoForceMaxMeshArea(false)
    , nodeproplist()
    , lineproplist()
    , blockproplist()
    , circproplist()
    , labellist()
    , nodelist()
    , arclist()
    , solved(false)
    , meshnodes()
    , meshelems()
    , pathName()
{}
