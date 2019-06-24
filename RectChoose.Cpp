#include "RectChoose.h"
//CRectCall��
void CRectCall::RcevieClick(int x, int y)
{
	//�������еĶ���
	for (auto c:m_allObject)
	{
		RECT rt;
	
		rt = c->getRect();
		//�ж��Ƿ��ڷ�Χ��
		if (x > rt.left && x<rt.right && y>rt.top && y < rt.bottom)
		{
			c->click();//������������click����֪ͨ��
		}
	}
}

void CRectCall::RegisterRectChoose(CRectChoose *obj)
{
	this->m_allObject.push_back(obj);
}

void CRectCall::SetHwnd(HWND hwnd)
{
	this->m_hWnd = hwnd;
}
void CRectCall::RecviePaint(HWND hWnd,HDC dc)
{
	for (auto c : m_allObject)
	{
		c->paint(hWnd,dc);
	}
}
//





//CRectChoose��
CRectChoose::CRectChoose()
{
	this->m_rect = { 0 };
	this->m_dc = 0;
	this->m_pFuncClickpointer = nullptr;
	this->m_pFuncPaintpointer = nullptr;
}
RECT CRectChoose::getRect()
{
	return this->m_rect;
	
}

//






//CRectChooseText��
void CRectChooseText::SetText(string str)
{

	this->m_drawStr = str;
	 
	return;
}
void CRectChooseText::CalcStrRect(int fontSize,int& w, int& h)
{
	 
	Graphics gp(m_dc);
	wstring tmp = StrToWstr(m_drawStr);
	Font* ft = new Font(L"΢���ź�",(REAL)fontSize);
	RectF boundRect;
	gp.MeasureString(tmp.c_str(), tmp.length(), ft, PointF(0, 0), &boundRect);
	w = (int)boundRect.Width;
	h = (int)boundRect.Height;
	delete ft;
}
void CRectChooseText::click()
{
	if(m_pFuncClickpointer>0)
		this->m_pFuncClickpointer();
}
void CRectChooseText::paint(HWND hWnd,HDC dc)
{
	m_dc = dc;

	//���Ƴ�����
	this->DrawRectangle(hWnd,dc);

	//���Ƴ�����
	this->DrawOutText(hWnd,dc);

	//�������ע��Ļص�����
	if (m_pFuncPaintpointer > 0)
		this->m_pFuncPaintpointer(dc);
}

void CRectChooseText::DrawOutText(HWND hwnd, HDC dc)
{
	Gdiplus::Graphics gp(dc);
	//gp.DrawString()
	wstring tmp = StrToWstr(m_drawStr);					//���ֽ�תΪ���ֽڰ汾
	Font* ft = new Gdiplus::Font(L"΢���ź�",16);		//����Ĭ��ȫ��ʹ��΢���ź����� ����ʹ��16�Ŵ�С
	SolidBrush* sb = new SolidBrush(Color(100,125,0,255));//ʹ����ɫ���� ��͸��
	int w=0, h=0;
	CalcStrRect(16, w, h);								//��ȡ������Ⱥ͸߶�
	int x = m_rect.left;
	int y = m_rect.top;
	int rectWidth = m_rect.right - m_rect.left;
	int rectHeight = m_rect.bottom - m_rect.top;
	gp.DrawString(tmp.c_str(), tmp.length(), ft,PointF(REAL( x+ rectWidth/2-(w/2)), REAL(y+rectHeight/2-(h/2))), sb);
	delete ft;
	delete sb;
}
void CRectChooseText::DrawRectangle(HWND hWnd,HDC dc)
{
	//����һ��GDI+ͼ�ζ���
	Gdiplus::Graphics gp(dc);
	//����һֻ����
	Gdiplus::Pen hPen(Color(50,255,0,0),5.0f);

	//Gdiplus::SolidBrush hBrush(Color(100, 255, 0, 0));
	Gdiplus::Rect hRect(m_rect.left,  m_rect.top,  m_rect.right-m_rect.left,  m_rect.bottom-m_rect.top);
	//gp.DrawArc((const Gdiplus::Pen*)&hPen,hRect,100,360.0f);
	gp.DrawRectangle((const Gdiplus::Pen*) & hPen, hRect);

}
CRectChooseText::CRectChooseText(int x, int y, int w, int h, string str, pfunClick p)
{
	//�����ı���ʼ���� Ȼ�������ȸ߶�
	this->m_rect.left = x;
	this->m_rect.top = y;
	this->m_rect.right = x + w;
	this->m_rect.bottom = y + h;
	this->m_drawStr = str;
	

	//���õ���굥����ʱ��Ҫ���õ��Ǹ������ĺ�����ַ
	this->m_pFuncClickpointer = p;

}

//