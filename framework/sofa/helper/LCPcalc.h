#ifndef SOFA_HELPER_LCPCALC_H
#define SOFA_HELPER_LCPCALC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace sofa
{

namespace helper
{

//#define NULL 0

#define EPSILON_LCP		0.00000000001	// epsilon pour tests = 0
#define EPSILON_CONV	0.001			// for GS convergence
#define MAX_BOU	50	// nombre maximal de boucles de calcul

inline void set3Dof(double *vector, int index, double &vx, double &vy, double &vz)
{vector[3*index]=vx; vector[3*index+1]=vy; vector[3*index+2]=vz;}
inline void add3Dof(double *vector, int index, double &vx, double &vy, double &vz)
{vector[3*index]+=vx; vector[3*index+1]+=vy; vector[3*index+2]+=vz;}
inline double normError(double &f1x, double &f1y, double &f1z, double &f2x, double &f2y, double &f2z)
{
    return sqrt( ((f2x-f1x)*(f2x-f1x) + (f2y-f1y)*(f2y-f1y) + (f2z-f1z)*(f2z-f1z)) /
            (f1x*f1x + f1y*f1y + f1z*f1z) ) ;
}

inline double absError(double &f1x, double &f1y, double &f1z, double &f2x, double &f2y, double &f2z)
{return sqrt ((f2x-f1x)*(f2x-f1x) + (f2y-f1y)*(f2y-f1y) + (f2z-f1z)*(f2z-f1z));}


int resoudreLCP(int, double *, double **, double *);
int lcp_lexicolemke(int, double *, double **, double *);
// same with pre-allocated matrix A
int lcp_lexicolemke(int, double *, double **, double **, double *);


void afficheSyst(double *q,double **M, int *base, double **mat, int dim);
void afficheLCP(double *q, double **M, int dim);
void afficheLCP(double *q, double **M, double *f, int dim);

typedef double FemClipsReal;
void gaussSeidelLCP1(int, FemClipsReal *,FemClipsReal **, FemClipsReal *, double &, int &);

// inverted SymMatrix 3x3 //
class LocalBlock33
{
public:
    LocalBlock33() {computed=false;};
    ~LocalBlock33() {};

    void compute(double &w11, double &w12, double &w13, double &w22, double &w23, double &w33);
    void stickState(double &dn, double &dt, double &ds, double &fn, double &ft, double &fs);
    void slipState(double &mu, double &dn, double &dt, double &ds, double &fn, double &ft, double &fs);

    // computation of a new state using a simple gauss-seidel loop
    void GS_State(double &mu, double &dn, double &dt, double &ds, double &fn, double &ft, double &fs);

    void setPreviousForce(double &fn, double &ft, double &fs) {f_1[0]=fn; f_1[1]=ft; f_1[2]=fs;}

    bool computed;
private:

    double w[6];
    double wInv[6];
    double det;
    double f_1[3]; // previous value of force
};

// Gauss-Seidel like algorithm for contacts
int nlcp_gaussseidel(int, double *, double**, double *, double &, double &, int &, bool );

} // namespace helper

} // namespace sofa

#endif
