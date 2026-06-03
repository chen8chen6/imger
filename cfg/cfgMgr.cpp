#include "cfgMgr.h"
#include <QDomDocument>
#include <QSaveFile>
#include <QFile>
#include <QCoreApplication>	//applicationDirPath()
#include <QDebug>
#include "xmlHelper.h"

//TODO: imgView里要用的放到另一个头文件里
#define EMPTY_QSTR		QStringLiteral("")
#define TGT_XML			QStringLiteral("xml")
#define DATA_VER_ENCODE	QStringLiteral("version=\"1.0\" encoding=\"UTF-8\"")
#define TAG_CONFIG		QStringLiteral("config")
#define TAG_IMG_VIEW	QStringLiteral("imgView")
#define TAG_DSP_STGY	QStringLiteral("dspStrategy")
#define TAG_DSP_ORDER	QStringLiteral("dspOrder")
#define TAG_KEY_BINDING QStringLiteral("keyBinding")
//#define TAG_NEXT_PIC	QStringLiteral("nextPic")
//#define TAG_PREV_PIC	QStringLiteral("prevPic")
#define TAG_KEY_USAGE	QStringLiteral("keyUsage")

#define NO_TAG_VAL		EMPTY_QSTR
#define NO_ATTR			{}
#define ATTR_KEY_HASH	QStringLiteral("keyHash")
#define ATTR_USAGE		QStringLiteral("usage")

#define USAGE_NEXT_PIC	QStringLiteral("nextPic")
#define USAGE_PREV_PIC	QStringLiteral("prevPic")

#define NO_COMMENT		EMPTY_QSTR
#define COMMENT_IMG_VIEW QStringLiteral("used to browse imgs")
#define COMMENT_DSP_STGY QStringLiteral("0: RealSize; 1: FitWin")
#define COMMENT_DSP_ORDER QStringLiteral("0: SortByName, 1: SortByTime")

#define APPEND_KEY_USAGE(key, mod, usage) \
	do { appendKeyUsage(doc, keyBinding, CCfgHelper::getKeyHash(key, mod), usage); } while(0)

CCfgMgr* CCfgMgr::getSingleton(void)
{
	static CCfgMgr singleton;
	return &singleton;
}

int CCfgMgr::load(void)
{
	static const QString XML_FILE_PATH(QCoreApplication::applicationDirPath() + "/" + CFG_FILE_NAME);
	QFile xmlFile(XML_FILE_PATH);

	bool isSucc = false;
	do
	{
		QDomDocument doc;
		QString errorMsg;
		int errLine = 0, errCol = 0;
		if (false == doc.setContent(&xmlFile, &errorMsg, &errLine, &errCol))
		{
			qDebug() << XML_FILE_PATH << (xmlFile.exists() ? "exists" : "not exist");
			qDebug() << errorMsg << "line:" << errLine << ", errCol:" << errCol;
			break;
		}

		//root
		auto root = doc.documentElement();
		if (root.isNull())
			break;

		//imgView
		if (0 != parseCfg_imgView(doc, root))
			break;

		isSucc = true;
	} while (0);


	return isSucc ? 0 : -1;
}

int CCfgMgr::save(void) const
{
	QDomDocument doc;

	//declaration
	auto decl = doc.createProcessingInstruction(TGT_XML, DATA_VER_ENCODE);
	doc.appendChild(decl);

	//root
	auto root = doc.createElement(TAG_CONFIG);
	doc.appendChild(root);

	//imgView-keyBinding
	buildCfg_imgView(doc, root);

	return saveFile(doc);
}

void CCfgMgr::buildCfg_imgView(QDomDocument &doc, QDomElement &root) const
{

	auto imgView = doc.createElement(TAG_IMG_VIEW);
	root.appendChild(imgView);
	imgView.appendChild(doc.createComment(COMMENT_IMG_VIEW));

	//dspStgy
	static const QString FIT_TO_WIN("1");
	CXmlHelper::appendChild(doc, imgView,
		CXmlHelper::TXmlNode{ TAG_DSP_STGY, FIT_TO_WIN, NO_ATTR, COMMENT_DSP_STGY });
	
	//dspOrder
	static const QString SORT_BY_NAME("0");
	CXmlHelper::appendChild(doc, imgView,
		CXmlHelper::TXmlNode{ TAG_DSP_ORDER, SORT_BY_NAME, NO_ATTR, COMMENT_DSP_ORDER });

	//keyBinding
	auto keyBinding = doc.createElement(TAG_KEY_BINDING);
	imgView.appendChild(keyBinding);

	APPEND_KEY_USAGE(Qt::Key_F1, NO_MODIFILER, Usage::Help);
	APPEND_KEY_USAGE(Qt::Key_X, NO_MODIFILER, Usage::Next_Img);
	APPEND_KEY_USAGE(Qt::Key_Right, MOD_CTRL, Usage::Next_Img);
	APPEND_KEY_USAGE(Qt::Key_Z, NO_MODIFILER, Usage::Prev_Img);
	APPEND_KEY_USAGE(Qt::Key_Left, MOD_CTRL, Usage::Prev_Img);

	APPEND_KEY_USAGE(Qt::Key_Plus, NO_MODIFILER, Usage::ZoomIn);
	APPEND_KEY_USAGE(Qt::Key_Plus, MOD_SHIFT, Usage::ZoomIn);	//使用 +/= 键时, 需要按shift输入+
	APPEND_KEY_USAGE(Qt::Key_Minus, NO_MODIFILER, Usage::ZoomOut);
	APPEND_KEY_USAGE(Qt::Key_Equal, NO_MODIFILER, Usage::ZoomReset);
	APPEND_KEY_USAGE(Qt::Key_Up, NO_MODIFILER, Usage::SightUp);
	APPEND_KEY_USAGE(Qt::Key_Down, NO_MODIFILER, Usage::SightDown);
	APPEND_KEY_USAGE(Qt::Key_Left, NO_MODIFILER, Usage::SightLeft);
	APPEND_KEY_USAGE(Qt::Key_Right, NO_MODIFILER, Usage::SightRight);
	APPEND_KEY_USAGE(Qt::Key_Up, MOD_SHIFT, Usage::SightUp_1px);
	APPEND_KEY_USAGE(Qt::Key_Down, MOD_SHIFT, Usage::SightDown_1px);
	APPEND_KEY_USAGE(Qt::Key_Left, MOD_SHIFT, Usage::SightLeft_1px);
	APPEND_KEY_USAGE(Qt::Key_Right, MOD_SHIFT, Usage::SightRight_1px);

	return;
}

int CCfgMgr::parseCfg_imgView(QDomDocument &doc, QDomElement &root)
{
	bool isSucc = false;
	do
	{
		CXmlHelper::TXmlNode parsedNode;	//用来接收节点解析结果

		//TODO: 找不到节点, 不中止解析, 而是用默认值
		auto imgView = root.firstChildElement(TAG_IMG_VIEW);
		if (imgView.isNull())
			break;

		//dspStgy
		if (0 == CXmlHelper::parseChild(parsedNode, imgView, TAG_DSP_STGY))
			m_cfg.imgView.dspStgy = parsedNode.val.toInt();
		qDebug() << "dspStgy:" << m_cfg.imgView.dspStgy;

		//dspOrder
		if (0 == CXmlHelper::parseChild(parsedNode, imgView, TAG_DSP_ORDER))
			m_cfg.imgView.dspOrder = parsedNode.val.toInt();
		qDebug() << "dspOrder:" << m_cfg.imgView.dspOrder;

		//keyBinding
		auto &keyBindingCfg = m_cfg.imgView.keyUsageDict;
		auto keyUsageDict = imgView.firstChildElement(TAG_KEY_BINDING);
		if (keyUsageDict.isNull())
			break;

		auto keyUsage = keyUsageDict.firstChildElement();
		while (!keyUsage.isNull())
		{
			//TODO: 不能有1个按键被分配到2个功能
			keyHash_t keyHash = keyUsage.attribute(ATTR_KEY_HASH).toLongLong(nullptr, 16);
			Usage usage = static_cast<Usage>(keyUsage.attribute(ATTR_USAGE).toInt(nullptr, 16));
			keyBindingCfg.insert({keyHash, usage});
			keyUsage = keyUsage.nextSiblingElement();
			qDebug() << QString::number(keyHash, 16) << QString::number(static_cast<int>(usage),  16);
		}

		isSucc = true;
	} while (0);
	return 0;
}

int CCfgMgr::saveFile(QDomDocument& doc) const
{
	static const QString XML_FILE_PATH(QCoreApplication::applicationDirPath() + "/" + CFG_FILE_NAME);
	QSaveFile xmlFile(XML_FILE_PATH);
	xmlFile.open(QIODevice::WriteOnly);
	if (!xmlFile.isOpen())
	{
		qDebug() << "open" << xmlFile.fileName() << "failed";
		return -1;
	}
	qDebug() << doc.toString();
	xmlFile.write(doc.toByteArray());
	bool isSucc = xmlFile.commit();

	return isSucc ? -1 : 0;
}

void CCfgMgr::appendKeyUsage(QDomDocument& doc, QDomElement& parent, keyHash_t keyHash, Usage usage)
{
	CXmlHelper::appendChild(doc, parent, CXmlHelper::TXmlNode{
		TAG_KEY_USAGE, NO_TAG_VAL,
		{
			{ATTR_KEY_HASH, QString::number(keyHash, 16)},
			{ATTR_USAGE, QString::number(static_cast<int>(usage), 16)}
		},
		NO_COMMENT });
	return;
}
