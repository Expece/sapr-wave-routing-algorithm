#include "Header.h"

using namespace std;

void SetColor(int text, int bg) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

struct Points {
	int x = 0;
	int y = 0;
	//---------------------------------------------------
	void setCoords(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

class Solution: Points {
	const int DRP_SIZE = 25;
	vector <vector <int>> drp;
	Points A;
	Points B;
	//---------------------------------------------------
	void resizeDrp() {
		drp.resize(DRP_SIZE);
		for (int i = 0; i < DRP_SIZE; i++)
			drp[i].resize(DRP_SIZE);
	}
	//---------------------------------------------------
	void fillDrp(ifstream &fileTxt) {
		resizeDrp();
		for (int i = 0; i < DRP_SIZE; i++) {
			for (int j = 0; j < DRP_SIZE; j++) {
				fileTxt >> drp[i][j];
			}
		}
	}
	//---------------------------------------------------
	void setFirstWave() {
		if ((A.y - 1) >= 0 && drp[A.y - 1][A.x] == 0)
			drp[A.y - 1][A.x] = 1;
		if ((A.y + 1) < DRP_SIZE && drp[A.y + 1][A.x] == 0)
			drp[A.y + 1][A.x] = 1;
		if ((A.x - 1) >= 0 && drp[A.y][A.x - 1] == 0)
			drp[A.y][A.x - 1] = 1;
		if ((A.x + 1) < DRP_SIZE && drp[A.y][A.x + 1] == 0)
			drp[A.y][A.x + 1] = 1;
	}
	//---------------------------------------------------
	bool validatePoints() {
		return A.y < DRP_SIZE && A.y >= 0 && B.y < DRP_SIZE && B.y >= 0 &&
			A.x < DRP_SIZE && A.x >= 0 && B.x < DRP_SIZE && B.x >= 0;
	}
	//---------------------------------------------------
	bool checkWall(int y, int x) {
		if (drp[y][x] == -1)
			return true;
		return false;
	}
public:
	//---------------------------------------------------
	void fillDrpFromFile() {
		ifstream fileTxt;
		string filename;
		while (!(fileTxt.is_open())) {
			cout << "Input file name: ";
			cin >> filename;
			fileTxt.open(filename);
			if (!(fileTxt.is_open()))
				cout << "File dont find" << endl;
		}
		cout << "Open from " << filename << endl;
		fillDrp(fileTxt);
		fileTxt.close();
	}
	//---------------------------------------------------
	void printDrp(int setwValue) {
		cout << endl;
		for (int i = 0; i < DRP_SIZE; i++) {
			for (int j = 0; j < DRP_SIZE; j++) {
				cout << setw(setwValue);
				if (drp[i][j] == 0)
					cout << '.';
				else if (drp[i][j] == -1)
					cout << '#';
				else if (drp[i][j] == -2) {
					SetColor(4, 0);
					cout << "A";
					SetColor(7, 0);
				}
				else if (drp[i][j] == -3) {
					SetColor(4, 0);
					cout << "B";
					SetColor(7, 0);
				}
				else if (drp[i][j] == -4) {
					SetColor(9, 0);
					cout << '%';
					SetColor(7, 0);
				}
				else
					cout << drp[i][j];
			}
			cout << endl;
		}
		cout << endl;
	}
	//---------------------------------------------------
	void setPoints() {
		cout << "Input A and B coordinates(0 <= x,y < drp size):" << endl;

		bool wall = true;
		while (!validatePoints() || wall) {
			cout << "Ay: ";
			cin >> A.y;
			cout << "Ax: ";
			cin >> A.x;
			cout << "By: ";
			cin >> B.y;
			cout << "Bx: ";
			cin >> B.x;
			if (cin.fail()) {
				cin.clear();
				cout << "Invalid input.\n\n";
				cin.ignore(100, '\n');
			}
			wall = false;
			if (checkWall(A.y, A.x)) {
				wall = true;
				cout << "The coordinates of point A are the wall" << endl;
			}
			if (checkWall(B.y, B.x)) {
				wall = true;
				cout << "The coordinates of point B are the wall" << endl;
			}
			if (!wall) {
				drp[A.y][A.x] = -2;
				drp[B.y][B.x] = -3;
			}
		}
	}
	//---------------------------------------------------
	void setWaveRouting() {
		setFirstWave();
		int counter = 1;
		bool stop = false;
		while (!stop)
		{
			stop = true;
			for (int y = 0; y < DRP_SIZE; y++)
			{
				for (int x = 0; x < DRP_SIZE; x++)
				{
					if (drp[y][x] == counter)
					{
						stop = false;
						if ((y - 1) >= 0 && drp[y - 1][x] == 0)
							drp[y - 1][x] = counter + 1;
						if ((y + 1) < DRP_SIZE && drp[y + 1][x] == 0)
							drp[y + 1][x] = counter + 1;
						if ((x - 1) >= 0 && drp[y][x - 1] == 0)
							drp[y][x - 1] = counter + 1;
						if ((x + 1) < DRP_SIZE && drp[y][x + 1] == 0)
							drp[y][x + 1] = counter + 1;
					}
				}
			}
			counter++;
		}
	}
	//---------------------------------------------------
	void makeLeedWay() {
		int d = getWayLength();
		int bx = B.x;
		int by = B.y;
		while (d > 0) {
			if (by - 1 >= 0 && drp[by - 1][bx] > 0 && drp[by - 1][bx] == d)
				by = by - 1;
			else if (by + 1 < DRP_SIZE && drp[by + 1][bx] > 0 && drp[by + 1][bx] == d)
				by = by + 1;
			else if (bx - 1 >= 0 && drp[by][bx - 1] > 0 && drp[by][bx - 1] == d)
				bx = bx - 1;
			else if (bx + 1 < DRP_SIZE && drp[by][bx + 1] > 0 && drp[by][bx + 1] == d)
				bx = bx + 1;
			drp[by][bx] = -4;
			d--;
		}
	}
	//---------------------------------------------------
	bool checkWay() {
		int aCount = 0;
		int bCount = 0;
		if (A.y - 1 >= 0 && drp[A.y - 1][A.x] > 0)
			aCount++;
		if (A.y + 1 < DRP_SIZE && drp[A.y + 1][A.x] > 0)
			aCount++;
		if (A.x - 1 >= 0 && drp[A.y][A.x - 1] == 0)
			aCount++;
		if (A.x + 1 < DRP_SIZE && drp[A.y][A.x + 1] > 0)
			aCount++;

		if (B.y - 1 >= 0 && drp[B.y - 1][B.x] == 0)
			bCount++;
		if (B.y + 1 < DRP_SIZE && drp[B.y + 1][B.x] == 0)
			bCount++;
		if (B.x - 1 >= 0 && drp[B.y][B.x - 1] == 0)
			bCount++;
		if (B.x + 1 < DRP_SIZE && drp[B.y][B.x + 1] == 0)
			bCount++;

		if (aCount >= 1 && bCount >= 1)
			return true;
		return false;
	}
	//---------------------------------------------------
	int getWayLength() {
		int wayLength = DRP_SIZE * DRP_SIZE;
		int x = B.x;
		int y = B.y;
		if (y - 1 >= 0 && drp[y - 1][x] < wayLength && drp[y - 1][x] > 0)
			wayLength = drp[y - 1][x];
		if (y + 1 < DRP_SIZE && drp[y + 1][x] < wayLength && drp[y + 1][x] > 0)
			wayLength = drp[y + 1][x];
		if (x - 1 >= 0 && drp[y][x - 1] < wayLength && drp[y][x - 1] > 0)
			wayLength = drp[y][x - 1];
		if (x + 1 < DRP_SIZE && drp[y][x + 1] < wayLength && drp[y][x + 1] > 0)
			wayLength = drp[y][x + 1];

		if (y - 1 >= 0 && drp[y - 1][x] == -2)
			return 0;
		if (y + 1 < DRP_SIZE && drp[y + 1][x] == -2)
			return 0;
		if (x - 1 >= 0 && drp[y][x - 1] == -2)
			return 0;
		if (x + 1 < DRP_SIZE && drp[y][x + 1] == -2)
			return 0;
		return wayLength;
	}
	
};

int main() {
	Solution app;
	app.fillDrpFromFile();
	app.printDrp(4);
	app.setPoints();
	app.printDrp(4);
	if (!app.checkWay()) {
		cout << "No way" << endl;
		return 0;
	}
	app.setWaveRouting();
	app.printDrp(4);
	app.makeLeedWay();
	app.printDrp(4);
	cout << "Way length: " << app.getWayLength() << endl;
	return 0;
}
