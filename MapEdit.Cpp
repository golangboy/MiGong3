#include "myMap.h"
#include "RectChoose.h"
#include "AStar.h"
#include "MapEdit.h"
#include <mmsystem.h>
#include <comutil.h>  

#pragma comment(lib, "winmm.lib") 
#pragma comment(lib, "comsuppw.lib")

// ȫ�ֱ���:
HWND g_mainHwnd = 0;
CMapEdit g_mapEd;
HINSTANCE g_hins=0;
// �˴���ģ���а����ĺ�����ǰ������:
ATOM                MyRegisterClass3(HINSTANCE hInstance);
BOOL                InitInstance3(HINSTANCE, int);
LRESULT CALLBACK    WndProc3(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain3(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	g_hins = hInstance;

	//Ĭ�����õ�ͼ��� 30��20��
	g_mapEd.setMapSize(30, 20);
	g_mainHwnd = (HWND)hPrevInstance;


	// TODO: �ڴ˴����ô��롣


	// ��ʼ��ȫ���ַ���
	MyRegisterClass3(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance3(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = 0;

	MSG msg;

	// ����Ϣѭ��:
	while (GetMessage(&msg, 0, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return (int)msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass3(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc3;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = L"Migong3";
	wcex.hIconSm = 0;

	return RegisterClassExW(&wcex);
}


//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance3(HINSTANCE hInstance, int nCmdShow)
{
 
	int mapWidth=0, mapHeight=0;

	//���ݲ�ͬ�ĵ�ͼ��С �������Ҫ�Ŀ���
	//��������Ķ�ά�����ͼ�Ĵ�С����Ӧ�ô����Ĵ��ڴ�С
	auto [windowWidth,windowHeight] = g_mapEd.calcMapWidthHeight();

 
	HWND hWnd = CreateWindowW(L"Migong3", L"��ͼ�༭��", WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, 0, windowWidth+10, windowHeight+10, 0, 0, hInstance, 0);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc3(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int ret = 0;
	
	switch (message)
	{
	case WM_CREATE://�������ڴ������
	{
		
		g_mapEd.loadPic(g_hins);

		break;
	}
	case WM_LBUTTONDOWN:
	{
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		auto [iX,iY,wx,wy] = g_mapEd.getMapPointByWindowPoint(xPos, yPos);
		int val = g_mapEd.getMapRes(iX,iY);
		val = (1+val) % 4;
		g_mapEd.setMapNode(iX, iY, val);
		RECT rt = { 0 };
		rt.left = wx;
		rt.top = wy;
		rt.bottom = rt.top + gameMap::m_box_height;
		rt.right = rt.left + gameMap::m_box_height;
		InvalidateRect(hWnd, &rt, true);


		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		
		g_mapEd.drawMap(ps.hdc);

		//��������
		EndPaint(hWnd, &ps);


		break;
	}
	case WM_CLOSE:
	{	
		ret = MessageBoxA(g_mainHwnd, "�Ƿ񱣴��ͼ?", "ѯ���㰡", MB_OKCANCEL);
		if (ret == 1)
		{
			g_mapEd.saveMapToFile();
			MessageBoxA(g_mainHwnd, "��ͼ�Ѿ������ڵ�ǰĿ¼��newmap.txt", "��ʾ", MB_OK | MB_ICONMASK);
		}
		DestroyWindow(hWnd);
		break;
	}
	case WM_DESTROY:
		ShowWindow(g_mainHwnd, SW_SHOW);
		PostQuitMessage(0);
		
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void CMapEdit::drawMap(HDC drawdc)
{
	unsigned int i, j;
	int ls = 0;

	//���Ʊ���ͼ
	

	for (j = 0; j < this->m_height; j++)
	{
		for (i = 0; i < this->m_width; i++)
		{
			int state = 0;
			int srcx, srcy;

			Gdiplus::Graphics graphics(drawdc);
			state = this->m_Map[j][i];
			auto p1 = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0));
			Gdiplus::SolidBrush* b1;
			srcx = m_box_width * i;
			srcy = j * m_box_height;

			tuple<int,int> winXY;
			switch (state)
			{
			case 1://�ϰ�
				//����gdi+������������ Ư��
				b1 = new Gdiplus::SolidBrush(Color(100, 255, 0, 0));

				graphics.FillRectangle(b1, srcx, srcy, m_box_width, m_box_height);

				delete b1;
				break;
			case 2://���
				winXY = this->getWindowPointByMapPoint(i, j);
				drawBmp(drawdc, m_qid, get<0>(winXY), get<1>(winXY), m_box_width, m_box_height);
				break;
			case 3://�����յ�

				//�����յ�����
				winXY = this->getWindowPointByMapPoint(i, j);
				drawBmp(drawdc, m_zhd, get<0>(winXY), get<1>(winXY), m_box_width, m_box_height);
				break;

			}
			//���Ʒ���
			graphics.DrawRectangle(p1, 0 + m_box_width * i, 0 + j * m_box_height, m_box_width, m_box_height);

		}
	}
}

void CMapEdit::setMapSize(int w, int h)
{
	this->m_width = w;
	this->m_height = h;
}
void CMapEdit::setMapNode(int x, int y,int val)
{

	this->m_Map[x][y] = val;
}
void CMapEdit::saveMapToFile()
{
	int i=0, j=0;
	ofstream ofmap;
	ofmap.open("newmap.txt", ios::out);
	if (ofmap.is_open())
	{
		for (i=0;i<this->m_height;i++)
		{
			for (j = 0; j < this->m_width; j++)
			{
				ofmap << m_Map[i][j];
				if (j != this->m_width - 1)
				{
					ofmap << ",";
				}
			}
			ofmap << endl;
		}
		ofmap.close();
	}
	
}