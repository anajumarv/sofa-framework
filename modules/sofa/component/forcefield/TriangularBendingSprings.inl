/*******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 1       *
*                (c) 2006-2007 MGH, INRIA, USTL, UJF, CNRS                     *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Contact information: contact@sofa-framework.org                              *
*                                                                              *
* Authors: J. Allard, P-J. Bensoussan, S. Cotin, C. Duriez, H. Delingette,     *
* F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza, M. Nesme, P. Neumann,        *
* and F. Poyer                                                                 *
*******************************************************************************/
//
// C++ Implementation: TriangularBendingSprings
//
// Description:
//
//
// Author: The SOFA team </www.sofa-framework.org>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SOFA_COMPONENT_FORCEFIELD_TRIANGULARBENDINGSPRINGS_INL
#define SOFA_COMPONENT_FORCEFIELD_TRIANGULARBENDINGSPRINGS_INL

#include <sofa/component/forcefield/TriangularBendingSprings.h>
// #include <sofa/component/topology/MeshTopology.h>
#include <fstream> // for reading the file
#include <iostream> //for debugging

#include <sofa/helper/gl/template.h>
#if defined (__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <sofa/component/topology/TriangleData.inl>
#include <sofa/component/topology/EdgeData.inl>

namespace sofa
{

namespace component
{

namespace forcefield
{

using namespace sofa::defaulttype;
using namespace	sofa::component::topology;
using namespace core::componentmodel::topology;

using std::cerr;
using std::cout;
using std::endl;

using namespace core::componentmodel::behavior;

template< class DataTypes>
void TriangularBendingSprings<DataTypes>::TriangularBSEdgeCreationFunction(int /*edgeIndex*/, void* param, EdgeInformation &ei,
        const Edge& ,  const sofa::helper::vector< unsigned int > &,
        const sofa::helper::vector< double >&)
{
    TriangularBendingSprings<DataTypes> *ff= (TriangularBendingSprings<DataTypes> *)param;
    if (ff)
    {
        //TriangleSetTopology<DataTypes> *_mesh=ff->getTriangularTopology();
        //assert(_mesh!=0);
        unsigned int u,v;
        /// set to zero the edge stiffness matrix
        for (u=0; u<3; ++u)
        {
            for (v=0; v<3; ++v)
            {
                ei.DfDx[u][v]=0;
            }
        }

        ei.is_activated=false;
        ei.is_initialized=false;

    }
}

template< class DataTypes>
void TriangularBendingSprings<DataTypes>::TriangularBSTriangleCreationFunction (const sofa::helper::vector<unsigned int> &triangleAdded,
        void* param, vector<EdgeInformation> &edgeData)
{

    TriangularBendingSprings<DataTypes> *ff= (TriangularBendingSprings<DataTypes> *)param;
    if (ff)
    {
        TriangleSetTopology<DataTypes> *_mesh=ff->getTriangularTopology();
        assert(_mesh!=0);
        TriangleSetTopologyContainer *container=_mesh->getTriangleSetTopologyContainer();

        //const sofa::helper::vector< Edge > &edgeArray=container->getEdgeArray() ;
        const sofa::helper::vector< Triangle > &triangleArray=container->getTriangleArray() ;
        const sofa::helper::vector< TriangleEdges > &triangleEdgeArray=container->getTriangleEdgeArray() ;

        double m_ks=ff->getKs();
        double m_kd=ff->getKd();

        unsigned int u,v;

        unsigned int nb_activated = 0;

        const typename DataTypes::VecCoord *restPosition=_mesh->getDOF()->getX0();

        for (unsigned int i=0; i<triangleAdded.size(); ++i)
        {

            TriangleSetTopology<DataTypes> *_mesh=ff->getTriangularTopology();
            assert(_mesh!=0);
            TriangleSetTopologyContainer *container=_mesh->getTriangleSetTopologyContainer();

            sofa::component::topology::TriangleSetTopologyContainer *tstc= dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer *>(container);

            /// describe the jth edge index of triangle no i
            TriangleEdges te2 = triangleEdgeArray[triangleAdded[i]];
            /// describe the jth vertex index of triangle no i
            Triangle t2 = triangleArray[triangleAdded[i]];

            for(unsigned int j=0; j<3; ++j)
            {

                EdgeInformation &ei = edgeData[te2[j]]; // ff->edgeInfo
                if(!(ei.is_initialized))
                {

                    unsigned int edgeIndex = te2[j];
                    ei.is_activated=true;

                    /// set to zero the edge stiffness matrix
                    for (u=0; u<3; ++u)
                    {
                        for (v=0; v<3; ++v)
                        {
                            ei.DfDx[u][v]=0;
                        }
                    }

                    container->getTriangleEdgeShellArray();
                    container->getTriangleVertexShellArray();

                    const sofa::helper::vector< unsigned int > shell = tstc->getTriangleEdgeShell(edgeIndex);
                    if (shell.size()==2)
                    {

                        nb_activated+=1;

                        TriangleEdges te1;
                        Triangle t1;

                        if(shell[0] == triangleAdded[i])
                        {

                            te1 = triangleEdgeArray[shell[1]];
                            t1 = triangleArray[shell[1]];

                        }
                        else   // shell[1] == triangleAdded[i]
                        {

                            te1 = triangleEdgeArray[shell[0]];
                            t1 = triangleArray[shell[0]];
                        }

                        int i1 = tstc->getEdgeIndexInTriangle(te1, edgeIndex); //edgeIndex //te1[j]
                        int i2 = tstc->getEdgeIndexInTriangle(te2, edgeIndex); // edgeIndex //te2[j]

                        ei.m1 = t1[i1];
                        ei.m2 = t2[i2];

                        //TriangularBendingSprings<DataTypes> *fftest= (TriangularBendingSprings<DataTypes> *)param;
                        ei.ks=m_ks; //(fftest->ks).getValue();
                        ei.kd=m_kd; //(fftest->kd).getValue();

                        Coord u = (*restPosition)[ei.m1] - (*restPosition)[ei.m2];

                        Real d = u.norm();

                        ei.restlength=(double) d;

                        ei.is_activated=true;

                    }
                    else
                    {

                        ei.is_activated=false;

                    }

                    ei.is_initialized = true;
                }
            }

        }

    }

}

template< class DataTypes>
void TriangularBendingSprings<DataTypes>::TriangularBSTriangleDestructionFunction (const sofa::helper::vector<unsigned int> &triangleRemoved,
        void* param, vector<EdgeInformation> &edgeData)
{
    TriangularBendingSprings<DataTypes> *ff= (TriangularBendingSprings<DataTypes> *)param;
    if (ff)
    {
        TriangleSetTopology<DataTypes> *_mesh=ff->getTriangularTopology();
        assert(_mesh!=0);
        TriangleSetTopologyContainer *container=_mesh->getTriangleSetTopologyContainer();
        //const sofa::helper::vector< Edge > &edgeArray=container->getEdgeArray() ;
        const sofa::helper::vector< Triangle > &triangleArray=container->getTriangleArray() ;
        const sofa::helper::vector< TriangleEdges > &triangleEdgeArray=container->getTriangleEdgeArray() ;

        double m_ks=ff->getKs(); // typename DataTypes::
        double m_kd=ff->getKd(); // typename DataTypes::

        //unsigned int u,v;

        const typename DataTypes::VecCoord *restPosition=_mesh->getDOF()->getX0();

        for (unsigned int i=0; i<triangleRemoved.size(); ++i)
        {

            TriangleSetTopology<DataTypes> *_mesh=ff->getTriangularTopology();
            assert(_mesh!=0);
            TriangleSetTopologyContainer *container=_mesh->getTriangleSetTopologyContainer();

            sofa::component::topology::TriangleSetTopologyContainer *tstc= dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer *>(container);

            /// describe the jth edge index of triangle no i
            TriangleEdges te = triangleEdgeArray[triangleRemoved[i]];
            /// describe the jth vertex index of triangle no i
            Triangle t = triangleArray[triangleRemoved[i]];


            for(unsigned int j=0; j<3; ++j)
            {

                EdgeInformation &ei = edgeData[te[j]]; // ff->edgeInfo
                if(ei.is_initialized)
                {

                    unsigned int edgeIndex = te[j];

                    container->getTriangleEdgeShellArray();
                    container->getTriangleVertexShellArray();

                    const sofa::helper::vector< unsigned int > shell = tstc->getTriangleEdgeShell(edgeIndex);
                    if (shell.size()==3)
                    {

                        TriangleEdges te1;
                        Triangle t1;
                        TriangleEdges te2;
                        Triangle t2;

                        if(shell[0] == triangleRemoved[i])
                        {
                            te1 = triangleEdgeArray[shell[1]];
                            t1 = triangleArray[shell[1]];
                            te2 = triangleEdgeArray[shell[2]];
                            t2 = triangleArray[shell[2]];

                        }
                        else
                        {

                            if(shell[1] == triangleRemoved[i])
                            {

                                te1 = triangleEdgeArray[shell[2]];
                                t1 = triangleArray[shell[2]];
                                te2 = triangleEdgeArray[shell[0]];
                                t2 = triangleArray[shell[0]];

                            }
                            else   // shell[2] == triangleRemoved[i]
                            {

                                te1 = triangleEdgeArray[shell[0]];
                                t1 = triangleArray[shell[0]];
                                te2 = triangleEdgeArray[shell[1]];
                                t2 = triangleArray[shell[1]];

                            }
                        }

                        int i1 = tstc->getEdgeIndexInTriangle(te1, edgeIndex);
                        int i2 = tstc->getEdgeIndexInTriangle(te2, edgeIndex);

                        ei.m1 = t1[i1];
                        ei.m2 = t2[i2];

                        //TriangularBendingSprings<DataTypes> *fftest= (TriangularBendingSprings<DataTypes> *)param;
                        ei.ks=m_ks; //(fftest->ks).getValue();
                        ei.kd=m_kd; //(fftest->kd).getValue();

                        Coord u = (*restPosition)[ei.m1] - (*restPosition)[ei.m2];
                        Real d = u.norm();

                        ei.restlength=(double) d;

                        ei.is_activated=true;

                    }
                    else
                    {

                        ei.is_activated=false;
                        ei.is_initialized = false;

                    }

                }
                else
                {

                    ei.is_activated=false;
                    ei.is_initialized = false;

                }
            }

        }

    }
}


template<class DataTypes>
TriangularBendingSprings<DataTypes>::TriangularBendingSprings(/*component::MechanicalObject<DataTypes>* m_dof */ /*double _ks, double _kd*/)
    : _mesh(NULL)
    , updateMatrix(true)
    , f_ks(initData(&f_ks,(double) 100000.0,"stiffness","uniform stiffness for the all springs")) //(Real)0.3 ??
    , f_kd(initData(&f_kd,(double) 1.0,"damping","uniform damping for the all springs")) // (Real)1000. ??
{
    //std::cerr<<"TriangularBendingSprings<DataTypes>::TriangularBendingSprings"<<std::endl;
}

template<class DataTypes>
TriangularBendingSprings<DataTypes>::~TriangularBendingSprings()
{}


template <class DataTypes> void TriangularBendingSprings<DataTypes>::handleTopologyChange()
{
    sofa::core::componentmodel::topology::BaseTopology *topology = static_cast<sofa::core::componentmodel::topology::BaseTopology *>(getContext()->getMainTopology());

    std::list<const TopologyChange *>::const_iterator itBegin=topology->firstChange();
    std::list<const TopologyChange *>::const_iterator itEnd=topology->lastChange();

    edgeInfo.handleTopologyEvents(itBegin,itEnd);
    //triangleInfo.handleTopologyEvents(itBegin,itEnd);

    while( itBegin != itEnd )
    {
        core::componentmodel::topology::TopologyChangeType changeType = (*itBegin)->getChangeType();

        sofa::core::componentmodel::topology::BaseTopology* bt = dynamic_cast<sofa::core::componentmodel::topology::BaseTopology *>(getContext()->getMainTopology());
        sofa::core::componentmodel::topology::TopologyContainer *container=bt->getTopologyContainer();

        sofa::component::topology::TriangleSetTopologyContainer *tstc= dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer *>(container);
        const sofa::helper::vector< Triangle > &triangleArray=tstc->getTriangleArray() ;
        const sofa::helper::vector< TriangleEdges > &triangleEdgeArray=tstc->getTriangleEdgeArray() ;


        if(tstc && changeType == core::componentmodel::topology::POINTSREMOVED)
        {


            const sofa::helper::vector< sofa::helper::vector<unsigned int> > &tvsa=tstc->getTriangleVertexShellArray();
            unsigned int last = tvsa.size() -1;
            unsigned int i,j;

            const sofa::helper::vector<unsigned int> tab = ( dynamic_cast< const sofa::component::topology::PointsRemoved * >( *itBegin ) )->getArray();

            sofa::helper::vector<unsigned int> lastIndexVec; //= tab;
            for(unsigned int i_init = 0; i_init < tab.size(); ++i_init)
            {

                lastIndexVec.push_back(last - i_init);
            }

            for ( i = 0; i < tab.size(); ++i)
            {

                unsigned int i_next = i;
                bool is_reached = false;
                while( (!is_reached) && (i_next < lastIndexVec.size() - 1))
                {

                    i_next += 1 ;
                    is_reached = is_reached || (lastIndexVec[i_next] == tab[i]);
                }

                if(is_reached)
                {

                    lastIndexVec[i_next] = lastIndexVec[i];

                }


                const sofa::helper::vector<unsigned int> &shell= tvsa[lastIndexVec[i]]; // tvsa[last]; //
                for (j=0; j<shell.size(); ++j)
                {

                    Triangle tj = triangleArray[shell[j]];

                    //int vertexIndex = tstc->getVertexIndexInTriangle(tj, lastIndexVec[i]);

                    TriangleEdges tej = triangleEdgeArray[shell[j]];

                    //unsigned int ind_j = tej[vertexIndex];

                    for (unsigned int j_edge=0; j_edge<3; ++j_edge)
                    {

                        unsigned int ind_j = tej[j_edge];

                        if (edgeInfo[ind_j].m1 == (int) last)
                        {
                            edgeInfo[ind_j].m1=(int) tab[i];
                            //std::cout << "INFO_print : OK m1 for ind_j =" << ind_j << std::endl;
                        }
                        else
                        {
                            if (edgeInfo[ind_j].m2 == (int) last)
                            {
                                edgeInfo[ind_j].m2=(int) tab[i];
                                //std::cout << "INFO_print : OK m2 for ind_j =" << ind_j << std::endl;
                            }
                        }

                    }
                }


                for (unsigned int j_loc=0; j_loc<edgeInfo.size(); ++j_loc)
                {

                    bool is_forgotten = false;
                    if (edgeInfo[j_loc].m1 == (int) last)
                    {
                        edgeInfo[j_loc].m1 =(int) tab[i];
                        is_forgotten=true;
                        //std::cout << "INFO_print : MISS m1 for j_loc =" << j_loc << std::endl;

                    }
                    else
                    {
                        if (edgeInfo[j_loc].m2 ==(int) last)
                        {
                            edgeInfo[j_loc].m2 =(int) tab[i];
                            is_forgotten=true;
                            //std::cout << "INFO_print : MISS m2 for j_loc =" << j_loc << std::endl;

                        }

                    }

                    /*
                    if(is_forgotten){

                    	unsigned int ind_forgotten = j_loc;

                    	bool is_in_shell = false;
                    	for (unsigned int j_glob=0;j_glob<shell.size();++j_glob) {

                    		TriangleEdges tetest = triangleEdgeArray[shell[j_glob]];

                    		for (unsigned int j_test=0;j_test<3;++j_test) {
                    			is_in_shell = is_in_shell || (tetest[j_test] == ind_forgotten);
                    		}
                    	}

                    	if(!is_in_shell) {
                    		std::cout << "INFO_print : Vis - edge is forgotten in SHELL !!! global indices (point, edge) = ( "  << last << " , " << ind_forgotten  << " )" << std::endl;

                    		if(ind_forgotten<tstc->getEdgeArray().size()){
                    			const sofa::component::topology::Edge t_forgotten = tstc->getEdge(ind_forgotten);
                    			std::cout << "INFO_print : Vis - last = " << last << std::endl;
                    			std::cout << "INFO_print : Vis - tab.size() = " << tab.size() << " , tab[i] = " << tab[i] << std::endl;

                    		}

                    	}

                    }
                    */

                }

                --last;
            }

        }
        ++itBegin;
    }
}


template<class DataTypes>
void TriangularBendingSprings<DataTypes>::init()
{

    std::cerr << "initializing TriangularBendingSprings" << std::endl;
    this->Inherited::init();

    _mesh =0;
    if (getContext()->getMainTopology()!=0)
        _mesh= dynamic_cast<TriangleSetTopology<DataTypes>*>(getContext()->getMainTopology());

    if ((_mesh==0) || (_mesh->getTriangleSetTopologyContainer()->getNumberOfTriangles()==0))
    {
        std::cerr << "ERROR(TriangularBendingSprings): object must have a Triangular Set Topology.\n";
        return;
    }

    TriangleSetTopologyContainer *container=_mesh->getTriangleSetTopologyContainer();

    /// prepare to store info in the edge array
    edgeInfo.resize(container->getNumberOfEdges());

    unsigned int i;
    // set edge tensor to 0
    const sofa::helper::vector<Edge> &edgeArray=container->getEdgeArray();
    for (i=0; i<container->getNumberOfEdges(); ++i)
    {

        TriangularBSEdgeCreationFunction(i, (void*) this, edgeInfo[i],
                edgeArray[i],  (const sofa::helper::vector< unsigned int > )0,
                (const sofa::helper::vector< double >)0);
    }

    // create edge tensor by calling the triangle creation function
    sofa::helper::vector<unsigned int> triangleAdded;
    for (i=0; i<container->getNumberOfTriangles(); ++i)
    {
        triangleAdded.push_back(i);
    }
    TriangularBSTriangleCreationFunction(triangleAdded,(void*) this,
            edgeInfo);

    edgeInfo.setCreateFunction(TriangularBSEdgeCreationFunction);
    edgeInfo.setCreateTriangleFunction(TriangularBSTriangleCreationFunction);
    edgeInfo.setDestroyTriangleFunction(TriangularBSTriangleDestructionFunction);
    edgeInfo.setCreateParameter( (void *) this );
    edgeInfo.setDestroyParameter( (void *) this );

}

template <class DataTypes>
double TriangularBendingSprings<DataTypes>::getPotentialEnergy(const VecCoord& /*x*/)
{
    std::cerr<<"TriangularBendingSprings::getPotentialEnergy-not-implemented !!!"<<endl;
    return 0;
}


template<class DataTypes>
void TriangularBendingSprings<DataTypes>::addForce(VecDeriv& f, const VecCoord& x, const VecDeriv& v)
{

    TriangleSetTopologyContainer *container=_mesh->getTriangleSetTopologyContainer();
    unsigned int nbEdges=container->getNumberOfEdges();
    //const sofa::helper::vector<Edge> &edgeArray=container->getEdgeArray();

    EdgeInformation *einfo;

    //const helper::vector<Spring>& m_springs= this->springs.getValue();
    //this->dfdx.resize(nbEdges); //m_springs.size()
    f.resize(x.size());
    m_potentialEnergy = 0;
    /*        cerr<<"TriangularBendingSprings<DataTypes>::addForce()"<<endl;*/

    for(unsigned int i=0; i<nbEdges; i++ )
    {
        einfo=&edgeInfo[i];

        /*            cerr<<"TriangularBendingSprings<DataTypes>::addForce() between "<<springs[i].m1<<" and "<<springs[i].m2<<endl;*/

        if(einfo->is_activated)
        {
            //this->addSpringForce(m_potentialEnergy,f,x,v, i, einfo->spring);

            int a = einfo->m1;
            int b = einfo->m2;
            Coord u = x[b]-x[a];
            Real d = u.norm();
            if( d>1.0e-4 )
            {
                Real inverseLength = 1.0f/d;
                u *= inverseLength;
                Real elongation = (Real)(d - einfo->restlength);
                m_potentialEnergy += elongation * elongation * einfo->ks / 2;
                /*      cerr<<"TriangularBendingSprings<DataTypes>::addSpringForce, p = "<<p<<endl;

                		cerr<<"TriangularBendingSprings<DataTypes>::addSpringForce, new potential energy = "<<potentialEnergy<<endl;*/
                Deriv relativeVelocity = v[b]-v[a];
                Real elongationVelocity = dot(u,relativeVelocity);
                Real forceIntensity = (Real)(einfo->ks*elongation+einfo->kd*elongationVelocity);
                Deriv force = u*forceIntensity;
                f[a]+=force;
                f[b]-=force;

                updateMatrix=true;

                Mat3& m = einfo->DfDx; //Mat& m = this->dfdx[i];
                Real tgt = forceIntensity * inverseLength;
                for( int j=0; j<3; ++j )
                {
                    for( int k=0; k<3; ++k )
                    {
                        m[j][k] = ((Real)einfo->ks-tgt) * u[j] * u[k];
                    }
                    m[j][j] += tgt;
                }
            }
            else // null length, no force and no stiffness
            {
                Mat3& m = einfo->DfDx; //Mat& m = this->dfdx[i];
                for( int j=0; j<3; ++j )
                {
                    for( int k=0; k<3; ++k )
                    {
                        m[j][k] = 0;
                    }
                }
            }
        }
    }


    //for (unsigned int i=0; i<springs.size(); i++)
    //{
    /*            cerr<<"TriangularBendingSprings<DataTypes>::addForce() between "<<springs[i].m1<<" and "<<springs[i].m2<<endl;*/
    //    this->addSpringForce(m_potentialEnergy,f,x,v, i, springs[i]);
    //}
}

template<class DataTypes>
void TriangularBendingSprings<DataTypes>::addDForce(VecDeriv& df, const VecDeriv& dx)
{

    TriangleSetTopologyContainer *container=_mesh->getTriangleSetTopologyContainer();
    unsigned int nbEdges=container->getNumberOfEdges();
    //const sofa::helper::vector<Edge> &edgeArray=container->getEdgeArray();

    EdgeInformation *einfo;

    df.resize(dx.size());
    //cerr<<"TriangularBendingSprings<DataTypes>::addDForce, dx1 = "<<dx1<<endl;
    //cerr<<"TriangularBendingSprings<DataTypes>::addDForce, df1 before = "<<f1<<endl;
    //const helper::vector<Spring>& springs = this->springs.getValue();

    for(unsigned int i=0; i<nbEdges; i++ )
    {
        einfo=&edgeInfo[i];

        /*            cerr<<"TriangularBendingSprings<DataTypes>::addForce() between "<<springs[i].m1<<" and "<<springs[i].m2<<endl;*/

        if(einfo->is_activated)
        {
            //this->addSpringDForce(df,dx, i, einfo->spring);

            const int a = einfo->m1;
            const int b = einfo->m2;
            const Coord d = dx[b]-dx[a];
            const Deriv dforce = einfo->DfDx*d; //this->dfdx[i]*d;
            df[a]+=dforce;
            df[b]-=dforce;
            //cerr<<"TriangularBendingSprings<DataTypes>::addSpringDForce, a="<<a<<", b="<<b<<", dforce ="<<dforce<<endl;

            if(updateMatrix)
            {

            }
            updateMatrix=false;
        }
    }

    //for (unsigned int i=0; i<springs.size(); i++)
    //{
    //    this->addSpringDForce(df,dx, i, springs[i]);
    //}
    //cerr<<"TriangularBendingSprings<DataTypes>::addDForce, df = "<<f<<endl;
}


/*
template<class DataTypes>
void TriangularBendingSprings<DataTypes>::updateLameCoefficients()
{
	lambda= f_youngModulus.getValue()*f_poissonRatio.getValue()/(1-f_poissonRatio.getValue()*f_poissonRatio.getValue());
	mu = f_youngModulus.getValue()*(1-f_poissonRatio.getValue())/(1-f_poissonRatio.getValue()*f_poissonRatio.getValue());
//	std::cerr << "initialized Lame coef : lambda=" <<lambda<< " mu="<<mu<<std::endl;
}
*/


template<class DataTypes>
void TriangularBendingSprings<DataTypes>::draw()
{
    unsigned int i;
    if (!getContext()->getShowForceFields()) return;
    if (!this->mstate) return;

    if (getContext()->getShowWireFrame())
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    VecCoord& x = *this->mstate->getX();
    //VecCoord& x_rest = *this->mstate->getX0();
    //TriangleSetTopologyContainer *container=_mesh->getTriangleSetTopologyContainer();
    //unsigned int nbTriangles=container->getNumberOfTriangles();
    //const sofa::helper::vector< Triangle> &triangleArray=container->getTriangleArray() ;


    glDisable(GL_LIGHTING);


    /*
    glBegin(GL_TRIANGLES);
    for(i=0;i<nbTriangles; ++i)
    {
    	int a = triangleArray[i][0];
    	int b = triangleArray[i][1];
    	int c = triangleArray[i][2];

    	glColor4f(0,1,0,1);
    	helper::gl::glVertexT(x[a]);
    	glColor4f(0,0.5,0.5,1);
    	helper::gl::glVertexT(x[b]);
    	glColor4f(0,0,1,1);
    	helper::gl::glVertexT(x[c]);
    }
    */
    unsigned int nb_to_draw = 0;

    glBegin(GL_LINES);
    for(i=0; i<edgeInfo.size(); ++i)
    {
        if(edgeInfo[i].is_activated)
        {


            bool external=true;
            Real d = (x[edgeInfo[i].m2]-x[edgeInfo[i].m1]).norm();
            if (external)
            {
                if (d<edgeInfo[i].restlength*0.9999)
                    glColor4f(0,0,1,1); // (1,0,0,1);
                else
                    glColor4f(0,1,0,1);
            }
            else
            {
                if (d<edgeInfo[i].restlength*0.9999)
                    glColor4f(1,0.5f,0,1);
                else
                    glColor4f(0,1,0.5f,1);
            }


            nb_to_draw+=1;

            //glColor4f(0,1,0,1);
            helper::gl::glVertexT(x[edgeInfo[i].m1]);
            helper::gl::glVertexT(x[edgeInfo[i].m2]);

        }
    }
    glEnd();


    if (getContext()->getShowWireFrame())
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


} // namespace forcefield

} // namespace component

} // namespace sofa

#endif
