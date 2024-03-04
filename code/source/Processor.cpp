#include "../include/Processor.h"
#include "../include/element.h"

//uint32_t Processor::calcStage = 0;

Processor::Processor() {
	count = 0;
	start();
}

Processor::~Processor() {
	stop();
}

bool Processor::process() {
	//if (calcStage == 1) {
	//	//std::lock_guard<std::mutex> mtxLock(mtx);
	//	if (data) {
	//		for (size_t i = 0; i < count; i++) {
	//			data[i].calcStep1();
	//		}
	//	}
	//	data = nullptr;
	//}
	//if (calcStage == 2) {
	//	//std::lock_guard<std::mutex> mtxLock(mtx);
	//	if (data) {
	//		for (size_t i = 0; i < count; i++) {
	//			data[i].calcStep2();
	//		}
	//	}
	//	data = nullptr;
	//}
	{
		std::lock_guard<std::mutex> mtxLock(mtx);
		if (data) {
			if (calcStage == 1) {
				for (size_t i = 0; i < count; i++) {
					data[i].calcStep1();
				}
			}
			if (calcStage == 2) {
				for (size_t i = 0; i < count; i++) {
					data[i].calcStep2();
				}
			}
			data = nullptr;
		}
	}
	return true;
}

bool Processor::isReady() {
	std::lock_guard<std::mutex> mtxLock(mtx);
	return data == nullptr;
}

bool Processor::putData(Elem* dataPointer, size_t dataCount) {
	{
		std::lock_guard<std::mutex> mtxLock(mtx);
		if (data != nullptr) return false;
		data = dataPointer;
		count = dataCount;
	}
	return true;
}

std::string Processor::getProcessName() {
	return "mesh process";
}