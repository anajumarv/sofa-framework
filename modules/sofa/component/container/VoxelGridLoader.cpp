/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
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

#include <sofa/component/container/VoxelGridLoader.h>

#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/io/Image.h>
#include <sofa/helper/io/ImageRAW.h>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

namespace sofa
{

  namespace component
  {

  namespace container
  {

    using namespace sofa::defaulttype;

    SOFA_DECL_CLASS ( VoxelGridLoader );

    int VoxelGridLoaderClass = core::RegisterObject ( "Voxel Grid Loader" )
                               .add< VoxelGridLoader >()
                               ;

    VoxelGridLoader::VoxelGridLoader()
        : MeshLoader(),
        voxelSize ( initData ( &voxelSize, Vector3 ( 1.0f,1.0f,1.0f ), "voxelSize", "Dimension of one voxel" ) ),
        dataResolution ( initData ( &dataResolution, Vec3i ( 0,0,0 ), "resolution", "Resolution of the voxel file" ) ),
		roi ( initData ( &roi, Vec6i ( 0,0,0, 0xFFFF, 0xFFFF, 0xFFFF ), "ROI", "Region of interest (xmin, ymin, zmin, xmax, ymax, zmax)" ) ),
        headerSize ( initData ( &headerSize, 0, "header", "Header size in bytes" ) ),
	    segmentationFile( initData(&segmentationFile,"segmentationFile","Segmentation file")),
        segmentationHeaderSize ( initData ( &segmentationHeaderSize, 0, "segmentationHeader", "Header size in bytes" ) ),
		backgroundValue ( initData ( &backgroundValue, "bgValue", "Background values (to be ignored)" ) ),
        activeValue ( initData ( &activeValue, "dataValue", "Active data values" ) ),
	generateHexa( initData ( &generateHexa, true, "generateHexa", "Interpret voxel as either hexa or points")),
	image(NULL),
	segmentation(NULL),
	bpp(8) // bits per pixel
    {	}

	VoxelGridLoader::~VoxelGridLoader()
	{
		clear();
	}

    void VoxelGridLoader::init()
	{
		clear();

		const Vec3i &res = dataResolution.getValue();
		Vec6i&	ROI = (* roi.beginEdit());
		if(ROI[0] < 0)	ROI[0] = 0;
		if(ROI[1] < 0)	ROI[1] = 0;
		if(ROI[2] < 0)	ROI[2] = 0;
		if(ROI[3] >= res[0]) ROI[3] = res[0] - 1;
		if(ROI[4] >= res[1]) ROI[4] = res[1] - 1;
		if(ROI[5] >= res[2]) ROI[5] = res[2] - 1;
		if(ROI[0] > ROI[3]) ROI[3] = ROI[0];
		if(ROI[1] > ROI[4]) ROI[4] = ROI[1];
		if(ROI[2] > ROI[5]) ROI[5] = ROI[2];
		roi.endEdit();

		if(!filename.getValue().empty())
                        image = loadImage(filename.getFullPath(), res, headerSize.getValue());

		if ( image == NULL )
			return;
#ifndef NDEBUG
		else
			std::cout << "[VoxelGridLoader::init] file " << filename.getValue() << " loaded." << std::endl;
#endif

		if(!segmentationFile.getValue().empty())
                        segmentation = loadImage(segmentationFile.getFullPath(), res, segmentationHeaderSize.getValue());

		reinit();
	}

	void VoxelGridLoader::reinit()
	{
      seqPoints.clear();
	  seqHexahedra.clear();
	  _idxInRegularGrid.clear();

	  const Vec6i&	ROI = roi.getValue();

	  if(generateHexa.getValue())
	  {
		  const unsigned int numVoxelsX = dataResolution.getValue()[0];
		  const unsigned int numVoxelsY = dataResolution.getValue()[1];
	//	  const unsigned int numVoxelsZ = dataResolution.getValue()[2];

	//    const unsigned int numVoxels = numVoxelsX * numVoxelsY * numVoxelsZ;

		  const unsigned int numPointsX = numVoxelsX + 1;
		  const unsigned int numPointsY = numVoxelsY + 1;
	//	  const unsigned int numPointsZ = numVoxelsZ + 1;

	//    const unsigned int numPoints = numPointsX * numPointsY * numPointsZ;

		  std::set<unsigned int> keepPoint;

		  for ( unsigned int k=(unsigned)ROI[2]; k<=(unsigned)ROI[5]; ++k )
			for ( unsigned int j=(unsigned)ROI[1]; j<=(unsigned)ROI[4]; ++j )
			  for ( unsigned int i=(unsigned)ROI[0]; i<=(unsigned)ROI[3]; ++i )
			  {
				unsigned int idx = i + j * numVoxelsX + k * numVoxelsX * numVoxelsY;

				if ( isActive(idx) )
				{
				  for ( unsigned int q=0; q<8; ++q )
				  {
					const unsigned int pidx = ( i + ( q % 2 ) ) + ( j + ( ( q & 2 ) >> 1 ) ) * numPointsX + ( k + ( q / 4 ) ) * numPointsX * numPointsY;
					keepPoint.insert ( pidx );
				  }
				}
			  }

		  std::cout << "[VoxelGridLoader::init] inserting " << keepPoint.size() << " points ... ";

		  unsigned int pointIdx = 0;
		  seqPoints.resize ( keepPoint.size() );
		  std::map<unsigned int, unsigned int>  renumberingMap;
		  for ( unsigned int k=(unsigned)ROI[2]; k<=(unsigned)ROI[5]+1; ++k )
			for ( unsigned int j=(unsigned)ROI[1]; j<=(unsigned)ROI[4]+1; ++j )
			  for ( unsigned int i=(unsigned)ROI[0]; i<=(unsigned)ROI[3]+1; ++i )
			  {
				// add only points that were used above
				const unsigned int pidx = i + j * numPointsX + k * numPointsX * numPointsY;
				if ( keepPoint.find ( pidx ) != keepPoint.end() )
				{
				  renumberingMap[pidx] = pointIdx;
				  Point& pnt = seqPoints[pointIdx];
				  pnt[0] = i*voxelSize.getValue()[0];
				  pnt[1] = j*voxelSize.getValue()[1];
				  pnt[2] = k*voxelSize.getValue()[2];
				  pointIdx++;
				}
			  }
		  keepPoint.clear();

		  std::cout << " done. " << std::endl;

		  std::cout << "[VoxelGridLoader::init] inserting hexa ... ";

		  for ( unsigned int k=(unsigned)ROI[2]; k<=(unsigned)ROI[5]; ++k )
			for ( unsigned int j=(unsigned)ROI[1]; j<=(unsigned)ROI[4]; ++j )
			  for ( unsigned int i=(unsigned)ROI[0]; i<=(unsigned)ROI[3]; ++i )
			  {
				unsigned int idx = i + j * numVoxelsX + k * numVoxelsX * numVoxelsY;

				if ( isActive(idx) )
				{
				  unsigned int p[8];
				  for ( unsigned int q=0; q<8; ++q )
				  {
					p[q] = renumberingMap[ ( i + ( q % 2 ) ) + ( j + ( ( q & 2 ) >> 1 ) ) * numPointsX + ( k + ( q / 4 ) ) * numPointsX * numPointsY];
				  }
	#ifdef SOFA_NEW_HEXA
				  seqHexahedra.push_back ( Hexahedron ( p[0], p[1], p[3], p[2], p[4], p[5], p[7], p[6] ) );
	#else                                  
				  seqHexahedra.push_back ( Hexahedron ( p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7] ) );
	#endif
				  _idxInRegularGrid.push_back ( idx );
				}
			  }

		  std::cout << "done. (" << seqHexahedra.size() << ")" << std::endl;
	  }
	  else
	  {
		  const unsigned int numVoxelsX = dataResolution.getValue()[0];
		  const unsigned int numVoxelsY = dataResolution.getValue()[1];
	//	  const unsigned int numVoxelsZ = dataResolution.getValue()[2];

		  std::cout << "[VoxelGridLoader::init] inserting points ... ";

		  for ( unsigned int k=(unsigned)ROI[2]; k<=(unsigned)ROI[5]; ++k )
			for ( unsigned int j=(unsigned)ROI[1]; j<=(unsigned)ROI[4]; ++j )
			  for ( unsigned int i=(unsigned)ROI[0]; i<=(unsigned)ROI[3]; ++i )
			  {
				unsigned int idx = i + j * numVoxelsX + k * numVoxelsX * numVoxelsY;

				if ( isActive(idx) )
				{
				  Point pnt;
				  pnt[0] = i*voxelSize.getValue()[0];
				  pnt[1] = j*voxelSize.getValue()[1];
				  pnt[2] = k*voxelSize.getValue()[2];

				  seqPoints.push_back(pnt);
				  _idxInRegularGrid.push_back ( idx );
				}
			  }

		  std::cout << "done. (" << seqPoints.size() << ")" << std::endl;
	  }
    }

    void VoxelGridLoader::clear()
    {
      MeshLoader::clear();
	  _idxInRegularGrid.clear();

      if(image != NULL)
		  delete image;
	  image = NULL;

	  if(segmentation != NULL)
		  delete segmentation;
	  segmentation = NULL;
    }

	void  VoxelGridLoader::getIndicesInRegularGrid(helper::vector<unsigned int>& idxInRegularGrid) const
	{
      idxInRegularGrid.assign ( _idxInRegularGrid.begin(), _idxInRegularGrid.end() );
	}

    bool VoxelGridLoader::load ( const char* filename )
    {
      clear();

      std::string _filename ( filename );

	  image = loadImage(_filename, dataResolution.getValue(), headerSize.getValue());

	  if(image != NULL)
	  {
	      setFilename ( _filename );
		  return true;
	  }
	  else
	      return false;
    }

    helper::io::Image* VoxelGridLoader::loadImage ( const std::string& filename, const Vec3i& res, const int hsize ) const
	{
	  helper::io::Image* image = NULL;

      std::string _filename ( filename );

	  if(res.norm2() > 0 && bpp > 0)
	  {
		  if ( _filename.length() > 4 && _filename.compare ( _filename.length()-4, 4, ".raw" ) ==0 )
		  {
			helper::io::ImageRAW *imageRAW = new helper::io::ImageRAW();
			imageRAW->init(res[0], res[1], res[2], bpp, hsize);
			if(imageRAW->load( _filename ))
				image = imageRAW;
		  }
	  }

	  if(image == NULL)
      {
		  std::cout << "Unable to load file " <<  _filename << std::endl;
      }

	  return image;
	}


	int VoxelGridLoader::getDataSize() const
	{
		return dataResolution.getValue()[0]*dataResolution.getValue()[1]*dataResolution.getValue()[2];
	}

    void VoxelGridLoader::setResolution ( const Vec3i res )
    {
      ( *dataResolution.beginEdit() ) = res;
      dataResolution.endEdit();
    }

    void VoxelGridLoader::getResolution ( Vec3i& res ) const
    {
      res = dataResolution.getValue();
    }

    void VoxelGridLoader::setVoxelSize ( const defaulttype::Vector3 vSize )
    {
      ( *voxelSize.beginEdit() ) = vSize;
      voxelSize.endEdit();
    }

    void VoxelGridLoader::getVoxelSize ( defaulttype::Vector3& vSize ) const
    {
      vSize = voxelSize.getValue();
    }

    void VoxelGridLoader::addBackgroundValue ( const int value )
	{
		helper::vector<int>& vecVal = ( *backgroundValue.beginEdit() );
		vecVal.push_back(value);
		std::sort(vecVal.begin(), vecVal.end());
		std::unique(vecVal.begin(), vecVal.end());
		backgroundValue.endEdit();
		reinit();
	}

    int VoxelGridLoader::getBackgroundValue(const unsigned int idx) const
    {
		const helper::vector<int>& vecVal = backgroundValue.getValue();
		if(idx < vecVal.size())
			return vecVal[idx];
		else
			return -1;
    }

	void VoxelGridLoader::addActiveDataValue(const int value)
	{
		helper::vector<int>& vecVal = ( *activeValue.beginEdit() );
		vecVal.push_back(value);
		std::sort(vecVal.begin(), vecVal.end());
		std::unique(vecVal.begin(), vecVal.end());
		activeValue.endEdit();
		reinit();
	}

	int VoxelGridLoader::getActiveDataValue(const unsigned int idx) const
	{
		const helper::vector<int>& vecVal = activeValue.getValue();
		if(idx < vecVal.size())
			return vecVal[idx];
		else
			return -1;
	}

	unsigned char * VoxelGridLoader::getData()
	{
		return image->getData();
	}

	const unsigned char * VoxelGridLoader::getData() const
	{
		return image->getData();
	}

  unsigned char * VoxelGridLoader::getSegmentID()
  {
    if( segmentation)
      return segmentation->getData();
    else
      return NULL;
  }

  const unsigned char * VoxelGridLoader::getSegmentID() const
  {
    if( segmentation)
      return segmentation->getData();
    else
      return NULL;
  }

  VoxelGridLoader::Vec6i VoxelGridLoader::getROI() const
  {
    return roi.getValue();
  }

	bool VoxelGridLoader::isActive(const unsigned int idx) const
	{
		const helper::vector<int>& activeVal = activeValue.getValue();
		const helper::vector<int>& bgVal = backgroundValue.getValue();

		if(activeVal.empty() && bgVal.empty())
			return true;

		helper::io::Image* img = (segmentation == NULL) ? image : segmentation;
		const unsigned char value = img->getData()[idx];

		if(!activeVal.empty()) // active values were specified
		{
			if(std::binary_search(activeVal.begin(), activeVal.end(), value))
				return true;
			else
				return false;
		}

		if(!bgVal.empty()) // background values were specified
		{
			if(std::binary_search(bgVal.begin(), bgVal.end(), value))
				return false;
			else
				return true;
		}

		return true;
	}

	// fill the texture by 'image' only where there is the 'segmentation' of 'activeValue' and give the 3D texture sizes
	void VoxelGridLoader::createSegmentation3DTexture( unsigned char **textureData, int& width, int& height, int& depth)
	{
		const Vec3i &resol = dataResolution.getValue();

		// with, height and depth are the nearest power of 2 greater or equal to the resolution
		for(width=1; resol[0]>width; width <<= 1) ;
		for(height=1; resol[1]>height; height <<= 1) ;
		for(depth=1; resol[2]>depth; depth <<= 1) ;

		int textureSize = width*height*depth;

		*textureData = new unsigned char [textureSize];
		for(unsigned char *ptr = (*textureData) + textureSize; ptr != *textureData; )
			*(--ptr) = (unsigned char) 0;

		const unsigned char *data = getData();

		// for all "active" data voxels
		for(unsigned i=0; i<_idxInRegularGrid.size(); ++i)
		{
			const int idxData = _idxInRegularGrid[i];
			const int I = idxData % resol[0];
			const int J = (idxData/resol[0]) % resol[1];
			const int K = idxData / (resol[0] * resol[1]);
			const int idxTexture = I + (J + K * height) * width;
			(*textureData)[idxTexture] = data[idxData];
		}
	}

  }

  } // namespace component

} // namespace sofa

