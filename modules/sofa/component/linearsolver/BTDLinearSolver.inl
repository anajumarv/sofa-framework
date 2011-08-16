#ifndef SOFA_COMPONENT_LINEARSOLVER_BTDLINEARSOLVER_INL
#define SOFA_COMPONENT_LINEARSOLVER_BTDLINEARSOLVER_INL

#include "BTDLinearSolver.h"



namespace sofa
{

namespace component
{

namespace linearsolver
{


/// Factorize M
///
///     [ A0 C0 0  0  ]         [ a0 0  0  0  ] [ I  l0 0  0  ]
/// M = [ B1 A1 C1 0  ] = L U = [ B1 a1 0  0  ] [ 0  I  l1 0  ]
///     [ 0  B2 A2 C2 ]         [ 0  B2 a2 0  ] [ 0  0  I  l2 ]
///     [ 0  0  B3 A3 ]         [ 0  0  B3 a3 ] [ 0  0  0  I  ]
///     [ a0 a0l0    0       0       ]
/// M = [ B1 B1l0+a1 a1l1    0       ]
///     [ 0  B2      B2l1+a2 a2l2    ]
///     [ 0  0       B3      B3l2+a3 ]
/// L X = [ a0X0 B1X0+a1X1 B2X1+a2X2 B3X2+a3X3 ]
///        [                       inva0                   0             0     0 ]
/// Linv = [               -inva1B1inva0               inva1             0     0 ]
///        [         inva2B2inva1B1inva0       -inva2B2inva1         inva2     0 ]
///        [ -inva3B3inva2B2inva1B1inva0 inva3B3inva2B2inva1 -inva3B3inva2 inva3 ]
/// U X = [ X0+l0X1 X1+l1X2 X2+l2X3 X3 ]
/// Uinv = [ I -l0 l0l1 -l0l1l2 ]
///        [ 0   I  -l1    l1l2 ]
///        [ 0   0    I     -l2 ]
///        [ 0   0    0       I ]
///
///                    [ (I+l0(I+l1(I+l2inva3B3)inva2B2)inva1B1)inva0 -l0(I+l1(I+l2inva3B3)inva2B2)inva1 l0l1(inva2+l2inva3B3inva2) -l0l1l2inva3 ]
/// Minv = Uinv Linv = [    -((I+l1(I+l2inva3B3)inva2B2)inva1B1)inva0    (I+l1(I+l2inva3B3)inva2B2)inva1  -l1(inva2+l2inva3B3inva2)    l1l2inva3 ]
///                    [         (((I+l2inva3B3)inva2B2)inva1B1)inva0       -((I+l2inva3B3)inva2B2)inva1      inva2+l2inva3B3inva2     -l2inva3 ]
///                    [                  -inva3B3inva2B2inva1B1inva0                inva3B3inva2B2inva1             -inva3B3inva2        inva3 ]
///
///                    [ inva0-l0(Minv10)              (-l0)(Minv11)              (-l0)(Minv12)           (-l0)(Minv13) ]
/// Minv = Uinv Linv = [         (Minv11)(-B1inva0) inva1-l1(Minv21)              (-l1)(Minv22)           (-l1)(Minv23) ]
///                    [         (Minv21)(-B1inva0)         (Minv22)(-B2inva1) inva2-l2(Minv32)           (-l2)(Minv33) ]
///                    [         (Minv31)(-B1inva0)         (Minv32)(-B2inva1)         (Minv33)(-B3inva2)       inva3   ]
///
/// if M is symmetric (Ai = Ait and Bi+1 = C1t) :
/// li = invai*Ci = (invai)t*(Bi+1)t = (B(i+1)invai)t
///
///                    [ inva0-l0(Minv11)(-l0t)     Minv10t          Minv20t      Minv30t ]
/// Minv = Uinv Linv = [  (Minv11)(-l0t)  inva1-l1(Minv22)(-l1t)     Minv21t      Minv31t ]
///                    [  (Minv21)(-l0t)   (Minv22)(-l1t)  inva2-l2(Minv33)(-l2t) Minv32t ]
///                    [  (Minv31)(-l0t)   (Minv32)(-l1t)   (Minv33)(-l2t)   inva3  ]
///
template<class Matrix, class Vector>
void BTDLinearSolver<Matrix,Vector>::my_identity(SubMatrix& Id, const int size_id)
{
    Id.resize(size_id,size_id);
    for (int i=0; i<size_id; i++)
        Id.set(i,i,1.0);
}

template<class Matrix, class Vector>
void BTDLinearSolver<Matrix,Vector>::invert(SubMatrix& Inv, const BlocType& m)
{
    SubMatrix M;
    M = m;
    // Check for diagonal matrices
    unsigned int i0 = 0;
    const unsigned int n = M.Nrows();
    Inv.resize(n,n);
    while (i0 < n)
    {
        unsigned int j0 = i0+1;
        double eps = M.element(i0,i0)*1.0e-10;
        while (j0 < n)
            if (fabs(M.element(i0,j0)) > eps) break;
            else ++j0;
        if (j0 == n)
        {
            // i0 row is the identity
            Inv.set(i0,i0,(float)1.0/M.element(i0,i0));
            ++i0;
        }
        else break;
    }
    if (i0 < n)
//if (i0 == 0)
        Inv = M.i();
    //else if (i0 < n)
    //        Inv.sub(i0,i0,n-i0,n-i0) = M.sub(i0,i0,n-i0,n-i0).i();
    //else return true;
    //return false;
}

template<class Matrix, class Vector>
void BTDLinearSolver<Matrix,Vector>::invert(Matrix& M)
{
    const bool verbose  = this->f_verbose.getValue() || this->f_printLog.getValue();

    if( verbose )
    {
        serr<<"BTDLinearSolver, invert Matrix = "<< M <<sendl;
    }

    const int bsize = Matrix::getSubMatrixDim(f_blockSize.getValue());
    const int nb = M.rowSize() / bsize;
    if (nb == 0) return;
    //alpha.resize(nb);
    alpha_inv.resize(nb);
    lambda.resize(nb-1);
    B.resize(nb);

    /////////////////////////// subpartSolve init ////////////

    if(subpartSolve.getValue() )
    {
        this->init_partial_inverse(nb,bsize);
    }

    SubMatrix A, C;
    //int ndiag = 0;
    M.getAlignedSubMatrix(0,0,bsize,bsize,A);
    //if (verbose) sout << "A[0] = " << A << sendl;
    M.getAlignedSubMatrix(0,1,bsize,bsize,C);
    //if (verbose) sout << "C[0] = " << C << sendl;
    //alpha[0] = A;
    invert(alpha_inv[0],A);
    if (verbose) sout << "alpha_inv[0] = " << alpha_inv[0] << sendl;
    lambda[0] = alpha_inv[0]*C;
    if (verbose) sout << "lambda[0] = " << lambda[0] << sendl;
    //if (verbose) sout << "C[0] = alpha[0]*lambda[0] = " << alpha[0]*lambda[0] << sendl;


    for (int i=1; i<nb; ++i)
    {
        M.getAlignedSubMatrix((i  ),(i  ),bsize,bsize,A);
        //if (verbose) sout << "A["<<i<<"] = " << A << sendl;
        M.getAlignedSubMatrix((i  ),(i-1),bsize,bsize,B[i]);
        //if (verbose) sout << "B["<<i<<"] = " << B[i] << sendl;
        //alpha[i] = (A - B[i]*lambda[i-1]);


        BlocType Temp1= B[i]*lambda[i-1];
        BlocType Temp2= A - Temp1;
        invert(alpha_inv[i], Temp2);


        //if(subpartSolve.getValue() ) {
        //	helper::vector<SubMatrix> nHn_1; // bizarre: pb compilation avec SubMatrix nHn_1 = B[i] *alpha_inv[i];
        //	nHn_1.resize(1);
        //	nHn_1[0] = B[i] *alpha_inv[i-1];
        //	H.insert(make_pair(IndexPair(i,i-1),nHn_1[0])); //IndexPair(i+1,i) ??
        //	serr<<" Add pair ("<<i<<","<<i-1<<")"<<sendl;
        //}

        if (verbose) sout << "alpha_inv["<<i<<"] = " << alpha_inv[i] << sendl;
        //if (verbose) sout << "A["<<i<<"] = B["<<i<<"]*lambda["<<i-1<<"]+alpha["<<i<<"] = " << B[i]*lambda[i-1]+alpha[i] << sendl;
        if (i<nb-1)
        {
            M.getAlignedSubMatrix((i  ),(i+1),bsize,bsize,C);
            //if (verbose) sout << "C["<<i<<"] = " << C << sendl;
            lambda[i] = alpha_inv[i]*C;
            if (verbose) sout << "lambda["<<i<<"] = " << lambda[i] << sendl;
            //if (verbose) sout << "C["<<i<<"] = alpha["<<i<<"]*lambda["<<i<<"] = " << alpha[i]*lambda[i] << sendl;
        }
    }
    nBlockComputedMinv.resize(nb);
    for (int i=0; i<nb; ++i)
        nBlockComputedMinv[i] = 0;

    // WARNING : cost of resize here : ???
    Minv.resize(nb*bsize,nb*bsize);
    Minv.setAlignedSubMatrix((nb-1),(nb-1),bsize,bsize,alpha_inv[nb-1]);

    //std::cout<<"Minv.setSubMatrix call for block number"<<(nb-1)<<std::endl;

    nBlockComputedMinv[nb-1] = 1;

    if(subpartSolve.getValue() )
    {
        SubMatrix iHi; // bizarre: pb compilation avec SubMatrix nHn_1 = B[i] *alpha_inv[i];
        my_identity(iHi, bsize);
        H.insert( make_pair(  IndexPair(nb-1, nb-1), iHi  ) );

        // on calcule les blocks diagonaux jusqu'au bout!!
        // TODO : ajouter un compteur "first_block" qui évite de descendre les déplacements jusqu'au block 0 dans partial_solve si ce block n'a pas été appelé
        computeMinvBlock(0, 0);
    }

    //sout << "BTDLinearSolver: "<<ndiag<<"/"<<nb<<"diagonal blocs."<<sendl;
}



///
///                    [ inva0-l0(Minv10)     Minv10t          Minv20t      Minv30t ]
/// Minv = Uinv Linv = [  (Minv11)(-l0t)  inva1-l1(Minv21)     Minv21t      Minv31t ]
///                    [  (Minv21)(-l0t)   (Minv22)(-l1t)  inva2-l2(Minv32) Minv32t ]
///                    [  (Minv31)(-l0t)   (Minv32)(-l1t)   (Minv33)(-l2t)   inva3  ]
///

template<class Matrix, class Vector>
void BTDLinearSolver<Matrix,Vector>::computeMinvBlock(int i, int j)
{
    //serr<<"computeMinvBlock("<<i<<","<<j<<")"<<sendl;

    if (i < j)
    {
        // i < j correspond to the upper diagonal
        // for the computation, we use the lower diagonal matrix
        int t = i; i = j; j = t;
    }
    if (nBlockComputedMinv[i] > i-j) return; // the block was already computed



    ///// the block was not computed yet :

    // the block is computed now :
    // 1. all the diagonal block between N and i need to be computed
    const int bsize = Matrix::getSubMatrixDim(f_blockSize.getValue());
    int i0 = i;
    while (nBlockComputedMinv[i0]==0)
        ++i0;
    // i0 is the "closest" block of the diagonal that is computed
    // we need to compute all the Minv[i0][i0] (with i0>=i) till i0=i
    while (i0 > i)
    {
        //serr<<"i0 ="<<i0<<"nBlockComputedMinv[i0]="<<nBlockComputedMinv[i0]<<sendl;
        if (nBlockComputedMinv[i0] == 1) // only the bloc on the diagonal is computed : need of the the bloc [i0][i0-1]
        {
            // compute bloc (i0,i0-1)
            //Minv[i0][i0-1] = Minv[i0][i0]*-L[i0-1].t()
            Minv.asub((i0  ),(i0-1),bsize,bsize) = Minv.asub((i0  ),(i0  ),bsize,bsize)*(-(lambda[i0-1].t()));
            ++nBlockComputedMinv[i0];

            if(subpartSolve.getValue() )
            {
                // store -L[i0-1].t() H structure
                SubMatrix iHi_1;
                iHi_1 = - lambda[i0-1].t();
                H.insert( make_pair(  IndexPair(i0, i0-1), iHi_1  ) );
                // compute bloc (i0,i0-1) :  the upper diagonal blocks Minv[i0-1][i0]
                Minv.asub((i0-1),(i0),bsize,bsize) = -lambda[i0-1] * Minv.asub((i0  ),(i0  ),bsize,bsize);
            }

        }


        // compute bloc (i0-1,i0-1)  : //Minv[i0-1][i0-1] = inv(M[i0-1][i0-1]) + L[i0-1] * Minv[i0][i0-1]
        Minv.asub((i0-1),(i0-1),bsize,bsize) = alpha_inv[i0-1] - lambda[i0-1]*Minv.asub((i0  ),(i0-1),bsize,bsize);

        if(subpartSolve.getValue() )
        {
            // store Id in H structure
            SubMatrix iHi;
            my_identity(iHi, bsize);
            H.insert( make_pair(  IndexPair(i0-1, i0-1), iHi  ) );
        }

        ++nBlockComputedMinv[i0-1]; // now Minv[i0-1][i0-1] is computed so   nBlockComputedMinv[i0-1] = 1
        --i0;                       // we can go down to the following block (till we reach i)
    }


    //2. all the block on the lines of block i between the diagonal and the block j are computed
    // i0=i

    int j0 = i-nBlockComputedMinv[i];


    /////////////// ADD : Calcul pour faire du partial_solve //////////
    // first iHj is initiallized to iHj0+1 (that is supposed to be already computed)
    SubMatrix iHj ;
    if(subpartSolve.getValue() )
    {


        H_it = H.find( IndexPair(i0,j0+1) );
        //serr<<" find pair ("<<i<<","<<j0+1<<")"<<sendl;

        if (H_it == H.end())
        {
            my_identity(iHj, bsize);
            if (i0!=j0+1)
                serr<<"WARNING !! element("<<i0<<","<<j0+1<<") not found : nBlockComputedMinv[i] = "<<nBlockComputedMinv[i]<<sendl;
        }
        else
        {
            //serr<<"element("<<i0<<","<<j0+1<<")  found )!"<<sendl;
            iHj = H_it->second;
        }

    }
    /////////////////////////////////////////////////////////////////////

    while (j0 >= j)
    {
        // compute bloc (i0,j0)
        // Minv[i][j0] = Minv[i][j0+1] * (-L[j0].t)
        Minv.asub((i0  ),(j0  ),bsize,bsize) = Minv.asub((i0  ),(j0+1),bsize,bsize)*(-lambda[j0].t());
        if(subpartSolve.getValue() )
        {
            // iHj0 = iHj0+1 * (-L[j0].t)
            iHj = iHj * -lambda[j0].t();
            H.insert(make_pair(IndexPair(i0,j0),iHj));

            // compute bloc (j0,i0)  the upper diagonal blocks Minv[j0][i0]
            Minv.asub((j0  ),(i0  ),bsize,bsize) = -lambda[j0]*Minv.asub((j0+1),(i0),bsize,bsize);
        }
        ++nBlockComputedMinv[i0];
        --j0;
    }
}

template<class Matrix, class Vector>
double BTDLinearSolver<Matrix,Vector>::getMinvElement(int i, int j)
{
    const int bsize = Matrix::getSubMatrixDim(f_blockSize.getValue());
    if (i < j)
    {
        // lower diagonal
        return getMinvElement(j,i);
    }
    computeMinvBlock(i/bsize, j/bsize);
    return Minv.element(i,j);
}

template<class Matrix, class Vector>
void BTDLinearSolver<Matrix,Vector>::solve (Matrix& /*M*/, Vector& x, Vector& b)
{
    const bool verbose  = this->f_verbose.getValue() || this->f_printLog.getValue();

    if( verbose )
    {
        serr<<"BTDLinearSolver, b = "<< b <<sendl;
    }

    //invert(M);

    const int bsize = Matrix::getSubMatrixDim(f_blockSize.getValue());
    const int nb = b.size() / bsize;
    if (nb == 0) return;

    //if (verbose) sout << "D["<<0<<"] = " << b.asub(0,bsize) << sendl;
    x.asub(0,bsize) = alpha_inv[0] * b.asub(0,bsize);
    //if (verbose) sout << "Y["<<0<<"] = " << x.asub(0,bsize) << sendl;
    for (int i=1; i<nb; ++i)
    {
        //if (verbose) sout << "D["<<i<<"] = " << b.asub(i,bsize) << sendl;
        x.asub(i,bsize) = alpha_inv[i]*(b.asub(i,bsize) - B[i]*x.asub((i-1),bsize));
        //if (verbose) sout << "Y["<<i<<"] = " << x.asub(i,bsize) << sendl;
    }
    //x.asub((nb-1),bsize) = Y.asub((nb-1),bsize);
    //if (verbose) sout << "x["<<nb-1<<"] = " << x.asub((nb-1),bsize) << sendl;
    for (int i=nb-2; i>=0; --i)
    {
        x.asub(i,bsize) /* = Y.asub(i,bsize)- */ -= lambda[i]*x.asub((i+1),bsize);
        //if (verbose) sout << "x["<<i<<"] = " << x.asub(i,bsize) << sendl;
    }

    // x is the solution of the system
    if( verbose )
    {
        serr<<"BTDLinearSolver::solve, solution = "<<x<<sendl;
    }
}

template<class Matrix, class Vector>
bool BTDLinearSolver<Matrix,Vector>::addJMInvJt(defaulttype::BaseMatrix* result, defaulttype::BaseMatrix* J, double fact)
{
    if (FullMatrix<double>* r = dynamic_cast<FullMatrix<double>*>(result))
    {
        if (SparseMatrix<double>* j = dynamic_cast<SparseMatrix<double>*>(J))
        {
            return addJMInvJt(*r,*j,fact);
        }
        else if (SparseMatrix<float>* j = dynamic_cast<SparseMatrix<float>*>(J))
        {
            return addJMInvJt(*r,*j,fact);
        }
    }
    else if (FullMatrix<float>* r = dynamic_cast<FullMatrix<float>*>(result))
    {
        if (SparseMatrix<double>* j = dynamic_cast<SparseMatrix<double>*>(J))
        {
            return addJMInvJt(*r,*j,fact);
        }
        else if (SparseMatrix<float>* j = dynamic_cast<SparseMatrix<float>*>(J))
        {
            return addJMInvJt(*r,*j,fact);
        }
    }
    else if (defaulttype::BaseMatrix* r = result)
    {
        if (SparseMatrix<double>* j = dynamic_cast<SparseMatrix<double>*>(J))
        {
            return addJMInvJt(*r,*j,fact);
        }
        else if (SparseMatrix<float>* j = dynamic_cast<SparseMatrix<float>*>(J))
        {
            return addJMInvJt(*r,*j,fact);
        }
    }
    return false;
}



///////////////////////////////////////
///////  partial solve  //////////
///////////////////////////////////////


template<class Matrix, class Vector>
void BTDLinearSolver<Matrix,Vector>::init_partial_inverse(const int &/*nb*/, const int &/*bsize*/)
{
    // need to stay in init_partial_inverse (called before inverse)
    H.clear();

}

template<class Matrix, class Vector>
void BTDLinearSolver<Matrix,Vector>::init_partial_solve()
{

    const int bsize = Matrix::getSubMatrixDim(f_blockSize.getValue());
    const int nb = this->currentGroup->systemRHVector->size() / bsize;

    //TODO => optimisation ??
    bwdContributionOnLH.clear();
    bwdContributionOnLH.resize(nb*bsize);
    fwdContributionOnRH.clear();
    fwdContributionOnRH.resize(nb*bsize);


    _rh_buf.resize(nb*bsize);
    _acc_rh_bloc=0;
    _acc_rh_bloc.resize(bsize);
    _acc_lh_bloc=0;
    _acc_lh_bloc.resize(bsize);

    // Bloc that is currently being proceed => start from the end (so that we use step2 bwdAccumulateLHGlobal and accumulate potential initial forces)
    current_bloc = nb-1;


    // DF represents the variation of the right hand side of the equation (Force in mechanics)
    Vec_dRH.resize(nb);
    for (int i=0; i<nb; i++)
    {
        Vec_dRH[i]=0;
        Vec_dRH[i].resize(bsize);
        _rh_buf.asub(i,bsize) = this->currentGroup->systemRHVector->asub(i,bsize) ;

    }




}



template<class Matrix, class Vector>
void BTDLinearSolver<Matrix,Vector>::bwdAccumulateRHinBloc(int indMaxBloc)
{
    const int bsize = Matrix::getSubMatrixDim(f_blockSize.getValue());

    int b=indMaxBloc;

    //debug
    if (indMaxBloc <  current_bloc)
    {
        std::cout<<" WARNING in bwdAccumulateRHinBloc : indMaxBloc = "<<indMaxBloc <<" <  "<<" current_bloc = "<<current_bloc<<std::endl;
    }

    std::cout<<" in step 1: start accumulating from bloc "<<b<<std::endl;

    SubVector RHbloc;
    RHbloc.resize(bsize);

    _acc_lh_bloc=0;


    while(b > current_bloc )
    {

        // evaluate the Right Hand Term for the bloc b
        RHbloc = this->currentGroup->systemRHVector->asub(b,bsize) ;

        // compute the contribution on LH created by RH
        _acc_lh_bloc  += Minv.asub(b,b,bsize,bsize) * RHbloc;


        b--;
        // accumulate this contribution on LH on the lower blocs
        _acc_lh_bloc =  -(lambda[b]*_acc_lh_bloc);



        // store the contribution as bwdContributionOnLH
        bwdContributionOnLH.asub(b,bsize) = _acc_lh_bloc;

    }

    // here b==current_bloc
}

template<class Matrix, class Vector>
void BTDLinearSolver<Matrix,Vector>::bwdAccumulateLHGlobal( )
{
    const int bsize = Matrix::getSubMatrixDim(f_blockSize.getValue());
    _acc_lh_bloc =  bwdContributionOnLH.asub(current_bloc, bsize);

    while( current_bloc > 0)
    {
        // BwdLH += Minv*RH
        _acc_lh_bloc +=  Minv.asub(current_bloc,current_bloc,bsize,bsize) * this->currentGroup->systemRHVector->asub(current_bloc,bsize) ;

        current_bloc--;
        // BwdLH(n-1) = H(n-1)(n)*BwdLH(n)
        _acc_lh_bloc = -(lambda[current_bloc]*_acc_lh_bloc);

        bwdContributionOnLH.asub(current_bloc, bsize) = _acc_lh_bloc;
    }

    // at this point, current_bloc must be equal to 0

    // all the forces from RH were accumulated through bwdAccumulation:
    _indMaxNonNullForce = 0;

    // need to update all the value of LH during forward
    _indMaxLHComputed = 0;

    // init fwdContribution
    fwdContributionOnRH.asub(0, bsize) = 0;


}




template<class Matrix, class Vector>
void BTDLinearSolver<Matrix,Vector>::fwdAccumulateRHGlobal(int indMinBloc)
{
    const int bsize = Matrix::getSubMatrixDim(f_blockSize.getValue());
    _acc_rh_bloc =fwdContributionOnRH.asub(current_bloc, bsize);

    while( current_bloc< indMinBloc)
    {

        // fwdRH(n) += RH(n)
        _acc_rh_bloc += this->currentGroup->systemRHVector->asub(current_bloc,bsize);

        // fwdRH(n+1) = H(n+1)(n) * fwdRH(n)
        _acc_rh_bloc = -(lambda[current_bloc].t() * _acc_rh_bloc);
        current_bloc++;

        fwdContributionOnRH.asub(current_bloc, bsize) = _acc_rh_bloc;

    }

}


template<class Matrix, class Vector>
void BTDLinearSolver<Matrix,Vector>::fwdComputeLHinBloc(int indMaxBloc)
{

    const int bsize = Matrix::getSubMatrixDim(f_blockSize.getValue());

    while(_indMaxLHComputed < indMaxBloc )
    {
        int b = _indMaxLHComputed;

        if(b>=0)
        {
            // fwdRH(n+1) = H(n+1)(n) * (fwdRH(n) + RH(n))
            fwdContributionOnRH.asub(b+1, bsize) = (-lambda[b].t())* ( fwdContributionOnRH.asub(b, bsize) + this->currentGroup->systemRHVector->asub(b,bsize) ) ;
        }

        _indMaxLHComputed++; b++;

        // compute the bloc which indice is _indMaxLHComputed
        this->currentGroup->systemLHVector->asub(b,bsize) = Minv.asub( b, b ,bsize,bsize) * ( fwdContributionOnRH.asub(b, bsize) + this->currentGroup->systemRHVector->asub(b,bsize) ) +
                bwdContributionOnLH.asub(b, bsize);


    }


}

template<class Matrix, class Vector>
void BTDLinearSolver<Matrix,Vector>::partial_solve(ListIndex&  Iout, ListIndex&  Iin , bool NewIn)  ///*Matrix& M, Vector& result, Vector& rh, */
{





    int MinIdBloc_OUT = Iout.front();
    int MaxIdBloc_OUT = Iout.back();


    std::cout<<"partial_solve: need update on position for bloc between dofs "<< MinIdBloc_OUT<< "  and "<<MaxIdBloc_OUT<<std::endl;
    if (verification.getValue())
    {
        const int bsize = Matrix::getSubMatrixDim(f_blockSize.getValue());
        std::cout<<" input Force= ";
        for (int i=MinIdBloc_OUT; i<=MaxIdBloc_OUT; i++)
        {
            std::cout<<"     ["<<i<<"] "<<this->currentGroup->systemRHVector->asub(i,bsize);
        }
        std::cout<<" "<<std::endl;
    }


    if( NewIn)
    {

        int MinIdBloc_IN = Iin.front(); //  Iin needs to be sorted
        int MaxIdBloc_IN = Iin.back();  //



        //debug
        std::cout<<"STEP1: new force on bloc between dofs "<< MinIdBloc_IN<< "  and "<<MaxIdBloc_IN<<std::endl;

        if (MaxIdBloc_IN > this->_indMaxNonNullForce)
            this->_indMaxNonNullForce = MaxIdBloc_IN;

        //step 1:
        bwdAccumulateRHinBloc(this->_indMaxNonNullForce );

        // need to update (in step 4) all the values corresponding to the bloc
        this->_indMaxLHComputed = MinIdBloc_IN -1;

    }


    if (current_bloc > MinIdBloc_OUT)
    {
        //debug
        std::cout<<"STEP2 (bwd GLOBAL on structure) : current_bloc ="<<current_bloc<<" > to  MinIdBloc_OUT ="<<MinIdBloc_OUT<<std::endl;

        // step 2:
        bwdAccumulateLHGlobal();

        //debug
        std::cout<<" new current_bloc = "<<current_bloc<<std::endl;
    }


    if (current_bloc < MinIdBloc_OUT)
    {
        //debug
        std::cout<<"STEP3 (fwd GLOBAL on structure) : current_bloc ="<<current_bloc<<" < to  MinIdBloc_OUT ="<<MinIdBloc_OUT<<std::endl;

        //step 3:
        fwdAccumulateRHGlobal(MinIdBloc_OUT);

        // debug
        std::cout<<" new current_bloc = "<<current_bloc<<std::endl;
    }



    if ( _indMaxLHComputed < MaxIdBloc_OUT)
    {
        //debug
        std::cout<<" STEP 4 :_indMaxLHComputed = "<<_indMaxLHComputed<<" < "<<"MaxIdBloc_OUT = "<<MaxIdBloc_OUT<<"  - verify that current_bloc="<<current_bloc<<" == "<<" MinIdBloc_OUT ="<<MinIdBloc_OUT<<std::endl;

        fwdComputeLHinBloc(MaxIdBloc_OUT );

        std::cout<<"  new _indMaxLHComputed = "<<_indMaxLHComputed<<std::endl;
    }








    // debug: test
    if (verification.getValue())
    {
        const int bsize = Matrix::getSubMatrixDim(f_blockSize.getValue());
        std::cout<<" Found solution for bloc OUT :";
        for (int i=MinIdBloc_OUT; i<=MaxIdBloc_OUT; i++)
        {
            std::cout<<"     ["<<i<<"] "<<this->currentGroup->systemLHVector->asub(i,bsize);
        }
        std::cout<<std::endl;

        solve(*this->currentGroup->systemMatrix,*this->currentGroup->systemLHVector, *this->currentGroup->systemRHVector);

        std::cout<<" after complete resolution OUT :";
        for (int i=MinIdBloc_OUT; i<=MaxIdBloc_OUT; i++)
        {
            std::cout<<"     ["<<i<<"] "<<this->currentGroup->systemLHVector->asub(i,bsize);
        }
        std::cout<<std::endl;


        return;
    }



}





template<class Matrix, class Vector>
template<class RMatrix, class JMatrix>
bool BTDLinearSolver<Matrix,Vector>::addJMInvJt(RMatrix& result, JMatrix& J, double fact)
{
    //const int Jrows = J.rowSize();
    const unsigned int Jcols = J.colSize();
    if (Jcols != Minv.rowSize())
    {
        serr << "BTDLinearSolver::addJMInvJt ERROR: incompatible J matrix size." << sendl;
        return false;
    }


#if 0
// WARNING !!!
    //Getting all elements of Minv modifies the obtained Matrix "result"!!
    // It seems that result is computed more accurately.
    // There is a BUG to find here...
    if (!problem.getValue())
    {
        for  (int mr=0; mr<Minv.rowSize(); mr++)
        {
            for (int mc=0; mc<Minv.colSize(); mc++)
            {
                getMinvElement(mr,mc);
            }
        }
    }
////////////////////////////////////////////
#endif
    if (f_verbose.getValue())
    {
// debug christian: print of the inverse matrix:
        sout<< "C = ["<<sendl;
        for  (unsigned int mr=0; mr<Minv.rowSize(); mr++)
        {
            sout<<" "<<sendl;
            for (unsigned int mc=0; mc<Minv.colSize(); mc++)
            {
                sout<<" "<< getMinvElement(mr,mc);
            }
        }
        sout<< "];"<<sendl;

// debug christian: print of matrix J:
        sout<< "J = ["<<sendl;
        for  (unsigned int jr=0; jr<J.rowSize(); jr++)
        {
            sout<<" "<<sendl;
            for (unsigned int jc=0; jc<J.colSize(); jc++)
            {
                sout<<" "<< J.element(jr, jc) ;
            }
        }
        sout<< "];"<<sendl;
    }


    const typename JMatrix::LineConstIterator jitend = J.end();
    for (typename JMatrix::LineConstIterator jit1 = J.begin(); jit1 != jitend; ++jit1)
    {
        int row1 = jit1->first;
        for (typename JMatrix::LineConstIterator jit2 = jit1; jit2 != jitend; ++jit2)
        {
            int row2 = jit2->first;
            double acc = 0.0;
            for (typename JMatrix::LElementConstIterator i1 = jit1->second.begin(), i1end = jit1->second.end(); i1 != i1end; ++i1)
            {
                int col1 = i1->first;
                double val1 = i1->second;
                for (typename JMatrix::LElementConstIterator i2 = jit2->second.begin(), i2end = jit2->second.end(); i2 != i2end; ++i2)
                {
                    int col2 = i2->first;
                    double val2 = i2->second;
                    acc += val1 * getMinvElement(col1,col2) * val2;
                }
            }
            sout << "W("<<row1<<","<<row2<<") += "<<acc<<" * "<<fact<<sendl;
            acc *= fact;
            result.add(row1,row2,acc);
            if (row1!=row2)
                result.add(row2,row1,acc);
        }
    }
    return true;
}

template<> const char* BTDMatrix<1,float>::Name() { return "BTDMatrix1f"; }
template<> const char* BTDMatrix<1,double>::Name() { return "BTDMatrix1d"; }
template<> const char* BTDMatrix<2,float>::Name() { return "BTDMatrix2f"; }
template<> const char* BTDMatrix<2,double>::Name() { return "BTDMatrix2d"; }
template<> const char* BTDMatrix<3,float>::Name() { return "BTDMatrix3f"; }
template<> const char* BTDMatrix<3,double>::Name() { return "BTDMatrix3d"; }
template<> const char* BTDMatrix<4,float>::Name() { return "BTDMatrix4f"; }
template<> const char* BTDMatrix<4,double>::Name() { return "BTDMatrix4d"; }
template<> const char* BTDMatrix<5,float>::Name() { return "BTDMatrix5f"; }
template<> const char* BTDMatrix<5,double>::Name() { return "BTDMatrix5d"; }
template<> const char* BTDMatrix<6,float>::Name() { return "BTDMatrix6f"; }
template<> const char* BTDMatrix<6,double>::Name() { return "BTDMatrix6d"; }

} // namespace linearsolver

} // namespace component

} // namespace sofa

#endif
