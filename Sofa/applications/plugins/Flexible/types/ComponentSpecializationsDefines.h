// preprocessor functions to concatenate 2 variables
#define PASTER(x,y) x ## y
#define EVALUATOR(x,y)  PASTER(x,y)


// define all needed types based on TYPEABSTRACTNAME
//#define TYPEABSTRACTNAME3Types EVALUATOR(TYPEABSTRACTNAME,3Types)
#define TYPEABSTRACTNAME3dTypes EVALUATOR(TYPEABSTRACTNAME,3dTypes)
#define TYPEABSTRACTNAME3fTypes EVALUATOR(TYPEABSTRACTNAME,3fTypes)
#define StdTYPEABSTRACTNAMETypes EVALUATOR(Std,EVALUATOR(TYPEABSTRACTNAME,Types))
#define TYPEABSTRACTNAMEMass EVALUATOR(TYPEABSTRACTNAME,Mass)
//#define TYPEABSTRACTNAME3Mass EVALUATOR(TYPEABSTRACTNAME,3Mass)
#define TYPEABSTRACTNAME3dMass EVALUATOR(TYPEABSTRACTNAME,3dMass)
#define TYPEABSTRACTNAME3fMass EVALUATOR(TYPEABSTRACTNAME,3fMass)
//#define FLEXIBLE_TYPEABSTRACTNAMEComponents_CPP EVALUATOR(FLEXIBLE_,EVALUATOR(TYPEABSTRACTNAME,_CPP))


// preprocessor functions to stringify variables
//#define XSTRINGINFICATION(s) STRINGINFICATION(s)
//#define STRINGINFICATION(s) #s

// include filenames
//#define HEADERINCLUDE XSTRINGINFICATION( EVALUATOR(TYPEABSTRACTNAME,Components.h) )
//#define HEADERTYPESINCLUDE XSTRINGINFICATION( TYPEABSTRACTNAME ## Types.h)
