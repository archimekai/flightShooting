// fs中使用的基本类
#include <iostream>
#include <chrono>
#ifndef FSOBJECTS_H
#define FSOBJECTS_H

using namespace std;

namespace flightshooting {
#define FS_DEFAULT_FIRE_INTERVAL std::chrono::milliseconds(500)  // 默认的开火间隔
#define FS_DEFAULT_FRAME_INTERVAL 10 // 默认的刷新频率，即每隔多少毫秒刷新一次
#define FS_DEFAULT_MOVE_STEP 0.01   // 在moveleft()方法和moveright()方法中默认的每次移动距离
#define FS_MOVE_LEFT_KEY 'a'  // 控制玩家飞机向左移动一个单位所对应的按键
#define FS_MOVE_RIGHT_KEY 'd' // 控制玩家飞机向右移动一个单位所对应的按键
#define FS_ESCAPE_KEY ' '  // 退出游戏所使用的按键
#define FS_DEFAULT_BULLET_FLY_SPEED 3.0  // 默认子弹飞行速度，3代表每秒飞行三个屏幕
#define FS_DEFAULT_AIRCRAFT_FLY_SPEED 0.1  // 默认的飞机飞行速度
#define FS_DEFAULT_BULLET_COLOR fsColor(255,0,0,0)  // 默认子弹颜色
#define FS_DEFAULT_BULLET_SIZE 0.1 //默认的子弹正方形边长
#define FS_DEFAULT_BULLET_SIZE_X 0.05 //默认的子弹长方形在x轴方向的长度
#define FS_DEFAULT_BULLET_SIZE_Y 0.1 //默认的子弹长方形在y轴方向的长度
#define FS_DEFAULT_PLAYER_SYMBOL '*'  // 默认的玩家的飞机的符号
#define FS_DEFAULT_ENEMY_SYMBOL '+' // 默认的敌人的飞机的符号
#define FS_DEFAULT_BULLET_SYMBOL 'o' // 默认的子弹的符号
#define FS_DEFAULT_SCOREBOARD_COLOR fsColor(0,255,0,0)  // 默认的计分版颜色
// 默认的边界位置，这些位置主要在move*方法中限制物体不要超出屏幕边界
#define FS_DEFAULT_COORD_LOWER_LIMIT 0.05
#define FS_DEFAULT_COORD_UPPER_LIMIT 0.95
#define FS_DEFAULT_COORD_LOWER_LIMIT_X 0.05
#define FS_DEFAULT_COORD_UPPER_LIMIT_X 0.95
#define FS_DEFAULT_COORD_LOWER_LIMIT_Y 0.05
#define FS_DEFAULT_COORD_UPPER_LIMIT_Y 0.95
	// 飞机子弹来源
	enum bulletSource {
		player = 1,  // 子弹来自玩家
		enemy = 2,  // 子弹来自敌人
		playerFriend = 3  // 子弹来自玩家的友军
	};
	// 子弹射击方向
	enum Direction
	{
		up = 1, // 子弹向上飞行
		down = 2,  // 子弹向下飞行
		right = 3, // 子弹向左飞行
		left = 4 // 子弹向右飞行
	};
	class fsInvalidInitializePointValException : public std::logic_error {
	public:
		fsInvalidInitializePointValException() :std::logic_error("Invalid initialze fsPoints!") {};
	};

	class fsOutRangeMoveToPointException : public std::logic_error {
	public:
		fsOutRangeMoveToPointException() : std::logic_error("the move to point is out of the range of 0 to 1 for either x or y") {};
	};

	class fsInvalidFireTimeException :public std::logic_error {
	public:
		fsInvalidFireTimeException() :std::logic_error("Not right time to fire!") {};
	};

	class fsInvalidBulletSpeedInException : public std::logic_error {
	public:
		fsInvalidBulletSpeedInException(): std::logic_error("bullet speed in must be a positive number!") {};
	};

	template <typename T>
	class fsPoint {
	public:
		T x, y; // 归一化之后的坐标位置，左上角为0,0  右下角为1,1
		fsPoint() {};
		fsPoint(T xval, T yval) :x(xval), y(yval) {};
		fsPoint<T> & operator = (const fsPoint<T> & rval){
			this->x = rval.x;
			this->y = rval.y;
			return *this;
		}
	};

	class fsColor {
	public:
		unsigned char r, g, b, a;  //全部为0~255  0 表示完全不透明
		fsColor() {};
		fsColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) :
			r(red), g(green), b(blue), a(alpha) {};
		
	};

	// 基类
	class fsObject {
	private:
		fsPoint<double> upperLeftCorner;  // the position 
		fsPoint<double> lowerRightCorner;
		fsColor			foregroundColor;
		char			symbol;
		int				visible;  // 物体是否可见，如果为0则不可见
		//预留纹理位置
	public:
		fsObject() {};
		fsObject(const fsPoint<double> & upperleft, // 指定物体左上角的位置
			const fsPoint<double> & lowerright, // 指定物体右下角的位置，这两个点确定后，一个物体的位置和大小也就完全确定了
			const fsColor & fgc,  // 指定物体的显示颜色，如果使用控制台进行输出，这一参数将会被忽略
			char sym = 0  // 指定物体的符号，主要用于在控制台中显示该物体。
			)
			{ 
				if(upperleft.x>lowerright.x||upperleft.y>lowerright.y)
					throw fsInvalidInitializePointValException();
				this->upperLeftCorner	= upperleft;
				this->lowerRightCorner	= lowerright;
				this->foregroundColor	= fgc;
				this->symbol			= sym;
				this->visible			= 1;
		}

		// 取得或设置物体左上角的位置
		fsPoint<double> getUpperLeftCorner() const { return this->upperLeftCorner; }
		//void setUpperLeftCorner(const fsPoint<double> & val) { this->upperLeftCorner = val };
		// 取得或物体右下角的位置
		fsPoint<double> getLowerRightCorner() const { return this->lowerRightCorner; }
		//void setUpperLeftCorner(const fsPoint<double> & val) { this->upperLeftCorner = val };
		// 取得该物体的符号
		char getSymbol() const { return this->symbol; }
		// 为物体设置新的符号
		void setSymbol(char c) { this->symbol = c; }
		// 得到或设置物体的可见性，返回0为不可见，返回1为完全可见，如果物体不可见，其也就不能击中飞机
		int getVisible() const { return this->visible; }
		void setVisible(int newVisible) { this->visible = newVisible; return; }


		// 移动位置，向左移动一个单位（步长为FS_DEFAULT_MOVE_STEP） 移动成功则返回1 移动失败返回0
		int moveleft() {
			if (this->upperLeftCorner.x - FS_DEFAULT_MOVE_STEP < 0)
				return 0;
			this->upperLeftCorner.x = this->upperLeftCorner.x - FS_DEFAULT_MOVE_STEP;
			this->lowerRightCorner.x = this->lowerRightCorner.x - FS_DEFAULT_MOVE_STEP;
			return 1;
		}
		// 移动位置，向右移动一个单位（步长为FS_DEFAULT_MOVE_STEP） 移动成功则返回1 移动失败返回0
		int moveright() {
			if (this->lowerRightCorner.x + FS_DEFAULT_MOVE_STEP > 1)
				return 0;
			this->upperLeftCorner.x = this->upperLeftCorner.x + FS_DEFAULT_MOVE_STEP;
			this->lowerRightCorner.x = this->lowerRightCorner.x + FS_DEFAULT_MOVE_STEP;
			return 1;
		}
		//在竖直方向上移动位置，如果输入的delta导致物体的左上角超出屏幕，就将其置为最近的FS_DEFAULT_COORD_UPPER_LIMIT_Y或FS_DEFAULT_COORD_UPPER_LIMIT_Y
		int moveVertically(double delta) {
			if (this->upperLeftCorner.y + delta > FS_DEFAULT_COORD_UPPER_LIMIT_Y) {
				this->upperLeftCorner.y = FS_DEFAULT_COORD_UPPER_LIMIT_Y;
				return 0;
			}
			if (this->upperLeftCorner.y + delta < FS_DEFAULT_COORD_LOWER_LIMIT_Y) {
				this->upperLeftCorner.y = FS_DEFAULT_COORD_LOWER_LIMIT_Y;
				return 0;
			}
			this->upperLeftCorner.y = this->upperLeftCorner.y + delta;
			this->lowerRightCorner.y = this->lowerRightCorner.y + delta;
			return 1;
		}
		//在水平方向上移动位置，如果输入的delta导致物体的左上角超出屏幕，就将其置为最近的FS_DEFAULT_COORD_UPPER_LIMIT_X或FS_DEFAULT_COORD_UPPER_LIMIT_X
		int moveHorizontally(double delta) {
			if (this->upperLeftCorner.x + delta > FS_DEFAULT_COORD_UPPER_LIMIT_X) {
				this->upperLeftCorner.x = FS_DEFAULT_COORD_UPPER_LIMIT_X;
				return 0;
			}
			if (this->upperLeftCorner.x + delta < FS_DEFAULT_COORD_LOWER_LIMIT_X) {
				this->upperLeftCorner.x = FS_DEFAULT_COORD_LOWER_LIMIT_X;
				return 0;
			}
			this->upperLeftCorner.x = this->upperLeftCorner.x + delta;
			this->lowerRightCorner.x = this->lowerRightCorner.x + delta;
			return 1;
		}
		// 直接将物体移到左上角与某一点重合的位置，成功则返回1，失败返回0
		int moveToPoint(const fsPoint<double> & dest) {
			if (dest.x > 1 || dest.x < 0 || dest.y >1 || dest.y < 0) {
				throw fsOutRangeMoveToPointException();
			}
			this->lowerRightCorner.x = this->lowerRightCorner.x + dest.x - this->upperLeftCorner.x;
			this->lowerRightCorner.y = this->lowerRightCorner.x + dest.y - this->upperLeftCorner.y;
			this->upperLeftCorner = dest;
		}
		
		// 打印出物体左上角的位置
		void printpos() const {
			cout << "ULC:" << (this->getLowerRightCorner()).x << "  " << (this->getLowerRightCorner()).y << endl;
			return;
		}

	};

	// 子弹
	class fsBullet : public fsObject {
	private:
		bulletSource source;
		Direction bulletDirection;  // 飞行方向
		double vec; // 飞行速度，目前暂时不允许子弹斜着飞 并且不允许是负数
		int bulletDamage;
		std::chrono::high_resolution_clock::time_point lastPositionUpdateTime;  // 最后一次更新物体位置的时间
	public:
		fsBullet() {};
		fsBullet(bulletSource sourceOfBullet,  // 子弹的来源
			Direction directionOfBullet,  // 子弹的飞行方向
			int damageOfBullet, // 子弹的威力，也即子弹击中后玩家获得多少分
			fsPoint<double> sourceulc, // 发出子弹物体的左上角坐标，通过该坐标可以确定子弹的初始位置
			double bulletVec = FS_DEFAULT_BULLET_FLY_SPEED  // 子弹的飞行速度
			) :
			fsObject(sourceulc, fsPoint<double>(sourceulc.x + FS_DEFAULT_BULLET_SIZE_X, sourceulc.y + FS_DEFAULT_BULLET_SIZE_Y), FS_DEFAULT_BULLET_COLOR),
			source(sourceOfBullet),
			bulletDirection(directionOfBullet), bulletDamage(damageOfBullet),
			lastPositionUpdateTime(std::chrono::high_resolution_clock::now()) {
			if (bulletVec < 0)
				throw fsInvalidBulletSpeedInException();
			this->vec = bulletVec;
			if (this->bulletDirection == up)
				vec = vec * -1;
			this->setSymbol(FS_DEFAULT_BULLET_SYMBOL);
		};
		~fsBullet() {
			//this->moveToPoint(fsPoint<double>(-0.5, -0.5));
		};

		// 更新子弹的位置，每次循环中都要执行此函数以更新子弹的位置，如果返回0，则表示子弹已经飞行到边缘。
		int updataPosition() {
			std::chrono::microseconds timeelapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - this->lastPositionUpdateTime);
			double delta = 1.0 * timeelapsed.count() / 1000000 * vec;
			//cout << "delta" << delta << endl;
			this->lastPositionUpdateTime = std::chrono::high_resolution_clock::now();
			if (this->moveVertically(delta) == 0)
				return 0;
			return 1;
		}
	};

	// 游戏中所有飞机的基类，玩家的飞机和敌人的飞机均由此派生
	class fsAircraft : public fsObject {
	private:
		chrono::milliseconds fireInterval; // 两发炮弹之间的间隔，单位为ms
		chrono::steady_clock::time_point lastFireTime;
		int lifeLeft = 1;  // 剩余的生命值
		int bulletDamage = 1;  // 默认的机体伤害值
		bulletSource source;
		Direction bullteDirection;
	public:
		fsAircraft() {};
		fsAircraft(fsPoint<double> upperleft,  // 飞机左上角的位置
			fsPoint<double> lowerright,   // 飞机右下角的位置
			fsColor fgc,  // 飞机的显示颜色，在控制台中此参数无效
			std::chrono::milliseconds fireinterval = FS_DEFAULT_FIRE_INTERVAL  // 飞机的子弹发射间隔，在经过此段时间后飞机才能发射子弹
			) :
			fsObject(upperleft, lowerright, fgc),
			fireInterval(fireinterval) {
			this->lastFireTime = chrono::high_resolution_clock::now();
		};
		// 发射子弹的相关逻辑，判断能否发射子弹，也即自上一次发射子弹之后，是否经历了足够的时间
		// 如果可以发射子弹则返回1，不能则返回0
		inline int fireReady() {
			if (chrono::high_resolution_clock::now() - this->lastFireTime > fireInterval)
			{
				return 1;
			}
			return 0;			
		}
		// 强制发射子弹，返回fsBullet的对象，即该飞机所发射的子弹
		fsBullet fire() {
			//if (fireReady()) {
				this->lastFireTime = chrono::high_resolution_clock::now();
				return fsBullet(this->source, this->bullteDirection, this->bulletDamage, this->getUpperLeftCorner());
			//}
			//throw fsInvalidFireTimeException();
		}

		// 设置新的飞机子弹伤害
		int setBullteDamage(int newBulletDamage) {
			this->bulletDamage = newBulletDamage;
			return 0;
		}
		// 设置飞机是玩家的飞机还是敌人的飞机
		void setSource(bulletSource newSource) { this->source = newSource; }
		// 设置新的子弹飞行方向
		void setFireDirection(Direction newDirection) { this->bullteDirection = newDirection; }

		//返回剩余生命值，当返回0或者负数时说明死亡（本程序中没有使用）
		int getLifeLeft() const{
			return this->lifeLeft;
		}

		//被打中了，返回剩余生命值  当返回0或者负数时说明死亡  子弹威力为负数说明为生命补给包（本程序中没有使用）
		int gotHit(int bulletDamage = 1) {
			if (this->lifeLeft - bulletDamage <= 0) {
				this->lifeLeft = this->lifeLeft - bulletDamage;
				return 0;
			}			
			this->lifeLeft = this->lifeLeft - bulletDamage;
		}
	};
	// 玩家的飞机，玩家可以通过键盘操纵其位置
	class fsMyAircraft : public fsAircraft {
	private:

	public:
		fsMyAircraft(fsPoint<double> upperleft,  // 飞机左上角的位置
			fsPoint<double> lowerright,   // 飞机右下角的位置
			fsColor fgc,   // 飞机的显示颜色，在控制台中此参数无效
			std::chrono::milliseconds  fireinterval = FS_DEFAULT_FIRE_INTERVAL  // 飞机的子弹发射间隔，发射子弹后，再经过此段时间后才能再次发射子弹
			) :
			fsAircraft(upperleft, lowerright, fgc, fireinterval) {
			this->setSource(player);
			this->setFireDirection(up);
			this->setSymbol(FS_DEFAULT_PLAYER_SYMBOL);
		};

	};

	// 敌人的飞机
	class fsEnemyAircraft : public fsAircraft {
	private:
		std::chrono::high_resolution_clock::time_point lastPositionUpdateTime;  // 最后一次更新物体位置的时间
		std::chrono::high_resolution_clock::time_point lastDirectionUpdateTime;  // 最后一次更新运动方向的时间
		double vec; // 飞行速度，指在水平方向上的飞行速度
		std::chrono::milliseconds directionUpdateInterval;  // 两次速度方向更新之间的时间间隔，最好能用随机数产生
	public:
		fsEnemyAircraft(fsPoint<double> upperleft, // 飞机左上角的位置
			fsPoint<double> lowerright,  // 飞机右下角的位置
			fsColor fgc,  // 飞机的显示颜色，在控制台中此参数无效
			std::chrono::milliseconds  fireinterval = FS_DEFAULT_FIRE_INTERVAL  // 飞机的子弹发射间隔，在经过此段时间后飞机才能发射子弹
			):
			fsAircraft(upperleft, lowerright, fgc, fireinterval){
			this->setSource(enemy);
			this->setFireDirection(down);
			this->setSymbol(FS_DEFAULT_ENEMY_SYMBOL);
			this->lastPositionUpdateTime = std::chrono::high_resolution_clock::now();
			this->lastDirectionUpdateTime = std::chrono::high_resolution_clock::now();
			this->directionUpdateInterval = std::chrono::milliseconds(1000);
			this->vec = FS_DEFAULT_AIRCRAFT_FLY_SPEED;
		}
		
		// 更新敌机的位置，敌机随机移动，每隔一段时间更换一个方向，移动会在一个矩形框中进行
		int updatePosition() {
			// 是否要更换方向
			if (std::chrono::high_resolution_clock::now() - lastDirectionUpdateTime > directionUpdateInterval) {
				//产生一个新的方向
				lastDirectionUpdateTime = std::chrono::high_resolution_clock::now();
				this->vec = this->vec * (-1);
				srand((unsigned)time(NULL));
				directionUpdateInterval = std::chrono::milliseconds(rand() / 5 + 1000);  // 随机数的范围在1s到7s之间
			}
			std::chrono::milliseconds elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastPositionUpdateTime);
			lastPositionUpdateTime = std::chrono::high_resolution_clock::now();
			this->moveHorizontally(1.0 * elapsedtime.count() / 1000 * vec);
			return 0;
		}
	};

	// 计分版 用于记录玩家的分数
	class fsScoreBoard : public fsObject {
	private:
		int score = 0;  // 每次击中算一分

	public:
		fsScoreBoard() {};
		fsScoreBoard(const fsPoint<double> & upperleft, // 计分版左上角的位置 推荐将左上角的横坐标设置为大于1的数，避免分数的显示与游戏画面重叠
			const fsPoint<double> & lowerright,  // 计分版右下角的位置
			const fsColor & fgc = FS_DEFAULT_SCOREBOARD_COLOR  // 计分版颜色，在控制台情况下无效
			) :fsObject(upperleft, lowerright, fgc) {
			;
		}

		// 获取计分版分数
		int getCurScore() const {
			return this->score;
		}
		// 增加得分
		void addScore(int delta = 1) {
			this->score = this->score + 1;
		}
	};
	

	
	// 判断两个物体是否重叠 如果重叠将返回1 否则返回0，主要用来判定子弹是否击中了飞机
	int fsOverlap(const fsObject & obj1, const fsObject & obj2);

	// 判断一个点是否在一个矩形中，包括在矩形边上的情况
	template <typename T> 	
	bool fsPointInRect(fsPoint<T> point, fsPoint<T> RectUpperLeft, fsPoint<T> RectLowerRight);

	int fsInitialize();
	//int fsGameLoop();
}
#endif // !FSOBJECT_H

