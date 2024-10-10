// header file for configuration handling

#ifndef configurator_h
#define configurator_h


#include <string>
#include <vector>

#include <assert.h>

// #include <stdlib.h>


#if defined(_MSC_VER)
#define DLL extern "C" __declspec(dllexport)
#else
#define DLL
#endif

// got removed from xercesc 4 
#define XERCES_STD_QUALIFIER  std ::


#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
// obsolete with xerces-c 3.0?
// #include <xercesc/dom/DOMBuilder.hpp>
// new for >= 3.0
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMError.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMLocator.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMCharacterData.hpp>
#include <xercesc/util/XMLChar.hpp>




#include <iostream>
#include <fstream>

using namespace std;
using namespace XERCES_CPP_NAMESPACE;

// It seems that this got removed from xerces c version 4 over version 3
// #ifndef XERCES_CPP_NAMESPACE_USE


class XStr
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XStr(const char* const toTranscode)
    {
        // Call the private transcoding method
        fUnicodeForm = XMLString::transcode(toTranscode);
    }

    ~XStr()
    {
        XMLString::release(&fUnicodeForm);
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* unicodeForm() const
    {
        return fUnicodeForm;
    }

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fUnicodeForm
    //      This is the Unicode XMLCh format of the string.
    // -----------------------------------------------------------------------
    XMLCh*   fUnicodeForm;
};

#define X(str) XStr(str).unicodeForm()



class StrX{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    StrX(const XMLCh* const toTranscode)
    {
        // Call the private transcoding method
        fLocalForm = XMLString::transcode(toTranscode);
    }

    ~StrX()
    {
        XMLString::release(&fLocalForm);
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const char* localForm() const
    {
        return fLocalForm;
    }

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fLocalForm
    //      This is the local code page form of the string.
    // -----------------------------------------------------------------------
    char*   fLocalForm;
};

inline  ostream& operator<<( ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}




// need now really a private error handler
class DOMConfigErrorHandler : public DOMErrorHandler {
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    DOMConfigErrorHandler();
    ~DOMConfigErrorHandler();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool getSawErrors() const;


    // -----------------------------------------------------------------------
    //  Implementation of the DOM ErrorHandler interface
    // -----------------------------------------------------------------------
    bool handleError(const DOMError& domError);
    void resetErrors();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DOMConfigErrorHandler(const DOMConfigErrorHandler&);
    void operator=(const DOMConfigErrorHandler&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSawErrors
    //      This is set if we get any errors, and is queryable via a getter
    //      method. Its used by the main code to suppress output if there are
    //      errors.
    // -----------------------------------------------------------------------
    bool    fSawErrors;
};


class Config {

private:
  class ParamNotFoundException {
  public:
    const string msg;
    ParamNotFoundException(string mesg): msg(mesg) {}
  }; 


public:
     // need to do platform initialize in here.
	Config();
  

           
  // the first two parameters are values for name-attributes, the 
  // entity names are intended to be verified through a schema or DTD
  void Get(const string, const string,  const string,  int&) throw (ParamNotFoundException);
  void Get(const string, const string,  const string, string&) throw (ParamNotFoundException);
  void Get(const string, const string,  const string, double&) throw (ParamNotFoundException);
  void Init(const char* cfgFile = 0);

  // E=Entity,A=Attribute-Name, V-Attribute Value which must match,
  // R-Result is Attribute Value (optional, if Format ends with A,
  // T-Text section is result
  string GetDataByFormat(string format,vector<string> keys,bool &error);   

// protected:  
  string GetParam(const string, const string,  const string param_name,  const string);


private:
XercesDOMParser* parser; //  = new XercesDOMParser();
	// DOMBuilder *parser;
  // XercesDOMParser *parser;
  // DefaultHandler h;
  DOMConfigErrorHandler h; 
 
  //  DOMDocument *doc;
  DOMNode *doc;
  bool initialized;
  const XMLCh * XMLtopdomain;
};




#endif
