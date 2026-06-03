#ifndef XML_HELPER_H
#define XML_HELPER_H

#include <vector>
#include <memory>   //std::pair
#include <QString>

class QDomDocument;
class QDomElement;

class CXmlHelper
{
public:
    typedef struct tag_xmlNode
    {
        QString tagName;
        QString val;
        std::vector<std::pair<QString, QString>> attrList;  //<attrName, value>
        QString comment;
    } TXmlNode;

public:
    static void appendChild(QDomDocument& doc, QDomElement& parent, const TXmlNode &xmlNode);
    static int parseChild(TXmlNode& nodeParsed/*Output*/, QDomElement& parent, const QString& tagName);
    //static int parseInt(QDomElement& parent, const QString& tagName
        /*, IN_OUT tag_xmlNode &xmlNode*/
        ///*function<> greatThen(0) :public validator, defVal*/);

private:
    CXmlHelper(void) = delete;
    CXmlHelper(CXmlHelper&) = delete;
    CXmlHelper(CXmlHelper&&) = delete;
};


#endif	//!XML_HELPER_H