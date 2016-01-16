// 绘图类及绘图方法
#include "fsObjects.h"
#include <Windows.h>
#include <conio.h>
#ifndef FSDRAW_H
#define FSDRAW_H

using namespace flightshooting;
using namespace std;
namespace flightshooting {
#define FS_DEFAULT_CONSOLE_BUFFER_SCALE 50
	//设置控制台光标位置（x,y）
	void gotoxy(int x, int y) {
		COORD pos;
		pos.X = x;
		pos.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	}
	void gotoxy(int x, int y, HANDLE consoleBuf) {
		COORD pos;
		pos.X = x;
		pos.Y = y;
		SetConsoleCursorPosition(consoleBuf, pos);
	}

	// 设置控制台颜色
	void setcolor(WORD color) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}
	void setcolor(WORD color, HANDLE consoleBuf) {
		SetConsoleTextAttribute(consoleBuf, color);
	}

	// 画出对象，对象的位置由其左上角位置决定，对象的符号由getSymbol()方法返回的字符决定，可以通过
	// setSymbol()方法改变这一符号
	int fsDraw(const fsObject & fso) {
			if (fso.getVisible() == 0) return 0;
			gotoxy((fso.getUpperLeftCorner()).x * FS_DEFAULT_CONSOLE_BUFFER_SCALE,
				(fso.getUpperLeftCorner()).y * FS_DEFAULT_CONSOLE_BUFFER_SCALE);
			setcolor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			cout << fso.getSymbol();
			return 0;
	}
	int fsDraw(const fsObject & fso, HANDLE consoleBuf) {
		if (fso.getVisible() == 0) return 0;
		gotoxy((fso.getUpperLeftCorner()).x * FS_DEFAULT_CONSOLE_BUFFER_SCALE,
			(fso.getUpperLeftCorner()).y * FS_DEFAULT_CONSOLE_BUFFER_SCALE, consoleBuf);
		setcolor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED, consoleBuf);
		cout << fso.getSymbol();
		return 0;
	}

	// 画出计分版并显示分数
	int fsDrawScoreBoard(const fsScoreBoard & fsb) {
			if (fsb.getVisible() == 0) return 0;
			gotoxy((fsb.getUpperLeftCorner()).x * FS_DEFAULT_CONSOLE_BUFFER_SCALE,
				(fsb.getUpperLeftCorner()).y * FS_DEFAULT_CONSOLE_BUFFER_SCALE);
			setcolor(FOREGROUND_GREEN);
			cout << "SCORE: " << fsb.getCurScore();
			return 0;
	}
	int fsDrawScoreBoard(const fsScoreBoard & fsb, HANDLE consoleBuf) {
		if (fsb.getVisible() == 0) return 0;
		gotoxy((fsb.getUpperLeftCorner()).x * FS_DEFAULT_CONSOLE_BUFFER_SCALE,
			(fsb.getUpperLeftCorner()).y * FS_DEFAULT_CONSOLE_BUFFER_SCALE, consoleBuf);
		setcolor(FOREGROUND_GREEN, consoleBuf);
		cout << "SCORE: " << fsb.getCurScore();
		return 0;
	}

	// 用空格填充控制台的缓存
	int fsFillWithBlank() {
		gotoxy(0, 0);
		for (int i = 0; i < 1000 * 1000; ++i) {
			cout << " ";
		}
		return 0;
	}

}




#endif // !FSDRAW_H

