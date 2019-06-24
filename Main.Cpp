#include "myMap.h"
#include "RectChoose.h"
#include "AStar.h"
#include <mmsystem.h>
#include <comutil.h>  

#pragma comment(lib, "winmm.lib") 
#pragma comment(lib, "comsuppw.lib")

// ȫ�ֱ���:
gameMap *g_map=nullptr;
HINSTANCE g_hInst;                                // ��ǰʵ��
HWND g_parentHwnd = 0;							  //�����ڵĴ��ھ��
HWND g_myhWnd = 0;								  //��ǰ���ڵĴ��ھ��
CRectCall g_call;								  //����������ھ�����굥���Ķ���
int mapWidth=0, mapHeight=0;					  //��ͼ�ڴ����ϵĿ��Ⱥ͸߶�
CRectChooseText *g_findPath=nullptr;			  //���Զ�Ѱ·������

int g_Click = 0;								 //���������
bool g_ExitFlag = false;						 //��ǰ�����˳����
// �˴���ģ���а����ĺ�����ǰ������:
ATOM                MyRegisterClass2(HINSTANCE hInstance);
BOOL                InitInstance2(HINSTANCE, int);
LRESULT CALLBACK    WndProc2(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain2(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	g_Click = 0;
	mapWidth = mapHeight = 0;
	g_myhWnd = 0;
	g_parentHwnd = 0;

	g_parentHwnd = (HWND)hPrevInstance;			//���������ڴ��ھ�� ��Ϊ��Ϸ��������Ҫ��ʾ����
	wstring tmp(lpCmdLine);						//��ʱ��wstring���� 
	g_map = new gameMap(WstrToStr(tmp).c_str());//wstring����תΪstring����
	 

	// TODO: �ڴ˴����ô��롣


	// ��ʼ��ȫ���ַ���
	MyRegisterClass2(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance2(hInstance, nCmdShow))
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
ATOM MyRegisterClass2(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc2;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = L"Migong2";
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
BOOL InitInstance2(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	

	//���ݲ�ͬ�ĵ�ͼ��С �������Ҫ�Ŀ���
	auto wh = g_map->calcMapWidthHeight();
	mapWidth = get<0>(wh);
	mapHeight = get<1>(wh);

	//�ڵ�ͼ��С�Ļ����ϼ���һ��ƫ�ƴ�С �����ռ�����ʾ����ѡ��
	HWND hWnd = CreateWindowW(L"Migong2", L"����������", WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, 0, mapWidth + 250, mapHeight + 50, 0, 0, hInstance, 0);


	//���ڴ�����Ϻ�
	g_findPath = new CRectChooseText(mapWidth , 20, 220, 40, "���ǲ�����,��ʼѰ·", findPath);
	g_findPath->SetText("���ǲ�����,��ʼѰ·");
	g_call.RegisterRectChoose(g_findPath);

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
LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE://�������ڴ������
	{
		g_myhWnd = hWnd;

		g_call.SetHwnd(hWnd);

		//�����ڴ������ ����ͼƬ��Դ
		g_map->loadPic(g_hInst);


		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WatchThread, NULL, NULL, NULL);

		break;
	}
	case WM_LBUTTONDOWN:
	{
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		//������������
		g_map->UpDateXY(xPos, yPos);

		//֪ͨ���������Ϣ
		g_call.RcevieClick(xPos, yPos);

		g_Click ++ ;

		//�����ڸոմ�����ʱ������Ϊtrue
		g_ExitFlag = true;
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		RECT rect;
		//��ʼ���� ����һ����Ļ������
		HDC hdc = BeginPaint(hWnd, &ps);


		//��ȡ���ڴ�С
		GetWindowRect(hWnd, &rect);
		//����һ��ʹ��ڻ������ô���ڴ滭��
		auto bitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
		auto mdc = CreateCompatibleDC(hdc);
		//������Ҫ���Ļ���ѡ���ڴ滭����
		SelectObject(mdc, bitmap);

		//��ͼ ������ͼ����������ڴ滭����
		g_map->setHwnd(hWnd);
		g_map->setDc(mdc);
		g_map->drawIndc(mdc);

		//������ڴ滭���������ǵ���Ļ������
		BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, mdc, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, mdc);


		//֪ͨ�������ǵ�����
		g_call.RecviePaint(hWnd, hdc);

		//��������
		EndPaint(hWnd, &ps);


		break;
	}

	case WM_DESTROY:
		//�����˳�����Ϊfalse
		g_ExitFlag = false;
		PostQuitMessage(0);
		ShowWindow(g_parentHwnd, SW_SHOW);

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//�������� �ļ�·���ǳ�������Ŀ¼�µ��ļ�jntm.mp3
void playstartMusic()
{
	mciSendStringA("open jntm.mp3 alias media", NULL, NULL, NULL);
	mciSendStringA("play media", NULL, NULL, NULL);
}

//�ӵ�ǰ�������� ��ʼѰ·
void findPath()
{
	//���Ȼ�ȡ�����ڶ�ά�����е���������
	//��ǰ�������� ���
	auto [x,y] = g_map->getMapIndexPos();
	//��ͼ�յ�
	auto [winEndx,winEndY,arryEndx, arryEndy] = g_map->getEndPoint();
	//��ͼָ��
	int* pMap = g_map->getMapPointer();
	//��ͼ���ȸ߶�
	auto [w,h] = g_map->getMapWidthHeight();
	

	//����һ��A*Ѱ·����
	AStarFind* pAStart = new AStarFind;
	pAStart->SetMap(pMap, w, h);	//���õ�ͼ
	pAStart->SetmyPoint(x, y, arryEndx, arryEndy);	//��������յ�
	auto result=pAStart->StartFind();
	
	DrawPath(result);
}

//����·�� �����Ǹ�����������Ϸ���
void DrawPath(vector<vector<int>> Path)
{
	HDC dc;
	//��ȡһ���������
	dc = GetDC(g_myhWnd);
	//����һ��GDI+���� 
	Gdiplus::Graphics gp(dc);

 
	Gdiplus::SolidBrush* hBrush = new SolidBrush(Color(125,0,125,0)); //͸����125  ��ɫ
	for (auto z:Path)
	{
		int arryx = z[0];
		int arryy = z[1];
		

		auto [winy,winx]=g_map->getWindowPointByMapPoint(arryx, arryy);

		//gp.DrawRectangle(hPen, winx, winy, gameMap::m_box_width, gameMap::m_box_height);
		gp.FillRectangle(hBrush,winx,winy, gameMap::m_box_width, gameMap::m_box_height);
	}

	//�ͷ�
	ReleaseDC(g_myhWnd,dc);

	delete hBrush;
}

//���ڼ�¼����� �� ��ʾʱ��
void WatchThread()
{
	int iTime = 0;
	char buf[30] = { 0 };
	HDC dc = GetDC(g_myhWnd);

	//����ʹ�ñ���ɫ ����ʹ�ú�ɫ
	SetBkMode(dc, OPAQUE);
	SetBkColor(dc, RGB(255, 0, 0));

	HFONT ft = CreateFontA(-20/*�߶�*/, -7/*����*/, 0/*���ù�*/, 0/*���ù�*/, 400 /*һ�����ֵ��Ϊ400*/, FALSE/*����б��*/, FALSE/*�����»���*/, FALSE/*����ɾ����*/, DEFAULT_CHARSET,      OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,                 DEFAULT_QUALITY,                 FF_DONTCARE,      "΢���ź�"              );
		
	SelectObject(dc, ft);

	while (true)
	{
		//HWND g_myhWnd = 0;								  //��ǰ���ڵĴ��ھ��
	
		sprintf_s(buf, "Time:%05d(s)", iTime);

		TextOutA(dc, mapWidth, 100,buf, strlen(buf));


		sprintf_s(buf, "��������:%02d", g_Click);

		TextOutA(dc, mapWidth, 300, buf, strlen(buf));

		Sleep(1000);
		
		iTime++;

		//�����ǰ�����Ѿ��ر��� ���˳��߳�
		if (g_ExitFlag == true)
		{
			//ExitThread(0);
		}

		
		
	}

	ReleaseDC(g_myhWnd, dc);
	

}

/*
296:�ο�https://blog.csdn.net/softn/article/details/51718347
*/