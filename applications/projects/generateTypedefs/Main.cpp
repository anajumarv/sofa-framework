/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc., 51  *
* Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.                   *
*******************************************************************************
*                            SOFA :: Applications                             *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include <iostream>


#include <sofa/core/SofaLibrary.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/core/objectmodel/Base.h>


#include <sofa/helper/system/SetDirectory.h>
#include <sofa/helper/system/FileRepository.h>

using sofa::core::SofaLibrary;
using sofa::core::CategoryLibrary;
using sofa::core::ComponentLibrary;


const std::string headerFile("\
/******************************************************************************\n\
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *\n\
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *\n\
*                                                                             *\n\
* This library is free software; you can redistribute it and/or modify it     *\n\
* under the terms of the GNU Lesser General Public License as published by    *\n\
* the Free Software Foundation; either version 2.1 of the License, or (at     *\n\
* your option) any later version.                                             *\n\
*                                                                             *\n\
* This library is distributed in the hope that it will be useful, but WITHOUT *\n\
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *\n\
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *\n\
* for more details.                                                           *\n\
*                                                                             *\n\
* You should have received a copy of the GNU Lesser General Public License    *\n\
* along with this library; if not, write to the Free Software Foundation,     *\n\
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *\n\
*******************************************************************************\n\
*                               SOFA :: Modules                               *\n\
*                                                                             *\n\
* Authors: The SOFA Team and external contributors (see Authors.txt)          *\n\
*                                                                             *\n\
* Contact information: contact@sofa-framework.org                             *\n\
******************************************************************************/\n\
\n\n\n\
// File automatically generated by \"generateTypedef\"\n\n\
"
                            );

enum TYPES {DOUBLE, FLOAT, COMBINATION};

typedef sofa::core::ObjectFactory::Creator Creator;

static std::map< std::string, std::string >               templateExtension;
static std::map< TYPES, std::string >                     fileExtension;


static std::map< std::string, std::string >               includeComponents;
static std::multimap< std::string, std::string >          typedefComponents;
static std::multimap< std::string, std::string > simplificationTypedefComponents;


void printIncludes( const CategoryLibrary &category)
{

    const std::string pathIncludeFiles = sofa::helper::system::SetDirectory::GetParentDir(sofa::helper::system::DataRepository.getFirstPath().c_str()) + "/modules/";


    const CategoryLibrary::VecComponent &components = category.getComponents();
    for (CategoryLibrary::VecComponentIterator itComp=components.begin(); itComp != components.end(); ++itComp)
    {
        std::ostringstream output;

        const ComponentLibrary &component = *(*itComp);

        const std::type_info& defaultTypeInfo=component.getEntry()->creatorList.begin()->second->type();

        std::string namespaceComponent = sofa::core::objectmodel::Base::decodeNamespaceName(component.getEntry()->creatorList.begin()->second->type());

        std::size_t positionDoublePoints = namespaceComponent.find("::");
        while (positionDoublePoints != std::string::npos)
        {
            namespaceComponent.replace(positionDoublePoints, 2, "/");
            positionDoublePoints =  namespaceComponent.find("::");
        }

        std::string filename = namespaceComponent+"/"+sofa::core::objectmodel::Base::decodeClassName(defaultTypeInfo)+".h";

        std::string f(pathIncludeFiles+filename);
        if (sofa::helper::system::DataRepository.findFile( f ) )
        {
            output << "#include <" << namespaceComponent  << "/"
                    << sofa::core::objectmodel::Base::decodeClassName(defaultTypeInfo) << ".h>\n";
            includeComponents.insert(std::make_pair(component.getName(),output.str()));
        }

    }


};


void printFullTypedefs( const CategoryLibrary &category, TYPES t)
{

//   output << "\n\n//Declaration of the typedefs \n";
    std::vector< std::pair< std::string, std::string > > typedefWritten;
    const CategoryLibrary::VecComponent &components = category.getComponents();
    for (CategoryLibrary::VecComponentIterator itComp=components.begin(); itComp != components.end(); ++itComp)
    {
        const ComponentLibrary &component = *(*itComp);
        const std::list< std::pair<std::string, Creator*> > &creatorList=component.getEntry()->creatorList;
        std::list< std::pair<std::string, Creator*> >::const_iterator itCreator;

        //Iterate on all the possible creation of templates
        for (itCreator=creatorList.begin(); itCreator!=creatorList.end(); ++itCreator)
        {
            const std::type_info& typeInfo=itCreator->second->type();

            std::string templateName = itCreator->first;

            if (templateExtension.find(templateName) != templateExtension.end())
            {
                std::string extension=templateExtension[templateName];
                bool isFloat = ( *(extension.rbegin()) == 'f');
                if ( (isFloat && t==FLOAT) || (!isFloat && t==DOUBLE))
                {
                    const std::string finalName = component.getName() +  templateExtension[templateName];

                    simplificationTypedefComponents.insert(std::make_pair(component.getName(),finalName + " " + finalName.substr(0,finalName.size()-1)));
                    typedefComponents.insert(std::make_pair(component.getName(), sofa::core::objectmodel::Base::decodeFullName(typeInfo) + " " + finalName));
                }
            }
            else
            {
                //Template not recognize

                std::string categoryName=category.getName();
                std::string componentName=component.getName();
                std::string templateCombination=templateName;
                bool isMapping=false;
                //Special Case of Mapping!
                if (categoryName == templateName.substr(0, categoryName.size()))
                {
                    //Special case of Mappings
                    //Transform: Mapping<Vec3d,Vec3d> into Vec3d,Vec3d
                    templateCombination = templateName.substr(categoryName.size()+1);
                    templateCombination = templateCombination.substr(0,templateCombination.size()-1);
                    isMapping=true;
                    //Change the name of the typedef to do a difference between a mapping and a mechanical mapping
                    if (categoryName.find("MechanicalMapping") != std::string::npos)
                    {
                        componentName = componentName.substr(0,componentName.size()-7);
                        componentName += "MechanicalMapping";
                    }
                }
                std::size_t separation = templateCombination.find(',');
                if (separation != std::string::npos)
                {
                    std::string t1=templateCombination.substr(0,separation);
                    std::string t2=templateCombination.substr(separation+1);

                    if (templateExtension.find(t1) != templateExtension.end() &&
                        templateExtension.find(t2) != templateExtension.end())
                    {

                        bool isT1Float  = ( *(t1.rbegin()) == 'f'); bool isT2Float  = ( *(t2.rbegin()) == 'f');
                        bool isT1Double = ( *(t1.rbegin()) == 'd'); bool isT2Double = ( *(t2.rbegin()) == 'd');

                        switch(t)
                        {
                        case DOUBLE:
                        {
                            //Special case of Visual Mapping
                            if ( isT1Float ) continue;
                            if ( isT2Float && t2 != "ExtVec3f") continue;
                            break;
                        }
                        case FLOAT:
                        {
                            if (isT1Double || isT2Double) continue;

                            break;
                        }
                        case COMBINATION:
                        {

                            if ( t2 == "ExtVec3f" ||
                                    (isT1Float  &&  isT2Float) ||
                                    (isT1Double && isT2Double)     )continue;
                            break;
                        }
                        }

                        std::string separatorCombination;
                        if (isMapping) separatorCombination="_to_";
                        else           separatorCombination="_";

                        const std::string finalName = componentName +  templateExtension[t1] + separatorCombination + templateExtension[t2];
                        std::string smartFinalName;
                        if (t2 != "ExtVec3d")
                        {
                            smartFinalName=  componentName
                                    + templateExtension[t1].substr(0,templateExtension[t1].size()-1)
                                    + separatorCombination
                                    + templateExtension[t2].substr(0,templateExtension[t2].size()-1);
                        }
                        else
                        {
                            smartFinalName=  componentName
                                    + templateExtension[t1].substr(0,templateExtension[t1].size()-1)
                                    + separatorCombination
                                    + templateExtension[t2];
                        }
                        simplificationTypedefComponents.insert(std::make_pair(component.getName(),finalName + " " + smartFinalName));
                        typedefComponents.insert(std::make_pair(component.getName(), sofa::core::objectmodel::Base::decodeFullName(typeInfo) + " " + finalName));

                    }
                }
            }
        }
    }
}










static std::string outputPath=sofa::helper::system::SetDirectory::GetParentDir(sofa::helper::system::DataRepository.getFirstPath().c_str()) + "/modules/sofa/component/typedef/";

void writeFile(const CategoryLibrary &category,  TYPES t, std::ostream &generalOutput)
{

    if (includeComponents.empty() || typedefComponents.empty()) return;


    const std::string filename=category.getName()+fileExtension[t] + ".h";

    generalOutput << "#include <sofa/component/typedef/" << filename << ">\n";

    std::ofstream typedefFile( (outputPath+filename).c_str() );
    typedefFile << headerFile << "\n";
    typedefFile << "#ifndef SOFA_TYPEDEF_" << category.getName() << fileExtension[t] << "_H" << "\n"
            << "#define SOFA_TYPEDEF_" << category.getName() << fileExtension[t] << "_H" << "\n";


    //---------------------------------------------------------------------------------------------
    //DEFAULT INCLUDES
    typedefFile <<
            "\n\
//Default files containing the declaration of the vector type\n\
#include <sofa/defaulttype/VecTypes.h>\n\
#include <sofa/defaulttype/RigidTypes.h>\n\
#include <sofa/defaulttype/Mat.h>\n\n\
";

            if (category.getName() == "Mapping")
            {
            typedefFile << "\
            //Default files needed to create a Mapping\n\
#include <sofa/core/componentmodel/behavior/State.h>\n\
#include <sofa/core/componentmodel/behavior/MappedModel.h>\n\
#include <sofa/core/Mapping.h>\n\
";
}
        else if (category.getName() == "MechanicalMapping")
{
    typedefFile << "\
//Default files needed to create a Mechanical Mapping\n\
#include <sofa/core/componentmodel/behavior/MechanicalState.h>\n\
#include <sofa/core/componentmodel/behavior/MechanicalMapping.h>\n\
";
}
    typedefFile << "\n\n";

    //---------------------------------------------------------------------------------------------
    //COMPONENTS INCLUDES
    std::map< std::string, std::string >::const_iterator itIncludes;
    for (itIncludes=includeComponents.begin(); itIncludes!=includeComponents.end(); ++itIncludes)
{
    //Only write the include if a typedef will follow
    if (typedefComponents.count(itIncludes->first))
{
    typedefFile << itIncludes->second;
}
}

    typedefFile << "\n\n";

    typedef std::multimap< std::string, std::string >::const_iterator multimapConstIterator;
    //---------------------------------------------------------------------------------------------
    //TYPEDEF DECLARATIONS
    for (itIncludes=includeComponents.begin(); itIncludes!=includeComponents.end(); ++itIncludes)
{
    std::pair<multimapConstIterator,multimapConstIterator > range;
    range = typedefComponents.equal_range(itIncludes->first);

    if (typedefComponents.count(itIncludes->first))
{
    typedefFile << "\n//---------------------------------------------------------------------------------------------\n\
    //Typedef for " << itIncludes->first << "\n";
    for (multimapConstIterator it=range.first; it!=range.second; it++)
    {
        typedefFile << "typedef " << it->second << ";\n";
    }
    typedefFile << "\n\n";
}
}


typedefFile << "\n\n";
//---------------------------------------------------------------------------------------------
//TYPEDEF SIMPLIFICATIONS
if (t != COMBINATION)
{
    switch (t)
    {
    case DOUBLE:
    {
        typedefFile << "\n#ifndef SOFA_FLOAT\n";
        break;
    }
    case FLOAT:
    {
        typedefFile << "\n#ifdef SOFA_FLOAT\n";
        break;
    }
    default:
    {
        break;
    }
    }

    for (itIncludes=includeComponents.begin(); itIncludes!=includeComponents.end(); ++itIncludes)
    {
        std::pair<multimapConstIterator,multimapConstIterator > range;
        range = simplificationTypedefComponents.equal_range(itIncludes->first);

        for (multimapConstIterator it=range.first; it!=range.second; it++)
        {
            typedefFile << "typedef " << it->second << ";\n";
        }
    }

    typedefFile << "#endif\n";
}
//---------------------------------------------------------------------------------------------
//CLOSE THE FILE
typedefFile << "\n#endif\n";
typedefFile.close();
}


















int main(int , char** )
{

    SofaLibrary library; library.build();
    const SofaLibrary::VecCategory &categories = library.getCategories();

    templateExtension.insert(std::make_pair("Vec6d", "6d"));
    templateExtension.insert(std::make_pair("Vec4d", "4d"));
    templateExtension.insert(std::make_pair("Vec3d", "3d"));
    templateExtension.insert(std::make_pair("Vec2d", "1d"));
    templateExtension.insert(std::make_pair("Vec1d", "2d"));
    templateExtension.insert(std::make_pair("Rigid", "Rigid3d"));
    templateExtension.insert(std::make_pair("Rigid2d", "Rigid2d"));

    templateExtension.insert(std::make_pair("Vec6f", "6f"));
    templateExtension.insert(std::make_pair("Vec4f", "4f"));
    templateExtension.insert(std::make_pair("Vec3f", "3f"));
    templateExtension.insert(std::make_pair("Vec2f", "1f"));
    templateExtension.insert(std::make_pair("Vec1f", "2f"));
    templateExtension.insert(std::make_pair("Rigid3f", "Rigid3f"));
    templateExtension.insert(std::make_pair("Rigid2f", "Rigid2f"));
    templateExtension.insert(std::make_pair("ExtVec3f", "Ext3f"));
    templateExtension.insert(std::make_pair("ExtVec3d", "Ext3d"));

    fileExtension.insert    (std::make_pair(DOUBLE, "_double"));
    fileExtension.insert    (std::make_pair(FLOAT, "_float"));
    fileExtension.insert    (std::make_pair(COMBINATION, "_combination"));


    std::string filenameSofaTypedef=outputPath+"Sofa_typedef.h";
    std::ofstream sofaTypedef( filenameSofaTypedef.c_str() ); sofaTypedef << headerFile << "\n";
    sofaTypedef << "\
#ifndef SOFA_TYPEDEF\n\
#define SOFA_TYPEDEF\n\
\n\n\
//Compilation WITHOUT double\n\
#ifndef SOFA_DOUBLE\n\
#include <sofa/component/typedef/Sofa_float.h>\n\
#endif\n\
\n\
//Compilation WITHOUT float\n\
#ifndef SOFA_FLOAT\n\
#include <sofa/component/typedef/Sofa_double.h>\n\
#endif\n\
\n\n\
#ifndef SOFA_DOUBLE\n\
#ifndef SOFA_FLOAT\n\
\n";

    std::string filenameSofaDouble=outputPath+"Sofa_double.h";
    std::ofstream sofaDouble( filenameSofaDouble.c_str() );   sofaDouble  << headerFile << "\n";
    sofaDouble << "\
#ifndef SOFA_TYPEDEF_DOUBLE\n\
#define SOFA_TYPEDEF_DOUBLE\n\
\n";
    sofaDouble << "\n#include <sofa/component/typedef/Particles_double.h>\n";

    std::string filenameSofaFloat=outputPath+"Sofa_float.h";
    std::ofstream sofaFloat( filenameSofaFloat.c_str() );     sofaFloat   << headerFile << "\n";
    sofaFloat << "\
#ifndef SOFA_TYPEDEF_FLOAT\n\
#define SOFA_TYPEDEF_FLOAT\n\
\n";
    sofaFloat << "\n#include <sofa/component/typedef/Particles_float.h>\n";

    for (SofaLibrary::VecCategoryIterator itCat=categories.begin(); itCat!=categories.end(); ++itCat)
    {
    const CategoryLibrary &category = *(*itCat);
    const CategoryLibrary::VecComponent &components = category.getComponents();


    //First read all the components of the categories, and try to know if templates exist
    bool needToCreateTypedefs=false;
    for (CategoryLibrary::VecComponentIterator itComp=components.begin(); itComp != components.end(); ++itComp)
    {
    const ComponentLibrary &component = *(*itComp);
    if (!component.getTemplates().empty()) {needToCreateTypedefs=true; break;}
}

    //If templates have been found, we create an empty file, in which we will write the typedefs associated
    if (needToCreateTypedefs)
    {
    //Find the files needed to be included
    printIncludes( category);

    //Get typedefs for the DOUBLE
    printFullTypedefs( category, DOUBLE);
    writeFile( category, DOUBLE, sofaDouble);
    typedefComponents.clear();
    simplificationTypedefComponents.clear();

    //Get typedefs for the FLOAT
    printFullTypedefs( category, FLOAT);
    writeFile( category, FLOAT, sofaFloat);
    typedefComponents.clear();
    simplificationTypedefComponents.clear();

    //Get typedefs for the COMBINATION
    printFullTypedefs( category, COMBINATION);
    writeFile( category, COMBINATION, sofaTypedef);
    typedefComponents.clear();
    simplificationTypedefComponents.clear();

}
}

    sofaTypedef << "\n#endif\n";
    sofaTypedef << "#endif\n";


    sofaTypedef << "#endif\n";
    sofaDouble  << "\n#endif\n";
    sofaFloat   << "\n#endif\n";
    return 0;
}
