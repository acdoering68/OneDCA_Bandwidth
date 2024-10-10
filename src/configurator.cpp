// configuration handler: 




#include "configurator.h"



DOMConfigErrorHandler::DOMConfigErrorHandler() :
    fSawErrors(false)
{
}

DOMConfigErrorHandler::~DOMConfigErrorHandler(){};


bool DOMConfigErrorHandler::handleError(const DOMError& domError)
{
    fSawErrors = true;
    if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
        XERCES_STD_QUALIFIER cerr << "\nWarning at file ";
    else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR)
        XERCES_STD_QUALIFIER cerr << "\nError at file ";
    else
        XERCES_STD_QUALIFIER cerr << "\nFatal Error at file ";

    XERCES_STD_QUALIFIER cerr << StrX(domError.getLocation()->getURI())
         << ", line " << domError.getLocation()->getLineNumber()
         << ", char " << domError.getLocation()->getColumnNumber()
         << "\n  Message: " << StrX(domError.getMessage()) << XERCES_STD_QUALIFIER endl;

    return true;
}

void DOMConfigErrorHandler::resetErrors()
{
    fSawErrors = false;
}


Config::Config()
: initialized(false) { 
	   string h("CellAutBW");
 
try
    {
         // use default values for locale etc.
            XMLPlatformUtils::Initialize();

    }
  catch (const XMLException& toCatch)
    {
         XERCES_STD_QUALIFIER cerr << "Error during configurator initialization! :\n"
              << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
         return;
    }
  	XMLtopdomain = XStr(h.c_str()).unicodeForm();

}

void Config::Init(const char * filename)
{
AbstractDOMParser::ValSchemes valScheme = AbstractDOMParser::Val_Auto;
  bool                       errorOccurred = false;

 

  static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(gLS);
     parser = new XercesDOMParser();
	// /* pre 3.0 */ parser = ((DOMImplementationLS*)impl)->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

    DOMConfigErrorHandler errorHandler;
    parser->setErrorHandler((ErrorHandler*) new HandlerBase());
	// /* pre 3.0 */ parser->setErrorHandler(&errorHandler);
ifstream configfile;
doc = 0;
errorHandler.resetErrors();// needed?
try{
	       //       doc = parser->parseURI(filename);
        // instead for 3.0
	parser->parse(filename);
		doc = parser->getDocument();
}
catch (const XMLException& toCatch)
        {
            XERCES_STD_QUALIFIER cerr << "\nError during parsing: '" << filename << "'\n"
                 << "Exception message is:  \n"
                 << StrX(toCatch.getMessage()) << "\n" << XERCES_STD_QUALIFIER endl;
            errorOccurred = true;
                  }
        catch (const DOMException& toCatch)
        {
            const unsigned int maxChars = 2047;
            XMLCh errText[maxChars + 1];

            XERCES_STD_QUALIFIER cerr << "\nDOM Error during parsing: '" << filename << "'\n"
                 << "DOMException code is:  " << toCatch.code << XERCES_STD_QUALIFIER endl;

            if (DOMImplementation::loadDOMExceptionMsg(toCatch.code, errText, maxChars))
                 XERCES_STD_QUALIFIER cerr << "Message is: " << StrX(errText) << XERCES_STD_QUALIFIER endl;
            exit(0);  
            // errorOccurred = true;
            
        }
        catch (...)
        {
            XERCES_STD_QUALIFIER cerr << "\nUnexpected exception during parsing: '" << filename << "'\n";
            errorOccurred = true;
        }


}

// const XMLCh * XMLname = xercesc::XMLString::transcode("name"); 


DOMNode * GetChildWithAttribute(DOMNode & CurrentNode, const XMLCh * XMLname, const XMLCh * attribval)
   {
          
    DOMNodeList * children = CurrentNode.getChildNodes();
    DOMNode * NameAttribute;
    const XMLCh * ThisNameValue; 
    unsigned int i;
    for(i=0; i<children->getLength(); i++) {
      if( children->item(i)->hasAttributes() ) {
        NameAttribute = children->item(i)->getAttributes()->getNamedItem(XMLname);
        if (NameAttribute)
	  { ThisNameValue = NameAttribute->getNodeValue();
            if (XMLString::compareIString(ThisNameValue,attribval))
            {return children->item(i);}
		}
	  }
	}               
      return NULL;
}

DOMNode * FindNodeByAttribute(DOMNode & CurrentNode, const string EntityName, const string AttribName, const string AttribVal)
   {
    const XMLCh * XEntityName = xercesc::XMLString::transcode(EntityName.c_str());     
	const XMLCh * XAttribName = xercesc::XMLString::transcode(AttribName.c_str());     
	const XMLCh * XAttribVal = xercesc::XMLString::transcode(AttribVal.c_str());
    XMLCh * XcurrentName; 
    short NodeAttrib = CurrentNode.getNodeType();
	assert(NodeAttrib == DOMNode::ELEMENT_NODE);
    DOMNodeList * children = CurrentNode.getChildNodes();
    DOMNode * NameAttribute;
    const XMLCh * ThisNameValue; 
    unsigned int i;
    for(i=0; i<children->getLength(); i++) {
      if( children->item(i)->hasAttributes() ) {
        NameAttribute = children->item(i)->getAttributes()->getNamedItem(XAttribName);
        if (NameAttribute)
	  { ThisNameValue = NameAttribute->getNodeValue();
            if (XMLString::compareIString(ThisNameValue,XAttribVal))
            {return children->item(i);}
		}
	  }
	}               
      return NULL;
}

DOMNode * FindNodeByName(DOMNode & CurrentNode, const string EntityName)
{
	// not sure whether I just could use getNamedItem
	const XMLCh * XEntityName = xercesc::XMLString::transcode(EntityName.c_str());     
	const XMLCh * XcurrentName; 
	DOMNodeList * children = CurrentNode.getChildNodes();
	unsigned int i;
	for(i=0; i<children->getLength(); i++) {
		if (children->item(i)->getNodeType() == DOMNode::ELEMENT_NODE )
		{
			// XcurrentName = children->item(i)->getNodeName();
			XcurrentName = dynamic_cast< xercesc::DOMElement* >(children->item(i))->getTagName();
			if (XMLString::compareIString(XcurrentName,XEntityName)==0)return children->item(i);
		}   
	}              
	return NULL;
}


 string Config::GetParam(const string domain, const string subdomain,  const string param_name, const string param) {



  XMLCh * XMLCdomain = xercesc::XMLString::transcode(domain.c_str()); // does this type conversion work?
  XMLCh * XMLCsubdomain = xercesc::XMLString::transcode(subdomain.c_str());
  XMLCh * XMLCparam = xercesc::XMLString::transcode(param.c_str());   
  XMLCh * XMLCparam_name = xercesc::XMLString::transcode(param_name.c_str());   


  DOMNode * domainnode = GetChildWithAttribute(*doc,XMLCdomain,XMLCparam_name);
  DOMNode * subdomainnode =  GetChildWithAttribute(*domainnode,XMLCsubdomain,XMLCparam);
   
  const XMLCh * AttribValue = ((DOMAttr *)subdomainnode)->getValue();
  char * res_cstr =xercesc::XMLString::transcode(AttribValue);
  return string(res_cstr);

return "";
 };


 

string Config::GetDataByFormat(string format,vector<string> keys, bool &error)
 {
   // cout << " Getting parm from "  << keys[keys.size()-2] << " , " << keys[keys.size()-1] << endl;
char ch;
bool done = false;
error = false;
int i; // format index
int k; //key index
int l = format.length();
int lk = keys.size();
assert(format != "");
ch = format[0];
XMLCh * resultString;
const XMLCh * XMLAttribName;  
const XMLCh * CurNodeName;
DOMAttr * currentAttribute;
short NodeAttrib;

DOMNamedNodeMap * currentAttributeList;


/* for my purpose, indexing is better than an iterator, cause of lookahead */
DOMNode * current_node = doc;k=0;
CurNodeName = current_node->getNodeName(); NodeAttrib = current_node->getNodeType();  
for(i=0;i<l && !done && current_node;ch=format[++i])
 {
	 // E=Entity,A=Attribute-Name, V-Attribute Value which must match,
  // R-Result is Attribute Value (optional, if Format ends with A,
  // T-Text section is result
   switch (ch)
   {
   case 'E' : 
	    if (i<l-2 && format[i+1] == 'A' && format[i+2] == 'V' && k <lk-2)
		{ // search for an entity which has an attribute with a predefined value
	     current_node=FindNodeByAttribute((*current_node),keys[k],keys[k+1],keys[k+2]);
		 k=k+3;
		}else
		{current_node = FindNodeByName(*current_node,keys[k++]);
		};break;
   case 'A' :  
	   
	   if (!current_node->hasAttributes()) {done = true; error=true; break;}
	     XMLAttribName = xercesc::XMLString::transcode(keys[k++].c_str());
         // currentAttributeList = ((DOMElement *)current_node)->getAttributes();
		 // currentAttribute = currentAttributeList->getNamedItem(XMLAttribName);       
		 // XMLAttribName=XStr(keys[k++].c_str()).unicodeForm();
	     currentAttribute = ((DOMElement *)current_node)->getAttributeNode(XMLAttribName); 
		 if ((i==l-1) && currentAttribute) return string(xercesc::XMLString::transcode(currentAttribute->getValue()));
		 break;
   case 'T': 
	   if (current_node->getNodeType() != DOMNode::TEXT_NODE) {error = true;}
	   else {return string(xercesc::XMLString::transcode(((xercesc::DOMCharacterData *)current_node)->getData()));
		       }
		 break;
   case 'V': 
        if (currentAttribute)  
          {
            string res;
            res=string(xercesc::XMLString::transcode(currentAttribute->getValue()));
	    // cout << "Found " << res << " for " << keys[keys.size()-2] << " , " << keys[keys.size()-1] << endl;
          return res;
	  }
   };
   CurNodeName = current_node->getNodeName(); NodeAttrib = current_node->getNodeType();  

};

// this would be the error case,  don't want to handle it now
return "";
 };
