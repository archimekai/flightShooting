#pragma once
#include "fsObjects.h"
#include "fsDraw.h"

int fstestOverlap() {
	fsMyAircraft playerAircraft = fsMyAircraft(fsPoint<double>(0.5, 0.99),
		fsPoint<double>(0.51, 1.00),
		fsColor(255, 255, 255, 0));
	fsEnemyAircraft enemy1 = fsEnemyAircraft(fsPoint<double>(0.505, 0.995),
		fsPoint<double>(0.515, 1.005),
		fsColor(255, 0, 0, 0));

	fsDraw(playerAircraft);
//	fsDraw(mybullet);
	fsDraw(enemy1);
	bool isoverlap = fsOverlap(playerAircraft, enemy1);
	cout << "JUDEG" << isoverlap;

	system("pause");
	return 0;
}

// !!!!这个函数要挪到其它的文件中
int fsGameLoop()
{
	fsMyAircraft playerAircraft = fsMyAircraft(fsPoint<double>(0.5, 0.99),
		fsPoint<double>(0.51, 1.00),
		fsColor(255, 255, 255, 0));
	fsEnemyAircraft enemy1 = fsEnemyAircraft(fsPoint<double>(0.5, 0.10),
		fsPoint<double>(0.51, 0.11),
		fsColor(255, 0, 0, 0));

	fsScoreBoard scoreBoard = fsScoreBoard(fsPoint<double>(1.1, 0.3), fsPoint<double>(1.11, 0.31));
	// 实现双缓冲技术
	// 建立矩形以存储缓冲数据
	SMALL_RECT srctReadRect, srctWriteRect;
	CHAR_INFO chiBuffer[7200];
	// 确定源矩阵和目标矩阵的大小
	srctReadRect.Top = 0;
	srctReadRect.Left = 0;
	srctReadRect.Right = 79;
	srctReadRect.Bottom = 60;

	srctWriteRect = srctReadRect;

	COORD coordBufSize;
	COORD coordBufCoord;

	// 确定暂存空间的大小为2 * 80 
	coordBufSize.X = 80;
	coordBufSize.Y = 90;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;



	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 0,0 };
	DWORD bytes = 0;
	char data[10000];
	// 创建新的缓冲区
	HANDLE hOutBuf = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL);
	COORD newsize = { 100,80 };
	SetConsoleScreenBufferSize(hOutBuf, newsize);
	HANDLE output[2] = { hOutput, hOutBuf };
	//设置新的缓冲区为活动显示缓冲
	SetConsoleActiveScreenBuffer(hOutBuf);
	//隐藏两个缓冲区的光标
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = 0;
	cci.dwSize = 1;
	SetConsoleCursorInfo(hOutput, &cci);
	SetConsoleCursorInfo(hOutBuf, &cci);
	
	
	Sleep(1000);
	fsBullet mybullet;
	if (playerAircraft.fireReady()) {
		mybullet = playerAircraft.fire();
	}

	char ch = '/0';
	while (1) {
		if (_kbhit())
		{
			ch = getche();

		}
		switch (ch)
		{
		case FS_MOVE_LEFT_KEY:
			playerAircraft.moveleft();
		//	playerAircraft.printpos();
			break;
		case FS_MOVE_RIGHT_KEY:
			playerAircraft.moveright();
		//	playerAircraft.printpos();
			break;
		case FS_ESCAPE_KEY:
			cout << "You ended this game!";
			return 0;
		default:
			break;
		}
		ch = 0;
		if (playerAircraft.fireReady()) {
			mybullet = playerAircraft.fire();
		}
		mybullet.updataPosition();
		//	mybullet.printpos();
		//if (playerAircraft.fireReady()) cout << "Ready to fire!";
		//system("cls");
		fsFillWithBlank();

		////// 呈现图像
		fsDraw(playerAircraft);
		fsDraw(mybullet);
		fsDraw(enemy1);
		fsDrawScoreBoard(scoreBoard);

		ReadConsoleOutput(
			hOutput,
			chiBuffer,
			coordBufSize,
			coordBufCoord,
			&srctReadRect);
		WriteConsoleOutput(
			hOutBuf,
			chiBuffer,
			coordBufSize,
			coordBufCoord,
			&srctWriteRect);
		// 子弹击中的判定
		if (fsOverlap(mybullet, enemy1)) {
			scoreBoard.addScore();
			cout << "HIT!!!!";
			Sleep(1000);
		}
		Sleep(FS_DEFAULT_FRAME_INTERVAL);
	}

}


int gameTestOneBuf() {
	// 初始化玩家飞机
	fsMyAircraft playerAircraft = fsMyAircraft(fsPoint<double>(0.5, 0.99),
		fsPoint<double>(0.51, 1.00),
		fsColor(255, 255, 255, 0));
	// 初始化敌方飞机
	fsEnemyAircraft enemy1 = fsEnemyAircraft(fsPoint<double>(0.5, 0.10),
		fsPoint<double>(0.51, 0.11),
		fsColor(255, 0, 0, 0));
	// 初始化计分板
	fsScoreBoard scoreBoard = fsScoreBoard(fsPoint<double>(1.1, 0.3), fsPoint<double>(1.11, 0.31));
	// 等待1s以便能够通过fire函数成功初始化子弹
	Sleep(1000);
	fsBullet mybullet;
	if (playerAircraft.fireReady()) {
		mybullet = playerAircraft.fire();
	}
	fsBullet enemybullet;
	if (enemy1.fireReady()) {
		enemybullet = enemy1.fire();
	}
	// 初始化键盘按键存储变量
	char ch = '/0';

	// 游戏循环
	while (1) {
		// 获取按键
		if (_kbhit())
		{
			ch = getche();

		}
		// 依据不同的按键做出对应的反应
		switch (ch)
		{
		case FS_MOVE_LEFT_KEY:
			playerAircraft.moveleft();			
			break;
		case FS_MOVE_RIGHT_KEY:
			playerAircraft.moveright();
			break;
		case FS_ESCAPE_KEY:
			cout << "You ended this game!";
			return 0;
		default:
			break;
		}
		ch = 0;
		// 更新各对象的状态
		if (playerAircraft.fireReady()) {
			mybullet = playerAircraft.fire();
		}
		if (enemy1.fireReady()) {
			enemybullet = enemy1.fire();
		}
		mybullet.updataPosition();
		enemybullet.updataPosition();
		enemy1.updatePosition();
		
		// 呈现图像
		system("cls");
		fsDraw(playerAircraft);
		fsDraw(mybullet);
		fsDraw(enemy1);
		fsDraw(enemybullet);
		fsDrawScoreBoard(scoreBoard);
		// 子弹击中的判定，如果子弹击中，则将原来的子弹设为不可见
		if (fsOverlap(mybullet, enemy1)) {
			scoreBoard.addScore();
			mybullet.setVisible(0);
		}
		if (fsOverlap(enemybullet, playerAircraft)) {
			cout << "you are hit!";
			enemybullet.setVisible(0);
			Sleep(500);
		}
		Sleep(FS_DEFAULT_FRAME_INTERVAL);
	}
	return 0; 
}