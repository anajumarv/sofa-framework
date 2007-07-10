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
#ifndef SOFA_COMPONENT_TOPOLOGY_TETRAHEDRONSETTOPOLOGY_H
#define SOFA_COMPONENT_TOPOLOGY_TETRAHEDRONSETTOPOLOGY_H

#include <sofa/component/topology/TriangleSetTopology.h>
#include <vector>
#include <map>

namespace sofa
{

namespace component
{

namespace topology
{

/// defining Tetrahedra as 3 DOFs indices
typedef fixed_array<unsigned int,4> Tetrahedron;
/// defining TetrahedronTriangles as 4 Triangles indices
typedef fixed_array<unsigned int,4> TetrahedronTriangles;
/// defining TetrahedronEdges as 6 Edge indices
typedef fixed_array<unsigned int,6> TetrahedronEdges;



/////////////////////////////////////////////////////////
/// TopologyChange subclasses
/////////////////////////////////////////////////////////



/** indicates that some tetrahedra were added */
class TetrahedraAdded : public core::componentmodel::topology::TopologyChange
{

public:
    unsigned int nTetrahedra;

    std::vector< Tetrahedron > tetrahedronArray;

    std::vector< unsigned int > tetrahedronIndexArray;

    std::vector< std::vector< unsigned int > > ancestorsList;

    std::vector< std::vector< double > > coefs;

    TetrahedraAdded(const unsigned int nT,
            const std::vector< Tetrahedron >& _tetrahedronArray = (const std::vector< Tetrahedron >)0,
            const std::vector< unsigned int >& tetrahedraIndex = (const std::vector< unsigned int >)0,
            const std::vector< std::vector< unsigned int > >& ancestors = (const std::vector< std::vector< unsigned int > >)0,
            const std::vector< std::vector< double > >& baryCoefs = (const std::vector< std::vector< double > >)0)
        : core::componentmodel::topology::TopologyChange(core::componentmodel::topology::TETRAHEDRAADDED), nTetrahedra(nT), tetrahedronArray(_tetrahedronArray), tetrahedronIndexArray(tetrahedraIndex),ancestorsList(ancestors), coefs(baryCoefs)
    {   }

    unsigned int getNbAddedTetrahedra() const
    {
        return nTetrahedra;
    }

};



/** indicates that some tetrahedra are about to be removed */
class TetrahedraRemoved : public core::componentmodel::topology::TopologyChange
{

public:
    std::vector<unsigned int> removedTetrahedraArray;

public:
    TetrahedraRemoved(const std::vector<unsigned int> _tArray) : core::componentmodel::topology::TopologyChange(core::componentmodel::topology::TETRAHEDRAREMOVED), removedTetrahedraArray(_tArray)
    {
    }

    const std::vector<unsigned int> &getArray() const
    {
        return removedTetrahedraArray;
    }
    unsigned int getNbRemovedTetrahedra() const
    {
        return removedTetrahedraArray.size();
    }

};



/////////////////////////////////////////////////////////
/// TetrahedronSetTopology objects
/////////////////////////////////////////////////////////


/** a class that stores a set of tetrahedra and provides access with adjacent triangles, edges and vertices */
class TetrahedronSetTopologyContainer : public TriangleSetTopologyContainer
{
private:
    /** \brief Creates the array of edge indices for each tetrahedron
    *
    * This function is only called if the TetrahedronEdge array is required.
    * m_tetrahedronEdge[i] contains the 6 indices of the 6 edges of each tetrahedron
    The number of each edge is the following : edge 0 links vertex 0 and 1, edge 1 links vertex 0 and 2,
    edge 2 links vertex 0 and 3, edge 3 links vertex 1 and 2, edge 4 links vertex 1 and 3,
    edge 5 links vertex 2 and 3
    */
    void createTetrahedronEdgeArray();
    /** \brief Creates the array of triangle indices for each tetrahedron
    *
    * This function is only called if the TetrahedronTriangle array is required.
    * m_tetrahedronTriangle[i] contains the 4 indices of the 4 triangles opposite to the ith vertex
    */
    void createTetrahedronTriangleArray();
    /** \brief Creates the Tetrahedron Vertex Shell Array
    *
    * This function is only called if the TetrahedronVertexShell array is required.
    * m_tetrahedronVertexShell[i] contains the indices of all tetrahedra adjacent to the ith vertex
    */
    void createTetrahedronVertexShellArray();

    /** \brief Creates the Tetrahedron Edge Shell Array
    *
    * This function is only called if the TetrahedronEdheShell array is required.
    * m_tetrahedronEdgeShell[i] contains the indices of all tetrahedra adjacent to the ith edge
    */
    void createTetrahedronEdgeShellArray();
    /** \brief Creates the Tetrahedron Triangle Shell Array
    *
    * This function is only called if the TetrahedronTriangleShell array is required.
    * m_tetrahedronTriangleShell[i] contains the indices of all tetrahedra adjacent to the ith edge
    */
    void createTetrahedronTriangleShellArray();
protected:
    /// provides the set of tetrahedra
    std::vector<Tetrahedron> m_tetrahedron;
    /// provides the set of edges for each tetrahedron
    std::vector<TetrahedronEdges> m_tetrahedronEdge;
    /// provides the set of triangles for each tetrahedron
    std::vector<TetrahedronTriangles> m_tetrahedronTriangle;

    /// for each vertex provides the set of tetrahedra adjacent to that vertex
    std::vector< std::vector< unsigned int > > m_tetrahedronVertexShell;
    /// for each edge provides the set of tetrahedra adjacent to that edge
    std::vector< std::vector< unsigned int > > m_tetrahedronEdgeShell;
    /// for each triangle provides the set of tetrahedra adjacent to that edge
    std::vector< std::vector< unsigned int > > m_tetrahedronTriangleShell;


    /** \brief Creates the EdgeSet array.
     *
     * Create the set of edges when needed.
    */
    virtual void createEdgeSetArray() {createTetrahedronEdgeArray();}

    /** \brief Creates the TriangleSet array.
     *
     * Create the array of triangles
    */
    virtual void createTriangleSetArray() {createTetrahedronTriangleArray();}

    /** \brief Creates the TetrahedronSet array.
     *
     * This function is only called by derived classes to create a list of edges from a set of tetrahedra or tetrahedra
    */
    virtual void createTetrahedronSetArray() {}

public:
    /** \brief Returns the Tetrahedron array.
     *
     */
    const std::vector<Tetrahedron> &getTetrahedronArray();

    /** \brief Returns the ith Tetrahedron.
     *
     */
    const Tetrahedron &getTetrahedron(const unsigned int i);

    /** \brief Returns the number of tetrahedra in this topology.
     *
     */
    unsigned int getNumberOfTetrahedra() ;

    /** \brief Returns the Tetrahedron Vertex Shells array.
     *
     */
    const std::vector< std::vector<unsigned int> > &getTetrahedronVertexShellArray() ;

    /** \brief Returns the set of tetrahedra adjacent to a given vertex.
     *
     */
    const std::vector< unsigned int > &getTetrahedronVertexShell(const unsigned int i) ;

    /** \brief Returns the index of the edge joining vertex v1 and vertex v2; returns -1 if no edge exists
     *
     */

    /** \brief Returns the Tetrahedron Edges  array.
     *
     */
    const std::vector< TetrahedronEdges > &getTetrahedronEdgeArray() ;

    /** \brief Returns the 6 edges adjacent to a given tetrahedron.
     *
     */
    const TetrahedronEdges &getTetrahedronEdges(const unsigned int i) ;

    /** \brief Returns the Tetrahedron Triangles  array.
     *
     */
    const std::vector< TetrahedronTriangles > &getTetrahedronTriangleArray() ;

    /** \brief Returns the 4 triangles adjacent to a given tetrahedron.
     *
     */
    const TetrahedronTriangles &getTetrahedronTriangles(const unsigned int i) ;


    /** \brief Returns the Tetrahedron Edge Shells array.
     *
     */
    const std::vector< std::vector<unsigned int> > &getTetrahedronEdgeShellArray() ;

    /** \brief Returns the set of tetrahedra adjacent to a given edge.
     *
     */
    const std::vector< unsigned int > &getTetrahedronEdgeShell(const unsigned int i) ;

    /** \brief Returns the Tetrahedron Triangle Shells array.
         *
         */
    const std::vector< std::vector<unsigned int> > &getTetrahedronTriangleShellArray() ;

    /** \brief Returns the set of tetrahedra adjacent to a given triangle.
     *
     */
    const std::vector< unsigned int > &getTetrahedronTriangleShell(const unsigned int i) ;


    /** Returns the indices of a tetrahedron given four vertex indices : returns -1 if none */
    int getTetrahedronIndex(const unsigned int v1, const unsigned int v2, const unsigned int v3, const unsigned int v4);


    /** returns the index (either 0, 1 ,2 or 3) of the vertex whose global index is vertexIndex. Returns -1 if none */
    int getVertexIndexInTetrahedron(Tetrahedron &t,unsigned int vertexIndex) const;


    TetrahedronSetTopologyContainer(core::componentmodel::topology::BaseTopology *top,
            const std::vector< unsigned int > &DOFIndex = (const std::vector< unsigned int >)0,
            const std::vector< Tetrahedron >         &tetrahedra    = (const std::vector< Tetrahedron >)        0 );

    template< typename DataTypes >
    friend class TetrahedronSetTopologyModifier;
protected:
    /** \brief Returns a non-const tetrahedron vertex shell given a vertex index for subsequent modification
     *
     */
    std::vector< unsigned int > &getTetrahedronVertexShellForModification(const unsigned int vertexIndex);
    /** \brief Returns a non-const tetrahedron edge shell given the index of an edge for subsequent modification
      *
      */
    std::vector< unsigned int > &getTetrahedronEdgeShellForModification(const unsigned int edgeIndex);


};


template <class DataTypes>
class TetrahedronSetTopologyLoader;

/**
 * A class that modifies the topology by adding and removing tetrahedra
 */
template<class DataTypes>
class TetrahedronSetTopologyModifier : public TriangleSetTopologyModifier <DataTypes>
{

public:
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;

    TetrahedronSetTopologyModifier(core::componentmodel::topology::BaseTopology *top) : TriangleSetTopologyModifier<DataTypes>(top)
    {
    }
    /** \brief Build  a tetrahedron set topology from a file : also modifies the MechanicalObject
     *
     */
    virtual bool load(const char *filename);


    /** \brief Sends a message to warn that some tetrahedra were added in this topology.
     *
     * \sa addTetrahedraProcess
     */
    void addTetrahedraWarning(const unsigned int nTetrahedra,
            const std::vector< Tetrahedron >& tetrahedraList,
            const std::vector< unsigned int >& tetrahedraIndexList,
            const std::vector< std::vector< unsigned int > > & ancestors= (const std::vector< std::vector<unsigned int > >) 0 ,
            const std::vector< std::vector< double > >& baryCoefs= (const std::vector< std::vector< double > >)0) ;



    /** \brief Actually Add some tetrahedra to this topology.
     *
     * \sa addTetrahedraWarning
     */
    virtual void addTetrahedraProcess(const std::vector< Tetrahedron > &tetrahedra);

    /** \brief Sends a message to warn that some tetrahedra are about to be deleted.
     *
     * \sa removeTetrahedraProcess
     *
     * Important : parameter indices is not const because it is actually sorted from the highest index to the lowest one.
     */
    void removeTetrahedraWarning( std::vector<unsigned int> &tetrahedra);

    /** \brief Remove a subset of tetrahedra
     *
     * Elements corresponding to these points are removed form the mechanical object's state vectors.
     *
     * Important : some structures might need to be warned BEFORE the points are actually deleted, so always use method removeEdgesWarning before calling removeEdgesProcess.
     * \sa removeTetrahedraWarning
     * @param removeIsolatedItems if true remove isolated triangles, edges and vertices
     */
    virtual void removeTetrahedraProcess( const std::vector<unsigned int> &indices,const bool removeIsolatedItems=false);

    /** \brief Actually Add some triangles to this topology.
       *
       * \sa addTrianglesWarning
       */
    virtual void addTrianglesProcess(const std::vector< Triangle > &triangles);


    /** \brief Remove a subset of triangles
     *
     * Important : some structures might need to be warned BEFORE the points are actually deleted, so always use method removeEdgesWarning before calling removeEdgesProcess.
     * @param removeIsolatedItems if true remove isolated edges and vertices
     */
    virtual void removeTrianglesProcess(const std::vector<unsigned int> &indices,const bool removeIsolatedItems=false);

    /** \brief Add some edges to this topology.
    *
    * \sa addEdgesWarning
    */
    virtual void addEdgesProcess(const std::vector< Edge > &edges);


    /** \brief Remove a subset of edges
    *
    * Important : some structures might need to be warned BEFORE the points are actually deleted, so always use method removeEdgesWarning before calling removeEdgesProcess.
    * \sa removeEdgesWarning
    *
    * Important : parameter indices is not const because it is actually sorted from the highest index to the lowest one.
    * @param removeIsolatedItems if true remove isolated vertices
    */
    virtual void removeEdgesProcess( const std::vector<unsigned int> &indices,const bool removeIsolatedItems=false);



    /** \brief Add some points to this topology.
     *
     * Use a list of ancestors to create the new points.
     * Last parameter baryCoefs defines the coefficient used for the creation of the new points.
     * Default value for these coefficient (when none is defined) is 1/n with n being the number of ancestors
     * for the point being created.
     *
     * \sa addPointsWarning
     */
    virtual void addPointsProcess(const unsigned int nPoints,
            const std::vector< std::vector< unsigned int > >& ancestors = (const std::vector< std::vector< unsigned int > >)0,
            const std::vector< std::vector< double > >& baryCoefs = (const std::vector< std::vector< double > >)0 );



    /** \brief Remove a subset of points
     *
     * Elements corresponding to these points are removed form the mechanical object's state vectors.
     *
     * Important : some structures might need to be warned BEFORE the points are actually deleted, so always use method removePointsWarning before calling removePointsProcess.
     * \sa removePointsWarning
     */
    virtual void removePointsProcess( std::vector<unsigned int> &indices);



    /** \brief Reorder this topology.
     *
     * \see MechanicalObject::renumberValues
     */
    virtual void renumberPointsProcess( const std::vector<unsigned int> &index );


protected:
    void addTetrahedron(Tetrahedron e);

public:
    //template <class DataTypes>
    friend class TetrahedronSetTopologyLoader<DataTypes>;

};



/**
 * A class that performs topology algorithms on an TetrahedronSet.
 */
template < class DataTypes >
class TetrahedronSetTopologyAlgorithms : public PointSetTopologyAlgorithms<DataTypes>
{

public:

    TetrahedronSetTopologyAlgorithms(sofa::core::componentmodel::topology::BaseTopology *top) : PointSetTopologyAlgorithms<DataTypes>(top)
    {
    }
};

/**
 * A class that provides geometry information on an TetrahedronSet.
 */
template < class DataTypes >
class TetrahedronSetGeometryAlgorithms : public EdgeSetGeometryAlgorithms<DataTypes>
{
public:
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::Real Real;
    typedef typename DataTypes::Coord Coord;


    TetrahedronSetGeometryAlgorithms(sofa::core::componentmodel::topology::BaseTopology *top) : EdgeSetGeometryAlgorithms<DataTypes>(top)
    {
    }

    /// computes the volume of tetrahedron no i and returns it
    Real computeTetrahedronVolume(const unsigned int i) const;
    /// computes the tetrahedron volume of all tetrahedra are store in the array interface
    void computeTetrahedronVolume( BasicArrayInterface<Real> &ai) const;
    /// computes the tetrahedron volume  of tetrahedron no i and returns it
    Real computeRestTetrahedronVolume(const unsigned int i) const;
};


/** Describes a topological object that only consists as a set of points :
it is a base class for all topological objects */
template<class DataTypes>
class TetrahedronSetTopology : public PointSetTopology <DataTypes>
{

public:
    TetrahedronSetTopology(component::MechanicalObject<DataTypes> *obj);


    virtual void init();
    /** \brief Returns the TetrahedronSetTopologyContainer object of this TetrahedronSetTopology.
     */
    TetrahedronSetTopologyContainer *getTetrahedronSetTopologyContainer() const
    {
        return (TetrahedronSetTopologyContainer *)this->m_topologyContainer;
    }
    /** \brief Returns the TetrahedronSetTopologyAlgorithms object of this TetrahedronSetTopology.
     */
    TetrahedronSetTopologyAlgorithms<DataTypes> *getTetrahedronSetTopologyAlgorithms() const
    {
        return (TetrahedronSetTopologyAlgorithms<DataTypes> *)this->m_topologyAlgorithms;
    }
    /** \brief Returns the TetrahedronSetTopologyAlgorithms object of this TetrahedronSetTopology.
     */
    TetrahedronSetGeometryAlgorithms<DataTypes> *getTetrahedronSetGeometryAlgorithms() const
    {
        return (TetrahedronSetGeometryAlgorithms<DataTypes> *)this->m_geometryAlgorithms;
    }

};

} // namespace topology

} // namespace component

} // namespace sofa

#endif
