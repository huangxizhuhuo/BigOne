// BigOneDoc.cpp : CBigOneDoc 类的实现
//

#include "stdafx.h"
#include "BigOne.h"

#include "BigOneDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBigOneDoc

IMPLEMENT_DYNCREATE(CBigOneDoc, CDocument)

BEGIN_MESSAGE_MAP(CBigOneDoc, CDocument)
END_MESSAGE_MAP()


// CBigOneDoc 构造/析构

CBigOneDoc::CBigOneDoc()
{
	// TODO: 在此添加一次性构造代码

}

CBigOneDoc::~CBigOneDoc()
{
}

BOOL CBigOneDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CBigOneDoc 序列化

void CBigOneDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CBigOneDoc 诊断

#ifdef _DEBUG
void CBigOneDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBigOneDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBigOneDoc 命令
