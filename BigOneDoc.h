// BigOneDoc.h : CBigOneDoc ��Ľӿ�
//


#pragma once


class CBigOneDoc : public CDocument
{
protected: // �������л�����
	CBigOneDoc();
	DECLARE_DYNCREATE(CBigOneDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CBigOneDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


