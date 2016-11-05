
#include "hmesh.h"
#include "fparse.h"

#include <string>
#include <iostream>
using std::string;
using namespace femm;

CHMaterialProp::CHMaterialProp()
    : Kx(1)
    , Ky(1)
    , Kt(0)
    , qv(0)
    , npts(0)
{
}

CHMaterialProp::~CHMaterialProp()
{

}

CHMaterialProp::CHMaterialProp( const CHMaterialProp & other)
{
    Kx = other.Kx;
    Ky = other.Kx;
	Kt = other.Kt;  // volumetric heat capacity
	qv = other.qv;  // volume heat generation

	// properties for nonlinear conductivity
	npts = other.npts;			// number of points in the nonlinear conductivity curve

	for (int i = 0; i < npts; i++)
	{
	    // copy the thermal conductivity data points.
        Kn[i] = other.Kn[i];
	}
}

CComplex CHMaterialProp::GetK(double t)
{
	int i,j;

	// Kx returned as real part;
	// Ky returned as imag part

	if (npts==0) return (Kx+I*Ky);
	if (npts==1) return (Im(Kn[0])*(1+I));
	if (t<=Re(Kn[0])) return (Im(Kn[0])*(1+I));
	if (t>=Re(Kn[npts-1])) return (Im(Kn[npts-1])*(1+I));

	for(i=0,j=1;j<npts;i++,j++)
	{
		if((t>=Re(Kn[i])) && (t<=Re(Kn[j])))
		{
			return (1+I)*(Im(Kn[i])+Im(Kn[j]-Kn[i])*Re(t-Kn[i])/Re(Kn[j]-Kn[i]));
		}
	}

    return (Kx+I*Ky);
}

CHMaterialProp CHMaterialProp::fromStream(std::istream &input, std::ostream &err)
{
    CHMaterialProp prop;

    if( expectToken(input, "<beginblock>", err) )
    {
        string token;
        while (input.good() && token != "<endblock>")
        {
            nextToken(input,&token);

            if( token == "<kx>" )
            {
                expectChar(input, '=', err);
                parseValue(input, prop.Kx, err);
                continue;
            }

            if( token == "<ky>" )
            {
                expectChar(input, '=', err);
                parseValue(input, prop.Ky, err);
                continue;
            }

            if( token == "<kt>" )
            {
                expectChar(input, '=', err);
                parseValue(input, prop.Kt, err);
                continue;
            }

            if( token == "<qv>" )
            {
                expectChar(input, '=', err);
                parseValue(input, prop.qv, err);
                continue;
            }

            if( token == "<blockname>" )
            {
                expectChar(input, '=', err);
                parseString(input, &(prop.name), err);
                continue;
            }

            if( token == "<tkpoints>" )
            {
                expectChar(input, '=', err);
                parseValue(input, prop.npts, err);
                if (prop.npts > 0)
                {
                    // FIXME: make Kn variable size
                    if (prop.npts>128)
                    {
                        err << "MaterialProp exceeds maximum size! File a bug report...";
                        prop.npts = 128;
                    }
                    for(int i=0; i<prop.npts; i++)
                    {
                        input >> prop.Kn[i].re >> prop.Kn[i].im;
                    }
                }
                continue;
            }
            if( token != "<endblock>")
                err << "CHMaterialProp: unexpected token: "<<token << "\n";
        }
    }

    return prop;
}

void CHMaterialProp::toStream(std::ostream &out) const
{
    out << "  <BeginBlock>\n";
    out << "    <Kx> = " << Kx << "\n";
    out << "    <Ky> = " << Ky << "\n";
    out << "    <Kt> = " << Kt << "\n";
    out << "    <qv> = " << qv << "\n";
    if (!name.empty())
        out << "    <BlockName> = \"" << name << "\"\n";
    out << "    <TKPoints> = " << npts << "\n";
    if (npts > 0)
    {
        for(int i=0; i<npts; i++)
        {
            out << "      " << Kn[i].re << " " << Kn[i].im << "\n";
        }
    }
    out << "  <EndBlock>\n";
}


std::ostream &operator<<(std::ostream &os, const CHMaterialProp &prop)
{
    prop.toStream(os);
    return os;
}


CHPointProp CHPointProp::fromStream(std::istream &input, std::ostream &err)
{
    CHPointProp prop;

    if( expectToken(input, "<beginpoint>", err) )
    {
        string token;
        while (input.good() && token != "<endpoint>")
        {
            nextToken(input, &token);

            if( token == "<tp>" )
            {
                expectChar(input, '=', err);
                parseValue(input, prop.V, err);
                continue;
            }

            if( token == "<qp>" )
            {
                expectChar(input, '=', err);
                parseValue(input, prop.qp, err);
                continue;
            }
            if (token != "<endpoint>")
                err << "CHPointProp: unexpected token: "<<token << "\n";
        }
    }

    return prop;
}

void CHPointProp::toStream(std::ostream &out) const
{
    out << "<BeginPoint>\n";
    out << "<Tp> = " << V << "\n";
    out << "<qp> = " << qp << "\n";
    out << "<EndPoint>\n";
}

CHConductor CHConductor::fromStream(std::istream &input, std::ostream &err)
{
    CHConductor prop;

    if( expectToken(input, "<beginconductor>", err) )
    {
        string token;
        while (input.good() && token != "<endconductor>")
        {
            nextToken(input, &token);

            if( token == "<tc>" )
            {
                expectChar(input, '=', err);
                parseValue(input, prop.V, err);
                continue;
            }

            if( token == "<qc>" )
            {
                expectChar(input, '=', err);
                parseValue(input, prop.q, err);
                continue;
            }

            if( token == "<conductortype>" )
            {
                expectChar(input, '=', err);
                parseValue(input, prop.CircType, err);
                continue;
            }

            if ( token != "<endconductor>")
                err << "CHConductor: unexpected token: "<<token << "\n";
        }
    }

    return prop;
}

void CHConductor::toStream(std::ostream &out) const
{
    out << "<BeginConductor>\n";
    out << "<Tc>" << V << "\n";
    out << "<qc>" << q << "\n";
    out << "<ConductorType>" << CircType << "\n";
    if (!CircName.empty())
        out << "<ConductorName> =\"" << CircName << "\"\n";
    out << "<EndConductor>\n";
}
