#include <mutex>
#include "../../lib/include/Process.h"

class Elem;

class Processor : public Process {
public:
	std::mutex mtx;
	Elem* data = nullptr;
	size_t count;

	Processor();
	~Processor();

	bool process() override;
	bool isReady();
	bool putData(Elem* dataPointer, size_t dataCount);
	std::string getProcessName() override;
};