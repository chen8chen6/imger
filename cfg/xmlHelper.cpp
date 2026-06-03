#include "xmlHelper.h"
#include <QDomDocument>
#include <QDebug>

void CXmlHelper::appendChild(QDomDocument& doc, QDomElement& parent, const TXmlNode& xmlNode)
{
    auto child = doc.createElement(xmlNode.tagName);
    parent.appendChild(child);

    //value
    if (!xmlNode.val.isEmpty())
        child.appendChild(doc.createTextNode(xmlNode.val));

    //attribute
    if (!xmlNode.attrList.empty())
    {
        for (const auto& attr : xmlNode.attrList)
        {
            auto attrNode = doc.createAttribute(attr.first);
            attrNode.setValue(attr.second);
            child.setAttributeNode(attrNode);
        }
    }

    //comment
    if (!xmlNode.comment.isEmpty())
        child.appendChild(doc.createComment(xmlNode.comment));

    return;
}

int CXmlHelper::parseChild(TXmlNode& nodeParsed, QDomElement& parent, const QString &tagName)
{
    auto tgt = parent.firstChildElement(tagName);
    if (tgt.isNull())
    {
        qDebug() << "warnning: xml node <" << tagName << "> not found!";
        return -1;
    }

    //name, val
    nodeParsed.tagName = tgt.tagName();
    nodeParsed.val = tgt.text();

    //attributes
    nodeParsed.attrList.clear();
    const auto attrMap = tgt.attributes();
    const int cnt = attrMap.count();
    for (int i = 0; i < cnt; ++i)
    {
        const auto attrNode = attrMap.item(i).toAttr();
        nodeParsed.attrList.emplace_back(attrNode.name(), attrNode.value());
    }

    //comment-ÔÝ²»½âÎö
    nodeParsed.comment.clear();

    return 0;
}

