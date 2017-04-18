#include <iostream>
#include <array>
#include <chrono>
#include <vector>
#include <string>

using namespace std;

const string SHIP = "SHIP";
const string BARREL = "BARREL";
const int MY = 1;
const int EN = 0;

namespace Com {
	const string MOVE = "MOVE ";
	const string SLOWER = "SLOWER ";
	const string WAIT = "WAIT";

	const string Move(int x, int y) {
		return MOVE + to_string(x) + " " + to_string(y);
	}
	const string Slower() {
		return SLOWER;
	}
	const string Wait() {
		return WAIT;
	}

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

struct EntityShip {
	EntityShip() : EntityShip(0, 0, 0, 0, 0, 0) {}
	EntityShip(int id, int x, int y, int rot, int speed, int stock) {
		this->id = id;
		this->x = x;
		this->y = y;
		this->rot = rot;
		this->speed = speed;
		this->stock = stock;
	}
	int id;
	int x;
	int y;
	int rot;
	int speed;
	int stock;
};

struct EntityBarrel {
	EntityBarrel() : EntityBarrel(0, 0, 0, 0) {}
	EntityBarrel(int id, int x, int y, int amount) {
		this->id = id;
		this->x = x;
		this->y = y;
		this->amount = amount;
	}
	int id;
	int x;
	int y;
	int amount;
};

struct Input;
class Share {
public:


	friend Input;

private:

	static int myShipCount;
	static int enShipCount;

	static vector<EntityShip> myShip;
	static vector<EntityShip> enShip;

	static vector<EntityBarrel> barrel;

};

int Share::myShipCount = 0;
int Share::enShipCount = 0;
vector<EntityShip> Share::myShip;
vector<EntityShip> Share::enShip;
vector<EntityBarrel> Share::barrel;

struct Input {

	static void init() {

	}

	const static bool update() {

		Share::myShip.clear();
		Share::enShip.clear();
		Share::barrel.clear();

		cin >> Share::myShipCount; cin.ignore();
		int n;
		cin >> n; cin.ignore();

		for (int i = 0; i < n; i++) {
			int entityId;
			string entityType;
			int x;
			int y;
			int arg1;
			int arg2;
			int arg3;
			int arg4;
			cin >> entityId >> entityType >> x >> y >> arg1 >> arg2 >> arg3 >> arg4; cin.ignore();

			if (entityType == SHIP)
			{
				EntityShip ship(entityId, x, y, arg1, arg2, arg3);
				if (arg4 == MY)
				{
					Share::myShip.push_back(ship);
				}
				else
				{
					Share::enShip.push_back(ship);
				}
			}
			else if (entityType == BARREL)
			{
				EntityBarrel barrel(entityId, x, y, arg1);
				Share::barrel.push_back(barrel);
			}
		}

		return true;
	}

};

class AI {
public:

	const string think() {
		return Com::Move(0, 0);
	}

private:



};

int main() {

	Input::init();
	AI ai;

	Stopwatch sw;

	while (Input::update()) {

		sw.start();
		const string com = ai.think();
		sw.stop();

		cout << com << endl;

		cerr << sw.millisecond() << "ms" << endl;
		cout.flush();
		cerr.flush();
	}

	return 0;
}
