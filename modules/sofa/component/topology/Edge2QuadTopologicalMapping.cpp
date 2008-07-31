/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 3      *
*                (c) 2006-2008 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <sofa/component/topology/Edge2QuadTopologicalMapping.h>

#include <sofa/core/ObjectFactory.h>

#include <sofa/core/componentmodel/topology/BaseMeshTopology.h>

#include <sofa/component/topology/QuadSetTopology.h>
#include <sofa/component/topology/EdgeSetTopology.h>

#include <sofa/defaulttype/Vec.h>
#include <map>
#include <sofa/defaulttype/VecTypes.h>

#include <sofa/core/componentmodel/behavior/MechanicalState.h>

#include <math.h>
#include <sofa/defaulttype/Vec.h>

#include <sofa/defaulttype/RigidTypes.h>


namespace sofa
{

namespace component
{

namespace topology
{

using namespace sofa::defaulttype;

using namespace sofa::component::topology;
using namespace sofa::core::componentmodel::topology;

/// Input Topology
typedef BaseMeshTopology In;
/// Output Topology
typedef BaseMeshTopology Out;

SOFA_DECL_CLASS(Edge2QuadTopologicalMapping)

// Register in the Factory
int Edge2QuadTopologicalMappingClass = core::RegisterObject("Special case of mapping where EdgeSetTopology is converted to QuadSetTopology")
        .add< Edge2QuadTopologicalMapping >()

        ;

// Implementation


In* Edge2QuadTopologicalMapping::getFromModel()
{
    return this->fromModel;
}


Out* Edge2QuadTopologicalMapping::getToModel()
{
    return this->toModel;
}


objectmodel::BaseObject* Edge2QuadTopologicalMapping::getFrom()
{
    return this->fromModel;
}


objectmodel::BaseObject* Edge2QuadTopologicalMapping::getTo()
{
    return this->toModel;
}



void Edge2QuadTopologicalMapping::setModels(In* from, Out* to)
{
    this->fromModel = from;
    this->toModel = to;
}



void Edge2QuadTopologicalMapping::init()
{

    unsigned int N = m_nbPointsOnEachCircle.getValue();
    double rho = m_radius.getValue();

    //std::cout << "INFO_print : init Edge2QuadTopologicalMapping" << std::endl;

    // INITIALISATION of QUADULAR mesh from EDGE mesh :

    component::MechanicalObject<Rigid3Types>* from_mstate = dynamic_cast<component::MechanicalObject<Rigid3Types>*>(fromModel->getContext()->getMechanicalState());
    component::MechanicalObject<Vec3Types>* to_mstate = dynamic_cast<component::MechanicalObject<Vec3Types>*>(toModel->getContext()->getMechanicalState());

    if (fromModel)
    {

        std::cout << "INFO_print : Edge2QuadTopologicalMapping - from = edge" << std::endl;

        if (toModel)
        {

            std::cout << "INFO_print : Edge2QuadTopologicalMapping - to = quad" << std::endl;

            QuadSetTopologyModifier *to_tstm;
            toModel->getContext()->get(to_tstm);

            const sofa::helper::vector<Edge> &edgeArray=fromModel->getEdges();

            Loc2GlobVec.clear();
            In2OutMap.clear();

            // CREATION of the points (new DOFs for the output topology) along the circles around each point of the input topology

            Vec Y0;
            Vec Z0;
            Y0[0] = (Real) (0.0); Y0[1] = (Real) (1.0); Y0[2] = (Real) (0.0);
            Z0[0] = (Real) (0.0); Z0[1] = (Real) (0.0); Z0[2] = (Real) (1.0);

            to_mstate->resize(fromModel->getNbPoints() * N);

            for (unsigned int i=0; i<(unsigned int) fromModel->getNbPoints(); ++i)
            {
                unsigned int p0=i;

                Mat rotation;
                (*from_mstate->getX())[p0].writeRotationMatrix(rotation);

                Vec t;
                t=(*from_mstate->getX())[p0].getCenter();

                Vec Y;
                Vec Z;

                Y = rotation * Y0;
                Z = rotation * Z0;

                for(unsigned int j=0; j<N; ++j)
                {

                    Vec x = t + (Y*cos((Real) (2.0*j*M_PI/N)) + Z*sin((Real) (2.0*j*M_PI/N)))*((Real) rho);

                    (*to_mstate->getX())[p0*N+j]=x;
                }
            }

            // CREATION of the quads based on the the circles

            for (unsigned int i=0; i<edgeArray.size(); ++i)
            {

                unsigned int p0 = edgeArray[i][0];
                unsigned int p1 = edgeArray[i][1];

                sofa::helper::vector<unsigned int> out_info;

                for(unsigned int j=0; j<N; ++j)
                {

                    unsigned int q0 = p0*N+j;
                    unsigned int q1 = p1*N+j;
                    unsigned int q2 = p1*N+((j+1)%N);
                    unsigned int q3 = p0*N+((j+1)%N);

                    to_tstm->addQuad(Quad(helper::make_array<unsigned int>((unsigned int) q0, (unsigned int) q1, (unsigned int) q2, (unsigned int) q3)));
                    Loc2GlobVec.push_back(i);
                    out_info.push_back(Loc2GlobVec.size()-1);
                }

                In2OutMap[i]=out_info;
            }

            QuadSetTopologyAlgorithms<Vec3Types> *to_alg;
            toModel->getContext()->get(to_alg);
            to_alg->notifyEndingEvent();
        }

    }
}


unsigned int Edge2QuadTopologicalMapping::getFromIndex(unsigned int ind)
{
    return ind; // identity
}

void Edge2QuadTopologicalMapping::updateTopologicalMapping()
{

    unsigned int N = m_nbPointsOnEachCircle.getValue();

    // INITIALISATION of QUADULAR mesh from EDGE mesh :

    if (fromModel)
    {

        QuadSetTopologyAlgorithms<Vec3Types> *to_alg;
        toModel->getContext()->get(to_alg);

        QuadSetTopologyModifier *to_tstm;
        toModel->getContext()->get(to_tstm);

        if (toModel)
        {

            std::list<const TopologyChange *>::const_iterator itBegin=fromModel->firstChange();
            std::list<const TopologyChange *>::const_iterator itEnd=fromModel->lastChange();

            while( itBegin != itEnd )
            {
                TopologyChangeType changeType = (*itBegin)->getChangeType();

                switch( changeType )
                {

                case core::componentmodel::topology::ENDING_EVENT:
                {
                    //std::cout << "INFO_print : TopologicalMapping - ENDING_EVENT" << std::endl;
                    to_alg->notifyEndingEvent();
                    break;
                }

                case core::componentmodel::topology::EDGESADDED:
                {
                    //std::cout << "INFO_print : TopologicalMapping - EDGESADDED" << std::endl;
                    if (fromModel)
                    {

                        const sofa::helper::vector<Edge> &edgeArray=fromModel->getEdges();

                        const sofa::helper::vector<unsigned int> &tab = ( static_cast< const EdgesAdded *>( *itBegin ) )->edgeIndexArray;

                        sofa::helper::vector< Quad > quads_to_create;
                        sofa::helper::vector< unsigned int > quadsIndexList;
                        int nb_elems = toModel->getNbQuads();

                        for (unsigned int i = 0; i < tab.size(); ++i)
                        {
                            unsigned int k = tab[i];

                            unsigned int p0 = edgeArray[k][0];
                            unsigned int p1 = edgeArray[k][1];

                            sofa::helper::vector<unsigned int> out_info;

                            for(unsigned int j=0; j<N; ++j)
                            {

                                unsigned int q0 = p0*N+j;
                                unsigned int q1 = p1*N+j;
                                unsigned int q2 = p1*N+((j+1)%N);
                                unsigned int q3 = p0*N+((j+1)%N);

                                Quad t = Quad(helper::make_array<unsigned int>((unsigned int) q0, (unsigned int) q1, (unsigned int) q2, (unsigned int) q3));

                                //quads_to_create.clear();
                                //quadsIndexList.clear();

                                quads_to_create.push_back(t);
                                quadsIndexList.push_back(nb_elems);
                                nb_elems+=1;

                                Loc2GlobVec.push_back(k);
                                out_info.push_back(Loc2GlobVec.size()-1);

                                //to_tstm->addQuadsProcess(quads_to_create) ;
                                //to_tstm->addQuadsWarning(quads_to_create.size(), quads_to_create, quadsIndexList) ;
                                //toModel->propagateTopologicalChanges();
                            }

                            In2OutMap[k]=out_info;
                        }

                        to_tstm->addQuadsProcess(quads_to_create) ;
                        to_tstm->addQuadsWarning(quads_to_create.size(), quads_to_create, quadsIndexList) ;
                        toModel->propagateTopologicalChanges();
                    }
                    break;
                }
                case core::componentmodel::topology::EDGESREMOVED:
                {
                    //std::cout << "INFO_print : TopologicalMapping - EDGESREMOVED" << std::endl;

                    if (fromModel)
                    {

                        const sofa::helper::vector<unsigned int> &tab = ( static_cast< const EdgesRemoved *>( *itBegin ) )->getArray();

                        int last= fromModel->getNbEdges() - 1;

                        unsigned int ind_tmp;

                        sofa::helper::vector<unsigned int> ind_real_last;
                        int ind_last=toModel->getNbQuads();

                        for (unsigned int i = 0; i < tab.size(); ++i)
                        {
                            unsigned int k = tab[i];
                            sofa::helper::vector<unsigned int> ind_k;

                            std::map<unsigned int, sofa::helper::vector<unsigned int> >::iterator iter_1 = In2OutMap.find(k);
                            if(iter_1 != In2OutMap.end())
                            {

                                sofa::helper::vector<unsigned int> ind_list;
                                for(unsigned int j=0; j<N; ++j)
                                {
                                    ind_list.push_back(In2OutMap[k][j]);
                                }

                                ind_last = ind_last - 1;

                                ind_k = In2OutMap[k];
                                ind_real_last = ind_k;

                                std::map<unsigned int, sofa::helper::vector<unsigned int> >::iterator iter_2 = In2OutMap.find(last);
                                if(iter_2 != In2OutMap.end())
                                {

                                    ind_real_last = In2OutMap[last];

                                    if(k != last)
                                    {

                                        In2OutMap.erase(In2OutMap.find(k));
                                        In2OutMap[k] = ind_real_last;

                                        In2OutMap.erase(In2OutMap.find(last));
                                        In2OutMap[last] = ind_k;

                                        for(unsigned int j=0; j<N; ++j)
                                        {

                                            ind_tmp = Loc2GlobVec[ind_real_last[j]];
                                            Loc2GlobVec[ind_real_last[j]] = Loc2GlobVec[ind_k[j]];
                                            Loc2GlobVec[ind_k[j]] = ind_tmp;
                                        }
                                    }
                                }
                                else
                                {
                                    std::cout << "INFO_print : Edge2QuadTopologicalMapping - In2OutMap should have the edge " << last << std::endl;
                                }

                                if( ind_k[N-1] != ind_last)
                                {

                                    In2OutMap.erase(In2OutMap.find(Loc2GlobVec[ind_last]));
                                    In2OutMap[Loc2GlobVec[ind_last]] = ind_k;

                                    sofa::helper::vector<unsigned int> out_info;
                                    for(unsigned int j=0; j<N; ++j)
                                    {
                                        out_info.push_back(ind_last-j);
                                    }

                                    In2OutMap.erase(In2OutMap.find(Loc2GlobVec[ind_k[N-1]]));
                                    In2OutMap[Loc2GlobVec[ind_k[N-1]]] = out_info;

                                    ind_tmp = Loc2GlobVec[ind_k[N-1]];
                                    Loc2GlobVec[ind_k[N-1]] = Loc2GlobVec[ind_last];
                                    Loc2GlobVec[ind_last] = ind_tmp;

                                }

                                for(unsigned int j=1; j<N; ++j)
                                {

                                    ind_last = ind_last-1;

                                    if( ind_k[N-1-j] != ind_last)
                                    {

                                        ind_tmp = Loc2GlobVec[ind_k[N-1-j]];
                                        Loc2GlobVec[ind_k[N-1-j]] = Loc2GlobVec[ind_last];
                                        Loc2GlobVec[ind_last] = ind_tmp;
                                    }
                                }

                                In2OutMap.erase(In2OutMap.find(Loc2GlobVec[Loc2GlobVec.size() - 1]));

                                Loc2GlobVec.resize( Loc2GlobVec.size() - N );

                                sofa::helper::vector< unsigned int > quads_to_remove;
                                for(unsigned int j=0; j<N; ++j)
                                {
                                    quads_to_remove.push_back(ind_list[j]);
                                }

                                to_alg->removeQuads(quads_to_remove, true, true);

                            }
                            else
                            {
                                std::cout << "INFO_print : Edge2QuadTopologicalMapping - In2OutMap should have the edge " << k << std::endl;
                            }

                            --last;
                        }
                    }

                    break;
                }

                case core::componentmodel::topology::POINTSRENUMBERING:
                {
                    //std::cout << "INFO_print : Edge2QuadTopologicalMapping - POINTSRENUMBERING" << std::endl;

                    const sofa::helper::vector<unsigned int> &tab = ( static_cast< const PointsRenumbering * >( *itBegin ) )->getIndexArray();
                    const sofa::helper::vector<unsigned int> &inv_tab = ( static_cast< const PointsRenumbering * >( *itBegin ) )->getinv_IndexArray();

                    sofa::helper::vector<unsigned int> indices;
                    sofa::helper::vector<unsigned int> inv_indices;

                    for(unsigned int i = 0; i < tab.size(); ++i)
                    {

                        for(unsigned int j=0; j<N; ++j)
                        {
                            indices.push_back(tab[i]*N + j);
                            inv_indices.push_back(inv_tab[i]*N + j);
                        }

                        //std::cout << "INFO_print : Edge2QuadTopologicalMapping - renumber point = " << tab[i] << std::endl;
                    }

                    sofa::helper::vector<unsigned int>& tab_indices = indices;
                    sofa::helper::vector<unsigned int>& inv_tab_indices = inv_indices;

                    to_tstm->renumberPointsWarning(tab_indices, inv_tab_indices, true);
                    toModel->propagateTopologicalChanges();
                    to_tstm->renumberPointsProcess(tab_indices, inv_tab_indices, true);

                    break;
                }

                case core::componentmodel::topology::POINTSADDED:
                {
                    //std::cout << "INFO_print : Edge2QuadTopologicalMapping - POINTSADDED" << std::endl;

                    const sofa::component::topology::PointsAdded *ta=static_cast< const sofa::component::topology::PointsAdded * >( *itBegin );

                    unsigned int to_nVertices = ta->getNbAddedVertices() * N;
                    sofa::helper::vector< sofa::helper::vector< unsigned int > > to_ancestorsList;
                    sofa::helper::vector< sofa::helper::vector< double > > to_coefs;

                    for(unsigned int i =0; i < ta->getNbAddedVertices(); i++)
                    {

                        sofa::helper::vector< unsigned int > my_ancestors;
                        sofa::helper::vector< double > my_coefs;

                        for(unsigned int j =0; j < N; j++)
                        {

                            for(unsigned int k = 0; k < ta->ancestorsList[i].size(); k++)
                            {
                                my_ancestors.push_back(ta->ancestorsList[i][k]*N + j);
                            }
                            for(unsigned int k = 0; k < ta->coefs[i].size(); k++)
                            {
                                my_coefs.push_back(ta->coefs[i][k]*N + j);
                            }

                            to_ancestorsList.push_back(my_ancestors);
                            to_coefs.push_back(my_coefs);
                        }
                    }

                    to_tstm->addPointsProcess(to_nVertices, to_ancestorsList, to_coefs, true);
                    to_tstm->addPointsWarning(to_nVertices, to_ancestorsList, to_coefs, true);
                    toModel->propagateTopologicalChanges();

                    break;
                }

                default:
                    // Ignore events that are not Quad  related.
                    break;
                };

                ++itBegin;
            }
            toModel->propagateTopologicalChanges();
        }
    }

    return;
}

} // namespace topology

} // namespace component

} // namespace sofa

