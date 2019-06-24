#include "AStar.h"
//A*Ѱ·��Ķ���
void AStarFind::SetMap(int* mapmyPoint, int mapWidth, int mapHeight)
{

	int temp[50][50] = { 0 };
	memcpy(temp, mapmyPoint, 50 * 50 * sizeof(int));

	//���ж�ָ��Ϸ���
	if (mapmyPoint > 0)
	{
		int i = 0, j = 0;
		m_width = mapWidth;
		m_height = mapHeight;
		for (i = 0; i < mapHeight; i++)
		{
			m_vecMap.push_back(vector<int>(mapWidth, 0));
		}
		for (i = 0; i < mapHeight; i++)
		{
			for (j = 0; j < mapWidth; j++)
			{

				//m_vecMap[i][j] = mapmyPoint[i * mapWidth + j];
				m_vecMap[i][j] = temp[i][j];
			}
			
		}

		
	}
}


bool AStarFind::boxCanArrive(myPoint detmyPoint)
{
	int x = detmyPoint.m_x;
	int y = detmyPoint.m_y;
	if (x < 0 || x>m_height - 1 || y<0 || y>m_width - 1)
		return false;
	if (m_vecMap[detmyPoint.m_x][detmyPoint.m_y] == 1)
		return false;
	return true;
}



//�ҳ���ǰ����Ե��������Χ�ĵ�
vector<Box> AStarFind::findBoxsCanArrive(myPoint nowmyPoint)
{
	vector<Box> ret;
	int x = nowmyPoint.m_x;
	int y = nowmyPoint.m_y;

	//��
	int nx = x - 1;
	int ny = y;
	if (true == boxCanArrive(myPoint(nx, ny)))
	{
		if (g_Flag[myPoint(nx, ny)] == 0)
			ret.push_back(Box(myPoint(nx, ny)));
	}
	//��
	nx = x + 1;
	ny = y;
	if (true == boxCanArrive(myPoint(nx, ny)))
	{
		if (g_Flag[myPoint(nx, ny)] == 0)
			ret.push_back(Box(myPoint(nx, ny)));
	}
	//��
	nx = x;
	ny = y - 1;
	if (true == boxCanArrive(myPoint(nx, ny)))
	{
		if (g_Flag[myPoint(nx, ny)] == 0)
			ret.push_back(Box(myPoint(nx, ny)));
	}
	//��
	nx = x;
	ny = y + 1;
	if (true == boxCanArrive(myPoint(nx, ny)))
	{
		if (g_Flag[myPoint(nx, ny)] == 0)
			ret.push_back(Box(myPoint(nx, ny)));
	}

	return ret;
}

//�ҳ�Fֵ��С�ķ���
Box AStarFind::findFleast(vector<Box>& b)
{
	int Minval = 9999999;
	Box MinBox;
	for (auto c : b)
	{
		if (c.F < Minval)
		{
			Minval = c.F;
			MinBox = c;
		}
	}
	return MinBox;
}

//��box�Ƴ�List
bool AStarFind::removeBox(vector<Box>& list, Box bx)
{
	auto st = list.begin();
	auto end = list.end();
	for (; st != end; st++)
	{
		if (*st == bx)
		{
			list.erase(st);
			return true;
		}
	}
	return false;
}
//�б���Ѱ���Ƿ����ĳ��Box
bool AStarFind::findBox(vector<Box>& list, Box bx)
{
	for (auto c : list)
	{
		if (c == bx)
			return true;
	}
	return false;
}

//���������پ���
int AStarFind::CalcmyPointH(myPoint& a, myPoint& b)
{
	return abs(a.m_x - b.m_x) + abs(a.m_y - b.m_y);
}

bool AStarFind::isEnd(vector<Box> all, myPoint End)
{
	for (auto c : all)
	{
		if (c.zb == End)
			return true;
	}
	return false;
}


vector<vector<int>> AStarFind::getResult(myPoint End)
{
	vector<vector<int>> ret;
	myPoint last = End;
	while (true)
	{

		//�Ѿ�û�������� ����ѭ��
		if (last.m_x == -1 || last.m_y == -1)
		{
			break;
		}
		//���뵽�����б�
		vector<int> vec = { last.m_x ,last.m_y };
		ret.push_back(vec);

		//��ѯ�����
		myPoint tmp = g_ParentmyPoint[last];
		last = tmp;

	}
	return ret;
}


void AStarFind::SetmyPoint(int startX, int startY, int endX, int endY)
{
	this->m_startx = startX;
	this->m_starty = startY;
	this->m_endx = endX;
	this->m_endy = endY;
}

vector<vector<int>> AStarFind::StartFind()
{
	vector<vector<int>> ret;

	myPoint startmyPoint(this->m_startx, this->m_starty);
	myPoint endmyPoint(this->m_endx, this->m_endy);
	Box startBox(startmyPoint);
	Box endBox(endmyPoint);

	vector<Box> OpenList, CloseList;
	int sumStep = 0;  //��¼����ߵ���ǰ���ӵľ���
	OpenList.push_back(startBox);

	g_Flag[startmyPoint] = 1;

	//��¼ѭ������ ����һ��������Ϊ��ѭ��
	int cLoop = 0;
	while (true)
	{
		Box t;

		//�Ƿ����յ�
		if (true == isEnd(OpenList, endBox.zb))
		{

			auto tmp = getResult(endBox.zb);

			//���ص�������˳���Ǵ�ĩβ����� ��˳�򵹹���
			reverse(tmp.begin(), tmp.end());

			return tmp;
			
		}

		//�ҵ�Fֵ��С�ķ���
		t = findFleast(OpenList);

		//�Ƴ�OpenList
		removeBox(OpenList, t);


		//���뵽CloseList
		CloseList.push_back(t);

		//�ҳ���ǰ�������ҿ��Ե���ĸ���
		auto tmpp = findBoxsCanArrive(t.zb);

		//���ĸ�������һ�� ���Բ���+1
		sumStep++;
		for (auto c : tmpp)
		{
			//���Ƿ��и��Ӵ���OpenList��
			if (false == findBox(OpenList, c))
			{
				//������� ����OpenList
				//����F,G,Hֵ
				//�ѵ�ǰ������Ϊ���Ҫ����Openlist���ӵĸ��׽��
				c.H = CalcmyPointH(c.zb, endmyPoint);
				c.G = sumStep;
				c.F = c.H + c.G;
				c.parent = t.zb;
				g_ParentmyPoint[c.zb] = t.zb;

				//���뵽OpenList
				OpenList.push_back(c);

				myPoint tmp = c.zb;
				g_Flag[tmp] = 1;
			}

		}
		Box next;
		next.zb = t.zb;
		next.parent.m_x = startBox.zb.m_x;
		next.parent.m_y = startBox.zb.m_y;

		startBox = next;

		cLoop++;
		if (cLoop >= 10000)
			break;
	}
	return ret;
}