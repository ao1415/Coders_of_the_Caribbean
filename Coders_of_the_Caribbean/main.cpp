#include <iostream>
#include <array>
#include <chrono>
#include <vector>
#include <string>
#include <random>
#include <queue>

using namespace std;

const string SHIP = "SHIP";
const string BARREL = "BARREL";
const string CANNON = "CANNONBALL";
const string MINE = "MINE";

const int MY = 1;
const int EN = 0;

const int Width = 22;
const int Height = 21;

using Stage = array<array<int, Width>, Height>;

namespace Com {
	const string MOVE = "MOVE ";
	const string SLOWER = "SLOWER ";
	const string WAIT = "WAIT";
	const string FIRE = "FIRE ";
	const string MINE = "MINE ";
};

class Stopwatch {
public:

	inline void start() {
		s = std::chrono::high_resolution_clock::now();
		e = s;
	}
	inline void stop() {
		e = std::chrono::high_resolution_clock::now();
	}

	inline const long long nanoseconds() const { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
	inline const long long microseconds() const { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
	inline const long long millisecond() const { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }
	inline const long long second() const { return std::chrono::duration_cast<std::chrono::seconds>(e - s).count(); }
	inline const long long minutes() const { return std::chrono::duration_cast<std::chrono::minutes>(e - s).count(); }
	inline const long long hours() const { return std::chrono::duration_cast<std::chrono::hours>(e - s).count(); }

private:

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	std::chrono::time_point<std::chrono::high_resolution_clock> e;

};

class Timer {
public:

	Timer() = default;
	Timer(const std::chrono::nanoseconds& _time) { type = Type::nanoseconds; time = _time.count(); }
	Timer(const std::chrono::microseconds& _time) { type = Type::microseconds; time = _time.count(); }
	Timer(const std::chrono::milliseconds& _time) { type = Type::milliseconds; time = _time.count(); }
	Timer(const std::chrono::seconds& _time) { type = Type::seconds; time = _time.count(); }
	Timer(const std::chrono::minutes& _time) { type = Type::minutes; time = _time.count(); }
	Timer(const std::chrono::hours& _time) { type = Type::hours; time = _time.count(); }

	void set(const std::chrono::nanoseconds& _time) { type = Type::nanoseconds; time = _time.count(); }
	void set(const std::chrono::microseconds& _time) { type = Type::microseconds; time = _time.count(); }
	void set(const std::chrono::milliseconds& _time) { type = Type::milliseconds; time = _time.count(); }
	void set(const std::chrono::seconds& _time) { type = Type::seconds; time = _time.count(); }
	void set(const std::chrono::minutes& _time) { type = Type::minutes; time = _time.count(); }
	void set(const std::chrono::hours& _time) { type = Type::hours; time = _time.count(); }

	void start() { s = std::chrono::high_resolution_clock::now(); }

	inline const bool check() const {
		const auto e = std::chrono::high_resolution_clock::now();
		switch (type)
		{
		case Type::nanoseconds: return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() >= time;
		case Type::microseconds: return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count() >= time;
		case Type::milliseconds: return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() >= time;
		case Type::seconds: return std::chrono::duration_cast<std::chrono::seconds>(e - s).count() >= time;
		case Type::minutes: return std::chrono::duration_cast<std::chrono::minutes>(e - s).count() >= time;
		case Type::hours: return std::chrono::duration_cast<std::chrono::hours>(e - s).count() >= time;
		default: return true;
		}
	}

	operator bool() const { return check(); }

private:

	enum class Type {
		nanoseconds,
		microseconds,
		milliseconds,
		seconds,
		minutes,
		hours
	};

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	long long time;
	Type type;

};

struct Point {

	Point() : Point(0, 0) {}
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}

	int x;
	int y;

	const bool operator==(const Point& o) const { return (x == o.x && y == o.y); }

	const Point operator+(const Point& o) const { return Point(x + o.x, y + o.y); }
	const Point operator-(const Point& o) const { return Point(x - o.x, y - o.y); }

	const bool inside() const { return (0 <= x && x < Width && 0 <= y && y < Height); }

	const string toString() const { return to_string(x) + " " + to_string(y); }

};

struct Entity {
	int id;
	string type;
	Point pos;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
};

struct EntityShip {

	EntityShip() : EntityShip(0, 0, 0, 0, 0, 0) {}
	EntityShip(int id, int x, int y, int rot, int speed, int stock) {
		this->id = id;
		pos.x = x;
		pos.y = y;
		this->rot = rot;
		this->speed = speed;
		this->stock = stock;
	}
	int id;
	Point pos;
	int rot;
	int speed;
	int stock;
};

struct EntityBarrel {

	EntityBarrel() : EntityBarrel(0, 0, 0, 0) {}
	EntityBarrel(int id, int x, int y, int amount) {
		this->id = id;
		pos.x = x;
		pos.y = y;
		this->amount = amount;
	}
	int id;
	Point pos;
	int amount;
};

struct EntityCannon {

	EntityCannon() : EntityCannon(0, 0, 0, 0, 0) {}
	EntityCannon(int id, int x, int y, int attacker, int time) {
		this->id = id;
		pos.x = x;
		pos.y = y;
		this->attacker = attacker;
		this->time = time;
	}

	int id;
	Point pos;
	int attacker;
	int time;
};

struct EntityMine {

	EntityMine() : EntityMine(0, 0, 0) {}
	EntityMine(int id, int x, int y) {
		this->id = id;
		pos.x = x;
		pos.y = y;
	}

	int id;
	Point pos;
};

struct Input;
class Share {
public:

	const static int getMyShipCount() { return myShipCount; }
	const static int getEnShipCount() { return enShipCount; }

	const static vector<EntityShip>& getMyShip() { return myShip; }
	const static vector<EntityShip>& getEnShip() { return enShip; }

	const static vector<EntityBarrel>& getBarrel() { return barrel; }

	const static vector<EntityCannon>& getCannon() { return cannon; }

	const static vector<EntityMine>& getMine() { return mine; }
	const static Stage& getMineStage() { return mineStage; }

	friend Input;

private:

	static int myShipCount;
	static int enShipCount;

	static vector<EntityShip> myShip;
	static vector<EntityShip> enShip;

	static vector<EntityBarrel> barrel;

	static vector<EntityCannon> cannon;

	static vector<EntityMine> mine;
	static Stage mineStage;

};

int Share::myShipCount = 0;
int Share::enShipCount = 0;
vector<EntityShip> Share::myShip;
vector<EntityShip> Share::enShip;
vector<EntityBarrel> Share::barrel;
vector<EntityCannon> Share::cannon;
vector<EntityMine> Share::mine;
Stage Share::mineStage;

struct Input {

	static void init() {

	}

	const static bool update() {

		Share::myShip.clear();
		Share::enShip.clear();
		Share::barrel.clear();
		Share::cannon.clear();

		//地雷データはそのうち継続して使いたい(視界内の地雷のみを更新したい)
		Share::mine.clear();
		for (auto& v : Share::mineStage) v.fill(0);

		cin >> Share::myShipCount; cin.ignore();
		int n;
		cin >> n; cin.ignore();

		for (int i = 0; i < n; i++) {
			Entity entity;
			cin >> entity.id >> entity.type >> entity.pos.x >> entity.pos.y >> entity.arg1 >> entity.arg2 >> entity.arg3 >> entity.arg4; cin.ignore();

			if (entity.type == SHIP)
			{
				EntityShip ship(entity.id, entity.pos.x, entity.pos.y, entity.arg1, entity.arg2, entity.arg3);
				if (entity.arg4 == MY)
				{
					Share::myShip.push_back(ship);
				}
				else
				{
					Share::enShip.push_back(ship);
				}
			}
			else if (entity.type == BARREL)
			{
				EntityBarrel barrel(entity.id, entity.pos.x, entity.pos.y, entity.arg1);
				Share::barrel.push_back(barrel);
			}
			else if (entity.type == CANNON)
			{
				EntityCannon cannon(entity.id, entity.pos.x, entity.pos.y, entity.arg1, entity.arg2);
				Share::cannon.push_back(cannon);
			}
			else if (entity.type == MINE)
			{
				EntityMine mine(entity.id, entity.pos.x, entity.pos.y);
				Share::mine.push_back(mine);
				Share::mineStage[mine.pos.y][mine.pos.x] = 1;
			}
		}

		return true;
	}

};

inline std::ostream& operator << (ostream& os, const Point& p) { return os << "(" << p.x << ", " << p.y << ")"; }

const int range(const Point& p1, const Point& p2) {
	const int z1 = p1.x + (p1.y + 1) / 2;
	const int z2 = p2.x + (p2.y + 1) / 2;
	const int dx = abs(p1.x - p2.x);
	const int dy = abs(p1.y - p2.y);
	const int dz = abs(z1 - z2);
	return max(dx, max(dy, dz));
}

inline const string Move(int x, int y) {
	return Com::MOVE + to_string(x) + " " + to_string(y);
}
inline const string Move(const Point& p) {
	return Move(p.x, p.y);
}
inline const string Fire(int x, int y) {
	return Com::FIRE + to_string(x) + " " + to_string(y);
}
inline const string Fire(const Point& p) {
	return Fire(p.x, p.y);
}
inline const string Slower() {
	return Com::SLOWER;
}
inline const string Wait() {
	return Com::WAIT;
}

class AI {
public:

	AI() {

	}

	const vector<string> think() {

		const int Turn = 5;
		const int ChokudaiWidth = 1;

		array<priority_queue<Data>, Turn + 1> qData;
		{
			Data now;
			now.myShip = Share::getMyShip();
			now.enShip = Share::getMyShip();
			now.barrel = Share::getBarrel();
			now.cannon = Share::getCannon();
			now.mine = Share::getMine();
			now.mineStage = Share::getMineStage();
			qData[0].emplace(now);
		}

		Timer timer(chrono::milliseconds(20));
		timer.start();

		const Point dp[2][6] = { { Point(-1,-1),Point(0,-1),Point(-1,0),Point(1,0),Point(-1,1),Point(0,1) },
		{ Point(0,-1),Point(1,-1),Point(-1,0),Point(1,0),Point(0,-1),Point(1,1) } };

		const int ShipCount = Share::getMyShipCount();

		array<vector<Command>, Turn> coms;
		for (auto& v : coms) v.resize(ShipCount);

		while (!timer)
		{
			for (int id = 0; id < ShipCount; id++)
			{
				qData.swap(array<priority_queue<Data>, Turn + 1>());

				for (int t = 0; t < Turn; t++)
				{
					const Data& data = qData[t].top();
					{
						const int line = data.myShip[id].pos.y % 2;
						for (int d = 0; d < 6; d++)
						{
							const Point dist = data.myShip[id].pos + dp[line][d];
							if (dist.inside())
							{
								//このターンの決まっている船のコマンド
								vector<Command> c = coms[t];
								//この船のコマンドを設定
								c[id] = Command(0, dist);

								const auto nextData = next(data, c, c[id]);
								qData[t + 1].emplace(nextData);
							}
						}
					}
				}

				//この船のいい感じの移動パターンを取り出す
				const auto& top = qData[Turn].top();
				for (int t = 0; t < Turn; t++)
				{
					coms[id][t] = top.command[t];
				}
			}
		}

		vector<string> stringComs;
		for (const auto& c : coms[0])
		{
			stringComs.push_back(c.toString());
		}

		return stringComs;
	}

private:

	struct Command {

		Command() {

		}
		Command(int c) {
			com = c;
		}
		Command(int c, const Point& p) {
			com = c;
			pos = p;
		}

		int com = 4;
		Point pos;

		const string toString() const {
			string str;
			switch (com)
			{
			case 0: str = Com::MOVE + pos.toString(); break;
			case 1: str = Com::FIRE + pos.toString(); break;
			case 2: str = Com::MINE; break;
			case 3: str = Com::SLOWER; break;
			case 4: str = Com::WAIT; break;
			}
			return str;
		}

	};

	struct Data {
		vector<EntityShip> myShip;
		vector<EntityShip> enShip;
		vector<EntityBarrel> barrel;
		vector<EntityCannon> cannon;
		vector<EntityMine> mine;
		Stage mineStage;
		vector<Command> command;
		int score = 0;

		const bool operator<(const Data& d) const {
			return score < d.score;
		}

	};

	const Data next(Data data, const vector<Command>& coms, const Command& com) const {

		const auto& enShip = data.enShip;
		auto& myShip = data.myShip;
		const int ShipCount = myShip.size();
		auto& barrel = data.barrel;
		auto& cannon = data.cannon;
		auto& mine = data.mine;
		vector<Point> deleteMinePoint;

		const Point dp[2][6] = { { Point(1,0),Point(0,-1),Point(-1,-1),Point(-1,0),Point(-1,1),Point(0,1) },
		{ Point(1,0),Point(1,-1),Point(0,-1),Point(-1,0),Point(0,-1),Point(1,1) } };

		for (int id = 0; id < ShipCount; id++)
		{
			switch (coms[id].com)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				const Point np = myShip[id].pos + dp[myShip[id].pos.y % 2][myShip[id].rot];
				break;
			}
		}

		for (const auto& p : deleteMinePoint)
			data.mineStage[p.y][p.x] = 0;

		data.command.push_back(com);
		data.score = eval(data);
		return data;
	}

	/*
	キャノンは打った瞬間に当たる
	それぞれの船が独立して動く(1つの船のみで、数ターン先をみる)
	それぞれの行動が決まったら、もう一度各船でシミュレートする(他の船は事前に決まった行動をする)

	行動パターン
	MOVE 周囲6方向
	FIRE
	SLOWER

	MINE
	*/
	const int eval(const Data& data) const {

		return 0;
	}

};

int main() {

	Input::init();
	AI ai;

	Stopwatch sw;

	while (Input::update()) {

		sw.start();
		const auto& coms = ai.think();
		sw.stop();

		for (const auto& com : coms)
		{
			if (com == "")
				cout << Wait() << endl;
			else
				cout << com << endl;
		}

		cerr << sw.millisecond() << "ms" << endl;
		cout.flush();
		cerr.flush();
	}

	return 0;
}
