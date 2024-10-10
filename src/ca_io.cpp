


/* This is the main io module
for describin a CA and the algorithmic tasks, 
we use xerces-C++

*/

#include <iostream>

// on Debian, using libxerces-dev 
// the include files are located centrally 

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
// #include <xercesc/util/XercesString.hpp>

int ca_main(  int  argc,  char ** argv)
{
    
    bool bFailed = false;
    XercesDOMParser* parser = new XercesDOMParser();
    ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    parser->setErrorHandler(errHandler);
 

    if (argc < 2) 
      {
	cerr << "Usage: ca_calc <xmlfile>\n ";
      }
    else
      {
 try {
   XMLPlatformUtils::Initialize();
     }
  catch (const XMLException& toCatch) {
  cerr << " Could not setup XML-parser environment" ;
  exit(-1); 
     }

   try {
      XercesString src(argv[1]);
      LocalFileInputSource source(src);
      parser->parse(source);
      bFailed = parser->getErrorCount() != 0;
      //  parser->parse(argv[1]);
      
      DOMNode *pDoc = parser->getDocument();
   }  
   catch (const XMLException& e)
        {
   cerr << " Could not setup XML-parser environment" ;
	}
      }
}

