#ifndef Async_h
#define Async_h

struct ScheduleNode {
	unsigned long lastExecution;
	unsigned long interval;
	bool isLoop;
	bool finished;
	void (*function)(void);
};

class Async {
	private:
		ScheduleNode* nodePool;
		unsigned int nNodes;
		unsigned int sizePool;

	public:
		Async(unsigned int sizePool);
		~Async();
		int setTimeout(void (*fun)(void) = nullptr, unsigned long time = 0);
		int setInterval(void (*fun)(void) = nullptr, unsigned long time = 0);
		bool clearInterval(int id = -1);
		void run();
};

#endif
