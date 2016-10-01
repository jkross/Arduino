#pragma once
typedef unsigned long ticks_t;

class snapshotTime {
private:
	ticks_t _now;

public:
	snapshotTime() {
		set(0);
	}

	void set(ticks_t now) {
		_now = now;
	}

	ticks_t get() {
		return _now;
	}
};